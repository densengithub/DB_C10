#include <string.h> 
#include "mmp_capture.h"
#include "capture/video_device_table.h"
#include "capture_module/capture_module.h"
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
static MMP_BOOL   gtDeviceReboot      = MMP_TRUE;
static CaptureModuleDriver md;

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
    //Tri-State All Device
    ithModule_OutputPinTriState(md, true);
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

#ifdef CFG_CAPTURE_MODULE_ENABLE
    unsigned char name [] ={CFG_CAPTURE_MODULE_NAME};

    if (gtDeviceReboot){
        md = ithModule_Create(name);
        ithModule_Init(md, 0);
    }

    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s error \n", __FUNCTION__);
#endif
    return result;
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
    if (gtDeviceReboot){
        ithModule_OutputPinTriState(md, true);
        ithModule_PowerDown(md, true);
    }
}

//=============================================================================
/**
 * Cap Get Device Signal State.
 */
//=============================================================================
MMP_BOOL
ithCapDeviceIsSignalStable(
    void)
{
    MMP_BOOL isSignalStable = MMP_FALSE;
    isSignalStable = (MMP_BOOL)ithModule_IsSignalStable(md, 0);
    return isSignalStable;
}

//=============================================================================
/**
 * CapGetDeviceInfo for modules 
 */
//=============================================================================

void
ithCapGetDeviceInfo(
    CAPTURE_HANDLE *ptDev)
{
    MMP_BOOL            matchResolution = MMP_FALSE;
    MMP_UINT32          i = 0;
    CAP_CONTEXT         *Capctxt        = &ptDev->cap_info;
    
    Capctxt->ininfo.capwidth   = ithModule_GetProperty(md,  GetWidth);
    Capctxt->ininfo.capheight  = ithModule_GetProperty(md,  GetHeight);
    Capctxt->ininfo.Interleave = ithModule_GetProperty(md,  GetModuleIsInterlace);

    switch(ithModule_GetProperty(md, Rate))
    {
        case 2500:
            Capctxt->ininfo.framerate  = MMP_CAP_FRAMERATE_25HZ;
            break;
        case 2997:
            Capctxt->ininfo.framerate  = MMP_CAP_FRAMERATE_29_97HZ;
            break;
        case 3000:
            Capctxt->ininfo.framerate  = MMP_CAP_FRAMERATE_30HZ; 
            break;
        case 6000:
            Capctxt->ininfo.framerate  = MMP_CAP_FRAMERATE_60HZ; 
            break;
    }
    
    for(i = 0; i < ITH_COUNT_OF(ADV7180_TABLE); i++)
    {
        if ((Capctxt->ininfo.capwidth == ADV7180_TABLE[i].HActive) &&
            (Capctxt->ininfo.capheight == ADV7180_TABLE[i].VActive))
            {
                 /* Set input ROI info */
                Capctxt->ininfo.ROIPosX         = ADV7180_TABLE[i].ROIPosX;
                Capctxt->ininfo.ROIPosY         = ADV7180_TABLE[i].ROIPosY;
                Capctxt->ininfo.ROIWidth        = ADV7180_TABLE[i].ROIWidth;
                Capctxt->ininfo.ROIHeight       = ADV7180_TABLE[i].ROIHeight;
                matchResolution = MMP_TRUE;
            }
    }
    
    /* default color depth 8 bits */
    Capctxt->YUVinfo.ColorDepth     = COLOR_DEPTH_8_BITS;
    /* RGBTOYUV  FUNCTION */
    Capctxt->funen.EnCSFun          = MMP_FALSE;

    /* YUV Pitch */
    Capctxt->ininfo.PitchY          = Capctxt->ininfo.capwidth;
    Capctxt->ininfo.PitchUV         = Capctxt->ininfo.capwidth;

    /* Set output info */
    Capctxt->outinfo.OutWidth      = Capctxt->ininfo.ROIWidth;
    Capctxt->outinfo.OutHeight     = Capctxt->ininfo.ROIHeight;
    Capctxt->outinfo.OutAddrOffset = 0;
    Capctxt->outinfo.OutMemFormat  = SEMI_PLANAR_420;        

    Capctxt->bMatchResolution  = matchResolution;

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
        printf("------Not Reboot Device-------\n");
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

