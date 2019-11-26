#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "can_api.h"
#include "can_hw.h"
//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================
static pthread_mutex_t internal_mutex = PTHREAD_MUTEX_INITIALIZER;
static ITPPendFunction rx_pendfunction[2];
static ITPPendFunction tx_pendfunction[2];


//=============================================================================
//                Private Function Definition
//=============================================================================

static bool _CANSetWritePtr(CAN_HANDLE* base, CAN_TXOBJ* info)
{
    if(base->TTCAN_Enable)
    {
		uint32_t TBPTR_DATA = ithReadRegA((base->ADDR | CAN_VER_0_REG));
		TBPTR_DATA = (TBPTR_DATA & CAN_TBPTR) >> CAN_TBPTR_SHIFT;
		TBPTR_DATA = ((TBPTR_DATA > TBSLOT_BUFFER_MAX) ? 0 : (TBPTR_DATA + 1));//TBPTR++
		ithWriteRegMaskA((base->ADDR | CAN_VER_0_REG), TBPTR_DATA << CAN_TBPTR_SHIFT, CAN_TBPTR);
		return true;
    }
    else
    {   
		// Set TBSEL to PTB or STB
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), info->TBSEL << CAN_TBSEL_SHIFT, CAN_TBSEL);
		// IF SEL STB, need check STB is not full.
		if (info->TBSEL)
		{
			if (ithCANGetTBStatus(base) == BUFF_FULL){
				printf("tx stb buffer full\n");
				return false;
			}
		}
		return true;
    }   
}
static void _CANSendMsg(CAN_HANDLE* base, CAN_TXOBJ* info)
{
    if(base->TTCAN_Enable)
    {
		//set TBPTR buffer full
		ithWriteRegMaskA((base->ADDR | CAN_VER_0_REG), CAN_TBF, CAN_TBF);
		//write TBPTR to TTPTR
		uint32_t TBPTR_DATA = ithReadRegA((base->ADDR | CAN_VER_0_REG));
		TBPTR_DATA &= CAN_TBPTR;
		ithWriteRegMaskA((base->ADDR | CAN_TRIG_CFG_0_REG), TBPTR_DATA, CAN_TTPTR);
		//set trigger type
		ithTTCANSetTrig(base, TTCAN_Transmit_Start_Trigger);
    }
    else
    {
		if (info->TBSEL)
		{
			//STB BUFFER pointer next BUFFER
			ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_TSNEXT, CAN_TSNEXT);
			//STB SEND
			ithCANSecondarySendCtrl(base, 1);
		}
		else
		{
			ithCANPrimarySendCtrl(base, 1);
		}
    }
}

static void _CANIntrHandler(void* arg)
{
    CAN_HANDLE* base = (CAN_HANDLE*) arg;
    uint32_t flagstatus = ithCANGetIntrFlag(base);
    
    if (flagstatus & CAN_RIF)//Receive interrupt Flag
    {
        itpPendFunctionCallFromISR(rx_pendfunction[base->Instance],(void*)NULL,0);
        ithCANClearIntrFlag(base, Receive_Interrupt_Flag);
    }

    if (flagstatus & CAN_TPIF)//Transmission Primary Interrupt Flag
    {
        
        itpPendFunctionCallFromISR(tx_pendfunction[base->Instance],(void*)NULL,0);
        ithCANClearIntrFlag(base, TP_Interrupt_Flag);
    }

    if (flagstatus & CAN_TSIF)//Transmission Secondary Interrupt Flag
    {
        itpPendFunctionCallFromISR(tx_pendfunction[base->Instance],(void*)NULL,1);
        ithCANClearIntrFlag(base, TS_Interrupt_Flag);
    }
}

static void _TTCANIntrHandler(void* arg)
{
    CAN_HANDLE* base = (CAN_HANDLE*) arg;
    uint32_t flagstatus = ithTTCANGetIntrFlag(base);
    
    if (flagstatus & CAN_TTIF)//Time Trigger interrupt Flag
    {
        CAN_TRIGGER_TYPE type = (CAN_TRIGGER_TYPE)ithTTCANGetTrig(base);
        
        if(type == TTCAN_Time_Trigger)
            itpPendFunctionCallFromISR(rx_pendfunction[base->Instance],(void*)type,0);//return tigger type to callback function
        else if(type >= TTCAN_OneShot_Trans_Trigger && type <= TTCAN_Transmit_Stop_Trigger)
            itpPendFunctionCallFromISR(tx_pendfunction[base->Instance],(void*)type,0);//return tigger type to callback function
            
        ithCANClearIntrFlag(base, Time_Trigger_Interrupt_Flag);
    }

    if(flagstatus & CAN_TEIF)
    {
        ithCANClearIntrFlag(base, Trigger_Error_Interrupt_Flag);
    }
}

