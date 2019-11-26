/*
 * Copyright (c) 2010 ITE technology Corp. All Rights Reserved.
 */
/** @file tsi.c
 * Used to receive data through the transport stream interface (TSI).
 *
 * @author I-Chun Lai
 * @version 0.1
 */

//=============================================================================
//                              Include Files
//=============================================================================
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mmp_tsi.h"
#include "tsi_hw.h"
#include "pthread.h"

#if (!defined(WIN32))
//#define TSI_IRQ_ENABLE
#endif

#if defined(TSI_IRQ_ENABLE)
    #if defined(__OPENRTOS__)
        #include "ite/ith.h"
    #else
        #include "intr/intr.h"
    #endif
#endif

//=============================================================================
//                              fucking code
//=============================================================================
#define MMP_RESULT  uint32_t
#define MMP_ULONG   uint32_t
#define MMP_UINT    uint32_t
#define MMP_UINT32  uint32_t
#define MMP_UINT16  uint16_t
#define MMP_UINT8   uint8_t
#define MMP_INLINE  inline
#define MMP_SUCCESS 0
#define MMP_NULL    0
#define MMP_BOOL    bool
#define MMP_TRUE    true
#define MMP_FALSE   false

// Host
#define HOST_ReadRegister(add, data)            (*data = ithReadReg16(add))
#define HOST_WriteRegister(add, data)           ithWriteReg16(add, data)
#define HOST_WriteRegisterMask(add, data, mask) ithWriteReg16(add, (ithReadReg16(add) & ~mask) | (data & mask))
// AMBA
#define AHB_ReadRegister(add, data)             (*data = ithReadRegA(add))
#define AHB_WriteRegister(add, data)            ithWriteRegA(add, data)
#define AHB_WriteRegisterMask(add, data, mask)  ithWriteRegMaskA(add, data, mask)

#define PalSleep(ms)                            usleep((ms * 1000))

/**
 * Handle malloc memory. Compiler allow size = 0, but our sysetm can't.
 **/
#define tsi_malloc(size)                        ((size) ? malloc(size) : NULL)

static void
HOST_PCR_EnableClock(
    void)
{
    HOST_WriteRegisterMask( MMP_PCR_CLOCK_REG_42,   0xFFFF, MMP_PCR_EN_PCLK);
    HOST_WriteRegisterMask( MMP_PCR_CLOCK_REG_40,   0xFFFF, MMP_PCR_EN_DIV_PCLK);
    //HOST_WriteRegisterMask(MMP_HOST_BUS_CONTROLLER_REG_202, 0xFFFF, MMP_HOST_BUS_EN_MMIO_FPC);
}

static void
HOST_PCR_DisableClock(
    void)
{
    HOST_WriteRegisterMask( MMP_PCR_CLOCK_REG_42,               0x0000, MMP_PCR_EN_PCLK);
    HOST_WriteRegisterMask( MMP_PCR_CLOCK_REG_40,               0x0000, MMP_PCR_EN_DIV_PCLK);
    HOST_WriteRegisterMask( MMP_HOST_BUS_CONTROLLER_REG_202,    0x0000, MMP_HOST_BUS_EN_MMIO_FPC);
}

static void
HOST_ReadOffsetBlockMemory(MMP_UINT32 dst, MMP_UINT32 src, MMP_UINT32 size)
{
    void *mappingSysRam = ithMapVram(src, size, ITH_VRAM_READ);
    memcpy((void *)dst, mappingSysRam, size);
    ithUnmapVram(mappingSysRam, size);
}

//=============================================================================
//                              Constant Definition
//=============================================================================
#define TSO_LOOPBACK_TEST 0

/* Default buffer size definition */
#if !defined(CFG_TSI_BUF_SIZE)
    #error "Need to set tsi cache buffer size"
#endif

#ifdef WIN32
    #define TSI_MEMORY_SIZE (0xFFEE0)
#else
// DVB-T bit rate limit is 31,668,449 bit/s
// call mmpTsiReceive() every 20 ms (refer to ts_stream_reader.c)
// 31,668,449 / 50 * 2 = 1266737.96 = 155k byte
    #define TSI_MEMORY_SIZE CFG_TSI_BUF_SIZE                     //(3<<20)
#endif

#define MAX_PID_NUMBER      (8192)

//=============================================================================
//                              Macro Definition
//=============================================================================
#define REG(tsiId, offset)                      ((MMP_UINT32)(gtTsi[(tsiId)].regBaseAddr + (offset)))

#if defined(CFG_TSI_SERIAL_MODE)
    #define _verify_tsi_index(index, err_code)  ((index) = !!(index))
#elif defined(CFG_TSI_PARALLEL_MODE)
    #define _verify_tsi_index(index, err_code)  do {if (((index) = !!(index)) != 1) \
                                                        return err_code; \
                                                } while (0)
#else
    #error "unknow tsi interface in mmp_tsi.c !! "
#endif

//=============================================================================
//                              Structure Definition
//=============================================================================

