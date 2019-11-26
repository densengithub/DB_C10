#include <string.h> 
#include "mmp_capture.h"
#include "capture/video_device_table.h"
#include "hdmirx/mmp_hdmirx.h"
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
static MMP_BOOL     gtDeviceReboot      = MMP_TRUE;
static MMP_UINT16   gtHDMIResolution    = 0xFF;
MMP_CAP_INPUT_INFO  infoIdx;

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
    ithHDMIRXOutputPinTriState(MMP_TRUE);
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

    if (gtDeviceReboot)
    {
        gtHDMIResolution    = 0xFF;
        ithHDMIRXInitialize();
    }

    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s error \n", __FUNCTION__);

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
    if (gtDeviceReboot)
    {
        ithCapDeviceAllDeviceTriState();

        ithHDMIRXTerminate();

        gtHDMIResolution    = 0xFF;
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
    MMP_BOOL isSignalChange = MMP_FALSE;

    isSignalChange = ithHDMIRXIsSignalStable();

    return (MMP_BOOL)isSignalChange;
}

//=============================================================================
/**
 * CapGetDeviceInfo for HDMI souce
 * call sequential : ithCapDeviceInitialize -> wait ithCapDeviceIsSignalStable = true -> ithCapGetDeviceInfo
 */
//=============================================================================