static void _CANEnableIrq(CAN_HANDLE* base, bool OnOff, ITHIntrHandler handler)
{
    if(base->Instance == 0)
    {
        #if 0 // can0
        ithIntrDisableIrq(ITH_INTR_CAN0);
        ithIntrClearIrq(ITH_INTR_CAN0);
        if(OnOff)
        {
            ithIntrSetTriggerModeIrq(ITH_INTR_CAN0, ITH_INTR_EDGE);
            ithIntrRegisterHandlerIrq(ITH_INTR_CAN0,  handler, (void*)base);
            //Enable the interrupts. 
            ithIntrEnableIrq(ITH_INTR_CAN0);
        }
        #endif
     }
     else
     {
        #if 0 // can1
        ithIntrDisableIrq(ITH_INTR_CAN1);
        ithIntrClearIrq(ITH_INTR_CAN1);
        if(OnOff)
        {
            ithIntrSetTriggerModeIrq(ITH_INTR_CAN1, ITH_INTR_EDGE);
            ithIntrRegisterHandlerIrq(ITH_INTR_CAN1,  handler, (void*)base);
            //Enable the interrupts. 
            ithIntrEnableIrq(ITH_INTR_CAN1);
        }
        #endif
     }

}

static void _CAN_CONFIG_20M_Nominal(CAN_HANDLE* base, CAN_BTATTR* s_bt)
{
    switch(base->BaudRate)
    {
		case CAN_250K_500K:
		case CAN_250K_833K:
		case CAN_250K_1M:
		case CAN_250K_1M5:
		case CAN_250K_2M:
		case CAN_250K_3M:
		case CAN_250K_4M:
            s_bt->Prescaler = 10 - 1;
            s_bt->Bit_Time = 80;
            s_bt->Seg_1 = 64 - 2;
            s_bt->Seg_2 = 16 - 1; 
            s_bt->SJW = 16 - 1;
            s_bt->SSPOFF = 0;
            break;
		case CAN_500K_1M:
		case CAN_500K_2M:
		case CAN_500K_3M:
		case CAN_500K_4M:
		case CAN_500K_5M:
            s_bt->Prescaler = 10 - 1;
            s_bt->Bit_Time = 40;
            s_bt->Seg_1 = 32 - 2;
            s_bt->Seg_2 = 8 - 1; 
            s_bt->SJW = 8 - 1;
			s_bt->SSPOFF = 0;
            break;
		case CAN_1000K_4M:
			s_bt->Prescaler = 10 - 1;
			s_bt->Bit_Time = 20;
			s_bt->Seg_1 = 16 - 2;
			s_bt->Seg_2 = 4 - 1;
			s_bt->SJW = 4 - 1;
			s_bt->SSPOFF = 0;
			break;
        default:
            break;
    }

}
static void _CAN_CONFIG_20M_Data(CAN_HANDLE* base, CAN_BTATTR* f_bt)
{
	switch (base->BaudRate)
    {
		case CAN_250K_500K:
            f_bt->Prescaler = 10 - 1;
            f_bt->Bit_Time = 40;
            f_bt->Seg_1 = 32 - 2;
            f_bt->Seg_2 = 8 - 1; 
            f_bt->SJW = 8 - 1;
			f_bt->SSPOFF = 0;
            break;
		case CAN_250K_833K:
            f_bt->Prescaler = 10 - 1;
            f_bt->Bit_Time = 24;
            f_bt->Seg_1 = 19 - 2;
            f_bt->Seg_2 = 5 - 1; 
            f_bt->SJW = 5 - 1;
			f_bt->SSPOFF = 0;
            break;
		case CAN_250K_1M:
		case CAN_500K_1M:
            f_bt->Prescaler = 10 - 1;
            f_bt->Bit_Time = 20;
            f_bt->Seg_1 = 16 - 2;
            f_bt->Seg_2 = 4 - 1; 
            f_bt->SJW = 4 - 1;
			f_bt->SSPOFF = 16;
            break; 
		case CAN_250K_1M5:
            f_bt->Prescaler = 10 - 1;
            f_bt->Bit_Time = 13;
            f_bt->Seg_1 = 10 - 2;
            f_bt->Seg_2 = 3 - 1; 
            f_bt->SJW = 3 - 1;
			f_bt->SSPOFF = 10;
            break; 
		case CAN_250K_2M:
		case CAN_500K_2M:
            f_bt->Prescaler = 10 - 1;
            f_bt->Bit_Time = 10;
            f_bt->Seg_1 = 8 - 2;
            f_bt->Seg_2 = 2 - 1; 
            f_bt->SJW = 2 - 1;
			f_bt->SSPOFF = 8;
        break;

		case CAN_250K_3M:
		case CAN_500K_3M:
			printf("[error]no support\n");
			break;
		case CAN_250K_4M:
		case CAN_500K_4M:
		case CAN_1000K_4M:
            f_bt->Prescaler = 10 - 1;
            f_bt->Bit_Time = 5;
            f_bt->Seg_1 = 4 - 2;
            f_bt->Seg_2 = 1 - 1; 
            f_bt->SJW = 1 - 1;
			f_bt->SSPOFF = 4;
        break; 
		case CAN_500K_5M:
            f_bt->Prescaler = 10 - 1;
            f_bt->Bit_Time = 4;
            f_bt->Seg_1 = 3 - 2;
            f_bt->Seg_2 = 1 - 1; 
            f_bt->SJW = 1 - 1;
			f_bt->SSPOFF = 3;
        break;
        default:
            break;        
    }

}
static void _CAN_CONFIG_40M_Nominal(CAN_HANDLE* base, CAN_BTATTR* s_bt)
{
	switch (base->BaudRate)
    {
		case CAN_250K_500K:
		case CAN_250K_833K:
		case CAN_250K_1M:
		case CAN_250K_1M5:
		case CAN_250K_2M:
		case CAN_250K_3M:
		case CAN_250K_4M:
			s_bt->Prescaler = 5 - 1;
            s_bt->Bit_Time = 80;
            s_bt->Seg_1 = 64 - 2;
            s_bt->Seg_2 = 16 - 1; 
            s_bt->SJW = 16 - 1;
			s_bt->SSPOFF = 0;
            break;
		case CAN_500K_1M:
		case CAN_500K_2M:
		case CAN_500K_3M:
		case CAN_500K_4M:
		case CAN_500K_5M:
			s_bt->Prescaler = 5 - 1;
            s_bt->Bit_Time = 80;
            s_bt->Seg_1 = 64 - 2;
            s_bt->Seg_2 = 16 - 1; 
            s_bt->SJW = 16 - 1;
			s_bt->SSPOFF = 0;
            break;
        default:
            break;
    }

}

