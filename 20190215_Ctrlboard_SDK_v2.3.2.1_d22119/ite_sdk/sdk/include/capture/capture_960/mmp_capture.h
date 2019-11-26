#ifndef __MMP_CAPTURE_H__
#define __MMP_CAPTURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "capture_types.h"

//=============================================================================
//                              Constant Definition
//=============================================================================
#define CAP_API     extern

//=============================================================================
//                              Constant Definition
//=============================================================================

#define CAP_HDMI_INPUT_VESA      14
/**
 *  Device Select
 */
typedef enum MMP_CAP_DEVICE_ID_TAG
{
    MMP_CAP_UNKNOW_DEVICE = 0,
    MMP_CAP_DEV_ADV7180,
    MMP_CAP_DEV_CAT9883,
    MMP_CAP_DEV_HDMIRX,
    MMP_CAP_DEV_SENSOR,
    MMP_CAP_DEV_MAX,
} MMP_CAP_DEVICE_ID;

typedef enum MMP_CAP_SKIP_MODE_TAG
{
    MMP_CAPTURE_NO_DROP = 0,
    MMP_CAPTURE_SKIP_BY_TWO,
    MMP_CAPTURE_SKIP_BY_THREE,
    MMP_CAPTURE_SKIP_30FPS_TO_25FPS,
    MMP_CAPTURE_SKIP_60FPS_TO_25FPS,
} MMP_CAP_SKIP_MODE;

typedef enum MMP_CAP_FUN_FLAG_TAG
{
    //Enable/Disable Interrupt
    MMP_CAP_INTERRUPT = 0,

    //Enable/Disable Capture Onfly Mode
    MMP_CAP_ONFLY_MODE,
} MMP_CAP_FUN_FLAG;

typedef enum MMP_CAP_LANE_STATUS_TAG
{
    MMP_CAP_LANE0_STATUS,
    MMP_CAP_LANE1_STATUS,
    MMP_CAP_LANE2_STATUS,
    MMP_CAP_LANE3_STATUS
} MMP_CAP_LANE_STATUS;

typedef enum MMP_CAP_ERROR_CODE_TAG
{
    MMP_HSYNC_ERR = 0x1,        //0x01
    MMP_VSYNC_ERR,              //0x02
    MMP_SIZE_ERROR_ERR,         //0x03
    MMP_OVERFLOW_ERR,           //0x04
    MMP_ABNORMAL_TERMINATE_ERR, //0x05
    MMP_TIMEOUT_ERR,            //0x06
    MMP_SCDTO_ERR,              //0x07
    MMP_BT656_SYNC_ERR,         //0x08
    MMP_BT656_PREAMBLE_ERR,     //0x09
    MMP_MEM_LAST_ERR,           //0x10
    MMP_FRAME_SYNC_POINT_ERR,   //0x11
    MMP_FRAME_SYNC_ACTIVE_ERR,  //0x12
    MMP_FIRE_ISP_ERR,           //0x13
    MMP_INTERNAL_DATAPTH_ERR    //0x14
} MMP_CAP_ERROR_CODE;

typedef enum MMP_CAP_FRAMERATE_TAG
{
    MMP_CAP_FRAMERATE_UNKNOW = 0,
    MMP_CAP_FRAMERATE_25HZ,
    MMP_CAP_FRAMERATE_50HZ,
    MMP_CAP_FRAMERATE_30HZ,
    MMP_CAP_FRAMERATE_60HZ,
    MMP_CAP_FRAMERATE_29_97HZ,
    MMP_CAP_FRAMERATE_59_94HZ,
    MMP_CAP_FRAMERATE_23_97HZ,
    MMP_CAP_FRAMERATE_24HZ,
    MMP_CAP_FRAMERATE_56HZ,
    MMP_CAP_FRAMERATE_70HZ,
    MMP_CAP_FRAMERATE_72HZ,
    MMP_CAP_FRAMERATE_75HZ,
    MMP_CAP_FRAMERATE_85HZ,
    MMP_CAP_FRAMERATE_VESA_30HZ,
    MMP_CAP_FRAMERATE_VESA_60HZ
} MMP_CAP_FRAMERATE;

