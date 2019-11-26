
#include <unistd.h>
#include <pthread.h>
#include "ts_airfile.h"
#include "ite/itp.h"
//#include "mmp_tsi.h"

#if (CONFIG_TS_AIRFILE_DESC_PURETS_IPTV_DESC)

//=============================================================================
//                  Constant Definition
//=============================================================================

typedef enum USB_FILL_BUF_STATE_TAG
{
    USB_FILL_BUF_STOP = 0,
    USB_FILL_BUF_START,
    USB_FILL_BUF_STOPPING,

} USB_FILL_BUF_STAGE;

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct _PURETS_BASE_INFO_TAG
{
    const char          *name;
    bool                bUsed;

    const bool          bUsb_tsi;
    const uint32_t      tsi_index;
    const uint32_t      channlTotalCnt;

}PURETS_BASE_INFO;

typedef struct _TS_PRESET_INFO_TAG
{
    const char          *name;
    uint32_t            ts_stream_id;
    int                 tsi_id;

}TS_PRESET_INFO;

typedef struct _USB_TS_BUF_MGR_TAG
{
    pthread_mutex_t     usbBufMutex;
    bool                bFirstSample;
    bool                bFirstSampleStop;
    uint32_t            usbBuf_RPtr;
    uint32_t            usbBuf_WPtr;
    uint8_t             *pUsbTsBuf;
    uint32_t            bufferSize;
    USB_FILL_BUF_STAGE  bufState;

} USB_TS_BUF_MGR;

//=============================================================================
//                  Global Data Definition
//=============================================================================
static PURETS_BASE_INFO  g_purets_base_info[] =
{
    {"TS0",    false, false, 0,    2},
    {"TS1",    false, false, 0,    2},
    {"TS2",    false, true,  (-1), 2},
    {"TS3",    false, true,  (-1), 2},
};


//=============================================================================
//                  Private Function Definition
//============================================================================= 
static uint32_t
_tsi_cache_GetDataSize(
    uint32_t            index,
    TSAF_CRTL_INFO      *pTsafCtrlInfo)
{
    uint32_t    validSize = 0;
    uint32_t    maxCacheBufSize = pTsafCtrlInfo->cacheBufMgr.maxCacheBufSize;

    //_tsaf_mutex_lock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pTsafCtrlInfo->cacheBufMgr.cacheBufAccessMutex);

    if( pTsafCtrlInfo->cacheBufMgr.cacheBuf_WPtr < pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr )
    {
        validSize = (maxCacheBufSize - pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr) + pTsafCtrlInfo->cacheBufMgr.cacheBuf_WPtr;
    }
    else
    {
        validSize = pTsafCtrlInfo->cacheBufMgr.cacheBuf_WPtr - pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr;
    }

    //_tsaf_mutex_unlock(TSAF_MSG_TYPE_TRACE_CACHE_BUF, pTsafCtrlInfo->cacheBufMgr.cacheBufAccessMutex);

    return validSize;
}

#if defined(CFG_SYSTEM_PESI_IPTV)
#define START_DUMMY_LOOP_CNT 4048
#define DUMMY_ADJUST_RANGE 800
bool first_get_data = true;
uint32_t airfile_dummy_cnt = START_DUMMY_LOOP_CNT;

#define airfile_thold 100*1024
#endif
struct timeval buff_chk_startT = {0};

