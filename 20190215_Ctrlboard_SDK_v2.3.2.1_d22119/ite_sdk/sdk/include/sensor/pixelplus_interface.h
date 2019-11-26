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
void Pixelplus_Regcallback(sensorcontroler* ptr);
void Pixelplus_Initialize(void);
void Pixelplus_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on);
void Pixelplus_SetEffectDefault(void);
void Pixelplus_GetEffectDefault(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness);
void Pixelplus_ImageMirror(MMP_BOOL bEnHorMirror,MMP_BOOL bEnVerMirror);
void Pixelplus_SetFlickMode(MMP_SENSOR_FLICK_MODE mode);
MMP_SENSOR_FLICK_MODE Pixelplus_GetFlickMode(void);


