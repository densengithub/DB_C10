#include "register_template.h"
#include "ts_airfile_def.h"
#include "ts_airfile.h"
#include "ite_ts_airfile.h"
#include "ite/itp.h"
//#include "mmp_tsi.h"
#include "ssp/mmp_spi.h"
#include "xcpu_master/itx.h"
//#include "xcpu/xcpu_io.h"

//=============================================================================
//                  Constant Definition
//=============================================================================
#define MAX_SUPPORT_CACHE_CNT               1

#define CACHE_BUFFER_SIZE                   12582840//10454868//4095956//3145616//8388560
#define CACHE_GET_SAMPLE_TIME_DELAY         1000

#define CACHE_THREAD_TIMER_GAP              3
#if defined(CFG_SYSTEM_PESI_IPTV)
#define TS_IN_LED0                          6
#define TS_IN_LED1                          7
#if (ITX_BUS_TYPE == ITX_BUS_I2C)
#define MEM_SHARED_INFO         0xC0200000
#else
#define MEM_SHARED_INFO         0x1000
#endif
#endif

#define TRANSPORT_PACKET_TOTAL_SIZE 1316 //188*7
#define MUX_BUFFER_VALID_COUNT 512



/**
 * ts air file handle status
 **/
typedef enum _TSAF_SITUATION_TAG
{
    TSAF_SITUATION_IDLE = 0x11,
    TSAF_SITUATION_BUSY = 0xBB,
    TSAF_SITUATION_FAIL = 0xFF,

}TSAF_SITUATION;

//=============================================================================
//                  Macro Definition
//=============================================================================
DEFINE_REGISTER_TEMPLATE(TS_AIRFILE_DESC, TS_AIRFILE_TYPE_ID);

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct _TSAF_DEV_TAG
{
    TSAF_HANDLE        hTsaf;

    TSAF_SITUATION     situation;

    pthread_mutex_t    tsaf_mutex;

    TS_AIRFILE_DESC    *pCurDesc;

    TSAF_CRTL_INFO     tsafCtrlInfo;

    TSAF_INIT_PARAM    initParam;

    TSAF_CACHE_INFO    cacheInfo;

    // to integrate with aggregation module
    uint32_t            demod_port_index;
    uint32_t            tsi_index;

}TSAF_DEV;

//=============================================================================
//                  Global Data Definition
//=============================================================================
uint32_t  tsafMsgOnFlag = (0x1);

#if defined(CFG_SYSTEM_TRANWO_IPTV)
static bool gbIsEncoderAlive = false;
#endif
static bool gbstreamhdcpon = true;
//=============================================================================
//                  Private Function Definition
//=============================================================================
#if 1
#include "ts_demuxer/ite_ts_demuxer.h"

#define IS_THREAD_RUNNING(index, pBufMgr)     ((pBufMgr)->bStartCacheThread)
#define IS_START_CACHE(index, pBufMgr)        ((pBufMgr)->bStartCache)

//static FILE *f_ts;
static bool stream_error = false; 

