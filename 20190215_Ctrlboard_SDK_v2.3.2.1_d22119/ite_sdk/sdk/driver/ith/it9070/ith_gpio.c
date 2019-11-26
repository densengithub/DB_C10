/*
 * Copyright(c) 2015 ITE Tech.Inc.All Rights Reserved.
 */
/** @file
 * HAL GPIO functions.
 *
 * @author Jim Tan
 * @author Benson Lin
 * @author I-Chun Lai
 * @version 1.0
 */
#include "../ith_cfg.h"

static const uint32_t PINDIR_REG_ADDR[2]        = { ITH_GPIO_BASE + ITH_GPIO1_PINDIR_REG,       ITH_GPIO_BASE + ITH_GPIO2_PINDIR_REG };
static const uint32_t DATASET_REG_ADDR[2]       = { ITH_GPIO_BASE + ITH_GPIO1_DATASET_REG,      ITH_GPIO_BASE + ITH_GPIO2_DATASET_REG };
static const uint32_t DATACLR_REG_ADDR[2]       = { ITH_GPIO_BASE + ITH_GPIO1_DATACLR_REG,      ITH_GPIO_BASE + ITH_GPIO2_DATACLR_REG };
static const uint32_t DATAIN_REG_ADDR[2]        = { ITH_GPIO_BASE + ITH_GPIO1_DATAIN_REG,       ITH_GPIO_BASE + ITH_GPIO2_DATAIN_REG };
static const uint32_t INTREN_REG_ADDR[2]        = { ITH_GPIO_BASE + ITH_GPIO1_INTREN_REG,       ITH_GPIO_BASE + ITH_GPIO2_INTREN_REG };
static const uint32_t INTRCLR_REG_ADDR[2]       = { ITH_GPIO_BASE + ITH_GPIO1_INTRCLR_REG,      ITH_GPIO_BASE + ITH_GPIO2_INTRCLR_REG };
static const uint32_t BOUNCEEN_REG_ADDR[2]      = { ITH_GPIO_BASE + ITH_GPIO1_BOUNCEEN_REG,     ITH_GPIO_BASE + ITH_GPIO2_BOUNCEEN_REG };
static const uint32_t PULLEN_REG_ADDR[2]        = { ITH_GPIO_BASE + ITH_GPIO1_PULLEN_REG,       ITH_GPIO_BASE + ITH_GPIO2_PULLEN_REG };
static const uint32_t PULLTYPE_REG_ADDR[2]      = { ITH_GPIO_BASE + ITH_GPIO1_PULLTYPE_REG,     ITH_GPIO_BASE + ITH_GPIO2_PULLTYPE_REG };
static const uint32_t INTRTRIG_REG_ADDR[2]      = { ITH_GPIO_BASE + ITH_GPIO1_INTRTRIG_REG,     ITH_GPIO_BASE + ITH_GPIO2_INTRTRIG_REG };
static const uint32_t INTRBOTH_REG_ADDR[2]      = { ITH_GPIO_BASE + ITH_GPIO1_INTRBOTH_REG,     ITH_GPIO_BASE + ITH_GPIO2_INTRBOTH_REG };
static const uint32_t INTRRISENEG_REG_ADDR[2]   = { ITH_GPIO_BASE + ITH_GPIO1_INTRRISENEG_REG,  ITH_GPIO_BASE + ITH_GPIO2_INTRRISENEG_REG };
static const uint32_t MODE_REG_ADDR[4]          = { ITH_GPIO_BASE + ITH_GPIO1_MODE_REG,         ITH_GPIO_BASE + ITH_GPIO2_MODE_REG,
                                                    ITH_GPIO_BASE + ITH_GPIO3_MODE_REG,         ITH_GPIO_BASE + ITH_GPIO4_MODE_REG };