typedef enum MMP_CAP_VIDEO_SOURCE_TAG
{
    MMP_CAP_VIDEO_SOURCE_UNKNOW = 0,
    MMP_CAP_VIDEO_SOURCE_CVBS,
    MMP_CAP_VIDEO_SOURCE_SVIDEO,
    MMP_CAP_VIDEO_SOURCE_YPBPR,
    MMP_CAP_VIDEO_SOURCE_VGA,
    MMP_CAP_VIDEO_SOURCE_HDMI,
    MMP_CAP_VIDEO_SOURCE_SENSOR
} MMP_CAP_VIDEO_SOURCE;

typedef enum MMP_CAP_INPUT_INFO_TAG
{
    MMP_CAP_INPUT_INFO_640X480_60P   = 0,
    MMP_CAP_INPUT_INFO_720X480_59I   = 1,
    MMP_CAP_INPUT_INFO_720X480_59P   = 2,
    MMP_CAP_INPUT_INFO_720X480_60I   = 3,
    MMP_CAP_INPUT_INFO_720X480_60P   = 4,
    MMP_CAP_INPUT_INFO_720X576_50I   = 5,
    MMP_CAP_INPUT_INFO_720X576_50P   = 6,
    MMP_CAP_INPUT_INFO_1280X720_50P  = 7,
    MMP_CAP_INPUT_INFO_1280X720_59P  = 8,
    MMP_CAP_INPUT_INFO_1280X720_60P  = 9,
    MMP_CAP_INPUT_INFO_1920X1080_23P = 10,
    MMP_CAP_INPUT_INFO_1920X1080_24P = 11,
    MMP_CAP_INPUT_INFO_1920X1080_25P = 12,
    MMP_CAP_INPUT_INFO_1920X1080_29P = 13,
    MMP_CAP_INPUT_INFO_1920X1080_30P = 14,
    MMP_CAP_INPUT_INFO_1920X1080_50I = 15,
    MMP_CAP_INPUT_INFO_1920X1080_50P = 16,
    MMP_CAP_INPUT_INFO_1920X1080_59I = 17,
    MMP_CAP_INPUT_INFO_1920X1080_59P = 18,
    MMP_CAP_INPUT_INFO_1920X1080_60I = 19,
    MMP_CAP_INPUT_INFO_1920X1080_60P = 20,
    MMP_CAP_INPUT_INFO_800X600_60P   = 21,
    MMP_CAP_INPUT_INFO_1024X768_60P  = 22,
    MMP_CAP_INPUT_INFO_1280X768_60P  = 23,
    MMP_CAP_INPUT_INFO_1280X800_60P  = 24,
    MMP_CAP_INPUT_INFO_1280X960_60P  = 25,
    MMP_CAP_INPUT_INFO_1280X1024_60P = 26,
    MMP_CAP_INPUT_INFO_1360X768_60P  = 27,
    MMP_CAP_INPUT_INFO_1366X768_60P  = 28,
    MMP_CAP_INPUT_INFO_1440X900_60P  = 29,
    MMP_CAP_INPUT_INFO_1400X1050_60P = 30,
    MMP_CAP_INPUT_INFO_1440X1050_60P = 31,
    MMP_CAP_INPUT_INFO_1600X900_60P  = 32,
    MMP_CAP_INPUT_INFO_1600X1200_60P = 33,
    MMP_CAP_INPUT_INFO_1680X1050_60P = 34,
    MMP_CAP_INPUT_INFO_ALL           = 35,
    MMP_CAP_INPUT_INFO_NUM           = 36,
    MMP_CAP_INPUT_INFO_UNKNOWN       = 37,
    MMP_CAP_INPUT_INFO_CAMERA        = 38,
} MMP_CAP_INPUT_INFO;

typedef MMP_UINT32 AV_SYNC_COUNTER_CTRL;