static void*
_default_cache_BufThread(
    void *args)
{
    TSAF_INIT_PARAM     *pInitParam = (TSAF_INIT_PARAM*)args;
    const uint32_t      cache_index = pInitParam->ts_stream_index;
    CACHE_BUF_MGR       *pCacheBufMgr = 0;
    uint32_t            waiting_time = 0;
    GET_TS_STREAM_FUNC  getTsStreamFunc = 0;
    int                 ret, i = 0, state = 0;
    struct timeval timeout_t;

    bool                no_data = false;
    uint32_t            xcpu_version = 0;

    _tsaf_trace_enter(TSAF_MSG_TYPE_TRACE_CACHE_BUF, "\t cache_index = %d\n", cache_index);

    if( cache_index >= MAX_SUPPORT_CACHE_CNT || !pInitParam )      return 0;

    pCacheBufMgr = pInitParam->pCacheBufMgr;
    waiting_time = pInitParam->waiting_time;

#if defined(CFG_SYSTEM_PESI_IPTV)
    ithGpioEnable(TS_IN_LED0);
    ithGpioEnable(TS_IN_LED1);
    ithGpioSetOut(TS_IN_LED0);
    ithGpioSetOut(TS_IN_LED1);
#endif
    //------------------------------------------
    // init parameters
    _tsaf_mutex_init(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);
    // _tsaf_mutex_init(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheThreadStopMutex);
    // _tsaf_mutex_init(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheStopMutex);

    pCacheBufMgr->bStartCacheThread = true;

    getTsStreamFunc = pInitParam->getTsStreamFunc;
    printf("\tsi_idx[%d], getTsStreamFunc=0x%x\n", cache_index, getTsStreamFunc);

    if( pCacheBufMgr->pCacheBuf )      free(pCacheBufMgr->pCacheBuf);

    if( !(pCacheBufMgr->pCacheBuf = (uint8_t*)tsaf_malloc(CACHE_BUFFER_SIZE)) )
    {
        pCacheBufMgr->bStartCacheThread = false;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, " cache (%d-th) buffer alloc fail !", cache_index);
    }

    tsaf_get_clock(&pCacheBufMgr->lastT);
    tsaf_get_clock(&timeout_t);

    //--------------------------
    // thread running
    while( IS_THREAD_RUNNING(cache_index, pCacheBufMgr) )
    {
        if( tsaf_get_duration(&pCacheBufMgr->lastT) > CACHE_THREAD_TIMER_GAP )
        {
            struct timeval timeout;
            uint8_t     *pSampleAddr = 0;
            uint32_t    sampleSize = 0;
            uint32_t    readywriteSize = 0;
            uint32_t    stc_time = 0;
            ITX_STATE   pState = ITX_STATE_ENCODING;

            tsaf_get_clock(&pCacheBufMgr->lastT);
            tsaf_get_clock(&timeout);

            // cache buffer access
            while( IS_START_CACHE(cache_index, pCacheBufMgr) )
            {
                if( pCacheBufMgr->cacheBuf_WPtr > pCacheBufMgr->cacheBuf_RPtr )
                {
                    readywriteSize = (CACHE_BUFFER_SIZE - pCacheBufMgr->cacheBuf_WPtr) + pCacheBufMgr->cacheBuf_RPtr;
                }
                else
                {   
                    readywriteSize = pCacheBufMgr->cacheBuf_RPtr - pCacheBufMgr->cacheBuf_WPtr;
                }

#if defined(CFG_SYSTEM_PESI_IPTV)                
                if( (9500000 < readywriteSize < 10500000) && (state != 1))
                {
                    ithWriteRegH(0x00b6, 0x0005);
                    state = 1;
                }
                else if( readywriteSize > 12000000  && (state != 2))
                {
                    ithWriteRegH(0x00b6, 0x0403);
                    state = 2;
                }
                else if( readywriteSize > 11200000  && (state != 3))
                {
                    ithWriteRegH(0x00b6, 0x0401);
                    state = 3;
                }
                else if( readywriteSize > 10600000  && (state != 4))
                {
                    ithWriteRegH(0x00b6, 0x0000);
                    state = 4;
                }
                else if( readywriteSize < 9500000  && (state != 5))
                {
                    ithWriteRegH(0x00b6, 0x00015);
                    state = 5;
                }
#endif                                       
                if( tsaf_get_duration(&timeout) > (CACHE_GET_SAMPLE_TIME_DELAY << 2) )
                {
                    tsaf_msg(1, " cache (%d-th) buffer: No data input (%d ms) !\n", cache_index, tsaf_get_duration(&timeout));
#if defined(CFG_SYSTEM_TRANWO_IPTV)
                    tsaf_set_is_encoder_alive(false);
#endif

#if defined(CFG_SYSTEM_PESI_IPTV)
                    mmpSpiSetControlMode(0, SPI_CONTROL_SLAVE);
                    xCpuIO_ReadMemoryUInt32(
                        (uint32_t)&xcpu_version,
                        (uint32_t)MEM_SHARED_INFO,
                        4);
                    mmpSpiResetControl(0);
                    if( xcpu_version != 0x1111 )
                    {
                        printf("Encoder ERROR!!!\n");
                        ithGpioSet(TS_IN_LED0);
                        ithGpioSet(TS_IN_LED1);
                    }
                    else if( no_data == false )
                    {
                        ithGpioSet(TS_IN_LED0);
                        ithGpioClear(TS_IN_LED1);
                        no_data = true;
                    }
#endif                    
                    //mmpSpiSetControlMode(SPI_CONTROL_SLAVE);
                    //printf("check 9919 HDMI state: 0x%X, 0x%X\n",xCpuIO_ReadRegister(0x16B4), xCpuIO_ReadRegister(0x16B6));
                }

                //--------------------------------
                // get ts sample
               
                if( getTsStreamFunc )       getTsStreamFunc(pInitParam->tsi_index, &pSampleAddr, &sampleSize, 0);
                //printf("sample size: %d\n", sampleSize);
#if 0
                if(++i <= 100)
                {
                    ret = fwrite(pSampleAddr, 1, sampleSize, f_ts);
                }
                else
                {
                    ret = fclose(f_ts);
                    if (!ret)
                        printf("fclose: %d\n", ret);
                }
#endif
                //----------------------------------
                // Copy to cache buffer
                if( (int)sampleSize > 0 )
                {
                    uint32_t    remainderSize = 0;
                    
#if defined(CFG_SYSTEM_PESI_IPTV)
                    //printf("!!!state %d\n", pState);
                    if( pState !=  ITX_STATE_STOP_ENCODING )
                    {
                        ithGpioSet(TS_IN_LED1);
                        ithGpioClear(TS_IN_LED0);
                    }
                    else if( pState !=  ITX_STATE_ENCODING )
                    {
                        ithGpioSet(TS_IN_LED0);
                        ithGpioClear(TS_IN_LED1);
                    }
#endif                    
                    tsaf_get_clock(&timeout);

                    if( (sampleSize > readywriteSize) && (readywriteSize > 0))            
                        stream_error = true;

                    //tsaf_msg(1, "sampleSize[%d]=%d\n", cache_index, sampleSize);
                    remainderSize = (CACHE_BUFFER_SIZE - pCacheBufMgr->cacheBuf_WPtr);

                    if( sampleSize > remainderSize )
                    {
                        //printf("1 buffer 0x%X : %X %X %X %X \n",pSampleAddr ,pSampleAddr[0], pSampleAddr[1], pSampleAddr[2], pSampleAddr[3]);
                        // swap copy
                        memcpy(pCacheBufMgr->pCacheBuf + pCacheBufMgr->cacheBuf_WPtr, pSampleAddr, remainderSize);
                        memcpy(pCacheBufMgr->pCacheBuf, pSampleAddr + remainderSize, sampleSize - remainderSize);

                        _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);
                        pCacheBufMgr->cacheBuf_WPtr = (sampleSize - remainderSize);
                        _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);

                        tsaf_msg(0, "..... cache (%d-th) buf WPtr=0\n", cache_index);
                    }
                    else
                    {
                        //printf("2 buffer 0x%X: %X %X %X %X \n",pSampleAddr,pSampleAddr[0], pSampleAddr[1], pSampleAddr[2], pSampleAddr[3]);
                        memcpy(pCacheBufMgr->pCacheBuf + pCacheBufMgr->cacheBuf_WPtr, pSampleAddr, sampleSize);

                        _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);
                        pCacheBufMgr->cacheBuf_WPtr += sampleSize;
                        _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);
                    }

                    if( tsaf_get_duration(&timeout_t) > 3000 )
                    {
                        
#if defined(CFG_SYSTEM_TRANWO_IPTV)
                        tsaf_set_is_encoder_alive(true);
#endif
                        //_tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);
                        tsaf_GetSTCtime(&stc_time);                        
                        //_tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);
                        printf("foruse: %d, sample: %d, stctime: %d\n", readywriteSize, sampleSize, stc_time/1000);
                        tsaf_get_clock(&timeout_t);

#if defined(CFG_SYSTEM_PESI_IPTV)                        
                        mmpSpiSetControlMode(0, SPI_CONTROL_SLAVE);
                        itxGetState(&pState);
                        mmpSpiResetControl(0);
#endif                        
                    }
                }

                usleep(waiting_time);
            }
        }
        else
        {
            usleep(2000);
        }
    }

    //--------------------------
    // reset global parameters
    if( pCacheBufMgr->pCacheBuf )
    {   
        
        free(pCacheBufMgr->pCacheBuf);
        pCacheBufMgr->pCacheBuf = NULL;
    }

    _tsaf_trace_leave(TSAF_MSG_TYPE_TRACE_CACHE_BUF);

    _tsaf_mutex_deinit(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheBufAccessMutex);
    // _tsaf_mutex_deinit(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheThreadStopMutex);
    // _tsaf_mutex_deinit(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pCacheBufMgr->cacheStopMutex);

    
    memset(pCacheBufMgr, 0x0, sizeof(CACHE_BUF_MGR));
    _tsaf_thread_exit(NULL);
    return 0;
}