static int
_tsi_cache_NoBlockingGetData(
    uint32_t        index,
    uint8_t         *buffer,
    uint32_t        bufferLength,
    uint32_t        *pRealLength,
    TSAF_CRTL_INFO  *pTsafCtrlInfo)
{
    int         ret = (-1);
    uint32_t    duration_time = 0;
    struct timeval currT = {0};
    uint32_t    ReadRemainderSize = 0;
    uint32_t    maxCacheBufSize = pTsafCtrlInfo->cacheBufMgr.maxCacheBufSize;
#if defined(CFG_SYSTEM_PESI_IPTV)
    uint32_t    getdatasize = _tsi_cache_GetDataSize(index, pTsafCtrlInfo);
    uint32_t    i = 0;
    
    if( pRealLength )   *pRealLength = 0;

    if(first_get_data)
    {
        if(getdatasize < airfile_thold)
        {
            for (i = 0; i < 2048; i++) { asm(""); }
            return ret;
        }
        else
           first_get_data = false; 
    }

    if(getdatasize < 100*1024)
    {
        first_get_data = true;
        
        for (i = 0; i < 2048; i++) { asm(""); }
        return ret;
    }
#else
	if( pRealLength )   *pRealLength = 0;

	if( _tsi_cache_GetDataSize(index, pTsafCtrlInfo) < bufferLength )
    {
        usleep(1000);
        return ret;
    }
#endif
    ReadRemainderSize = (maxCacheBufSize - pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr);

    if( ReadRemainderSize < bufferLength )
    {
        // swap copy
        memcpy(buffer, pTsafCtrlInfo->cacheBufMgr.pCacheBuf + pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr, ReadRemainderSize);
        memcpy(buffer + ReadRemainderSize, pTsafCtrlInfo->cacheBufMgr.pCacheBuf, bufferLength - ReadRemainderSize);
        pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr = (bufferLength - ReadRemainderSize);
    }
    else
    {
        memcpy(buffer, pTsafCtrlInfo->cacheBufMgr.pCacheBuf + pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr, bufferLength);
        pTsafCtrlInfo->cacheBufMgr.cacheBuf_RPtr += bufferLength;
    }

    if( pRealLength )   *pRealLength = bufferLength;

    ret = 0;
    gettimeofday(&currT, NULL);
    //duration_time = (currT.tv_sec - buff_chk_startT.tv_sec) * 1000;
    if(duration_time = itpTimevalDiff(&buff_chk_startT, &currT) > 2000)
    {
        printf("live555 get data time = %d!!!\n", duration_time);
    }

    gettimeofday(&buff_chk_startT, NULL);
    return ret;
}

////////////////////////////////////////////////

static TSAF_ERR
purets_iptv_Init(
    TSAF_CRTL_INFO  *pCtrlInfo,
    TSAF_INIT_PARAM *pInitParam,
    void            *extraDat)
{
    TSAF_ERR            result = TSAF_ERR_OK;
    TS_PRESET_INFO      presetInfo = {0};

    do{
        int                 i, rc;
        THREAD_ROUTINE_FUNC  _routine_func = 0;


        //---------------------------------------
        // auto select un-used ts device
        for(i = 0; i < MAX_TSI_SUPPORT_CNT; i++)
        {
            if( g_purets_base_info[i].bUsed == false )
            {
                presetInfo.ts_stream_id = i;
                presetInfo.tsi_id   = g_purets_base_info[i].tsi_index;

                g_purets_base_info[i].bUsed = true;
                break;
            }
        }

        pInitParam->ts_stream_index = presetInfo.ts_stream_id;
        pCtrlInfo->tsi_index = pInitParam->tsi_index = presetInfo.tsi_id;
        
        if( g_purets_base_info[pCtrlInfo->tsi_index].bUsb_tsi)
        {
            USB_TS_BUF_MGR      *pUsbBufMgr = 0;
        
            //***to be define***//
        }
        else
        {
            _routine_func = pInitParam->threadFunc;

            //mmpTsiInitialize(presetInfo.tsi_id);

            // -------------------------
            // note: tsi module access operator use default
            //       L2 cache buffer operator use default
            pCtrlInfo->cacheInfo.cache_GetData = _tsi_cache_NoBlockingGetData;

            // -------------------------
            // Create thread
            rc = pthread_create(&pInitParam->thread, NULL, _routine_func, (void *)pInitParam);
            if( rc )
            {
                tsaf_msg_ex(TSAF_MSG_TYPE_ERR, "ERROR; pthread_create() fail %d\n", rc);
            }

            pCtrlInfo->thread = pInitParam->thread;

            usleep(5000);
        }

    }while(0) ;

    return result;
}

static TSAF_ERR
purets_iptv_deInit(
    TSAF_CRTL_INFO  *pCtrlInfo,
    void            *extraDat)
{
    TSAF_ERR     result = TSAF_ERR_OK;

    g_purets_base_info[pCtrlInfo->tsi_index].bUsed = false;

    // destroy thread


    return result;
}

