/*
 * Copyright (c) 2011 ITE Tech. Inc. All Rights Reserved.
 */
/** @file
 * PAL Remote IR functions.
 *
 * @author Jim Tan
 * @version 1.0
 */
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "openrtos/FreeRTOS.h"
#include "openrtos/queue.h"
#include "itp_cfg.h"


//#define ENABLE_IR_INTR
//#define ENABLE_DBG_COMPARE_CODE

//#define ENABLE_IR_DMA

#ifdef ENABLE_IR_DMA
typedef struct LLP_CONTEXT_TAG
{
    uint32_t  SrcAddr;
    uint32_t  DstAddr;
    uint32_t  LLP;
    uint32_t  Control;
    uint32_t  TotalSize;
}LLP_CONTEXT;

#define IR_DMA_BUFFER_SIZE      1024
#define IR_DMA_TRANSFER_SIZE    IR_DMA_BUFFER_SIZE/2 //1:8bit 2:16bit 4:32bit

char *RxChName = NULL;
char *TxChName = NULL;
uint16_t *RxBuffer = NULL;
uint16_t *TxBuffer = NULL;
static int RxChannel = 0;
static int TxChannel = 0;
LLP_CONTEXT *gLLPCtxt = NULL;

static int RxWriteIdx = 0;
static int RxReadIdx = 0;
static int irWidth = 0;

#endif


#define QUEUE_LEN 256

static const uint32_t irTypeTable[] =
{
#include "ir_type.inc"
};

#if 1
static const uint32_t irValidTxCodeTable[] =
{
    0x55,0xAA
};
#else
static const uint32_t irValidTxCodeTable[] =
{
#include "ir_TxTable.inc"
};
#endif

#define STATE_NUM               (5)
#define MAX_VAL                 ((1<<10)-1)

#define PRECISION               16
#define SAMP_DUR                10    // sample duration in microseconds

#define REPEAT_THRESHOLD_BEGIN  (5)     // accumulate number of repeat-key will start dispatch key
#define REPEAT_THRESHOLD_SPDUP  (2)     // accumulate number of key will change to high speed mode
#define REPEAT_THRESHOLD_HOLD1  (5)     // dispatch a key foreach number of repeat-key at low speed mode
#define REPEAT_THRESHOLD_HOLD2  (1)     // dispatch a key foreach number of repeat-key at high speed mode

enum RCState {
    WAIT_START,
    WAIT_START_REPEAT_KEY,      // hiden state for repeat key
    WAIT_BIT,
    WAIT_BIT_ONE,               // hiden state for a "1" bit
    WAIT_END
};

uint16_t    g_IrTxBuf[]={0x7c, 0x15, 0x15, 0x0a, 0x0a, 0x0b, 0x0b, 0x0a,
                        0x0b, 0x0a, 0x0a, 0x15, 0x15, 0x14, 0x15, 0x15,
                        0x15, 0x14, 0x0b, 0x0a, 0x0a, 0x15, 0x0a, 0x0b,
                        0x0a, 0x0b, 0x14, 0x15, 0x15, 0x0a, 0x15, 0x14,
                        0x15, 0x174,0x68};

static int irThresholds[STATE_NUM][2];     // min & max for a signal stste
static int irRecvBitCount;
static enum RCState irCurrState;
static unsigned long irRecvCode;      // LSB received code
static unsigned long irRecvCodeH;     // MSB received code if irTypeTable[5] > 32
static int irRepeatKeyPress, irRepeatKeyHold, irRepeatKeyFast, irRepeatKeyCnt;

static QueueHandle_t irQueue;
static QueueHandle_t irTxQueue;
static ITPKeypadEvent irLastEvent;
static int  g_IR_TX_HAS_INIT = 0;
static int  g_RxIndex=0;

static inline int MULSHIFT(int x, int y, int shift)
{
    int64_t xext, yext;
    xext = (int64_t)x;
    yext = (int64_t)y;
    xext = ((xext * yext) >> shift);
    return (int)xext;
}

