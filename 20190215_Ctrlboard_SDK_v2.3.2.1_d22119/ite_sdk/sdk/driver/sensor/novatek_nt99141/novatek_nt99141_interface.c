#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "sensor/novatek_nt99141_io.h"
#include "sensor/novatek_nt99141_driver.h"
#include "sensor/novatek_nt99141_interface.h"

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
void NovatekNT99141_Regcallback(sensorcontroler* ptr)
{
	ptr->SensorInitialize = NovatekNT99141_Initialize;
	ptr->SensorPowerOn = NovatekNT99141_PowerOn;
	ptr->SensorSetEffectDefault = NovatekNT99141_SetEffectDefault;
	ptr->SensorGetEffectDefault = NovatekNT99141_GetEffectDefault;
	ptr->SensorImageMirror = NovatekNT99141_SetMirror;
	ptr->SensorSetBrightness = NovatekNT99141_SetDefault;
	ptr->SensorGetBrightness = NovatekNT99141_GetDefault;
	ptr->SensorSetContrast = NovatekNT99141_SetDefault;
	ptr->SensorGetContrast = NovatekNT99141_GetDefault;
	ptr->SensorSetSaturation = NovatekNT99141_SetDefault;
	ptr->SensorGetSaturation = NovatekNT99141_GetDefault;
	ptr->SensorSetSharpness = NovatekNT99141_SetDefault;
	ptr->SensorGetSharpness = NovatekNT99141_GetDefault;
	ptr->SensorSetFlickMode = NovatekNT99141_SetFlickMode;
	ptr->SensorGetFlickMode = NovatekNT99141_GetFlickMode;
}

void NovatekNT99141_Initialize(void){}

void NovatekNT99141_PowerOn(MMP_BOOL bEnable, MMP_BOOL dummy_on)
{
	if (bEnable)
    {
   		ithNovaTekPowerOn();

#if !defined(CFG_DOORBELL_LOBBY)		
        if (dummy_on == MMP_FALSE)
            ithNovaTekLedOn();
#endif

#if defined(CFG_SENSOR_FLICK_60HZ)
        ithNovaTekSetAntiFlicker60Hz();
#elif defined(CFG_SENSOR_FLICK_50HZ)
        ithNovaTekSetAntiFlicker50Hz();
#endif
	}
	else
	{
		ithNovaTekPowerOff();
#if !defined(CFG_DOORBELL_LOBBY)
        if (dummy_on == MMP_FALSE)
            ithNovaTekLedOff();
#endif	
	}
}

void NovatekNT99141_SetEffectDefault(void){}

void NovatekNT99141_GetEffectDefault(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness){}

void NovatekNT99141_SetMirror(MMP_BOOL bEnHorMirror, MMP_BOOL bEnVerMirror){}

void NovatekNT99141_SetFlickMode(MMP_SENSOR_FLICK_MODE mode)
{
    switch (mode)
    {
	    case SENSOR_FLICK_MODE_50HZ:     // 50Hz
	        ithNovaTekSetAntiFlicker50Hz();
	        break;
	    case SENSOR_FLICK_MODE_60HZ:     // 60Hz
	        ithNovaTekSetAntiFlicker60Hz();
	        break;
	    case SENSOR_FLICK_MODE_AUTO:    // auto
	        break;
	    case SENSOR_FLICK_MODE_OFF:     // off
	    default:
	        break;
    }
}

MMP_SENSOR_FLICK_MODE NovatekNT99141_GetFlickMode(void)
{
	MMP_SENSOR_FLICK_MODE   mode = SENSOR_FLICK_MODE_OFF;
	return mode;
}

void NovatekNT99141_SetDefault(MMP_UINT8 value){}

void NovatekNT99141_GetDefault(MMP_UINT8 *value){}