static int
_default_cache_StopThread(
    uint32_t            index,
    TSAF_CRTL_INFO      *pTsafCtrlInfo)
{
    _tsaf_trace_enter(TSAF_MSG_TYPE_TRACE_CACHE_BUF, "\n");

    pTsafCtrlInfo->cacheBufMgr.bStartCacheThread = false;

    _tsaf_trace_leave(TSAF_MSG_TYPE_TRACE_CACHE_BUF);

    return 0;
}

static int
_default_cache_Stop(
    uint32_t            index,
    TSAF_CRTL_INFO      *pTsafCtrlInfo)
{
    

    _tsaf_trace_enter(TSAF_MSG_TYPE_TRACE_CACHE_BUF, "\n");

    pTsafCtrlInfo->cacheBufMgr.bStartCache = false;
    //tsd_Control((TSD_HANDLE*)pTsafCtrlInfo->extra_handle, TSD_CTRL_DISABLE_TSI, 0, 0);

    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pTsafCtrlInfo->cacheBufMgr.cacheBufAccessMutex);
    pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr = pTsafCtrlInfo->cacheBufMgr.cacheBuf_WPtr = 0;
    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pTsafCtrlInfo->cacheBufMgr.cacheBufAccessMutex);

    _tsaf_trace_leave(TSAF_MSG_TYPE_TRACE_CACHE_BUF);

    return 0;
}

