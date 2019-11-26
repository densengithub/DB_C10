#ifndef __MMP_SENSOR_H__
#define __MMP_SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "encoder/encoder_types.h"

//=============================================================================
//                              Constant Definition
//=============================================================================
#define SENSOR_API extern

//=============================================================================
//                              Constant Definition
//=============================================================================
typedef enum MMP_SENSOR_FLICK_MODE_TAG
{
    SENSOR_FLICK_MODE_AUTO,
    SENSOR_FLICK_MODE_50HZ,
    SENSOR_FLICK_MODE_60HZ,
    SENSOR_FLICK_MODE_OFF
} MMP_SENSOR_FLICK_MODE;

//=============================================================================
//                              Structure Definition
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

typedef struct sensorcontroler
{
    void (*SensorInitialize)(void);
    void (*SensorPowerOn)(MMP_BOOL bEnable, MMP_BOOL dummy_on);
    void (*SensorSetEffectDefault)(void);
    void (*SensorGetEffectDefault)(MMP_UINT8 *brightness, MMP_UINT8 *contrast, MMP_UINT8 *saturation, MMP_UINT8 *sharpness);
    void (*SensorImageMirror)(MMP_BOOL bEnHorMirror, MMP_BOOL bEnVerMirror);
    void (*SensorSetBrightness)(MMP_UINT8 value);
    void (*SensorGetBrightness)(MMP_UINT8 *value);
    void (*SensorSetContrast)(MMP_UINT8 value);
    void (*SensorGetContrast)(MMP_UINT8 *value);
    void (*SensorSetSaturation)(MMP_UINT8 value);
    void (*SensorGetSaturation)(MMP_UINT8 *value);
    void (*SensorSetSharpness)(MMP_UINT8 value);
    void (*SensorGetSharpness)(MMP_UINT8 *value);
    void (*SensorSetFlickMode)(MMP_SENSOR_FLICK_MODE mode);
    MMP_SENSOR_FLICK_MODE (*SensorGetFlickMode)(void);
} sensorcontroler;

//=============================================================================
/**
 * Regsister Sensor call back function
 */
//=============================================================================
void
ithSensorRegsisterCallback(
    void);

//=============================================================================
/**
 * Device Initialize
 */
//=============================================================================
SENSOR_API void
ithSensorInitialize(
    void);

void
ithSensorPowerOn(
    MMP_BOOL bEnable,
    MMP_BOOL dummy_on);

void
ithSensorSetEffectDefault(
    void);

void
ithSensorGetEffectDefault(
    MMP_UINT8 *brightness,
    MMP_UINT8 *contrast,
    MMP_UINT8 *saturation,
    MMP_UINT8 *sharpness);

void
ithSensorImageMirror(
    MMP_BOOL bEnHorMirror,
    MMP_BOOL bEnVerMirror);

void
ithSensorSetBrightness(
    MMP_UINT8 value);

void
ithSensorGetBrightness(
    MMP_UINT8 *value);

void
ithSensorSetContrast(
    MMP_UINT8 value);

void
ithSensorGetContrast(
    MMP_UINT8 *value);

void
ithSensorSetSaturation(
    MMP_UINT8 value);

void
ithSensorGetSaturation(
    MMP_UINT8 *value);

void
ithSensorSetSharpness(
    MMP_UINT8 value);

void
ithSensorGetSharpness(
    MMP_UINT8 *value);

void
ithSensorSetFlickMode(
    MMP_SENSOR_FLICK_MODE mode);

MMP_SENSOR_FLICK_MODE
ithSensorGetFlickMode(
    void);
#ifdef __cplusplus
}
#endif

#endif
