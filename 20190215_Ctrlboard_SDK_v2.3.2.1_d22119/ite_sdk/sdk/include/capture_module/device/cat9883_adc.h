///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <ADC.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2011/08/12
//   @fileversion: CAT9883_V43
//******************************************/
#include "capture_module.h"

#define Reg_Pair_initial(x) Load_Reg_ini(x, (sizeof(x) / sizeof(x[0])))

uint16_t CAT9883_InWidth;
uint16_t CAT9883_InHeight;
uint16_t CAT9883_InFrameRate;
uint16_t CAT9883_InIsInterlace;
uint16_t CAT9883_InIsTVMode;

bool Mode_stable(bool isFrameRateCheck);

void ADC_Mode_Change();

void CAT9883CInitial();
void CAT9883Cmodechange();

void ModeDetectingprocess();
void AutoColorCalibration();

uint16_t Hsync_counter();
uint16_t Vsync_timer();
uint16_t Get_mode_number(bool isFrameRateCheck);
void Set_mode(uint16_t);
bool Is_TV_mode();

uint16_t TV_mode(bool isFrameRateCheck);
uint16_t PC_mode();

uint16_t Frame_rate();

void Set_CAT9883_Tri_State_Enable();
void Set_CAT9883_Tri_State_Disable();

void CAT9883_PowerDown(
    bool enable);

bool CAT9883_IsNTSCorPAL(
    void);

//X10LightDriver_t1.h
typedef struct CAT9883CaptureModuleDriverStruct *CAT9883CaptureModuleDriver;
CaptureModuleDriver CAT9883CaptureModuleDriver_Create();
static void CAT9883CaptureModuleDriver_Destory(CaptureModuleDriver base);
void CAT9883Initialize(uint16_t Mode);
void CAT9883Terminate(void);
void CAT9883OutputPinTriState(unsigned char flag);
uint16_t CAT9883GetProperty(MODULE_GETPROPERTY property);
bool CAT9883GetStatus(MODULE_GETSTATUS Status);
void CAT9883PowerDown(unsigned char enable);
void CAT9883ForCaptureDriverSetting(CAP_CONTEXT *Capctxt );
unsigned char CAT9883IsSignalStable(uint16_t Mode);
//end of X10LightDriver_t1.h