#define AUDIO_COUNTER_CLEAR 0x0001
#define VIDEO_COUNTER_CLEAR 0x0002
#define MUTE_COUNTER_CLEAR  0x0004
#define AUDIO_COUNTER_LATCH 0x0008
#define VIDEO_COUNTER_LATCH 0x0008
#define MUTE_COUNTER_LATCH  0x0010
#define AUDIO_COUNTER_SEL   0x0020
#define VIDEO_COUNTER_SEL   0x0040
#define MUTE_COUNTER_SEL    0x0080
#define I2S_SOURCE_SEL      0x0100
#define SPDIF_SOURCE_SEL    0x0200
#define MUTEPRE_COUNTER_SEL 0x0400

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct MMP_CAP_COLOR_CTRL_TAG
{
    MMP_INT32 brightness;
    MMP_FLOAT contrast;
    MMP_INT32 hue;
    MMP_FLOAT saturation;
} MMP_CAP_COLOR_CTRL;

typedef struct ITE_CAP_VIDEO_INFO_TAG
{
    MMP_UINT16          OutWidth;
    MMP_UINT16          OutHeight;
    MMP_UINT8           IsInterlaced;
    MMP_CAP_FRAMERATE FrameRate;
    MMP_UINT8           *DisplayAddrY;
    MMP_UINT8           *DisplayAddrU;
    MMP_UINT8           *DisplayAddrV;
    MMP_UINT8           *DisplayAddrOldY;
    MMP_UINT16          PitchY;
    MMP_UINT16          PitchUV;
	MMP_UINT16          OutMemFormat;
} ITE_CAP_VIDEO_INFO;

typedef enum CAPTURE_DEV_ID_TAG
{
    CAP_DEV_ID0     = 0,
    CAP_DEV_ID1     = 1
} CAPTURE_DEV_ID;


typedef struct CAPTURE_HANDLE_TAG
{
    CAP_CONTEXT         cap_info;   //cap device info
    CAPTURE_DEV_ID      cap_id;     //cap 0 or cap1
    MMP_CAP_DEVICE_ID   source_id;  // front source id
} CAPTURE_HANDLE;

typedef struct CAPTURE_SETTING_TAG
{
	MMP_CAP_DEVICE_ID inputsource;
	MMP_BOOL OnflyMode_en;
	MMP_BOOL Interrupt_en;
	MMP_UINT32 Max_Width;
	MMP_UINT32 Max_Height;
	
} CAPTURE_SETTING;

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================

//=============================================================================
/**
 * Cap Device Tri-State.
 */
//=============================================================================
CAP_API void
ithCapDeviceAllDeviceTriState(
    void);

//=============================================================================
/**
 * Cap Device initialization.
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapDeviceInitialize(
    void);

//=============================================================================
/**
 * Cap Device terminate.
 */
