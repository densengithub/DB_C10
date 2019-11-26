#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "sensor/omnivision_io.h"
#include "sensor/omnivision_driver.h"
#include "sensor/sensor_gpio_config.h"
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
void
ithOmnivisionInitialize(
    void)
{
    printf("init Omnivision\n");
    ithOmnivisionPowerOn();
    ithOmnivisionLedOn();

    Omnivision_WriteI2C_Byte(0x09, 0x03); //[1:0]: driving, 00: 1x, 01: 2x, 10: 3x, 11: 4x
    uint8_t data;
    data = Omnivision_ReadI2C_Byte(0x09);
    printf("set Omnivision driving %x\n", data);
}

void
ithOmnivisionPowerOn(
    void)
{
    //GPIO30 PWDN
    //Set GPIO30 to Low
    ithGpioClear(SN1_PWN_GPIO);
    //Set GPIO30 Output Mode
    ithGpioSetOut(SN1_PWN_GPIO);
    //Set GPIO30 Mode0
    ithGpioSetMode(SN1_PWN_GPIO, ITH_GPIO_MODE0);
}

void
ithOmnivisionPowerOff(
    void)
{
    ithGpioSet(SN1_PWN_GPIO);
}

void
ithOmnivisionLedOn(
    void)
{
    //GPIO31 LED_EN
    //Set GPIO31 to High
    ithGpioSet(SN1_LED_GPIO);
    //Set GPIO31 Output Mode
    ithGpioSetOut(SN1_LED_GPIO);
    //Set GPIO31 Mode0
    ithGpioSetMode(SN1_LED_GPIO, ITH_GPIO_MODE0);
}

void
ithOmnivisionLedOff(
    void)
{
    ithGpioClear(SN1_LED_GPIO);
}

void
ithOmnivisionSetAntiFlicker50Hz(
    void)
{
    Omnivision_WriteI2C_Byte(0x2b, 0x9e);
};

void
ithOmnivisionSetAntiFlicker60Hz(
    void)
{
    Omnivision_WriteI2C_Byte(0x2b, 0x0);
};