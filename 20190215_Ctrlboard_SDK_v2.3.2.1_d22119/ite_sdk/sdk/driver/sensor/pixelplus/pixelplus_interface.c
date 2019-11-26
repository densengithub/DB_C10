#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "sensor/pixelplus_io.h"
#include "sensor/pixelplus_driver.h"
#include "sensor/pixelplus_interface.h"

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
void Pixelplus_Regcallback(sensorcontroler* ptr)
{
	ptr->SensorInitialize = Pixelplus_Initialize;
	ptr->SensorPowerOn = Pixelplus_PowerOn;
	ptr->SensorSetEffectDefault = ithPixelPlusSetEffectDefault;
	ptr->SensorGetEffectDefault = ithPixelPlusGetEffectDefault;
	ptr->SensorImageMirror = ithPixelPlusSetMirror;
	ptr->SensorSetBrightness = ithPixelPlusSetBrightness;
	ptr->SensorGetBrightness = ithPixelPlusGetBrightness;
	ptr->SensorSetContrast = ithPixelPlusSetContrast;
	ptr->SensorGetContrast = ithPixelPlusGetContrast;
	ptr->SensorSetSaturation = ithPixelPlusSetSaturation;
	ptr->SensorGetSaturation = ithPixelPlusGetSaturation;
	ptr->SensorSetSharpness = ithPixelPlusSetSharpness;
	ptr->SensorGetSharpness = ithPixelPlusGetSharpness;
	ptr->SensorSetFlickMode = Pixelplus_SetFlickMode;
	ptr->SensorGetFlickMode = Pixelplus_GetFlickMode;
}

void Pixelplus_Initialize(void)
{
	
    ithPixelPlusPowerOn();
    ithPixelPlusInitialize();
	
	#if defined(CFG_SENSOR_FLICK_60HZ)
		ithPixelPlusSetAntiFlicker60Hz();
	#elif defined(CFG_SENSOR_FLICK_50HZ)
		ithPixelPlusSetAntiFlicker50Hz();
	#endif
	
	ithPixelPlusSetMirror(MMP_TRUE, MMP_TRUE);
    //ithPixelPlusPowerOff();
}

void Pixelplus_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on)
{
	if (bEnable)
    {
   		ithPixelPlusPowerOn();
		if (dummy_on == MMP_FALSE)
            ithPixelPlusLedOn();
	}
	else
	{
		ithPixelPlusPowerOff();
		if (dummy_on == MMP_FALSE)
			ithPixelPlusLedOff();
	}
}

void Pixelplus_SetFlickMode(MMP_SENSOR_FLICK_MODE mode)
{
    switch (mode)
    {
	    case SENSOR_FLICK_MODE_50HZ:     // 50Hz
	        ithPixelPlusSetAntiFlicker50Hz();
	        break;
	    case SENSOR_FLICK_MODE_60HZ:     // 60Hz
	        ithPixelPlusSetAntiFlicker60Hz();
	        break;
	    case SENSOR_FLICK_MODE_AUTO:     // auto
	        ithPixelPlusSetAntiFlickerAuto();
	        break;
	    case SENSOR_FLICK_MODE_OFF:     // off
	    default:
	        ithPixelPlusSetAntiFlickerOff();
	        break;
    }
}

MMP_SENSOR_FLICK_MODE Pixelplus_GetFlickMode(void)
{
	MMP_SENSOR_FLICK_MODE   mode = SENSOR_FLICK_MODE_OFF;
	MMP_UINT8               value;
    ithPixelPlusGetFlickerMode(&value);
    switch (value)
    {
	    case 0x04:     // 50Hz
	        mode    = SENSOR_FLICK_MODE_50HZ;
	        break;
	    case 0x08:     // 60Hz
	        mode    = SENSOR_FLICK_MODE_60HZ;
	        break;
	    case 0x40:     // auto
	        mode    = SENSOR_FLICK_MODE_AUTO;
	        break;
	    case 0x0:     // off
	    default:
	        mode = SENSOR_FLICK_MODE_OFF;
	        break;
    }
	return mode;
}
