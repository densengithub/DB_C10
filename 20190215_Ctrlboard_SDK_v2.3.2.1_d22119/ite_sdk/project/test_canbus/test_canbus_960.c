/*
 * Copyright (c) 2018 ITE Tech. Inc. All Rights Reserved.
 *
 * @brief Can ctrl test code.
 * @author 
 * @version 1.0.0
 *
 */
#include <stdio.h>
#include <malloc.h>
#include "ite/itp.h"	//for all ith driver
#include "ite/ith.h"
#include "can_bus/it960/can_api.h"


CAN_HANDLE* can0;

static pthread_mutex_t txmutex = PTHREAD_MUTEX_INITIALIZER;

static void CANRXCallback(void* arg1, uint32_t arg2)
{
    printf("rx interrupt cb\n");
}
static void CANTXCallback(void* arg1, uint32_t arg2)
{
    printf("tx interrupt cb. type(%d)\n", arg2);
}

void* TestFunc1(void* arg)
{
    uint8_t txbuffer[8] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
    
    itpInit();
    printf("test 960 canbus!\n");

	ithWriteRegA(0XD1000258, 0x800c800b);//CAN0 GPIO  RX = 12 TX = 11

	//define can0
    can0 = (CAN_HANDLE*)malloc(sizeof(CAN_HANDLE));
    can0->Instance = 0; 
	can0->ADDR = CAN0_BASE_ADDRESS;
	can0->BaudRate = CAN_1000K_4M;
    can0->SourceClock =  CAN_SRCCLK_20M;
    can0->ProtocolType = protocol_FD_ISO;
    can0->TTCAN_Enable = false;//disable TTCAN
    can0->TTCAN_RefID = 0x0;
	can0->TTCAN_RefIDE = 0x0;
	can0->ExternalLoopBackMode = false;
	can0->InternalLoopBackMode = false;
    can0->TransceiverStandbyMode = false;
    can0->ListenOnlyMode   = false;
    can0->TPSingleShotMode = false;
    can0->TSSingleShotMode = false;

    //try open & init can ctrl
    ithCANOpen(can0, (void*)CANRXCallback, (void*)CANTXCallback);

    //tx information
    CAN_TXOBJ txinfo;
    txinfo.Id = 0x2;
    txinfo.Control.BRS = 0x0;
    txinfo.Control.DLC = CAN_DLC_8;
    txinfo.Control.EDL = 0x0;
    txinfo.Control.RTR = 0x0;
    txinfo.Control.IDE = 0x0;
    txinfo.TBSEL = 0x0;
    txinfo.TTSENSEL = 0x0;
    
    //main loop
	while(1)
	{   
		uint32_t i;
		#if 0
		uint32_t r = ithCANWrite(can0, &txinfo, txbuffer);
		printf("can0 send data r = %x\n",r);
		//check error count
		printf("error count = %d\n", ithCANGetTransmitErrorCouNT(can0));
        printf("kind of error = %x\n", ithCANGetKindOfError(can0));
		#else
        CAN_RXOBJ rxObj;

        if(ithCANRead(can0, &rxObj) == 0)
        {
			printf("\nRead from[%x]: ", rxObj.Identifier);
			
			printf("rxObj IDE = %x, EDL = %x , DLC = %x\n", rxObj.Control.IDE, rxObj.Control.EDL, rxObj.Control.DLC);

			printf("[ ");
            for(i = 0; i < ithCANDlcToBytes(rxObj.Control.DLC); i++)
            {
                printf(" %x,",rxObj.RXData[i]);  
            }
			printf("t0 = %x, t1 = %x ", rxObj.RXRTS[0], rxObj.RXRTS[1]);
			printf(" ]\n");
        }
		#endif
		sleep(1);
	}

	return NULL;
}