static const uint32_t URTXSEL_REG_ADDR[2]       = { ITH_GPIO_BASE + ITH_GPIO_URTXSEL1_REG,      ITH_GPIO_BASE + ITH_GPIO_URTXSEL2_REG, };
static const uint32_t URRXSEL_REG_ADDR[2]       = { ITH_GPIO_BASE + ITH_GPIO_URRXSEL1_REG,      ITH_GPIO_BASE + ITH_GPIO_URRXSEL2_REG, };
static const uint32_t DRIVING_SET_REG_ADDR[4]   = { ITH_GPIO_BASE + ITH_GPIO1_DRIVING_SET_REG,  ITH_GPIO_BASE + ITH_GPIO2_DRIVING_SET_REG,
                                                    ITH_GPIO_BASE + ITH_GPIO3_DRIVING_SET_REG,  ITH_GPIO_BASE + ITH_GPIO4_DRIVING_SET_REG };

static uint32_t gpioRegs[(ITH_GPIO_URRXSEL2_REG - ITH_GPIO1_PINDIR_REG + 4) / 4];

/*
The above code is equal to:
static const uint32_t PINDIR_REG_ADDR[2]        = { 0xDE000000 + 0x08, 0xDE000000 + 0x48 };
static const uint32_t DATASET_REG_ADDR[2]       = { 0xDE000000 + 0x0C, 0xDE000000 + 0x4C };
static const uint32_t DATACLR_REG_ADDR[2]       = { 0xDE000000 + 0x10, 0xDE000000 + 0x50 };
static const uint32_t DATAIN_REG_ADDR[2]        = { 0xDE000000 + 0x04, 0xDE000000 + 0x44 };
static const uint32_t INTREN_REG_ADDR[2]        = { 0xDE000000 + 0x1C, 0xDE000000 + 0x5C };
static const uint32_t INTRCLR_REG_ADDR[2]       = { 0xDE000000 + 0x2C, 0xDE000000 + 0x6C };
static const uint32_t BOUNCEEN_REG_ADDR[2]      = { 0xDE000000 + 0x3C, 0xDE000000 + 0x7C };
static const uint32_t PULLEN_REG_ADDR[2]        = { 0xDE000000 + 0x14, 0xDE000000 + 0x54 };
static const uint32_t PULLTYPE_REG_ADDR[2]      = { 0xDE000000 + 0x18, 0xDE000000 + 0x58 };
static const uint32_t INTRTRIG_REG_ADDR[2]      = { 0xDE000000 + 0x30, 0xDE000000 + 0x70 };
static const uint32_t INTRBOTH_REG_ADDR[2]      = { 0xDE000000 + 0x34, 0xDE000000 + 0x74 };
static const uint32_t INTRRISENEG_REG_ADDR[2]   = { 0xDE000000 + 0x38, 0xDE000000 + 0x78 };
static const uint32_t MODE_REG_ADDR[4]          = { 0xDE000000 + 0x90, 0xDE000000 + 0x94, 0xDE000000 + 0x98, 0xDE000000 + 0x9C };
static const uint32_t URTXSEL_REG_ADDR[2]       = { 0xDE000000 + 0xD8, 0xDE000000 + 0xDC };
static const uint32_t URRXSEL_REG_ADDR[2]       = { 0xDE000000 + 0xE0, 0xDE000000 + 0xE4 };
static const uint32_t DRIVING_SET_REG_ADDR[4]   = { 0xDE000000 + 0xA0, 0xDE000000 + 0xA4, 0xDE000000 + 0xA8, 0xDE000000 + 0xAC };

static uint32_t gpioRegs[(0xE4 - 0x08 + 4) / 4];
*/