static int
_default_cache_Start(
    uint32_t            index,
    TSAF_CRTL_INFO      *pTsafCtrlInfo)
{
    _tsaf_trace_enter(TSAF_MSG_TYPE_TRACE_CACHE_BUF, "\n");

    pTsafCtrlInfo->cacheBufMgr.bStartCache = true;
    //tsd_Control((TSD_HANDLE*)pTsafCtrlInfo->extra_handle, TSD_CTRL_ENABLE_TSI, 0, 0);

    _tsaf_trace_leave(TSAF_MSG_TYPE_TRACE_CACHE_BUF);
    return 0;
}

static uint32_t
_default_cache_GetDataSize(
    uint32_t            index,
    TSAF_CRTL_INFO      *pTsafCtrlInfo)
{
    uint32_t    validSize = 0;

    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pTsafCtrlInfo->cacheBufMgr.cacheBufAccessMutex);

    if( pTsafCtrlInfo->cacheBufMgr.cacheBuf_WPtr < pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr )
    {
        validSize = (CACHE_BUFFER_SIZE - pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr) + pTsafCtrlInfo->cacheBufMgr.cacheBuf_WPtr;
    }
    else
    {
        validSize = pTsafCtrlInfo->cacheBufMgr.cacheBuf_WPtr - pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr;
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pTsafCtrlInfo->cacheBufMgr.cacheBufAccessMutex);

    return validSize;
}

