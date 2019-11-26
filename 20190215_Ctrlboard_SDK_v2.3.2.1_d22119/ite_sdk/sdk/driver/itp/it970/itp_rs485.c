/** @file
 * PAL RS485 functions.
 *
 * @author
 * @version 1.0
 * @date 20141014
 * @copyright ITE Tech. Inc. All Rights Reserved.
 */
#include <errno.h>
#include <sys/socket.h>
#include <pthread.h>
#include "itp_cfg.h"

#ifndef _MSC_VER
#include "openrtos/FreeRTOS.h"
#include "openrtos/queue.h"
#endif

#if defined (CFG_RS485_DMA)
#define DMA_TIMEOUT     10000
#define DMA_BUFFER_SIZE CFG_RS485_DMA_BUF_SIZE

typedef struct LLP_CONTEXT_TAG
{
    uint32_t  SrcAddr;
    uint32_t  DstAddr;
    uint32_t  LLP;
    uint32_t  Control;
    uint32_t  TotalSize;
}LLP_CONTEXT;
#endif

typedef struct _RS485_OBJ
{
	ITHRS485Port  rs485_port;
    ITHUartPort   uart_port;
	int           baudrate;
	ITHUartParity parity;
    ITHIntr       Intr;
    int           TxGpio;
    int           RxGpio;
	int           EnableGpio;
    QueueHandle_t xRxedChars;
    QueueHandle_t xCharsForTx;
    int           RxQueueFull;
	ITPPendFunction  itpRs485DeferIntrHandler;
	uint8_t       Rs485DeferIntrOn;
	uint32_t      timeout;	
#ifdef CFG_RS485_DMA
    char*         rdch_name;
    char*         wtch_name;
    int           dma_tx_req;
    int           dma_rx_req;
    int           readDmaChannel;
    int           writeDmaChannel;
    uint8_t *     tempTxBuf;
    uint8_t *     gpDMABuffer;
    uint32_t      gWriteIndex;
    uint32_t      gReadIndex;
    LLP_CONTEXT * g_LLPCtxt;
#endif
}RS485_OBJ;

static RS485_OBJ Rs485obj[6] =
{
    {
        ITH_RS485_0,
		ITH_UART0,
#ifdef CFG_RS485_0_BAUDRATE
		CFG_RS485_0_BAUDRATE,
#else
		0,
#endif
		ITH_UART_NONE,
        ITH_INTR_UART0,
#ifdef CFG_GPIO_RS485_0_TX
        CFG_GPIO_RS485_0_TX,
        CFG_GPIO_RS485_0_RX,
        -1,
#else
        -1,
        -1,
        -1,
#endif
        NULL,                           // xRxedChars
        NULL,                           // xCharsForTx
        0,                              //RxQueueFull
        NULL,
        0,
        0,								//timeout
#ifdef CFG_RS485_DMA
        "dma_rs485_0_read",
        "dma_rs485_0_write",
		ITH_DMA_UART0_TX,
		ITH_DMA_UART0_RX,
#endif
    },
	{
		ITH_RS485_1,
		ITH_UART1,
#ifdef CFG_RS485_1_BAUDRATE
		CFG_RS485_1_BAUDRATE,
#else
		0,
#endif
		ITH_UART_NONE,
		ITH_INTR_UART1,
#ifdef CFG_GPIO_RS485_1_TX
		CFG_GPIO_RS485_1_TX,
		CFG_GPIO_RS485_1_RX,
		-1,
#else
		-1,
		-1,
		-1,
#endif
		NULL,							// xRxedChars
		NULL,							// xCharsForTx
		0,								//RxQueueFull
		NULL,
		0,
		0,								//timeout
#ifdef CFG_RS485_DMA
		"dma_rs485_1_read",
		"dma_rs485_1_write",
		ITH_DMA_UART1_TX,
		ITH_DMA_UART1_RX,
#endif
	},
	{
		ITH_RS485_2,
		ITH_UART2,
#ifdef CFG_RS485_2_BAUDRATE
		CFG_RS485_2_BAUDRATE,
#else
		2,
#endif
		ITH_UART_NONE,
		ITH_INTR_UART2,
#ifdef CFG_GPIO_RS485_2_TX
		CFG_GPIO_RS485_2_TX,
		CFG_GPIO_RS485_2_RX,
		-1,
#else
		-1,
		-1,
		-1,
#endif
		NULL,							// xRxedChars
		NULL,							// xCharsForTx
		0,								//RxQueueFull
		NULL,
		0,
		0,								//timeout
#ifdef CFG_RS485_DMA
		"dma_rs485_2_read",
		"dma_rs485_2_write",
		ITH_DMA_UART2_TX,
		ITH_DMA_UART2_RX,
#endif
	},
	{
		ITH_RS485_3,
		ITH_UART3,
#ifdef CFG_RS485_3_BAUDRATE
		CFG_RS485_3_BAUDRATE,
#else
		0,
#endif
		ITH_UART_NONE,
		ITH_INTR_UART3,
#ifdef CFG_GPIO_RS485_3_TX
		CFG_GPIO_RS485_3_TX,
		CFG_GPIO_RS485_3_RX,
		-1,
#else
		-1,
		-1,
		-1,
#endif
		NULL,							// xRxedChars
		NULL,							// xCharsForTx
		0,								//RxQueueFull
		NULL,
		0,
		0,								//timeout
#ifdef CFG_RS485_DMA
		"dma_rs485_3_read",
		"dma_rs485_3_write",
		ITH_DMA_UART3_TX,
		ITH_DMA_UART3_RX,
#endif
	},
	{
		ITH_RS485_4,
		ITH_UART4,
#ifdef CFG_RS485_4_BAUDRATE
		CFG_RS485_4_BAUDRATE,
#else
		0,
#endif
		ITH_UART_NONE,
		ITH_INTR_UART4,
#ifdef CFG_GPIO_RS485_4_TX
		CFG_GPIO_RS485_4_TX,
		CFG_GPIO_RS485_4_RX,
		-1,
#else
		-1,
		-1,
		-1,
#endif
		NULL,							// xRxedChars
		NULL,							// xCharsForTx
		0,								//RxQueueFull
		NULL,
		0,
		0,								//timeout
#ifdef CFG_RS485_DMA
		"dma_rs485_4_read",
		"dma_rs485_4_write",
		ITH_DMA_UART4_TX,
		ITH_DMA_UART4_RX,
#endif
	},
	{
		ITH_RS485_5,
		ITH_UART5,
#ifdef CFG_RS485_5_BAUDRATE
		CFG_RS485_5_BAUDRATE,
#else
		0,
#endif
		ITH_UART_NONE,
		ITH_INTR_UART5,
#ifdef CFG_GPIO_RS485_5_TX
		CFG_GPIO_RS485_5_TX,
		CFG_GPIO_RS485_5_RX,
		-1,
#else
		-1,
		-1,
		-1,
#endif
		NULL,							// xRxedChars
		NULL,							// xCharsForTx
		0,								//RxQueueFull
		NULL,
		0,
		0,								//timeout
#ifdef CFG_RS485_DMA
		"dma_rs485_5_read",
		"dma_rs485_5_write",
		ITH_DMA_UART5_TX,
		ITH_DMA_UART5_RX,
#endif
	},
};

