#ifndef __MMP_IT6604_H__
#define __MMP_IT6604_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                              Constant Definition
//=============================================================================

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

typedef enum MMP_IT6604_PROPERTY_TAG
{
    IT6604_IS_DVI_MODE,
    IT6604_CHECK_ENGINE_IDLE,
    IT6604_ASPECT_RATIO,
    IT6604_OUTPUT_VIDEO_MODE,
    IT6604_PCLK,
    IT6604_HEIGHT,
    IT6604_WIDTH,
    IT6604_HTOTAL,
    IT6604_VTOTAL,
    IT6604_COLOR_DEPTH,
    IT6604_IS_INTERLACE,
    IT6604_IS_VID_STABLE,
    IT6604_IS_AUDIO_ON,
    IT6604_IS_AUDIO_RESET,
    IT6604_AUDIO_SAMPLERATE,
    IT6604_AUDIO_CHANNEL_NUMBER,
    IT6604_AUDIO_MODE,
    IT6604_CHECK_PLG5V_PWR,
} MMP_IT6604_PROPERTY;

typedef enum MMP_IT6604_AUDIO_MODE_TAG
{
    IT6604_AUDIO_OFF,
    IT6604_AUDIO_HBR,
    IT6604_AUDIO_DSD,
    IT6604_AUDIO_NLPCM,
    IT6604_AUDIO_LPCM,
} MMP_IT6604_AUDIO_MODE;

typedef enum MMP_IT6604_PAR_TAG
{
    IT6604_PAR_4_3  = 1,
    IT6604_PAR_16_9 = 2,
} MMP_IT6604_PAR;

typedef enum MMP_HDMIRX_PROPERTY_TAG
{
    HDMIRX_IS_DVI_MODE              = IT6604_IS_DVI_MODE            ,
    HDMIRX_CHECK_ENGINE_IDLE        = IT6604_CHECK_ENGINE_IDLE      ,
    HDMIRX_ASPECT_RATIO             = IT6604_ASPECT_RATIO           ,
    HDMIRX_OUTPUT_VIDEO_MODE        = IT6604_OUTPUT_VIDEO_MODE      ,
    HDMIRX_PCLK                     = IT6604_PCLK                   ,
    HDMIRX_HEIGHT                   = IT6604_HEIGHT                 ,
    HDMIRX_WIDTH                    = IT6604_WIDTH                  ,
    HDMIRX_HTOTAL                   = IT6604_HTOTAL                 ,
    HDMIRX_VTOTAL                   = IT6604_VTOTAL                 ,
    HDMIRX_COLOR_DEPTH              = IT6604_COLOR_DEPTH            ,
    HDMIRX_IS_INTERLACE             = IT6604_IS_INTERLACE           ,
    HDMIRX_IS_VID_STABLE            = IT6604_IS_VID_STABLE          ,
    HDMIRX_IS_AUDIO_ON              = IT6604_IS_AUDIO_ON            ,
    HDMIRX_IS_AUDIO_RESET           = IT6604_IS_AUDIO_RESET         ,
    HDMIRX_AUDIO_SAMPLERATE         = IT6604_AUDIO_SAMPLERATE       ,
    HDMIRX_AUDIO_CHANNEL_NUMBER     = IT6604_AUDIO_CHANNEL_NUMBER   ,
    HDMIRX_AUDIO_MODE               = IT6604_AUDIO_MODE             ,
    HDMIRX_CHECK_PLG5V_PWR          = IT6604_CHECK_PLG5V_PWR        ,

} MMP_HDMIRX_PROPERTY;

typedef enum MMP_HDMIRX_AUDIO_MODE_TAG
{
    HDMIRX_AUDIO_OFF                = IT6604_AUDIO_OFF              ,
    HDMIRX_AUDIO_HBR                = IT6604_AUDIO_HBR              ,
    HDMIRX_AUDIO_DSD                = IT6604_AUDIO_DSD              ,
    HDMIRX_AUDIO_NLPCM              = IT6604_AUDIO_NLPCM            ,
    HDMIRX_AUDIO_LPCM               = IT6604_AUDIO_LPCM             ,

} MMP_HDMIRX_AUDIO_MODE;

typedef enum MMP_HDMIRX_PAR_TAG
{
    HDMIRX_PAR_4_3                  = IT6604_PAR_4_3                ,
    HDMIRX_PAR_16_9                 = IT6604_PAR_16_9               ,

} MMP_HDMIRX_PAR;
//=============================================================================
//                              Structure Definition
//=============================================================================
typedef struct MMP_IT6604_DETIMING_TAG
{
    unsigned int HDES;
    unsigned int HDEE;
    unsigned int VDES;
    unsigned int VDEE;
} MMP_IT6604_DETIMING;

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

//=============================================================================
//                Public Function Definition
//=============================================================================

//=============================================================================
/**
 * Device Initialize
 */
//=============================================================================
void
mmpIT6604Initialize(
    void);

//=============================================================================
/**
 * Device Output Pin Tri-State.
 */
//=============================================================================
void
mmpIT6604OutputPinTriState(
    BOOL flag);

//=============================================================================
/**
 * Device Signal State
 */
//=============================================================================
BOOL
mmpIT6604IsSignalStable(
    void);

//=============================================================================
/**
 * Device property.
 */
//=============================================================================
unsigned int
mmpIT6604GetProperty(
    MMP_IT6604_PROPERTY property);

//=============================================================================
/**
 * * Device set property.
 */
//=============================================================================
void
mmpIT6604SetProperty(
    MMP_IT6604_PROPERTY property, unsigned int value);

//=============================================================================
/**
 * * Check HDCP.
 */
//=============================================================================
BOOL
mmpIT6604IsHDCPOn(
    void);

//=============================================================================
/**
 * * Get DE Timing.
 */
//=============================================================================
void
mmpIT6604GetDETiming(
    MMP_IT6604_DETIMING *pDETiming);

//=============================================================================
/**
 * Device power down
 */
//=============================================================================
BOOL
mmpIT6604PowerDown(
    BOOL enable);

//=============================================================================
/**
 * HDCP ON/OFF
 */
//=============================================================================
void
mmpIT6604DisableHDCP(
    BOOL flag);

#ifdef __cplusplus
}
#endif

#endif