void ithGpioSetMode(unsigned int pin, ITHGpioMode mode)
{
    uint32_t value, mask;
    int group = pin >> 5;

    ithEnterCritical();
    switch (mode)
    {

	// for UART0 output
	case ITH_GPIO_MODE_TX0:
	case ITH_GPIO_MODE_RX0:
		mode = 1;
		break;

    // for UART1 output
    case ITH_GPIO_MODE_TX1:
        ithSetRegBitA(URTXSEL_REG_ADDR[group], (pin & 0x1F));
        mode = 0;
        break;

    case ITH_GPIO_MODE_RX1:
        ithSetRegBitA(URRXSEL_REG_ADDR[group], (pin & 0x1F));
        mode = 0;
        break;
    }


    value = mode << ((pin & 0xF)* 2);
    mask  = 0x3 << ((pin & 0xF) * 2);
    ithWriteRegMaskA(MODE_REG_ADDR[pin >> 4], value, mask);

    ithExitCritical();
}

void ithGpioCtrlEnable(unsigned int pin, ITHGpioCtrl ctrl)
{
    int group = pin >> 5;

    ithEnterCritical();

    switch (ctrl)
    {
    case ITH_GPIO_PULL_ENABLE:
        ithSetRegBitA(PULLEN_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_PULL_UP:
        ithSetRegBitA(PULLTYPE_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_INTR_LEVELTRIGGER:
        ithSetRegBitA(INTRTRIG_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_INTR_BOTHEDGE:
        ithSetRegBitA(INTRBOTH_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_INTR_TRIGGERFALLING:
        ithSetRegBitA(INTRRISENEG_REG_ADDR[group], (pin & 0x1F));
        break;
    }

    ithExitCritical();
}

void ithGpioCtrlDisable(unsigned int pin, ITHGpioCtrl ctrl)
{
    int group = pin >> 5;

    ithEnterCritical();

    switch (ctrl)
    {
    case ITH_GPIO_PULL_ENABLE:
        ithClearRegBitA(PULLEN_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_PULL_UP:
        ithClearRegBitA(PULLTYPE_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_INTR_LEVELTRIGGER:
        ithClearRegBitA(INTRTRIG_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_INTR_BOTHEDGE:
        ithClearRegBitA(INTRBOTH_REG_ADDR[group], (pin & 0x1F));
        break;

    case ITH_GPIO_INTR_TRIGGERFALLING:
        ithClearRegBitA(INTRRISENEG_REG_ADDR[group], (pin & 0x1F));
        break;
    }

    ithExitCritical();
}

void ithGpioSuspend(void)
{
    int i;

    for (i = 0; i < ITH_COUNT_OF(gpioRegs); i++)
    {
        switch (i)
        {
        case ITH_GPIO1_DATASET_REG:
        case ITH_GPIO1_DATACLR_REG:
        case ITH_GPIO1_INTRRAWSTATE_REG:
        case ITH_GPIO1_INTRMASKSTATE_REG:
        case ITH_GPIO1_INTRCLR_REG:
        case ITH_GPIO2_DATAOUT_REG:
        case ITH_GPIO2_DATAIN_REG:
        case ITH_GPIO2_DATASET_REG:
        case ITH_GPIO2_DATACLR_REG:
        case ITH_GPIO2_INTRRAWSTATE_REG:
        case ITH_GPIO2_INTRMASKSTATE_REG:
        case ITH_GPIO2_INTRCLR_REG:
        case ITH_GPIO_FEATURE_REG:
        case ITH_GPIO_REV_REG:
            // don't need to backup
            break;

        default:
            gpioRegs[i] = ithReadRegA(ITH_GPIO_BASE + ITH_GPIO1_PINDIR_REG + i * 4);
        }
    }
}

void ithGpioResume(void)
{
    int i;

    for (i = 0; i < ITH_COUNT_OF(gpioRegs); i++)
    {
        switch (i)
        {
        case ITH_GPIO1_DATASET_REG:
        case ITH_GPIO1_DATACLR_REG:
        case ITH_GPIO1_INTRRAWSTATE_REG:
        case ITH_GPIO1_INTRMASKSTATE_REG:
        case ITH_GPIO1_INTRCLR_REG:
        case ITH_GPIO2_DATAOUT_REG:
        case ITH_GPIO2_DATAIN_REG:
        case ITH_GPIO2_DATASET_REG:
        case ITH_GPIO2_DATACLR_REG:
        case ITH_GPIO2_INTRRAWSTATE_REG:
        case ITH_GPIO2_INTRMASKSTATE_REG:
        case ITH_GPIO2_INTRCLR_REG:
        case ITH_GPIO_FEATURE_REG:
        case ITH_GPIO_REV_REG:
            // don't need to restore
            break;

        default:
            ithWriteRegA(ITH_GPIO_BASE + ITH_GPIO1_PINDIR_REG + i * 4, gpioRegs[i]);
        }
    }
}

void ithGpioEnableClock(void)
{
    // enable clock
    ithSetRegBitH(ITH_APB_CLK2_REG, ITH_EN_W1CLK_BIT);
}

void ithGpioDisableClock(void)
{
    // disable clock
    ithClearRegBitH(ITH_APB_CLK2_REG, ITH_EN_W1CLK_BIT);
}

void ithGpioStats(void)
{
    PRINTF("GPIO:\r\n");
    ithPrintRegA(ITH_GPIO_BASE + ITH_GPIO1_DATAOUT_REG, ITH_GPIO_URRXSEL2_REG - ITH_GPIO1_DATAOUT_REG + sizeof(uint32_t));
}

inline void ithGpioSetIn(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
    ithClearRegBitA(PINDIR_REG_ADDR[group], pin & 0x1F);
    ithExitCritical();
}

inline void ithGpioSetOut(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
    ithSetRegBitA(PINDIR_REG_ADDR[group], pin & 0x1F);
    ithExitCritical();
}

inline void ithGpioSet(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
    ithWriteRegA(DATASET_REG_ADDR[group], 0x1 << (pin & 0x1F));
    ithExitCritical();
}

void ithGpioSetDriving(unsigned int pin, ITHGpioDriving level)
{
	ithEnterCritical();
    ithWriteRegMaskA(DRIVING_SET_REG_ADDR[pin >> 4], level << ((pin & 0xF) * 2),  (0x3) <<((pin & 0xF) * 2));
    ithExitCritical();
}

inline void ithGpioClear(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
    ithWriteRegA(DATACLR_REG_ADDR[group], 0x1 << (pin & 0x1F));
    ithExitCritical();
}

inline uint32_t ithGpioGet(unsigned int pin)
{
    int group = pin >> 5;

    return ithReadRegA(DATAIN_REG_ADDR[group]) & (0x1 << (pin & 0x1F));
}

inline void ithGpioEnableIntr(unsigned int pin)
{
    int group = pin >> 5;

    //ithEnterCritical();
    ithSetRegBitA(INTREN_REG_ADDR[group], (pin & 0x1F));
    //ithExitCritical();
}

inline void ithGpioDisableIntr(unsigned int pin)
{
    int group = pin >> 5;

    //ithEnterCritical();
    ithClearRegBitA(INTREN_REG_ADDR[group], (pin & 0x1F));
    //ithExitCritical();
}

inline void ithGpioClearIntr(unsigned int pin)
{
    int group = pin >> 5;

    //ithEnterCritical();
    ithSetRegBitA(INTRCLR_REG_ADDR[group], (pin & 0x1F));
    //ithExitCritical();
}

inline void ithGpioEnableBounce(unsigned int pin)
{
    int group = pin >> 5;

    //ithEnterCritical();
    ithSetRegBitA(BOUNCEEN_REG_ADDR[group], (pin & 0x1F));
    //ithExitCritical();
}

inline void ithGpioDisableBounce(unsigned int pin)
{
    int group = pin >> 5;

    //ithEnterCritical();
    ithClearRegBitA(BOUNCEEN_REG_ADDR[group], (pin & 0x1F));
    //ithExitCritical();
}

inline void ithGpioSetDebounceClock(unsigned int clk)
{
    ithWriteRegA(ITH_GPIO_BASE + ITH_GPIO2_BOUNCEPRESCALE_REG, ithGetBusClock() / clk - 1);
}