typedef enum TSI_FLAG_TAG
{
    TSI_ZERO    = 0,
    TSI_ENABLED = 1
} TSI_FLAG;

typedef struct TSI_MODULE_TAG
{
    pthread_mutex_t tMgrMutex;
    MMP_UINT32      flag;
    MMP_UINT32      refCount;
    MMP_UINT32      baseAddr;
    MMP_UINT32      size;
    MMP_UINT32      readPtr;
    MMP_UINT32      regBaseAddr;

    MMP_UINT16      totalPmtCount;
    MMP_UINT16      filterTable[TSI_TOTAL_FILTER_COUNT];
} TSI_MODULE, *MMP_TSI_HANDLE;

//=============================================================================
//                              Global Data Definition
//=============================================================================

static TSI_MODULE   gtTsi[MAX_TSI_COUNT] = {0};
static MMP_UINT32   lastWritePtr[MAX_TSI_COUNT] = {0};

#if defined(TSI_IRQ_ENABLE)
static MMP_BOOL     g_IsTsiIntr[MAX_TSI_COUNT] = {0};
static MMP_UINT16   g_IntrStatus    = 0;
static MMP_UINT16   g_TsiPID        = 0;
static MMP_UINT32   g_TsiPCR        = 0;
static MMP_UINT32   g_TsiPcrCnt     = 0;
#endif

//=============================================================================
//                              Private Function Declaration
//=============================================================================
static MMP_INLINE MMP_RESULT
_TSI_Initialize(
    MMP_UINT32  tsiId,
    MMP_UINT32  bufBaseAddr,
    MMP_UINT32  bufSize);

static MMP_INLINE void
_TSI_Terminate(
    MMP_UINT32 tsiId);

static MMP_INLINE void
_TSI_Enable(
    MMP_UINT32 tsiId);

static MMP_INLINE void
_TSI_Disable(
    MMP_UINT32 tsiId);

static MMP_INLINE MMP_UINT32
_TSI_GetWritePtr(
    MMP_UINT32 tsiId);

static MMP_INLINE void
_TSI_SetReadPtr(
    MMP_UINT32  tsiId,
    MMP_UINT32  readPointerOffset);

static MMP_RESULT
_TSI_WaitEngineIdle(
    MMP_UINT32 tsiId);

static MMP_RESULT
_TSI_TsiMemClk_Enable(
    void);

static MMP_RESULT
_TSI_TsiMemClk_Disable(
    void);

//=============================================================================
//                              Public Function Definition
//=============================================================================
MMP_RESULT
mmpTsiInitialize(
    MMP_UINT32 tsiId)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    _TSI_TsiMemClk_Enable();

#if defined(CFG_TSI_PARALLEL_MODE)
    if (tsiId != 1)  // only tsi 1 support parallel mode
    {
        ithPrintf("only tsi 1 support parallel mode\n");
        goto error;
    }
    //AHB_WriteRegisterMask(ITH_GPIO_BASE + 0x94, ((0x1<<6) | (0x1<<8) | (0x1<<10) | (0x1<<12)), ((0x1<<6) | (0x1<<8) | (0x1<<10) | (0x1<<12)));

    //{
    //    uint32_t    data = 0;
    //    printf("-- set tsi 1 (parallel) gpio, %s[#%d]\n", __FUNCTION__, __LINE__);
    //    printf("--     tsi 1 (parallel) gpio, reg[0x%x]=0x%x\n", ITH_GPIO_BASE + 0x94, ithReadRegA(ITH_GPIO_BASE + 0x94));
    //}
    // set data pins
    //AHB_WriteRegisterMask(ITH_GPIO_BASE + 0x94,
    //        ((0x1<<14) | (0x1<<16) | (0x1<<18) | (0x1<<20) | (0x1<<22) | (0x1<<24) | (0x1<<26)),
    //        ((0x1<<14) | (0x1<<16) | (0x1<<18) | (0x1<<20) | (0x1<<22) | (0x1<<24) | (0x1<<26)));
#else
    switch (tsiId)
    {
    case 0:
        ithGpioSetMode( 53, ITH_GPIO_MODE4);
        ithGpioSetMode( 54, ITH_GPIO_MODE4);
        ithGpioSetMode( 55, ITH_GPIO_MODE4);
        ithGpioSetMode( 56, ITH_GPIO_MODE4);
        break;

    case 1:
        printf("Need to be defined!!!\n");
        goto error;
        break;
    }
#endif

    if (gtTsi[tsiId].refCount++ == 0)
    {
        if (MMP_NULL == gtTsi[tsiId].tMgrMutex)
            pthread_mutex_init(&gtTsi[tsiId].tMgrMutex, NULL);

        if (!gtTsi[tsiId].tMgrMutex)
        {
            result = MMP_TSI_OUT_OF_MEM;
            goto error;
        }

        // malloc tsi buf
        gtTsi[tsiId].baseAddr   = (MMP_UINT32)tsi_malloc(TSI_MEMORY_SIZE);
        gtTsi[tsiId].size       = TSI_MEMORY_SIZE;
        printf("\n\n\t tsiBuf = 0x%x, size = %d\n", gtTsi[tsiId].baseAddr, gtTsi[tsiId].size);

        if (!gtTsi[tsiId].baseAddr)
        {
            result = MMP_TSI_OUT_OF_MEM;
            goto error;
        }

        gtTsi[tsiId].regBaseAddr    = tsiId ? REG_TSP_1_BASE : REG_TSP_0_BASE;
        lastWritePtr[tsiId]         = gtTsi[tsiId].size;
        result                      = _TSI_Initialize(tsiId, gtTsi[tsiId].baseAddr, gtTsi[tsiId].size);
        if (result)
            goto error;
    }
    return result;