static int
_default_cache_GetData(
    uint32_t        index,
    uint8_t         *buffer,
    uint32_t        bufferLength,
    uint32_t        *pRealLength,
    TSAF_CRTL_INFO  *pTsafCtrlInfo)
{
    int     ret = -1;
    struct timeval timeout;

    
    _tsaf_trace_enter(TSAF_MSG_TYPE_TRACE_CACHE_BUF, "\n");

    tsaf_get_clock(&timeout);
    while( tsaf_get_duration(&timeout) < 15000 )
    {
        uint32_t    validSize = 0;

        if( !IS_START_CACHE(index, &pTsafCtrlInfo->cacheBufMgr) )    break;

        validSize = _default_cache_GetDataSize(index, pTsafCtrlInfo);
        if( validSize < bufferLength )
        {
            
            // usleep(1000);
            if( pRealLength )       *pRealLength = 0;
        }
        else
        {
            
            uint32_t    ReadRemainderSize = 0;

            ReadRemainderSize = (CACHE_BUFFER_SIZE - pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr);

            if( ReadRemainderSize < bufferLength )
            {
                // swap copy
                memcpy(buffer, pTsafCtrlInfo->cacheBufMgr.pCacheBuf + pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr, ReadRemainderSize);
                memcpy(buffer + ReadRemainderSize, pTsafCtrlInfo->cacheBufMgr.pCacheBuf, bufferLength - ReadRemainderSize);

                // _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, g_cacheBufMgr[index].cacheBufAccessMutex);
                pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr = (bufferLength - ReadRemainderSize);
                // _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, g_cacheBufMgr[index].cacheBufAccessMutex);
            }
            else
            {
                memcpy(buffer, pTsafCtrlInfo->cacheBufMgr.pCacheBuf + pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr, bufferLength);

                // _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, g_cacheBufMgr[index].cacheBufAccessMutex);
                pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr += bufferLength;
                // _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, g_cacheBufMgr[index].cacheBufAccessMutex);
            }

            ret = 0;
            if( pRealLength )       *pRealLength = bufferLength;
            break;
        }
    }

    _tsaf_trace_leave(TSAF_MSG_TYPE_TRACE_CACHE_BUF);
    return ret;
}

//////////////////////////////////////
static uint32_t
_get_Ts_Stream(
    uint32_t    tsi_idx,
    uint8_t     **sampleAddr,
    uint32_t    *sampleLength,
    void        *extraData)
{
    uint32_t    result = 0;
    static uint32_t last_writeIndex = 0;
    
    taskStreamMux_GetBufferInfo(sampleAddr, sampleLength);

    if(sampleAddr != NULL)
    {
        uint32_t writeIndex = taskStreamMux_GetWriteIndex();

        if(writeIndex != last_writeIndex)
        {
            if(last_writeIndex < writeIndex)
            {
                *sampleAddr += last_writeIndex*TRANSPORT_PACKET_TOTAL_SIZE;
                *sampleLength = TRANSPORT_PACKET_TOTAL_SIZE * (writeIndex - last_writeIndex);
                last_writeIndex = writeIndex;
            }
            else
            {
                *sampleAddr += last_writeIndex*TRANSPORT_PACKET_TOTAL_SIZE;
                *sampleLength = TRANSPORT_PACKET_TOTAL_SIZE * (MUX_BUFFER_VALID_COUNT - last_writeIndex);
                last_writeIndex = 0;
            }
        }
        else
        {
            *sampleLength = 0;
            //printf("same write index wait...\n");
        }
    }
    else
    {
        printf("Null sample addr\n");
        *sampleLength = 0;
    }

    if( sampleLength )  
    {
        result = *sampleLength;
    }
    else
    {
        printf("No Sample??\n");
    }

    return result;
}

static TSAF_CACHE_INFO     g_def_cacheInfo =
{
    _default_cache_BufThread,
    _default_cache_StopThread,
    _default_cache_Stop,
    _default_cache_Start,
    _default_cache_GetData,
    0
};
#else
    #define _get_Ts_Stream     0
    static TSAF_CACHE_INFO     g_def_cacheInfo = {0};
