#include <stdio.h>
#include <string.h>
#include "can_hw.h"
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

void ithCANEnableIntr(CAN_HANDLE* base, CAN_INTERRUPTS_EN intr)
{
    if(intr >= Time_Trigger_Enable)
        ithSetRegBitA((base->ADDR | CAN_VER_0_REG), intr);
    else
        ithSetRegBitA((base->ADDR | CAN_RTIE_REG), intr);
}

void ithCANDisableIntr(CAN_HANDLE* base, CAN_INTERRUPTS_EN intr)
{
    if(intr >= Time_Trigger_Enable)
        ithClearRegBitA((base->ADDR | CAN_VER_0_REG), intr);
    else
        ithClearRegBitA((base->ADDR | CAN_RTIE_REG), intr);
}

void ithCANClearIntrFlag(CAN_HANDLE* base, CAN_INTERRUPTS_FLAG flag)
{
    if(flag >= Time_Trigger_Interrupt_Flag)
        ithSetRegBitA((base->ADDR | CAN_VER_0_REG), flag);//WRITE ONE CLEAR
    else
        ithSetRegBitA((base->ADDR | CAN_RTIE_REG), flag);//WRITE ONE CLEAR
}
uint32_t ithCANGetIntrFlag(CAN_HANDLE* base)
{   
    return ithReadRegA((base->ADDR | CAN_RTIE_REG));
}

uint32_t ithTTCANGetIntrFlag(CAN_HANDLE* base) 
{
    return ithReadRegA((base->ADDR | CAN_VER_0_REG));
}
//=============================================================================
/**
 * CAN-CTRL Reset
 * @param[in]   base address
 * @param[in]   0 - no local reset of CAN-CTRL, 1 - The host controller performs a local reset of CAN-CTRL.
 * @return none
 */
//=============================================================================

void ithCANSWReset(CAN_HANDLE* base, bool OnOff)
{
    if(OnOff)
    {
        //sw reset 
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_RESET, CAN_RESET);
        //bus off ,reset RECNT TECNT
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_BUSOFF, CAN_BUSOFF);
    }
    else
    {
        //sw reset stop 
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), ~CAN_RESET, CAN_RESET);
        //bus on
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), ~CAN_BUSOFF, CAN_BUSOFF);
    }
}
//=============================================================================
/**
 * CAN-CTRL Loop Back Mode Setting
 * @param[in]   base address
 * @param[in]   external 0 - Disabled, 1 - Enabled
 * @param[in]   internal 0 - Disabled, 1 - Enabled
 * @return none
 */
//=============================================================================

void ithCANSetLoopBack(CAN_HANDLE* base, bool external, bool internal)
{
	if (external)
	{
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_LBME, CAN_LBME);
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_SACK, CAN_SACK);
	}
	else
	{
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), ~CAN_LBME, CAN_LBME);
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), ~CAN_SACK, CAN_SACK);
	}
        
	if (internal)
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_LBMI, CAN_LBMI);
    else
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), ~CAN_LBMI, CAN_LBMI);
}
//=============================================================================
/**
 * CAN-CTRL Listen Only Mode Setting
 * @param[in]   base address
 * @param[in]   0 - Disabled, 1 - Enabled
 * @return none
 */
//=============================================================================

void ithCANSetListenOnlyMode(CAN_HANDLE* base, bool onoff)
{
    if(onoff)
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_LOM, CAN_LOM);
    else
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), ~CAN_LOM, CAN_LOM);
}
//=============================================================================
/**
 * CAN-CTRL Transceiver Standby Mode Setting
 * @param[in]   base address
 * @param[in]   0 - Disabled, 1 - Enabled
 * @return none
 */
//=============================================================================

void ithCANSetTransStandbyMode(CAN_HANDLE* base, bool onoff)
{
    if(onoff)
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_STBY, CAN_STBY);
    else
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), ~CAN_STBY, CAN_STBY);
}
//=============================================================================
/**
 * CAN FD ISO mode 
 * @param[in]   base address
 * @param[in]   0 - Bosch CAN FD (non-ISO) mode , 1 - ISO mode
 * @return none
 */
