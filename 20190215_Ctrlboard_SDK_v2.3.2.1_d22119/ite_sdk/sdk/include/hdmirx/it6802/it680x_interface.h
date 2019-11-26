#ifndef __IT680x_INTERFACE_H__
#define __IT680x_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL
#define BOOL unsigned char
#endif
//=============================================================================
//                              Constant Definition
//=============================================================================
#define MMP_HDMIRX_DETIMING IT6802_DETIMING

//=============================================================================
//                              Constant Definition
//=============================================================================

typedef enum IT6802_PROPERTY_TAG
{
    IT6802_IS_DVI_MODE,
    IT6802_CHECK_ENGINE_IDLE,
    IT6802_ASPECT_RATIO,
    IT6802_OUTPUT_VIDEO_MODE,
    IT6802_PCLK,
    IT6802_HEIGHT,
    IT6802_WIDTH,
    IT6802_HTOTAL,
    IT6802_VTOTAL,
    IT6802_COLOR_DEPTH,
    IT6802_IS_INTERLACE,
    IT6802_IS_VID_STABLE,
    IT6802_IS_AUDIO_ON,
    IT6802_IS_AUDIO_RESET,
    IT6802_AUDIO_SAMPLERATE,
    IT6802_AUDIO_CHANNEL_NUMBER,
    IT6802_AUDIO_MODE,
    IT6802_CHECK_PLG5V_PWR,

} IT6802_PROPERTY;


typedef enum IT6802_AUDIO_MODES_TAG
{
    IT6802_AUDIO_OFF,
    IT6802_AUDIO_HBR,
    IT6802_AUDIO_DSD,
    IT6802_AUDIO_NLPCM,
    IT6802_AUDIO_LPCM,

} IT6802_AUDIO_MODES;

typedef enum IT6802_PAR_TAG
{
    IT6802_PAR_4_3 = 1,
    IT6802_PAR_16_9 = 2,
    IT6802_PAR_14_9 = 3,

} IT6802_PAR;

typedef enum MMP_HDMIRX_PROPERTY_TAG
{
    HDMIRX_IS_DVI_MODE              = IT6802_IS_DVI_MODE            ,
    HDMIRX_CHECK_ENGINE_IDLE        = IT6802_CHECK_ENGINE_IDLE      ,
    HDMIRX_ASPECT_RATIO             = IT6802_ASPECT_RATIO           ,
    HDMIRX_OUTPUT_VIDEO_MODE        = IT6802_OUTPUT_VIDEO_MODE      ,
    HDMIRX_PCLK                     = IT6802_PCLK                   ,
    HDMIRX_HEIGHT                   = IT6802_HEIGHT                 ,
    HDMIRX_WIDTH                    = IT6802_WIDTH                  ,
    HDMIRX_HTOTAL                   = IT6802_HTOTAL                 ,
    HDMIRX_VTOTAL                   = IT6802_VTOTAL                 ,
    HDMIRX_COLOR_DEPTH              = IT6802_COLOR_DEPTH            ,
    HDMIRX_IS_INTERLACE             = IT6802_IS_INTERLACE           ,
    HDMIRX_IS_VID_STABLE            = IT6802_IS_VID_STABLE          ,
    HDMIRX_IS_AUDIO_ON              = IT6802_IS_AUDIO_ON            ,
    HDMIRX_IS_AUDIO_RESET           = IT6802_IS_AUDIO_RESET         ,
    HDMIRX_AUDIO_SAMPLERATE         = IT6802_AUDIO_SAMPLERATE       ,
    HDMIRX_AUDIO_CHANNEL_NUMBER     = IT6802_AUDIO_CHANNEL_NUMBER   ,
    HDMIRX_AUDIO_MODE               = IT6802_AUDIO_MODE             ,
    //HDMIRX_CHECK_PLG5V_PWR          = IT6802_CHECK_PLG5V_PWR        ,

} MMP_HDMIRX_PROPERTY;

typedef enum MMP_HDMIRX_AUDIO_MODE_TAG
{
    HDMIRX_AUDIO_OFF                = IT6802_AUDIO_OFF              ,
    HDMIRX_AUDIO_HBR                = IT6802_AUDIO_HBR              ,
    HDMIRX_AUDIO_DSD                = IT6802_AUDIO_DSD              ,
    HDMIRX_AUDIO_NLPCM              = IT6802_AUDIO_NLPCM            ,
    HDMIRX_AUDIO_LPCM               = IT6802_AUDIO_LPCM             ,

} MMP_HDMIRX_AUDIO_MODE;

typedef enum MMP_HDMIRX_PAR_TAG
{
    HDMIRX_PAR_4_3                  = IT6802_PAR_4_3                ,
    HDMIRX_PAR_16_9                 = IT6802_PAR_16_9               ,
    HDMIRX_PAR_14_9                 = IT6802_PAR_14_9               ,
} MMP_HDMIRX_PAR;


//=============================================================================
//                              Structure Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================


//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct IT6802_DETIMING_TAG
{
    unsigned int        HDES; //H DATA ENABLE START 
    unsigned int        HDEE; //H DATA ENABLE END  
    unsigned int        VDES; //V DATA ENABLE START 
    unsigned int        VDEE; //V DATA ENABLE END 
} IT6802_DETIMING;

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
* Device Initialize
*/
//=============================================================================
void IT680xInitialize(void);
//=============================================================================
/**
 * Device Output Pin Tri-State.
 */
//=============================================================================
void IT680xOutputPinTriState(BOOL flag);
//=============================================================================
/**
* Device Signal State
*/
//=============================================================================
BOOL IT680xIsSignalStable(void);
//=============================================================================
/**
* Device property.
*/
//=============================================================================
unsigned int IT680xGetProperty(MMP_HDMIRX_PROPERTY property);
//=============================================================================
/**
* * Device set property.
*/
//=============================================================================
void IT680xSetProperty(MMP_HDMIRX_PROPERTY property, unsigned int value);
//=============================================================================
/**
* * Check HDCP.
*/
//=============================================================================
BOOL IT680xIsHDCPOn(void);
void IT680xDisableHDCP(BOOL flag);
//=============================================================================
/**
* * Get DE Timing.
*/
//=============================================================================
void IT680xGetDETiming(MMP_HDMIRX_DETIMING *pDETiming);
//=============================================================================
/**
* Device power down
*/
//=============================================================================
BOOL IT680xPowerDown(BOOL enable);


#ifdef __cplusplus
}
#endif

#endif


