#ifndef __CAPTURE_MODULE_H__
#define __CAPTURE_MODULE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ite/ith.h"
//=============================================================================
//                              Constant Definition
//=============================================================================


//=============================================================================
//                              Constant Definition
//=============================================================================


//=============================================================================
//                              Structure Definition
//=============================================================================
#if 0
typedef struct CAP_GET_PROPERTY_TAG
{
    uint16_t  GetTopFieldPolarity;
    uint16_t  GetHeight;
    uint16_t  GetWidth;
    uint16_t  Rate;
    uint16_t  GetModuleIsInterlace;
    uint16_t  FrameRate;
    uint16_t  matchResolution;
    uint16_t  HPolarity : 1;
    uint16_t  VPolarity : 1;
    uint16_t  HStar;
    uint16_t  HEnd;
    uint16_t  VStar1;
    uint16_t  VEnd1;
    uint16_t  VStar2;
    uint16_t  VEnd2;
} CAP_GET_PROPERTY;
#endif

typedef enum MODULE_GETPROPERTY_TAG
{
    GetTopFieldPolarity = 0,
    GetHeight,
    GetWidth,
    Rate,
    GetModuleIsInterlace,
    matchResolution,
    HPolarity,
    VPolarity,
    HStar,
    HEnd,
    VStar1,
    VEnd1,
    VStar2,
    VEnd2,
}MODULE_GETPROPERTY;

typedef enum MODULE_SETPROPERTY_TAG
{
	Cat9883Reinit = 0,
	Customdefine = 1,	
}MODULE_SETPROPERTY;

typedef enum MODULE_GETSTATUS_TAG
{
	IsPowerDown =0,
	IsSVideoInput,  //ADV7180
	IsTVMODE,       //CAT9883
	IsNTSCorPAL,    //CAT9883

}MODULE_GETSTATUS;


typedef struct CAP_TIMINFO_TABLE_TAG
{
    uint16_t Index;
    uint16_t HActive;
    uint16_t VActive;
    uint16_t Rate;
    uint16_t FrameRate;
    uint16_t HPolarity : 1;
    uint16_t VPolarity : 1;
    uint16_t HStar;
    uint16_t HEnd;
    uint16_t VStar1;
    uint16_t VEnd1;
    uint16_t VStar2;
    uint16_t VEnd2;
} CAP_TIMINFO_TABLE;

typedef enum CAPTURE_MODULE_GET_PROPERTY_TAG
{
    CAPTURE_MODULE_PROPRRTY_HEIGHT,
    CAPTURE_MODULE_PROPERTY_WIDTH,
    CAPTURE_MODULE_PROPERTY_FRAMERATE,
    CAPTURE_MODULE_PROPERTY_IS_INTERLACE,
} CAPTURE_MODULE_GET_PROPERTY;

//Benson add for function pointer.
//LightDriver_t1.h
typedef struct CaptureModuleDriverStruct * CaptureModuleDriver;

typedef struct CaptureModuleDriverInterfaceStruct * CaptureModuleDriverInterface;


typedef struct CaptureModuleDriverStruct
{
    CaptureModuleDriverInterface vtable;
    const char * type;
} CaptureModuleDriverStruct;
//end of LightDriver_t1.h


//LightDriverPrivate_t2.h
typedef struct CaptureModuleDriverInterfaceStruct
{
    void (*Init)(uint16_t Mode);
    void (*Terminate)(void);
    void (*OutputPinTriState)(unsigned char flag);
    unsigned char (*IsSignalStable)(uint16_t Mode);
    uint16_t(*GetProperty)(MODULE_GETPROPERTY Property);
	bool (*GetStatus)(MODULE_GETSTATUS Status);
	void(*SetProperty)(MODULE_SETPROPERTY Property, uint16_t Value);
    void (*PowerDown)(unsigned char enable);
    void (*Destroy)(CaptureModuleDriver);
} CaptureModuleDriverInterfaceStruct;
//end of LightDriverPrivate_t2.h

//LightDriver_t3.h
CaptureModuleDriver ithModule_Create(unsigned char* moduleName);
unsigned char ithModule_IsSignalStable(CaptureModuleDriver, uint16_t Mode);
void ithModule_Init(CaptureModuleDriver, uint16_t Mode);
void ithModule_Destroy(CaptureModuleDriver);
void ithModule_DeInit(CaptureModuleDriver);
uint16_t ithModule_GetProperty(CaptureModuleDriver , MODULE_GETPROPERTY Property);
bool ithModule_GetStatus(CaptureModuleDriver , MODULE_GETSTATUS Status);
void ithModule_SetProperty(CaptureModuleDriver , MODULE_SETPROPERTY Property, uint16_t Value);
void ithModule_PowerDown(CaptureModuleDriver , unsigned char enable);
void ithModule_OutputPinTriState(CaptureModuleDriver , unsigned char flag);
//end of LightDriver_t3.h




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

    
#ifdef __cplusplus
}
#endif

#endif