//=============================================================================

void ithCANSetFDISO(CAN_HANDLE* base)
{
    if(base->ProtocolType == protocol_FD_ISO)
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG),CAN_FD_ISO, CAN_FD_ISO);
    else
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG),~CAN_FD_ISO, CAN_FD_ISO);
}
//=============================================================================
/**
 * CAN Bit Timing Setting
 * @param[in]   base address
 * @param[in]   s_bt Slow speed is used for CAN 2.0 and the CAN FD arbitration phase
 * @param[in]   f_bt  Fast speed is used for the CAN FD data phase
 * @return none
 */
//=============================================================================

void ithCANSetBitRate(CAN_HANDLE* base, CAN_BTATTR s_bt, CAN_BTATTR f_bt)
{
    uint32_t S_Seg1_DATA;
    S_Seg1_DATA = (s_bt.Seg_1 << CAN_S_Seg_1_SHIFT) | (s_bt.Seg_2 << CAN_S_Seg_2_SHIFT) 
                    | (s_bt.SJW << CAN_S_SJW_SHIFT) | (s_bt.Prescaler << CAN_S_PRESC_SHIFT);
    
    ithWriteRegA((base->ADDR | CAN_S_Seg_1_REG), S_Seg1_DATA);

    uint32_t F_Seg1_DATA;
    F_Seg1_DATA = (f_bt.Seg_1 << CAN_F_Seg_1_SHIFT) | (f_bt.Seg_2 << CAN_F_Seg_2_SHIFT)
                    | (f_bt.SJW << CAN_F_SJW_SHIFT) | (f_bt.Prescaler << CAN_F_PRESC_SHIFT);
    
    ithWriteRegA((base->ADDR | CAN_F_Seg_1_REG), F_Seg1_DATA);

	ithWriteRegMaskA((base->ADDR | CAN_EALCAP_REG), f_bt.SSPOFF << CAN_SSPOFF_SHIFT, CAN_SSPOFF);

    ithWriteRegMaskA((base->ADDR | CAN_EALCAP_REG), 0x1 << CAN_TDCEN_SHIFT, CAN_TDCEN);

}
//=============================================================================
/**
 * CiA 603 Time-Stamping
 * @param[in]  base address
 * @param[in]  TIME-stamping Enable ,0-disabled , 1-enabled
 * @param[in]  TIME-stamping Position, 0-SOF , 1-EOF
 * @return none
 */
//=============================================================================

void ithCANSetCIA603(CAN_HANDLE* base, bool OnOff, bool position)
{   
    ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), 0x0 << CAN_TIMEEN_SHIFT, CAN_TIMEEN);
    if(OnOff)
    {
        ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), position << CAN_TIMEPOS_SHIFT, CAN_TIMEPOS);
        ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), 0x1 << CAN_TIMEEN_SHIFT, CAN_TIMEEN);
    }

}
//=============================================================================
/**
 * Set Acceptance CODE
 * @param[in]  base address
 * @param[in]  Acceptance index range(0 - 15)
 * @param[in]  CODE number  bit(0-28)
 * @return none
 */
//=============================================================================

void ithCANSetACODE(CAN_HANDLE* base, CAN_ACODE c_index, uint32_t value)
{
    assert(c_index < CFG_ACF_NUMBER);
    ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG),  ~CAN_SELMASK, CAN_SELMASK);//SELMASK =0;
    ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), c_index, CAN_ACFADR);//Set ACFADR

    uint32_t ACF_DATA = ithReadRegA((base->ADDR | CAN_ACF_0_REG));
    ACF_DATA = value & 0x1FFFFFFF;
    ithWriteRegA((base->ADDR | CAN_ACF_0_REG), ACF_DATA);
}
//=============================================================================
/**
 * Set Acceptance MASK
 * @param[in]  base address
 * @param[in]  Acceptance MASK index range(0 - 15)
 * @param[in]  MASK number  bit(0-28)
 * @param[in]  AIDEE 0 - accepts both standard or extended 1 - defined by AIDE
 * @param[in]  AIDE   0 - only standard frames 1- only extended frames
 * @return none
 */
