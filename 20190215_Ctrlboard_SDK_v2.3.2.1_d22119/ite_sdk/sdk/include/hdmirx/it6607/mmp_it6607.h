#ifndef __MMP_IT6607_H__
#define __MMP_IT6607_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL
#define BOOL unsigned char
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//=============================================================================
//                              Constant Definition
//=============================================================================

//=============================================================================
//                              Constant Definition
//=============================================================================

typedef enum MMP_IT6607_PROPERTY_TAG
{
    IT6607_IS_DVI_MODE,
    IT6607_CHECK_ENGINE_IDLE,
    IT6607_ASPECT_RATIO,
    IT6607_OUTPUT_VIDEO_MODE,
    IT6607_PCLK,
    IT6607_HEIGHT,
    IT6607_WIDTH,
    IT6607_HTOTAL,
    IT6607_VTOTAL,
    IT6607_COLOR_DEPTH,
    IT6607_IS_INTERLACE,
    IT6607_IS_VID_STABLE,
    IT6607_IS_AUDIO_ON,
    IT6607_IS_AUDIO_RESET,
    IT6607_AUDIO_SAMPLERATE,
    IT6607_AUDIO_CHANNEL_NUMBER,
    IT6607_AUDIO_MODE,
    IT6607_CHECK_PLG5V_PWR,

} MMP_IT6607_PROPERTY;


typedef enum MMP_IT6607_AUDIO_MODE_TAG
{
    IT6607_AUDIO_OFF,
    IT6607_AUDIO_HBR,
    IT6607_AUDIO_DSD,
    IT6607_AUDIO_NLPCM,
    IT6607_AUDIO_LPCM,

} MMP_IT6607_AUDIO_MODE;

typedef enum MMP_IT6607_PAR_TAG
{
    IT6607_PAR_4_3 = 1,
    IT6607_PAR_16_9 = 2,

} MMP_IT6607_PAR;

typedef enum MMP_HDMIRX_PROPERTY_TAG
{
    HDMIRX_IS_DVI_MODE              = IT6607_IS_DVI_MODE            ,
    HDMIRX_CHECK_ENGINE_IDLE        = IT6607_CHECK_ENGINE_IDLE      ,
    HDMIRX_ASPECT_RATIO             = IT6607_ASPECT_RATIO           ,
    HDMIRX_OUTPUT_VIDEO_MODE        = IT6607_OUTPUT_VIDEO_MODE      ,
    HDMIRX_PCLK                     = IT6607_PCLK                   ,
    HDMIRX_HEIGHT                   = IT6607_HEIGHT                 ,
    HDMIRX_WIDTH                    = IT6607_WIDTH                  ,
    HDMIRX_HTOTAL                   = IT6607_HTOTAL                 ,
    HDMIRX_VTOTAL                   = IT6607_VTOTAL                 ,
    HDMIRX_COLOR_DEPTH              = IT6607_COLOR_DEPTH            ,
    HDMIRX_IS_INTERLACE             = IT6607_IS_INTERLACE           ,
    HDMIRX_IS_VID_STABLE            = IT6607_IS_VID_STABLE          ,
    HDMIRX_IS_AUDIO_ON              = IT6607_IS_AUDIO_ON            ,
    HDMIRX_IS_AUDIO_RESET           = IT6607_IS_AUDIO_RESET         ,
    HDMIRX_AUDIO_SAMPLERATE         = IT6607_AUDIO_SAMPLERATE       ,
    HDMIRX_AUDIO_CHANNEL_NUMBER     = IT6607_AUDIO_CHANNEL_NUMBER   ,
    HDMIRX_AUDIO_MODE               = IT6607_AUDIO_MODE             ,
    HDMIRX_CHECK_PLG5V_PWR          = IT6607_CHECK_PLG5V_PWR        ,

} MMP_HDMIRX_PROPERTY;

typedef enum MMP_HDMIRX_AUDIO_MODE_TAG
{
    HDMIRX_AUDIO_OFF                = IT6607_AUDIO_OFF              ,
    HDMIRX_AUDIO_HBR                = IT6607_AUDIO_HBR              ,
    HDMIRX_AUDIO_DSD                = IT6607_AUDIO_DSD              ,
    HDMIRX_AUDIO_NLPCM              = IT6607_AUDIO_NLPCM            ,
    HDMIRX_AUDIO_LPCM               = IT6607_AUDIO_LPCM             ,

} MMP_HDMIRX_AUDIO_MODE;

typedef enum MMP_HDMIRX_PAR_TAG
{
    HDMIRX_PAR_4_3                  = IT6607_PAR_4_3                ,
    HDMIRX_PAR_16_9                 = IT6607_PAR_16_9               ,

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
typedef struct MMP_IT6607_DETIMING_TAG
{
    unsigned int          HDES;
    unsigned int          HDEE;
    unsigned int          VDES;
    unsigned int          VDEE;
} MMP_IT6607_DETIMING;

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
void
IT6607Initialize(
    void);

//=============================================================================
/**
 * Device Output Pin Tri-State.
 */
//=============================================================================
void
IT6607OutputPinTriState(
    BOOL flag);

//=============================================================================
/**
* Device Signal State
*/
//=============================================================================
BOOL
IT6607IsSignalStable(
    void);

//=============================================================================
/**
* Device property.
*/
//=============================================================================
unsigned int
IT6607GetProperty(
    MMP_IT6607_PROPERTY    property);

//=============================================================================
/**
* * Device set property.
*/
//=============================================================================
void
IT6607SetProperty(
    MMP_IT6607_PROPERTY property, unsigned int value);

//=============================================================================
/**
* * Check HDCP.
*/
//=============================================================================
BOOL
IT6607IsHDCPOn(
    void);

//=============================================================================
/**
* * Get DE Timing.
*/
//=============================================================================
void
IT6607GetDETiming(
    MMP_IT6607_DETIMING *pDETiming);

//=============================================================================
/**
* Device power down
*/
//=============================================================================
BOOL
IT6607PowerDown(
    BOOL enable);

//=============================================================================
/**
* HDCP ON/OFF
*/
//=============================================================================
void
IT6607DisableHDCP(
    BOOL flag);

BOOL
IT6607IsDisableHDCP(
    void);

BOOL
IT6607IsHDCPKeyEmpty(
    void);

#ifdef __cplusplus
}
#endif

#endif


