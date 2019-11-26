#include "sensor/mmp_sensor.h"
#include "sensor/omnivision_interface.h"
#include "sensor/pixelplus_interface.h"
#include "sensor/novatek_nt99141_interface.h"
#include "sensor/ar0130_interface.h"

//=============================================================================
//                Constant Definition
//=============================================================================
#define SENSOR_CONFIG_NOR_SIZE     64 * 1024
#define SENSOR_CONFIG_NOR_ADDR     ((1 * 1024 * 1024) - (SENSOR_CONFIG_NOR_SIZE) -(128 * 1024))

#define SENSOR_CONFIG_HEADER_SIZE  4
#define SENSOR_CONFIG_PAYLOAD_SIZE SENSOR_CONFIG_NOR_SIZE - SENSOR_CONFIG_HEADER_SIZE

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================
sensorcontroler s_handle;
//=============================================================================
//                Private Function Definition
//=============================================================================
//=============================================================================
//                Public Function Definition
//=============================================================================
//=============================================================================
/**
 * Regsister Sensor call back function
 */
//=============================================================================

void
ithSensorRegsisterCallback(void)
{
#if defined(SENSOR_PIXELPLUS_PO3100)
    Pixelplus_Regcallback(&s_handle);
#elif defined(SENSOR_NOVATEK_NT99141)
    NovatekNT99141_Regcallback(&s_handle);
#elif defined(SENSOR_OMNIVISION_OV7725)
    Omnivision_Regcallback(&s_handle);
#elif defined(SENSOR_AR0130)
    AR0130_Regcallback(&s_handle);
#endif
}

//=============================================================================
/**
 * Sensor Initialize
 */
//=============================================================================
void
ithSensorInitialize(
    void)
{
    if (s_handle.SensorInitialize != NULL)
        s_handle.SensorInitialize();
}

void
ithSensorPowerOn(
    MMP_BOOL bEnable,
    MMP_BOOL dummy_on)
{
    if (s_handle.SensorPowerOn != NULL)
        s_handle.SensorPowerOn(bEnable, dummy_on);
}

void
ithSensorSetEffectDefault(
    void)
{
    if (s_handle.SensorSetEffectDefault != NULL)
        s_handle.SensorSetEffectDefault();
}

void
ithSensorGetEffectDefault(
    MMP_UINT8 *brightness,
    MMP_UINT8 *contrast,
    MMP_UINT8 *saturation,
    MMP_UINT8 *sharpness)
{
    if (s_handle.SensorGetEffectDefault != NULL)
        s_handle.SensorGetEffectDefault(brightness, contrast, saturation, sharpness);
}

void
ithSensorImageMirror(
    MMP_BOOL bEnHorMirror,
    MMP_BOOL bEnVerMirror)
{
    if (s_handle.SensorImageMirror != NULL)
        s_handle.SensorImageMirror(bEnHorMirror, bEnVerMirror);
}

void
ithSensorSetBrightness(
    MMP_UINT8 value)
{
    if (s_handle.SensorSetBrightness != NULL)
        s_handle.SensorSetBrightness(value);
}

void
ithSensorGetBrightness(
    MMP_UINT8 *value)
{
    if (s_handle.SensorGetBrightness != NULL)
        s_handle.SensorGetBrightness(value);
}

void
ithSensorSetContrast(
    MMP_UINT8 value)
{
    if (s_handle.SensorSetContrast != NULL)
        s_handle.SensorSetContrast(value);
}

void
ithSensorGetContrast(
    MMP_UINT8 *value)
{
    if (s_handle.SensorGetContrast != NULL)
        s_handle.SensorGetContrast(value);
}

void
ithSensorSetSaturation(
    MMP_UINT8 value)
{
    if (s_handle.SensorSetSaturation != NULL)
        s_handle.SensorSetSaturation(value);
}

void
ithSensorGetSaturation(
    MMP_UINT8 *value)
{
    if (s_handle.SensorGetSaturation != NULL)
        s_handle.SensorGetSaturation(value);
}

void
ithSensorSetSharpness(
    MMP_UINT8 value)
{
    if (s_handle.SensorSetSharpness != NULL)
        s_handle.SensorSetSharpness(value);
}

void
ithSensorGetSharpness(
    MMP_UINT8 *value)
{
    if (s_handle.SensorGetSharpness != NULL)
        s_handle.SensorGetSharpness(value);
}

MMP_SENSOR_FLICK_MODE
ithSensorGetFlickMode(
    void)
{
    if (s_handle.SensorGetFlickMode != NULL)
        return s_handle.SensorGetFlickMode();
    return -1;
}

void
ithSensorSetFlickMode(
    MMP_SENSOR_FLICK_MODE mode)
{
    if (s_handle.SensorSetFlickMode != NULL)
        s_handle.SensorSetFlickMode(mode);
}