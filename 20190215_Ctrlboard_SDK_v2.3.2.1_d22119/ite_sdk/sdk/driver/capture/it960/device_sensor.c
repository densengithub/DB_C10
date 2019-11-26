#include <string.h> 
#include "mmp_capture.h"
#include "capture/sensor_device_table.h"
#include "capture_config.h"

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
static MMP_BOOL     gtDeviceReboot = MMP_TRUE;

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
void
ithCapDeviceAllDeviceTriState(
    void)
{
}

//=============================================================================
/**
 * Cap Device initialization.
 */
//=============================================================================
MMP_RESULT
ithCapDeviceInitialize(
    void)
{
    MMP_RESULT result = MMP_SUCCESS;

    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s error %d", __FUNCTION__, __LINE__);

    return (MMP_RESULT)result;
}

//=============================================================================
/**
 * Cap Device terminate.
 */
//=============================================================================
void
ithCapDeviceTerminate(
    CAPTURE_HANDLE *ptDev)
{
}

//=============================================================================
/**
 * Cap Device Signal State.
 */
//=============================================================================
MMP_BOOL
ithCapDeviceIsSignalStable(
    void)
{
    return MMP_TRUE;
}


//=============================================================================
/**
 * CapGetDeviceInfo
 */
//=============================================================================
void
ithCapGetDeviceInfo(
    CAPTURE_HANDLE *ptDev)
{
    CAP_CONTEXT        *Capctxt        = &ptDev->cap_info;

    /* Set Active Region */
    Capctxt->ininfo.capwidth       = (MMP_UINT16)SENSOR_TABLE.HActive;
    Capctxt->ininfo.capheight      = (MMP_UINT16)SENSOR_TABLE.VActive;
    /* Y UV Pitch */
    Capctxt->ininfo.PitchY         = Capctxt->ininfo.capwidth;
    Capctxt->ininfo.PitchUV        = Capctxt->ininfo.capwidth;
    #if defined(CFG_SENSOR_FLICK_50HZ)
    Capctxt->ininfo.framerate      = MMP_CAP_FRAMERATE_25HZ;
    #else
    Capctxt->ininfo.framerate      = MMP_CAP_FRAMERATE_30HZ;
    #endif
    /* Set input ROI info */
    Capctxt->ininfo.ROIPosX        = SENSOR_TABLE.ROIPosX;
    Capctxt->ininfo.ROIPosY        = SENSOR_TABLE.ROIPosY;
    Capctxt->ininfo.ROIWidth       = SENSOR_TABLE.ROIWidth;
    Capctxt->ininfo.ROIHeight      = SENSOR_TABLE.ROIHeight;   
    /* Set Interlace or Prograssive */
    Capctxt->ininfo.Interleave     = MMP_TRUE;
    /* Set Polarity */
    Capctxt->ininfo.HSyncPol       = SENSOR_TABLE.HPolarity;
    Capctxt->ininfo.VSyncPol       = SENSOR_TABLE.VPolarity;
    Capctxt->funen.EnCSFun         = MMP_FALSE;
    /* Set Output Parameter */
    Capctxt->outinfo.OutWidth      = Capctxt->ininfo.ROIWidth;  //ROIWidth;
    Capctxt->outinfo.OutHeight     = Capctxt->ininfo.ROIHeight; //ROIHeight;
    Capctxt->outinfo.OutAddrOffset = 0;
    Capctxt->outinfo.OutMemFormat  = SEMI_PLANAR_420;
    
    Capctxt->bMatchResolution = MMP_TRUE;
}

//=============================================================================
/**
 * Device reboot.
 */
//=============================================================================
MMP_RESULT
ithCapSetDeviceReboot(
    MMP_BOOL flag)
{
    gtDeviceReboot = flag;

    if (!gtDeviceReboot)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "------Not Reboot Device-------\n");
}

MMP_BOOL
ithCapGetDeviceReboot(
    void)
{
    return gtDeviceReboot;
}

//=============================================================================
/**
 * Capture Resolution.
 */
//=============================================================================
MMP_UINT16
ithCapGetResolutionIndex(
    MMP_CAP_DEVICE_ID id)
{
    return 0;
}

//=============================================================================
/**
 * Capture VESA Timing Check.
 */
//=============================================================================
MMP_BOOL
ithCapVESATimingCheck(
    CAPTURE_HANDLE          *ptDev,
    MMP_UINT16          width,
    MMP_UINT16          height,
    MMP_CAP_INPUT_INFO  *info)
{
    (*info) = MMP_CAP_INPUT_INFO_CAMERA;
    return MMP_TRUE;
}
