#include "capture_module.h"
#include <string.h>
CaptureModuleDriver ithModule_Create(unsigned char* moduleName)
{
	//module 1
	if(CFG_CAPTURE_MODULE_NAME == "adv7180.c" )
	{
		if(strcmp(moduleName,"adv7180.c") == 0)
			return (CaptureModuleDriver)ADV7180CaptureModuleDriver_Create();
	}
	//if(CFG_CAPTURE_MODULE_NAME == "cat9883_adc.c")
	//{
		//if (strcmp(moduleName,"cat9883_adc.c") == 0)
			//return (CaptureModuleDriver)CAT9883CaptureModuleDriver_Create();
	//}

	//module 2
	#ifdef CFG_SENCOND_CAPTURE_MODULE_ENABLE
	if(CFG_SENCOND_CAPTURE_MODULE_NAME == "adv7180.c")
	{
		if(strcmp(moduleName,"adv7180.c") == 0)
			return (CaptureModuleDriver)ADV7180CaptureModuleDriver_Create();
	}
	//if(CFG_SENCOND_CAPTURE_MODULE_NAME == "cat9883_adc.c")
	//{
		//if (strcmp(moduleName,"cat9883_adc.c") == 0)
			//return (CaptureModuleDriver)CAT9883CaptureModuleDriver_Create();
	//}
	#endif
	
	return NULL;
}

void ithModule_Init(CaptureModuleDriver self, uint16_t Mode)
{
    if (self)
        self->vtable->Init(Mode);
}

void ithModule_Destroy(CaptureModuleDriver self)
{
    if (self)
        self->vtable->Destroy(self);
}

void ithModule_DeInit(CaptureModuleDriver self)
{
    if (self)
        self->vtable->Terminate();
}

unsigned char ithModule_IsSignalStable(CaptureModuleDriver self, uint16_t Mode)
{
    if (self)
        return self->vtable->IsSignalStable(Mode);
}

uint16_t ithModule_GetProperty(CaptureModuleDriver self, MODULE_GETPROPERTY Property)
{
    if (self)
        return self->vtable->GetProperty(Property);
}

bool ithModule_GetStatus(CaptureModuleDriver self, MODULE_GETSTATUS Status)
{
    if (self)
        return self->vtable->GetStatus(Status);

}
void ithModule_SetProperty(CaptureModuleDriver self, MODULE_SETPROPERTY Property, uint16_t Value)
{
    if (self)
        self->vtable->SetProperty(Property,Value);
}

void ithModule_PowerDown(CaptureModuleDriver self, unsigned char enable)
{
    if (self)
        self->vtable->PowerDown(enable);
}

void ithModule_OutputPinTriState(CaptureModuleDriver self, unsigned char flag)
{
    if (self)
        self->vtable->OutputPinTriState(flag);
}