//=============================================================================

void ithCANSetAMASK(CAN_HANDLE* base, CAN_AMASK m_index, uint32_t value , bool AIDEE, bool AIDE)
{
    assert(m_index < CFG_ACF_NUMBER);
    ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), CAN_SELMASK, CAN_SELMASK);//SELMASK =1;
    ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), m_index, CAN_ACFADR);//Set ACFADR

    uint32_t ACF_DATA = ithReadRegA((base->ADDR | CAN_ACF_0_REG));
    ACF_DATA = (value & 0x1FFFFFFF) | (AIDE << 29) | (AIDEE << 30);
    ithWriteRegA((base->ADDR | CAN_ACF_0_REG), ACF_DATA);
}
//=============================================================================
/**
 * Acceptance filter Enable 
 * @param[in]   base address
 * @param[in]   index ACF id (range 0x0 - CFG_ACF_NUMBER)
 * @param[in]   On/Off
 * @return none
 */
//=============================================================================

void ithCANACFEnable(CAN_HANDLE* base, CAN_ACFE index, bool OnOff)
{
    assert(index < CFG_ACF_NUMBER);
    if(index <= 0x7)
        ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), OnOff << CAN_AE0_x_SHIFT, CAN_AE0_x);
    else
        ithWriteRegMaskA((base->ADDR | CAN_ACFCTRL_REG), OnOff << CAN_AE1_x_SHIFT, CAN_AE1_x);
}
//=============================================================================
/**
 * Receive buffer STATus
 * @param[in]   base address
 * @return 00 - empty , 01 - > empty and < almost full (AFWL) , 10 - almost full , 11 - full
 */
//=============================================================================

char ithCANGetRBStatus(CAN_HANDLE* base)
{
    uint32_t STAT_DATA;
    char RSTAT;
    STAT_DATA = ithReadRegA((base->ADDR | CAN_CFG_STAT_REG));
	printf("STAT_DATA = %x", STAT_DATA);
    RSTAT =  (STAT_DATA & CAN_RSTAT) >> CAN_RSTAT_SHIFT;
    return RSTAT;
}
//=============================================================================
/**
 * Transmission Secondary buffer STATus
 * @param[in]   base address
 * @return 00 - empty , 01 - > empty and < almost full (AFWL) , 10 - almost full , 11 - full
 */
//=============================================================================

char ithCANGetTBStatus(CAN_HANDLE* base)
{
    uint32_t STAT_DATA;
    char TSTAT;
    STAT_DATA = ithReadRegA((base->ADDR | CAN_CFG_STAT_REG));
    TSTAT =  (STAT_DATA & CAN_TSSTAT) >> CAN_TSSTAT_SHIFT;
    return TSTAT;
}

//=============================================================================
/**
 * Transmit Primary Enable
 * @param[in]   base address
 * @param[in]   Ctrl bit  1 - Start Transmit,  0 - Stop Transmit
 * @return none
 */
//=============================================================================

void ithCANPrimarySendCtrl(CAN_HANDLE* base, bool OnOff)
{
	printf("0xa = %x\n", ithReadRegA((base->ADDR | CAN_CFG_STAT_REG)));

    if(OnOff)
    {
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_TPE, CAN_TPE);
    }
    else
    {
        ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_TPA, CAN_TPA);
    }
}
//=============================================================================
/**
* Transmit Secondary ALL frame Enable
* @param[in]   base address
* @param[in]   Ctrl bit  1 - Start Transmit,  0 - Stop Transmit
* @return none
*/
//=============================================================================

