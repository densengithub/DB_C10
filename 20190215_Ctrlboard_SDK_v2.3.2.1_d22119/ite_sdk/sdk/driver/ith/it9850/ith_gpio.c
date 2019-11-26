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

static uint32_t gpioRegs[(ITH_GPIO_REV_REG - ITH_GPIO1_PINDIR_REG + 4) / 4];

static const uint32_t PINDIR_REG_ADDR[4]        = { ITH_GPIO_BASE + ITH_GPIO1_PINDIR_REG,       ITH_GPIO_BASE + ITH_GPIO2_PINDIR_REG,       ITH_GPIO_BASE + ITH_GPIO3_PINDIR_REG,       ITH_GPIO_BASE + ITH_GPIO4_PINDIR_REG, };
static const uint32_t DATASET_REG_ADDR[4]       = { ITH_GPIO_BASE + ITH_GPIO1_DATASET_REG,      ITH_GPIO_BASE + ITH_GPIO2_DATASET_REG,      ITH_GPIO_BASE + ITH_GPIO3_DATASET_REG,      ITH_GPIO_BASE + ITH_GPIO4_DATASET_REG, };
static const uint32_t DATACLR_REG_ADDR[4]       = { ITH_GPIO_BASE + ITH_GPIO1_DATACLR_REG,      ITH_GPIO_BASE + ITH_GPIO2_DATACLR_REG,      ITH_GPIO_BASE + ITH_GPIO3_DATACLR_REG,      ITH_GPIO_BASE + ITH_GPIO4_DATACLR_REG, };
static const uint32_t DATAIN_REG_ADDR[4]        = { ITH_GPIO_BASE + ITH_GPIO1_DATAIN_REG,       ITH_GPIO_BASE + ITH_GPIO2_DATAIN_REG,       ITH_GPIO_BASE + ITH_GPIO3_DATAIN_REG,       ITH_GPIO_BASE + ITH_GPIO4_DATAIN_REG, };
static const uint32_t DATAOUT_REG_ADDR[4]       = { ITH_GPIO_BASE + ITH_GPIO1_DATAOUT_REG,      ITH_GPIO_BASE + ITH_GPIO2_DATAOUT_REG,      ITH_GPIO_BASE + ITH_GPIO3_DATAOUT_REG,      ITH_GPIO_BASE + ITH_GPIO4_DATAOUT_REG, };
static const uint32_t INTREN_REG_ADDR[4]        = { ITH_GPIO_BASE + ITH_GPIO1_INTREN_REG,       ITH_GPIO_BASE + ITH_GPIO2_INTREN_REG,       ITH_GPIO_BASE + ITH_GPIO3_INTREN_REG,       ITH_GPIO_BASE + ITH_GPIO4_INTREN_REG, };
static const uint32_t INTRCLR_REG_ADDR[4]       = { ITH_GPIO_BASE + ITH_GPIO1_INTRCLR_REG,      ITH_GPIO_BASE + ITH_GPIO2_INTRCLR_REG,      ITH_GPIO_BASE + ITH_GPIO3_INTRCLR_REG,      ITH_GPIO_BASE + ITH_GPIO4_INTRCLR_REG, };
static const uint32_t BOUNCEEN_REG_ADDR[4]      = { ITH_GPIO_BASE + ITH_GPIO1_BOUNCEEN_REG,     ITH_GPIO_BASE + ITH_GPIO2_BOUNCEEN_REG,     ITH_GPIO_BASE + ITH_GPIO3_BOUNCEEN_REG,     ITH_GPIO_BASE + ITH_GPIO4_BOUNCEEN_REG, };
static const uint32_t PULLEN_REG_ADDR[4]        = { ITH_GPIO_BASE + ITH_GPIO1_PULLEN_REG,       ITH_GPIO_BASE + ITH_GPIO2_PULLEN_REG,       ITH_GPIO_BASE + ITH_GPIO3_PULLEN_REG,       ITH_GPIO_BASE + ITH_GPIO4_PULLEN_REG, };
static const uint32_t PULLTYPE_REG_ADDR[4]      = { ITH_GPIO_BASE + ITH_GPIO1_PULLTYPE_REG,     ITH_GPIO_BASE + ITH_GPIO2_PULLTYPE_REG,     ITH_GPIO_BASE + ITH_GPIO3_PULLTYPE_REG,     ITH_GPIO_BASE + ITH_GPIO4_PULLTYPE_REG, };
static const uint32_t INTRTRIG_REG_ADDR[4]      = { ITH_GPIO_BASE + ITH_GPIO1_INTRTRIG_REG,     ITH_GPIO_BASE + ITH_GPIO2_INTRTRIG_REG,     ITH_GPIO_BASE + ITH_GPIO3_INTRTRIG_REG,     ITH_GPIO_BASE + ITH_GPIO4_INTRTRIG_REG, };
static const uint32_t INTRBOTH_REG_ADDR[4]      = { ITH_GPIO_BASE + ITH_GPIO1_INTRBOTH_REG,     ITH_GPIO_BASE + ITH_GPIO2_INTRBOTH_REG,     ITH_GPIO_BASE + ITH_GPIO3_INTRBOTH_REG,     ITH_GPIO_BASE + ITH_GPIO4_INTRBOTH_REG, };
static const uint32_t INTRRISENEG_REG_ADDR[4]   = { ITH_GPIO_BASE + ITH_GPIO1_INTRRISENEG_REG,  ITH_GPIO_BASE + ITH_GPIO2_INTRRISENEG_REG,  ITH_GPIO_BASE + ITH_GPIO3_INTRRISENEG_REG,  ITH_GPIO_BASE + ITH_GPIO4_INTRRISENEG_REG, };
static const uint32_t MODE_REG_ADDR[7]          = { ITH_GPIO_BASE + ITH_GPIO1_MODE_REG,         ITH_GPIO_BASE + ITH_GPIO2_MODE_REG,         ITH_GPIO_BASE + ITH_GPIO3_MODE_REG,         ITH_GPIO_BASE + ITH_GPIO4_MODE_REG,
                                                    ITH_GPIO_BASE + ITH_GPIO5_MODE_REG,         ITH_GPIO_BASE + ITH_GPIO6_MODE_REG,         ITH_GPIO_BASE + ITH_GPIO7_MODE_REG };
