#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL/SDL.h"
#include "ite/itu.h"
#include "ite/itp.h"
#include "scene.h"
#include "project.h"

static const unsigned int McuBkGpioTable[] = {MCU_BK1_PIN,MCU_BK2_PIN,MCU_BK3_PIN};
static const unsigned int CanbusIntGpioTable[] = {CAN_INT_PIN,CAN_INT0_PIN,CAN_INT1_PIN};
volatile unsigned int Gyrosensor_Intr_Flag = 0;
volatile unsigned int Psensor_Intr_Flag = 0;
volatile unsigned int Elec_Lock_Flag = 0;

void Mcubk1IntrHandler(unsigned int pin, void* arg)
{
    
}

void Mcubk2IntrHandler(unsigned int pin, void* arg)
{
    
}

void Mcubk3IntrHandler(unsigned int pin, void* arg)
{
    
}

void CanbusIntrHandler(unsigned int pin, void* arg)
{
    
}

void Canbus0IntrHandler(unsigned int pin, void* arg)
{
    
}

void Canbus1IntrHandler(unsigned int pin, void* arg)
{
    
}

void McuAccIntrHandler(unsigned int pin, void* arg)
{
    if(MCU_ACC_PIN == pin&&(!ithGpioGet(MCU_ACC_PIN)))
    {
        Elec_Lock_Flag = 1;
    }    
    ithGpioClearIntr(pin);
}


#if 0
void GyrosensorIntrHandler(unsigned int pin, void* arg)
{
    if (GYRO_MPUINT_PIN == pin)
    {
       
    }   
    ithGpioClearIntr(pin);
}

void PsensorIntrHandler(unsigned int pin, void* arg)
{
    if (ALSPS_EPLINT_PIN == pin)
    {
       
    }   
    ithGpioClearIntr(pin);
}
#endif

ITHGpioIntrHandler McuBkIntrHandlerTable[]={Mcubk1IntrHandler,Mcubk2IntrHandler,Mcubk3IntrHandler};
ITHGpioIntrHandler CanbusIntrHandlerTable[]={CanbusIntrHandler,Canbus0IntrHandler,Canbus1IntrHandler};

void BspGpioInit(void)
{
    IIC_MODE iic_port1_mode = MASTER_MODE;
    Gyrosensor_Intr_Flag = 0;
    Elec_Lock_Flag = 0;
   

    /***init MCU BK pin**/
    // McuBkHandlerTable[0] = Mcubk1IntrHandler;
    // McuBkHandlerTable[1] = Mcubk2IntrHandler;
    // McuBkHandlerTable[2] = Mcubk3IntrHandler;    
    for (int i = 0; i < ARRAY_COUNT_OF(McuBkGpioTable);i++)
    {
        ithGpioRegisterIntrHandler(McuBkGpioTable[i], McuBkIntrHandlerTable[i], NULL);
        ithGpioCtrlEnable(McuBkGpioTable[i], ITH_GPIO_INTR_TRIGGERLOW);
        ithGpioEnableIntr(McuBkGpioTable[i]);
        ithGpioSetIn(McuBkGpioTable[i]);
        ithGpioEnable(McuBkGpioTable[i]);
    }
    
    /***init Acc pin**/
    ithGpioRegisterIntrHandler(MCU_ACC_PIN, McuAccIntrHandler, NULL);
    ithGpioCtrlEnable(MCU_ACC_PIN, ITH_GPIO_INTR_TRIGGERLOW);
    ithGpioEnableIntr(MCU_ACC_PIN);
    ithGpioSetIn(MCU_ACC_PIN);
    ithGpioEnable(MCU_ACC_PIN); 
    

    /***init gyrosensor int pin**/
    // ithGpioRegisterIntrHandler(GYRO_MPUINT_PIN, GyrosensorIntrHandler, NULL);
    // ithGpioCtrlEnable(GYRO_MPUINT_PIN, ITH_GPIO_INTR_TRIGGERLOW);
    // ithGpioEnableIntr(GYRO_MPUINT_PIN);
    // ithGpioSetIn(GYRO_MPUINT_PIN);
    // ithGpioEnable(GYRO_MPUINT_PIN);    

    // itpRegisterDevice(ITP_DEVICE_I2C1, &itpDeviceGyroSensor);
    // ioctl(ITP_DEVICE_I2C1, ITP_IOCTL_INIT, (void *)iic_port1_mode);   

    /***init P-sensor int pin**/
    // ithGpioRegisterIntrHandler(ALSPS_EPLINT_PIN, PsensorIntrHandler, NULL);
    // ithGpioCtrlEnable(ALSPS_EPLINT_PIN, ITH_GPIO_INTR_TRIGGERLOW);
    // ithGpioEnableIntr(ALSPS_EPLINT_PIN);
    // ithGpioSetIn(ALSPS_EPLINT_PIN);
    // ithGpioEnable(ALSPS_EPLINT_PIN); 

    // itpRegisterDevice(ITP_DEVICE_I2C1, &itpDeviceI2c1);
    // ioctl(ITP_DEVICE_I2C1, ITP_IOCTL_INIT, (void *)iic_port1_mode);   
    
    /***init Can-Bus int pin**/
   for (int i = 0; i < ARRAY_COUNT_OF(CanbusIntGpioTable);i++)
    {
        //ithGpioRegisterIntrHandler(CanbusIntGpioTable[i], CanbusIntrHandlerTable[i], NULL);
        //ithGpioCtrlEnable(CanbusIntGpioTable[i], ITH_GPIO_INTR_TRIGGERLOW);
        //ithGpioEnableIntr(CanbusIntGpioTable[i]);
        ithGpioSetIn(CanbusIntGpioTable[i]);
        ithGpioEnable(CanbusIntGpioTable[i]);
    }
    //itpRegisterDevice(ITP_DEVICE_SPI, &itpDeviceSpi0);   
    //ioctl(ITP_DEVICE_SPI, ITP_IOCTL_INIT, NULL);    
    
    #if 0
    /***init RS485 pin**/  
    {
        //ithGpioRegisterIntrHandler(CanbusIntGpioTable[i], CanbusIntrHandlerTable[i], NULL);
        //ithGpioCtrlEnable(CanbusIntGpioTable[i], ITH_GPIO_INTR_TRIGGERLOW);
        //ithGpioEnableIntr(CanbusIntGpioTable[i]);
        ithGpioSetIn(CanbusIntGpioTable[i]);
        ithGpioEnable(CanbusIntGpioTable[i]);
    }
    //itpRegisterDevice(ITP_DEVICE_SPI, &itpDeviceSpi0);   
    //ioctl(ITP_DEVICE_SPI, ITP_IOCTL_INIT, NULL);       
    #endif

}