static int IrProbe(int signal)
{
    int completeACode = 0, code;
    unsigned long getBit;

    if(signal != -1)
    {
        printf("IrProbe(),signal=0x%x\n",signal);

        #ifdef  ENABLE_DBG_COMPARE_CODE
        {
            uint32_t modFreq = ithReadRegA(ITH_IR_BASE + ITH_IR_MOD_FILTER_REG);

            if(signal==0x3FFF)
            {
                g_RxIndex = 0;
            }
            else
            {
                int highR,lowR;

                if( 20 > g_IrTxBuf[g_RxIndex] )
                {
                    highR = g_IrTxBuf[g_RxIndex] + 1;
                    if(g_IrTxBuf[g_RxIndex]>1)  lowR = g_IrTxBuf[g_RxIndex] - 1;
                    else                        lowR = 0;
                }
                else
                {
                    highR = g_IrTxBuf[g_RxIndex] + (g_IrTxBuf[g_RxIndex]*5)/100;
                    lowR = g_IrTxBuf[g_RxIndex] - (g_IrTxBuf[g_RxIndex]*5)/100;
                }

                printf(" ### check ir-RX code, signal=%x, g_RxIndex=%x,H&L=[%x,%x], modfrq=%x\n",signal,g_RxIndex,highR,lowR,modFreq);
                if( (signal > highR) || (signal < lowR) )
                //if( signal != g_IrTxBuf[g_RxIndex] )
                {
                    printf("error, IR code RX != TX, index=%x,[%x,%x] \n",g_RxIndex,signal,g_IrTxBuf[g_RxIndex]);
                    //while(1);
                }
                g_RxIndex++;
            }
        }
        #endif

        switch (irCurrState)
        {
        case WAIT_START:
            //printf("WStart\n");
            if ((signal >= irThresholds[WAIT_START][0]) && (signal <= irThresholds[WAIT_START][1]))
            {
                //printf("WB.1\n");
                irCurrState = WAIT_BIT;
            }
            else if ((signal >= irThresholds[WAIT_START_REPEAT_KEY][0]) && (signal <= irThresholds[WAIT_START_REPEAT_KEY][1]))
            {
                code = irRecvCode;

                //printf("WE.0,code=%x\n",irRecvCode);

            #ifdef CFG_IR_REPEAT
                if (irRepeatKeyPress >= REPEAT_THRESHOLD_BEGIN)
                {
                    //ithPrintf("Hold\n");

                    if ((irRepeatKeyFast == 0 && irRepeatKeyHold >= REPEAT_THRESHOLD_HOLD1) ||
                        (irRepeatKeyFast == 1 && irRepeatKeyHold >= REPEAT_THRESHOLD_HOLD2))
                    {
                        //ithPrintf("Send repeat key (%08x)\n", irRecvCode);

                        completeACode = 1;
                        irRepeatKeyHold = 0;
                        if (irRepeatKeyCnt >= REPEAT_THRESHOLD_SPDUP)
                            irRepeatKeyFast = 1;
                        else
                            irRepeatKeyCnt++;
                    }
                    else
                        irRepeatKeyHold++;
                }
                else
                    irRepeatKeyPress++;

            #endif // CFG_IR_REPEAT

                irCurrState = WAIT_END;
            }
            else
            {
                //printf("W_S\n");
                irCurrState = WAIT_START; // error
            }

            break;

        case WAIT_BIT:
            irRepeatKeyPress = irRepeatKeyHold = 0;
            irRepeatKeyFast = irRepeatKeyCnt = 0;
            if ((signal >= irThresholds[WAIT_BIT][0]) && (signal <= irThresholds[WAIT_BIT][1]))
            {
                // bit "0"
                getBit = 0;
            }
            else if ((signal >= irThresholds[WAIT_BIT_ONE][0]) && (signal <= irThresholds[WAIT_BIT_ONE][1]))
            {
                // bit "1"
                if (irTypeTable[6])
                    getBit = 0x80000000L;
                else
                    getBit = 0x00000001L;
            }
            else
            {
                // error
                irRecvCode = irRecvCodeH = irRecvBitCount = 0;
                irCurrState = WAIT_START;
                break;
            }

            if (irTypeTable[6])
            {
                irRecvCodeH = (irRecvCodeH >> 1) | ((irRecvCode & 1) << 31);
                irRecvCode  = (irRecvCode  >> 1) | getBit;
            }
            else
            {
                irRecvCodeH = (irRecvCodeH << 1) | (irRecvCode & 0x80000000L);
                irRecvCode  = (irRecvCode  << 1) | getBit;
            }

            irRecvBitCount++;
            if (irRecvBitCount < irTypeTable[5])
                irCurrState = WAIT_BIT;   // not yet complet a code
            else
            {
                irCurrState = WAIT_END;
            }
            break;

        case WAIT_END:
            printf("IR wait end!\n");
            if (signal >= irThresholds[WAIT_END][1]/*(signal >= irThresholds[WAIT_END][0]) && (signal <= irThresholds[WAIT_END][1])*/)
            {
                if (irRecvBitCount >= irTypeTable[5])
                {
                    //printf("WE1:code=%x\n",irRecvCode);
                    completeACode = 1;
                    code = irRecvCode;
                }
                else
                {
                    //printf("WE2\n");
                    completeACode  = 0;
                }

                irRecvBitCount = 0;
                irCurrState = WAIT_START;
            }
            else
            {
                // error
                //printf("WE3\n");
                irRecvBitCount = 0;
                irRepeatKeyPress = irRepeatKeyHold = 0;
                irRepeatKeyFast = irRepeatKeyCnt = 0;
                completeACode = 0;
                irCurrState = WAIT_START;
            }
            break;

        default:
            printf(" probe_default!!\n");
            break;
        }

        if (completeACode)
        {
            //printf(" got IR code=%x\n",code);
            return code;
        }
    }
    //printf("IR NOT complete!!\n");
    return -1;
}

