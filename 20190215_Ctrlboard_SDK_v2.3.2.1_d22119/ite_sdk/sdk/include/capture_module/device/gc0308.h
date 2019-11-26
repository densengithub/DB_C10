#ifndef __GC0308_H__
#define __GC0308_H__

#include "ite/itp.h"
#include "ite/ith_defs.h"
#include "ite/ith.h"

#include "encoder/encoder_types.h"
#include "capture_module.h" //Benson

#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 */
//#define	bool	unsigned char

#ifndef MMP_TRUE
    #define MMP_TRUE  1
#endif
#ifndef MMP_FALSE
    #define MMP_FALSE 0
#endif

#define COMPOSITE_DEV //Benson

typedef enum CAP_FRAMERATE_TAG
{
    CAP_FRAMERATE_UNKNOW = 0,
    CAP_FRAMERATE_10HZ,
} CAP_FRAMERATE;

uint16_t GC0308_InWidth;
uint16_t GC0308_InHeight;
uint16_t GC0308_InFrameRate;

void Set_GC0308_Tri_State_Enable();

void Set_GC0308_Tri_State_Disable();

void GC0308Initial();

bool GC0308_IsStable();

void GC0308_PowerDown(
    bool enable);

//X10LightDriver_t1.h
typedef struct GC0308CaptureModuleDriverStruct *GC0308CaptureModuleDriver;
CaptureModuleDriver GC0308CaptureModuleDriver_Create();
static void GC0308CaptureModuleDriver_Destory(CaptureModuleDriver base);
void GC0308Initialize(uint16_t Mode);
void GC0308Terminate(void);
void GC0308OutputPinTriState(unsigned char flag);
uint16_t GC0308GetProperty(MODULE_GETPROPERTY property);
bool GC0308GetStatus(MODULE_GETSTATUS Status);
void GC0308SetProperty(MODULE_SETPROPERTY Property, uint16_t Value);
void GC0308PowerDown(unsigned char enable);
void GC0308ForCaptureDriverSetting(CAP_CONTEXT *Capctxt );
unsigned char GC0308IsSignalStable(uint16_t Mode);
//end of X10LightDriver_t1.h

#ifdef __cplusplus
}
#endif

#endif