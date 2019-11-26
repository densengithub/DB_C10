#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "sensor/Omnivision_io.h"
#include "sensor/Omnivision_driver.h"
#include "sensor/Omnivision_interface.h"

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
void Omnivision_Regcallback(sensorcontroler* ptr)
{
	ptr->SensorInitialize = Omnivision_Initialize;
	ptr->SensorPowerOn = Omnivision_PowerOn;
	ptr->SensorSetEffectDefault = Omnivision_SetEffectDefault;
	ptr->SensorGetEffectDefault = Omnivision_GetEffectDefault;
	ptr->SensorImageMirror = Omnivision_SetMirror;
	ptr->SensorSetBrightness = Omnivision_SetDefault;
	ptr->SensorGetBrightness = Omnivision_GetDefault;
	ptr->SensorSetContrast = Omnivision_SetDefault;
	ptr->SensorGetContrast = Omnivision_GetDefault;
	ptr->SensorSetSaturation = Omnivision_SetDefault;
	ptr->SensorGetSaturation = Omnivision_GetDefault;
	ptr->SensorSetSharpness = Omnivision_SetDefault;
	ptr->SensorGetSharpness = Omnivision_GetDefault;
	ptr->SensorSetFlickMode = Omnivision_SetFlickMode;
	ptr->SensorGetFlickMode = Omnivision_GetFlickMode;
}

void Omnivision_Initialize(void){}

void Omnivision_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on)
{
	if (bEnable)
    {
   		ithOmnivisionPowerOn();
		
		if (dummy_on == MMP_FALSE)
            ithOmnivisionLedOn();

		#if defined(CFG_SENSOR_FLICK_60HZ)
        	ithOmnivisionSetAntiFlicker60Hz();
    	#elif defined(CFG_SENSOR_FLICK_50HZ)
        	ithOmnivisionSetAntiFlicker50Hz();
    	#endif
	}
	else
	{
		ithOmnivisionPowerOff();
		if (dummy_on == MMP_FALSE)
			ithOmnivisionLedOff();
	}
}

void Omnivision_SetEffectDefault(void){}

void Omnivision_GetEffectDefault(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness){}

void Omnivision_SetMirror(MMP_BOOL bEnHorMirror, MMP_BOOL bEnVerMirror){}

void Omnivision_SetFlickMode(MMP_SENSOR_FLICK_MODE mode)
{
    switch (mode)
    {
	    case SENSOR_FLICK_MODE_50HZ:     // 50Hz
	        ithOmnivisionSetAntiFlicker50Hz();
	        break;
	    case SENSOR_FLICK_MODE_60HZ:     // 60Hz
	        ithOmnivisionSetAntiFlicker60Hz();
	        break;
	    case SENSOR_FLICK_MODE_AUTO:    // auto
	        break;
	    case SENSOR_FLICK_MODE_OFF:     // off
	    default:
	        break;
    }
}

MMP_SENSOR_FLICK_MODE Omnivision_GetFlickMode(void)
{
	MMP_SENSOR_FLICK_MODE   mode = SENSOR_FLICK_MODE_OFF;
	return mode;
}

void Omnivision_SetDefault(MMP_UINT8 value){}

void Omnivision_GetDefault(MMP_UINT8 *value){}

