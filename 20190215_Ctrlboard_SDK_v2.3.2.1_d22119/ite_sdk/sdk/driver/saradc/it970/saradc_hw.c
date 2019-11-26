#include "ite/ith.h"
#include "saradc_reg.h"
#include "saradc_hw.h"

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
//=============================================================================
/**
 * Fire SARADC Engine
 */
//=============================================================================
void
SARADC_DriverFire_Reg(
    void)
{
    ithWriteRegMaskA(SARADC_REG_CR0, (0x1 & SARADC_BIT_DRIVER_FIRE_EN) << SARADC_SHT_DRIVER_FIRE_EN,
            SARADC_BIT_DRIVER_FIRE_EN << SARADC_SHT_DRIVER_FIRE_EN);

    ithWriteRegMaskA(SARADC_REG_CR0, (0x1 & SARADC_BIT_PHY_FIRE_EN) << SARADC_SHT_PHY_FIRE_EN,
            SARADC_BIT_PHY_FIRE_EN << SARADC_SHT_PHY_FIRE_EN);
}

//=============================================================================
/**
 * Stop SARADC Engine
 */
//=============================================================================
void
SARADC_DriverStop_Reg(
    void)
{
    ithWriteRegMaskA(SARADC_REG_CR0, (0x0 & SARADC_BIT_PHY_FIRE_EN) << SARADC_SHT_PHY_FIRE_EN,
            SARADC_BIT_PHY_FIRE_EN << SARADC_SHT_PHY_FIRE_EN);

    ithWriteRegMaskA(SARADC_REG_CR0, (0x0 & SARADC_BIT_DRIVER_FIRE_EN) << SARADC_SHT_DRIVER_FIRE_EN,
            SARADC_BIT_DRIVER_FIRE_EN << SARADC_SHT_DRIVER_FIRE_EN);
}

//=============================================================================
/**
 * Set Clock Divider of SARADC
 */
//=============================================================================
void
SARADC_SetCLKDIV_Reg(
    SARADC_CLK_DIV divider)
{
    ithWriteRegMaskA(SARADC_REG_CR0, (divider & SARADC_BIT_CLK_DIV) << SARADC_SHT_CLK_DIV,
            SARADC_BIT_CLK_DIV << SARADC_SHT_CLK_DIV);
}

//=============================================================================
/**
 * Set the Mode of SARADC
 */
//=============================================================================
void
SARADC_SetMode_Reg(
    SARADC_MODE mode)
{
    ithWriteRegMaskA(SARADC_REG_CR0, (mode & SARADC_BIT_MODE) << SARADC_SHT_MODE,
            SARADC_BIT_MODE << SARADC_SHT_MODE);
}

//=============================================================================
/**
 * Set GAIN Driving of SARADC
 */
//=============================================================================
void
SARADC_SetGAIN_Reg(
    SARADC_AMPLIFY_GAIN driving)
{
    ithWriteRegMaskA(SARADC_REG_CR0, (driving & SARADC_BIT_GAIN_SETTING) << SARADC_SHT_GAIN_SETTING,
            SARADC_BIT_GAIN_SETTING << SARADC_SHT_GAIN_SETTING);
}

//=============================================================================
/**
 * Set Write Buffer Size of SARADC
 */
//=============================================================================
void
SARADC_SetWBSize_Reg(
    uint16_t wbSize)
{
    ithWriteRegMaskA(SARADC_REG_CR1, ((uint32_t)wbSize & SARADC_BIT_BUFFER_LEN) << SARADC_SHT_BUFFER_LEN,
            SARADC_BIT_BUFFER_LEN << SARADC_SHT_BUFFER_LEN);
}

//=============================================================================
/**
 * Set Average Calculation Count of SARADC
 */
//=============================================================================
void
SARADC_SetAVGCALCount_Reg(
    SARADC_AVG_CAL_COUNT avgCalCount)
{
    ithWriteRegMaskA(SARADC_REG_CR1, (avgCalCount & SARADC_BIT_AVG_COUNT) << SARADC_SHT_AVG_COUNT,
            SARADC_BIT_AVG_COUNT << SARADC_SHT_AVG_COUNT);
}

//=============================================================================
/**
 * Set Valid XAIN of SARADC
 */
//=============================================================================
void
SARADC_SetValidXAIN_Reg(
    uint8_t validXAIN)
{
    ithWriteRegMaskA(SARADC_REG_CR0, (validXAIN & SARADC_BIT_XAIN_VALID) << SARADC_SHT_XAIN_VALID,
            SARADC_BIT_XAIN_VALID << SARADC_SHT_XAIN_VALID);
}