void ithCANSecondarySendCtrl(CAN_HANDLE* base, bool OnOff)
{
	printf("0xa = %x\n", ithReadRegA((base->ADDR | CAN_CFG_STAT_REG)));

	if (OnOff)
	{
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_TSALL, CAN_TSALL);
	}
	else
	{
		ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), CAN_TSA, CAN_TSA);
	}
}
//=============================================================================
/**
* Get Receive Error Count
* @param[in]   base address
* @return Receive Error Count
*/
//=============================================================================

uint32_t ithCANGetReceiveErrorCouNT(CAN_HANDLE* base)
{
	uint32_t EALCAP_DATA = ithReadRegA((base->ADDR | CAN_EALCAP_REG));
	return (EALCAP_DATA & CAN_RECNT) >> CAN_RECNT_SHIFT;
}
//=============================================================================
/**
* Get Transmit Error Count
* @param[in]   base address
* @return Transmit Error Count
*/
//=============================================================================

uint32_t ithCANGetTransmitErrorCouNT(CAN_HANDLE* base)
{
	uint32_t EALCAP_DATA = ithReadRegA((base->ADDR | CAN_EALCAP_REG));
	return (EALCAP_DATA & CAN_TECNT) >> CAN_TECNT_SHIFT;
}

//=============================================================================
/**
* Get Kind of Error
* @param[in]   base address
* @return Kind of Error
* @ 000 - no error
* @ 001 - BIT ERROR
* @ 010 - FORM ERROR
* @ 011 - STUFF ERROR
* @ 100 - ACKNOWLEDGEMENT ERROR
* @ 101 - CRC ERROR
* @ 110 - OTHER ERROR
*/
//=============================================================================

uint32_t ithCANGetKindOfError(CAN_HANDLE* base)
{
	uint32_t EALCAP_DATA = ithReadRegA((base->ADDR | CAN_EALCAP_REG));
	return (EALCAP_DATA & CAN_KOER) >> CAN_KOER_SHIFT;
}

//=============================================================================
/**
* Get Arbitration Lost Capture
* @param[in]   base address
* @return Arbitration Lost Capture
*/
//=============================================================================

uint32_t ithCANGetALC(CAN_HANDLE* base)
{
	uint32_t EALCAP_DATA = ithReadRegA((base->ADDR | CAN_EALCAP_REG));
	return (EALCAP_DATA & CAN_ALC) >> CAN_ALC_SHIFT;
}

//=============================================================================
/**
 * Time Trigger Mode Enable
 * @param[in]   base address
 * @param[in]  TTCAN Timer PRESCaler(b00 - b11), b00-1, b01-2 ,b10-4 ,b11-8
 * @return none
 */
//=============================================================================

void ithTTCANCrtlON(CAN_HANDLE* base, uint16_t PRESCaler, uint16_t Trigger_Time)
{
    uint32_t VER_0_DATA;
    uint32_t TRIG_CFG_0_DATA;
	assert(PRESCaler <= 0x11);

    VER_0_DATA = ithReadRegA((base->ADDR | CAN_VER_0_REG));
    VER_0_DATA |= (0x1 << CAN_TTEN_SHIFT);
    VER_0_DATA |= (PRESCaler << CAN_T_PRESC_SHIFT);
	printf("VER_0_DATA = %x \n", VER_0_DATA);
    ithWriteRegA((base->ADDR | CAN_VER_0_REG), VER_0_DATA);
    
    TRIG_CFG_0_DATA = ithReadRegA((base->ADDR | CAN_TRIG_CFG_0_REG));
    TRIG_CFG_0_DATA |= (Trigger_Time << CAN_TT_TRIG0_SHIFT);
    ithWriteRegA((base->ADDR | CAN_TRIG_CFG_0_REG), TRIG_CFG_0_DATA);

	ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), 0x1 << CAN_TTTBM_SHIFT, CAN_TTTBM);
}
//=============================================================================
/**
* Time Trigger Mode Disable
* @param[in]   base address
* @return none
*/
//=============================================================================