static void _CAN_CONFIG_40M_Data(CAN_HANDLE* base, CAN_BTATTR* f_bt)
{
	switch (base->BaudRate)
    {
		case CAN_250K_500K:
            f_bt->Prescaler = 5 - 1;
            f_bt->Bit_Time = 40;
            f_bt->Seg_1 = 32 - 2;
            f_bt->Seg_2 = 8 - 1; 
            f_bt->SJW = 8 - 1;
			f_bt->SSPOFF = 0;
            break;
		case CAN_250K_833K:
            f_bt->Prescaler = 5 - 1;
            f_bt->Bit_Time = 24;
            f_bt->Seg_1 = 19 - 2;
            f_bt->Seg_2 = 5 - 1; 
            f_bt->SJW = 5 - 1;
			f_bt->SSPOFF = 0;
            break;
		case CAN_250K_1M:
		case CAN_500K_1M:
            f_bt->Prescaler = 5 - 1;
            f_bt->Bit_Time = 40;
            f_bt->Seg_1 = 32 - 2;
			f_bt->Seg_2 = 8 - 1;
			f_bt->SJW = 8 - 1;
			f_bt->SSPOFF = 32;
            break; 
		case CAN_250K_1M5:
            f_bt->Prescaler = 5 - 1;
            f_bt->Bit_Time = 26;
            f_bt->Seg_1 = 20 - 2;
            f_bt->Seg_2 = 6 - 1; 
            f_bt->SJW = 6 - 1;
			f_bt->SSPOFF = 20;
            break; 
		case CAN_250K_2M:
		case CAN_500K_2M:
			f_bt->Prescaler = 5 - 1;
            f_bt->Bit_Time = 20;
            f_bt->Seg_1 = 16 - 2;
            f_bt->Seg_2 = 4 - 1; 
            f_bt->SJW = 4 - 1;
			f_bt->SSPOFF = 16;
        break;
		case CAN_250K_3M:
		case CAN_500K_3M:
			f_bt->Prescaler = 5 - 1;
			f_bt->Bit_Time = 13;
			f_bt->Seg_1 = 10 - 2;
			f_bt->Seg_2 = 3 - 1;
			f_bt->SJW = 3 - 1;
			f_bt->SSPOFF = 10;
			break;
		case CAN_250K_4M:
		case CAN_500K_4M:
			f_bt->Prescaler = 5 - 1;
            f_bt->Bit_Time = 10;
            f_bt->Seg_1 = 8 - 2;
            f_bt->Seg_2 = 2 - 1; 
            f_bt->SJW = 2 - 1;
			f_bt->SSPOFF = 8;
        break; 
		case CAN_500K_5M:
			f_bt->Prescaler = 5 - 1;
            f_bt->Bit_Time = 8;
            f_bt->Seg_1 = 6 - 2;
            f_bt->Seg_2 = 2 - 1; 
            f_bt->SJW = 2 - 1;
			f_bt->SSPOFF = 6;
        break;
#if 0
        case 8000000u:
			f_bt->Prescaler = 0;
            f_bt->Bit_Time = 5;
            f_bt->Seg_1 = 4;
            f_bt->Seg_2 = 1; 
            f_bt->SJW = 1;
			f_bt->SSPOFF = 4;            
        break;
        case 10000000u:
			f_bt->Prescaler = 0;
            f_bt->Bit_Time = 4;
            f_bt->Seg_1 = 3;
            f_bt->Seg_2 = 1; 
            f_bt->SJW = 1;
			f_bt->SSPOFF = 3;            
        break;
#endif
        default:
            break;        
    }

}