#endif

static void
_tsaf_Register_all(
    void)
{
    static bool bInitialized = false;

    if( bInitialized )
        return;
    bInitialized = true;

    REGISTER_ITEM(TS_AIRFILE_DESC, PURETS_IPTV, purets_iptv);
}

//=============================================================================
//                  Public Function Definition
//=============================================================================
TSAF_ERR
tsaf_CreateHandle(
    TSAF_HANDLE         **pHTsaf,
    TSAF_SETUP_INFO     *pSetupInfo,
    void                *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    do{
        TS_AIRFILE_TYPE_ID      typeId = 0;

        if( *pHTsaf != 0 )
        {
            tsaf_msg_ex(TSAF_MSG_TYPE_ERR, " error, Exist uiEnc handle !!");
            result = TSAF_ERR_INVALID_PARAMETER;
            break;
        }

        // ------------------------
        // craete dev info
        pTsafDev = tsaf_malloc(sizeof(TSAF_DEV));
        if( !pTsafDev )
        {
            tsaf_msg_ex(TSAF_MSG_TYPE_ERR, " error, allocate fail !!");
            result = TSAF_ERR_ALLOCATE_FAIL;
            break;
        }

        memset((void*)pTsafDev, 0x0, sizeof(TSAF_DEV));

        switch( pSetupInfo->insrcType )
        {
            case TSAF_INSRC_PURETS_IPTV:        typeId = TS_AIRFILE_ID_PURETS_IPTV;     break;
            default :                           typeId = TS_AIRFILE_ID_UNKNOW;          break;
        }

        //----------------------------------------
        // init parameters
        _tsaf_Register_all();

        pTsafDev->pCurDesc = FIND_DESC_ITEM(TS_AIRFILE_DESC, typeId);
        if( !pTsafDev->pCurDesc )
        {
            tsaf_msg_ex(TSAF_MSG_TYPE_ERR, " error, can't find descriptor !!");
            result = TSAF_ERR_INVALID_PARAMETER;
            break;
        }

        // -------------------------------
        // create mutex
        _tsaf_mutex_init(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

        // if not error
        (*pHTsaf) = &pTsafDev->hTsaf;

    }while(0);

    if( result != TSAF_ERR_OK )
    {
        if( pTsafDev )
        {
            TSAF_HANDLE   *pHTmp = &pTsafDev->hTsaf;
            tsaf_DestroyHandle(&pHTmp);
        }
        pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }
    return result;
}

TSAF_ERR
tsaf_DestroyHandle(
    TSAF_HANDLE     **pHTsaf)
{
    TSAF_ERR            result = TSAF_ERR_OK;
    TSAF_DEV            *pTsafDev = 0;
    pthread_mutex_t     tsaf_mutex = 0;

    /**
     * Ap layer need to check all threads, which assess this handle, in STOP state.
     * Or system maybe crash.
     **/
     
    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);

    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    if( pTsafDev )
    {
        tsaf_mutex = pTsafDev->tsaf_mutex;
        free(pTsafDev);
        *pHTsaf = 0;
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, tsaf_mutex);

    // de-init mutex
    _tsaf_mutex_deinit(TSAF_MSG_TYPE_TRACE_TSAF, tsaf_mutex);
    return result;
}