//=============================================================================
CAP_API void
ithCapDeviceTerminate(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * Cap Get Device Signal State.
 */
//=============================================================================
CAP_API MMP_BOOL
ithCapDeviceIsSignalStable(
    void);

//=============================================================================
/**
 * ithCapGetDeviceInfo
 */
//=============================================================================
CAP_API void
ithCapGetDeviceInfo(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * Device reboot.
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapSetDeviceReboot(
    MMP_BOOL flag);

CAP_API MMP_BOOL
ithCapGetDeviceReboot(
    void);

//=============================================================================
/**
 * Capture Resolution.
 */
//=============================================================================
CAP_API MMP_UINT16
ithCapGetResolutionIndex(
    MMP_CAP_DEVICE_ID id);

//=============================================================================
/**
 * Capture VESA Timing Check.
 */
//=============================================================================
CAP_API MMP_BOOL
ithCapVESATimingCheck(
	CAPTURE_HANDLE *ptDev,
    MMP_UINT16 width,
    MMP_UINT16 height,
    MMP_CAP_INPUT_INFO *info);

//=============================================================================
/**
 * Cap context initialization.
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapInitialize(
    void);


//=============================================================================
/**
 * Cap Connected Source.
 */
//=============================================================================

CAP_API MMP_RESULT
ithCapConnect(
    CAPTURE_HANDLE *cap_handle, CAPTURE_SETTING info);

//=============================================================================
/**
 * Cap DisConnected Source.
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapDisConnect(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * Cap terminate.
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapTerminate(
    void);

//=============================================================================
/**
 * ithCapIsFire
 */
//=============================================================================
CAP_API MMP_BOOL
ithCapIsFire(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapGetEngineErrorStatus
 */
//=============================================================================
CAP_API MMP_UINT32
ithCapGetEngineErrorStatus(
    CAPTURE_HANDLE *ptDev,MMP_CAP_LANE_STATUS lanenum);

#if 0
//=============================================================================
/**
 * ithCapGetEngineErrorCodeStatus
 */
//=============================================================================
CAP_API MMP_UINT32
ithCapGetEngineErrorCodeStatus(
    CAPTURE_HANDLE *ptDev);
#endif
//=============================================================================
/**
 * ithCapGetAllEngineStatus
 */
//=============================================================================
CAP_API void
ithCapGetAllEngineStatus(
    CAPTURE_HANDLE *ptDev);

#if 0
//=============================================================================
/**
 * ithCapOnflyResetAllEngine
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapOnflyResetAllEngine(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapResetEngine
 */
//=============================================================================
MMP_RESULT
ithCapResetEngine(
    CAPTURE_HANDLE *ptDev);
#endif
//=============================================================================
/**
 * ithCapProcess
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapParameterSetting(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapFire Enable or Disable
 */
//=============================================================================
CAP_API void
ithCapFire(
    CAPTURE_HANDLE *ptDev, MMP_BOOL enable);

#if 0
//=============================================================================
/**
 * ithCapMemoryInitialize
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapMemoryInitialize(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapMemoryClear
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapMemoryClear(
    CAPTURE_HANDLE *ptDev);
#endif
//=============================================================================
/**
 * ithCapRegisterIRQ
 */
//=============================================================================
CAP_API void
ithCapRegisterIRQ(
    ITHIntrHandler caphandler,CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapDisableIRQ
 */
//=============================================================================
CAP_API void
ithCapDisableIRQ(
    void);

//=============================================================================
/**
 * ithCapClearInterrupt
 */
//=============================================================================
CAP_API MMP_UINT16
ithCapClearInterrupt(
    CAPTURE_HANDLE *ptDev, MMP_BOOL get_err);

//=============================================================================
/**
 * Cap Write Buffer Index
 * @return index number
 */
//=============================================================================
CAP_API MMP_UINT16
ithCapReturnWrBufIndex(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapGetInputFrameRate
 */
//=============================================================================
CAP_API MMP_CAP_FRAMERATE
ithCapGetInputFrameRate(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithAVSyncCounterInit
 */
//=============================================================================
CAP_API void
ithAVSyncCounterCtrl(
    CAPTURE_HANDLE *ptDev, AV_SYNC_COUNTER_CTRL mode, MMP_UINT16 divider);

//=============================================================================
/**
 * ithAVSyncCounterReset
 */
//=============================================================================
CAP_API void
ithAVSyncCounterReset(
    CAPTURE_HANDLE *ptDev, AV_SYNC_COUNTER_CTRL mode);

//=============================================================================
/**
 * ithAVSyncCounterLatch
 */
//=============================================================================
CAP_API MMP_UINT32
ithAVSyncCounterLatch(
    CAPTURE_HANDLE *ptDev,AV_SYNC_COUNTER_CTRL cntSel);

//=============================================================================
/**
 * ithAVSyncCounterRead
 */
//=============================================================================
CAP_API MMP_UINT32
ithAVSyncCounterRead(
    CAPTURE_HANDLE *ptDev, AV_SYNC_COUNTER_CTRL mode);

//=============================================================================
/**
 * ithAVSyncMuteDetect
 */
//=============================================================================
CAP_API MMP_BOOL
ithAVSyncMuteDetect(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapSetSkipPattern
 */
//=============================================================================
CAP_API MMP_RESULT
ithCapSetSkipMode(
    CAPTURE_HANDLE *ptDev,MMP_CAP_SKIP_MODE mode);

#if 0
//=============================================================================
/**
 * ithCapIsOnflyMode
 */
//=============================================================================
CAP_API MMP_BOOL
ithCapIsOnflyMode(
    CAPTURE_HANDLE *ptDev);
#endif
//=============================================================================
/**
 * ithCapGetOutputFrameRate
 */
//=============================================================================
CAP_API MMP_CAP_FRAMERATE
ithCapGetOutputFrameRate(
    CAPTURE_HANDLE *ptDev,MMP_UINT32 *FrameRated);

//=============================================================================
/**
 * ithCapGetInputSrcInfo
 */
//=============================================================================
CAP_API MMP_CAP_INPUT_INFO
ithCapGetInputSrcInfo(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * Set color control value.
 */
//=============================================================================
CAP_API void
ithCapSetColorCtrl(
    CAPTURE_HANDLE *ptDev,const MMP_CAP_COLOR_CTRL     *data);

//=============================================================================
/**
 * Get color control value.
 */
//=============================================================================
CAP_API void
ithCapGetColorCtrl(
    CAPTURE_HANDLE *ptDev,MMP_CAP_COLOR_CTRL     *data);

//=============================================================================
/**
 * Update Color Matrix.
 */
//=============================================================================
CAP_API void
ithCapUpdateColorMatrix(
     CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * Capture Power Up.
 */
//=============================================================================
CAP_API void
ithCapPowerUp(
    void);

//=============================================================================
/**
 * Capture Power Down.
 */
//=============================================================================
CAP_API void
ithCapPowerDown(
    void);
#if 0
//=============================================================================
/**
 * ithCapFunEnable.
 */
//=============================================================================
CAP_API int32_t
ithCapFunEnable(
     CAPTURE_HANDLE *ptDev,MMP_CAP_FUN_FLAG capfun);

//=============================================================================
/**
 * ithCapFunDisable
 */
//=============================================================================
CAP_API int32_t
ithCapFunDisable(
    CAPTURE_HANDLE *ptDev, MMP_CAP_FUN_FLAG capfun);
#endif
//=============================================================================
/**
 * ithCapSetPolarity
 */
//=============================================================================
CAP_API void
ithCapSetPolarity (
	CAPTURE_HANDLE *ptDev,
    MMP_UINT16 HPolarity,
    MMP_UINT16 VPolarity);

//=============================================================================
/**
 * Wait  Capture engine idle.
 */
//=============================================================================

CAP_API MMP_UINT32
ithCapWaitEngineIdle(
    CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * ithCapEngineReset
 */
//=============================================================================
CAP_API void
ithCapEngineReset(
    void);

//=============================================================================
/**
 * Capture Get Detected Witdh.
 */
//=============================================================================
CAP_API MMP_UINT32 
ithCapGetDetectedWidth(
	CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * Capture Get Detected Height.
 */
//=============================================================================
CAP_API MMP_UINT32 
ithCapGetDetectedHeight(
	CAPTURE_HANDLE *ptDev);

//=============================================================================
/**
 * Capture Get Detected Interleave or not.
 */
//=============================================================================
CAP_API MMP_UINT32 
ithCapGetDetectedInterleave(
	CAPTURE_HANDLE *ptDev);
//=============================================================================
/**
 * Capture Set Polarity
 */
//=============================================================================
CAP_API void
ithCapSetPolarity(
	CAPTURE_HANDLE *ptDev,
    MMP_UINT16 HPolarity,
    MMP_UINT16 VPolarity);
//=============================================================================
/**
 * Capture Set Source is Interleave
 */
//=============================================================================
CAP_API void 
ithCapSetInterleave(
	CAPTURE_HANDLE *ptDev,
	MMP_UINT32 interleave);
//=============================================================================
/**
 * Capture SetWidthHeight
 */
//=============================================================================
CAP_API void 
ithCapSetWidthHeight(
	CAPTURE_HANDLE *ptDev,
	MMP_UINT32 width,
	MMP_UINT32 height);
//=============================================================================
/**
 * Capture SetROIWidthHeight
 */
//=============================================================================
CAP_API void 
ithCapSetROIWidthHeight(
	CAPTURE_HANDLE *ptDev ,
	MMP_UINT32 width,
	MMP_UINT32 height);

#ifdef __cplusplus
}
#endif

#endif