//=============================================================================
/**
 * Enable XAIN Interrupt of SARADC
 */
//=============================================================================
void
SARADC_EnableXAININTR_Reg(
    uint8_t enXAININTR)
{
    ithWriteRegMaskA(SARADC_REG_CR0, (enXAININTR & SARADC_BIT_INTR_VALID) << SARADC_SHT_INTR_VALID,
            SARADC_BIT_INTR_VALID << SARADC_SHT_INTR_VALID);
}

//=============================================================================
/**
 * Set XAIN memory base of SARADC
 */
//=============================================================================
void
SARADC_SetXAINMEMBase_Reg(
    SARADC_PORT hwPort,
    uint8_t     *baseAddr)
{
    switch (hwPort)
    {
    case SARADC_0:
        ithWriteRegA(SARADC_REG_XAIN_BASEADDR_0,
                ((uint32_t)baseAddr & SARADC_BIT_BASEADDR_0) << SARADC_SHT_BASEADDR_0);
        break;

    case SARADC_1:
        ithWriteRegA(SARADC_REG_XAIN_BASEADDR_1,
                ((uint32_t)baseAddr & SARADC_BIT_BASEADDR_1) << SARADC_SHT_BASEADDR_1);
        break;

    case SARADC_2:
        ithWriteRegA(SARADC_REG_XAIN_BASEADDR_2,
                ((uint32_t)baseAddr & SARADC_BIT_BASEADDR_2) << SARADC_SHT_BASEADDR_2);
        break;

    case SARADC_3:
        ithWriteRegA(SARADC_REG_XAIN_BASEADDR_3,
                ((uint32_t)baseAddr & SARADC_BIT_BASEADDR_3) << SARADC_SHT_BASEADDR_3);
        break;

    default:
        break;
    }
}

//=============================================================================
/**
 * Set XAIN detection rule of SARADC
 */
//=============================================================================
void
SARADC_SetXAINDetectRule_Reg(
    SARADC_PORT hwPort,
    uint16_t    maxAVGDetect,
    uint16_t    minAVGDetect)
{
    switch (hwPort)
    {
    case SARADC_0:
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_0,
                ((uint32_t)maxAVGDetect & SARADC_BIT_XAIN_RULE_MAX_0) << SARADC_SHT_XAIN_RULE_MAX_0,
                SARADC_BIT_XAIN_RULE_MAX_0 << SARADC_SHT_XAIN_RULE_MAX_0);
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_0,
                ((uint32_t)minAVGDetect & SARADC_BIT_XAIN_RULE_MIN_0) << SARADC_SHT_XAIN_RULE_MIN_0,
                SARADC_BIT_XAIN_RULE_MIN_0 << SARADC_SHT_XAIN_RULE_MIN_0);
        break;

    case SARADC_1:
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_1,
                ((uint32_t)maxAVGDetect & SARADC_BIT_XAIN_RULE_MAX_1) << SARADC_SHT_XAIN_RULE_MAX_1,
                SARADC_BIT_XAIN_RULE_MAX_1 << SARADC_SHT_XAIN_RULE_MAX_1);
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_1,
                ((uint32_t)minAVGDetect & SARADC_BIT_XAIN_RULE_MIN_1) << SARADC_SHT_XAIN_RULE_MIN_1,
                SARADC_BIT_XAIN_RULE_MIN_1 << SARADC_SHT_XAIN_RULE_MIN_1);
        break;

    case SARADC_2:
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_2,
                ((uint32_t)maxAVGDetect & SARADC_BIT_XAIN_RULE_MAX_2) << SARADC_SHT_XAIN_RULE_MAX_2,
                SARADC_BIT_XAIN_RULE_MAX_2 << SARADC_SHT_XAIN_RULE_MAX_2);
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_2,
                ((uint32_t)minAVGDetect & SARADC_BIT_XAIN_RULE_MIN_2) << SARADC_SHT_XAIN_RULE_MIN_2,
                SARADC_BIT_XAIN_RULE_MIN_2 << SARADC_SHT_XAIN_RULE_MIN_2);
        break;

    case SARADC_3:
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_3,
                ((uint32_t)maxAVGDetect & SARADC_BIT_XAIN_RULE_MAX_3) << SARADC_SHT_XAIN_RULE_MAX_3,
                SARADC_BIT_XAIN_RULE_MAX_3 << SARADC_SHT_XAIN_RULE_MAX_3);
        ithWriteRegMaskA(SARADC_REG_XAIN_DECTRULE_3,
                ((uint32_t)minAVGDetect & SARADC_BIT_XAIN_RULE_MIN_3) << SARADC_SHT_XAIN_RULE_MIN_3,
                SARADC_BIT_XAIN_RULE_MIN_3 << SARADC_SHT_XAIN_RULE_MIN_3);
        break;

    default:
        break;
    }
}

