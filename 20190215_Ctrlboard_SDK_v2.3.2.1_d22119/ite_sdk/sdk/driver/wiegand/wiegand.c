#include "wiegand.h"
#include <sys/time.h>
#include "ite/ith.h"
#include "wiegand/wiegand.h"

#define WM0             0x000
#define WM1             0x010
#define WSR0            0x004
#define WSR1            0x014
#define WUSR0           0x008
#define WUSR1           0x018
#define WIR0       		0x00C

#define WIEGAND_RST_ENABLE		(1 << 31)
#define WIEGAND_RST_DISABLE 	(0 << 31)
#define WIEGAND_RESET_MASK  	(1 << 31)
#define WIEGAND_D0_SEL_MASK	 	0x7F
#define WIEGAND_D1_SEL_MASK		(0x7F << 8)

typedef struct WIEGAND_OBJECT_TAG
{
	WIEGAND_IN_ID id;
}WIEGAND_OBJECT;

static void _wiegand_reset(WIEGAND_IN_ID id)
{
	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, WIEGAND_RST_ENABLE, WIEGAND_RESET_MASK);
	usleep(1000);
	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, WIEGAND_RST_DISABLE, WIEGAND_RESET_MASK);
}

static void _wiegand_controller_enable(WIEGAND_IN_ID id, int enable)
{
	if (enable)
		ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (1 << 30), (1 << 30));
	else
		ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (0 << 30), (1 << 30));
}

static void _wiegand_parity_enable(WIEGAND_IN_ID id, int enable)
{
	if (enable)
		ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (1 << 29), (1 << 29));
	else
		ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (0 << 29), (1 << 29));
}

static void _wiegand_set_polarity(WIEGAND_IN_ID id, int polarity)
{
	if (polarity)
		ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (1 << 28), (1 << 28));
	else
		ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (0 << 28), (1 << 28));
}

static void _wiegand_set_bitcnt(WIEGAND_IN_ID id, int bitcnt)
{
#if CFG_CHIP_FAMILY == 970 || CFG_CHIP_FAMILY == 9850

	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (1 << 16), (1 << 16));

	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (bitcnt << 20), (0xff << 20));

#else
	int mode = 0;
	switch (bitcnt)
	{
	case 26:
		mode = 0x0;
		break;
	case 34:
		mode = 0x1;
		break;
	case 37:
		mode = 0x2;
		break;
	default:
		mode = 0x4; //auto detect
		break;
	}
	//printf("mode = %d\n", mode);

	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (mode << 16), (0x7 << 16));
#endif
}

void wiegand_verify_enable(WIEGAND_IN_ID id, int enable)
{
	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (enable << 18), (1 << 18));
}

static void _wiegand_set_order(WIEGAND_IN_ID id, int order)
{
	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, (order << 17), (1 << 17));
}

static void _wiegand_set_pins(WIEGAND_IN_ID id, int d0pin, int d1pin)
{
	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, d0pin, WIEGAND_D0_SEL_MASK);
	ithWriteRegMaskA(WIEGAND_BASE + id * 0x010, d1pin << 8, WIEGAND_D1_SEL_MASK);
}

static void _wiegand_set_debounce(WIEGAND_IN_ID id, int value)
{
	ithWriteRegMaskA(WIEGAND_BASE + WSR0 + id * 0x010, (value << 30), (0x3 << 30));
}

static void _wiegand_set_prescale(WIEGAND_IN_ID id, int value)
{
	ithWriteRegMaskA(WIEGAND_BASE + WSR0 + id * 0x010, (value << 16), (0x1FFF << 16));
}

static void _wiegand_set_timeout(WIEGAND_IN_ID id, int value)
{
	ithWriteRegMaskA(WIEGAND_BASE + WSR0 + id * 0x010, value, 0x1FFF);
}

static void _wiegand_set_uartprescale(WIEGAND_IN_ID id, int value)
{
	ithWriteRegMaskA(WIEGAND_BASE + WUSR0 + id * 0x010, (value << 16), (0x1F << 16));
}

static void _wiegand_set_uartdiv(WIEGAND_IN_ID id, int value)
{
	ithWriteRegMaskA(WIEGAND_BASE + WUSR0 + id * 0x010, value, 0xFFFF);
}

static void _wiegand_set_intr_clear(WIEGAND_IN_ID id, int mode)
{
	if (mode)
		ithWriteRegMaskA(WIEGAND_BASE + WIR0 + id * 0x010, (1 << 31), (1 << 31));
	else
		ithWriteRegMaskA(WIEGAND_BASE + WIR0 + id * 0x010, (0 << 31), (1 << 31));
}

static void _wiegand_set_probsel(WIEGAND_IN_ID id, int mode)
{
	ithWriteRegMaskA(WIEGAND_BASE + WIR0 + id * 0x010, (mode << 6), (0x7 << 6));
}