static void _CAN_Bit_Timing_Calculator(CAN_HANDLE* base, CAN_BTATTR* s_bt, CAN_BTATTR* f_bt)
{    
    
    switch (base->SourceClock) {
        case CAN_SRCCLK_20M:
            _CAN_CONFIG_20M_Nominal(base, s_bt);
            _CAN_CONFIG_20M_Data(base, f_bt);
            break;
        case CAN_SRCCLK_40M:
            _CAN_CONFIG_40M_Nominal(base, s_bt);
            _CAN_CONFIG_40M_Data(base, f_bt);
            break;
        default:
            break;
    }
    printf("[S]Prescaler = %d, Bit_Time = %d, Seg1 = %d, Seg2 = %d, SJW = %d, SSPOFF =%d\n",
		s_bt->Prescaler, s_bt->Bit_Time, s_bt->Seg_1, s_bt->Seg_2, s_bt->SJW, s_bt->SSPOFF);
    printf("[F]Prescaler = %d, Bit_Time = %d, Seg1 = %d, Seg2 = %d, SJW = %d, SSPOFF=%d\n",
		f_bt->Prescaler, f_bt->Bit_Time, f_bt->Seg_1, f_bt->Seg_2, f_bt->SJW, f_bt->SSPOFF);
    
}

//=============================================================================
//                Public Function Definition
//=============================================================================
void ithCANOpen(CAN_HANDLE* base, void* rx_callback, void* tx_callback)
{
    pthread_mutex_lock(&internal_mutex);
    assert(base != NULL);
    
    CAN_BTATTR s_bt;
    CAN_BTATTR f_bt;


    if(rx_callback != NULL && base->Instance < 2)
        rx_pendfunction[base->Instance] = (ITPPendFunction)rx_callback;
    else
        printf("rx callback error\n");

    if(tx_callback != NULL && base->Instance < 2)
        tx_pendfunction[base->Instance]  = (ITPPendFunction)tx_callback;
    else
        printf("tx callback error\n");

    // Set Reset ON
    ithCANSWReset(base, true);

    // Set CAN-FD ISO or Non ISO
    ithCANSetFDISO(base);
    
    _CAN_Bit_Timing_Calculator(base, &s_bt, &f_bt);
    // BitRate setting
    ithCANSetBitRate(base, s_bt, f_bt);
    // Filter enable setting
    ithCANACFEnable(base, CAN_ACFE0, true);
    ithCANSetACODE(base, CAN_ACODE0 , 0x0);
    ithCANSetAMASK(base, CAN_AMASK0, 0xff, false, false);
    // Set Reset OFF
    ithCANSWReset(base, false);
	// LoopBack Mode Setting (for self test)
	ithCANSetLoopBack(base, base->ExternalLoopBackMode, base->InternalLoopBackMode);
	// Listen Only Mode Setting (for self test)
	ithCANSetListenOnlyMode(base, base->ListenOnlyMode);
	// Trans Standby Mode Setting (for self test)
	ithCANSetTransStandbyMode(base, base->TransceiverStandbyMode);

    if(base->TTCAN_Enable){
        // Enable TTCAN,and set trigger time
        ithTTCANCrtlON(base, 0x0, 0xffff);
        // Set TTCAN use reference id
		ithTTCANSetREFID(base, base->TTCAN_RefID, base->TTCAN_RefIDE);
        // Set trigger type
        //ithTTCANSetTrig(base, TTCAN_Time_Trigger);
        // Resister irq  and enable irq
		//temp closed
		#if 0
        _CANEnableIrq(base, true, _TTCANIntrHandler);
        ithCANEnableIntr(base, Time_Trigger_Enable);
		#endif
    }
    else{
		ithTTCANCrtlOFF(base);
        // Resister irq  and enable irq
		#if 0//temp closed
        _CANEnableIrq(base, true, _CANIntrHandler);
        ithCANEnableIntr(base, Receive_Enable);
		ithCANEnableIntr(base, TS_Enable);
		ithCANEnableIntr(base, TP_Enable);
		#endif
	}   
	//open time stamp
	//ithCANSetCIA603(base, true, 0x0);
	//printf("0xbc = %x \n", ithReadRegA((base->ADDR | CAN_VER_0_REG)));
	//printf("0xa0 = %x \n", ithReadRegA((base->ADDR | CAN_CFG_STAT_REG)));
    pthread_mutex_unlock(&internal_mutex);
}

