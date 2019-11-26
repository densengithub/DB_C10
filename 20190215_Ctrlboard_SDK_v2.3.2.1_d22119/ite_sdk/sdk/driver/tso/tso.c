/*
 * Copyright (c) 2010 ITE technology Corp. All Rights Reserved.
 */
/** @file tso.c
 * Used to receive data through the transport stream output interface (TSO).
 *
 * @author
 * @version 0.1
 */
//=============================================================================
//                              Include Files
//=============================================================================

#include "mmp_tso.h"
#include "pthread.h"
#include "ite/ith.h"
#include "ite/itp.h"

//=============================================================================
//                              Constant Definition
//=============================================================================
#if (CFG_CHIP_FAMILY == 970)
    #define TSO_BASE_REGISTER               0xD0600000
#elif (CFG_CHIP_FAMILY == 9850)
    #define TSO_BASE_REGISTER               0x2200
#else
    #define TSO_BASE_REGISTER               0x2100
#endif
//Define Register
#if (CFG_CHIP_FAMILY == 970)
    #define TSO_START_MEM_BASE_ADDR_REG1    (TSO_BASE_REGISTER + 0X0000)
    #define TSO_START_MEM_BASE_ADDR_REG2    (TSO_BASE_REGISTER + 0x0004)
    #define TSO_END_MEM_BASE_ADDR_REG1      (TSO_BASE_REGISTER + 0x0008)
    #define TSO_END_MEM_BASE_ADDR_REG2      (TSO_BASE_REGISTER + 0x000c)
    #define TSO_WRITE_LEN_REG1              (TSO_BASE_REGISTER + 0x0010)
    #define TSO_WRITE_LEN_REG2              (TSO_BASE_REGISTER + 0x0014)
    #define TSO_WRITE_CTRL_REG              (TSO_BASE_REGISTER + 0x0018)
    #define TSO_VIDEO_PID_REG               (TSO_BASE_REGISTER + 0x001C)
    #define TSO_INTERFACE_CTRL_REG          (TSO_BASE_REGISTER + 0x0020)
    #define PCR_CLOCK_REG                   (TSO_BASE_REGISTER + 0x0024)
    #define PCR_BASE_INIT_VAL_REG1          (TSO_BASE_REGISTER + 0x0028)
    #define PCR_BASE_INIT_VAL_REG2          (TSO_BASE_REGISTER + 0x002C)
    #define PCR_EXT_INIT_VAL_REG            (TSO_BASE_REGISTER + 0x0030)
    #define TS_INTERNAL_CTRL_REG1           (TSO_BASE_REGISTER + 0x0034)
    #define TS_NULL_PKT_DATA                (TSO_BASE_REGISTER + 0x0038)
    #define TSO_FIRE_REG                    (TSO_BASE_REGISTER + 0x003C)
    #define TS_READ_LEN_REG1                (TSO_BASE_REGISTER + 0x0040)
    #define TS_READ_LEN_REG2                (TSO_BASE_REGISTER + 0x0044)
    #define TSO_ENG_STATUS_REG              (TSO_BASE_REGISTER + 0x0048)
    #define PCR_BASE_CNT1                   (TSO_BASE_REGISTER + 0x004C)
    #define PCR_BASE_CNT2                   (TSO_BASE_REGISTER + 0x0050)
    #define PCR_EXT_CNT                     (TSO_BASE_REGISTER + 0x0054)

    #define TSO_CLOCK_SETTING_REG           (ITH_HOST_BASE + 0x005C)
    #define TSO_CLOCK_RESET_BIT_OFFSET      31
#else
    #define TSO_START_MEM_BASE_ADDR_REG1    (TSO_BASE_REGISTER + 0X00)
    #define TSO_START_MEM_BASE_ADDR_REG2    (TSO_BASE_REGISTER + 0x02)
    #define TSO_END_MEM_BASE_ADDR_REG1      (TSO_BASE_REGISTER + 0x04)
    #define TSO_END_MEM_BASE_ADDR_REG2      (TSO_BASE_REGISTER + 0x06)
    #define TSO_WRITE_LEN_REG1              (TSO_BASE_REGISTER + 0x08)
    #define TSO_WRITE_LEN_REG2              (TSO_BASE_REGISTER + 0x0A)
    #define TSO_WRITE_CTRL_REG              (TSO_BASE_REGISTER + 0x0C)
    #define TSO_VIDEO_PID_REG               (TSO_BASE_REGISTER + 0x0E)
    #define TSO_INTERFACE_CTRL_REG          (TSO_BASE_REGISTER + 0x10)
    #define PCR_CLOCK_REG                   (TSO_BASE_REGISTER + 0x12)
    #define PCR_BASE_INIT_VAL_REG1          (TSO_BASE_REGISTER + 0x14)
    #define PCR_BASE_INIT_VAL_REG2          (TSO_BASE_REGISTER + 0x16)
    #define PCR_EXT_INIT_VAL_REG            (TSO_BASE_REGISTER + 0x18)
    #define TS_INTERNAL_CTRL_REG1           (TSO_BASE_REGISTER + 0x1A)
    #define TS_INTERNAL_CTRL_REG2           (TSO_BASE_REGISTER + 0x1C)
    #define TSO_FIRE_REG                    (TSO_BASE_REGISTER + 0x1E)
    #define TS_READ_LEN_REG1                (TSO_BASE_REGISTER + 0x20)
    #define TS_READ_LEN_REG2                (TSO_BASE_REGISTER + 0x22)
    #define TSO_ENG_STATUS_REG              (TSO_BASE_REGISTER + 0x24)

    #define TSO_CLOCK_SETTING_REG           (0x005C)
    #define TSO_CLOCK_RESET_BIT_OFFSET      10
