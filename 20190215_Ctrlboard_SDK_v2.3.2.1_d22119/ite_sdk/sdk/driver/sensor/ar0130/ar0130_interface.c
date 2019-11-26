#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "sensor/AR0130_driver.h"
#include "sensor/AR0130_interface.h"

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
void AR0130_Regcallback(sensorcontroler* ptr)
{
	ptr->SensorInitialize = AR0130_Initialize;
	ptr->SensorPowerOn = AR0130_PowerOn;
	ptr->SensorSetEffectDefault = AR0130_SetEffectDefault;
	ptr->SensorGetEffectDefault = AR0130_GetEffectDefault;
	ptr->SensorImageMirror = AR0130_SetMirror;
	ptr->SensorSetBrightness = AR0130_SetDefault;
	ptr->SensorGetBrightness = AR0130_GetDefault;
	ptr->SensorSetContrast = AR0130_SetDefault;
	ptr->SensorGetContrast = AR0130_GetDefault;
	ptr->SensorSetSaturation = AR0130_SetDefault;
	ptr->SensorGetSaturation = AR0130_GetDefault;
	ptr->SensorSetSharpness = AR0130_SetDefault;
	ptr->SensorGetSharpness = AR0130_GetDefault;
	ptr->SensorSetFlickMode = AR0130_SetFlickMode;
	ptr->SensorGetFlickMode = AR0130_GetFlickMode;
}

void AR0130_Initialize(void)
{
    ithAR0130PowerOn();
    ithAR0130Initialize();
}

void AR0130_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on)
{
	if (bEnable)
    {
   		ithAR0130PowerOn();
		
		if (dummy_on == MMP_FALSE)
            ithAR0130LedOn();
	}
	else
	{
		ithAR0130PowerOff();
		if (dummy_on == MMP_FALSE)
			ithAR0130LedOff();
	}
}

void AR0130_SetEffectDefault(void){}

void AR0130_GetEffectDefault(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness){}

void AR0130_SetMirror(MMP_BOOL bEnHorMirror, MMP_BOOL bEnVerMirror){}

void AR0130_SetFlickMode(MMP_SENSOR_FLICK_MODE mode){}

MMP_SENSOR_FLICK_MODE AR0130_GetFlickMode(void)
{
	MMP_SENSOR_FLICK_MODE   mode = SENSOR_FLICK_MODE_OFF;
	return mode;
}

void AR0130_SetDefault(MMP_UINT8 value){}

void AR0130_GetDefault(MMP_UINT8 *value){}

