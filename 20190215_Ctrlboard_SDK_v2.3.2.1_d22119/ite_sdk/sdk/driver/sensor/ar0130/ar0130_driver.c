#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "sensor/ar0130_driver.h"

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
ithAR0130Initialize(
    void)
{
    printf("init AR0130\n");
    ithAR0130PowerOn();
    ithAR0130LedOn();
}

void
ithAR0130PowerOn(
    void)
{
    //GPIO30 PWDN
    //Set GPIO30 to Low
    ithGpioClear(ITH_GPIO_PIN30);
    //Set GPIO30 Output Mode
    ithGpioSetOut(ITH_GPIO_PIN30);
    //Set GPIO30 Mode0
    ithGpioSetMode(ITH_GPIO_PIN30, ITH_GPIO_MODE0);
}

void
ithAR0130PowerOff(
    void)
{
    ithGpioSet(ITH_GPIO_PIN30);
}

void
ithAR0130LedOn(
    void)
{
    //GPIO31 LED_EN
    //Set GPIO31 to High
    ithGpioSet(ITH_GPIO_PIN31);
    //Set GPIO31 Output Mode
    ithGpioSetOut(ITH_GPIO_PIN31);
    //Set GPIO31 Mode0
    ithGpioSetMode(ITH_GPIO_PIN31, ITH_GPIO_MODE0);
}

void
ithAR0130LedOff(
    void)
{
    ithGpioClear(ITH_GPIO_PIN31);
}

void
ithAR0130SetAntiFlicker50Hz(
    void)
{
}

void
ithAR0130SetAntiFlicker60Hz(
    void)
{
}