#endif

//=============================================================================
//                              Structure Definition
//=============================================================================

typedef struct TSO_MODULE_TAG
{
    pthread_mutex_t tMgrMutex;
    MMP_UINT8       *pAllocAddr;
    MMP_UINT32      baseAddr;
    MMP_UINT32      videoPid;
    MMP_FLOAT       pcrClock;
    MMP_UINT32      pcrInitValue;
    MMP_BOOL        bInjectPcr;
    MMP_INT32       bufferSize;
    MMP_UINT32      writeIndex;
    MMP_UINT32      outByteRate;
} TSO_MODULE;

//=============================================================================
//                              Global Data Definition
//=============================================================================

static TSO_MODULE gtTso = { 0 };

//=============================================================================
//                              Private Function Declaration
//=============================================================================

void
_TSO_SetPadSel(
    MMP_UINT32 startPort);

MMP_FLOAT
_TSO_GetPcrClock(
    void);

//=============================================================================
//                              Public Function Definition
//=============================================================================
MMP_RESULT
mmpTsoInitialize(
    MMP_UINT32  padStart,
    MMP_UINT32  videoPid,
    MMP_FLOAT   pcrClock,
    MMP_UINT32  startPcrValue,
    MMP_UINT8   *pExternalBuffer,
    MMP_INT32   bufferSize,
    MMP_BOOL    bInjectPcr)
{
    MMP_RESULT  result              = MMP_SUCCESS;
    MMP_FLOAT   calculatePcrClock   = 0;
    MMP_INT32   pcrInteger          = (MMP_INT32) pcrClock;
    MMP_FLOAT   pcrDecimal          = (MMP_FLOAT) (pcrClock - pcrInteger);
    MMP_UINT16  regVal              = 0;

    // automatically caculate PCR clock
    if(0)
    {
        calculatePcrClock   = _TSO_GetPcrClock();
        printf("pcr clock: %f\n", calculatePcrClock);
        pcrInteger          = (MMP_INT32) calculatePcrClock;
        pcrDecimal          = (MMP_FLOAT) (calculatePcrClock - pcrInteger);
    }else
    	printf("pcr clock: %f\n", pcrClock);

    // TSO module was inited.
    if (gtTso.tMgrMutex)
    {
        return MMP_SUCCESS;
    }

    // Make buffer is 188 byte alignment
    if (0 == bufferSize || (bufferSize % 192) || ((MMP_UINT32) pExternalBuffer & 0x7))
    {
        goto tso_init_fail;
    }
    else
    {
        if (pExternalBuffer)
        {
            // Make the address to 8 byte aligment to gain better performance.
            gtTso.baseAddr = (MMP_UINT32) pExternalBuffer;
#if (CFG_CHIP_FAMILY == 970)
            AHB_WriteRegister16(TSO_START_MEM_BASE_ADDR_REG1,   ((gtTso.baseAddr >> 3) & 0xFFFF));
            AHB_WriteRegister16(TSO_START_MEM_BASE_ADDR_REG2,   ((gtTso.baseAddr >> 19) & 0xFFFF));
            AHB_WriteRegister16(TSO_END_MEM_BASE_ADDR_REG1,     (((gtTso.baseAddr + bufferSize - 1) >> 3) & 0xFFFF));
            AHB_WriteRegister16(TSO_END_MEM_BASE_ADDR_REG2,     (((gtTso.baseAddr + bufferSize) >> 19) & 0xFFFF));
#else
            HOST_WriteRegister( TSO_START_MEM_BASE_ADDR_REG1,   ((gtTso.baseAddr >> 3) & 0xFFFF));
            HOST_WriteRegister( TSO_START_MEM_BASE_ADDR_REG2,   ((gtTso.baseAddr >> 19) & 0xFFFF));
            HOST_WriteRegister( TSO_END_MEM_BASE_ADDR_REG1,     (((gtTso.baseAddr + bufferSize - 1) >> 3) & 0xFFFF));
            HOST_WriteRegister( TSO_END_MEM_BASE_ADDR_REG2,     (((gtTso.baseAddr + bufferSize) >> 19) & 0xFFFF));
#endif
            memset((MMP_UINT8 *) gtTso.baseAddr, 0x0, bufferSize);
            gtTso.pAllocAddr = MMP_NULL;
        }
        else
        {
            gtTso.pAllocAddr = (MMP_UINT8 *) itpVmemAlloc(bufferSize + 8);
            if (MMP_NULL == gtTso.pAllocAddr)
            {
                goto tso_init_fail;
            }
            // Make the address to 8 byte aligment to gain better performance.
            gtTso.baseAddr = (((MMP_UINT32)gtTso.pAllocAddr + 8) & 0xFFFFFFF8);
#if (CFG_CHIP_FAMILY == 970)
            AHB_WriteRegister16(TSO_START_MEM_BASE_ADDR_REG1,   ((gtTso.baseAddr >> 3) & 0xFFFF));
            AHB_WriteRegister16(TSO_START_MEM_BASE_ADDR_REG2,   ((gtTso.baseAddr >> 19) & 0xFFFF));
            AHB_WriteRegister16(TSO_END_MEM_BASE_ADDR_REG1,     (((gtTso.baseAddr + bufferSize - 1) >> 3) & 0xFFFF));
            AHB_WriteRegister16(TSO_END_MEM_BASE_ADDR_REG2,     (((gtTso.baseAddr + bufferSize) >> 19) & 0xFFFF));
#else
            HOST_WriteRegister( TSO_START_MEM_BASE_ADDR_REG1,   ((gtTso.baseAddr >> 3) & 0xFFFF));
            HOST_WriteRegister( TSO_START_MEM_BASE_ADDR_REG2,   ((gtTso.baseAddr >> 19) & 0xFFFF));
            HOST_WriteRegister( TSO_END_MEM_BASE_ADDR_REG1,     (((gtTso.baseAddr + bufferSize - 1) >> 3) & 0xFFFF));
            HOST_WriteRegister( TSO_END_MEM_BASE_ADDR_REG2,     (((gtTso.baseAddr + bufferSize) >> 19) & 0xFFFF));
#endif
            memset((MMP_UINT8 *) gtTso.baseAddr, 0x0, bufferSize);
        }
    }

    if (MMP_NULL == gtTso.tMgrMutex)
    {
        //gtTso.tMgrMutex = PalCreateMutex(MMP_NULL);
        pthread_mutex_init(&gtTso.tMgrMutex, NULL);
        if (MMP_NULL == gtTso.tMgrMutex)
        {
            goto tso_init_fail;
        }
    }

    // If engine autogenerate and inject PCR into output ts packet,
    // the following parameters will be applied.
    if (bInjectPcr && videoPid && pcrInteger)
    {
        if (videoPid > 0x1FFF
            || pcrInteger > 128)
        {
            goto tso_init_fail;
        }
        // Set Video PID
#if (CFG_CHIP_FAMILY == 970)
        AHB_WriteRegister16(TSO_VIDEO_PID_REG, ((videoPid & 0x1FFF) | (0x1 << 15)));
#else
        HOST_WriteRegister(TSO_VIDEO_PID_REG, ((videoPid & 0x1FFF) | (0x1 << 15)));
#endif
        printf("Enable Hw pcr\n");
        // Set PCR clock
        // 8 7 6 5 4 3 2   |  1 0
        // Integer         |  Decimal
        regVal = 0;
        if (pcrDecimal >= 0.75)
        {
            regVal |= 3;
        }
        else if (pcrDecimal >= 0.5)
        {
            regVal |= 2;
        }
        else if (pcrDecimal >= 0.25)
        {
            regVal |= 1;
        }
        regVal |= (pcrInteger << 2);
#if (CFG_CHIP_FAMILY == 970)
        AHB_WriteRegister16(PCR_CLOCK_REG, regVal);
#else
        HOST_WriteRegister(PCR_CLOCK_REG, regVal);
#endif
        //printf("tso.c(%d), PCR clock : %f Mhz, regVal: 0x%X\n", __LINE__, calculatePcrClock, regVal);

        // PCR Init value
#if (CFG_CHIP_FAMILY == 970)
        AHB_WriteRegister16(PCR_BASE_INIT_VAL_REG1, (startPcrValue & 0xFFFF));
        AHB_WriteRegister16(PCR_BASE_INIT_VAL_REG2, ((startPcrValue >> 16) & 0xFFFF));
#else
        HOST_WriteRegister( PCR_BASE_INIT_VAL_REG1, (startPcrValue & 0xFFFF));
        HOST_WriteRegister( PCR_BASE_INIT_VAL_REG2, ((startPcrValue >> 16) & 0xFFFF));
#endif
        if (startPcrValue)
        {
            MMP_UINT16 pcrReg1 = 0, pcrReg2 = 0;
#if (CFG_CHIP_FAMILY == 970)
            AHB_ReadRegister16( PCR_BASE_INIT_VAL_REG1, &pcrReg1);
            AHB_ReadRegister16( PCR_BASE_INIT_VAL_REG2, &pcrReg2);
#else
            HOST_ReadRegister(  PCR_BASE_INIT_VAL_REG1, &pcrReg1);
            HOST_ReadRegister(  PCR_BASE_INIT_VAL_REG2, &pcrReg2);
#endif
            printf("statr PCR: %u, reg1: 0x%X, reg2: 0x%X\n", startPcrValue, pcrReg1, pcrReg2);
        }
    }
    else
    {
        // Disable Video PCR injection
#if (CFG_CHIP_FAMILY == 970)
        AHB_WriteRegister16(TSO_VIDEO_PID_REG, 0x0);
#else
        HOST_WriteRegister(TSO_VIDEO_PID_REG, 0x0);
#endif
    }
    // Turn on CLK inverter
#if (CFG_CHIP_FAMILY == 970)
    AHB_ReadRegister16(TSO_INTERFACE_CTRL_REG, &regVal);
    regVal  |= (0x1 << 7) | (0x1 << 12) | 0x1;
	//regVal  |= (0x1 << 5);//bit 5 for null packet insert enable
    AHB_WriteRegister16(TSO_INTERFACE_CTRL_REG, regVal);
    //AHB_ReadRegister16(PCR_BASE_INIT_VAL_REG1, &regVal);
    //regVal  |= 0x1;
    AHB_WriteRegister16(TS_INTERNAL_CTRL_REG1, 0x8);
#else
    HOST_WriteRegisterMask( TSO_INTERFACE_CTRL_REG, (0x1 << 7), (0x1 << 7));
    HOST_WriteRegisterMask( TS_INTERNAL_CTRL_REG1,  0x1,        0xF);
#endif
    gtTso.videoPid      = videoPid;
    gtTso.pcrClock      = pcrClock;
    gtTso.pcrInitValue  = startPcrValue;
    gtTso.bInjectPcr    = bInjectPcr;
    gtTso.bufferSize    = bufferSize;
    _TSO_SetPadSel(padStart);
    return result;

tso_init_fail:
    if (gtTso.pAllocAddr)
    {
        itpVmemFree((void *) gtTso.pAllocAddr);
    }
    if (gtTso.tMgrMutex)
    {
        //PalDestroyMutex(gtTso.tMgrMutex);
        pthread_mutex_destroy(&gtTso.tMgrMutex);
    }
    memset(&gtTso, 0x0, sizeof(TSO_MODULE));
    return MMP_TSO_INIT_FAIL;
}