error:
    if (gtTsi[tsiId].baseAddr) free((void *)gtTsi[tsiId].baseAddr);

    if (gtTsi[tsiId].tMgrMutex)
    {
        //pthread_mutex_destroy(&gtTsi[tsiId].tMgrMutex);
    }
    memset(&gtTsi[tsiId], 0x0, sizeof(gtTsi));
    return result;
}

MMP_RESULT
mmpTsiTerminate(
    MMP_UINT32 tsiId)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    if (gtTsi[tsiId].refCount > 0 && --gtTsi[tsiId].refCount == 0)
    {
        mmpTsiDisable(tsiId);

        if (MMP_SUCCESS == _TSI_WaitEngineIdle(tsiId))
        {
            MMP_UINT16 bit = (0x1 << 12) << tsiId;

            HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48, bit, bit);
            PalSleep(1);
            HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48, 0, bit);
        }

        if (gtTsi[tsiId].baseAddr) free((void *)gtTsi[tsiId].baseAddr);

        pthread_mutex_destroy(&gtTsi[tsiId].tMgrMutex);
        memset(&gtTsi[tsiId], 0, sizeof(TSI_MODULE));
    }

    _TSI_TsiMemClk_Disable();

    return result;
}

MMP_RESULT
mmpTsiEnable(
    MMP_UINT32 tsiId)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    pthread_mutex_lock(&gtTsi[tsiId].tMgrMutex);

    if (gtTsi[tsiId].refCount > 0 && !(gtTsi[tsiId].flag & TSI_ENABLED))
    {
        _TSI_Enable(tsiId);
    }

    gtTsi[tsiId].readPtr = gtTsi[tsiId].size;
    while (gtTsi[tsiId].readPtr >= gtTsi[tsiId].size)
        gtTsi[tsiId].readPtr = _TSI_GetWritePtr(tsiId);

    gtTsi[tsiId].flag |= TSI_ENABLED;

    pthread_mutex_unlock(&gtTsi[tsiId].tMgrMutex);
    return result;
}

// Caution! Disable TSI acts as Terminate TSI, all TSI's reg will be reset.
// DON'T use it.
MMP_RESULT
mmpTsiDisable(
    MMP_UINT32 tsiId)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    pthread_mutex_lock(&gtTsi[tsiId].tMgrMutex);

    if (gtTsi[tsiId].refCount > 0 && (gtTsi[tsiId].flag & TSI_ENABLED))
    {
        _TSI_Disable(tsiId);
        gtTsi[tsiId].flag &= ~TSI_ENABLED;
    }

    pthread_mutex_unlock(&gtTsi[tsiId].tMgrMutex);

    return result;
}

MMP_RESULT
mmpTsiReset(
    MMP_UINT32 tsiId)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    pthread_mutex_lock(&gtTsi[tsiId].tMgrMutex);

    //TSI Disable
    HOST_WriteRegisterMask(REG(tsiId, REG_TSI_SETTING_OFFSET), 0x0000, 0x0001);

    if (MMP_SUCCESS == _TSI_WaitEngineIdle(tsiId))
    {
        MMP_UINT16 bit = (0x1 << 12) << tsiId;

        HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48, bit, bit);
        PalSleep(1);
        HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48, 0, bit);
    }
    gtTsi[tsiId].regBaseAddr    = tsiId ? REG_TSP_1_BASE : REG_TSP_0_BASE;
    lastWritePtr[tsiId]         = gtTsi[tsiId].size;
    result                      = _TSI_Initialize(tsiId, gtTsi[tsiId].baseAddr, gtTsi[tsiId].size);

    pthread_mutex_unlock(&gtTsi[tsiId].tMgrMutex);
    return result;
}

