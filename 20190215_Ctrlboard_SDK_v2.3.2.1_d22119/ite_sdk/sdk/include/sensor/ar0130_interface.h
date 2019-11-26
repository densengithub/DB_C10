#include "sensor/mmp_sensor.h"
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
void AR0130_Regcallback(sensorcontroler* ptr);
void AR0130_Initialize(void);
void AR0130_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on);
void AR0130_SetEffectDefault(void);
void AR0130_GetEffectDefault(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness);
void AR0130_SetMirror(MMP_BOOL bEnHorMirror,MMP_BOOL bEnVerMirror);
void AR0130_SetFlickMode(MMP_SENSOR_FLICK_MODE mode);
MMP_SENSOR_FLICK_MODE AR0130_GetFlickMode(void);
void AR0130_SetDefault(MMP_UINT8 value);
void AR0130_GetDefault(MMP_UINT8 *value);