#if (CFG_CHIP_FAMILY == 970)
MMP_RESULT
mmpTsoTerminate(
    void)
{
    MMP_UINT32  regVal  = 0;
    MMP_UINT32  i       = 0;
    if (gtTso.pAllocAddr)
    {
        itpVmemFree((void *) gtTso.pAllocAddr);
    }
    if (gtTso.tMgrMutex)
    {
        //PalDestroyMutex(gtTso.tMgrMutex);
        pthread_mutex_destroy(&gtTso.tMgrMutex);
    }
    memset(&gtTso, 0x0, sizeof(TSO_MODULE));
    // Reset Hardware Clock
    AHB_ReadRegister(TSO_CLOCK_SETTING_REG, &regVal);
    // Reset
    regVal |= (0x1 << TSO_CLOCK_RESET_BIT_OFFSET);
    AHB_WriteRegister(TSO_CLOCK_SETTING_REG, regVal);

    // Dummy waiting
    for (i = 0; i < 1024; i++) { asm(""); }

    // Normal operation
    regVal &= (~(0x1 << TSO_CLOCK_RESET_BIT_OFFSET));
    AHB_WriteRegister(TSO_CLOCK_SETTING_REG, regVal);

    return MMP_SUCCESS;
}

#else
MMP_RESULT
mmpTsoTerminate(
    void)
{
    MMP_UINT16  regVal  = 0;
    MMP_UINT32  i       = 0;
    if (gtTso.pAllocAddr)
    {
        itpVmemFree((void *) gtTso.pAllocAddr);
    }
    if (gtTso.tMgrMutex)
    {
        //PalDestroyMutex(gtTso.tMgrMutex);
        pthread_mutex_destroy(&gtTso.tMgrMutex);
    }
    memset(&gtTso, 0x0, sizeof(TSO_MODULE));
    // Reset Hardware Clock
    HOST_ReadRegister(TSO_CLOCK_SETTING_REG, &regVal);
    // Reset
    regVal |= (0x1 << TSO_CLOCK_RESET_BIT_OFFSET);
    HOST_WriteRegister(TSO_CLOCK_SETTING_REG, regVal);

    // Dummy waiting
    for (i = 0; i < 1024; i++) { asm(""); }

    // Normal operation
    regVal &= (~(0x1 << TSO_CLOCK_RESET_BIT_OFFSET));
    HOST_WriteRegister(TSO_CLOCK_SETTING_REG, regVal);

    return MMP_SUCCESS;
}
#endif