TSAF_ERR
tsaf_Init_Ts(
    TSAF_HANDLE     **pHTsaf,
    void            *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    if( pTsafDev && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        // set cache buffer info
        pTsafDev->initParam.threadFunc      = g_def_cacheInfo.cache_BufThread;
        pTsafDev->initParam.getTsStreamFunc = _get_Ts_Stream;
        pTsafDev->initParam.pCacheBufMgr    = &pTsafDev->tsafCtrlInfo.cacheBufMgr;
        pTsafDev->initParam.waiting_time    = CACHE_GET_SAMPLE_TIME_DELAY;

        // assign max cache buffer size
        pTsafDev->tsafCtrlInfo.cacheBufMgr.maxCacheBufSize = CACHE_BUFFER_SIZE;

        // assign cache buffer operator
        pTsafDev->tsafCtrlInfo.cacheInfo = g_def_cacheInfo;

        //pTsafDev->tsafCtrlInfo.tsi_index = pTsafDev->tsi_index;

        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_init )
            result = pTsafDev->pCurDesc->taf_init(&pTsafDev->tsafCtrlInfo, &pTsafDev->initParam, 0);

        
        if( pTsafDev->cacheInfo.cache_Stop )
            pTsafDev->cacheInfo.cache_Stop(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);

        pTsafDev->cacheInfo = pTsafDev->tsafCtrlInfo.cacheInfo;

    }

    if( result != TSAF_ERR_OK )
    {
        pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}

TSAF_ERR
tsaf_deInit_Ts(
    TSAF_HANDLE     **pHTsaf,
    void            *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    if( pTsafDev && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_deinit )
        {
            
            if( pTsafDev->cacheInfo.cache_Stop )
                pTsafDev->cacheInfo.cache_Stop(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);

            if( pTsafDev->cacheInfo.cache_StopThread )
                pTsafDev->cacheInfo.cache_StopThread(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);

            _tsaf_thread_join(&pTsafDev->tsafCtrlInfo.thread, 0);

            result = pTsafDev->pCurDesc->taf_deinit(&pTsafDev->tsafCtrlInfo, 0);
        }
    }

    if( result != TSAF_ERR_OK )
    {
        pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}

TSAF_ERR
tsaf_Open(
    TSAF_HANDLE     **pHTsaf,
    uint32_t        service_index,
    void            *extraData)
{    
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    if( pTsafDev && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        //f_ts = fopen("A:/udp_0001.ts", "wb");
        if( pTsafDev->cacheInfo.cache_Stop )
            pTsafDev->cacheInfo.cache_Stop(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);

        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_open )
        {
            result = pTsafDev->pCurDesc->taf_open(&pTsafDev->tsafCtrlInfo, service_index, 0);
        }

        if( pTsafDev->cacheInfo.cache_Start )
            pTsafDev->cacheInfo.cache_Start(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);
    }

    if( result != TSAF_ERR_OK )
    {
        //pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }
    else
        pTsafDev->hTsaf.state = TSAF_STATE_READY;    

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}

TSAF_ERR
tsaf_Close(
    TSAF_HANDLE     **pHTsaf,
    void            *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    if( pTsafDev && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        
        if( pTsafDev->cacheInfo.cache_Stop )
            pTsafDev->cacheInfo.cache_Stop(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);

        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_close )
            result = pTsafDev->pCurDesc->taf_close(&pTsafDev->tsafCtrlInfo, 0);
    }

    if( result != TSAF_ERR_OK )
    {
        //pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }
    else
        pTsafDev->hTsaf.state = TSAF_STATE_IDLE;    

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}


TSAF_ERR
tsaf_Read(
    TSAF_HANDLE     **pHTsaf,
    uint8_t         *pDstBuf,
    int             length,
    uint32_t        *realLength,
    void            *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    //taskStreamMux_GetBufferInfo(&pDstBuf, &realLength);

    if( pTsafDev && pDstBuf && realLength && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_read )
        {
            int rst = 0;
            
            rst = pTsafDev->pCurDesc->taf_read(&pTsafDev->tsafCtrlInfo, pDstBuf, length, realLength, 0);

            if( rst < 0 )   *realLength = 0;
        }
    }

    if( result != TSAF_ERR_OK )
    {
        pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}


TSAF_ERR
tsaf_Write(
    TSAF_HANDLE     **pHTsaf,
    void            *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    if( pTsafDev && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_write )
            result = pTsafDev->pCurDesc->taf_write(&pTsafDev->tsafCtrlInfo, 0);
    }

    if( result != TSAF_ERR_OK )
    {
        pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}