void ithCANClose(CAN_HANDLE* base)
{
    pthread_mutex_lock(&internal_mutex);
    
    _CANEnableIrq(base, false, NULL);
    
    if(base->TTCAN_Enable){
        ithCANDisableIntr(base, Time_Trigger_Enable);
    }
    else{
        ithCANDisableIntr(base, Receive_Enable);
        ithCANDisableIntr(base, TP_Enable);
        ithCANDisableIntr(base, TS_Enable);        
    }
    
    ithCANSWReset(base, true);

    pthread_mutex_unlock(&internal_mutex);
}

int ithCANRead(CAN_HANDLE* base, CAN_RXOBJ* info)
{
    pthread_mutex_lock(&internal_mutex);
    int result = 0;
    
    // Check RBUF status
    if(ithCANGetRBStatus(base)!= BUFF_EMPTY){
        CAN_RXOBJ RX = {0};
        uint32_t i = 0;
        uint32_t lenof4byte = 0;
        uint32_t control = 0;
        uint32_t id = 0;
        // Read control info
        control = ithReadRegA((base->ADDR | CAN_RB_CONTROL_REG));
        RX.Control.DLC = control & CAN_RB_DLC;
        RX.Control.BRS = (control & CAN_RB_BRS)  >> CAN_RB_BRS_SHIFT;
        RX.Control.EDL = (control & CAN_RB_EDL)  >> CAN_RB_EDL_SHIFT;
        RX.Control.RTR = (control & CAN_RB_RTR)  >> CAN_RB_RTR_SHIFT;
        RX.Control.IDE = (control & CAN_RB_IDE)  >> CAN_RB_IDE_SHIFT;
        RX.Status.TX   = (control & CAN_RB_TX)   >> CAN_RB_TX_SHIFT;
        RX.Status.KOER = (control & CAN_RB_KOER) >> CAN_RB_KOER_SHIFT;
        // Read identifier 
        id = ithReadRegA((base->ADDR | CAN_RB_IDENTIFIER_REG));
              
        RX.Identifier = (RX.Control.IDE ? (id & 0xFFFFFFF) : (id & N11_BITS_MSK));

        if(RX.Status.KOER == CAN_NO_ERROR){
           lenof4byte = Alignment4BYTE(ithCANDlcToBytes(RX.Control.DLC));
           // Copy data from RBuffer reg
           for(i = 0; i < lenof4byte; i++){
                unsigned int tmp = ithReadRegA(base->ADDR | (CAN_RB_DATA0_REG + i*4));
                memcpy(&RX.RXData[i*4] ,&tmp, sizeof(uint8_t) * 4);
           }
		   //time stamp(CIA603)
		   //RX.RXRTS[0] = ithReadRegA(base->ADDR | CAN_RB_RTS0_REG);
		   //RX.RXRTS[1] = ithReadRegA(base->ADDR | CAN_RB_RTS1_REG);

           memcpy(info, &RX, sizeof(CAN_RXOBJ));
        }
        else{
            printf("Read Error code = %x \n",RX.Status.KOER);
            result = 1;
        }
        // Release this RXbuf
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_RREL, CAN_RREL);
    }
    else{
        printf("RBUF is empty\n");
        result = 1;
    }
    
    pthread_mutex_unlock(&internal_mutex);
    // Return 0-read success, 1-read fail
    return result; 
}