MMP_RESULT
mmpTsoEnable(
    void)
{
#if (CFG_CHIP_FAMILY == 970)
    AHB_WriteRegister16(TSO_FIRE_REG, 0x1);
#else
    HOST_WriteRegister(TSO_FIRE_REG, 0x1);
#endif
    return MMP_SUCCESS;
}

MMP_RESULT
mmpTsoDisable(
    void)
{
    //PalWaitMutex(gtTso.tMgrMutex, PAL_MUTEX_INFINITE);
    pthread_mutex_lock(&gtTso.tMgrMutex);
    //PalReleaseMutex(gtTso.tMgrMutex);
    pthread_mutex_unlock(&gtTso.tMgrMutex);
    return MMP_SUCCESS;
}

MMP_RESULT
mmpTsoWrite(
    MMP_UINT8   *pInputBuffer,
    MMP_ULONG   bufferSize)
{
    MMP_RESULT  result              = MMP_SUCCESS;
    MMP_INT32   remainBufferSize    = 0;
    MMP_UINT16  regVal              = 0;
    MMP_INT32   sizeToEnd           = 0;
    MMP_INT32   usedLen             = 0;

    //PalWaitMutex(gtTso.tMgrMutex, PAL_MUTEX_INFINITE);
    pthread_mutex_lock(&gtTso.tMgrMutex);

    if (pInputBuffer && bufferSize)
    {
        if (bufferSize > gtTso.bufferSize)
        {
            result = MMP_TSO_OUT_OF_MEM;
            goto exit;
        }

        do
        {
#if (CFG_CHIP_FAMILY == 970)
            AHB_ReadRegister16( TS_READ_LEN_REG1,   &regVal);
            usedLen             |= regVal;
            AHB_ReadRegister16( TS_READ_LEN_REG2,   &regVal);
            usedLen             |= ((regVal & 0x3F) << 16);
#else
            HOST_ReadRegister(  TS_READ_LEN_REG1,   &regVal);
            usedLen             |= regVal;
            HOST_ReadRegister(  TS_READ_LEN_REG2,   &regVal);
            usedLen             |= ((regVal & 0x3F) << 16);
#endif
            remainBufferSize    = (gtTso.bufferSize - usedLen);
            if (bufferSize > remainBufferSize)
            {
                PalSleep(1);
            }
            else
            {
                sizeToEnd = gtTso.bufferSize - gtTso.writeIndex;
                if (sizeToEnd >= bufferSize)
                {
                    memcpy((MMP_UINT8 *) (gtTso.baseAddr + gtTso.writeIndex),
                           pInputBuffer,
                           bufferSize);
                    //printf("1IN: %x, %x\n", pInputBuffer[0], pInputBuffer[bufferSize-1]);
                }
                else
                {
                    memcpy( (MMP_UINT8 *) (gtTso.baseAddr + gtTso.writeIndex),
                            pInputBuffer,
                            sizeToEnd);
                    memcpy( (MMP_UINT8 *) gtTso.baseAddr,
                            &pInputBuffer[sizeToEnd],
                            bufferSize - sizeToEnd);
                    //printf("2IN: %x, %x\n", pInputBuffer[0], pInputBuffer[sizeToEnd]);
                }
                gtTso.writeIndex += bufferSize;
                if (gtTso.writeIndex >= gtTso.bufferSize)
                {
                    gtTso.writeIndex = bufferSize - sizeToEnd;
                }
#if (CFG_CHIP_FAMILY == 970)
                AHB_WriteRegister16(TSO_WRITE_LEN_REG1, (MMP_UINT16) (bufferSize & 0xFFFF));
                AHB_WriteRegister16(TSO_WRITE_LEN_REG2, (MMP_UINT16) ((bufferSize >> 16) & 0x3F));
                AHB_WriteRegister16(TSO_WRITE_CTRL_REG, 0x1);
#else
                HOST_WriteRegister( TSO_WRITE_LEN_REG1, (MMP_UINT16) (bufferSize & 0xFFFF));
                HOST_WriteRegister( TSO_WRITE_LEN_REG2, (MMP_UINT16) ((bufferSize >> 16) & 0x3F));
                HOST_WriteRegister( TSO_WRITE_CTRL_REG, 0x1);
#endif
                break;
            }
        } while (1);
    }
exit:
    //PalReleaseMutex(gtTso.tMgrMutex);
    pthread_mutex_unlock(&gtTso.tMgrMutex);
    return result;
}