static const uint32_t DRIVING_SET_REG_ADDR[7]   = { ITH_GPIO_BASE + ITH_GPIO1_DRIVING_SET_REG,  ITH_GPIO_BASE + ITH_GPIO2_DRIVING_SET_REG,  ITH_GPIO_BASE + ITH_GPIO3_DRIVING_SET_REG,  ITH_GPIO_BASE + ITH_GPIO4_DRIVING_SET_REG,
                                                    ITH_GPIO_BASE + ITH_GPIO5_DRIVING_SET_REG,  ITH_GPIO_BASE + ITH_GPIO6_DRIVING_SET_REG,  ITH_GPIO_BASE + ITH_GPIO7_DRIVING_SET_REG };

/*
The above code is equal to:
static const uint32_t PINDIR_REG_ADDR[4]        = { 0xDE000000 + 0x08,  0xDE000000 + 0x48,  0xDE000000 + 0x88,  0xDE000000 + 0xC8 };
static const uint32_t DATASET_REG_ADDR[4]       = { 0xDE000000 + 0x0C,  0xDE000000 + 0x4C,  0xDE000000 + 0x8C,  0xDE000000 + 0xCC };
static const uint32_t DATACLR_REG_ADDR[4]       = { 0xDE000000 + 0x10,  0xDE000000 + 0x50,  0xDE000000 + 0x90,  0xDE000000 + 0xD0 };
static const uint32_t DATAIN_REG_ADDR[4]        = { 0xDE000000 + 0x04,  0xDE000000 + 0x44,  0xDE000000 + 0x84,  0xDE000000 + 0xC4 };
static const uint32_t DATAOUT_REG_ADDR[4]       = { 0xDE000000 + 0x00,  0xDE000000 + 0x40,  0xDE000000 + 0x80,  0xDE000000 + 0xC0 };
static const uint32_t INTREN_REG_ADDR[4]        = { 0xDE000000 + 0x1C,  0xDE000000 + 0x5C,  0xDE000000 + 0x9C,  0xDE000000 + 0xDC };
static const uint32_t INTRCLR_REG_ADDR[4]       = { 0xDE000000 + 0x2C,  0xDE000000 + 0x6C,  0xDE000000 + 0xAC,  0xDE000000 + 0xEC };
static const uint32_t BOUNCEEN_REG_ADDR[4]      = { 0xDE000000 + 0x3C,  0xDE000000 + 0x7C,  0xDE000000 + 0xBC,  0xDE000000 + 0xFC };
static const uint32_t PULLEN_REG_ADDR[4]        = { 0xDE000000 + 0x14,  0xDE000000 + 0x54,  0xDE000000 + 0x94,  0xDE000000 + 0xD4 };
static const uint32_t PULLTYPE_REG_ADDR[4]      = { 0xDE000000 + 0x18,  0xDE000000 + 0x58,  0xDE000000 + 0x98,  0xDE000000 + 0xD8 };
static const uint32_t INTRTRIG_REG_ADDR[4]      = { 0xDE000000 + 0x30,  0xDE000000 + 0x70,  0xDE000000 + 0xB0,  0xDE000000 + 0xF0 };
static const uint32_t INTRBOTH_REG_ADDR[4]      = { 0xDE000000 + 0x34,  0xDE000000 + 0x74,  0xDE000000 + 0xB4,  0xDE000000 + 0xF4 };
static const uint32_t INTRRISENEG_REG_ADDR[4]   = { 0xDE000000 + 0x38,  0xDE000000 + 0x78,  0xDE000000 + 0xB8,  0xDE000000 + 0xF8 };
static const uint32_t MODE_REG_ADDR[7]          = { 0xDE000000 + 0x100, 0xDE000000 + 0x104, 0xDE000000 + 0x108, 0xDE000000 + 0x10C, 0xDE000000 + 0x110, 0xDE000000 + 0x114, 0xDE000000 + 0x118 };
static const uint32_t DRIVING_SET_REG_ADDR[7]   = { 0xDE000000 + 0x130, 0xDE000000 + 0x134, 0xDE000000 + 0x138, 0xDE000000 + 0x13C, 0xDE000000 + 0x140, 0xDE000000 + 0x144, 0xDE000000 + 0x148 };
*/