static void _wiegand_set_uart(WIEGAND_IN_ID id, unsigned int uartBase)
{
	uint32_t uartdiv, busclk;

	busclk = ithGetBusClock();
	uartdiv = busclk / (16 * WIEGAND_UART_BAUD);

	ithWriteRegMaskA(ITH_GPIO_BASE + ITH_GPIO_MISC_SET_REG, (1 << (30 + id)), (1 << (30 + id)));

	ithWriteRegA(uartBase + ITH_UART_LCR_REG, 0x0080);
	ithWriteRegA(uartBase + ITH_UART_IIR_REG, 0x0001);
	ithWriteRegA(uartBase + ITH_UART_THR_REG, uartdiv & 0xFF);
	ithWriteRegA(uartBase + ITH_UART_IER_REG, (uartdiv & 0xF00) >> 8);
	ithWriteRegA(uartBase + ITH_UART_IIR_TXFIFOFULL, 0x000d);	// 0x0d-> 0x01
	ithWriteRegA(uartBase + ITH_UART_LCR_REG, 0x0003);
	ithWriteRegA(uartBase + ITH_UART_FCR_REG, 0x0007);
	ithWriteRegA(uartBase + ITH_UART_IER_REG, 0x0002);
	ithWriteRegA(uartBase + 0x24, 0x0003);
}

static void _wiegand_suspend(WIEGAND_IN_ID id, unsigned int uartBase)
{
	uint32_t uartdiv, wuartdiv, prescale, timeout, busclk;

	busclk = ithGetBusClock();
	wuartdiv = busclk / WIEGAND_UART_BAUD;
	prescale = (10 * busclk) / 1000000;
	uartdiv = busclk / (16 * WIEGAND_UART_BAUD);

	_wiegand_set_uartdiv(id, wuartdiv);
	_wiegand_set_prescale(id, prescale);

	ithWriteRegA(uartBase + ITH_UART_LCR_REG, 0x0080);
	ithWriteRegA(uartBase + ITH_UART_THR_REG, uartdiv & 0xFF);
	ithWriteRegA(uartBase + ITH_UART_IER_REG, (uartdiv & 0xF00) >> 8);
	ithWriteRegA(uartBase + ITH_UART_LCR_REG, 0x0003);
}

static void _wiegand_resume(WIEGAND_IN_ID id, unsigned int uartBase)
{
	uint32_t uartdiv, wuartdiv, prescale, timeout, busclk;

	busclk = ithGetBusClock();
	wuartdiv = busclk / WIEGAND_UART_BAUD;
	prescale = (10 * busclk) / 1000000;
	uartdiv = busclk / (16 * WIEGAND_UART_BAUD);

	_wiegand_set_uartdiv(id, wuartdiv);
	_wiegand_set_prescale(id, prescale);

	ithWriteRegA(uartBase + ITH_UART_LCR_REG, 0x0080);
	ithWriteRegA(uartBase + ITH_UART_THR_REG, uartdiv & 0xFF);
	ithWriteRegA(uartBase + ITH_UART_IER_REG, (uartdiv & 0xF00) >> 8);
	ithWriteRegA(uartBase + ITH_UART_LCR_REG, 0x0003);
}

unsigned int wiegand_get_uart_base(WIEGAND_IN_ID id)
{
#if CFG_CHIP_FAMILY == 970
	if (id == wiegand_in_0)
	{
		return ITH_UART4_BASE;
	}
	else
	{
		return ITH_UART5_BASE;
	}
#else
	if (id == wiegand_in_0)
	{
		return ITH_UART2_BASE;
	}
	else
	{
		return ITH_UART3_BASE;
	}
#endif
}

void init_wiegand_controller(WIEGAND_IN_ID id)
{
	uint32_t busclk, prescale, uartdiv;

	busclk = ithGetBusClock();
	uartdiv = busclk / WIEGAND_UART_BAUD;
	prescale = (10 * busclk) / 1000000;

	_wiegand_reset(id);
	_wiegand_parity_enable(id, 1);
	_wiegand_set_prescale(id, prescale);
	_wiegand_set_uartdiv(id, uartdiv);
	_wiegand_set_timeout(id, 0x012C);
	_wiegand_set_debounce(id, 0x1);
	_wiegand_set_intr_clear(id, 1);
	_wiegand_set_probsel(id, 0x2);
}

void wiegand_controller_enable(WIEGAND_IN_ID id, int d0pin, int d1pin, int bitcnt)
{
	unsigned int uartBase = wiegand_get_uart_base(id);
	_wiegand_set_uart(id, uartBase);
	_wiegand_set_pins(id, d0pin, d1pin);
	_wiegand_set_bitcnt(id, bitcnt);
	_wiegand_controller_enable(id, 1);
}

void wiegand_suspend(WIEGAND_IN_ID id)
{
	unsigned int uartBase = wiegand_get_uart_base(id);
	_wiegand_suspend(id, uartBase);
}

void wiegand_resume(WIEGAND_IN_ID id)
{
	unsigned int uartBase = wiegand_get_uart_base(id);
	_wiegand_resume(id, uartBase);
}