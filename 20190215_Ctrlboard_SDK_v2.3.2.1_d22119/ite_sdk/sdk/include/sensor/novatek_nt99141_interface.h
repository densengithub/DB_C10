#ifndef NOVATEK_NT99141_INTERFACE_H
#define NOVATEK_NT99141_INTERFACE_H

#include "sensor/mmp_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

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

//=============================================================================
//                Public Function Definition
//=============================================================================
void NovatekNT99141_Regcallback(sensorcontroler *ptr);
void NovatekNT99141_Initialize(void);
void NovatekNT99141_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on);
void NovatekNT99141_SetEffectDefault(void);
void NovatekNT99141_GetEffectDefault(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness);
void NovatekNT99141_SetMirror(MMP_BOOL bEnHorMirror, MMP_BOOL bEnVerMirror);
void NovatekNT99141_SetFlickMode(MMP_SENSOR_FLICK_MODE mode);
MMP_SENSOR_FLICK_MODE NovatekNT99141_GetFlickMode(void);
void NovatekNT99141_SetDefault(MMP_UINT8 value);
void NovatekNT99141_GetDefault(MMP_UINT8 *value);

#ifdef __cplusplus
}
#endif

#endif