//=============================================================================
/**
 * Set XAIN Read pointer of SARADC
 */
//=============================================================================
void
SARADC_SetXAINRPTR_Reg(
    SARADC_PORT hwPort,
    uint16_t    rptr)
{
    switch (hwPort)
    {
    case SARADC_0:
        ithWriteRegMaskA(SARADC_REG_XAIN_RWPTR_0,
                ((uint32_t)rptr & SARADC_BIT_XAIN_RPTR_0) << SARADC_SHT_XAIN_RPTR_0,
                SARADC_BIT_XAIN_RPTR_0 << SARADC_SHT_XAIN_RPTR_0);
        break;

    case SARADC_1:
        ithWriteRegMaskA(SARADC_REG_XAIN_RWPTR_1,
                ((uint32_t)rptr & SARADC_BIT_XAIN_RPTR_1) << SARADC_SHT_XAIN_RPTR_1,
                SARADC_BIT_XAIN_RPTR_1 << SARADC_SHT_XAIN_RPTR_1);
        break;

    case SARADC_2:
        ithWriteRegMaskA(SARADC_REG_XAIN_RWPTR_2,
                ((uint32_t)rptr & SARADC_BIT_XAIN_RPTR_2) << SARADC_SHT_XAIN_RPTR_2,
                SARADC_BIT_XAIN_RPTR_2 << SARADC_SHT_XAIN_RPTR_2);
        break;

    case SARADC_3:
        ithWriteRegMaskA(SARADC_REG_XAIN_RWPTR_3,
                ((uint32_t)rptr & SARADC_BIT_XAIN_RPTR_3) << SARADC_SHT_XAIN_RPTR_3,
                SARADC_BIT_XAIN_RPTR_3 << SARADC_SHT_XAIN_RPTR_3);
        break;

    default:
        break;
    }
}

//=============================================================================
/**
 * Read Write pointer that XAIN recorded of SARADC
 */
//=============================================================================
void
SARADC_ReadXAINWPTR_Reg(
    SARADC_PORT hwPort,
    uint16_t    *wptr)
{
    switch (hwPort)
    {
    case SARADC_0:
        *wptr = (uint16_t)((ithReadRegA(SARADC_REG_XAIN_RWPTR_0) & SARADC_BIT_XAIN_WPTR_0 << SARADC_SHT_XAIN_WPTR_0) >> SARADC_SHT_XAIN_WPTR_0);
        break;

    case SARADC_1:
        *wptr = (uint16_t)((ithReadRegA(SARADC_REG_XAIN_RWPTR_1) & SARADC_BIT_XAIN_WPTR_1 << SARADC_SHT_XAIN_WPTR_1) >> SARADC_SHT_XAIN_WPTR_1);
        break;

    case SARADC_2:
        *wptr = (uint16_t)((ithReadRegA(SARADC_REG_XAIN_RWPTR_2) & SARADC_BIT_XAIN_WPTR_2 << SARADC_SHT_XAIN_WPTR_2) >> SARADC_SHT_XAIN_WPTR_2);
        break;

    case SARADC_3:
        *wptr = (uint16_t)((ithReadRegA(SARADC_REG_XAIN_RWPTR_3) & SARADC_BIT_XAIN_WPTR_3 << SARADC_SHT_XAIN_WPTR_3) >> SARADC_SHT_XAIN_WPTR_3);
        break;

    default:
        break;
    }
}

//=============================================================================
/**
 * Reset XAIN OVERWT Interrupt of SARADC
 */
//=============================================================================
void
SARADC_ResetXAINOVERWT_Reg(
    SARADC_PORT hwPort)
{
    switch (hwPort)
    {
    case SARADC_0:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_OVERWT_0);
        break;

    case SARADC_1:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_OVERWT_1);
        break;

    case SARADC_2:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_OVERWT_2);
        break;

    case SARADC_3:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_OVERWT_3);
        break;

    default:
        break;
    }
}