static TSAF_ERR
purets_iptv_Open(
    TSAF_CRTL_INFO  *pCtrlInfo,
    uint32_t        service_index,
    void            *extraDat)
{
    TSAF_ERR     result = TSAF_ERR_OK;

    //mmpTsiEnable(pCtrlInfo->tsi_index);
    return result;
}

static TSAF_ERR
purets_iptv_Close(
    TSAF_CRTL_INFO  *pCtrlInfo,
    void            *extraDat)
{
    TSAF_ERR     result = TSAF_ERR_OK;

    //mmpTsiDisable(pCtrlInfo->tsi_index);
    return result;
}

static int
purets_iptv_readData(
    TSAF_CRTL_INFO  *pCtrlInfo,
    uint8_t         *pDstBuf,
    int             length,
    uint32_t        *pRealLength,
    void            *extraDat)
{
    int         ret = -1;
    uint32_t    real_size = 0;

    if( pCtrlInfo->cacheInfo.cache_GetData )
    {
        ret = pCtrlInfo->cacheInfo.cache_GetData(pCtrlInfo->tsi_index, pDstBuf, length, &real_size, pCtrlInfo);
        if( ret < 0)   real_size = 0;
        //printf("%s %d realsize:%d\n",__FILE__,__LINE__,real_size);
    }

    if( pRealLength )       *pRealLength = real_size;

    return ret;
}

static TSAF_ERR
purets_iptv_get_inSrc_func(
    TSAF_CRTL_INFO  *pCtrlInfo,
    pFunc_inSrc     *ppFunc,
    void            *extraDat)
{
    TSAF_ERR     result = TSAF_ERR_OK;
    return result;
}

static TSAF_ERR
purets_iptv_control(
    TSAF_CRTL_INFO  *pCtrlInfo,
    uint32_t        cmd,
    uint32_t        *value,
    void            *extraDat)
{
    TSAF_ERR     result = TSAF_ERR_OK;

    switch( cmd )
    {
        default:        break;
    }
    return result;
}

//=============================================================================
//                  Public Function Definition
//=============================================================================

TS_AIRFILE_DESC  TS_AIRFILE_DESC_purets_iptv_desc =
{
    "ts air file purets iptv",      // char        *name;
    0,                          // struct _TS_AIRFILE_DESC_TAG  *next;
    TS_AIRFILE_ID_PURETS_IPTV,      // TS_AIRFILE_TYPE_ID           id;
    0,                          // void        *privInfo;

    // // operator
    purets_iptv_Init,               // int     (*taf_init)(TSAF_CRTL_INFO *pCtrlInfo, TSAF_INIT_PARAM *pInitParam, void *extraData);
    purets_iptv_deInit,             // int     (*taf_deinit)(TSAF_CRTL_INFO *pCtrlInfo, void *extraData);

    purets_iptv_Open,               // int     (*taf_open)(TSAF_CRTL_INFO *pCtrlInfo, void *extraData);
    purets_iptv_Close,              // int     (*taf_close)(TSAF_CRTL_INFO *pCtrlInfo, void *extraData);
    purets_iptv_readData,           // int     (*taf_read)(TSAF_CRTL_INFO *pCtrlInfo, void *extraData);
    0,                          // int     (*taf_write)(TSAF_CRTL_INFO *pCtrlInfo, void *extraData);
    purets_iptv_get_inSrc_func,     // int     (*taf_get_inSrc_func)(TSAF_CRTL_INFO *pCtrlInfo, pFunc_inSrc *ppFunc, void *extraData);
    0,           // int     (*taf_scanFreq)(TSAF_CRTL_INFO *pCtrlInfo, void *extraData);
    purets_iptv_control,            // int     (*taf_control)(TSAF_CRTL_INFO *pCtrlInfo, uint32_t cmd, uint32_t *value, void *extraData);
};
#else

TS_AIRFILE_DESC  TS_AIRFILE_DESC_purets_iptv_desc =
{
    "ts air file purets iptv",      // char        *name;
    0,                          // struct _TS_AIRFILE_DESC_TAG  *next;
    TS_AIRFILE_ID_PURETS_IPTV,      // TS_AIRFILE_TYPE_ID           id;
    0,                          // void        *privInfo;
};
#endif