#if 0//def WIN32
MMP_RESULT
mmpTsiReceive(
    MMP_UINT32  tsiId,
    void        *buffer,
    MMP_ULONG   maxSize,
    MMP_ULONG   *actualSize)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    pthread_mutex_lock(&gtTsi[tsiId].tMgrMutex);

    if (buffer && actualSize && maxSize > 0)
    {
        if (gtTsi[tsiId].flag & TSI_ENABLED)
        {
            MMP_UINT32  tsiWritePtr = _TSI_GetWritePtr(tsiId);
            MMP_UINT32  readSize    = 0;

            if (gtTsi[tsiId].readPtr < tsiWritePtr)
                readSize = tsiWritePtr - gtTsi[tsiId].readPtr;
            else if (tsiWritePtr < gtTsi[tsiId].readPtr)
                readSize = gtTsi[tsiId].size - gtTsi[tsiId].readPtr;
            else
            {
                (*actualSize) = readSize;
                goto exit;
            }

            if (readSize > maxSize)
                readSize = maxSize;

            // Copy the TS data from VRAM to system RAM
            // Copy the TS data from VRAM to system RAM
            HOST_ReadOffsetBlockMemory(
                (MMP_UINT32)buffer,
                gtTsi[tsiId].baseAddr + gtTsi[tsiId].readPtr,
                readSize);

            gtTsi[tsiId].readPtr += readSize;
            if (gtTsi[tsiId].readPtr >= gtTsi[tsiId].size)
                gtTsi[tsiId].readPtr = 0;

            _TSI_SetReadPtr(tsiId, gtTsi[tsiId].readPtr);

            (*actualSize) = readSize;
            goto exit;
        }
        else
        {
            result = MMP_TSI_IS_DISABLED;
        }
    }
    else
    {
        result = MMP_TSI_BAD_PARAM;
    }

exit:
    pthread_mutex_unlock(&gtTsi[tsiId].tMgrMutex);
    return result;
}

#else
MMP_RESULT
mmpTsiReceive(
    MMP_UINT32  tsiId,
    MMP_UINT8   **ppOutBuffer,
    MMP_ULONG   *outSize)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    pthread_mutex_lock(&gtTsi[tsiId].tMgrMutex);

    if (0 == gtTsi[tsiId].refCount)
    {
        result = MMP_TSP_IS_NONINITED;
        goto exit;
    }

    if (ppOutBuffer && outSize)
    {
        if (gtTsi[tsiId].flag & TSI_ENABLED)
        {
            MMP_UINT32  tsiWritePtr;
            MMP_UINT32  readSize = 0;
            tsiWritePtr = _TSI_GetWritePtr(tsiId);

            // workaround: avoid to read invalid writePtr
            if (tsiWritePtr < lastWritePtr[tsiId])
            {
                tsiWritePtr = _TSI_GetWritePtr(tsiId);
                if (tsiWritePtr < lastWritePtr[tsiId])
                {
                    tsiWritePtr = _TSI_GetWritePtr(tsiId);
                }
            }
            lastWritePtr[tsiId] = tsiWritePtr;

            if (tsiWritePtr < gtTsi[tsiId].size
                && gtTsi[tsiId].readPtr < tsiWritePtr)
            {
                readSize = tsiWritePtr - gtTsi[tsiId].readPtr;

                // workaround: maybe tsi still keep data in hw buffer
                // and doesn't write to memory yet
                // so we just ignore the latest data to avoid reading wrong data
                if (readSize > 32)
                    readSize -= 32;
                else
                    readSize = 0;
            }
            else if (tsiWritePtr < gtTsi[tsiId].readPtr)
            {
                readSize = gtTsi[tsiId].size - gtTsi[tsiId].readPtr;

                // workaround: maybe tsi still keep data in hw buffer
                // and doesn't write to memory yet
                // so we just ignore the latest data to avoid reading wrong data
                if (tsiWritePtr < 32)
                {
                    if (readSize + tsiWritePtr > 32)
                        readSize -= (32 - tsiWritePtr);
                    else
                        readSize = 0;
                }
            }
            else
            {
                (*outSize) = readSize;
                goto exit;
            }

            if (readSize > (gtTsi[tsiId].size >> 1))
                printf("tsi.c: consume data irregularly\n");

            (*ppOutBuffer)          = (MMP_UINT8 *) (gtTsi[tsiId].baseAddr + gtTsi[tsiId].readPtr);

            gtTsi[tsiId].readPtr    += readSize;
            if (gtTsi[tsiId].readPtr >= gtTsi[tsiId].size)
                gtTsi[tsiId].readPtr = 0;

            _TSI_SetReadPtr(tsiId, gtTsi[tsiId].readPtr);

            (*outSize) = readSize;
            goto exit;
        }
        else
        {
            result = MMP_TSI_IS_DISABLED;
        }
    }
    else
    {
        result = MMP_TSI_BAD_PARAM;
    }

exit:
    ithInvalidateDCacheRange((*ppOutBuffer), (*outSize));   //flush cache?
    pthread_mutex_unlock(&gtTsi[tsiId].tMgrMutex);
    return result;
}

#endif

MMP_BOOL
mmpTsiIsPcrInterruptTriggered(
    MMP_UINT32 tsiId)
{
#if defined(TSI_IRQ_ENABLE)
    tsiId = !!tsiId;

    return g_IsTsiIntr[tsiId];
#else
    MMP_UINT16  interruptBit;
    MMP_UINT16  tsiStatus;
    MMP_UINT16  value;

    tsiId           = !!tsiId;

    interruptBit    = 0x0020 << tsiId;

    HOST_ReadRegister(  MMP_GENERAL_INTERRUPT_REG_0C,       &value);
    HOST_ReadRegister(  REG(tsiId, REG_TSI_STATUS_OFFSET),  &tsiStatus);

    //Check TSI interrupt
    if (((value & (interruptBit)) == (interruptBit))
        && ((tsiStatus & (0x8)) == 0x8))
        return MMP_TRUE;
    else
        return MMP_FALSE;
#endif
}