void
ithCapGetDeviceInfo(
    CAPTURE_HANDLE *ptDev)
{
    MMP_UINT32          HTotal, VTotal, ColorDepth;
    MMP_UINT16          i, rate, Index;
    MMP_BOOL            matchResolution = MMP_FALSE;
    
    CAP_CONTEXT         *Capctxt        = &ptDev->cap_info;

    Capctxt->YUVinfo.ColorDepth = COLOR_DEPTH_8_BITS;

    if (ptDev->source_id == MMP_CAP_DEV_HDMIRX)
    {
        if(!ithHDMIRXIsSignalStable()){
            printf("[DEVICE_HDMI][Warring]hdmi not stable, can't get info\n");
            return;
        }
    
        Capctxt->ininfo.Interleave  = (MMP_BOOL)ithHDMIRXGetProperty(HDMIRX_IS_INTERLACE);

        if (Capctxt->ininfo.Interleave)
            Capctxt->ininfo.capheight = (MMP_UINT16)ithHDMIRXGetProperty(HDMIRX_HEIGHT) << 1;
        else
            Capctxt->ininfo.capheight = (MMP_UINT16)ithHDMIRXGetProperty(HDMIRX_HEIGHT);

        Capctxt->ininfo.capwidth= (MMP_UINT16)ithHDMIRXGetProperty(HDMIRX_WIDTH);

        HTotal      = ithHDMIRXGetProperty(HDMIRX_HTOTAL);
        VTotal      = ithHDMIRXGetProperty(HDMIRX_VTOTAL);
        /*CEA timing check*/
        for (i = 0; i < CAP_HDMI_INPUT_VESA ; i++)
        {
            if ((HTotal == HDMI_TABLE[i].HTotal) &&
                (VTotal == HDMI_TABLE[i].VTotal || VTotal == HDMI_TABLE[i].VTotal + 1))
            {
                if (i == 6 || i == 7) //1080p60/1080p50 or 1080p60/1080p25
                {
                    MMP_UINT32 hdmirxPCLK = ithHDMIRXGetProperty(HDMIRX_PCLK);
                    if (hdmirxPCLK < 100) //100MHz
                        i = i + 6;       //1080p60/1080p25
                }
                Capctxt->Skippattern         = HDMI_TABLE[i].Skippattern;
                Capctxt->SkipPeriod          = HDMI_TABLE[i].SkipPeriod;
                Capctxt->ininfo.framerate    = HDMI_TABLE[i].FrameRate;
                Capctxt->inmux_info.UCLKDly  = HDMI_TABLE[i].ClkDelay;
                matchResolution              = MMP_TRUE;
                gtHDMIResolution             = i;
                printf("[DEVICE_HDMI]CEA timing \n");
                break;
            }
        }
        /*VESA timing check*/
        if(matchResolution == MMP_FALSE)
        {
            for (i = CAP_HDMI_INPUT_VESA; i < ITH_COUNT_OF(HDMI_TABLE) ; i++)
            {
                if ((HTotal == HDMI_TABLE[i].HTotal) &&
                    (VTotal == HDMI_TABLE[i].VTotal || VTotal == HDMI_TABLE[i].VTotal + 1))
                {
                    Capctxt->Skippattern         = HDMI_TABLE[i].Skippattern;
                    Capctxt->SkipPeriod          = HDMI_TABLE[i].SkipPeriod;
                    Capctxt->ininfo.framerate    = HDMI_TABLE[i].FrameRate;
                    Capctxt->inmux_info.UCLKDly  = HDMI_TABLE[i].ClkDelay;
                    matchResolution              = MMP_TRUE;
                    gtHDMIResolution             = i;
                    printf("[DEVICE_HDMI]VESA timing \n");
                    break;
                }
            }
        }

        if (ithHDMIRXGetProperty(HDMIRX_OUTPUT_VIDEO_MODE) == 0)
        {
            Capctxt->funen.EnCSFun = MMP_TRUE;      //HDMI Rx RGB444 Output
            printf("[DEVICE_HDMI]RGB444 Output\n");
        }
        else
        {
            Capctxt->funen.EnCSFun = MMP_FALSE;     //HDMI Rx YUV444/YUV422 Output
            printf("[DEVICE_HDMI]YUV444/YUV422 Output\n");
        }
        
        if(matchResolution == MMP_FALSE)
            printf("[DEVICE_HDMI][Warring]Not supported timing\n");
        else
            printf("[DEVICE_HDMI]Htotal = %d, Vtotal = %d, w = %d h = %d, res = %d\n", HTotal, VTotal, Capctxt->ininfo.capwidth, Capctxt->ininfo.capheight, gtHDMIResolution);
        
        ColorDepth      = ithHDMIRXGetProperty(HDMIRX_COLOR_DEPTH);

        if (ColorDepth == 36)
            Capctxt->YUVinfo.ColorDepth = COLOR_DEPTH_12_BITS;
        else if (ColorDepth == 30)
            Capctxt->YUVinfo.ColorDepth = COLOR_DEPTH_10_BITS;
        else //(ColorDepth == 24)
            Capctxt->YUVinfo.ColorDepth = COLOR_DEPTH_8_BITS;

    }  

    /*Y UV Pitch*/
    Capctxt->ininfo.PitchY = Capctxt->ininfo.capwidth;
    Capctxt->ininfo.PitchUV = Capctxt->ininfo.capwidth;
    
    /* Set input ROI info*/
    Capctxt->ininfo.ROIPosX         = 0;
    Capctxt->ininfo.ROIPosY         = 0;
    Capctxt->ininfo.ROIWidth        = Capctxt->ininfo.capwidth;
    Capctxt->ininfo.ROIHeight       = Capctxt->ininfo.capheight;

    /*Set output info*/
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
    MMP_UINT16 resIndex;

    switch (id)
    {
        case MMP_CAP_DEV_HDMIRX:
            resIndex = gtHDMIResolution;
            break;
        default:
            resIndex = 0;
            break;
    }

    return resIndex;
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

    MMP_CAP_FRAMERATE   frameRateMode;

    frameRateMode = ithCapGetInputFrameRate(ptDev);

    switch (frameRateMode)
    {
    case MMP_CAP_FRAMERATE_59_94HZ:
    case MMP_CAP_FRAMERATE_60HZ:
        if (width == 800 && height == 600)
        {
            infoIdx = MMP_CAP_INPUT_INFO_800X600_60P;
        }
        else if (width == 1024 && height == 768)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1024X768_60P;
        }
        else if (width == 1280 && height == 720)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1280X720_60P;
        }
        else if (width == 1280 && height == 768)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1280X768_60P;
        }
        else if (width == 1280 && height == 800)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1280X800_60P;
        }
        else if (width == 1280 && height == 960)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1280X960_60P;
        }
        else if (width == 1280 && height == 1024)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1280X1024_60P;
        }
        else if (width == 1360 && height == 768)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1360X768_60P;
        }
        else if (width == 1366 && height == 768)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1366X768_60P;
        }
        else if (width == 1440 && height == 900)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1440X900_60P;
        }
        else if (width == 1400 && height == 1050)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1400X1050_60P;
        }
        else if (width == 1440 && height == 1050)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1440X1050_60P;
        }
        else if (width == 1600 && height == 1200)
        {
        	infoIdx = MMP_CAP_INPUT_INFO_1600X1200_60P;
        }
        else if (width == 1680 && height == 1050)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1680X1050_60P;
        }
        else if (width == 1920 && height == 1080)
        {
            infoIdx = MMP_CAP_INPUT_INFO_1920X1080_60P;
        }
        else
        {
            infoIdx = MMP_CAP_INPUT_INFO_UNKNOWN;
        }
        break;
    default:
        infoIdx = MMP_CAP_INPUT_INFO_UNKNOWN;
        break;
    }

    (*info) = infoIdx;

    if (infoIdx == MMP_CAP_INPUT_INFO_UNKNOWN)
        return MMP_FALSE;
    else
        return MMP_TRUE;
}