static ITHUartPort RS485PutcharPort;
static pthread_mutex_t RS485InternalMutex  = PTHREAD_MUTEX_INITIALIZER;
static ITHUartPort  gUartPort;
static ITHRS485Port gRS485_Port;


#if defined(CFG_RS485_INTR)

static void RS485TxDeferCB(void* arg1, uint32_t arg2)
{
    ITHUartPort port = (ITHUartPort) arg1;
	RS485_OBJ *rs485obj = NULL;

	switch(port)
	{
		case ITH_UART0:
			rs485obj = &Rs485obj[0];
			break;
		case ITH_UART1:
			rs485obj = &Rs485obj[1];
			break;
		case ITH_UART2:
			rs485obj = &Rs485obj[2];
			break;
		case ITH_UART3:
			rs485obj = &Rs485obj[3];
			break;
		case ITH_UART4:
			rs485obj = &Rs485obj[4];
			break;
		case ITH_UART5:
			rs485obj = &Rs485obj[5];
			break;
	}

	if(rs485obj->EnableGpio != -1) {
		//wait Tx and Tx Fifo empty and disable TX_ENABLE gpio.
		while(!ithUartIsTxWIthFifoEmpty(port) && ithGpioGet(rs485obj->EnableGpio)) {
			//printf("TX not ready!\n");
		}
		
	    ithGpioClear(rs485obj->EnableGpio);
	    ithGpioSetMode(rs485obj->EnableGpio, ITH_GPIO_MODE0);
	    ithGpioSetIn(rs485obj->EnableGpio);
	}
}

