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
void Omnivision_Regcallback(sensorcontroler* ptr);
void Omnivision_Initialize(void);
void Omnivision_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on);
void Omnivision_SetEffectDefault(void);
void Omnivision_GetEffectDefault(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness);
void Omnivision_SetMirror(MMP_BOOL bEnHorMirror,MMP_BOOL bEnVerMirror);
void Omnivision_SetFlickMode(MMP_SENSOR_FLICK_MODE mode);
MMP_SENSOR_FLICK_MODE Omnivision_GetFlickMode(void);
void Omnivision_SetDefault(MMP_UINT8 value);
void Omnivision_GetDefault(MMP_UINT8 *value);




