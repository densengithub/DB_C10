//****************************
/*TEST CANBUS 
/*VERSION  V0.1
/*MODULE   MCP2517FD click board
*****************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ite/ith.h"
#include "ite/itp.h"
#include "2517FD/can_api.h"


/******************************************/
/*                        LOCAL DEFINE                      */
/*****************************************/
#define STARTMODE APP_STATE_RECEIVE
//#define STARTMODE APP_STATE_TRANSMIT
//#define STARTMODE APP_STATE_TEST_RAM_ACCESS

/******************************************/
/*                        LOCAL STRUCT                     */
/*****************************************/

/******************************************/
/*                        LOCAL VALUE                       */
/*****************************************/
APP_DATA canData;

// Transmit objects
CAN_TX_MSGOBJ txObj;
uint8_t txd[MAX_DATA_BYTES];

// Receive objects
CAN_RX_MSGOBJ rxObj;
uint8_t rxd[MAX_DATA_BYTES];

bool SLEEP_TEST_FLAG = false;

/******************************************/
/*                        LOCAL FUN                          */
/*****************************************/


void* TestFunc1(void* arg)
{
    printf("========================================\n");
	printf("CAN BUS TEST\n");
    printf("MCP2517FD click board\n");
    printf("========================================\n");
    itpInit();
    uint32_t i = 0;
    uint32_t send_count = 0;

    canData.state = APP_STATE_INIT;
    
    /* Init SPI driver */
    DRV_SPI_Initialize(0); 
    while(1)
    {
        /* Check the application's current state. */
        switch (canData.state) {
                /* Application's initial state. */
            case APP_STATE_INIT:
            {
                Nop();
                ithCANOpen(CAN_500K_1M, CAN_SYSCLK_40M);            

                canData.state = APP_STATE_INIT_TXOBJ;
                break;
            }

                /* Initialize TX Object */
            case APP_STATE_INIT_TXOBJ:
            {
                /* Configure transmit message */
                txObj.word[0] = 0;
                txObj.word[1] = 0;

                txObj.bF.id.SID = 0x01;
                txObj.bF.id.EID = 0x00;
                
                #if 1/* CAN 2.0B TX */
                txObj.bF.ctrl.BRS = 0;         //bit rate switch, 2.0B BRS off
                txObj.bF.ctrl.DLC = CAN_DLC_8; //Data Length Code 2.0B  = 8
                txObj.bF.ctrl.FDF = 0;         //identifier FD frame
                txObj.bF.ctrl.IDE = 1;         //identifier Ex
                #else/* CAN FD TX */
                txObj.bF.ctrl.BRS = 1;         //bit rate switch, FD on
                txObj.bF.ctrl.DLC = CAN_DLC_64; //Data Length Code 
                txObj.bF.ctrl.FDF = 1;         //identifier FD frame
                txObj.bF.ctrl.IDE = 1;         //identifier Ex               
                #endif
                /* Init message data */
                for (i = 0; i < MAX_DATA_BYTES; i++) txd[i] = 0;
                // Delay loop
                Nop();
                Nop();

                if(SLEEP_TEST_FLAG){
                    canData.state = APP_STATE_TSET_SLEEP;
                    SLEEP_TEST_FLAG = false;
                }
                else
                    canData.state = STARTMODE;

                break;
            }
            
                /* Send TX Object */
            case APP_STATE_TRANSMIT:
            {
                for (i = 0; i < MAX_DATA_BYTES; i++) 
                { 
                    txd[i] = send_count;
                }
                ithCANWrite(&txObj, txd);
                printf("[*]SEND MSG\n");
                send_count++;
				sleep(1);
                
                break;
            }
            
                /* Receive a message */
            case APP_STATE_RECEIVE:
            {
                if(ithCANRead(&rxObj, rxd))
                {
                    // print receive data info
                    if(rxObj.bF.ctrl.FDF)
                        printf("[*]Rec FD,EID=0x%x,SID=0x%x,IDExtended=%x\n",rxObj.bF.id.EID, (rxObj.bF.id.SID & rxObj.bF.id.SID11 << 11), rxObj.bF.ctrl.IDE);
                    else
                        printf("[*]Rec 2B,EID=0x%x,SID=0x%x,IDExtended=%x\n",rxObj.bF.id.EID, rxObj.bF.id.SID, rxObj.bF.ctrl.IDE); 
                    
                    printf("[*]DLC=%x, TS = %x\n",rxObj.bF.ctrl.DLC, rxObj.bF.timeStamp);
                    
                    for(i = 0; i < ithCANDlcToDataBytes(rxObj.bF.ctrl.DLC); i++){
                        printf("rxd[%d] = %x\n",i,rxd[i]);
                    }
                }
                             
                /*loop receive*/
                canData.state = APP_STATE_RECEIVE;
                break;
            }

            case APP_STATE_TSET_SLEEP:
            {
                ithCANSleepMode();
                i = 0;
                //sleep 10s
                while(i < 10){ i++; sleep(1); printf("[*]In Sleep Mode\n");}
                
                canData.state = APP_STATE_INIT;
                break;
            }
                /* RAM access test */
            case APP_STATE_TEST_RAM_ACCESS:
            {
                bool passed = ithCANTestRamAccess();

                printf("[*]RAM ACCESS Result = %x \n",passed);
                if (!passed) {
                    Nop();
                    Nop();
                    Nop();
                    Nop();
                    Nop();
                }

                canData.state = APP_STATE_TEST_RAM_ACCESS;

                break;
            }
            #if 0
            case APP_STATE_PAYLOAD:
            {
                Nop();
                Nop();
                //APP_PayLoad_Tasks();
                canData.state = APP_STATE_SWITCH_CHANGED;
                break;
            }

            case APP_STATE_SWITCH_CHANGED:
            {
                //APP_TransmitSwitchState();
                canData.state = APP_STATE_RECEIVE;

                break;
            }

                /* Register access test */
            case APP_STATE_TEST_REGISTER_ACCESS:
            {
                //bool passed = APP_TestRamAccess();
                bool passed =true;

                if (!passed) {
                    Nop();
                }

                canData.state = APP_STATE_TEST_RAM_ACCESS;

                break;
            }

                /* Flash all LEDs */
            case APP_STATE_FLASH_LEDS:
            {
                // Delay loop
                Nop();
                Nop();

                // Delay expired, update LEDs and reset delayCount

                if (ledCount > (APP_N_LED + 1)) {
                    canData.state = APP_STATE_RECEIVE;
                } else {
                    canData.state = APP_STATE_FLASH_LEDS;
                }

                break;
            }
            #endif

                /* The default state should never be executed. */
            default:
            {
                /* TODO: Handle error in application's state machine. */
                Nop();
                Nop();
                canData.state = APP_STATE_INIT;

                break;
            }
        }
    }

}