static void RS485IntrHandler(void* arg)
{
    ITHUartPort port = (ITHUartPort) arg;
    uint32_t status = ithUartClearIntr(port);
	RS485_OBJ *rs485obj = NULL;

	signed char cChar;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	QueueHandle_t txQueue, rxQueue;
	int *pRxqueueFull = NULL;
	long result = 0;

	switch(port)
	{
		case ITH_UART0:
			rs485obj = &Rs485obj[0];
			break;
		case ITH_UART1:
			rs485obj = &Rs485obj[1];
			break;
		case ITH_UART2:
			rs485obj = &Rs485obj[2];
			break;
		case ITH_UART3:
			rs485obj = &Rs485obj[3];
			break;
		case ITH_UART4:
			rs485obj = &Rs485obj[4];
			break;
		case ITH_UART5:
			rs485obj = &Rs485obj[5];
			break;
	}

	txQueue = rs485obj->xCharsForTx;
	rxQueue = rs485obj->xRxedChars;
	pRxqueueFull = &rs485obj->RxQueueFull;


	if (status & ITH_UART_THR_EMPTY)
	{
	   unsigned int i;
	   /* The interrupt was caused by the THR becoming empty.  Are there any
		   more characters to transmit? */

	   for (;;)
	   {
		   if (xQueueReceiveFromISR(txQueue, &cChar, &xHigherPriorityTaskWoken) == pdTRUE)
		   {
				/* A character was retrieved from the queue so can be sent to the
					   THR now. */
				ithUartPutChar(port, cChar);

				if (ithUartIsTxFull(port))
					break;
           }
           else
           {
				/* Queue empty, nothing to send so turn off the Tx interrupt. */
				ithUartDisableIntr(port, ITH_UART_TX_READY);			   
				itpPendFunctionCallFromISR(RS485TxDeferCB, (void*)port, NULL);
				break;
           }
        }
    }

	if (status & ITH_UART_RECV_READY)
	{
	   /* The interrupt was caused by a character being received.  Grab the
		   character from the RHR and place it in the queue or received
		   characters. */
		   
	   while (ithUartIsRxReady(port))
	   {
			cChar = ithUartGetChar(port);
			result = xQueueSendFromISR( rxQueue, &cChar, &xHigherPriorityTaskWoken );		   
			if(result == 0)
				*pRxqueueFull = 1;

			if(rs485obj->Rs485DeferIntrOn)
				itpPendFunctionCallFromISR(rs485obj->itpRs485DeferIntrHandler,NULL,NULL);
			else
				rs485obj->itpRs485DeferIntrHandler(NULL,NULL);
	   }
	}

	/* If an event caused a task to unblock then we call "Yield from ISR" to
	   ensure that the unblocked task is the task that executes when the interrupt
	   completes if the unblocked task has a priority higher than the interrupted
	   task. */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

#elif defined(CFG_RS485_DMA)
static void
DummySleep(void)
{
    unsigned int idle = 100;
    unsigned int i = 0;
    unsigned int nothing = 0;

    for (i = 0; i < idle; i++)
    {
        nothing++;
    }
}
#endif 

static int RS485Putchar(int c)
{
#if defined(CFG_RS485_INTR)
    QueueHandle_t txQueue;
	RS485_OBJ *rs485obj = NULL;

	switch(RS485PutcharPort)
	{
	case ITH_UART0:
		rs485obj = &Rs485obj[0];
		break;
	case ITH_UART1:
		rs485obj = &Rs485obj[1];
		break;
	case ITH_UART2:
		rs485obj = &Rs485obj[2];
		break;
	case ITH_UART3:
		rs485obj = &Rs485obj[3];
		break;
	case ITH_UART4:
		rs485obj = &Rs485obj[4];
		break;
	case ITH_UART5:
		rs485obj = &Rs485obj[5];
		break;
	default:
		rs485obj = &Rs485obj[0];
	}

	txQueue = rs485obj->xCharsForTx;

    if (ithGetCpuMode() == ITH_CPU_SYS)
    {
        xQueueSend( txQueue, &c, portMAX_DELAY );

        if (ithUartIsTxEmpty(RS485PutcharPort))
        {
            signed char cChar;

            if (xQueueReceive(txQueue, &cChar, 0) == pdTRUE)
            {
                ithUartPutChar(RS485PutcharPort, cChar);
            }
        }
    }
    else
    {
        xQueueSendFromISR( txQueue, &c, 0 );

        if (ithUartIsTxEmpty(RS485PutcharPort))
        {
            signed char cChar;
            portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

            if (xQueueReceiveFromISR(txQueue, &cChar, &xHigherPriorityTaskWoken) == pdTRUE)
            {
                ithUartPutChar(RS485PutcharPort, cChar);
            }

            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }

    ithUartEnableIntr(RS485PutcharPort, ITH_UART_TX_READY);

#else

    // Is transmitter ready?
    while (ithUartIsTxFull(RS485PutcharPort));
    ithUartPutChar(RS485PutcharPort, c);

#endif 
    return c;
}

static void RS485DefaultIntrHandler(void)
{
    // DO NOTHING
}


static void RS485Open(ITHRS485Port RS485_Port,ITHUartPort UartPort, unsigned int baud)
{
	RS485_OBJ *rs485obj = NULL;
	
    pthread_mutex_lock(&RS485InternalMutex);

	switch(RS485_Port) {
		case ITH_RS485_0:
			rs485obj = &Rs485obj[0];
			break;
		case ITH_RS485_1:
			rs485obj = &Rs485obj[1];
			break;
		case ITH_RS485_2:
			rs485obj = &Rs485obj[2];
			break;
		case ITH_RS485_3:
			rs485obj = &Rs485obj[3];
			break;
		case ITH_RS485_4:
			rs485obj = &Rs485obj[4];
			break;
		case ITH_RS485_5:
			rs485obj = &Rs485obj[5];
			break;
	}

    /* Set the required protocol. */
    if(rs485obj->parity) {
    	ithUartReset(UartPort, baud, rs485obj->parity, 1, 8);
    }
	else {
    	ithUartReset(UartPort, baud, ITH_UART_NONE, 1, 8);
    }

    /* Enable Rx and Tx. */
    ithUartSetMode(UartPort, ITH_UART_DEFAULT, rs485obj->TxGpio, rs485obj->RxGpio);

    pthread_mutex_unlock(&RS485InternalMutex);
}

static void RS485Close(void)
{
    return ;
}

static void RS485Reset(ITHUartPort UartPort)
{
    unsigned int levelTx, levelRx , i;
    ITHUartFifoDepth fifoDepth;
	static int RS485_DMA_init = 0;
	RS485_OBJ *rs485obj = NULL;

    pthread_mutex_lock(&RS485InternalMutex);

	switch(UartPort)
	{
		case ITH_UART0:
			rs485obj = &Rs485obj[0];
			break;
		case ITH_UART1:
			rs485obj = &Rs485obj[1];
			break;
		case ITH_UART2:
			rs485obj = &Rs485obj[2];
			break;
		case ITH_UART3:
			rs485obj = &Rs485obj[3];
			break;
		case ITH_UART4:
			rs485obj = &Rs485obj[4];
			break;
		case ITH_UART5:
			rs485obj = &Rs485obj[5];
			break;
	}
			
#if defined(CFG_RS485_INTR)
    {
		if(!rs485obj->xRxedChars && !rs485obj->xCharsForTx) {
			rs485obj->xRxedChars = xQueueCreate( CFG_RS485_BUF_SIZE, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
			rs485obj->xCharsForTx = xQueueCreate( CFG_RS485_BUF_SIZE + 1, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
		}

        ithEnterCritical();

		rs485obj->itpRs485DeferIntrHandler = RS485DefaultIntrHandler;
		rs485obj->Rs485DeferIntrOn = 0;
		
        /* Enable the Rx interrupts.  The Tx interrupts are not enabled
        		til there are characters to be transmitted. */
        ithIntrDisableIrq(rs485obj->Intr);
        ithUartClearIntr(UartPort);
        ithIntrClearIrq(rs485obj->Intr);
 
        ithIntrSetTriggerModeIrq(rs485obj->Intr, ITH_INTR_LEVEL);
        ithIntrRegisterHandlerIrq(rs485obj->Intr, RS485IntrHandler, (void*)UartPort);
        ithUartEnableIntr(UartPort, ITH_UART_RX_READY);

        /* Enable the interrupts. */
        ithIntrEnableIrq(rs485obj->Intr);

        ithExitCritical();
    }

#elif defined (CFG_RS485_DMA)
	if(!RS485_DMA_init)
	{
        LLP_CONTEXT *llpaddr = NULL;

		rs485obj->readDmaChannel = ithDmaRequestCh(rs485obj->rdch_name, ITH_DMA_CH_PRIO_HIGH_3, NULL, NULL);
		ithDmaReset(rs485obj->readDmaChannel);
		rs485obj->writeDmaChannel = ithDmaRequestCh(rs485obj->wtch_name, ITH_DMA_CH_PRIO_HIGHEST, NULL, NULL);
		ithDmaReset(rs485obj->writeDmaChannel);

		rs485obj->tempTxBuf = (uint8_t *)itpVmemAlloc(DMA_BUFFER_SIZE);
		rs485obj->gpDMABuffer = (uint8_t *)itpVmemAlloc(DMA_BUFFER_SIZE);

        if (rs485obj->gpDMABuffer == NULL || rs485obj->tempTxBuf == NULL)
        {
            printf("Alloc DMA buffer fail\n");
        }
        else
        {
            rs485obj->g_LLPCtxt = (LLP_CONTEXT *)itpVmemAlloc(sizeof(LLP_CONTEXT) + 32);
            //printf("--- g_LLPCtxt addr = 0x%x , sizeof(LLP_CONTEXT) = 0x%x---\n", g_LLPCtxt, sizeof(LLP_CONTEXT));
            llpaddr = (LLP_CONTEXT *)(((uint32_t)rs485obj->g_LLPCtxt + 0x1F) & ~(0x1F));
            //printf("new g_LLPCtxt addr = 0x%x\n", llpaddr);
            llpaddr->SrcAddr =  le32_to_cpu(UartPort);
            llpaddr->DstAddr = le32_to_cpu(rs485obj->gpDMABuffer);
            llpaddr->LLP = le32_to_cpu(llpaddr);
            llpaddr->TotalSize = le32_to_cpu(DMA_BUFFER_SIZE);
            llpaddr->Control = le32_to_cpu(0x00210000);

            ithDmaSetSrcAddr(rs485obj->readDmaChannel, UartPort);
            ithDmaSetDstAddr(rs485obj->readDmaChannel, (uint32_t)rs485obj->gpDMABuffer);
			ithDmaSetRequest(rs485obj->readDmaChannel, ITH_DMA_HW_HANDSHAKE_MODE, rs485obj->dma_rx_req, ITH_DMA_NORMAL_MODE, ITH_DMA_MEM);
            ithDmaSetSrcParams(rs485obj->readDmaChannel, ITH_DMA_WIDTH_8, ITH_DMA_CTRL_FIX, ITH_DMA_MASTER_1);
            ithDmaSetDstParams(rs485obj->readDmaChannel, ITH_DMA_WIDTH_8, ITH_DMA_CTRL_INC, ITH_DMA_MASTER_0);
            ithDmaSetTxSize(rs485obj->readDmaChannel, DMA_BUFFER_SIZE);
            ithDmaSetBurst(rs485obj->readDmaChannel, ITH_DMA_BURST_1);
            ithDmaSetLLPAddr(rs485obj->readDmaChannel, (uint32_t)llpaddr);
            //printf("llpaddr:0x%x\n", (uint32_t)llpaddr);

            ithUartEnableDmaMode2(UartPort);
            ithDmaStart(rs485obj->readDmaChannel);
        }
	    
		RS485_DMA_init = 1;
	}
#endif

    if (RS485PutcharPort == 0)
        RS485PutcharPort = UartPort;
	
	usleep(1000*100); // if run roo fast, it will become sothing wrong, Benson

	if(!ithPutcharFunc)	ithPutcharFunc = RS485Putchar;
    pthread_mutex_unlock(&RS485InternalMutex);
}

static int RS485Read(int file, char *ptr, int len, void* info)
{
    ITHUartPort port = (ITHUartPort) info;
    int count = 0;
	int i;
	RS485_OBJ *rs485obj = NULL;

    pthread_mutex_lock(&RS485InternalMutex);

	switch(port)
	{
		case ITH_UART0:
			rs485obj = &Rs485obj[0];
			break;
		case ITH_UART1:
			rs485obj = &Rs485obj[1];
			break;
		case ITH_UART2:
			rs485obj = &Rs485obj[2];
			break;
		case ITH_UART3:
			rs485obj = &Rs485obj[3];
			break;
		case ITH_UART4:
			rs485obj = &Rs485obj[4];
			break;
		case ITH_UART5:
			rs485obj = &Rs485obj[5];
			break;
	}

#if defined(CFG_RS485_INTR)
    QueueHandle_t rxQueue; //  = (port == ITH_UART0) ? xRxedChars[0] : xRxedChars[1];
    int* pRxqueueFull = NULL;

	rxQueue = rs485obj->xRxedChars;
	pRxqueueFull = &rs485obj->RxQueueFull;
	
	if (xQueueReceive(rxQueue, ptr++, 0))
	{
		count++;		
		while (count < len)
		{			
			if (xQueueReceive(rxQueue, ptr++, 0))
				count++;							
			else
				break;
		}
	}
	if (*pRxqueueFull)
		printf("rxqueue is Full\n");

    pthread_mutex_unlock(&RS485InternalMutex);
    return count;
	
#elif defined (CFG_RS485_DMA)
    uint32_t transferSize = 0;
    uint32_t dataSize = 0;

	//Disable TX gpio
	if(rs485obj->EnableGpio != -1) {
		//wait Tx and Tx Fifo empty and disable TX_ENABLE gpio.
		while(!ithUartIsTxWIthFifoEmpty(port) && ithGpioGet(rs485obj->EnableGpio)) {
			//printf("TX not ready!\n");
		}
		
	    ithGpioClear(rs485obj->EnableGpio);
	    ithGpioSetMode(rs485obj->EnableGpio, ITH_GPIO_MODE0);
	    ithGpioSetIn(rs485obj->EnableGpio);
	}

    transferSize = ithReadRegA(ITH_DMA_BASE+ITH_DMA_SIZE_CH(rs485obj->readDmaChannel));
    
    rs485obj->gWriteIndex = (DMA_BUFFER_SIZE - transferSize);
	
	if (rs485obj->gWriteIndex > DMA_BUFFER_SIZE)
	{
    	ithUartDisableDmaMode2(port);
		pthread_mutex_unlock(&RS485InternalMutex);
    	return dataSize;
	}
	
    if (rs485obj->gWriteIndex != rs485obj->gReadIndex)
    {
        //printf("[transferSize]:0x%x [gWriteIndex]:0x%x [gReadIndex]:0x%x\n", transferSize, gWriteIndex, gReadIndex);
        if (rs485obj->gWriteIndex < rs485obj->gReadIndex)
        {
            dataSize = (DMA_BUFFER_SIZE - rs485obj->gReadIndex) + rs485obj->gWriteIndex;
            ithInvalidateDCacheRange(rs485obj->gpDMABuffer, DMA_BUFFER_SIZE);
            memcpy(ptr, rs485obj->gpDMABuffer + rs485obj->gReadIndex, DMA_BUFFER_SIZE - rs485obj->gReadIndex);
            memcpy(ptr + (DMA_BUFFER_SIZE - rs485obj->gReadIndex), rs485obj->gpDMABuffer, rs485obj->gWriteIndex);
        }
        else
        {
            dataSize = rs485obj->gWriteIndex - rs485obj->gReadIndex;
            ithInvalidateDCacheRange(rs485obj->gpDMABuffer, DMA_BUFFER_SIZE);
            memcpy(ptr, rs485obj->gpDMABuffer + rs485obj->gReadIndex, dataSize);
        }

        rs485obj->gReadIndex = rs485obj->gWriteIndex;
        //printf("dataSize:%d\n", dataSize);

    }

    ithUartDisableDmaMode2(port);
    pthread_mutex_unlock(&RS485InternalMutex);
    return dataSize;

#else

	//Disable TX gpio
	if(rs485obj->EnableGpio != -1) {
		//wait Tx and Tx Fifo empty and disable TX_ENABLE gpio.
		while(!ithUartIsTxWIthFifoEmpty(port) && ithGpioGet(rs485obj->EnableGpio)) {
			//printf("TX not ready!\n");
		}
		
	    ithGpioClear(rs485obj->EnableGpio);
	    ithGpioSetMode(rs485obj->EnableGpio, ITH_GPIO_MODE0);
	    ithGpioSetIn(rs485obj->EnableGpio);
	}

    count = 0;

    // Is a character waiting?
    while (ithUartIsRxReady(port))
    {
        *ptr++ = ithUartGetChar(port);       // Read character from RS485
        count++;
    }
    pthread_mutex_unlock(&RS485InternalMutex);
    if (count > 0)
        return count;

    return 0;

#endif
}

static int RS485Write(int file, char *ptr, int len, void* info)
{
    ITHUartPort port = (ITHUartPort) info;
    int i;
	RS485_OBJ *rs485obj = NULL;
	
    pthread_mutex_lock(&RS485InternalMutex);

	switch(port)
	{
		case ITH_UART0:
			rs485obj = &Rs485obj[0];
			break;
		case ITH_UART1:
			rs485obj = &Rs485obj[1];
			break;
		case ITH_UART2:
			rs485obj = &Rs485obj[2];
			break;
		case ITH_UART3:
			rs485obj = &Rs485obj[3];
			break;
		case ITH_UART4:
			rs485obj = &Rs485obj[4];
			break;
		case ITH_UART5:
			rs485obj = &Rs485obj[5];
			break;
	}

    //Enable TX gpio
    //usleep(1);

	if(rs485obj->EnableGpio != -1) {
	    ithGpioSet(rs485obj->EnableGpio);
	    ithGpioSetMode(rs485obj->EnableGpio, ITH_GPIO_MODE0);
	    ithGpioSetOut(rs485obj->EnableGpio);
	}

#if defined(CFG_RS485_INTR)
    QueueHandle_t txQueue; // = (port == ITH_UART0) ? xCharsForTx[0] : xCharsForTx[1];

	txQueue = rs485obj->xCharsForTx;

    for (i = 0; i < len; i++)
    {
        /* Place the character in the queue of characters to be transmitted. */
		if (xQueueSend(txQueue, &ptr[i], portMAX_DELAY) != pdPASS)
            break;
    }

    if (ithUartIsTxEmpty(port))
    {
        signed char cChar;
		if (xQueueReceive(txQueue, &cChar, 0) == pdTRUE)
        {
            ithUartPutChar(port, cChar);
        }
    }

    /* Turn on the Tx interrupt so the ISR will remove the character from the
    queue and send it.   This does not need to be in a critical section as
    if the interrupt has already removed the character the next interrupt
    will simply turn off the Tx interrupt again. */
    ithUartEnableIntr(port, ITH_UART_TX_READY);

	//Note. Disable TX_ENABLE Pin in ISR routine.
    pthread_mutex_unlock(&RS485InternalMutex);
    return i;

#elif defined (CFG_RS485_DMA)
    uint32_t timeout_ms = 3000;
    uint32_t dstWidth = ITH_DMA_WIDTH_16;
    uint32_t srcWidth = ITH_DMA_WIDTH_16;
    ITHDmaBurst burstSize    = ITH_DMA_BURST_1;

    /* Lock */
    //ithLockMutex(ithStorMutex);

    if (rs485obj->tempTxBuf == NULL)
    {
    	printf("tempTxBuf =NULL!\n");
        return 0;
    }

    memcpy(rs485obj->tempTxBuf, ptr, len);
    ithDmaSetSrcAddr(rs485obj->writeDmaChannel, (uint32_t)rs485obj->tempTxBuf);
    ithDmaSetDstAddr(rs485obj->writeDmaChannel, (ITHUartPort)info);

	ithDmaSetRequest(rs485obj->writeDmaChannel, ITH_DMA_NORMAL_MODE, ITH_DMA_MEM, ITH_DMA_HW_HANDSHAKE_MODE, rs485obj->dma_tx_req);

    ithDmaSetSrcParams(rs485obj->writeDmaChannel, ITH_DMA_WIDTH_8, ITH_DMA_CTRL_INC, ITH_DMA_MASTER_0);
    ithDmaSetDstParams(rs485obj->writeDmaChannel, ITH_DMA_WIDTH_8, ITH_DMA_CTRL_FIX, ITH_DMA_MASTER_1);
    ithDmaSetTxSize(rs485obj->writeDmaChannel, len);
    ithDmaSetBurst(rs485obj->writeDmaChannel, ITH_DMA_BURST_1);
	
    ithUartEnableDmaMode2((ITHUartPort)info);

    ithDmaStart(rs485obj->writeDmaChannel);

    while (ithDmaIsBusy(rs485obj->writeDmaChannel) /*&& --timeout_ms*/)
    {
        DummySleep();
    }

    ithUartDisableDmaMode2((ITHUartPort)info);

    /* Unlock */
    pthread_mutex_unlock(&RS485InternalMutex);

    return len;
#else

#ifndef _MSC_VER
#ifndef NDEBUG
    portSAVEDISABLE_INTERRUPTS();
#endif
#endif
    for (i = 0; i < len; i++)
    {
        // Is transmitter ready?
        while (ithUartIsTxFull(port));
        ithUartPutChar(port, *ptr++);
    }
#ifndef _MSC_VER	
#ifndef NDEBUG
    portRESTORE_INTERRUPTS();
#endif
#endif
    pthread_mutex_unlock(&RS485InternalMutex);

    return len;

#endif
}

static int RS485Ioctl(int file, unsigned long request, void* ptr, void* info)
{
    int i;
	ITHRS485Port RS485_Port;
	RS485_OBJ *rs485obj = NULL;
    gUartPort = (ITHUartPort) info;

	if(gUartPort == ITH_UART0) {
		rs485obj = &Rs485obj[0];
	}
	else if(gUartPort == ITH_UART1) {
		rs485obj = &Rs485obj[1];
	}
	else if(gUartPort == ITH_UART2) {
		rs485obj = &Rs485obj[2];
	}
	else if(gUartPort == ITH_UART3) {
		rs485obj = &Rs485obj[3];
	}
	else if(gUartPort == ITH_UART4) {
		rs485obj = &Rs485obj[4];
	}
	else if(gUartPort == ITH_UART5) {
		rs485obj = &Rs485obj[5];
	}	

    switch (request)
    {
    case FIONREAD:
        // Is a character waiting?
        break;

	case ITP_IOCTL_REG_RS485_DEFER_CB:
		rs485obj->Rs485DeferIntrOn = 1;
		break;
    case ITP_IOCTL_REG_RS485_CB:
		rs485obj->itpRs485DeferIntrHandler = (ITPPendFunction)ptr;
		break;
	
    case ITP_IOCTL_INIT:
        rs485obj->parity = *(ITHUartParity*)ptr;
        break;

    case ITP_IOCTL_ON:
        gRS485_Port = RS485_Port = *(ITHRS485Port*)ptr;

		switch(RS485_Port) {
			case ITH_RS485_0:
				rs485obj = &Rs485obj[0];
				break;
			case ITH_RS485_1:
				rs485obj = &Rs485obj[1];
				break;
			case ITH_RS485_2:
				rs485obj = &Rs485obj[2];
				break;
			case ITH_RS485_3:
				rs485obj = &Rs485obj[3];
				break;
			case ITH_RS485_4:
				rs485obj = &Rs485obj[4];
				break;
			case ITH_RS485_5:
				rs485obj = &Rs485obj[5];
				break;
		}

		if(rs485obj->EnableGpio != -1) {
			ithGpioClear(rs485obj->EnableGpio);
			ithGpioSetMode(rs485obj->EnableGpio, ITH_GPIO_MODE0);
			ithGpioSetIn(rs485obj->EnableGpio);
		}

		switch(RS485_Port) {
#if defined(CFG_RS485_0_ENABLE)
			case ITH_RS485_0:
				rs485obj->EnableGpio = CFG_GPIO_RS485_0_TX_ENABLE;
				break;
#endif
#if defined(CFG_RS485_1_ENABLE)
			case ITH_RS485_1:
				rs485obj->EnableGpio = CFG_GPIO_RS485_1_TX_ENABLE;
				break;
#endif
#if defined(CFG_RS485_2_ENABLE)
			case ITH_RS485_2:
				rs485obj->EnableGpio = CFG_GPIO_RS485_2_TX_ENABLE;
				break;
#endif
#if defined(CFG_RS485_3_ENABLE)
			case ITH_RS485_3:
				rs485obj->EnableGpio = CFG_GPIO_RS485_3_TX_ENABLE;
				break;
#endif
#if defined(CFG_RS485_4_ENABLE)
			case ITH_RS485_4:
				rs485obj->EnableGpio = CFG_GPIO_RS485_4_TX_ENABLE;
				break;
#endif
#if defined(CFG_RS485_5_ENABLE)
			case ITH_RS485_5:
				rs485obj->EnableGpio = CFG_GPIO_RS485_5_TX_ENABLE;
				break;
#endif
			default:
				break;
		}

	    ithGpioClear(rs485obj->RxGpio);
        ithGpioSetMode(rs485obj->RxGpio, ITH_GPIO_MODE0);
        ithGpioSetIn(rs485obj->RxGpio);
        ithGpioClear(rs485obj->TxGpio);
        ithGpioSetMode(rs485obj->TxGpio, ITH_GPIO_MODE0);
        ithGpioSetIn(rs485obj->TxGpio);
		
        RS485Open(RS485_Port , rs485obj->uart_port, rs485obj->baudrate);

        break;

    case ITP_IOCTL_OFF:
        RS485Close();
        break;
	
	case ITP_IOCTL_SET_BAUDRATE:
		rs485obj->baudrate = *(int*)ptr;
		ithUartSetBaudRate(gUartPort, rs485obj->baudrate);
		break;

	case ITP_IOCTL_SET_PARITY:
		rs485obj->parity = *(ITHUartParity*)ptr;
		ithUartSetParity(gUartPort, rs485obj->parity, 1, 8);
		break;

    case ITP_IOCTL_RESET:
        RS485Reset(gUartPort);
        break;

    default:
        errno = -1;
        return -1;
    }
    return 0;
}


const ITPDevice itpDeviceRS485_0 =
{
    ":rs485_0",
    itpOpenDefault,
    itpCloseDefault,
    RS485Read,
    RS485Write,
    itpLseekDefault,
    RS485Ioctl,
    (void*)ITH_UART0
};

const ITPDevice itpDeviceRS485_1 =
{
    ":rs485_1",
    itpOpenDefault,
    itpCloseDefault,
    RS485Read,
    RS485Write,
    itpLseekDefault,
    RS485Ioctl,
    (void*)ITH_UART1
};

const ITPDevice itpDeviceRS485_2 =
{
    ":rs485_2",
    itpOpenDefault,
    itpCloseDefault,
    RS485Read,
    RS485Write,
    itpLseekDefault,
    RS485Ioctl,
    (void*)ITH_UART2
};

const ITPDevice itpDeviceRS485_3 =
{
    ":rs485_3",
    itpOpenDefault,
    itpCloseDefault,
    RS485Read,
    RS485Write,
    itpLseekDefault,
    RS485Ioctl,
    (void*)ITH_UART3
};

const ITPDevice itpDeviceRS485_4 =
{
    ":rs485_4",
    itpOpenDefault,
    itpCloseDefault,
    RS485Read,
    RS485Write,
    itpLseekDefault,
    RS485Ioctl,
    (void*)ITH_UART4
};

const ITPDevice itpDeviceRS485_5 =
{
    ":rs485_5",
    itpOpenDefault,
    itpCloseDefault,
    RS485Read,
    RS485Write,
    itpLseekDefault,
    RS485Ioctl,
    (void*)ITH_UART5
};