void ithTTCANCrtlOFF(CAN_HANDLE* base)
{
	uint32_t VER_0_DATA;
	VER_0_DATA = ithReadRegA((base->ADDR | CAN_VER_0_REG));
	VER_0_DATA &= ~CAN_TTEN;
	ithWriteRegA((base->ADDR | CAN_VER_0_REG), VER_0_DATA);

	ithWriteRegMaskA((base->ADDR | CAN_CFG_STAT_REG), 0x0 , CAN_TTTBM);

}
//=============================================================================
/**
 * Time Trigger Mode , Set REFerence message IDentifier
 * @param[in]   base address
 * @param[in]  REFID , bit[30:0]  [2:0] forced 0
 * @param[in]  IDformat 1- extended ID, 0-standard ID
 * @return none
 */
//=============================================================================

void ithTTCANSetREFID(CAN_HANDLE* base, uint32_t REFID, bool IDformat)
{
    REFID = REFID & (~0x7); //[2:0] forced 0.
    if(IDformat)
        REFID |= CAN_REF_IDE;
    else
        REFID &= ~CAN_REF_IDE;
    
     ithWriteRegA((base->ADDR | CAN_REF_MSG_0_REG), REFID);
}
//=============================================================================
/**
 * Time Trigger Mode , Set Trigger Type
 * @param[in]   base address
 * @param[in]   trigger type
 * @return none
 */
//=============================================================================

void ithTTCANSetTrig(CAN_HANDLE* base, CAN_TRIGGER_TYPE trigtype)
{
    ithWriteRegMaskA((base->ADDR | CAN_TRIG_CFG_0_REG), trigtype << CAN_TTYPE_SHIFT, CAN_TTYPE);
}
//=============================================================================
/**
 * Time Trigger Mode , Get Current Trigger Type
 * @param[in]   base address
 * @return Trigger Type
 */
//=============================================================================

uint32_t ithTTCANGetTrig(CAN_HANDLE* base)
{
    uint32_t TRIG_DATA = ithReadRegA((base->ADDR | CAN_TRIG_CFG_0_REG));
    return (TRIG_DATA & CAN_TTYPE) >> CAN_TTYPE_SHIFT;
}   
//=============================================================================
/**
* Time Trigger Mode , Set Trigger Time
* @param[in]   base address
* @param[in]   trigger time
* @return none
*/
//=============================================================================

void ithTTCANSetTrigTime(CAN_HANDLE* base, uint16_t trigtime)
{
	uint32_t TT_TRIG_0 = (trigtime & 0xff);
	uint32_t TT_TRIG_1 = (trigtime & 0xff00) >> 8;
	ithWriteRegMaskA((base->ADDR | CAN_TRIG_CFG_0_REG), TT_TRIG_0 << CAN_TT_TRIG0_SHIFT, CAN_TT_TRIG0);
	ithWriteRegMaskA((base->ADDR | CAN_TRIG_CFG_0_REG), TT_TRIG_1 << CAN_TT_TRIG1_SHIFT, CAN_TT_TRIG1);
}
//=============================================================================
/**
* Time Trigger Mode , Set Watch Trigger Time
* @param[in]   base address
* @param[in]   watch Trigger Time
* @return none
*/
//=============================================================================

void ithTTCANSetWatchTrigTime(CAN_HANDLE* base, uint16_t watchtrigtime)
{
	uint32_t TT_WTRIG_0 = (watchtrigtime & 0xff);
	uint32_t TT_WTRIG_1 = (watchtrigtime & 0xff00) >> 8;
	ithWriteRegMaskA((base->ADDR | CAN_TT_WTRIG_0_REG), TT_WTRIG_0 << CAN_TT_WTRIG0_SHIFT, CAN_TT_WTRIG0);
	ithWriteRegMaskA((base->ADDR | CAN_TT_WTRIG_0_REG), TT_WTRIG_1 << CAN_TT_WTRIG1_SHIFT, CAN_TT_WTRIG1);
}