MMP_UINT32
mmpTsiGetPcrValue(
    MMP_UINT32 tsiId)
{
#if defined(TSI_IRQ_ENABLE)
    return g_TsiPCR;
#else
    MMP_UINT16  PCR_L;
    MMP_UINT16  PCR_M;

    tsiId = !!tsiId;

    HOST_ReadRegister(  REG(tsiId, REG_TSI_PCR_L_OFFSET),   &PCR_L);
    HOST_ReadRegister(  REG(tsiId, REG_TSI_PCR_M_OFFSET),   &PCR_M);

    return (((MMP_UINT32)PCR_M << 16) + PCR_L);
#endif
}

//////////////////////////////////////////////////////////////////////////
// Must read PCR_L register before read this PID register
//////////////////////////////////////////////////////////////////////////
MMP_UINT16
mmpTsiGetPcrPid(
    MMP_UINT32 tsiId)
{
#if defined(TSI_IRQ_ENABLE)
    //printf("mmp-get-PID=[%x]\n",g_TsiPID);
    return g_TsiPID;    //test
#else
    MMP_UINT16 pcrPid;

    tsiId = !!tsiId;

    HOST_ReadRegister(REG(tsiId, REG_TSI_GET_PCR_PID_OFFSET), &pcrPid);

    return pcrPid;
#endif
}

//////////////////////////////////////////////////////////////////////////
// Must read PCR_L register before read this PCRCnt register
//////////////////////////////////////////////////////////////////////////
MMP_UINT32
mmpTsiGetPCRCnt(
    MMP_UINT32 tsiId)
{
#if defined(TSI_IRQ_ENABLE)
    //printf("mmp-get-PcrCnt=[%x]\n",g_TsiPcrCnt);
    return g_TsiPcrCnt;
#else
    MMP_UINT16  PCRCnt_L;
    MMP_UINT16  PCRCnt_H;
    MMP_UINT32  PCRCnt = 0xFFFFFFFF;

    tsiId = !!tsiId;

    HOST_ReadRegister(  REG(tsiId, REG_TSI_PCR_CNT_L_OFFSET),   &PCRCnt_L);
    HOST_ReadRegister(  REG(tsiId, REG_TSI_PCR_CNT_H_OFFSET),   &PCRCnt_H);

    //Check PCRCnt overflow
    if ((PCRCnt_H & (1 << 10)) == 0)
    {
        PCRCnt = (((PCRCnt_H & 0x3FF) << 16) + PCRCnt_L);
    }
    //else
    //    printf( "PCR Cnt Overflow\n");

    return PCRCnt;
#endif
}

//////////////////////////////////////////////////////////////////////////
// If has interrupt, must be clear interrupt
//////////////////////////////////////////////////////////////////////////
void
mmpTsiClearInterrupt(
    MMP_UINT32 tsiId)
{
#if defined(TSI_IRQ_ENABLE)
    tsiId               = !!tsiId;

    g_IsTsiIntr[tsiId]  = MMP_FALSE;
#else
    MMP_UINT16 bit = (0x1 << 5) << tsiId;

    HOST_WriteRegisterMask(MMP_GENERAL_INTERRUPT_REG_0A, bit, bit);
#endif
}

MMP_RESULT
mmpTsiEnablePcrPidFilter(
    MMP_UINT32  tsiId,
    MMP_UINT16  pid)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    if (0 == gtTsi[tsiId].refCount)
        return MMP_TSP_IS_NONINITED;

    if (pid >= MAX_PID_NUMBER)
        return MMP_TSI_BAD_PARAM;

    HOST_WriteRegister(REG(tsiId, REG_TSI_SET_PCR_PID_FILTER_OFFSET), (pid << 1) | 0x1);

    return result;
}

MMP_RESULT
mmpTsiDisablePcrPidFilter(
    MMP_UINT32 tsiId)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    if (0 == gtTsi[tsiId].refCount)
        return result;

    HOST_WriteRegister(REG(tsiId, REG_TSI_SET_PCR_PID_FILTER_OFFSET), 0);

    return result;
}

// had better convert the counter to time duration
MMP_RESULT
mmpTsiEnablePCRCntThreshold(
    MMP_UINT32  tsiId,
    MMP_UINT16  threshold)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    if (0 == gtTsi[tsiId].refCount)
        return result;

    HOST_WriteRegister(REG(tsiId, REG_TSI_PCR_CNT_THRESHOLD_OFFSET), ((threshold & 0x3FF) << 1) | 0x1);

    return result;
}

MMP_RESULT
mmpTsiDisablePCRCntThreshold(
    MMP_UINT32 tsiId)
{
    MMP_RESULT result = MMP_SUCCESS;

    _verify_tsi_index(tsiId, result);

    if (0 == gtTsi[tsiId].refCount)
        return result;

    HOST_WriteRegister(REG(tsiId, REG_TSI_PCR_CNT_THRESHOLD_OFFSET), 0);

    return result;
}