MMP_RESULT
mmpTsoWriteWithoutCopy(
    MMP_ULONG bufferSize)
{
    MMP_INT32   sizeToEnd   = 0;
    MMP_RESULT  result      = MMP_SUCCESS;
    //PalWaitMutex(gtTso.tMgrMutex, PAL_MUTEX_INFINITE);
    pthread_mutex_lock(&gtTso.tMgrMutex);
    sizeToEnd           = gtTso.bufferSize - gtTso.writeIndex;
    gtTso.writeIndex    += bufferSize;
    if (gtTso.writeIndex >= gtTso.bufferSize)
    {
        gtTso.writeIndex = bufferSize - sizeToEnd;
    }
#if (CFG_CHIP_FAMILY == 970)
    AHB_WriteRegister16(TSO_WRITE_LEN_REG1, (MMP_UINT16) (bufferSize & 0xFFFF));
    AHB_WriteRegister16(TSO_WRITE_LEN_REG2, (MMP_UINT16) ((bufferSize >> 16) & 0x3F));
    AHB_WriteRegister16(TSO_WRITE_CTRL_REG, 0x1);
#else
    HOST_WriteRegister( TSO_WRITE_LEN_REG1, (MMP_UINT16) (bufferSize & 0xFFFF));
    HOST_WriteRegister( TSO_WRITE_LEN_REG2, (MMP_UINT16) ((bufferSize >> 16) & 0x3F));
    HOST_WriteRegister( TSO_WRITE_CTRL_REG, 0x1);
#endif
    //PalReleaseMutex(gtTso.tMgrMutex);
    pthread_mutex_unlock(&gtTso.tMgrMutex);
    return result;
}