void ithGpioSetMode(unsigned int pin, ITHGpioMode mode)
{
    uint32_t value, mask;

    ithEnterCritical();

    switch (mode)
    {
    // for UART0 output
    case ITH_GPIO_MODE_TX0:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL1_REG, ITH_GPIO_URTX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL1_REG, pin << ITH_GPIO_URTX_BIT, ITH_GPIO_URTX_MASK);
        mode = 0;
        break;

    case ITH_GPIO_MODE_RX0:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL1_REG, ITH_GPIO_URRX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL1_REG, pin << ITH_GPIO_URRX_BIT, ITH_GPIO_URRX_MASK);
        mode = 0;
        break;

    // for UART1 output
    case ITH_GPIO_MODE_TX1:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL2_REG, ITH_GPIO_URTX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL2_REG, pin << ITH_GPIO_URTX_BIT, ITH_GPIO_URTX_MASK);
        mode = 0;
        break;

    case ITH_GPIO_MODE_RX1:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL2_REG, ITH_GPIO_URRX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL2_REG, pin << ITH_GPIO_URRX_BIT, ITH_GPIO_URRX_MASK);
        mode = 0;
        break;

    // for UART2 output
    case ITH_GPIO_MODE_TX2:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL3_REG, ITH_GPIO_URTX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL3_REG, pin << ITH_GPIO_URTX_BIT, ITH_GPIO_URTX_MASK);
        mode = 0;
        break;

    case ITH_GPIO_MODE_RX2:
        ithClearRegBitA(ITH_GPIO_BASE + ITH_GPIO_MISC_SET_REG, ITH_GPIO_UR2_RXSRC_BIT);
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL3_REG, ITH_GPIO_URRX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL3_REG, pin << ITH_GPIO_URRX_BIT, ITH_GPIO_URRX_MASK);
        mode = 0;
        break;

    case ITH_GPIO_MODE_RX2WGAND:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_MISC_SET_REG, ITH_GPIO_UR2_RXSRC_BIT);
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL3_REG, ITH_GPIO_URRX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL3_REG, pin << ITH_GPIO_URRX_BIT, ITH_GPIO_URRX_MASK);
        mode = 0;
        break;

    // for UART3 output
    case ITH_GPIO_MODE_TX3:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL4_REG, ITH_GPIO_URTX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL4_REG, pin << ITH_GPIO_URTX_BIT, ITH_GPIO_URTX_MASK);
        mode = 0;
        break;

    case ITH_GPIO_MODE_RX3:
        ithClearRegBitA(ITH_GPIO_BASE + ITH_GPIO_MISC_SET_REG, ITH_GPIO_UR3_RXSRC_BIT);
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL4_REG, ITH_GPIO_URRX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL4_REG, pin << ITH_GPIO_URRX_BIT, ITH_GPIO_URRX_MASK);
        mode = 0;
        break;

    case ITH_GPIO_MODE_RX3WGAND:
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_MISC_SET_REG, ITH_GPIO_UR3_RXSRC_BIT);
        ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO_URSEL4_REG, ITH_GPIO_URRX_EN_BIT);
        ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_URSEL4_REG, pin << ITH_GPIO_URRX_BIT, ITH_GPIO_URRX_MASK);
        mode = 0;
        break;
    }

    value = mode << ((pin & 0xF) * 2);
    mask  = 0x3 << ((pin & 0xF) * 2);
    if (pin < 96)
    {
        ithWriteRegMaskA(MODE_REG_ADDR[pin >> 4], value, mask);
    }
    else
    {
        // workaround: Register ITH_GPIO7_MODE_REG is write only (IC defact).
        // So we need a temp variable to store the GPIO7 mode setting.
        static uint16_t gpio7ModeRegData = 0;
        gpio7ModeRegData = ((gpio7ModeRegData & ~mask) | (value & mask));
        ithWriteRegA(ITH_GPIO_BASE + ITH_GPIO7_MODE_REG, gpio7ModeRegData);
    }

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
    ithPrintRegA(ITH_GPIO_BASE + ITH_GPIO1_DATAOUT_REG, ITH_GPIO_REV_REG - ITH_GPIO1_DATAOUT_REG + sizeof(uint32_t));   //for 9850
}