//=============================================================================
/**
 * Reset average detection Interrupt of SARADC
 */
//=============================================================================
void
SARADC_ResetXAINAVGDetect_Reg(
    SARADC_PORT hwPort)
{
    switch (hwPort)
    {
    case SARADC_0:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_ABOMAX_0);
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_UNDMIN_0);
        break;

    case SARADC_1:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_ABOMAX_1);
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_UNDMIN_1);
        break;

    case SARADC_2:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_ABOMAX_2);
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_UNDMIN_2);
        break;

    case SARADC_3:
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_ABOMAX_3);
        ithClearRegBitA(SARADC_REG_XAIN_INTR, SARADC_SHT_XAIN_UNDMIN_3);
        break;

    default:
        break;
    }
}

//=============================================================================
/**
 * Check XAIN OVERWT Interrupt of SARADC
 */
//=============================================================================
bool
SARADC_IsXAINOVERWT_Reg(
    SARADC_PORT hwPort)
{
    switch (hwPort)
    {
    case SARADC_0:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_OVERWT_0 << SARADC_SHT_XAIN_OVERWT_0) >> SARADC_SHT_XAIN_OVERWT_0)
            return true;
        else
            return false;
        break;

    case SARADC_1:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_OVERWT_1 << SARADC_SHT_XAIN_OVERWT_1) >> SARADC_SHT_XAIN_OVERWT_1)
            return true;
        else
            return false;
        break;

    case SARADC_2:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_OVERWT_2 << SARADC_SHT_XAIN_OVERWT_2) >> SARADC_SHT_XAIN_OVERWT_2)
            return true;
        else
            return false;
        break;

    case SARADC_3:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_OVERWT_3 << SARADC_SHT_XAIN_OVERWT_3) >> SARADC_SHT_XAIN_OVERWT_3)
            return true;
        else
            return false;
        break;

    default:
        break;
    }
}

//=============================================================================
/**
 * Check XAIN average Interrupt of SARADC
 */
//=============================================================================
SARADC_INTR_AVG
SARADC_IsXAINOutOfRange_Reg(
    SARADC_PORT hwPort)
{
    switch (hwPort)
    {
    case SARADC_0:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_ABOMAX_0 << SARADC_SHT_XAIN_ABOMAX_0) >> SARADC_SHT_XAIN_ABOMAX_0)
            return SARADC_INTR_AVG_ABOMAX;
        else if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_UNDMIN_0 << SARADC_SHT_XAIN_UNDMIN_0) >> SARADC_SHT_XAIN_UNDMIN_0)
            return SARADC_INTR_AVG_UNDMIN;
        else
            return SARADC_INTR_AVG_VALID;
        break;

    case SARADC_1:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_ABOMAX_1 << SARADC_SHT_XAIN_ABOMAX_1) >> SARADC_SHT_XAIN_ABOMAX_1)
            return SARADC_INTR_AVG_ABOMAX;
        else if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_UNDMIN_1 << SARADC_SHT_XAIN_UNDMIN_1) >> SARADC_SHT_XAIN_UNDMIN_1)
            return SARADC_INTR_AVG_UNDMIN;
        else
            return SARADC_INTR_AVG_VALID;
        break;

    case SARADC_2:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_ABOMAX_2 << SARADC_SHT_XAIN_ABOMAX_2) >> SARADC_SHT_XAIN_ABOMAX_2)
            return SARADC_INTR_AVG_ABOMAX;
        else if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_UNDMIN_2 << SARADC_SHT_XAIN_UNDMIN_2) >> SARADC_SHT_XAIN_UNDMIN_2)
            return SARADC_INTR_AVG_UNDMIN;
        else
            return SARADC_INTR_AVG_VALID;
        break;

    case SARADC_3:
        if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_ABOMAX_3 << SARADC_SHT_XAIN_ABOMAX_3) >> SARADC_SHT_XAIN_ABOMAX_3)
            return SARADC_INTR_AVG_ABOMAX;
        else if ((ithReadRegA(SARADC_REG_XAIN_INTR) & SARADC_BIT_XAIN_UNDMIN_3 << SARADC_SHT_XAIN_UNDMIN_3) >> SARADC_SHT_XAIN_UNDMIN_3)
            return SARADC_INTR_AVG_UNDMIN;
        else
            return SARADC_INTR_AVG_VALID;
        break;

    default:
        break;
    }
}

