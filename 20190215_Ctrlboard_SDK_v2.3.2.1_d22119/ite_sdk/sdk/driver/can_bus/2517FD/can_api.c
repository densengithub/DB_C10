#include "can_api.h"

static pthread_mutex_t internal_mutex = PTHREAD_MUTEX_INITIALIZER;

bool _RX_INT()
{
    return (bool)!ithGpioGet(RX_INT);
}

void ithCANOpen(CAN_BITTIME_SETUP bitTime, CAN_SYSCLK_SPEED clk)
{
    //Device
    CAN_CONFIG config;
    //TX 
    CAN_TX_FIFO_CONFIG txConfig;
    CAN_TEF_CONFIG     tefConfig;
    //RX
    CAN_RX_FIFO_CONFIG rxConfig;
    REG_CiFLTOBJ fObj;
    REG_CiMASK mObj;
    
    pthread_mutex_lock(&internal_mutex);

    // Interrupts Gpio Setting
    ithGpioSetMode(GEN_INT, ITH_GPIO_MODE0);
    ithGpioSetIn(GEN_INT);
    
    ithGpioSetMode(RX_INT, ITH_GPIO_MODE0);
    ithGpioSetIn(RX_INT);

    ithGpioSetMode(TX_INT, ITH_GPIO_MODE0);
    ithGpioSetIn(TX_INT);

    ithGpioSetMode(CAN_STBY, ITH_GPIO_MODE0);
    ithGpioSetOut(CAN_STBY);
    ithGpioClear(CAN_STBY);
    
    //OSC on
    DRV_CANFDSPI_OscillatorEnable(DRV_CANFDSPI_INDEX_0);

    // DEVICE RESET
    DRV_CANFDSPI_Reset(DRV_CANFDSPI_INDEX_0);
    
    // Enable ECC and initialize RAM
    DRV_CANFDSPI_EccEnable(DRV_CANFDSPI_INDEX_0);

    DRV_CANFDSPI_RamInit(DRV_CANFDSPI_INDEX_0, 0xff);

    // Configure device
    DRV_CANFDSPI_ConfigureObjectReset(&config);
    config.IsoCrcEnable = 1;
    config.StoreInTEF = 0;

    DRV_CANFDSPI_Configure(DRV_CANFDSPI_INDEX_0, &config);

    // Setup TX FIFO
    DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig);
    txConfig.FifoSize = 7;
    txConfig.PayLoadSize = CAN_PLSIZE_64;
    txConfig.TxPriority = 1;

    DRV_CANFDSPI_TransmitChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txConfig);
    
    #ifdef APP_USE_TIME_STAMP
    tefConfig.TimeStampEnable = 0x1;
    DRV_CANFDSPI_TefConfigure(DRV_CANFDSPI_INDEX_0, &tefConfig);
    #endif

    // Setup RX FIFO
    DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig);
    rxConfig.FifoSize = 15;
    rxConfig.PayLoadSize = CAN_PLSIZE_64;
    #ifdef APP_USE_TIME_STAMP
    rxConfig.RxTimeStampEnable = 0x1;
    #endif

    DRV_CANFDSPI_ReceiveChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxConfig);

    // Setup RX Filter
    fObj.word = 0x00;
    fObj.bF.SID = 0x01;  //Filter setting for SID
    fObj.bF.EXIDE = 0;
    fObj.bF.EID = 0x00;

    DRV_CANFDSPI_FilterObjectConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &fObj.bF);

    // Setup RX Mask
    mObj.word = 0;
    mObj.bF.MSID = 0x0; //SID MASK
    mObj.bF.MIDE = 0;   // 1:Only allow standard IDs 0:allow standard & ex
    mObj.bF.MEID = 0x0;
    DRV_CANFDSPI_FilterMaskConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &mObj.bF);

    // Link FIFO and Filter
    DRV_CANFDSPI_FilterToFifoLink(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, APP_RX_FIFO, true);

    // Setup Bit Time
    //DRV_CANFDSPI_BitTimeConfigure(DRV_CANFDSPI_INDEX_0, CAN_500K_2M, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);
    DRV_CANFDSPI_BitTimeConfigure(DRV_CANFDSPI_INDEX_0, bitTime, CAN_SSP_MODE_AUTO, clk);

    // Setup Transmit and Receive Interrupts
    DRV_CANFDSPI_GpioModeConfigure(DRV_CANFDSPI_INDEX_0, GPIO_MODE_INT, GPIO_MODE_INT);
	#ifdef APP_USE_TX_INT
    DRV_CANFDSPI_TransmitChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, CAN_TX_FIFO_NOT_FULL_EVENT);
	#endif
    DRV_CANFDSPI_ReceiveChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
    DRV_CANFDSPI_ModuleEventEnable(DRV_CANFDSPI_INDEX_0, CAN_TX_EVENT | CAN_RX_EVENT);

    #ifdef APP_USE_TIME_STAMP
    //Time stamp setting
    DRV_CANFDSPI_TimeStampDisable(DRV_CANFDSPI_INDEX_0);
    //TBC increments 1 every 1us (SYSCLK 40MHZ  set 39 = 40 cycles count 1)
    DRV_CANFDSPI_TimeStampPrescalerSet(DRV_CANFDSPI_INDEX_0,39);
    DRV_CANFDSPI_TimeStampModeConfigure(DRV_CANFDSPI_INDEX_0,CAN_TS_SOF);
    DRV_CANFDSPI_TimeStampSet(DRV_CANFDSPI_INDEX_0, 0);
    DRV_CANFDSPI_TimeStampEnable(DRV_CANFDSPI_INDEX_0);
    #endif
    // Select Normal Mode
    DRV_CANFDSPI_OperationModeSelect(DRV_CANFDSPI_INDEX_0, CAN_NORMAL_MODE);
    Nop();
    printf("[*]CANFDSPI MODE = %d\n",DRV_CANFDSPI_OperationModeGet(DRV_CANFDSPI_INDEX_0));
    pthread_mutex_unlock(&internal_mutex);

}