//=============================================================================
/**
 * Update PID filter.
 *
 * @param tsiId The specific tsi.
 * @param pid   filter PID value.
 * @param type  The specific PID entry index.
 * @return none
 */
//=============================================================================
MMP_RESULT
mmpTsiUpdatePidFilter(
    MMP_UINT32              tsiId,
    MMP_UINT32              pid,
    TSI_PID_FILTER_INDEX    pidIndex)
{
    MMP_RESULT  result  = MMP_SUCCESS;
    MMP_BOOL    bInsert = MMP_TRUE;
    MMP_BOOL    bUpdate = MMP_TRUE;
    MMP_UINT32  i       = pidIndex;

    _verify_tsi_index(tsiId, result);

    if (0 == gtTsi[tsiId].refCount)
        return MMP_TSP_IS_NONINITED;

    if (pid >= MAX_PID_NUMBER || pidIndex >= TSI_TOTAL_FILTER_COUNT)
        return MMP_TSI_BAD_PARAM;

    // Only the PMT table owns more than one index.
    if (TSI_PID_FILTER_PMT_INDEX == pidIndex)
    {
        for (i; i < ((MMP_UINT32)TSI_PID_FILTER_PMT_INDEX + gtTsi[tsiId].totalPmtCount); ++i)
        {
            if (pid == gtTsi[tsiId].filterTable[i])
            {
                bInsert = MMP_FALSE;
                break;
            }
        }
        if (bInsert)
        {
            if (gtTsi[tsiId].totalPmtCount >= TSI_MAX_PMT_FILTER_COUNT)
            {
                printf("no more available PMT pid filter\n");
                bUpdate = MMP_FALSE;
            }
            else
                gtTsi[tsiId].totalPmtCount++;
        }
    }

    // If the pid is added to table already, ignore it.
    if (pid != gtTsi[tsiId].filterTable[i] && bUpdate)
    {
        HOST_WriteRegister(REG(tsiId, REG_TSI_PID_FILTER_BASE_OFFSET) + (MMP_UINT16)(i << 1),
                           (MMP_UINT16)(pid << 1) | 0x1);
    }
    gtTsi[tsiId].filterTable[i] = (MMP_UINT16)pid;

    return result;
}

//=============================================================================
/**
 * Turn on/off the hardware filter
 *
 * @return none
 */
//=============================================================================
MMP_RESULT
mmpTsiResetPidFilter(
    MMP_UINT32 tsiId)
{
    MMP_RESULT  result = MMP_SUCCESS;
    MMP_UINT    i;

    _verify_tsi_index(tsiId, result);

    if (0 == gtTsi[tsiId].refCount)
        return result;

    for (i = 0; i < 32; ++i)
    {
        HOST_WriteRegister(REG(tsiId, REG_TSI_PID_FILTER_BASE_OFFSET) + (i << 1), 0);
    }

    memset(gtTsi[tsiId].filterTable, 0xFF, sizeof(gtTsi[tsiId].filterTable));
    gtTsi[tsiId].totalPmtCount = 0;

    return result;
}

//=============================================================================
//                              Private Function Definition
//=============================================================================
#if defined(TSI_IRQ_ENABLE)
void _TSI_ISR(const MMP_UINT32 tsiId)
{
    MMP_UINT16  tmp = 0;
    MMP_UINT16  tmpPCR1;
    MMP_UINT16  tmpPCR2;
    MMP_UINT16  tmpPID;
    MMP_UINT16  PCRCnt_L;
    MMP_UINT16  PCRCnt_H;
    MMP_UINT32  TmpWtPtr = 0;

    ithPrintf("tsi%d.isr.in\n", tsiId);//ITH_INTR_TSI0

    /*
    if (startTime)
    {
        duration = PalGetDuration(startTime);
        ithPrintf("isr.in,dur=%u\n",duration);
        startTime=0;
    }
     */

    HOST_ReadRegister(REG(tsiId, REG_TSI_STATUS_OFFSET), &tmp);
    if (tmp & 0x10)
    {
        if (tmp & 0x08)
        {
            //printf("int PCR\n");
            g_IntrStatus |= 0x01;

            HOST_ReadRegister(REG(tsiId, REG_TSI_PCR_L_OFFSET),       &tmpPCR1);  //have to read PCR1 first before check PID & PCR2 & PCR3
            HOST_ReadRegister(REG(tsiId, REG_TSI_PCR_M_OFFSET),       &tmpPCR2);
            g_TsiPCR    = ((MMP_UINT32)tmpPCR2 << 16) + tmpPCR1; //merge PCR value

            HOST_ReadRegister(REG(tsiId, REG_TSI_GET_PCR_PID_OFFSET), &tmpPID);
            g_TsiPID    = tmpPID & 0x1FFF; //Mask the PID value

            HOST_ReadRegister(REG(tsiId, REG_TSI_PCR_CNT_L_OFFSET),   &PCRCnt_L);
            HOST_ReadRegister(REG(tsiId, REG_TSI_PCR_CNT_H_OFFSET),   &PCRCnt_H);
            g_TsiPcrCnt     = (((PCRCnt_H & 0x3FF) << 16) + PCRCnt_L);

            g_IsTsiIntr[tsiId]  = MMP_TRUE;
        }

        if (tmp & 0x04)
        {
            g_IntrStatus    |= 0x02;
            TmpWtPtr        = _TSI_GetWritePtr(tsiId);
            //_TSI_SetReadPtr(0, TmpWtPtr); //for testing only
        }

        if (tmp & 0x8000)
        {
            //printf("int Err non-188\n");
            g_IntrStatus |= 0x02;
        }
        if (tmp & 0x4000)
        {
            //printf("int Err fifo-full]\n");
            g_IntrStatus |= 0x04;
        }
    }
    //SYS_SetEventFromIsr(Tsi0IsrEvent);
    HOST_WriteRegisterMask(REG(tsiId, REG_TSI_CLEAL_INTR_OFFSET), 0x0001, 0x0001);
    //ithPrintf("tsi%d.isr.out[%04x,%x,%x]\n",tsiId,tmp,g_IsTsiIntr[tsiId],TmpWtPtr);
}

