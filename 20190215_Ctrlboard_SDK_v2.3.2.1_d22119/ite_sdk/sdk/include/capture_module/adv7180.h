#ifndef __ADV7180_H__
#define __ADV7180_H__

#include "ite/itp.h"
#include "ite/ith_defs.h"
#include "ite/ith.h"
#include "capture_module.h"

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

typedef enum _ADV7180_INPUT_MODE
{
    ADV7180_INPUT_CVBS   = 0,
    ADV7180_INPUT_SVIDEO = 1,
    ADV7180_INPUT_YPBPR  = 2,
} ADV7180_INPUT_MODE;

typedef enum _ADV7180_INPUT_STANDARD
{
    ADV7180_NTSC_M_J          = 0x0,
    ADV7180_NTSC_4_43         = 0x1,
    ADV7180_PAL_M             = 0x2,
    ADV7180_PAL_60            = 0x3,
    ADV7180_PAL_B_G_H_I_D     = 0x4,
    ADV7180_SECAM             = 0x5,
    ADV7180_PAL_COMBINATION_N = 0x6,
    ADV7180_SECAM_525         = 0x7,
} ADV7180_INPUT_STANDARD;


uint16_t ADV7180_InWidth;
uint16_t ADV7180_InHeight;
uint16_t ADV7180_InFrameRate;

void Set_ADV7180_Tri_State_Enable();

void Set_ADV7180_Tri_State_Disable();

ADV7180_INPUT_STANDARD Get_Auto_Detection_Result();

void ADV7180Initial(ADV7180_INPUT_MODE mode);

void ADV7180_Input_Mode(ADV7180_INPUT_MODE mode);

bool ADV7180_IsStable();

void ADV7180_PowerDown(bool enable);

bool ADV7180_IsSVideoInput();

bool ADV7180_IsPowerDown();

//X10LightDriver_t1.h
typedef struct ADV7180CaptureModuleDriverStruct *ADV7180CaptureModuleDriver;
CaptureModuleDriver ADV7180CaptureModuleDriver_Create();
static void ADV7180CaptureModuleDriver_Destory(CaptureModuleDriver base);
void ADV7180Initialize(uint16_t Mode);
void ADV7180Terminate(void);
void ADV7180OutputPinTriState(unsigned char flag);
uint16_t ADV7180GetProperty(MODULE_GETPROPERTY property);
bool ADV7180GetStatus(MODULE_GETSTATUS Status);
void ADV7180PowerDown(unsigned char enable);
unsigned char ADV7180IsSignalStable(uint16_t Mode);
//end of X10LightDriver_t1.h

#ifdef __cplusplus
}
#endif

#endif