MMP_UINT32
mmpTsoGetStatus(
    void)
{
    MMP_UINT16 regVal = 0;
#if (CFG_CHIP_FAMILY == 970)
    AHB_ReadRegister16(TSO_ENG_STATUS_REG, &regVal);
#else
    HOST_ReadRegister(TSO_ENG_STATUS_REG, &regVal);
#endif
    return regVal;
}

#if (CFG_CHIP_FAMILY == 970)
void
mmpTsoSetClockDivder(
    MMP_INT32 tsBitRate)
{
    MMP_INT32   tsoMBitRate     = tsBitRate / (1000 * 1000);
    MMP_INT32   pllClock        = 0;
    MMP_UINT32  regVal          = 0;
    MMP_UINT32  clockSrc        = 0;
    MMP_INT32   oriClock        = 0;
    MMP_INT32   clockDivider    = 0;
    MMP_INT32   tsoDivider      = 0;
    MMP_UINT32  i               = 0;

    AHB_ReadRegister(TSO_CLOCK_SETTING_REG, &regVal);
    clockSrc = ((regVal >> 12) & 0x7);
    //printf("clockSrc: %d, regVal: 0x%X\n", clockSrc, regVal);
    switch (clockSrc)
    {
    case 0:     //PLL1 output 1
    case 1:     //PLL1 output 2
        HOST_ReadRegister(  0x100,  &regVal);
        oriClock        = (regVal & 0x3FF);
        HOST_ReadRegister(  0xA2,   &regVal);
        clockDivider    = (regVal >> (clockSrc * 8)) & 0x7F;
        //printf("oriClock: %d, clockDivider: %d\n", oriClock, clockDivider);
        break;
    case 2:     //PLL2 output 1
    case 3:     //PLL2 output 2
        HOST_ReadRegister(  0xB4,   &regVal);
        oriClock        = (regVal & 0x3FF);
        HOST_ReadRegister(  0xB2,   &regVal);
        clockDivider    = (regVal >> ((clockSrc - 2) * 8)) & 0x7F;
        //printf("oriClock: %d, clockDivider: %d\n", oriClock, clockDivider);
        break;
    case 4:     //PLL3 output 1
    case 5:     //PLL3 output 2
    case 6:     //CKSYS (12MHz)
    case 7:     //Ring OSC (200KHz)
        break;
    }

    if (oriClock && clockDivider)
    {
        pllClock    = oriClock / clockDivider;
        tsoDivider  = (pllClock / tsoMBitRate);
        if (!(pllClock % tsoMBitRate))
        {
            tsoDivider -= 1;
        }
        //Reset TS Mux Clock
        AHB_WriteRegisterMask(TSO_CLOCK_SETTING_REG, (1 << 11) | (1 << 31), (1 << 11) | (1 << 31));

        //Delay
        for (i = 0; i < 512; i++) asm("");

        //Set TS Mux Clock Divder
        AHB_WriteRegisterMask(TSO_CLOCK_SETTING_REG, (tsoDivider | (0 << 11) | (0 << 31)), (0x3FF | (1 << 11) | (1 << 31)));

        AHB_ReadRegister(TSO_CLOCK_SETTING_REG, &regVal);

        printf("after setting: 0x%04X, tsoDivider: %d, oriClock: %d, tsBitRate: %d, tsoMBitRate: %d\n", regVal, tsoDivider, oriClock, tsBitRate, tsoMBitRate);
        gtTso.outByteRate = (pllClock * 1000 * 1000) / ((tsoDivider + 1) * 8);
    }
}

#else
void
mmpTsoSetClockDivder(
    MMP_INT32 tsBitRate)
{
    MMP_INT32   tsoMBitRate     = tsBitRate / (1000 * 1000);
    MMP_INT32   pllClock        = 0;
    MMP_UINT16  regVal          = 0;
    MMP_UINT32  clockSrc        = 0;
    MMP_INT32   oriClock        = 0;
    MMP_INT32   clockDivider    = 0;
    MMP_INT32   tsoDivider      = 0;
    MMP_UINT32  i               = 0;

    HOST_ReadRegister(0x5C, &regVal);
    clockSrc = ((regVal >> 11) & 0x7);
    //printf("clockSrc: %d, regVal: 0x%X\n", clockSrc, regVal);
    switch (clockSrc)
    {
    case 0:     //PLL1 output 1
    case 1:     //PLL1 output 2
        HOST_ReadRegister(  0xA4,   &regVal);
        oriClock        = (regVal & 0x3FF);
        HOST_ReadRegister(  0xA2,   &regVal);
        clockDivider    = (regVal >> (clockSrc * 8)) & 0x7F;
        //printf("oriClock: %d, clockDivider: %d\n", oriClock, clockDivider);
        break;
    case 2:     //PLL2 output 1
    case 3:     //PLL2 output 2
        HOST_ReadRegister(  0xB4,   &regVal);
        oriClock        = (regVal & 0x3FF);
        HOST_ReadRegister(  0xB2,   &regVal);
        clockDivider    = (regVal >> ((clockSrc - 2) * 8)) & 0x7F;
        //printf("oriClock: %d, clockDivider: %d\n", oriClock, clockDivider);
        break;
    case 4:     //PLL3 output 1
    case 5:     //PLL3 output 2
    case 6:     //CKSYS (12MHz)
    case 7:     //Ring OSC (200KHz)
        break;
    }

    if (oriClock && clockDivider)
    {
        pllClock    = oriClock / clockDivider;
        tsoDivider  = (pllClock / tsoMBitRate);
        if (!(pllClock % tsoMBitRate))
        {
            tsoDivider -= 1;
        }
        //Reset TS Mux Clock
        HOST_WriteRegisterMask(0x5C, (1 << 10) | (1 << 14), (1 << 10) | (1 << 14));

        //Delay
        for (i = 0; i < 512; i++) asm("");

        //Set TS Mux Clock Divder
        HOST_WriteRegisterMask(0x5C, (tsoDivider | (0 << 10) | (0 << 14)), (0x3FF | (1 << 10) | (1 << 14)));

        HOST_ReadRegister(0x5C, &regVal);

        printf("after setting: 0x%04X, tsoDivider: %d, oriClock: %d, tsBitRate: %d, tsoMBitRate: %d\n", regVal, tsoDivider, oriClock, tsBitRate, tsoMBitRate);
        gtTso.outByteRate = (pllClock * 1000 * 1000) / ((tsoDivider + 1) * 8);
    }
}
#endif

