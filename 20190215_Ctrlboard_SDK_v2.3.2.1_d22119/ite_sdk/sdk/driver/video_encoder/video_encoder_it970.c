#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "video_encoder/video_encoder_it970.h"

#include "h264encapi.h"
#include "ite/ith.h"
#include "capture_970/capture_types.h"
#include "capture_970/mmp_capture.h"
#include "mmp_sensor.h"
/*#define SENSOR_WIDTH 0
#define SENSOR_HEIGHT 0
#define SENSOR_BITRATE 0
#define SENSOR_WIDTH_2 0
#define SENSOR_HEIGHT_2 0
#define SENSOR_BITRATE_2 0*/
#include "ite/itp.h"

//=============================================================================
//                Constant Definition
//=============================================================================

//#define CFG_DUAL_STREAM
//#define TEST_MULTISTREAM

#ifndef CFG_DUAL_STREAM
#ifndef CFG_IPTV_TX
//#define LOW_LATENCY
#endif
//#define MULTI_SLICE
#endif

#define CAPTURE_RESULT_QUEUE_SIZE   15
#define ENCODED_RESULT_QUEUE_SIZE   CAPTURE_RESULT_QUEUE_SIZE
#define QUEUE_EMPTY                 (-1)
#define QUEUE_NOT_EMPTY             (0)
#define TIMER_NUM                   4
#if defined(CFG_DUAL_STREAM)

    #define ISP_TRANSFORM_NUM       2
    #define ENCODE_STREAM_NUM       2
    #define VIDEO_STREAM_NUM        2
    #define VIDEO_SAMPLE_NUM        7

#else

    #define ISP_TRANSFORM_NUM       1
    #define ENCODE_STREAM_NUM       1
    #define VIDEO_STREAM_NUM        1
#ifdef MULTI_SLICE
    #define VIDEO_SAMPLE_NUM        30
#else
    #define VIDEO_SAMPLE_NUM        7
#endif

#endif

#ifdef CFG_IPTV_TX
#define CAPTURE_MAXWIDTH 1920
#define CAPTURE_MAXHEIGHT 1080
#else
#define CAPTURE_MAXWIDTH 1280
#define CAPTURE_MAXHEIGHT 720
#endif

#if !defined(CFG_SENSOR_ENABLE)
#ifdef CFG_IPTV_TX
#define SENSOR_WIDTH            CAPTURE_MAXWIDTH
#define SENSOR_HEIGHT			CAPTURE_MAXHEIGHT
#else
#define SENSOR_WIDTH            1280
#define SENSOR_HEIGHT           720
#endif
#define SENSOR_BITRATE          3000
#endif

#define VIDEO_FRAME_RATE_30     30
#define VIDEO_FRAME_RATE_25     25

#if defined (CFG_SENSOR_FLICK_50HZ)
    #define    VIDEO_FRAME_RATE VIDEO_FRAME_RATE_25
#else
    #define    VIDEO_FRAME_RATE VIDEO_FRAME_RATE_30
#endif

//=============================================================================
//                Macro Definition
//=============================================================================
typedef MMP_UINT (*frameCount2TimeStamp)(MMP_UINT framecount);

//=============================================================================
//                Structure Definition
//=============================================================================

typedef enum ENCODER_TYPE_TAG
{
    JPEG_ENCODER,
    H264_ENCODER
} ENCODER_TYPE;

typedef struct CAPTURE_RESULT_TAG
{
    MMP_UINT    resIdx;
    MMP_UINT    frameCount;
    MMP_UINT    timeStamp;
    MMP_UINT    frameIndex;
    MMP_BOOL    bTopField;
    MMP_BOOL    bCapToTran;
    MMP_BOOL    bTranToEnc;
} CAPTURE_RESULT;

typedef MMP_RESULT (*doEncodeDequeue)(CAPTURE_RESULT *ptEntry);

typedef struct AVC_ENCODER_FLOW_TAG
{
    MMP_UINT                baseTimeStamp;
    MMP_UINT                currTimeStamp;
    MMP_UINT                captureFrameCount;
    MMP_UINT                encodedFrameCount;
    MMP_UINT                encodedTimeStamp;
    MMP_CAP_FRAMERATE       frameRate;
    MMP_CAP_FRAMERATE       captureFrameRate;
    MMP_CAP_FRAMERATE       detectFrameRate;
//    FRAME_RATE_SETTING_STATE    frameRateState;
    frameCount2TimeStamp    pfToTimeStamp;
    doEncodeDequeue         pfEnDequeue;
//    VIDEO_ENCODER_INPUT_INFO    encoderInfo;
    MMP_BOOL                bSkipFrame;
    MMP_UINT                ispFrameRate[ENCODE_STREAM_NUM];
    MMP_UINT                frameRateDiff[ENCODE_STREAM_NUM];
    MMP_UINT                frameCount[ENCODE_STREAM_NUM];
    MMP_UINT                skipCount[ENCODE_STREAM_NUM];
    MMP_UINT                frmRateChkCount;
    MMP_BOOL                encodeQue[5];
    MMP_UINT                maxFrmRateIdx;
} AVC_ENCODER_FLOW;

typedef struct CAPTURE_RESULT_QUEUE_TAG
{
    CAPTURE_RESULT  entry[CAPTURE_RESULT_QUEUE_SIZE];
    MMP_UINT        wIdx;
    MMP_UINT        rIdx;
} CAPTURE_RESULT_QUEUE;

typedef struct ENCODED_RESULT_TAG
{
    MMP_UINT    frameCount;
    MMP_UINT    timeStamp;
    MMP_UINT8   *pData;
    MMP_UINT    dataSize;
    MMP_UINT    InstanceNum;
} ENCODED_RESULT;

typedef struct ENCODED_RESULT_QUEUE_TAG
{
    ENCODED_RESULT  entry[ENCODED_RESULT_QUEUE_SIZE];
    MMP_UINT        wIdx;
    MMP_UINT        rIdx;
} ENCODED_RESULT_QUEUE;

/**
 *  Device Select
 */
typedef enum CAPTURE_DEVICE_TAG
{
    CAPTURE_DEV_UNKNOW          = MMP_CAP_UNKNOW_DEVICE,
    CAPTURE_DEV_ADV7180         = MMP_CAP_DEV_ADV7180,
    CAPTURE_DEV_CAT9883         = MMP_CAP_DEV_CAT9883,
    CAPTURE_DEV_HDMIRX          = MMP_CAP_DEV_HDMIRX,
    CAPTURE_DEV_SENSOR          = MMP_CAP_DEV_SENSOR,
} CAPTURE_DEVICE;

typedef struct ISP_TRANSFORM_PARAMETER_TAG
{
    MMP_UINT16  inWidth;
    MMP_UINT16  inHeight;
    MMP_UINT32  inAddrY[3];
    MMP_UINT32  inAddrUV[3];
    MMP_UINT16  outWidth;
    MMP_UINT16  outHeight;
    MMP_UINT32  outAddrY[5];
    MMP_UINT32  outAddrUV[5];
    MMP_BOOL    deinterlaceOn;
    MMP_BOOL    bframeDouble;
    MMP_BOOL    useTranBuf;
    MMP_UINT32  tranBufAddrY[3];
    MMP_UINT32  tranBufAddrUV[3];
} ISP_TRANSFORM_PARAMETER;

typedef struct H264_AVC_ENCODER_TAG
{
    H264EncInst encoder;

    MMP_UINT32  buf_vram_addr;
    MMP_UINT8*  buf_sys_addr;
    MMP_UINT32  hdr_buf_vram_addr;
    MMP_UINT8*  hdr_buf_sys_addr;
    MMP_UINT32  framecount;
    MMP_UINT8*  pHdrBufAddr[2];
    MMP_UINT8*  pStreamBufAdr[VIDEO_SAMPLE_NUM];
    MMP_UINT8*  pSourceBufAdrY[5];
    MMP_UINT8*  pSourceBufAdrU[5];
    MMP_UINT8*  pSourceBufAdrV[5];
    MMP_UINT32  ParaSetHdrSize[2];
    MMP_UINT32  streamBufCount;
    MMP_UINT32  streamBufSize;
    MMP_UINT32  sourceBufCount;
    MMP_UINT32  streamBufSelect;
    MMP_UINT32  sourceBufSelect;
    MMP_UINT32  frameWidth;
    MMP_UINT32  frameHeight;
    MMP_UINT32  framePitchY;
    MMP_UINT32  frameCropTop;
    MMP_UINT32  frameCropBottom;
    MMP_UINT32  frameCropLeft;
    MMP_UINT32  frameCropRight;
    MMP_BOOL    interlaced_frame;
    MMP_BOOL    bIFrame;
} H264_AVC_ENCODER;

//=============================================================================
//                Global Data Definition
//=============================================================================

static H264_AVC_ENCODER         *gptAVCEncoder[ENCODE_STREAM_NUM];
static AVC_ENCODER_FLOW         gtAVCEncoderFlow;
static CAPTURE_RESULT_QUEUE     gtCaptureResultQ;
static ENCODED_RESULT_QUEUE     gtEncodedResultQ;
static CAPTURE_RESULT_QUEUE     gtIspResultQ;
static MMP_BOOL                 gbEncodeFire        = MMP_FALSE;
static MMP_BOOL                 gbVideoEncoderInit  = MMP_FALSE;
static MMP_UINT32               gEncodeIdx          = 0;
static MMP_BOOL                 gbStrBufFire[VIDEO_STREAM_NUM][VIDEO_SAMPLE_NUM];
static MMP_UINT32               gVideoSelIdx[VIDEO_STREAM_NUM];
static MMP_UINT32               gVideoWrIdx[VIDEO_STREAM_NUM];
static ENCODE_PARA_CALLBACK     gfpCallback                             = MMP_NULL;
static sem_t                    gpEncoderSem;
static MMP_BOOL                 gbVideoEncoder                          = MMP_FALSE;
static MMP_BOOL                 gbStream0Enable                         = MMP_FALSE;
static MMP_BOOL                 gbStream1Enable                         = MMP_FALSE;
static MMP_BOOL                 gbJPEGEncoder                           = MMP_FALSE;
static MMP_UINT32               gJPEGBufIndex                           = 0;
static MMP_UINT32               gOpenEnginCnt                           = 0;
static pthread_mutex_t          VideoEngineMutex                        = PTHREAD_MUTEX_INITIALIZER;

static ISP_TRANSFORM_PARAMETER  gtIspTransformParm[ISP_TRANSFORM_NUM]   = {0};
static CAPTURE_RESULT           gtIspFireEntry;
static MMP_UINT32               gVideoEnWidth[2];
static MMP_UINT32               gVideoEnHeight[2];
static MMP_UINT32               gVideoEnBitRate[2];
static MMP_BOOL                 gStartgetVideo[VIDEO_STREAM_NUM];
static MMP_BOOL                 gVideoEncoding[VIDEO_STREAM_NUM];
static MMP_UINT8                gVideoUserNum[VIDEO_STREAM_NUM];

MMP_UINT32                      tickStart;
MMP_UINT32                      tickISP;
CAPTURE_HANDLE                  cap0;

#ifdef   LOG_ENCODER_PERFORMANCE
MMP_UINT32                      tickStart, tickEnd;
unsigned                        ticks_per_ms;
MMP_UINT32                      Enidx = 0;
MMP_UINT32                      EnTime;
#endif

static pthread_t                gEncoder_process_tid;
static pthread_t                gIsp_process_tid;
static ISP_DEVICE               gIspDev = 0;
static MMP_UINT32               WriteBufferIndex = 0;
static MMP_BOOL                 gbEncodeStart = MMP_FALSE;

static MMP_UINT32               stream_buf_vram_addr = 0;
static MMP_UINT8*               stream_buf_sys_addr = 0;
static MMP_UINT8*               gLastSlicePtr = 0;
static MMP_UINT32               gSliceNum = 0;

//=============================================================================
//                Private Function Definition
//=============================================================================
static unsigned int PalGetClock(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static unsigned long PalGetDuration(unsigned int clock)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return (unsigned int)(tv.tv_sec * 1000 + tv.tv_usec / 1000) - clock;
}

// Capture Q function
static void
cap_isr(
    void *arg);

static void
VP_mem_isr(
    void *arg);

static void
VP_onfly_isr(
    void *arg);

static void*
isp_task(
    void *arg);

static void*
encoder_task(
    void *arg);

static MMP_BOOL
_Chk_Skip_Frame(
    MMP_UINT32 InstanceNum);

static int
_Chk_Cap_Fire(
    CAPTURE_HANDLE* ptDev);


static MMP_INLINE void
_CaptureResultQ_Reset(
    void)
{
    gtCaptureResultQ.wIdx = gtCaptureResultQ.rIdx = 0;
}

static MMP_INLINE MMP_RESULT
_Check_CaptureResultQ(
    CAPTURE_RESULT *ptEntry)
{
    if (gtCaptureResultQ.wIdx == gtCaptureResultQ.rIdx)
        return (MMP_RESULT)QUEUE_EMPTY;

    ptEntry->resIdx     = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].resIdx;
    ptEntry->frameCount = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].frameCount;
    ptEntry->timeStamp  = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].timeStamp;
    ptEntry->frameIndex = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].frameIndex;
    ptEntry->bTopField  = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].bTopField;
    ptEntry->bCapToTran = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].bCapToTran;
    ptEntry->bTranToEnc = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].bTranToEnc;

    return (MMP_RESULT)QUEUE_NOT_EMPTY;
}