static void IrTxSend(uint16_t *ptr)
{
    int i;
    int IrTxCnt = sizeof(g_IrTxBuf)/sizeof(uint16_t);

    if(ptr) printf("### IrTxSend() ###\n");
    printf("## DO RX clear ##\n");
    ithIrCtrlEnable(ITH_IR_EN);
    for(i=0; i<IrTxCnt; i++)
    {
        printf("irTx send:[%x]=%x\n",i,ptr[i]);
        ithIrTxTransmit(ptr[i]);
    }
}

#ifdef  ENABLE_IR_INTR
static void IrIntrHandler(void* arg)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    int code = IrProbe();

    printf("IR code: 0x%X\n", code);
    //printf("RxModFreq=0x%x\n",ithReadRegA(ITH_IR_BASE + ITH_IR_MOD_FILTER_REG2));

    if (code == -1)
        return;

    if (((code & 0xFFFF) == CFG_IR_VENDOR_CODE) && (((code >> 16) & 0xFF) == (~((code >> 24) & 0xFF) & 0xFF)))
        code = (code >> 16) & 0xFF;

    xQueueSendFromISR(irQueue, &code, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

#elif defined(ENABLE_IR_DMA)

static void
DummySleep(void)
{
    unsigned int idle    = 100;
    unsigned int i       = 0;
    unsigned int nothing = 0;

    for (i = 0; i < idle; i++)
    {
        nothing++;
    }
}

static void IrRxDMAHandler(void *arg)
{
    uint32_t transferSize = 0;
    uint32_t dataSize = 0;
    uint16_t tmpBuffer[512]; //size is IR_DMA_BUFFER_SIZE
    int i;
    int code = 0;


    while(1) {

        transferSize = ithReadRegA(ITH_DMA_BASE + ITH_DMA_C0_TX_SIZE_REG + RxChannel* ITH_DMA_CH_OFFSET);
        RxWriteIdx = (IR_DMA_TRANSFER_SIZE - transferSize);

        if(RxWriteIdx > IR_DMA_TRANSFER_SIZE) {
            printf("IR DMA Read fail.\n");
            continue;
        }
        
        if(RxWriteIdx != RxReadIdx) {
            
            //printf("transferSize=%d RxWriteIdx=%d RxReadIdx=%d\n", transferSize, RxWriteIdx, RxReadIdx);
            memset(tmpBuffer, 0, IR_DMA_BUFFER_SIZE);
        
            if(RxWriteIdx < RxReadIdx) {
                dataSize = (IR_DMA_TRANSFER_SIZE - RxReadIdx) + RxWriteIdx;
                ithInvalidateDCacheRange(RxBuffer, IR_DMA_BUFFER_SIZE);
                memcpy(tmpBuffer, RxBuffer + RxReadIdx, (IR_DMA_TRANSFER_SIZE - RxReadIdx)*sizeof(uint16_t));
                memcpy(tmpBuffer + (IR_DMA_TRANSFER_SIZE - RxReadIdx), RxBuffer, RxWriteIdx*sizeof(uint16_t));
            }
            else {
                dataSize = RxWriteIdx - RxReadIdx;
                ithInvalidateDCacheRange(RxBuffer, IR_DMA_BUFFER_SIZE);
                memcpy(tmpBuffer, RxBuffer + RxReadIdx, dataSize*sizeof(uint16_t));
            }
            
            RxReadIdx = RxWriteIdx;
            //printf("dataSize=%d\n", dataSize);

            for(i = 0; i < dataSize; i++) {
                uint32_t tmp_reg = 0x0;
                tmp_reg = tmpBuffer[i] & ((0x1 << irWidth) - 1);
                code = IrProbe(tmp_reg);
            
                if (code != -1) {
                    printf("code: 0x%X\n", code);
            
                    if (((code & 0xFFFF) == CFG_IR_VENDOR_CODE) && (((code >> 16) & 0xFF) == (~((code >> 24) & 0xFF) & 0xFF)))
                        code = (code >> 16) & 0xFF;
            
                    //printf("IR code: 0x%X\n", code);
                    //printf("RxModFreq=0x%x\n",ithReadRegA(ITH_IR_BASE + ITH_IR_MOD_FILTER_REG2));
            
                    xQueueSend(irQueue, &code, 0);                    
                }
            }           
        }
        usleep(2000);
    }
}

static void IrTxDMAHandler(void *arg)
{
    int code = 0;

    while(1)
    {
        if(g_IR_TX_HAS_INIT)
        {
            code=0;
            if(xQueueReceive(irTxQueue, &code, 0))
            {
                int IrTxCnt = sizeof(g_IrTxBuf)/sizeof(uint16_t);
                
                printf("got irTX code=%x\n",code);
                //enable dma to SEND IR CODE
                
                ithIrCtrlEnable(ITH_IR_EN);

                memcpy(TxBuffer, g_IrTxBuf, sizeof(g_IrTxBuf));
                
                ithDmaSetSrcAddr(TxChannel, (uint32_t)TxBuffer);
                ithDmaSetDstAddr(TxChannel, ITH_IR_BASE + ITH_IRTX_SEND_DATA_REG);

                ithDmaSetRequest(TxChannel, ITH_DMA_NORMAL_MODE, ITH_DMA_MEM, ITH_DMA_HW_HANDSHAKE_MODE, ITH_DMA_HW_IR_Cap_Tx);
                ithDmaSetSrcParams(TxChannel, ITH_DMA_WIDTH_16, ITH_DMA_CTRL_INC, ITH_DMA_MASTER_0);
                ithDmaSetDstParams(TxChannel, ITH_DMA_WIDTH_16, ITH_DMA_CTRL_FIX, ITH_DMA_MASTER_1);
                ithDmaSetTxSize(TxChannel, sizeof(g_IrTxBuf));
                ithDmaSetBurst(TxChannel, ITH_DMA_BURST_1);

                ithDmaStart(TxChannel);

                while(ithDmaIsBusy(TxChannel) /*&& --timeout_ms*/)
                {
                    DummySleep();
                }
                
            }
        }
        usleep(1000);
    }

}

#else
static void IrIntrHandler(void* arg)
{
    int code;

    while(1)
    {
        int signal = 0;
        //printf("start to IrProbe\n");        
        
        signal = ithIrProbe();
        code = IrProbe(signal);

        if (code != -1)
        {
            printf("code: 0x%X\n", code);

            if (((code & 0xFFFF) == CFG_IR_VENDOR_CODE) && (((code >> 16) & 0xFF) == (~((code >> 24) & 0xFF) & 0xFF)))
                code = (code >> 16) & 0xFF;

            printf("IR code: 0x%X\n", code);
            printf("RxModFreq=0x%x\n", ithReadRegA(ITH_IR_BASE + ITH_IR_MOD_FILTER_REG2));

            //xQueueSendFromISR(irQueue, &code, &xHigherPriorityTaskWoken);
            xQueueSend(irQueue, &code, 0);
        }
        usleep(2000);
    }
}

static void IrTxIntrHandler(void* arg)
{
    int code=0;

    while(1)
    {
        if(g_IR_TX_HAS_INIT)
        {
            code=0;
            if(xQueueReceive(irTxQueue, &code, 0))
            {
                printf("got irTX code=%x\n",code);
                //enable dma to SEND IR CODE
                //1.parse event to get IR TX code(code table)
                IrTxSend(g_IrTxBuf);
            }
        }
        usleep(1000);
    }
}
#endif

#define MAX_CODE_INDEX  2
static int IrCheckCodeMapping(int code)
{
    int i;

    for(i=0; i<MAX_CODE_INDEX; i++)
    {
        if(code == irValidTxCodeTable[i])   return 1;
    }
    return 0;
}

static void IrCalcThresholds(void)
{
    int i, f1, f2;
    int durations[STATE_NUM];

    durations[0] = irTypeTable[0] * (1 << PRECISION) / 1000;
    durations[1] = irTypeTable[1] * (1 << PRECISION) / 1000;
    durations[2] = irTypeTable[2] * (1 << PRECISION) / 1000;
    durations[3] = irTypeTable[3] * (1 << PRECISION) / 1000;
    durations[4] = irTypeTable[4] * (1 << PRECISION) / 1000;

    // know the sampling duration
    // set the min & max for each state
    for (i = 0; i < STATE_NUM; i++)
    {
        // 0.92 & 1.08 is experienc value, duration range N is between 0.92*N and 1.08*N.
        f1 = ((durations[i]) << 1) / (SAMP_DUR * (1 << PRECISION) / 1000);
        f2 = MULSHIFT(f1, 0.92f * (1 << PRECISION), PRECISION) >> 1;
        if (f2 >= MAX_VAL)
        {
            LOG_ERR "Out of counter resolution!\n" LOG_END
        }

        irThresholds[i][0] = (f2 < 1) ? 1 : f2; // f1 * 0.92
        irThresholds[i][1] = (i == STATE_NUM - 1) ?
            MAX_VAL :
            ((MULSHIFT(f1, 1.08f * (1 << PRECISION), PRECISION) + 1) >> 1); // f1 * 1.08 + 0.5

        LOG_DBG "irThresholds[%d][0] = %d, irThresholds[%d][1] = %d\n", i, irThresholds[i][0], i, irThresholds[i][1] LOG_END
        printf("irThresholds[%d][0] = 0x%x, irThresholds[%d][1] = 0x%x\n", i, irThresholds[i][0], i, irThresholds[i][1]);
    }
}

static void IrInit(void)
{
    irCurrState = WAIT_START;
    irRecvCode = irRecvCodeH = irRecvBitCount = 0;
    irRepeatKeyPress = irRepeatKeyHold = 0;
    irRepeatKeyFast = irRepeatKeyCnt = 0;

    ithIrInit(CFG_GPIO_IR, 0, SAMP_DUR, PRECISION);
    IrCalcThresholds();

    // Prepare q and intr
    irLastEvent.code = -1;
    irQueue = xQueueCreate(QUEUE_LEN, (unsigned portBASE_TYPE) sizeof(int));

    ithIrTxInit(CFG_GPIO_IR_TX, 0, SAMP_DUR, PRECISION);

    // Prepare q and intr
    irTxQueue = xQueueCreate(QUEUE_LEN, (unsigned portBASE_TYPE) sizeof(int));


#ifdef  ENABLE_IR_INTR
    portENTER_CRITICAL();

    // Init IR(remote control) interrupt
    ithIntrDisableIrq(ITH_INTR_RC);
    ithIntrClearIrq(ITH_INTR_RC);
    ithIntrRegisterHandlerIrq(ITH_INTR_RC, IrIntrHandler, NULL);
    ithIntrEnableIrq(ITH_INTR_RC);

    ithIrCtrlEnable(ITH_IR_INT);
    ithIrIntrCtrlEnable(ITH_IR_DATA);

    // Enable IR Capture
    ithIrCtrlEnable(ITH_IR_EN);

    //Maybe the intr setting of IR-TX is not necessary.
    //So skips the initialization of IR-TX temporally
    //ITH_INTR_RC only just for IR-RX(no IR-TX's intr number)

    portEXIT_CRITICAL();
#elif defined(ENABLE_IR_DMA)

    printf("IR DMA Support\n");

    RxChName = "dma_ir_read";
    TxChName = "dam_ir_write";

    {
        LLP_CONTEXT *llpaddr = NULL;

        RxWriteIdx = 0;
        RxReadIdx = 0;

        RxChannel = ithDmaRequestCh(RxChName, ITH_DMA_CH_PRIO_HIGH_3, NULL, NULL);
        ithDmaReset(RxChannel);
        TxChannel = ithDmaRequestCh(TxChName, ITH_DMA_CH_PRIO_HIGHEST, NULL, NULL);
        ithDmaReset(TxChannel);


        RxBuffer = (uint16_t *)itpVmemAlloc(IR_DMA_BUFFER_SIZE);
        TxBuffer = (uint16_t *)itpVmemAlloc(IR_DMA_BUFFER_SIZE);

        if(RxBuffer == NULL || TxBuffer == NULL) {
            printf("Alloc IR DMA buffer fail\n");
        }
        else {
            gLLPCtxt = (LLP_CONTEXT *)itpVmemAlloc(sizeof(LLP_CONTEXT) + 32);
            //printf("--- gLLPCtxt addr = 0x%x , sizeof(LLP_CONTEXT) = 0x%x---\n", gLLPCtxt, sizeof(LLP_CONTEXT));
            llpaddr = (LLP_CONTEXT *)(((uint32_t)gLLPCtxt + 0x1F) & ~(0x1F));
            //printf("--- new llpaddr addr = 0x%x\n", llpaddr);
            llpaddr->SrcAddr =  le32_to_cpu(ITH_IR_BASE + ITH_IR_CAP_DATA_REG);
            llpaddr->DstAddr = le32_to_cpu(RxBuffer);
            llpaddr->LLP = le32_to_cpu(llpaddr);           
            llpaddr->TotalSize = le32_to_cpu(IR_DMA_TRANSFER_SIZE);
            llpaddr->Control = le32_to_cpu(0x02620000);  //16bit
            

            ithDmaSetSrcAddr(RxChannel, ITH_IR_BASE + ITH_IR_CAP_DATA_REG);
            ithDmaSetDstAddr(RxChannel, (uint32_t)RxBuffer);
            ithDmaSetRequest(RxChannel, ITH_DMA_HW_HANDSHAKE_MODE, ITH_DMA_HW_IR_Cap_Rx, ITH_DMA_NORMAL_MODE, ITH_DMA_MEM);

            ithDmaSetSrcParams(RxChannel, ITH_DMA_WIDTH_16, ITH_DMA_CTRL_FIX, ITH_DMA_MASTER_1);
            ithDmaSetDstParams(RxChannel, ITH_DMA_WIDTH_16, ITH_DMA_CTRL_INC, ITH_DMA_MASTER_0);
            ithDmaSetTxSize(RxChannel, IR_DMA_BUFFER_SIZE);
            ithDmaSetBurst(RxChannel, ITH_DMA_BURST_1);

            ithDmaSetLLPAddr(RxChannel, (uint32_t)llpaddr);
            //printf("--- llpaddr:0x%x\n", (uint32_t)llpaddr);

            ithDmaStart(RxChannel);
        }       
    }

    {
        int res;
        pthread_t task;
        pthread_attr_t attr;

        printf("Create DMA IR-RX pthread~~\n");

        pthread_attr_init(&attr);
        res = pthread_create(&task, &attr, IrRxDMAHandler, NULL);

        if(res)
        {
            printf( "[IR]%s() L#%ld: ERROR, create IrRxDMAHandler thread fail! res=%ld\n", __FUNCTION__, __LINE__, res );
            return -1;
        }

        irWidth = (ithReadRegA(ITH_IR_BASE + ITH_IR_HWCFG_REG) & ITH_IR_WIDTH_MASK) >> ITH_IR_WIDTH_BIT;
        //ithIrRxSetModFilter(0x770,0xFA0); //20~42
        ithIrRxSetModFilter(0x535, 0xFA0); //20~60k freq.(80M/freq.)

        // Enable IR Capture
        ithIrCtrlEnable(ITH_IR_EN);
    }

    {
        int res;
        pthread_t task;
        pthread_attr_t attr;

        printf("Create DMA IR-TX pthread~~\n");

        pthread_attr_init(&attr);
        res = pthread_create(&task, &attr, IrTxDMAHandler, NULL);

        if(res)
        {
            printf( "[IR]%s() L#%ld: ERROR, create IrRxDMAHandler() thread fail! res=%ld\n", __FUNCTION__, __LINE__, res );
            return -1;
        }

        // Enable IR Capture
        ithIrTxCtrlEnable(ITH_IR_EN);

        ithIrTxSetModFreq(0x839);//(0x1e);

        g_IR_TX_HAS_INIT = 1;
    }

#else
    {
        int res;
        pthread_t task;
        pthread_attr_t attr;

        printf("Create IR pthread~~\n");

        pthread_attr_init(&attr);
        res = pthread_create(&task, &attr, IrIntrHandler, NULL);

        if(res)
        {
            printf( "[IR]%s() L#%ld: ERROR, create IrIntrHandler() thread fail! res=%ld\n", res );
            return -1;
        }

        ithIrRxSetModFilter(0x535, 0xFA0); //20~60k freq.(80M/freq.)

        // Enable IR Capture
        ithIrCtrlEnable(ITH_IR_EN);
    }


    {
        int res;
        pthread_t task;
        pthread_attr_t attr;

        printf("Create IR-TX pthread~~\n");

        pthread_attr_init(&attr);
        res = pthread_create(&task, &attr, IrTxIntrHandler, NULL);

        if(res)
        {
            printf( "[IR]%s() L#%ld: ERROR, create IrTxIntrHandler() thread fail! res=%ld\n", res );
            return -1;
        }

        // Enable IR Capture
        ithIrTxCtrlEnable(ITH_IR_EN);

        ithIrTxSetModFreq(0x839);//(0x1e);

        g_IR_TX_HAS_INIT = 1;
    }
    ithPrintRegA(ITH_IR_BASE, 0x40);

 #endif //#ifdef    ENABLE_IR_INTR

}

static int IrRead(int file, char *ptr, int len, void* info)
{
    ITPKeypadEvent* ev = (ITPKeypadEvent*)ptr;

    if (xQueueReceive(irQueue, &ev->code, 0))
    {
        gettimeofday(&ev->time, NULL);
        ev->flags = ITP_KEYPAD_DOWN;

        if (irLastEvent.code == ev->code)
            ev->flags |= ITP_KEYPAD_REPEAT;

        irLastEvent.code = ev->code;
        irLastEvent.time.tv_sec = ev->time.tv_sec;
        irLastEvent.time.tv_usec = ev->time.tv_usec;

        return sizeof (ITPKeypadEvent);
    }
    else if (irLastEvent.code != -1)
    {
        struct timeval now;

        gettimeofday(&now, NULL);
        if (itpTimevalDiff(&irLastEvent.time, &now) >= CFG_IR_PRESS_INTERVAL)
        {
            ev->code            = irLastEvent.code;
            ev->time.tv_sec     = now.tv_sec;
            ev->time.tv_usec    = now.tv_usec;
            ev->flags           = ITP_KEYPAD_UP;
            irLastEvent.code    = -1;
            return sizeof (ITPKeypadEvent);
        }
    }

    return 0;
}

static int IrWrite(int file, char *ptr, int len, void* info)
{
    ITPKeypadEvent* ev = (ITPKeypadEvent*)ptr;
    printf("irTx: fd=%x, ptr=%x, ev->code=%x, len=%d, info=%x\n",file,ptr,ev->code,len,info);

    //get event
    if(ev->code)
    {
        if( IrCheckCodeMapping(ev->code) )
        {
            printf("sendQue2IrTx(TxQue=%x,code=%x)\n", irTxQueue, ev->code);
            xQueueSend(irTxQueue, &ev->code, 0);
        }
        else
        {
            return 0;//TODO: maybe return non "0" value??
        }
    }

    return 0;
}

static int IrIoctl(int file, unsigned long request, void* ptr, void* info)
{
    switch (request)
    {
    case ITP_IOCTL_INIT:
        IrInit();
        break;

    default:
        errno = (ITP_DEVICE_IR << ITP_DEVICE_ERRNO_BIT) | __LINE__;
        return -1;
    }
    return 0;
}

const ITPDevice itpDeviceIr =
{
    ":ir",
    itpOpenDefault,
    itpCloseDefault,
    IrRead,
    IrWrite,
    itpLseekDefault,
    IrIoctl,
    NULL
};