TSAF_ERR
tsaf_Scan_Freq(
    TSAF_HANDLE     **pHTsaf,
    TSAF_SCAN_PARAM *pScanParam,
    void            *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    if( pTsafDev && pScanParam && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        pTsafDev->tsafCtrlInfo.scanParam = *(pScanParam);

        pTsafDev->hTsaf.state = TSAF_STATE_SCANNING;

        
        if( pTsafDev->cacheInfo.cache_Stop )
            pTsafDev->cacheInfo.cache_Stop(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);

        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_scanFreq )
            result = pTsafDev->pCurDesc->taf_scanFreq(&pTsafDev->tsafCtrlInfo, 0);

        //if( pTsafDev->cacheInfo.cache_Start )
        //    pTsafDev->cacheInfo.cache_Start(pTsafDev->hTsaf.demodId, &pTsafDev->tsafCtrlInfo);
    }

    if( result != TSAF_ERR_OK )
    {
        //pTsafDev->situation = TSAF_SITUATION_FAIL;
        pTsafDev->hTsaf.state = TSAF_STATE_IDLE;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}

TSAF_ERR
tsaf_Control(
    TSAF_HANDLE     **pHTsaf,
    uint32_t        cmd,
    uint32_t        *value,
    void            *extraData)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    TSAF_DEV    *pTsafDev = 0;

    _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), 0, result);

    pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
    _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);

    if( pTsafDev && pTsafDev->situation != TSAF_SITUATION_FAIL )
    {
        if( pTsafDev->pCurDesc && pTsafDev->pCurDesc->taf_control )
            result = pTsafDev->pCurDesc->taf_control(&pTsafDev->tsafCtrlInfo, cmd, value, 0);
    }

    if( result != TSAF_ERR_OK )
    {
        pTsafDev->situation = TSAF_SITUATION_FAIL;
        tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
    }

    _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
    return result;
}

TSAF_ERR
tsaf_StreamStatus(
    TSAF_HANDLE     **pHTsaf)
{
    TSAF_ERR    result = TSAF_ERR_OK;

    if( stream_error == true)
    {
        result = TSAF_ERR_ALLOCATE_FAIL;
        stream_error = false;
        printf("Memory reset!!!\n");
    }   
    return result;
}

TSAF_ERR
tsaf_GetSTCtime(
    uint32_t* stc_time)
{
    TSAF_ERR    result = TSAF_ERR_OK;
    //*stc_time = STCGetTime();
    *stc_time = 0;

    return result;
}

// TSAF_ERR
// tsaf_tamplete(
//     TSAF_HANDLE     **pHTsaf,
//     void            *extraData)
// {
//     TSAF_ERR    result = TSAF_ERR_OK;
//     TSAF_DEV    *pTsafDev = 0;
//
//     _tsaf_verify_handle(TSAF_MSG_TYPE_TRACE_TSAF, (*pHTsaf), pTsafDev->tsaf_mutex, result);
//
//     pTsafDev = DOWN_CAST(TSAF_DEV, (*pHTsaf), hTsaf);
//
//     _tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
//
//     if( pTsafDev && pTsafDev->situation != TSAF_SITUATION_FAIL )
//     {
//
//     }
//
//     if( result != TSAF_ERR_OK )
//     {
//         pTsafDev->situation = TSAF_SITUATION_FAIL;
//         tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "%s() err 0x%x !", __FUNCTION__, result);
//     }
//
//     _tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_TSAF, pTsafDev->tsaf_mutex);
//     return result;
// }

void
tsaf_set_stream_hdcp(
	bool state)
{
	gbstreamhdcpon = state;
}

bool
tsaf_get_stream_hdcp(
	void)
{
	return gbstreamhdcpon;
}

#if defined(CFG_SYSTEM_TRANWO_IPTV)
void
tsaf_set_is_encoder_alive(
    bool state)
{
    gbIsEncoderAlive = state;
}

bool
tsaf_get_is_encoder_alive(
    void)
{
    return gbIsEncoderAlive;
}
#endif