static MMP_INLINE MMP_RESULT
_CaptureResultQ_Dequeue(
    CAPTURE_RESULT *ptEntry)
{
    if (gtCaptureResultQ.wIdx == gtCaptureResultQ.rIdx)
        return (MMP_RESULT)QUEUE_EMPTY;

    ptEntry->resIdx     = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].resIdx;
    ptEntry->frameCount = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].frameCount;
    ptEntry->timeStamp  = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].timeStamp;
    ptEntry->frameIndex = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].frameIndex;
    ptEntry->bTopField  = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].bTopField;
    ptEntry->bCapToTran = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].bCapToTran;
    ptEntry->bTranToEnc = gtCaptureResultQ.entry[gtCaptureResultQ.rIdx].bTranToEnc;

    gtCaptureResultQ.rIdx++;
    if (gtCaptureResultQ.rIdx >= CAPTURE_RESULT_QUEUE_SIZE)
        gtCaptureResultQ.rIdx = 0;
    return (MMP_RESULT)QUEUE_NOT_EMPTY;
}

static MMP_INLINE void
_CaptureResultQ_Enqueue(
    CAPTURE_RESULT *ptEntry)
{
    gtCaptureResultQ.entry[gtCaptureResultQ.wIdx].resIdx        = ptEntry->resIdx;
    gtCaptureResultQ.entry[gtCaptureResultQ.wIdx].frameCount    = ptEntry->frameCount;
    gtCaptureResultQ.entry[gtCaptureResultQ.wIdx].timeStamp     = ptEntry->timeStamp;
    gtCaptureResultQ.entry[gtCaptureResultQ.wIdx].frameIndex    = ptEntry->frameIndex;
    gtCaptureResultQ.entry[gtCaptureResultQ.wIdx].bTopField     = ptEntry->bTopField;
    gtCaptureResultQ.entry[gtCaptureResultQ.wIdx].bCapToTran    = ptEntry->bCapToTran;
    gtCaptureResultQ.entry[gtCaptureResultQ.wIdx].bTranToEnc    = ptEntry->bTranToEnc;

    gtCaptureResultQ.wIdx++;
    if (gtCaptureResultQ.wIdx >= CAPTURE_RESULT_QUEUE_SIZE)
        gtCaptureResultQ.wIdx = 0;
}

static MMP_INLINE void
_IspResultQ_Reset(
    void)
{
    gtIspResultQ.wIdx = gtIspResultQ.rIdx = 0;
}

static MMP_INLINE MMP_RESULT
_IspResultQ_Dequeue(
    CAPTURE_RESULT *ptEntry)
{
    if (gtIspResultQ.wIdx == gtIspResultQ.rIdx)
        return (MMP_RESULT)QUEUE_EMPTY;

    ptEntry->resIdx     = gtIspResultQ.entry[gtIspResultQ.rIdx].resIdx;
    ptEntry->frameCount = gtIspResultQ.entry[gtIspResultQ.rIdx].frameCount;
    ptEntry->timeStamp  = gtIspResultQ.entry[gtIspResultQ.rIdx].timeStamp;
    ptEntry->frameIndex = gtIspResultQ.entry[gtIspResultQ.rIdx].frameIndex;
    ptEntry->bCapToTran = gtIspResultQ.entry[gtIspResultQ.rIdx].bCapToTran;
    ptEntry->bTranToEnc = gtIspResultQ.entry[gtIspResultQ.rIdx].bTranToEnc;

    gtIspResultQ.rIdx++;
    if (gtIspResultQ.rIdx >= CAPTURE_RESULT_QUEUE_SIZE)
        gtIspResultQ.rIdx = 0;
    return (MMP_RESULT)QUEUE_NOT_EMPTY;
}

static MMP_INLINE void
_IspResultQ_Enqueue(
    CAPTURE_RESULT *ptEntry)
{
    gtIspResultQ.entry[gtIspResultQ.wIdx].resIdx        = ptEntry->resIdx;
    gtIspResultQ.entry[gtIspResultQ.wIdx].frameCount    = ptEntry->frameCount;
    gtIspResultQ.entry[gtIspResultQ.wIdx].timeStamp     = ptEntry->timeStamp;
    gtIspResultQ.entry[gtIspResultQ.wIdx].frameIndex    = ptEntry->frameIndex;
    gtIspResultQ.entry[gtIspResultQ.wIdx].bCapToTran    = ptEntry->bCapToTran;
    gtIspResultQ.entry[gtIspResultQ.wIdx].bTranToEnc    = ptEntry->bTranToEnc;

    gtIspResultQ.wIdx++;
    if (gtIspResultQ.wIdx >= CAPTURE_RESULT_QUEUE_SIZE)
        gtIspResultQ.wIdx = 0;
}

static MMP_INLINE void
_EncodedResultQ_Reset(
    void)
{
    gtEncodedResultQ.wIdx = gtEncodedResultQ.rIdx = 0;
}

static MMP_INLINE MMP_RESULT
_EncodedResultQ_Dequeue(
    ENCODED_RESULT *ptEntry)
{
    //MMP_INT result;

    if (gtEncodedResultQ.wIdx == gtEncodedResultQ.rIdx)
        return (MMP_RESULT)QUEUE_EMPTY;

    ptEntry->frameCount     = gtEncodedResultQ.entry[gtEncodedResultQ.rIdx].frameCount;
    ptEntry->timeStamp      = gtEncodedResultQ.entry[gtEncodedResultQ.rIdx].timeStamp;
    ptEntry->pData          = gtEncodedResultQ.entry[gtEncodedResultQ.rIdx].pData;
    ptEntry->dataSize       = gtEncodedResultQ.entry[gtEncodedResultQ.rIdx].dataSize;
    ptEntry->InstanceNum    = gtEncodedResultQ.entry[gtEncodedResultQ.rIdx].InstanceNum;

    gtEncodedResultQ.rIdx++;
    if (gtEncodedResultQ.rIdx >= ENCODED_RESULT_QUEUE_SIZE)
        gtEncodedResultQ.rIdx = 0;
    return (MMP_RESULT)QUEUE_NOT_EMPTY;
}

static MMP_INLINE void
_EncodedResultQ_Enqueue(
    ENCODED_RESULT *ptEntry)
{
    gtEncodedResultQ.entry[gtEncodedResultQ.wIdx].frameCount    = ptEntry->frameCount;
    gtEncodedResultQ.entry[gtEncodedResultQ.wIdx].timeStamp     = ptEntry->timeStamp;
    gtEncodedResultQ.entry[gtEncodedResultQ.wIdx].pData         = ptEntry->pData;
    gtEncodedResultQ.entry[gtEncodedResultQ.wIdx].dataSize      = ptEntry->dataSize;
    gtEncodedResultQ.entry[gtEncodedResultQ.wIdx].InstanceNum   = ptEntry->InstanceNum;

    gtEncodedResultQ.wIdx++;
    if (gtEncodedResultQ.wIdx >= ENCODED_RESULT_QUEUE_SIZE)
        gtEncodedResultQ.wIdx = 0;
}