int ithCANWrite(CAN_HANDLE* base, CAN_TXOBJ* info, uint8_t* dataptr)
{
    pthread_mutex_lock(&internal_mutex);
    uint8_t txbuffer[DLC_MAX];
    uint32_t *txptr;
    uint32_t  control = 0;
    uint32_t lenNumBytes = 0;
    uint32_t lenNumWords = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint16_t n = 0;

	if (!_CANSetWritePtr(base, info))
	{
		pthread_mutex_unlock(&internal_mutex);
		return 1;
	}
		
    // Set TB IDENTIFIER
    if(info->Control.IDE)
        ithWriteRegA((base->ADDR | CAN_TB_IDENTIFIER_REG), (info->Id & 0x1FFFFFFF) | info->TTSENSEL << 31);
    else
        ithWriteRegA((base->ADDR | CAN_TB_IDENTIFIER_REG), (info->Id & 0x7FF) | info->TTSENSEL << 31);

    // Set TB Control
    control |=  (info->Control.DLC & 0xF);// Set payload  length(DLC)
    control |=  info->Control.BRS << CAN_TB_BRS_SHIFT;
    control |=  info->Control.EDL << CAN_TB_EDL_SHIFT;
    control |=  info->Control.RTR << CAN_TB_RTR_SHIFT;
    control |=  info->Control.IDE << CAN_TB_IDE_SHIFT;
    ithWriteRegA((base->ADDR | CAN_TB_CONTROL_REG), control);
    
    lenNumBytes = ithCANDlcToBytes(info->Control.DLC);
    // Copy data to txbuffer
    memcpy(&txbuffer, dataptr, sizeof(uint8_t)* lenNumBytes);

    // Make sure we write a multiple of 4 bytes to RAM
    if (lenNumBytes % 4) {
        // Need to add bytes
        n = 4 - (lenNumBytes % 4);
        i = lenNumBytes + 8;

        for (j = 0; j < n; j++) {
            txbuffer[i + 8 + j] = 0;
        }
    }
    // Bytes to Words
    lenNumWords = Alignment4BYTE(lenNumBytes);
        
    for(i = 0 ; i < lenNumWords ; i++){
        txptr = (uint32_t*)&txbuffer[i*4];
        ithWriteRegA((base->ADDR | (CAN_TB_DATA0_REG + i*4)), *txptr);
    }

    // CAN ctrl send Msg to bus
    _CANSendMsg(base, info);
    
    pthread_mutex_unlock(&internal_mutex);
    // Return 0-write success, 1-write fail
    return 0;
}
uint32_t ithCANDlcToBytes(CAN_DLC dlc)
{
    uint32_t dataBytesInObject = 0;

    if (dlc < CAN_DLC_12) {
        dataBytesInObject = dlc;
    } else {
        switch (dlc) {
            case CAN_DLC_12:
                dataBytesInObject = 12;
                break;
            case CAN_DLC_16:
                dataBytesInObject = 16;
                break;
            case CAN_DLC_20:
                dataBytesInObject = 20;
                break;
            case CAN_DLC_24:
                dataBytesInObject = 24;
                break;
            case CAN_DLC_32:
                dataBytesInObject = 32;
                break;
            case CAN_DLC_48:
                dataBytesInObject = 48;
                break;
            case CAN_DLC_64:
                dataBytesInObject = 64;
                break;
            default:
                break;
        }
    }
    return dataBytesInObject;
}