void _TSI0_ISR(void *data)
{
    _TSI_ISR(0);
}

void _TSI1_ISR(void *data)
{
    _TSI_ISR(1);
}

void _TSI_EnableIntr(MMP_UINT32 tsiId)
{
    MMP_UINT32      irq = ITH_INTR_TSI0;
    ITHIntrHandler  isr = _TSI0_ISR;

    if (tsiId != 0)
    {
    	//only TSI0 support interrupt
    	printf("only TSI0 support interrupt!!!\n");
        return;
    }

    // Initialize TSI IRQ
    ithPrintf("Enable Tsi%d IRQ~~\n", tsiId);
    ithIntrDisableIrq(irq);
    ithIntrClearIrq(irq);

    #if defined (__FREERTOS__)
    // register TSI ISR
    ithPrintf("register TSI%d ISR!!\n", tsiId);
    ithIntrRegisterHandlerIrq(irq, isr, MMP_NULL);
    #endif // defined (__FREERTOS__)

    // set IRQ to edge trigger
    ithIntrSetTriggerModeIrq(irq, ITH_INTR_EDGE);

    // set IRQ to detect rising edge
    ithIntrSetTriggerLevelIrq(irq, ITH_INTR_HIGH_RISING);

    // Enable IRQ
    ithIntrEnableIrq(irq);

    ithPrintf("Enable Tsi%d IRQ$$\n", tsiId);
}

void _TSI_DisableIntr(MMP_UINT32 tsiId)
{
	//only TSI0 support interrupt
	if(tsiId != 0)
		printf("only TSI0 support interrupt!!!\n");
	else
	    ithIntrDisableIrq(ITH_INTR_TSI0);
}

#endif

MMP_RESULT
_TSI_Initialize(
    MMP_UINT32  tsiId,
    MMP_UINT32  bufBaseAddr,
    MMP_UINT32  bufSize)
{
    if (bufBaseAddr && bufSize > 0)
    {
        MMP_RESULT result;

        if (MMP_SUCCESS == (result = _TSI_WaitEngineIdle(tsiId)))
        {
            // Init TSI register
            HOST_WriteRegister(REG(tsiId, REG_TSI_MEMORY_BASE_L_OFFSET),
                               (MMP_UINT16)(bufBaseAddr & 0xFFFF));
            PalSleep(1);
            HOST_WriteRegister(REG(tsiId, REG_TSI_MEMORY_BASE_H_OFFSET),
                               (MMP_UINT16)((bufBaseAddr >> 16) & 0xFFFF));
            PalSleep(1);

            HOST_WriteRegister(REG(tsiId, REG_TSI_MEMORY_SIZE_L_OFFSET),
                               (MMP_UINT16)(bufSize & 0xFFFF));
            PalSleep(1);
            HOST_WriteRegister(REG(tsiId, REG_TSI_MEMORY_SIZE_H_OFFSET),
                               (MMP_UINT16)((bufSize >> 16) & 0xFFFF));
            PalSleep(1);

            HOST_WriteRegister(REG(tsiId, REG_TSI_READ_POINTER_L_OFFSET),
                               0x0000);
            PalSleep(1);
            HOST_WriteRegister(REG(tsiId, REG_TSI_READ_POINTER_H_OFFSET),
                               0x0000);
            PalSleep(1);

            //Adaptation_field_length > 6
            HOST_WriteRegister(REG(tsiId, REG_TSI_SYNC_BYTE_AF_LENGTH_OFFSET),
                               0x4706);
            PalSleep(1);

            //Enable TSI and FPC PCLK
            //HOST_PCR_EnableClock();  //YC mark

#if defined(TSI_IRQ_ENABLE)
            //enable intr & irq
            _TSI_EnableIntr(tsiId);
#endif

            return MMP_SUCCESS;
        }
        return result;
    }
    return MMP_TSI_BAD_PARAM;
}

