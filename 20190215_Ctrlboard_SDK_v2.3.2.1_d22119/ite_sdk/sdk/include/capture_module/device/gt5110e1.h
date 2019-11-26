#ifndef __GT5110E1_H__
#define __GT5110E1_H__

#include "ite/itp.h"
#include "ite/ith_defs.h"
#include "ite/ith.h"

#include "encoder/encoder_types.h"
#include "capture_module.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MMP_TRUE
    #define MMP_TRUE  1
#endif
#ifndef MMP_FALSE
    #define MMP_FALSE 0
#endif

typedef enum CAP_FRAMERATE_TAG
{
    CAP_FRAMERATE_UNKNOW = 0,
    CAP_FRAMERATE_30HZ,
} CAP_FRAMERATE;


typedef struct GT5110E1CaptureModuleDriverStruct *GT5110E1CaptureModuleDriver;
CaptureModuleDriver GT5110E1CaptureModuleDriver_Create();
static void GT5110E1CaptureModuleDriver_Destory(CaptureModuleDriver base);
void GT5110E1Initialize(uint16_t Mode);
void GT5110E1Terminate(void);
void GT5110E1OutputPinTriState(unsigned char flag);
uint16_t GT5110E1GetProperty(MODULE_GETPROPERTY property);
bool GT5110E1GetStatus(MODULE_GETSTATUS Status);
void GT5110E1PowerDown(unsigned char enable);
void GT5110E1ForCaptureDriverSetting(CAP_CONTEXT *Capctxt );
unsigned char GT5110E1IsSignalStable(uint16_t Mode);

#ifdef __cplusplus
}
#endif

#endif