void
mmpTsoSetPcrBase(
    MMP_UINT32 pcrBase)
{
    pcrBase = pcrBase * 90;
#if (CFG_CHIP_FAMILY == 970)
    AHB_WriteRegister16(PCR_BASE_INIT_VAL_REG1, (pcrBase & 0xFFFF));
    AHB_WriteRegister16(PCR_BASE_INIT_VAL_REG2, ((pcrBase >> 16) & 0xFFFF));
#else
    HOST_WriteRegister( PCR_BASE_INIT_VAL_REG1, (pcrBase & 0xFFFF));
    HOST_WriteRegister( PCR_BASE_INIT_VAL_REG2, ((pcrBase >> 16) & 0xFFFF));
#endif
    //if (pcrBase)
    {
        MMP_UINT16 pcrReg1 = 0, pcrReg2 = 0;
#if (CFG_CHIP_FAMILY == 970)
        AHB_ReadRegister16( PCR_BASE_INIT_VAL_REG1, &pcrReg1);
        AHB_ReadRegister16( PCR_BASE_INIT_VAL_REG2, &pcrReg2);
#else
        HOST_ReadRegister(  PCR_BASE_INIT_VAL_REG1, &pcrReg1);
        HOST_ReadRegister(  PCR_BASE_INIT_VAL_REG2, &pcrReg2);
#endif
        printf("statr PCR: %u, reg1: 0x%X, reg2: 0x%X\n", pcrBase, pcrReg1, pcrReg2);
    }
}

MMP_UINT32
mmpTsoGetTsoRemainSize(
    void)
{
    MMP_UINT16  regVal      = 0;
    MMP_UINT32  remainSize  = 0;
#if (CFG_CHIP_FAMILY == 970)
    AHB_ReadRegister16( TS_READ_LEN_REG1,   &regVal);
    remainSize  |= regVal;
    AHB_ReadRegister16( TS_READ_LEN_REG2,   &regVal);
    remainSize  |= ((regVal & 0x3F) << 16);
#else
    HOST_ReadRegister(  TS_READ_LEN_REG1,   &regVal);
    remainSize  |= regVal;
    HOST_ReadRegister(  TS_READ_LEN_REG2,   &regVal);
    remainSize  |= ((regVal & 0x3F) << 16);
#endif
    return remainSize;
}

MMP_UINT32
mmpTsoGetTsoOutByteRate(
    void)
{
    return gtTso.outByteRate;
}

//=============================================================================
//                              Private Function Definition
//=============================================================================

void
_TSO_SetPadSel(
    MMP_UINT32 startPort)
{
#if (CFG_CHIP_FAMILY == 9850)
    // GPIO 58, 59, 60, 61 TSO
    printf("set gpio 58\n");
    ithGpioSetMode( ITH_GPIO_PIN58, ITH_GPIO_MODE3);
    ithGpioSetMode( ITH_GPIO_PIN59, ITH_GPIO_MODE3);
    ithGpioSetMode( ITH_GPIO_PIN60, ITH_GPIO_MODE3);
    ithGpioSetMode( ITH_GPIO_PIN61, ITH_GPIO_MODE3);
#elif (CFG_CHIP_FAMILY == 970)
    printf("set gpio 82\n");
        // GPIO 13, 14, 15, 16 TSO
    ithGpioSetMode( ITH_GPIO_PIN82, ITH_GPIO_MODE4);
    ithGpioSetMode( ITH_GPIO_PIN83, ITH_GPIO_MODE4);
    ithGpioSetMode( ITH_GPIO_PIN84, ITH_GPIO_MODE4);
    ithGpioSetMode( ITH_GPIO_PIN85, ITH_GPIO_MODE4);
#else
    if (13 == startPort)
    {
        printf("set gpio 13\n");
        // GPIO 13, 14, 15, 16 TSO
        ithGpioSetMode( ITH_GPIO_PIN13, ITH_GPIO_MODE1);
        ithGpioSetMode( ITH_GPIO_PIN14, ITH_GPIO_MODE1);
        ithGpioSetMode( ITH_GPIO_PIN15, ITH_GPIO_MODE1);
        ithGpioSetMode( ITH_GPIO_PIN16, ITH_GPIO_MODE1);
    }
    else // GPIO 34, 35, 36, 37
    {
        printf("set gpio 34\n");
        // GPIO 34, 35, 36, 37 TSO
        ithGpioSetMode( ITH_GPIO_PIN34, ITH_GPIO_MODE1);
        ithGpioSetMode( ITH_GPIO_PIN35, ITH_GPIO_MODE1);
        ithGpioSetMode( ITH_GPIO_PIN36, ITH_GPIO_MODE1);
        ithGpioSetMode( ITH_GPIO_PIN37, ITH_GPIO_MODE1);
    }
#endif
}