void
_TSI_Terminate(
    MMP_UINT32 tsiId)
{}

void
_TSI_Enable(
    MMP_UINT32 tsiId)
{
    //TSI Interrupt Enable
    MMP_UINT16 interruptBit = (0x20 << tsiId);

    HOST_WriteRegisterMask(MMP_GENERAL_INTERRUPT_REG_06, interruptBit, interruptBit);

    HOST_WriteRegister(REG(tsiId, REG_TSI_FREQUENCY_RATIO_OFFSET), (27 << 2)); //27MHz

    //if (tsiId == 1)
    {
        //Rising sample and 2T
//        HOST_WriteRegister(REG(tsiId, REG_TSI_IO_SRC_AND_FALL_SAMP_OFFSET), 0x02A8);
#if TSO_LOOPBACK_TEST
        HOST_WriteRegister( REG(tsiId, REG_TSI_IO_SRC_AND_FALL_SAMP_OFFSET),    0x0 | 0x1 << 15);
        HOST_WriteRegister( REG(tsiId, 0x20),                                   0x1);
#else
        HOST_WriteRegister( REG(tsiId, REG_TSI_IO_SRC_AND_FALL_SAMP_OFFSET),    0x0);
#endif
    }

    // enable TSI, others is default
#if defined(CFG_TSI_PARALLEL_MODE)
    // parallel mode
    HOST_WriteRegister(REG(tsiId, REG_TSI_SETTING_OFFSET), 0x400F);
	//HOST_WriteRegister(REG(tsiId, REG_TSI_SETTING_OFFSET), 0x400B); // bit 2 to enable PCR intr
#else
    // serial mode
    HOST_WriteRegisterMask(REG(tsiId, REG_TSI_SETTING_OFFSET), 0x0000, 0x0F00);
	HOST_WriteRegister(REG(tsiId, REG_TSI_SETTING_OFFSET), 0x402F);
    //HOST_WriteRegister(REG(tsiId, REG_TSI_SETTING_OFFSET), 0x402B); // bit 2 to enable PCR intr
#endif
}

void
_TSI_Disable(
    MMP_UINT32 tsiId)
{
    HOST_WriteRegisterMask(REG(tsiId, REG_TSI_SETTING_OFFSET), 0x0000, 0x0001);

    // MMP_TSI_CLOCK_REG_48 will reset engine and register.
//    if (MMP_SUCCESS == _TSI_WaitEngineIdle(tsiId))
//    {
//        MMP_UINT16 bit = (0x1 << 12) << tsiId;
//
//        HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48, bit, bit);
//        PalSleep(1);
//        HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48,   0, bit);
//    }
}

MMP_UINT32
_TSI_GetWritePtr(
    MMP_UINT32 tsiId)
{
    MMP_UINT16 writePtr_L, writePtr_H;

    // Get write pointer offset
    HOST_ReadRegister(  REG(tsiId, REG_TSI_WRITE_POINTER_L_OFFSET), &writePtr_L);
    HOST_ReadRegister(  REG(tsiId, REG_TSI_WRITE_POINTER_H_OFFSET), &writePtr_H);

    return (((MMP_UINT32)writePtr_H << 16) + writePtr_L);
}

void
_TSI_SetReadPtr(
    MMP_UINT32  tsiId,
    MMP_UINT32  readPtr)
{
    HOST_WriteRegister( REG(tsiId, REG_TSI_READ_POINTER_L_OFFSET),
                        (MMP_UINT16)(readPtr & 0xFFFF));
    HOST_WriteRegister( REG(tsiId, REG_TSI_READ_POINTER_H_OFFSET),
                        (MMP_UINT16)((readPtr >> 16) & 0xFFFF));
}

/**
 * Wait TSI engine idle!
 */
MMP_RESULT
_TSI_WaitEngineIdle(
    MMP_UINT32 tsiId)
{
    volatile MMP_UINT16 status  = 0;
    MMP_UINT16          timeOut = 0;

    //
    //  Wait TSI engine idle!   D[1]  1: idle, 0: busy
    //
    do
    {
        HOST_ReadRegister(REG(tsiId, REG_TSI_STATUS_OFFSET), (MMP_UINT16 *)&status);
        if (status & 0x0002)
            break;

        if (++timeOut > 2000)
            return MMP_TSI_ENGINE_BUSY;

        PalSleep(1);
    } while (MMP_TRUE);

    return MMP_SUCCESS;
}

static MMP_RESULT
_TSI_TsiMemClk_Enable(void)
{
    HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48, MMP_PCR_EN_M12CLK, MMP_PCR_EN_M12CLK); //for TSI0
    return MMP_SUCCESS;
}

static MMP_RESULT
_TSI_TsiMemClk_Disable(void)
{
    HOST_WriteRegisterMask(MMP_TSI_CLOCK_REG_48, 0x0000, MMP_PCR_EN_M12CLK);    //for TSI0
    return MMP_SUCCESS;
}