static void
_WaitAllQueue_Empty(
    void)
{
    CAPTURE_RESULT  tEntry  = {0};
    MMP_UINT32      timeOut = 0;

    while (QUEUE_NOT_EMPTY == _CaptureResultQ_Dequeue(&tEntry)) // && !gbEncodeFire)
    {
        usleep(30000);
        if (++timeOut > 10)
        {
            ithPrintf("wait QUEUE_NOT_EMPTY timeout %s() #%d\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    while (QUEUE_NOT_EMPTY == _IspResultQ_Dequeue(&tEntry)) // && !gbEncodeFire)
    {
        usleep(30000);
        if (++timeOut > 10)
        {
            ithPrintf("wait QUEUE_NOT_EMPTY timeout %s() #%d\n", __FUNCTION__, __LINE__);
            break;
        }
    }
}

//Time Stamp function
static MMP_INLINE MMP_UINT
frameRate25Hz_frameCount2TimeStamp(
    MMP_UINT frameCount)
{
    return frameCount * 40;
}

static MMP_INLINE MMP_UINT
frameRate30Hz_frameCount2TimeStamp(
    MMP_UINT frameCount)
{
    return (frameCount / 3) * 100 + (frameCount % 3) * 33;
}

static MMP_INLINE MMP_UINT
frameRate60Hz_frameCount2TimeStamp(
    MMP_UINT frameCount)
{
    return (frameCount / 3) * 50 + (frameCount % 3) * 33 / 2;
}

static frameCount2TimeStamp _frameCount2TimeStamp_TABLE[16] =
{
    MMP_NULL,
    frameRate25Hz_frameCount2TimeStamp,
    MMP_NULL,
    frameRate30Hz_frameCount2TimeStamp,
    frameRate60Hz_frameCount2TimeStamp,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL,
    MMP_NULL
};

static MMP_INLINE frameCount2TimeStamp
_VIDEO_ENCODER_GetTimeStampConverter(
    MMP_CAP_FRAMERATE frameRate)
{
    frameCount2TimeStamp pf = MMP_NULL;

    if (frameRate <= 15)
        pf = _frameCount2TimeStamp_TABLE[frameRate];

    return pf;
}

// Sensor and ISP function
MMP_RESULT
_CaptureAndVP_Init(
    void)
{
    MMP_RESULT result = MMP_SUCCESS;
#ifdef CFG_IPTV_TX
	MMP_UINT32 i;

    ithCapPowerUp();
    
    /*Hdmi init*/
    ithCapDeviceInitialize();

    while(!ithCapDeviceIsSignalStable())
    {
        printf("!ithCapDeviceIsSignalStable in _CaptureAndVP_Init\n");
        sleep(1);
    }    
#endif

    /*1. Capture Init*/
    ithCapInitialize();

    /* 2. Setting Capture Frontend Device,Onfly mode, interrupt, max width ,max height */
#if defined(CFG_DUAL_STREAM)
    CAPTURE_SETTING capset =
    {CAPTURE_DEV_SENSOR, MMP_FALSE,  MMP_TRUE , CAPTURE_MAXWIDTH, CAPTURE_MAXHEIGHT};
    ithCapConnect(  &cap0,  capset);
#else
#ifdef CFG_IPTV_TX
	CAPTURE_SETTING capset = 
	{MMP_CAP_DEV_HDMIRX, MMP_TRUE,  MMP_FALSE, CAPTURE_MAXWIDTH, CAPTURE_MAXHEIGHT};
#else
    CAPTURE_SETTING capset =
    {CAPTURE_DEV_SENSOR, MMP_TRUE,  MMP_FALSE , CAPTURE_MAXWIDTH, CAPTURE_MAXHEIGHT};
#endif
    ithCapConnect(  &cap0,  capset);
#endif
    /*3. Sensor setting */
    ithSensorRegsisterCallback();
    //ithSensorPowerOn(MMP_TRUE, MMP_TRUE);
    ithSensorInitialize();

    /* 4. ISP Init */
    mmpIspInitialize(&gIspDev, MMP_ISP_CORE_0);

    return result;
}

MMP_RESULT
_CaptureAndVP_Terminate(
    void)
{
    MMP_RESULT result = MMP_SUCCESS;

    mmpIspTerminate(&gIspDev);
    WriteBufferIndex = 0;

    ithCapTerminate();

    ithCapDeviceTerminate(&cap0);

    return result;
}

MMP_RESULT
_CaptureAndVP_Fire(
    void)
{
    MMP_RESULT                result       = MMP_SUCCESS;
    MMP_ISP_SHARE             ispInput     = {0};
    MMP_ISP_CORE_INFO         ISPCOREINFO  = {0};
    MMP_ISP_OUTPUT_INFO       ispOutput    = {0};
    MMP_ISP_OUTPUT_INFO       ispOutput_PV = {0};
    MMP_ISP_WRITE_BUFFER_INFO ispWbInfo    = {0};

    MMP_UINT16              i, j;
    MMP_FLOAT               cap_scale;

    /* Cap get device information */
    ithCapGetDeviceInfo(&cap0);
    /* Cap paramete setting */
    ithCapParameterSetting(&cap0);

    memset(&gtAVCEncoderFlow, 0, sizeof(gtAVCEncoderFlow));
    gtAVCEncoderFlow.baseTimeStamp                      = 0;
    gtAVCEncoderFlow.frameRate                          = gtAVCEncoderFlow.captureFrameRate
                                                        = cap0.cap_info.ininfo.framerate

#if defined(CFG_SENSOR_FLICK_50HZ)
                                                        = MMP_CAP_FRAMERATE_25HZ;
#else
#ifdef CFG_IPTV_TX
                                                        = MMP_CAP_FRAMERATE_60HZ;
#else
                                                        = MMP_CAP_FRAMERATE_30HZ;
#endif
#endif

    gtAVCEncoderFlow.pfToTimeStamp                      = _VIDEO_ENCODER_GetTimeStampConverter(gtAVCEncoderFlow.frameRate);

#if defined(CFG_DUAL_STREAM)
    gtAVCEncoderFlow.pfEnDequeue                        = _IspResultQ_Dequeue;

    mmpIspResetEngine(gIspDev);
    WriteBufferIndex = 0;
    ISPCOREINFO.EnPreview = false;
    ISPCOREINFO.PreScaleSel = MMP_ISP_PRESCALE_NORMAL;
    mmpIspSetCore(gIspDev, &ISPCOREINFO);
    mmpIspSetMode(gIspDev, MMP_ISP_MODE_PLAY_VIDEO);
    /* Cap Register IRQ */
    ithCapRegisterIRQ(cap_isr, &cap0);
    mmpISPRegisterIRQ(gIspDev, VP_mem_isr);

    #if defined (CFG_SENSOR_FLICK_50HZ)
    gtAVCEncoderFlow.ispFrameRate[0]    = 25;
    gtAVCEncoderFlow.frameRateDiff[0]   = gtAVCEncoderFlow.ispFrameRate[0] - 25;      //EnPara.EnFrameRate;

    gtAVCEncoderFlow.ispFrameRate[1]    = 25;
    gtAVCEncoderFlow.frameRateDiff[1]   = gtAVCEncoderFlow.ispFrameRate[1] - 25;      //EnPara.EnFrameRate;
    #else
    gtAVCEncoderFlow.ispFrameRate[0]    = 30;
    gtAVCEncoderFlow.frameRateDiff[0]   = gtAVCEncoderFlow.ispFrameRate[0] - 30;      //EnPara.EnFrameRate;

    gtAVCEncoderFlow.ispFrameRate[1]    = 30;
    gtAVCEncoderFlow.frameRateDiff[1]   = gtAVCEncoderFlow.ispFrameRate[1] - 30;      //EnPara.EnFrameRate;
    #endif
    gtAVCEncoderFlow.frameCount[0]      = 1;
    gtAVCEncoderFlow.skipCount[0]       = 1;

    gtAVCEncoderFlow.frameCount[1]      = 1;
    gtAVCEncoderFlow.skipCount[1]       = 1;

    cap_scale                           = (MMP_FLOAT)cap0.cap_info.outinfo.OutWidth/ gptAVCEncoder[0]->frameWidth;

    if (cap_scale <= 1.6f)
    {
        cap0.cap_info.outinfo.OutWidth = gptAVCEncoder[0]->frameWidth;
    }

    for (i = 0; i < ISP_TRANSFORM_NUM; i++)
    {
        gtIspTransformParm[i].inWidth   = cap0.cap_info.outinfo.OutWidth;
        gtIspTransformParm[i].inHeight  = cap0.cap_info.outinfo.OutHeight;

        for (j = 0; j < 3; j++)
        {
            gtIspTransformParm[i].inAddrY[j]    = cap0.cap_info.OutAddrY[j];
            gtIspTransformParm[i].inAddrUV[j]   = cap0.cap_info.OutAddrUV[j];
        }
    }

    for (i = 0; i < ISP_TRANSFORM_NUM; i++)
    {
        for (j = 0; j < gptAVCEncoder[i]->sourceBufCount; j++)
        {
            gtIspTransformParm[i].outAddrY[j]   = (MMP_UINT32) gptAVCEncoder[i]->pSourceBufAdrY[j];
            gtIspTransformParm[i].outAddrUV[j]  = (MMP_UINT32) gptAVCEncoder[i]->pSourceBufAdrU[j];
        }

        gtIspTransformParm[i].outWidth      = gptAVCEncoder[i]->frameWidth;
        gtIspTransformParm[i].outHeight     = gptAVCEncoder[i]->frameHeight;

        //gptAVCEncoder[i]->interlaced_frame  = MMP_FALSE;
        gtIspTransformParm[i].deinterlaceOn = MMP_FALSE;
        gtIspTransformParm[i].bframeDouble  = MMP_FALSE;
    }
#else //else defined(CFG_DUAL_STREAM)
    gtAVCEncoderFlow.pfEnDequeue = _CaptureResultQ_Dequeue;

    mmpIspResetEngine(gIspDev);
    WriteBufferIndex = 0;
    ISPCOREINFO.EnPreview = false;
    ISPCOREINFO.PreScaleSel = MMP_ISP_PRESCALE_NORMAL;
    mmpIspSetCore(gIspDev, &ISPCOREINFO);
#ifdef LOW_LATENCY
    mmpIspSetLowLatencySlice(gIspDev, 2);
    mmpIspEnableLowLatency(gIspDev);
#endif
    mmpIspSetMode(gIspDev, MMP_ISP_MODE_PLAY_VIDEO);

    //Register IRQ
    mmpISPRegisterIRQ(gIspDev, VP_onfly_isr);
    /* Cap Disable IRQ */
    ithCapDisableIRQ();

    #if defined (CFG_SENSOR_FLICK_50HZ)
    gtAVCEncoderFlow.ispFrameRate[0]    = 25;
    gtAVCEncoderFlow.frameRateDiff[0]   = gtAVCEncoderFlow.ispFrameRate[0] - 25;      //EnPara.EnFrameRate;
    #else
	#ifdef CFG_IPTV_TX
	gtAVCEncoderFlow.ispFrameRate[0]    = 60;
	#else
    gtAVCEncoderFlow.ispFrameRate[0]    = 30;
    #endif
	gtAVCEncoderFlow.frameRateDiff[0]   = gtAVCEncoderFlow.ispFrameRate[0] - 30;      //EnPara.EnFrameRate;
    #endif

    gtAVCEncoderFlow.frameCount[0]      = 1;
    gtAVCEncoderFlow.skipCount[0]       = 1;

    //ISP Setting
    ispInput.format = MMP_ISP_IN_NV12;
    ispInput.width = cap0.cap_info.outinfo.OutWidth;
    ispInput.height = cap0.cap_info.outinfo.OutHeight;
    ispInput.pitchY = CAPTURE_MAXWIDTH;
    ispInput.pitchUv = CAPTURE_MAXWIDTH;

    // Signal Process Output Parameter
    for (i = 0; i < gptAVCEncoder[0]->sourceBufCount; ++i)
    {
        switch (i)
        {
            case 0:
                ispOutput.addrY = (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrY[i];
                ispOutput.addrU = (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrU[i];
                break;

            case 1:
                ispWbInfo.addrYRGB_1 = (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrY[i];
                ispWbInfo.addrU_1 =  (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrU[i];
                break;

            case 2:
                ispWbInfo.addrYRGB_2 = (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrY[i];
                ispWbInfo.addrU_2 =  (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrU[i];
                break;

            case 3:
                ispWbInfo.addrYRGB_3 = (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrY[i];
                ispWbInfo.addrU_3 =  (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrU[i];
                break;

            case 4:
                ispWbInfo.addrYRGB_4 = (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrY[i];
                ispWbInfo.addrU_4 =  (MMP_UINT32) gptAVCEncoder[0]->pSourceBufAdrU[i];
                break;

            default:
                break;
        }
    }

    mmpIspSetWriteBufferInfo(gIspDev, &ispWbInfo);
    //ISP ring buffer should delete default 1 buffer.
    if (gptAVCEncoder[0]->sourceBufCount > 0)
        mmpIspSetWriteBufferNum(gIspDev, gptAVCEncoder[0]->sourceBufCount - 1);

    ispOutput.startX = 0;
    ispOutput.startY = 0;
    ispOutput.width = gptAVCEncoder[0]->frameWidth;
    ispOutput.height = gptAVCEncoder[0]->frameHeight;
    ispOutput.format = MMP_ISP_OUT_NV12;
    ispOutput.pitchY = gptAVCEncoder[0]->framePitchY;
    ispOutput.pitchUv = gptAVCEncoder[0]->framePitchY;

    mmpIspEnableCAPOnFlyMode(gIspDev);
    //mmpIspEnable(gIspDev, MMP_ISP_OUTPUT_FIELD_MODE);
    mmpIspEnableInterrupt(gIspDev);
    mmpIspDisable(gIspDev, MMP_ISP_DEINTERLACE);
    mmpIspSetOutputWindow(gIspDev, &ispOutput);
    mmpIspSetVideoWindow(gIspDev, ispOutput.startX, ispOutput.startY, ispOutput.width, ispOutput.height);
    mmpIspPlayImageProcess(gIspDev, &ispInput);

#endif //end defined(CFG_DUAL_STREAM)

    /* Cap check timing & fire */
    _Chk_Cap_Fire(&cap0);

lab_end:
    return result;
}

// AVC Encoder function
static void
OpenEngine(ENCODER_TYPE type)
{
    MMP_UINT32 i;
    H264EncIn encIn;
    H264EncOut encOut;
    int ret;

    pthread_mutex_lock(&VideoEngineMutex);

    if (type == JPEG_ENCODER)
        gbJPEGEncoder = MMP_TRUE;

    if (type == H264_ENCODER)
        gbVideoEncoder = MMP_TRUE;

    if (!gbVideoEncoder)
        gbStream0Enable = MMP_TRUE;

    gOpenEnginCnt++;

    if (gOpenEnginCnt == 2)  // already open
        goto o_end;

    /*capture IO pin setting*/
#if defined(CFG_DOORBELL_LOBBY)
    for(i = ITH_GPIO_PIN40;i <= ITH_GPIO_PIN51;i++)
    {
        if(i == ITH_GPIO_PIN43)
        {
            //Set GPIO Output Mode
            ithGpioSetOut(i);
            //Set GPIO Mode2
            ithGpioSetMode(i, ITH_GPIO_MODE2);
        }
        else
        {
            if(i == ITH_GPIO_PIN47)
            {
                ithGpioSetIn(ITH_GPIO_PIN61);
                ithGpioSetFFIEn(ITH_GPIO_PIN61);
                //Set GPIO Mode2
                ithGpioSetMode(ITH_GPIO_PIN61, ITH_GPIO_MODE2);
            }
            else
            {
                //Set GPIO Input Mode
                ithGpioSetIn(i);
                ithGpioSetFFIEn(i);
                //Set GPIO Mode2
                ithGpioSetMode(i, ITH_GPIO_MODE2);
            }
        }
    }

#else
    //OUTDOOR
#ifdef CFG_CHIP_PKG_IT972
    //IT972
    for(i = ITH_GPIO_PIN44;i <= ITH_GPIO_PIN56;i++)
    {
        if(i == ITH_GPIO_PIN47)
        {
            //Set GPIO Output Mode
            ithGpioSetOut(i);
            //Set GPIO Mode2
            ithGpioSetMode(i, ITH_GPIO_MODE2);
        }
        else
        {
            //Set GPIO Input Mode
            ithGpioSetIn(i);
            ithGpioSetFFIEn(i);
            //Set GPIO Mode2
            ithGpioSetMode(i, ITH_GPIO_MODE2);
        }
    }
#else
    //IT976
    for(i = ITH_GPIO_PIN48;i <= ITH_GPIO_PIN59;i++)
    {
        if(i == ITH_GPIO_PIN51)
        {
            //Set GPIO Output Mode
            ithGpioSetOut(i);
            //Set GPIO Mode2
            ithGpioSetMode(i, ITH_GPIO_MODE2);
        }
        else
        {
            //Set GPIO Input Mode
            ithGpioSetIn(i);
            ithGpioSetFFIEn(i);
            //Set GPIO Mode2
            ithGpioSetMode(i, ITH_GPIO_MODE2);
        }
    }
#endif
#endif
    /***************************************************/
#ifndef CFG_IPTV_TX
    ithCapPowerUp();
    ithSensorPowerOn(MMP_TRUE, MMP_FALSE);
#endif

    memset (&encIn, 0, sizeof(H264EncIn));
    encIn.pOutBuf = (u32 *)(gptAVCEncoder[0]->pHdrBufAddr[0]);
    encIn.busOutBuf = (u32 *)(gptAVCEncoder[0]->pHdrBufAddr[0]);
    encIn.outBufSize = 2048;

    /* Start stream */
    ret = H264EncStrmStart(gptAVCEncoder[0]->encoder, &encIn, &encOut);
    if(ret != H264ENC_OK)
    {
        printf("H264EncStrmStart(%d) failed.\n", ret);
        //return -1;
    }

    gptAVCEncoder[0]->ParaSetHdrSize[0] = encOut.pNaluSizeBuf[0];
    gptAVCEncoder[0]->ParaSetHdrSize[1] = encOut.pNaluSizeBuf[1];
    gptAVCEncoder[0]->pHdrBufAddr[1] = gptAVCEncoder[0]->pHdrBufAddr[0] + encOut.pNaluSizeBuf[0];

    printf("SPS PPS stream %d %d %d\n", encOut.numNalus, encOut.pNaluSizeBuf[0], encOut.pNaluSizeBuf[1]);

#if defined(CFG_DUAL_STREAM)
    memset (&encIn, 0, sizeof(H264EncIn));
    encIn.pOutBuf = (u32 *)(gptAVCEncoder[1]->pHdrBufAddr[0]);
    encIn.busOutBuf = (u32 *)(gptAVCEncoder[1]->pHdrBufAddr[0]);
    encIn.outBufSize = 2048;

    /* Start stream */
    ret = H264EncStrmStart(gptAVCEncoder[1]->encoder, &encIn, &encOut);
    if(ret != H264ENC_OK)
    {
        printf("H264EncStrmStart() failed. %x", ret);
        //return -1;
    }

    gptAVCEncoder[1]->ParaSetHdrSize[0] = encOut.pNaluSizeBuf[0];
    gptAVCEncoder[1]->ParaSetHdrSize[1] = encOut.pNaluSizeBuf[1];
    gptAVCEncoder[1]->pHdrBufAddr[1] = gptAVCEncoder[1]->pHdrBufAddr[0] + encOut.pNaluSizeBuf[0];

    printf("SPS PPS stream %d %d %d\n", encOut.numNalus, encOut.pNaluSizeBuf[0], encOut.pNaluSizeBuf[1]);

#endif

    gbEncodeStart = MMP_TRUE;
    _CaptureAndVP_Fire();

#if defined(CFG_DUAL_STREAM)
    pthread_create(&gIsp_process_tid, NULL, isp_task, NULL);
#endif
    pthread_create(&gEncoder_process_tid, NULL, encoder_task, NULL);

#ifdef LOW_LATENCY        //temp solution
    {
    CAPTURE_RESULT  tEntry          = {0};
    MMP_UINT32      WBIndex         = WriteBufferIndex;

    WriteBufferIndex++;
    WriteBufferIndex %= gptAVCEncoder[0]->sourceBufCount;

    tEntry.frameIndex = WBIndex;
    tEntry.frameCount   = ++gtAVCEncoderFlow.captureFrameCount;

    if (gtAVCEncoderFlow.pfToTimeStamp)
    {
        tEntry.timeStamp                = gtAVCEncoderFlow.currTimeStamp
                                        = gtAVCEncoderFlow.baseTimeStamp
                                        + gtAVCEncoderFlow.pfToTimeStamp(tEntry.frameCount);
    }

    _CaptureResultQ_Enqueue(&tEntry);
    }
#endif

o_end:
    pthread_mutex_unlock(&VideoEngineMutex);
}

static void
CloseEngine(ENCODER_TYPE type)
{
    MMP_UINT32 i;
    H264EncIn encIn;
    H264EncOut encOut;
    int ret;

    pthread_mutex_lock(&VideoEngineMutex);

    if (gOpenEnginCnt == 0)
        goto c_end;

    if (type == JPEG_ENCODER)
    {
        gbJPEGEncoder = MMP_FALSE;
        if (!gbVideoEncoder)
            gbStream0Enable = MMP_FALSE;
    }

    if (type == H264_ENCODER)
    {
        gbVideoEncoder = MMP_FALSE;
        gbStream0Enable = MMP_FALSE;
        gbStream1Enable = MMP_FALSE;
        gbEncodeStart = MMP_FALSE;
    }

    gOpenEnginCnt--;

    if (gOpenEnginCnt == 1)
        goto c_end;

    //wait encoder task finish
    while(gVideoEncoding[0] == MMP_TRUE)
        usleep(1000);

    ithCapPowerDown();

    _WaitAllQueue_Empty();
    mmpIspResetEngine(gIspDev);
    WriteBufferIndex = 0;
    ithSensorPowerOn(MMP_FALSE, MMP_FALSE);

    // temp solution
    memset (&encIn, 0, sizeof(H264EncIn));
    encIn.pOutBuf = (u32 *)(gptAVCEncoder[0]->pHdrBufAddr[0]);
    encIn.busOutBuf = (u32 *)(gptAVCEncoder[0]->pHdrBufAddr[0]);
    encIn.outBufSize = 2048;

    ret = H264EncStrmEnd(gptAVCEncoder[0]->encoder, &encIn, &encOut);
    if(ret != H264ENC_OK)
    {
        printf("H264EncStrmEnd(%d) failed.\n", ret);
    }
    else
        printf("H264EncStrmEnd.\n");

#if defined(CFG_DUAL_STREAM)
    // temp solution
    memset (&encIn, 0, sizeof(H264EncIn));
    encIn.pOutBuf = (u32 *)(gptAVCEncoder[1]->pHdrBufAddr[0]);
    encIn.busOutBuf = (u32 *)(gptAVCEncoder[1]->pHdrBufAddr[0]);
    encIn.outBufSize = 2048;

    ret = H264EncStrmEnd(gptAVCEncoder[1]->encoder, &encIn, &encOut);

    if(ret != H264ENC_OK)
    {
        printf("H264EncStrmEnd() failed. %x", ret);
    }
    else
        printf("H264EncStrmEnd.\n");

    pthread_join(gIsp_process_tid, NULL);
#endif
    pthread_join(gEncoder_process_tid, NULL);
c_end:
    pthread_mutex_unlock(&VideoEngineMutex);
}

void
VideoEncoder_Init(
    //VIDEO_ENCODE_PARAMETER* enPara)
    void)
{
    MMP_UINT32 i;
    uint32_t size;
    uint32_t tmp;
    H264EncRet ret;
    H264EncConfig cfg;
    H264EncCodingCtrl codingCfg;
    H264EncRateCtrl rcCfg;
    H264EncIn encIn;
    H264EncOut encOut;
    H264EncPreProcessingCfg preProcCfg;

#ifndef CFG_IPTV_TX
#if !defined(CFG_SENSOR_ENABLE)
    return;
#endif
#endif

    for (i = 0; i < ENCODE_STREAM_NUM; i++)
    {
        if (MMP_NULL == gptAVCEncoder[i])
            gptAVCEncoder[i] = (H264_AVC_ENCODER *)malloc(sizeof(H264_AVC_ENCODER));

        if (MMP_NULL != gptAVCEncoder[i])
            memset(gptAVCEncoder[i], 0, sizeof(H264_AVC_ENCODER));
    }

    gptAVCEncoder[0]->sourceBufCount = 3;
    gptAVCEncoder[0]->streamBufCount = VIDEO_SAMPLE_NUM;

    size = (SENSOR_WIDTH * SENSOR_HEIGHT * 3 * gptAVCEncoder[0]->sourceBufCount) >> 1;

    gptAVCEncoder[0]->buf_vram_addr = itpVmemAlignedAlloc(8, size);
    gptAVCEncoder[0]->buf_sys_addr = (uint8_t*) ithMapVram(gptAVCEncoder[0]->buf_vram_addr, size, /*ITH_VRAM_READ |*/ ITH_VRAM_WRITE);

    for (i = 0; i < gptAVCEncoder[0]->sourceBufCount; i++)
    {
        gptAVCEncoder[0]->pSourceBufAdrY[i] = gptAVCEncoder[0]->buf_sys_addr + ((SENSOR_WIDTH * SENSOR_HEIGHT * 3 * i)/2);
        gptAVCEncoder[0]->pSourceBufAdrU[i] = gptAVCEncoder[0]->pSourceBufAdrY[i] + (SENSOR_WIDTH * SENSOR_HEIGHT);
        gptAVCEncoder[0]->pSourceBufAdrV[i] = gptAVCEncoder[0]->pSourceBufAdrU[i];
        //printf("++++++++init addr=(%x, %x, %x)\n", gptAVCEncoder[0]->pSourceBufAdrY[i], gptAVCEncoder[0]->pSourceBufAdrU[i], gptAVCEncoder[0]->pSourceBufAdrV[i]);
    }

    size = 20 * 1024;

    gptAVCEncoder[0]->hdr_buf_vram_addr = itpVmemAlignedAlloc(8, size);
    gptAVCEncoder[0]->hdr_buf_sys_addr = (uint8_t*) ithMapVram(gptAVCEncoder[0]->hdr_buf_vram_addr, size, /*ITH_VRAM_READ |*/ ITH_VRAM_WRITE);

    gptAVCEncoder[0]->pHdrBufAddr[0] = gptAVCEncoder[0]->hdr_buf_sys_addr;
    gptAVCEncoder[0]->pHdrBufAddr[1] = gptAVCEncoder[0]->hdr_buf_sys_addr + 10 * 1024;
    gptAVCEncoder[0]->ParaSetHdrSize[0] = 10 *1024;
    gptAVCEncoder[0]->ParaSetHdrSize[1] = 10 * 1024;

#ifdef MULTI_SLICE
    //stream buffer for multi-slice && low latency mode
    size = 500*1024;
    stream_buf_vram_addr = itpVmemAlignedAlloc(8, size);
    stream_buf_sys_addr = (uint8_t*) ithMapVram(stream_buf_vram_addr, size, /*ITH_VRAM_READ |*/ ITH_VRAM_WRITE);
#endif

#if defined(CFG_DUAL_STREAM)
    gptAVCEncoder[1]->sourceBufCount = 3;
    gptAVCEncoder[1]->streamBufCount = VIDEO_SAMPLE_NUM;

    size = (SENSOR_WIDTH_2 * SENSOR_HEIGHT_2 * 3 * gptAVCEncoder[1]->sourceBufCount) >> 1;

    gptAVCEncoder[1]->buf_vram_addr = itpVmemAlignedAlloc(8, size);
    gptAVCEncoder[1]->buf_sys_addr = (uint8_t*) ithMapVram(gptAVCEncoder[1]->buf_vram_addr, size, /*ITH_VRAM_READ |*/ ITH_VRAM_WRITE);

    for (i = 0; i < gptAVCEncoder[1]->sourceBufCount; i++)
    {
        gptAVCEncoder[1]->pSourceBufAdrY[i] = gptAVCEncoder[1]->buf_sys_addr + ((SENSOR_WIDTH_2 * SENSOR_HEIGHT_2 * 3 * i)/2);
        gptAVCEncoder[1]->pSourceBufAdrU[i] = gptAVCEncoder[1]->pSourceBufAdrY[i] + (SENSOR_WIDTH_2 * SENSOR_HEIGHT_2);
        gptAVCEncoder[1]->pSourceBufAdrV[i] = gptAVCEncoder[1]->pSourceBufAdrU[i];
    }

    size = 20 * 1024;

    gptAVCEncoder[1]->hdr_buf_vram_addr = itpVmemAlignedAlloc(8, size);
    gptAVCEncoder[1]->hdr_buf_sys_addr = (uint8_t*) ithMapVram(gptAVCEncoder[1]->hdr_buf_vram_addr, size, /*ITH_VRAM_READ |*/ ITH_VRAM_WRITE);

    gptAVCEncoder[1]->pHdrBufAddr[0] = gptAVCEncoder[1]->hdr_buf_sys_addr;
    gptAVCEncoder[1]->pHdrBufAddr[1] = gptAVCEncoder[1]->hdr_buf_sys_addr + 10 * 1024;
    gptAVCEncoder[1]->ParaSetHdrSize[0] = 10 *1024;
    gptAVCEncoder[1]->ParaSetHdrSize[1] = 10 * 1024;

#endif

    memset (&cfg, 0, sizeof(H264EncConfig));

    gptAVCEncoder[0]->frameWidth = gptAVCEncoder[0]->framePitchY = cfg.width = SENSOR_WIDTH;
    gptAVCEncoder[0]->frameHeight = cfg.height = SENSOR_HEIGHT;
    cfg.scaledWidth = 0;
    cfg.scaledHeight = 0;

#ifdef CFG_IPTV_TX
    cfg.frameRateDenom = 1;
    cfg.frameRateNum = 60;
#else
    if (VIDEO_FRAME_RATE == VIDEO_FRAME_RATE_25)
    {
        cfg.frameRateDenom = 1;
        cfg.frameRateNum = 25;
    } else {
        cfg.frameRateDenom = 1;
        cfg.frameRateNum = 30;
    }
#endif
    cfg.viewMode = H264ENC_BASE_VIEW_DOUBLE_BUFFER; /* Two buffers by default */
    cfg.streamType = H264ENC_BYTE_STREAM; //H264ENC_NAL_UNIT_STREAM;

    tmp = (gptAVCEncoder[0]->frameWidth * gptAVCEncoder[0]->frameHeight) >> 8;

    if (tmp <= 396)
        cfg.level = H264ENC_LEVEL_2;
    else if (tmp <= 792)
        cfg.level = H264ENC_LEVEL_2_1;
    else if (tmp <= 1620)
        cfg.level = H264ENC_LEVEL_3;
    else if (tmp <= 3600)
        cfg.level = H264ENC_LEVEL_3_1;
    else if (tmp <= 5120)
        cfg.level = H264ENC_LEVEL_3_2;
    else if (tmp <= 8192)
        cfg.level = H264ENC_LEVEL_4;

    //cfg.level = H264ENC_LEVEL_4;

    cfg.refFrameCompress = 0;
    cfg.rfcLumBufLimit = 0;
    cfg.rfcChrBufLimit = 0;
    cfg.refFrameAmount = 1;

    gbVideoEncoder = MMP_TRUE;
    //ithCapPowerUp();

    if((ret = H264EncInit(&cfg, &gptAVCEncoder[0]->encoder)) != H264ENC_OK)
    {
        printf("H264EncInit() failed. %x", ret);
        //return (int)ret;
    }

    /* Encoder setup: rate control */
    if((ret = H264EncGetRateCtrl(gptAVCEncoder[0]->encoder, &rcCfg)) != H264ENC_OK)
    {
        printf("H264EncGetRateCtrl() failed. %x", ret);
        //CloseEncoder(encoder);
        //return -1;
    }
    else
    {
        //printf("Get rate control: qp %2d [%2d, %2d]  %8d bps  "
        //       "pic %d mb %d skip %d  hrd %d\n  cpbSize %d gopLen %d "
        //       "intraQpDelta %2d\n",
        //       rcCfg.qpHdr, rcCfg.qpMin, rcCfg.qpMax, rcCfg.bitPerSecond,
        //       rcCfg.pictureRc, rcCfg.mbRc, rcCfg.pictureSkip, rcCfg.hrd,
        //       rcCfg.hrdCpbSize, rcCfg.gopLen, rcCfg.intraQpDelta);

        rcCfg.qpHdr = -1;
        rcCfg.qpMin = 15;
        rcCfg.qpMax = 50;
        rcCfg.pictureSkip = 0;
        rcCfg.pictureRc = 1;
        rcCfg.mbRc = 1;
#ifdef CFG_IOT_ENABLE
        rcCfg.bitPerSecond = 2000 * 1024;
#else
        rcCfg.bitPerSecond = SENSOR_BITRATE * 1024;
#endif
        rcCfg.hrd = 1;
        rcCfg.hrdCpbSize = 0;
        rcCfg.gopLen = VIDEO_FRAME_RATE;
        rcCfg.intraQpDelta = -3;
        rcCfg.fixedIntraQp = 0;
        rcCfg.mbQpAdjustment = 0;
        rcCfg.longTermPicRate = VIDEO_FRAME_RATE;
        rcCfg.mbQpAutoBoost = 0;

        //printf("Set rate control: qp %2d [%2d, %2d] %8d bps  "
        //       "pic %d mb %d skip %d  hrd %d\n"
        //       "  cpbSize %d gopLen %d intraQpDelta %2d "
        //       "fixedIntraQp %2d mbQpAdjustment %d "
        //       "mbQpAutoBoost %d\n",
        //       rcCfg.qpHdr, rcCfg.qpMin, rcCfg.qpMax, rcCfg.bitPerSecond,
        //       rcCfg.pictureRc, rcCfg.mbRc, rcCfg.pictureSkip, rcCfg.hrd,
        //       rcCfg.hrdCpbSize, rcCfg.gopLen, rcCfg.intraQpDelta,
        //       rcCfg.fixedIntraQp, rcCfg.mbQpAdjustment, rcCfg.mbQpAutoBoost);

        if((ret = H264EncSetRateCtrl(gptAVCEncoder[0]->encoder, &rcCfg)) != H264ENC_OK)
        {
            printf("H264EncSetRateCtrl() failed. %x", ret);
            //CloseEncoder(encoder);
            //return -1;
        }
    }

    /* Encoder setup: coding control */
    if((ret = H264EncGetCodingCtrl(gptAVCEncoder[0]->encoder, &codingCfg)) != H264ENC_OK)
    {
        printf("H264EncGetCodingCtrl() failed. %x", ret);
        //CloseEncoder(encoder);
        //return -1;
    }
    else
    {
#ifdef MULTI_SLICE
        codingCfg.sliceSize = 5;
#else
        codingCfg.sliceSize = 0;
#endif
        codingCfg.constrainedIntraPrediction = 0;
        codingCfg.disableDeblockingFilter = 0;
        codingCfg.enableCabac = 1;
        codingCfg.cabacInitIdc = 0;
        codingCfg.transform8x8Mode = 1;
        codingCfg.quarterPixelMv = 1;
        codingCfg.videoFullRange = 1;
        codingCfg.seiMessages = 0;
        codingCfg.idrHeader = 0;
        codingCfg.gdrDuration = 0;
        codingCfg.fieldOrder = 0;
        codingCfg.cirStart = 0;
        codingCfg.cirInterval = 0;
#ifdef LOW_LATENCY
        //Low Latency
        codingCfg.inputLineBufDepth = 2;
        codingCfg.inputLineBufHwModeEn = 1;
        codingCfg.inputLineBufEn = 1;
        codingCfg.inputLineBufLoopBackEn = 1;
#endif
        //printf
        //    ("Set coding control: SEI %d idrHdr %d Slice %5d   deblocking %d "
        //     "constrained intra %d video range %d\n"
        //     "  cabac %d  cabac initial idc %d  Adaptive 8x8 transform %d"
        //     "  quarter-pixel MV %d\n"
        //     "  adaptiveRoi %d,%d\n",
        //     codingCfg.seiMessages, codingCfg.idrHeader, codingCfg.sliceSize,
        //     codingCfg.disableDeblockingFilter,
        //     codingCfg.constrainedIntraPrediction, codingCfg.videoFullRange,
        //     codingCfg.enableCabac, codingCfg.cabacInitIdc,
        //     codingCfg.transform8x8Mode, codingCfg.quarterPixelMv,
        //     codingCfg.adaptiveRoi, codingCfg.adaptiveRoiColor);

        if((ret = H264EncSetCodingCtrl(gptAVCEncoder[0]->encoder, &codingCfg)) != H264ENC_OK)
        {
            printf("H264EncSetCodingCtrl() failed. %x", ret);
            //CloseEncoder(encoder);
            //return -1;
        }
    }
    if ((ret = H264EncGetPreProcessing(gptAVCEncoder[0]->encoder, &preProcCfg)) != H264ENC_OK)
    {
        printf("H264EncGetPreProcessing() failed.\n", ret);
    } else {

        preProcCfg.inputType = H264ENC_YUV420_SEMIPLANAR;
        preProcCfg.origWidth = cfg.width;
        preProcCfg.origHeight = cfg.height;
        preProcCfg.videoStabilization = 0;
        preProcCfg.scaledOutput = 0;

        if ((ret = H264EncSetPreProcessing(gptAVCEncoder[0]->encoder, &preProcCfg)) != H264ENC_OK)
        {
            printf("H264EncSetPreProcessing() failed.\n", ret);
        }
    }
#if defined(CFG_DUAL_STREAM)
    gptAVCEncoder[1]->frameWidth = gptAVCEncoder[1]->framePitchY = cfg.width = SENSOR_WIDTH_2;
    gptAVCEncoder[1]->frameHeight = cfg.height = SENSOR_HEIGHT_2;
    cfg.scaledWidth = 0;
    cfg.scaledHeight = 0;

    if (VIDEO_FRAME_RATE == VIDEO_FRAME_RATE_25)
    {
        cfg.frameRateDenom = 1;
        cfg.frameRateNum = 25;
    } else {
        cfg.frameRateDenom = 1;
        cfg.frameRateNum = 30;
    }

    cfg.viewMode = H264ENC_BASE_VIEW_DOUBLE_BUFFER; /* Two buffers by default */
    cfg.streamType = H264ENC_NAL_UNIT_STREAM;

    tmp = (gptAVCEncoder[1]->frameWidth * gptAVCEncoder[1]->frameHeight) >> 8;

    if (tmp <= 396)
        cfg.level = H264ENC_LEVEL_2;
    else if (tmp <= 792)
        cfg.level = H264ENC_LEVEL_2_1;
    else if (tmp <= 1620)
        cfg.level = H264ENC_LEVEL_3;
    else if (tmp <= 3600)
        cfg.level = H264ENC_LEVEL_3_1;
    else if (tmp <= 5120)
        cfg.level = H264ENC_LEVEL_3_2;
    else if (tmp <= 8192)
        cfg.level = H264ENC_LEVEL_4;

    //cfg.level = H264ENC_LEVEL_4;

    cfg.refFrameCompress = 0;
    cfg.rfcLumBufLimit = 0;
    cfg.rfcChrBufLimit = 0;
    cfg.refFrameAmount = 1;

    //_VIDEO_ENCODER_Init();
    if((ret = H264EncInit(&cfg, &gptAVCEncoder[1]->encoder)) != H264ENC_OK)
    {
        printf("H264EncInit() failed. %x", ret);
        //return (int)ret;
    }

    /* Encoder setup: rate control */
    if((ret = H264EncGetRateCtrl(gptAVCEncoder[1]->encoder, &rcCfg)) != H264ENC_OK)
    {
        printf("H264EncGetRateCtrl() failed. %x", ret);
        //CloseEncoder(encoder);
        //return -1;
    }
    else
    {
        //printf("Get rate control: qp %2d [%2d, %2d]  %8d bps  "
        //       "pic %d mb %d skip %d  hrd %d\n  cpbSize %d gopLen %d "
        //       "intraQpDelta %2d\n",
        //       rcCfg.qpHdr, rcCfg.qpMin, rcCfg.qpMax, rcCfg.bitPerSecond,
        //       rcCfg.pictureRc, rcCfg.mbRc, rcCfg.pictureSkip, rcCfg.hrd,
        //       rcCfg.hrdCpbSize, rcCfg.gopLen, rcCfg.intraQpDelta);

        rcCfg.qpHdr = -1;
        rcCfg.qpMin = 15;
        rcCfg.qpMax = 45;
        rcCfg.pictureSkip = 0;
        rcCfg.pictureRc = 1;
        rcCfg.mbRc = 1;
        rcCfg.bitPerSecond = SENSOR_BITRATE_2 * 1024;
        rcCfg.hrd = 0;
        rcCfg.hrdCpbSize = 0;
        rcCfg.gopLen = VIDEO_FRAME_RATE;
        rcCfg.intraQpDelta = -3;
        rcCfg.fixedIntraQp = 0;
        rcCfg.mbQpAdjustment = 0;
        rcCfg.longTermPicRate = VIDEO_FRAME_RATE;
        rcCfg.mbQpAutoBoost = 0;

        //printf("Set rate control: qp %2d [%2d, %2d] %8d bps  "
        //       "pic %d mb %d skip %d  hrd %d\n"
        //       "  cpbSize %d gopLen %d intraQpDelta %2d "
        //       "fixedIntraQp %2d mbQpAdjustment %d "
        //       "mbQpAutoBoost %d\n",
        //       rcCfg.qpHdr, rcCfg.qpMin, rcCfg.qpMax, rcCfg.bitPerSecond,
        //       rcCfg.pictureRc, rcCfg.mbRc, rcCfg.pictureSkip, rcCfg.hrd,
        //       rcCfg.hrdCpbSize, rcCfg.gopLen, rcCfg.intraQpDelta,
        //       rcCfg.fixedIntraQp, rcCfg.mbQpAdjustment, rcCfg.mbQpAutoBoost);

        if((ret = H264EncSetRateCtrl(gptAVCEncoder[1]->encoder, &rcCfg)) != H264ENC_OK)
        {
            printf("H264EncSetRateCtrl() failed. %x", ret);
            //CloseEncoder(encoder);
            //return -1;
        }
    }

    /* Encoder setup: coding control */
    if((ret = H264EncGetCodingCtrl(gptAVCEncoder[1]->encoder, &codingCfg)) != H264ENC_OK)
    {
        printf("H264EncGetCodingCtrl() failed. %x", ret);
        //CloseEncoder(encoder);
        //return -1;
    }
    else
    {
#ifdef MULTI_SLICE
        codingCfg.sliceSize = 5;
#else
        codingCfg.sliceSize = 0;
#endif
        codingCfg.constrainedIntraPrediction = 0;
        codingCfg.disableDeblockingFilter = 0;
        codingCfg.enableCabac = 1;
        codingCfg.cabacInitIdc = 0;
        codingCfg.transform8x8Mode = 1;
        codingCfg.quarterPixelMv = 1;
        codingCfg.videoFullRange = 1;
        codingCfg.seiMessages = 1;
        codingCfg.idrHeader = 1;
        codingCfg.gdrDuration = 0;
        codingCfg.fieldOrder = 0;
        codingCfg.cirStart = 0;
        codingCfg.cirInterval = 0;
#ifdef LOW_LATENCY
        //Low Latency
        codingCfg.inputLineBufDepth = 2;
        codingCfg.inputLineBufHwModeEn = 1;
        codingCfg.inputLineBufEn = 1;
        codingCfg.inputLineBufLoopBackEn = 1;

#endif
        //printf
        //    ("Set coding control: SEI %d idrHdr %d Slice %5d   deblocking %d "
        //     "constrained intra %d video range %d\n"
        //     "  cabac %d  cabac initial idc %d  Adaptive 8x8 transform %d"
        //     "  quarter-pixel MV %d\n"
        //     "  adaptiveRoi %d,%d\n",
        //     codingCfg.seiMessages, codingCfg.idrHeader, codingCfg.sliceSize,
        //     codingCfg.disableDeblockingFilter,
        //     codingCfg.constrainedIntraPrediction, codingCfg.videoFullRange,
        //     codingCfg.enableCabac, codingCfg.cabacInitIdc,
        //     codingCfg.transform8x8Mode, codingCfg.quarterPixelMv,
        //     codingCfg.adaptiveRoi, codingCfg.adaptiveRoiColor);

        if((ret = H264EncSetCodingCtrl(gptAVCEncoder[1]->encoder, &codingCfg)) != H264ENC_OK)
        {
            printf("H264EncSetCodingCtrl() failed. %x", ret);
            //CloseEncoder(encoder);
            //return -1;
        }
    }
#endif

    _CaptureAndVP_Terminate();
    _CaptureAndVP_Init();

//#ifndef SENSOR_NOVATEK_NT99141
//    ithSensorPowerOn(MMP_TRUE, MMP_TRUE);
//#endif
    //_CaptureAndVP_Fire();

    gbEncodeFire = MMP_FALSE;

    for (i = 0; i < VIDEO_STREAM_NUM; i++)
    {
        gVideoSelIdx[i]     = gVideoWrIdx[i] = 0;
        gStartgetVideo[i]   = MMP_FALSE;
        gVideoUserNum[i]    = 0;
    }
    gbVideoEncoderInit  = MMP_TRUE;
    gbJPEGEncoder       = MMP_FALSE;
    gbVideoEncoder      = MMP_FALSE;

    //ithCapPowerDown();
    //_WaitAllQueue_Empty();
    mmpIspResetEngine(gIspDev);
    WriteBufferIndex = 0;
//#ifndef SENSOR_NOVATEK_NT99141
//    ithSensorPowerOn(MMP_FALSE, MMP_TRUE);
//#endif

    memset(&gtAVCEncoderFlow, 0, sizeof(gtAVCEncoderFlow));
    _CaptureResultQ_Reset();
    _IspResultQ_Reset();
    sem_init(&gpEncoderSem, 0, 0);
}

void
VideoEncoder_SetStreamBuf(
    MMP_UINT32  stramIndex,
    MMP_UINT32  bufIndex,
    MMP_UINT8   *bufAdr)
{
    if (bufIndex < gptAVCEncoder[stramIndex]->streamBufCount)
    {
        gbStrBufFire[stramIndex][bufIndex]                  = MMP_FALSE;
        gptAVCEncoder[stramIndex]->pStreamBufAdr[bufIndex]  = bufAdr;
        //printf("VideoEncoder_SetStreamBuf %d\n", bufIndex);
    }
}

void
VideoEncoder_SetStreamBufCallBack(
    ENCODE_PARA_CALLBACK encodeCallback)
{
    gfpCallback = encodeCallback;
}

void
VideoEncoder_Open(
    MMP_UINT32 stramIndex)
{
    MMP_UINT32 i, j;

    if (!gbVideoEncoderInit)
        return;

    if (stramIndex == 0 && gbStream0Enable == MMP_FALSE)
    {
        gbStream0Enable             = MMP_TRUE;
        gStartgetVideo[stramIndex]  = MMP_TRUE;
        gVideoUserNum[stramIndex]   = 1;
    }
    else if (stramIndex == 0 && gbStream0Enable == MMP_TRUE)
        return;
    if (stramIndex == 1)
    {
        gbStream1Enable             = MMP_TRUE;
        gStartgetVideo[stramIndex]  = MMP_TRUE;
        gVideoUserNum[stramIndex]   = 1;
    }
    if (gbVideoEncoder)
        return;

    OpenEngine(H264_ENCODER);

    for (i = 0; i < VIDEO_STREAM_NUM; i++)
        for (j = 0; j < VIDEO_SAMPLE_NUM; j++)
            gbStrBufFire[i][j] = MMP_FALSE;

    gbEncodeFire = MMP_FALSE;

    for (i = 0; i < VIDEO_STREAM_NUM; i++)
        gVideoSelIdx[i] = gVideoWrIdx[i] = 0;

    gbVideoEncoder = MMP_TRUE;
}

void
VideoEncoder_GetHdrInfo(
    VIDEO_HEADER_INFO *headInfo)
{
    headInfo->SPSBuf    = gptAVCEncoder[headInfo->streamIndex]->pHdrBufAddr[0];
    headInfo->PPSBuf    = gptAVCEncoder[headInfo->streamIndex]->pHdrBufAddr[1];
    headInfo->SPS_Size  = gptAVCEncoder[headInfo->streamIndex]->ParaSetHdrSize[0];
    headInfo->PPS_Size  = gptAVCEncoder[headInfo->streamIndex]->ParaSetHdrSize[1];
}

void
VideoEncoder_Close(
    void)
{
    MMP_UINT32 i;

    if (!gbVideoEncoderInit)
        return;
    CloseEngine(H264_ENCODER);

    memset(&gtAVCEncoderFlow, 0, sizeof(gtAVCEncoderFlow));

    for (i = 0; i < VIDEO_STREAM_NUM; i++)
        gptAVCEncoder[i]->streamBufSelect = 0;

    _CaptureResultQ_Reset();
    _IspResultQ_Reset();

    gfpCallback         = MMP_NULL;
    for (i = 0; i < VIDEO_STREAM_NUM; i++)
    {
        gStartgetVideo[i]   = MMP_FALSE;
        gVideoUserNum[i]    = 0;
    }
}

MMP_BOOL
VideoEncoder_GetSreamstate(
    MMP_UINT8 stream_id)
{
    return gStartgetVideo[stream_id];
}

MMP_UINT8
VideoEncoder_GetSreamUserNum(
    MMP_UINT8 stream_id)
{
    //printf(">>>>>%s, %d, %d\n", __FUNCTION__, stream_id, gVideoUserNum[stream_id]);
    return gVideoUserNum[stream_id];
}

void
JPEGEncodeFrame(
    JPEG_ENCODE_PARAMETER *enPara)
{
    HJPG            *pHJpeg         = 0;
    JPG_INIT_PARAM  initParam       = {0};
    JPG_STREAM_INFO inStreamInfo    = {0};
    JPG_STREAM_INFO outStreamInfo   = {0};
    JPG_BUF_INFO    entropyBufInfo  = {0};
    JPG_USER_INFO   jpgUserInfo     = {0};
    uint32_t        jpgEncSize      = 0;
    //MMP_UINT32          jpgStart = 0;

    //jpgStart = PalGetClock();

    if (!gbVideoEncoderInit)
        return;

    OpenEngine(JPEG_ENCODER);

    // ------------------------------------------------------
    // encode JPEG
    initParam.codecType     = JPG_CODEC_ENC_JPG;
    initParam.exColorSpace  = JPG_COLOR_SPACE_NV12;
    initParam.outColorSpace = JPG_COLOR_SPACE_YUV420;
    initParam.width         = gptAVCEncoder[0]->frameWidth;
    initParam.height        = gptAVCEncoder[0]->frameHeight;
    initParam.encQuality    = enPara->quality;

#ifndef CFG_IPTV_TX
    iteJpg_CreateHandle(&pHJpeg, &initParam, 0);
#endif

    if (sem_wait(&gpEncoderSem) == 0)
    {
        inStreamInfo.streamIOType           = JPG_STREAM_IO_READ;
        inStreamInfo.streamType             = JPG_STREAM_MEM;

        // Y
        inStreamInfo.jstream.mem[0].pAddr   = (uint8_t *)gptAVCEncoder[0]->pSourceBufAdrY[gJPEGBufIndex];
        inStreamInfo.jstream.mem[0].pitch   = gptAVCEncoder[0]->framePitchY;
        //inStreamInfo.src.mem[0].length = _Get_Lcd_Width() * _Get_Lcd_Height();
        // U
        inStreamInfo.jstream.mem[1].pAddr   = (uint8_t *)gptAVCEncoder[0]->pSourceBufAdrU[gJPEGBufIndex];
        inStreamInfo.jstream.mem[1].pitch   = gptAVCEncoder[0]->framePitchY;
        //inStreamInfo.src.mem[1].length = (_Get_Lcd_Width()/2) * _Get_Lcd_Height();
        // V
        inStreamInfo.jstream.mem[2].pAddr   = (uint8_t *)gptAVCEncoder[0]->pSourceBufAdrV[gJPEGBufIndex];
        inStreamInfo.jstream.mem[2].pitch   = gptAVCEncoder[0]->framePitchY;
        //inStreamInfo.src.mem[2].length = (_Get_Lcd_Width()/2) * _Get_Lcd_Height();

        inStreamInfo.validCompCnt           = 3;

        //if( encName )
        //{
        //    outStreamInfo.streamType = JPG_STREAM_FILE;
        //    outStreamInfo.jstream.path   = (void*)encName;
        //}

        outStreamInfo.streamIOType          = JPG_STREAM_IO_WRITE;
        outStreamInfo.streamType            = JPG_STREAM_MEM;
        //outStreamInfo.jpg_reset_stream_info =  _reset_stream_info;
        outStreamInfo.jstream.mem[0].pAddr  = enPara->strmBuf;
        outStreamInfo.jstream.mem[0].pitch  = enPara->strmBuf_size;
        outStreamInfo.jstream.mem[0].length = enPara->strmBuf_size;
        outStreamInfo.validCompCnt          = 1;

#ifndef CFG_IPTV_TX
        iteJpg_SetStreamInfo(pHJpeg, &inStreamInfo, &outStreamInfo, 0);
#endif
        //iteJpg_GetStatus(pHJpeg, &jpgUserInfo, 0);
        //printf("  (%d, %d) %dx%d, dispMode=%d\r\n",
        //            jpgUserInfo.jpgRect.x, jpgUserInfo.jpgRect.y,
        //            jpgUserInfo.jpgRect.w, jpgUserInfo.jpgRect.h,
        //            initParam.dispMode);
        //printf("  LCD A=0x%x, LCD B=0x%x\n", _Get_Lcd_Addr_A(), _Get_Lcd_Addr_B());

#ifndef CFG_IPTV_TX
        iteJpg_Setup(pHJpeg, 0);

        iteJpg_Process(pHJpeg, &entropyBufInfo, &jpgEncSize, 0);

        iteJpg_GetStatus(pHJpeg, &jpgUserInfo, 0);
#endif
		printf("\n\tresult = %d, encode size = %f KB\n", jpgUserInfo.status, (float)jpgEncSize / 1024);
        enPara->enSize = jpgEncSize;
#ifndef CFG_IPTV_TX
        iteJpg_DestroyHandle(&pHJpeg, 0);
#endif
	}

    CloseEngine(JPEG_ENCODER);

    //ithPrintf("Encoding Time %d\n", PalGetDuration(jpgStart));
}

static void
_DoIsp(
    void)
{
    CAPTURE_RESULT            tEntry       = {0};
    MMP_ISP_SHARE             ispInput     = {0};
    MMP_ISP_OUTPUT_INFO       ispOutput    = {0};
    MMP_ISP_OUTPUT_INFO       ispOutput_PV = {0};
    MMP_ISP_WRITE_BUFFER_INFO ispWbInfo    = {0};

    MMP_UINT16                  inBufferIdx;
    MMP_UINT16                  outBufferIdx;
    static MMP_UINT16           srcCount    = 0;
    MMP_UINT16                  resIdx;

    if (QUEUE_NOT_EMPTY == _CaptureResultQ_Dequeue(&tEntry))
    {
        //ithPrintf("DoISP %d\n", tEntry.resIdx);

        //if (tEntry.resIdx == 1)
        //    return;

        resIdx              = tEntry.resIdx;
        inBufferIdx         = tEntry.frameIndex;

        outBufferIdx        = srcCount; //tEntry.frameIndex;
        gJPEGBufIndex       = outBufferIdx;
        tEntry.frameIndex   = outBufferIdx;

        srcCount            = (srcCount + 1) % gptAVCEncoder[resIdx]->sourceBufCount;

        ispInput.format = MMP_ISP_IN_NV12;
        ispInput.width = gtIspTransformParm[resIdx].inWidth;
        ispInput.height = gtIspTransformParm[resIdx].inHeight;
        ispInput.pitchY = CAPTURE_MAXWIDTH;
        ispInput.pitchUv = CAPTURE_MAXWIDTH;
        ispInput.isAdobe_CMYK = 0;
        ispInput.addrY = gtIspTransformParm[resIdx].inAddrY[inBufferIdx];
        ispInput.addrU = gtIspTransformParm[resIdx].inAddrUV[inBufferIdx];
        //ithPrintf("DoISP (%x,%x), %d, %d, %d, %d)\n", ispInput.addrY, ispInput.addrU, ispInput.width, ispInput.height, ispInput.pitchY, ispInput.pitchUv);
        if (inBufferIdx == 0)
            ispInput.addrYp = gtIspTransformParm[resIdx].inAddrY[2];
        else
            ispInput.addrYp = gtIspTransformParm[resIdx].inAddrY[inBufferIdx - 1];

        ispOutput.addrY = gtIspTransformParm[resIdx].outAddrY[outBufferIdx];
        ispOutput.addrU = gtIspTransformParm[resIdx].outAddrUV[outBufferIdx];
        ispOutput.startX = 0;
        ispOutput.startY = 0;
        ispOutput.width = gptAVCEncoder[resIdx]->frameWidth;
        ispOutput.height = gptAVCEncoder[resIdx]->frameHeight;
        ispOutput.format = MMP_ISP_OUT_NV12;
        ispOutput.pitchY = gptAVCEncoder[resIdx]->framePitchY;
        ispOutput.pitchUv = gptAVCEncoder[resIdx]->framePitchY;
        //ithPrintf("DoISP (%x, %x), (%d, %d, %d, %d)\n", ispOutput.addrY, ispOutput.addrU, ispOutput.width, ispOutput.height, ispOutput.pitchY, ispOutput.pitchUv);
        mmpIspEnableInterrupt(gIspDev);
        mmpIspDisable(gIspDev, MMP_ISP_DEINTERLACE);
        mmpIspSetOutputWindow(gIspDev, &ispOutput);
        mmpIspSetVideoWindow(gIspDev, ispOutput.startX, ispOutput.startY, ispOutput.width, ispOutput.height);
        mmpIspPlayImageProcess(gIspDev, &ispInput);

        memcpy(&gtIspFireEntry, &tEntry, sizeof(CAPTURE_RESULT));
    }
}

static void
_DoEncode(
    void)
{

}

static void
VP_onfly_isr(
    void *arg)
{
    CAPTURE_RESULT  tEntry          = {0};
    MMP_BOOL        bIsIdle         = MMP_FALSE;
    MMP_UINT16      i;
    MMP_UINT16      captureErrState = ithCapGetEngineErrorStatus(&cap0, MMP_CAP_LANE0_STATUS);
    MMP_UINT32      WBIndex         = WriteBufferIndex;

    //ithPrintf("ISP ISR\n");
    //ithPrintf("ISP %d\n", PalGetDuration(tickISP));
    //tickISP = PalGetClock();

    mmpIspClearInterrupt(gIspDev);
    WriteBufferIndex++;
    WriteBufferIndex %= gptAVCEncoder[0]->sourceBufCount;

    //Error frame not encode
    if ((captureErrState & 0x0F00) && gtAVCEncoderFlow.captureFrameCount == 0)
    {
        ithPrintf("---Error frame not encode---\n");
        return;
    }

    //gJPEGBufIndex       = tEntry.frameIndex = mmpVPReturnWrBufIndex();  // TODO: 970 VP not support
    gJPEGBufIndex       = tEntry.frameIndex = WBIndex;
    tEntry.frameCount   = ++gtAVCEncoderFlow.captureFrameCount;

    if (gtAVCEncoderFlow.pfToTimeStamp)
    {
        tEntry.timeStamp                = gtAVCEncoderFlow.currTimeStamp
                                        = gtAVCEncoderFlow.baseTimeStamp
                                        + gtAVCEncoderFlow.pfToTimeStamp(tEntry.frameCount);
    }

    //Error frame not encode
    if (captureErrState & 0x0F00)
    {
        ithPrintf("---Error frame not encode---\n");
        return;
    }

    //if (!_Chk_Skip_Frame(0))
    _CaptureResultQ_Enqueue(&tEntry);

    if (gbJPEGEncoder)
    {
        itpSemPostFromISR(&gpEncoderSem);
        gbJPEGEncoder = MMP_FALSE;
    }
}

static void
cap_isr(
    void *arg)
{
    CAPTURE_RESULT      tEntry          = {0};
    MMP_UINT32          dur             = 0;
    static MMP_UINT32   initClock       = 0;
    static MMP_UINT32   totalDur        = 0;
    MMP_UINT16          captureErrState = ithCapGetEngineErrorStatus(&cap0,MMP_CAP_LANE0_STATUS);
    MMP_UINT16          i;
    MMP_BOOL            bSkipFrame[ENCODE_STREAM_NUM];

    //ithPrintf("cap_isr\n");
    //clear interrupt
    ithCapClearInterrupt(&cap0, MMP_FALSE);

    tEntry.frameIndex   = ithCapReturnWrBufIndex(&cap0);
    tEntry.frameCount   = ++gtAVCEncoderFlow.captureFrameCount;

    if (gtAVCEncoderFlow.pfToTimeStamp)
    {
        tEntry.timeStamp                = gtAVCEncoderFlow.currTimeStamp
                                        = gtAVCEncoderFlow.baseTimeStamp
                                        + gtAVCEncoderFlow.pfToTimeStamp(tEntry.frameCount);
    }

    //Error frame not encode
    if (captureErrState & 0x0F00)
    {
        ithPrintf("---Error frame not encode---\n");
        return;
    }

    for (i = 0; i < VIDEO_STREAM_NUM; i++)
        bSkipFrame[i]   = MMP_FALSE;//_Chk_Skip_Frame(i);

    for (tEntry.resIdx = 0; tEntry.resIdx < ISP_TRANSFORM_NUM; tEntry.resIdx++)
    {
        if (!bSkipFrame[tEntry.resIdx] &&
            ((tEntry.resIdx == 0 && gbStream0Enable) || (tEntry.resIdx == 1 && gbStream1Enable)))
        {
            _CaptureResultQ_Enqueue(&tEntry);
        }
    }
}

static void
VP_mem_isr(
    void *arg)
{
    MMP_BOOL bIsIdle = MMP_FALSE;

    mmpIspClearInterrupt(gIspDev);

    //ithPrintf("VP_mem isr(0)\n");
    //ithPrintf("VP_T %d\n", PalGetDuration(tickStart));

    gtAVCEncoderFlow.encodeQue[gtIspFireEntry.frameIndex] = MMP_TRUE;
    _IspResultQ_Enqueue(&gtIspFireEntry);

    if (gbJPEGEncoder && gtIspFireEntry.resIdx == 0)
    {
        itpSemPostFromISR(&gpEncoderSem);
        gbJPEGEncoder = MMP_FALSE;
    }

}

static void*
isp_task(
    void *arg)
{
    while (gbStream0Enable)
    {
        if (mmpIspIsEngineIdle(gIspDev))
        {
            _DoIsp();
        }
        usleep(1000);
    }
    return 0;
}

#ifdef MULTI_SLICE
void H264SliceReady(H264EncSliceReady *slice)
{
    i32 i, size;
    u8 *strmPtr;
    uint32_t timeoutCnt;
    VIDEO_STREAM_INFO   streamInfo;
    //uint32_t time0;
    //static uint32_t time1;

    //time0 = PalGetClock();
    if (slice->slicesReadyPrev == 0)    /* New frame */
        strmPtr = (u8*)slice->pOutBuf;  /* Pointer to beginning of frame */
    else
        strmPtr = (u8*)slice->pAppData; /* Here we store the slice pointer */

    for (i = slice->slicesReadyPrev; i < slice->slicesReady; i++)
    {
        timeoutCnt = 100;
        ithInvalidateDCacheRange(&slice->sliceSizes[i], 32);
        while (slice->sliceSizes[i] == 0 && timeoutCnt > 0)
        {
            ithInvalidateDCacheRange(&slice->sliceSizes[i], 32);
            ithDelay(100);
            timeoutCnt--;
        }
        if (slice->sliceSizes[i] == 0)
            printf("Error Size(0)\n");
        //printf("#%d:%p:%d %d %d\n", i, strmPtr, slice->sliceSizes[i], slice->slicesReadyPrev, gtAVCEncoderFlow.encodedFrameCount);
        size = slice->sliceSizes[i];
        ithInvalidateDCacheRange(strmPtr, size);

        memcpy(gptAVCEncoder[gEncodeIdx]->pStreamBufAdr[gVideoWrIdx[gEncodeIdx]], strmPtr, size);

        streamInfo.streamIndex  = gEncodeIdx;
        streamInfo.bufIndex     = gVideoWrIdx[gEncodeIdx];
        if (slice->slicesReadyPrev == 0 && ((gtAVCEncoderFlow.encodedFrameCount % VIDEO_FRAME_RATE) == 1))
            streamInfo.bIFrame      = 1;
        else
            streamInfo.bIFrame      = 0;
        streamInfo.dataSize     = size;
        streamInfo.timeStamp    = gtAVCEncoderFlow.encodedTimeStamp;
        streamInfo.bLastSlice   = MMP_FALSE;

        gbStrBufFire[gEncodeIdx][gVideoSelIdx[gEncodeIdx]]  = MMP_TRUE;

        gVideoSelIdx[gEncodeIdx]                            = (gVideoSelIdx[gEncodeIdx] + 1) % VIDEO_SAMPLE_NUM;

        gfpCallback(&streamInfo);
        gVideoWrIdx[gEncodeIdx] = (gVideoWrIdx[gEncodeIdx] + 1) % VIDEO_SAMPLE_NUM;

        strmPtr += size;
        gSliceNum++;
    }
    //printf("Time %d %d\n", PalGetDuration(time0), PalGetDuration(time1));
    //time1 = PalGetClock();
    /* Store the slice pointer for next callback */
    gLastSlicePtr = slice->pAppData = strmPtr;

}

static void
encoder_task(
    void *arg)
{
    MMP_UINT32  streamLen;
    MMP_BOOL    bFrmEnd;
    CAPTURE_RESULT tEntry = {0};
    MMP_UINT8           *pData;
    MMP_UINT32          dataSize    = 500 * 1024; // tmp solution
    VIDEO_STREAM_INFO   streamInfo;
    H264EncIn encIn;
    H264EncOut encOut;
    int ret;
    int i;
    uint32_t timeoutCnt;

    gVideoEncoding[0] = MMP_TRUE;
    while (gbStream0Enable)
    {

         if (QUEUE_NOT_EMPTY == gtAVCEncoderFlow.pfEnDequeue(&tEntry) && gbEncodeStart)
         {
             gEncodeIdx = tEntry.resIdx;

             //if (gEncodeIdx == 1)
             //      return;
             //ithPrintf("gEncodeIdx %d\n", gEncodeIdx);
             if (gbStrBufFire[gEncodeIdx][gVideoSelIdx[gEncodeIdx]])
             {
                 ithPrintf("Encoding Buf Full\n");
                 continue;
             }
             gtAVCEncoderFlow.encodedFrameCount                  = tEntry.frameCount;
             gtAVCEncoderFlow.encodedTimeStamp                   = tEntry.timeStamp;

             gptAVCEncoder[gEncodeIdx]->sourceBufSelect          = tEntry.frameIndex;
             //gbEncodeFire                                        = MMP_TRUE;

             //pData      = gptAVCEncoder[gEncodeIdx]->pStreamBufAdr[gVideoWrIdx[gEncodeIdx]];
             gLastSlicePtr = pData      = stream_buf_sys_addr;

             memset (&encIn, 0, sizeof(H264EncIn));
             encIn.pOutBuf = pData;
             encIn.busOutBuf = pData;
             encIn.outBufSize = dataSize;

#ifndef CFG_IPTV_TX
             memset (pData, 0, dataSize);             
#if defined(CFG_CPU_WB) && (!defined(WIN32))
             ithFlushDCacheRange(pData, dataSize);
             ithFlushMemBuffer();
#endif
#endif
             if (gtAVCEncoderFlow.encodedFrameCount == 1)
                 encIn.timeIncrement = 0;
             else
                 encIn.timeIncrement = 1;

             if ((gtAVCEncoderFlow.encodedFrameCount % VIDEO_FRAME_RATE) == 1)
                 encIn.codingType = H264ENC_INTRA_FRAME;
             else
                 encIn.codingType = H264ENC_PREDICTED_FRAME;

             encIn.busLuma = gptAVCEncoder[gEncodeIdx]->pSourceBufAdrY[gptAVCEncoder[gEncodeIdx]->sourceBufSelect];
             encIn.busChromaU = gptAVCEncoder[gEncodeIdx]->pSourceBufAdrU[gptAVCEncoder[gEncodeIdx]->sourceBufSelect];
             encIn.busChromaV = gptAVCEncoder[gEncodeIdx]->pSourceBufAdrV[gptAVCEncoder[gEncodeIdx]->sourceBufSelect];

             encIn.ipf = H264ENC_REFERENCE_AND_REFRESH;
             encIn.ltrf = H264ENC_REFERENCE;

             //printf("H264 start\n");
             ret = H264EncStrmEncode(gptAVCEncoder[gEncodeIdx]->encoder, &encIn, &encOut, &H264SliceReady, NULL, NULL);

             switch (ret)
             {
                  case H264ENC_FRAME_READY:

                      for (i = gSliceNum; i < (encOut.numNalus-1); i++)
                      {
                          timeoutCnt = 100;
                          //if (gSliceNum != 8)
                          //    printf("NALNum %d %d %d\n", encOut.numNalus-1, gSliceNum, i);
                          ithInvalidateDCacheRange(&encOut.pNaluSizeBuf[i], 32);

                          while (encOut.pNaluSizeBuf[i] == 0 && timeoutCnt > 0)
                          {
                            ithInvalidateDCacheRange(&encOut.pNaluSizeBuf[i], 32);
                            ithDelay(100);
                            timeoutCnt--;
                          }

                          if (encOut.pNaluSizeBuf[i] == 0)
                          {
                              printf("Error Size(1) %d %d %d\n", i, encOut.pNaluSizeBuf[i], encOut.pNaluSizeBuf[i+1]);
                          }


                          ithInvalidateDCacheRange(gLastSlicePtr, encOut.pNaluSizeBuf[i]);
                          memcpy(gptAVCEncoder[gEncodeIdx]->pStreamBufAdr[gVideoWrIdx[gEncodeIdx]], gLastSlicePtr, encOut.pNaluSizeBuf[i]);
                          gLastSlicePtr+=encOut.pNaluSizeBuf[i];
                          //
                          streamInfo.streamIndex  = gEncodeIdx;
                          streamInfo.bufIndex     = gVideoWrIdx[gEncodeIdx];

                          if (i==0)
                              streamInfo.bIFrame      = (encIn.codingType == H264ENC_INTRA_FRAME) ? 1 : 0;
                          else
                              streamInfo.bIFrame      = 0;
                          streamInfo.dataSize     = encOut.pNaluSizeBuf[i];
                          streamInfo.timeStamp    = gtAVCEncoderFlow.encodedTimeStamp;
                          if (i==(encOut.numNalus-2))
                              streamInfo.bLastSlice   = MMP_TRUE;
                          else
                              streamInfo.bLastSlice   = MMP_FALSE;
                          ////printf("streamsize=%d(%d)<%x, %x, %x, %x, %x>\n", encOut.streamSize, streamInfo.bIFrame,
                          ////                              pData[0], pData[1], pData[2], pData[3], pData[4]);
                          //
                          gbStrBufFire[gEncodeIdx][gVideoSelIdx[gEncodeIdx]]  = MMP_TRUE;
                          //
                          gVideoSelIdx[gEncodeIdx]                            = (gVideoSelIdx[gEncodeIdx] + 1) % VIDEO_SAMPLE_NUM;

                          gfpCallback(&streamInfo);
                          gVideoWrIdx[gEncodeIdx] = (gVideoWrIdx[gEncodeIdx] + 1) % VIDEO_SAMPLE_NUM;
                      }

                      gSliceNum = 0;
                      break;

                  case H264ENC_SYSTEM_ERROR:
                      printf("H264ENC_SYSTEM_ERROR\n");
                      break;

                  case H264ENC_OUTPUT_BUFFER_OVERFLOW:
                      printf("H264ENC_OUTPUT_BUFFER_OVERFLOW\n");
                      break;

                  default:
                      printf("H264EncStrmEncode(%d) failed.", ret);
                      break;
             }
         }

         usleep(1000);
    }
    gVideoEncoding[0] = MMP_FALSE;
}

#else

static void*
encoder_task(
    void *arg)
{
#ifdef CFG_IPTV_TX
    MMP_UINT32  streamLen;
    MMP_BOOL    bFrmEnd;
#endif
    CAPTURE_RESULT tEntry = {0};
    MMP_UINT8           *pData;
#ifdef CFG_IPTV_TX
	MMP_UINT32          dataSize    = 1000 * 1024; // tmp solution
#else
    MMP_UINT32          dataSize    = 500 * 1024; // tmp solution
#endif
	VIDEO_STREAM_INFO   streamInfo;
    H264EncIn encIn;
    H264EncOut encOut;
    int ret;
    MMP_UINT32  accSize = 0;

    gVideoEncoding[0] = MMP_TRUE;
    while (gbStream0Enable)
    {

         if (QUEUE_NOT_EMPTY == gtAVCEncoderFlow.pfEnDequeue(&tEntry) && gbEncodeStart)
         {
             gEncodeIdx = tEntry.resIdx;

             if (gbStrBufFire[gEncodeIdx][gVideoSelIdx[gEncodeIdx]])
             {
                 ithPrintf("Encoding Buf Full\n");
                 continue;
             }

             gbStrBufFire[gEncodeIdx][gVideoSelIdx[gEncodeIdx]]  = MMP_TRUE;

             gVideoSelIdx[gEncodeIdx]                            = (gVideoSelIdx[gEncodeIdx] + 1) % VIDEO_SAMPLE_NUM;

             gtAVCEncoderFlow.encodedFrameCount                  = tEntry.frameCount;
             gtAVCEncoderFlow.encodedTimeStamp                   = tEntry.timeStamp;

             gptAVCEncoder[gEncodeIdx]->sourceBufSelect          = tEntry.frameIndex;
             //gbEncodeFire                                        = MMP_TRUE;

             pData      = gptAVCEncoder[gEncodeIdx]->pStreamBufAdr[gVideoWrIdx[gEncodeIdx]];

             memset (&encIn, 0, sizeof(H264EncIn));
             encIn.pOutBuf = pData;
             encIn.busOutBuf = pData;
             encIn.outBufSize = dataSize;

#ifndef CFG_IPTV_TX
             memset (pData, 0, dataSize);             
#if defined(CFG_CPU_WB) && (!defined(WIN32))
             ithFlushDCacheRange(pData, dataSize);
             ithFlushMemBuffer();
#endif
#endif
             if (gtAVCEncoderFlow.encodedFrameCount == 1)
                 encIn.timeIncrement = 0;
             else
                 encIn.timeIncrement = 1;

             if ((gtAVCEncoderFlow.encodedFrameCount % VIDEO_FRAME_RATE) == 1)
                 encIn.codingType = H264ENC_INTRA_FRAME;
             else
                 encIn.codingType = H264ENC_PREDICTED_FRAME;

             if (encIn.codingType == H264ENC_INTRA_FRAME)
                 accSize = 0;
                 
             encIn.busLuma = gptAVCEncoder[gEncodeIdx]->pSourceBufAdrY[gptAVCEncoder[gEncodeIdx]->sourceBufSelect];
             encIn.busChromaU = gptAVCEncoder[gEncodeIdx]->pSourceBufAdrU[gptAVCEncoder[gEncodeIdx]->sourceBufSelect];
             encIn.busChromaV = gptAVCEncoder[gEncodeIdx]->pSourceBufAdrV[gptAVCEncoder[gEncodeIdx]->sourceBufSelect];

             encIn.ipf = H264ENC_REFERENCE_AND_REFRESH;
             encIn.ltrf = H264ENC_REFERENCE;

             //printf("H264 start %d %x\n", gptAVCEncoder[gEncodeIdx]->sourceBufSelect, encIn.pOutBuf);
             ret = H264EncStrmEncode(gptAVCEncoder[gEncodeIdx]->encoder, &encIn, &encOut, NULL, NULL, NULL);

             switch (ret)
             {
                  case H264ENC_FRAME_READY:

                      {
                      //temp solution for network overflow
                      if (encOut.streamSize == 0 || accSize > SENSOR_BITRATE * 170)
                      {
                      	  //printf("Skip Frame %d %d\n", encOut.streamSize, accSize);                      	  
                          gVideoSelIdx[gEncodeIdx]                            = (gVideoSelIdx[gEncodeIdx] + VIDEO_SAMPLE_NUM - 1) % VIDEO_SAMPLE_NUM;
                          gbStrBufFire[gEncodeIdx][gVideoSelIdx[gEncodeIdx]]  = MMP_FALSE;
                      	  break;
                      }
                      
                      ithInvalidateDCacheRange(pData, encOut.streamSize);

                      streamInfo.streamIndex  = gEncodeIdx;
                      streamInfo.bufIndex     = gVideoWrIdx[gEncodeIdx];
                      streamInfo.bIFrame      = (encIn.codingType == H264ENC_INTRA_FRAME) ? 1 : 0;
                      streamInfo.dataSize     = encOut.streamSize;
                      streamInfo.timeStamp    = gtAVCEncoderFlow.encodedTimeStamp;
                      streamInfo.bLastSlice   = MMP_TRUE;
#ifdef CFG_IPTV_TX
                      streamInfo.pData        = pData;
                      streamInfo.frameCount   = gtAVCEncoderFlow.encodedFrameCount;
                      streamInfo.InstanceNum  = gEncodeIdx;
                       
                      streamInfo.frameRate    = gtAVCEncoderFlow.frameRate;
                      streamInfo.EnFrameRate  = gtAVCEncoderFlow.frameRate;
                      streamInfo.width        = gptAVCEncoder[gEncodeIdx]->frameWidth;
                      streamInfo.height       = gptAVCEncoder[gEncodeIdx]->frameHeight;
                      streamInfo.frameType    = (gptAVCEncoder[gEncodeIdx]->bIFrame) ? 0 : 1;
                      streamInfo.bitRate      = 1000*1024;
                      streamInfo.bSkipFrame   = gtAVCEncoderFlow.bSkipFrame;
#endif          
                      accSize += encOut.streamSize;       
                      //printf("streamsize=%d(%d)<%x, %x, %x, %x, %x> timeStamp:%d\n", encOut.streamSize, streamInfo.bIFrame,
                      //								pData[0], pData[1], pData[2], pData[3], pData[4],streamInfo.timeStamp);
                      
                      gfpCallback(&streamInfo);
                      gVideoWrIdx[gEncodeIdx] = (gVideoWrIdx[gEncodeIdx] + 1) % VIDEO_SAMPLE_NUM;           
#ifdef CFG_IPTV_TX
                      gbStrBufFire[gEncodeIdx][gVideoSelIdx[gEncodeIdx]]  = MMP_FALSE;
#endif
                      }

                      break;

                  case H264ENC_SYSTEM_ERROR:
                      printf("H264ENC_SYSTEM_ERROR\n");
                      break;

                  case H264ENC_OUTPUT_BUFFER_OVERFLOW:
                      printf("H264ENC_OUTPUT_BUFFER_OVERFLOW\n");
                      break;

                  default:
                      printf("H264EncStrmEncode(%d) failed.", ret);
                      break;
             }
         }

         usleep(1000);
    }
    gVideoEncoding[0] = MMP_FALSE;
    return 0;
}
#endif

static MMP_BOOL
_Chk_Skip_Frame(
    MMP_UINT32 InstanceNum)
{
    MMP_UINT32  skipNum;
    MMP_BOOL    bSkipFrm = MMP_FALSE;

    if (gtAVCEncoderFlow.frameRateDiff[InstanceNum] == 0 ||
        (!gbStream0Enable && InstanceNum == 0) || (!gbStream1Enable && InstanceNum == 1))
    {
        bSkipFrm = MMP_FALSE;
        goto End;
    }

    skipNum = (gtAVCEncoderFlow.ispFrameRate[InstanceNum] / gtAVCEncoderFlow.frameRateDiff[InstanceNum]) * gtAVCEncoderFlow.skipCount[InstanceNum];

    if (gtAVCEncoderFlow.frameCount[InstanceNum]++ == skipNum)
    {
        if (gtAVCEncoderFlow.skipCount[InstanceNum]++ == gtAVCEncoderFlow.frameRateDiff[InstanceNum])
            gtAVCEncoderFlow.skipCount[InstanceNum] = 1;

        if (gtAVCEncoderFlow.frameCount[InstanceNum] == (gtAVCEncoderFlow.ispFrameRate[InstanceNum] + 1))
            gtAVCEncoderFlow.frameCount[InstanceNum] = 1;

        bSkipFrm = MMP_TRUE;
    }
    else
        bSkipFrm = MMP_FALSE;

End:

    //ithPrintf("IsSkip(%d) FC %d SkipCount %d SkipNum %d\n", bSkipFrm, gtAVCEncoderFlow.frameCount[InstanceNum], gtAVCEncoderFlow.skipCount[InstanceNum], skipNum);

    return bSkipFrm;
}

static int
_Chk_Cap_Fire(
    CAPTURE_HANDLE* ptDev)
{
    uint32_t    OutWidth=0,OutHeight =0,ROI_Flag=0, timeout = 0;
    CAP_CONTEXT *Capctxt = &ptDev->cap_info;

#ifdef CFG_IPTV_TX
    //Detected Hsync.
    while(!(ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS) & 0x1))
    {
        if(++timeout > 50)
        {
            printf("Hsync not stable!\n", ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS));
            return 1;
        }
        usleep(1000);
    }

    //wait for getting interleave
    //usleep(30000);
    Capctxt->ininfo.Interleave = ithCapGetDetectedInterleave(ptDev);
    //printf("Get interleave=%d\n",Capctxt->ininfo.Interleave);
    ithCapSetInterleave(ptDev,Capctxt->ininfo.Interleave);

    while((ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS)&0x3) != 0x3)
    {
        //printf("Hsync or Vsync not stable!\n");
        usleep(1000);
    }

    Capctxt->ininfo.capwidth = Capctxt->ininfo.PitchY = Capctxt->ininfo.PitchUV = ithCapGetDetectedWidth(ptDev);
    Capctxt->ininfo.capheight = ithCapGetDetectedHeight(ptDev);
#else
    Capctxt->ininfo.capwidth = Capctxt->ininfo.PitchY = Capctxt->ininfo.PitchUV = 1280;
    Capctxt->ininfo.capheight = 720;
    Capctxt->ininfo.Interleave = 0;
#endif
	//printf("Get interleave=%d\n",Capctxt->ininfo.Interleave);
    ithCapSetInterleave(ptDev,Capctxt->ininfo.Interleave);
    //printf("Width=%d(%d, %d),Height=%d\n",Capctxt->ininfo.capwidth,Capctxt->ininfo.PitchY,Capctxt->ininfo.PitchUV,Capctxt->ininfo.capheight);
    ithCapSetWidthHeight(ptDev,Capctxt->ininfo.capwidth,Capctxt->ininfo.capheight);
    ithCapFire(ptDev, MMP_TRUE);
    //printf("Set WidthHeight and fire! (%d)\n", ptDev->cap_id);

    if(Capctxt->ininfo.capwidth % 2)
    {
        OutWidth = Capctxt->ininfo.capwidth -1;
        ROI_Flag = 1;
    }
    else
        OutWidth  = Capctxt->ininfo.capwidth;

    if(Capctxt->ininfo.capheight % 2)
    {
        OutHeight = Capctxt->ininfo.capheight - 1;
        ROI_Flag = 1;
    }
    else
        OutHeight = Capctxt->ininfo.capheight;

    printf("OutWidth =%d,OutHeight=%d\n",OutWidth,OutHeight);
    if(ROI_Flag)
    {
        printf("ROI_Flag=%d\n",ROI_Flag);
        ithCapWaitEngineIdle(ptDev);
        ithCapSetROIWidthHeight(ptDev,OutWidth,OutHeight);
        ithCapFire(ptDev, MMP_TRUE);
        ROI_Flag = 0;
    }
    return 0;
}
