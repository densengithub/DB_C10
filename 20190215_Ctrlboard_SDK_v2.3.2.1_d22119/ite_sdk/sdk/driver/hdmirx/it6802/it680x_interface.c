///*****************************************
//  Copyright (C) 2009-2018
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
#include <stdio.h>
#include <stdlib.h>
#include "hdmirx/it6802/typedef.h"
#include "hdmirx/it6802/it680x.h"
#include "hdmirx/it6802/IO.h"
#include "hdmirx/it6802/Utility.h"
#include "hdmirx/it6802/debug.h"
#include "hdmirx/it6802/it680x_interface.h"


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

//=============================================================================
//                Private Function Definition
//=============================================================================
static unsigned int IT6802_OPTION(IT6802_PROPERTY property)
{
    unsigned int value;
    switch (property)
    {
        case IT6802_IS_DVI_MODE:
            value = (IT6802getHDMIMode()? 0: 1);
            break;

        case IT6802_CHECK_ENGINE_IDLE:
            break;
        case IT6802_ASPECT_RATIO:
            switch(IT6802getDAR())
            {
                case M_AVI_FMT_ASRATIO_4_3:
                    value = IT6802_PAR_4_3;
                case M_AVI_FMT_ASRATIO_16_9:
                    value = IT6802_PAR_16_9;
                case M_AVI_FMT_ASRATIO_14_9:
                    value = IT6802_PAR_14_9;
            }
            break;
        case IT6802_OUTPUT_VIDEO_MODE:
            value = IT6802getOutputVideoMode();
            break;

        case IT6802_PCLK:
            value = IT6802getPCLK();
            break;

        case IT6802_HEIGHT:
            value = IT6802getVertActive();
            break;

        case IT6802_WIDTH:
            value = IT6802getHorzActive();
            break;

        case IT6802_HTOTAL:
            value = IT6802getHorzTotal();
            break;

        case IT6802_VTOTAL:
            value = IT6802getVertTotal();
            break;

        case IT6802_COLOR_DEPTH:
            //output color depth
            switch(IT6802getOutputColorDepth())
            {
                case 0x0:
                    value = 24;
                    break;
                case 0x01:
                    value = 30;
                    break;
                case 0x10:
                    value = 36;
                    break;    
            }
            break;

        case IT6802_IS_INTERLACE:
            value = IT6802getIsInterlace();// 1: interlace 
            break;

        case IT6802_IS_VID_STABLE:
            value = IT6802getSCDT(); // 1: stable  0:unstable
            break;
            
       case IT6802_IS_AUDIO_ON:
            value = IT6802getIsAudioOn();
            break;

        case IT6802_IS_AUDIO_RESET:
            value = IT6802getIsAudioReset();
            break;

        case IT6802_AUDIO_SAMPLERATE:
            value = IT6802getAudioSampleFrq();
            switch(value)
            {
                case B_FS_44100:
                    value = 44100; break;
                case B_FS_48000:
                    value = 48000; break;
                case B_FS_32000:
                    value  = 32000; break;
                case B_FS_22050:
                    value = 22050; break;
                case B_FS_24000:
                    value = 24000; break;
                case B_FS_88200:
                    value = 88200; break;
                case B_FS_768000:
                    value = 768000; break;
                case B_FS_96000:    
                    value = 96000; break;
                case B_FS_176400:       
                    value = 176400; break;
                case B_FS_192000:       
                    value = 192000; break;
                case B_FS_NOTID:
                default:
                    value = 0; break;    
               
            }
            break;

        case IT6802_AUDIO_CHANNEL_NUMBER:
            value = IT6802getAudioCannelNum();
            break;

        case IT6802_AUDIO_MODE:
            switch(IT6802getAudioMode())
            {
                case 1:
                    value = IT6802_AUDIO_HBR; break;
                case 2:
                    value = IT6802_AUDIO_DSD; break;
                case 3:
                    value = IT6802_AUDIO_NLPCM; break;
                case 4:
                    value = IT6802_AUDIO_LPCM; break;
                case 0:
                default:
                    value = IT6802_AUDIO_OFF; break;
            }
            break;

        //case IT6802_CHECK_PLG5V_PWR:
            //break;
                          
        default:
            break;
    }  
    return value;
}

//=============================================================================
//                Public Function Definition
//=============================================================================

//=============================================================================
/**
 * * Device Initialize
 */
//=============================================================================
void
IT680xInitialize(
    void)
{
    IT6802_fsm_init();
}

//=============================================================================
/**
 * Device Output Pin Tri-State.
 */
//=============================================================================
void
IT680xOutputPinTriState(
    BOOL flag)
{
    if(flag == 1)
    {
        IT6802VideoOutputEnable(0);//off
        IT6802AudioOutputEnable(0);
    }
    else
    {
        IT6802VideoOutputEnable(1);//on
        IT6802AudioOutputEnable(1);
    }
}

//=============================================================================
/**
 * check Device Signal State
 */
//=============================================================================
BOOL
IT680xIsSignalStable(
    void)
{
    IT6802_fsm();

    return (BOOL)(IsVideoOn() && IT6802getSCDT());
}

//=============================================================================
/**
 * * Device get property.
 */
//=============================================================================
unsigned int
IT680xGetProperty(
    MMP_HDMIRX_PROPERTY property)
{
    
    return IT6802_OPTION(property);;
}

//=============================================================================
/**
 * * Device set property.
 */
//=============================================================================
void
IT680xSetProperty(
    MMP_HDMIRX_PROPERTY property, unsigned int value)
{
    switch (property)
    {
        default:
            break;
    }
}
//=============================================================================
/**
 * * Check HDCP.
 */
//=============================================================================
void IT680xDisableHDCP(
    BOOL flag)
{
    printf("(%s) no supported \n",__FUNCTION__);
}

BOOL
IT680xIsHDCPOn(
    void)
{
    return 1;
}

//=============================================================================
/**
 * * Get DataEnable Timing.
 */
//=============================================================================
void
IT680xGetDETiming(
    MMP_HDMIRX_DETIMING *pDETiming)
{
    unsigned int HSync      = IT6802getHorzSyncWidth();
    unsigned int HBackPorch = IT6802getHorzBackPorch();
    unsigned int HActive    = IT6802getHorzActive();
    unsigned int VSync      = IT6802getVertSyncWidth();
    unsigned int VBackPorch = IT6802getVertBackPorch();
    unsigned int VActive    = IT6802getVertActive();

    pDETiming->HDES = HSync + HBackPorch - 2;
    pDETiming->HDEE = HSync + HBackPorch - 2 + HActive;

    pDETiming->VDES = VSync + VBackPorch - 1;
    pDETiming->VDEE = VSync + VBackPorch - 1 + VActive;

    //printf("---HDES = %d, HDEE = %d, VDES = %d, VDEE = %d---\n", pDETiming->HDES, pDETiming->HDEE, pDETiming->VDES, pDETiming->VDEE);
}

//=============================================================================
/**
 * Device power down
 */
//=============================================================================
BOOL
IT680xPowerDown(
    BOOL enable)
{
    //IT680x_StandbyMode(enable);// 1: power down (standby mode) 0 : resume
    IT6802PowerDown(enable);
    return TRUE;
}