#if (CFG_CHIP_FAMILY == 970)
MMP_FLOAT
_TSO_GetPcrClock(
    void)
{
    MMP_UINT16  pcrClockSrc     = 0;
    MMP_UINT16  pcrDivider      = 0;
    MMP_UINT32  regVal          = 0;
    MMP_FLOAT   pllClock        = 0;
    MMP_INT16   oriClock        = 0;
    MMP_INT16   clockDivider    = 0;

    AHB_ReadRegister(ITH_HOST_BASE + 0x0058, &regVal);
    pcrClockSrc = ((regVal >> 12) & 0x7);
    pcrDivider  = (regVal & 0xF) + 1;
    printf("pcrClockSrc: %d, pcrDivider: %d\n", pcrClockSrc, pcrDivider);
    switch (pcrClockSrc)
    {
    case 0:     //PLL1 output 1
    case 1:     //PLL1 output 2
        //HOST_ReadRegister(ITH_HOST_BASE + 0xA4,   &regVal);
        //oriClock        = (regVal & 0x3FF);
        //HOST_ReadRegister(ITH_HOST_BASE + 0xA2,   &regVal);
        //clockDivider    = (regVal >> (pcrClockSrc * 8)) & 0x7F;
        //pllClock        = ((MMP_FLOAT) oriClock) / clockDivider;
        //printf("oriClock: %d, clockDivider: %d, pllClock: %f\n", oriClock, clockDivider, pllClock);
        break;
    case 2:     //PLL2 output 1
    case 3:     //PLL2 output 2
        //HOST_ReadRegister(ITH_HOST_BASE + 0xB4,   &regVal);
        //oriClock        = (regVal & 0x3FF);
        //HOST_ReadRegister(ITH_HOST_BASE + 0xB2,   &regVal);
        //clockDivider    = (regVal >> ((pcrClockSrc - 2) * 8)) & 0x7F;
        //pllClock        = ((MMP_FLOAT) oriClock) / clockDivider;
        //printf("oriClock: %d, clockDivider: %d, pllClock: %f\n", oriClock, clockDivider, pllClock);
        break;
    case 4:     //PLL3 output 1
    case 5:     //PLL3 output 2
    case 6:     //CKSYS (12MHz)
    case 7:     //Ring OSC (200KHz)
        break;
    }
    return (pllClock / pcrDivider);
}
#else
MMP_FLOAT
_TSO_GetPcrClock(
    void)
{
    MMP_UINT16  pcrClockSrc     = 0;
    MMP_UINT16  pcrDivider      = 0;
    MMP_UINT16  regVal          = 0;
    MMP_FLOAT   pllClock        = 0;
    MMP_INT16   oriClock        = 0;
    MMP_INT16   clockDivider    = 0;

    HOST_ReadRegister(0x40, &regVal);
    pcrClockSrc = ((regVal >> 12) & 0x7);
    pcrDivider  = (regVal & 0xF) + 1;
    printf("pcrClockSrc: %d, pcrDivider: %d\n", pcrClockSrc, pcrDivider);
    switch (pcrClockSrc)
    {
    case 0:     //PLL1 output 1
    case 1:     //PLL1 output 2
        HOST_ReadRegister(  0xA4,   &regVal);
        oriClock        = (regVal & 0x3FF);
        HOST_ReadRegister(  0xA2,   &regVal);
        clockDivider    = (regVal >> (pcrClockSrc * 8)) & 0x7F;
        pllClock        = ((MMP_FLOAT) oriClock) / clockDivider;
        printf("oriClock: %d, clockDivider: %d, pllClock: %f\n", oriClock, clockDivider, pllClock);
        break;
    case 2:     //PLL2 output 1
    case 3:     //PLL2 output 2
        HOST_ReadRegister(  0xB4,   &regVal);
        oriClock        = (regVal & 0x3FF);
        HOST_ReadRegister(  0xB2,   &regVal);
        clockDivider    = (regVal >> ((pcrClockSrc - 2) * 8)) & 0x7F;
        pllClock        = ((MMP_FLOAT) oriClock) / clockDivider;
        printf("oriClock: %d, clockDivider: %d, pllClock: %f\n", oriClock, clockDivider, pllClock);
        break;
    case 4:     //PLL3 output 1
    case 5:     //PLL3 output 2
    case 6:     //CKSYS (12MHz)
    case 7:     //Ring OSC (200KHz)
        break;
    }
    return (pllClock / pcrDivider);
}
#endif