bool ithCANRead(CAN_RX_MSGOBJ* rxObj, uint8_t *rxd)
{
    bool received = false;
    uint32_t i = 0;
    CAN_RX_FIFO_EVENT rxFlags;

    Nop();
    Nop();
    pthread_mutex_lock(&internal_mutex);
    // Check if FIFO is not empty
#ifdef APP_USE_RX_INT
    if (_RX_INT()) {
#else
    DRV_CANFDSPI_ReceiveChannelEventGet(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxFlags);

    if (rxFlags & CAN_RX_FIFO_NOT_EMPTY_EVENT) {
#endif
        // Get message
        DRV_CANFDSPI_ReceiveMessageGet(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, rxObj, rxd, MAX_DATA_BYTES);
        received = true;
    }
    pthread_mutex_unlock(&internal_mutex);

    return received;
}

void ithCANWrite(CAN_TX_MSGOBJ* txObj, uint8_t *txd)
{
    uint8_t tec;
    uint8_t rec;
    CAN_ERROR_STATE errorFlags;
    CAN_TX_FIFO_EVENT txFlags;

    pthread_mutex_lock(&internal_mutex);

    uint8_t attempts = MAX_TXQUEUE_ATTEMPTS;

    // Check if FIFO is not full
    do {
        DRV_CANFDSPI_TransmitChannelEventGet(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txFlags);
        if (attempts == 0) {
            Nop();
            Nop();
            DRV_CANFDSPI_ErrorCountStateGet(DRV_CANFDSPI_INDEX_0, &tec, &rec, &errorFlags);
            return;
        }
        attempts--;
    }
    while (!(txFlags & CAN_TX_FIFO_NOT_FULL_EVENT));

    // Load message and transmit
    uint8_t n = DRV_CANFDSPI_DlcToDataBytes(txObj->bF.ctrl.DLC);

    DRV_CANFDSPI_TransmitChannelLoad(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, txObj, txd, n, true);
#if 0//DEBUG MSG
    DRV_CANFDSPI_ErrorCountStateGet(DRV_CANFDSPI_INDEX_0, &tec, &rec, &errorFlags);
    printf("tec = %d, rec = %d, errorFlags = %x\n", tec, rec, errorFlags);
    //DRV_CANFDSPI_TransmitQueueStatusGet(DRV_CANFDSPI_INDEX_0,&status);
    //printf("status = %x\n",status);
#endif
    pthread_mutex_unlock(&internal_mutex);

}

bool ithCANTestRamAccess(void)
{
    pthread_mutex_lock(&internal_mutex);
    // Variables
    uint8_t length;
    bool good = false;
    uint32_t i = 0;
    uint8_t ttxd[MAX_DATA_BYTES];
    uint8_t rrxd[MAX_DATA_BYTES];

    Nop();

    // Verify read/write with different access length
    // Note: RAM can only be accessed in multiples of 4 bytes
    for (length = 4; length <= MAX_DATA_BYTES; length += 4) {
        for (i = 0; i < length; i++) {
            ttxd[i] = rand() & 0xff;
            rrxd[i] = 0xff;
        }

        Nop();

        // Write data to RAM
        DRV_CANFDSPI_WriteByteArray(DRV_CANFDSPI_INDEX_0, cRAMADDR_START, ttxd, length);

        // Read data back from RAM
        DRV_CANFDSPI_ReadByteArray(DRV_CANFDSPI_INDEX_0, cRAMADDR_START, rrxd, length);

        // Verify
        good = false;
        for (i = 0; i < length; i++) {
            good = ttxd[i] == rrxd[i];

            if (!good) {
                Nop();
                Nop();

                // Data mismatch
                return false;
            }
        }
    }
    pthread_mutex_unlock(&internal_mutex);

    return true;
}

bool ithCANSleepMode(void)
{
    CAN_OSC_STATUS status;
    pthread_mutex_lock(&internal_mutex);
    DRV_CANFDSPI_OperationModeSelect(DRV_CANFDSPI_INDEX_0, CAN_SLEEP_MODE);
    Nop();
    DRV_CANFDSPI_OscillatorStatusGet(DRV_CANFDSPI_INDEX_0, &status);

    printf("[*]OSC Ready = %x\n",status.OscReady);

    pthread_mutex_unlock(&internal_mutex);
}

uint32_t ithCANDlcToDataBytes(CAN_DLC dlc)
{
    return DRV_CANFDSPI_DlcToDataBytes(dlc);
}

CAN_DLC ithCANDataBytesToDlc(uint8_t n)
{
    return DRV_CANFDSPI_DataBytesToDlc(n);
}