void ithGpioSetDriving(unsigned int pin, ITHGpioDriving level)
{
	ithEnterCritical();
    ithWriteRegMaskA(DRIVING_SET_REG_ADDR[pin >> 4], level << ((pin & 0xF) * 2),  (0x3) <<((pin & 0xF) * 2));
    ithExitCritical();
}

inline void ithGpioSetIn(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
	if(pin == 81)  //GPIO 81 has IC bugs, PinDIR and DataOutEnAnble needs invers , software workaround.
	{
		ithWriteRegA(ITH_GPIO_BASE + ITH_GPIO3_DATASET_REG ,  0x1 << (pin & 0x1F));
	}else
	{
    	ithClearRegBitA(PINDIR_REG_ADDR[group], pin & 0x1F);
	}
    ithExitCritical();
}

inline void ithGpioSetOut(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
	if( pin == 81) //GPIO 81 has IC bugs, PinDIR and DataOutEnAnble needs invers , software workaround.
	{
		ithWriteRegA(ITH_GPIO_BASE + ITH_GPIO3_DATACLR_REG,  0x1 << (pin & 0x1F));
	}
	else
	{
		ithSetRegBitA(PINDIR_REG_ADDR[group], pin & 0x1F);
	}
    ithExitCritical();
}

inline void ithGpioSetDataOut(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
	if( pin == 81) //GPIO 81 has IC bugs, PinDIR and DataOutEnAnble needs invers , software workaround.
	{
		ithWriteRegA(ITH_GPIO_BASE + ITH_GPIO3_DATAOUT_REG,  0x1 << (pin & 0x1F));
	}
	else
	{
		ithSetRegBitA(DATAOUT_REG_ADDR[group], pin & 0x1F);
	}
    ithExitCritical();
}

inline void ithGpioSet(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
	if( pin == 81)
	{
		ithClearRegBitA(ITH_GPIO_BASE + ITH_GPIO3_PINDIR_REG, pin & 0x1F);
	}else
	{
		ithWriteRegA(DATASET_REG_ADDR[group], 0x1 << (pin & 0x1F));
	}
    ithExitCritical();
}

inline void ithGpioClear(unsigned int pin)
{
    int group = pin >> 5;

    ithEnterCritical();
	if(pin == 81)
	{
		ithSetRegBitA(ITH_GPIO_BASE + ITH_GPIO3_PINDIR_REG, pin & 0x1F);
	}else
	{
    	ithWriteRegA(DATACLR_REG_ADDR[group], 0x1 << (pin & 0x1F));
	}
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