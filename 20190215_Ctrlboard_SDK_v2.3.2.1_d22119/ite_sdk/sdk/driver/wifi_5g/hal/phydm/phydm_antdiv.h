/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#ifndef	__PHYDMANTDIV_H__
#define    __PHYDMANTDIV_H__

/*#define ANTDIV_VERSION	"2.0"  //2014.11.04*/
/*#define ANTDIV_VERSION	"2.1"  //2015.01.13  Dino*/
#define ANTDIV_VERSION	"2.2"  /*2015.01.16  Dino*/

#include "phydm_types.h"

//1 ============================================================
//1  Definition
//1 ============================================================

#define	ANTDIV_INIT		0xff
#define	MAIN_ANT	1		//Ant A or Ant Main
#define	AUX_ANT		2		//AntB or Ant Aux
#define	MAX_ANT		3		// 3 for AP using

#define ANT1_2G 0 // = ANT2_5G
#define ANT2_2G 1 // = ANT1_5G

//Antenna Diversty Control Type
#define	ODM_AUTO_ANT	0
#define	ODM_FIX_MAIN_ANT	1
#define	ODM_FIX_AUX_ANT	2

#define ODM_ANTDIV_SUPPORT		(ODM_RTL8188E|ODM_RTL8192E|ODM_RTL8723B|ODM_RTL8821|ODM_RTL8881A|ODM_RTL8812)
#define ODM_N_ANTDIV_SUPPORT		(ODM_RTL8188E|ODM_RTL8192E|ODM_RTL8723B)
#define ODM_AC_ANTDIV_SUPPORT		(ODM_RTL8821|ODM_RTL8881A|ODM_RTL8812)
#define ODM_SMART_ANT_SUPPORT		(ODM_RTL8188E|ODM_RTL8192E)

#define ODM_OLD_IC_ANTDIV_SUPPORT		(ODM_RTL8723A|ODM_RTL8192C|ODM_RTL8192D)

#define ODM_ANTDIV_2G_SUPPORT_IC			(ODM_RTL8188E|ODM_RTL8192E|ODM_RTL8723B|ODM_RTL8881A)
#define ODM_ANTDIV_5G_SUPPORT_IC			(ODM_RTL8821|ODM_RTL8881A|ODM_RTL8812)

#define ODM_EVM_ENHANCE_ANTDIV_SUPPORT_IC	(ODM_RTL8192E)

#define ODM_ANTDIV_2G	BIT0
#define ODM_ANTDIV_5G	BIT1

#define ANTDIV_ON 1
#define ANTDIV_OFF 0

#define FAT_ON 1
#define FAT_OFF 0

#define TX_BY_DESC 1
#define REG 0

#define RSSI_METHOD 0
#define EVM_METHOD 1
#define CRC32_METHOD 2

#define INIT_ANTDIV_TIMMER 0
#define CANCEL_ANTDIV_TIMMER 1
#define RELEASE_ANTDIV_TIMMER 2

#define CRC32_FAIL 1
#define CRC32_OK 0

#define Evm_RSSI_TH_High 25
#define Evm_RSSI_TH_Low 20

#define NORMAL_STATE_MIAN 1
#define NORMAL_STATE_AUX 2
#define TRAINING_STATE 3

#define FORCE_RSSI_DIFF 10

#define CSI_ON 1
#define CSI_OFF 0

#define DIVON_CSIOFF 1
#define DIVOFF_CSION 2

#define BDC_DIV_TRAIN_STATE 0
#define BDC_BFer_TRAIN_STATE 1
#define BDC_DECISION_STATE 2
#define BDC_BF_HOLD_STATE 3
#define BDC_DIV_HOLD_STATE 4

#define BDC_MODE_1 1
#define BDC_MODE_2 2
#define BDC_MODE_3 3
#define BDC_MODE_4 4
#define BDC_MODE_NULL 0xff

#define SWAW_STEP_PEAK		0
#define SWAW_STEP_DETERMINE	1

//1 ============================================================
//1  structure
//1 ============================================================


typedef struct _SW_Antenna_Switch_
{
	unsigned char		Double_chk_flag;
	unsigned char		try_flag;
	long		PreRSSI;
	unsigned char		CurAntenna;
	unsigned char		PreAntenna;
	unsigned char		RSSI_Trying;
	unsigned char		TestMode;
	unsigned char		bTriggerAntennaSwitch;
	unsigned char		SelectAntennaMap;
	unsigned char		RSSI_target;
	unsigned char 		reset_idx;
	unsigned short		Single_Ant_Counter;
	unsigned short		Dual_Ant_Counter;
	unsigned short          Aux_FailDetec_Counter;
	unsigned short          Retry_Counter;

	// Before link Antenna Switch check
	unsigned char		SWAS_NoLink_State;
	unsigned int		SWAS_NoLink_BK_Reg860;
	unsigned int		SWAS_NoLink_BK_Reg92c;
	unsigned int		SWAS_NoLink_BK_Reg948;
	BOOLEAN		ANTA_ON;	//To indicate Ant A is or not
	BOOLEAN		ANTB_ON;	//To indicate Ant B is on or not
	BOOLEAN		Pre_Aux_FailDetec;
	BOOLEAN		RSSI_AntDect_bResult;
	unsigned char		Ant5G;
	unsigned char		Ant2G;

	long		RSSI_sum_A;
	long		RSSI_sum_B;
	long		RSSI_cnt_A;
	long		RSSI_cnt_B;

	unsigned long long		lastTxOkCnt;
	unsigned long long		lastRxOkCnt;
	unsigned long long 		TXByteCnt_A;
	unsigned long long 		TXByteCnt_B;
	unsigned long long 		RXByteCnt_A;
	unsigned long long 		RXByteCnt_B;
	unsigned char 		TrafficLoad;
	unsigned char		Train_time;
	unsigned char		Train_time_flag;
	RT_TIMER 	SwAntennaSwitchTimer;
#if (RTL8723B_SUPPORT == 1)||(RTL8821A_SUPPORT == 1)
	RT_TIMER 	SwAntennaSwitchTimer_8723B;
	unsigned int		PktCnt_SWAntDivByCtrlFrame;
	BOOLEAN		bSWAntDivByCtrlFrame;
#endif

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	#if USE_WORKITEM
	RT_WORK_ITEM			SwAntennaSwitchWorkitem;
#if (RTL8723B_SUPPORT == 1)||(RTL8821A_SUPPORT == 1)
	RT_WORK_ITEM			SwAntennaSwitchWorkitem_8723B;
	#endif
#endif
#endif
/* CE Platform use
#ifdef CONFIG_SW_ANTENNA_DIVERSITY
	_timer SwAntennaSwitchTimer;
	unsigned long long lastTxOkCnt;
	unsigned long long lastRxOkCnt;
	unsigned long long TXByteCnt_A;
	unsigned long long TXByteCnt_B;
	unsigned long long RXByteCnt_A;
	unsigned long long RXByteCnt_B;
	unsigned char DoubleComfirm;
	unsigned char TrafficLoad;
	//SW Antenna Switch


#endif
*/
#ifdef CONFIG_HW_ANTENNA_DIVERSITY
	//Hybrid Antenna Diversity
	unsigned int		CCK_Ant1_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int		CCK_Ant2_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int		OFDM_Ant1_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int		OFDM_Ant2_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int		RSSI_Ant1_Sum[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int		RSSI_Ant2_Sum[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned char		TxAnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned char		TargetSTA;
	unsigned char		antsel;
	unsigned char		RxIdleAnt;

#endif

}SWAT_T, *pSWAT_T;


#if (DM_ODM_SUPPORT_TYPE & (ODM_AP))
#if (defined(CONFIG_HW_ANTENNA_DIVERSITY))
typedef struct _BF_DIV_COEX_
{
	BOOLEAN w_BFer_Client[ODM_ASSOCIATE_ENTRY_NUM];
	BOOLEAN w_BFee_Client[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	MA_rx_TP[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	MA_rx_TP_DIV[ODM_ASSOCIATE_ENTRY_NUM];

	unsigned char  BDCcoexType_wBfer;
	unsigned char num_Txbfee_Client;
	unsigned char num_Txbfer_Client;
	unsigned char BDC_Try_counter;
	unsigned char BDC_Hold_counter;
	unsigned char BDC_Mode;
	unsigned char BDC_active_Mode;
	unsigned char BDC_state;
	unsigned char BDC_RxIdleUpdate_counter;
	unsigned char num_Client;
	unsigned char pre_num_Client;
	unsigned char num_BfTar;
	unsigned char num_DivTar;

	BOOLEAN bAll_DivSta_Idle;
	BOOLEAN bAll_BFSta_Idle;
	BOOLEAN BDC_Try_flag;
	BOOLEAN BF_pass;
	BOOLEAN DIV_pass;
}BDC_T,*pBDC_T;
#endif
#endif


typedef struct _FAST_ANTENNA_TRAINNING_
{
	unsigned char	Bssid[6];
	unsigned char	antsel_rx_keep_0;
	unsigned char	antsel_rx_keep_1;
	unsigned char	antsel_rx_keep_2;
	unsigned char	antsel_rx_keep_3;
	unsigned int	antSumRSSI[7];
	unsigned int	antRSSIcnt[7];
	unsigned int	antAveRSSI[7];
	unsigned char	FAT_State;
	unsigned int	TrainIdx;
	unsigned char	antsel_a[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned char	antsel_b[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned char	antsel_c[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	MainAnt_Sum[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	AuxAnt_Sum[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	MainAnt_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	AuxAnt_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned char	RxIdleAnt;
	unsigned char	AntDiv_OnOff;
	BOOLEAN	bBecomeLinked;
	unsigned int	MinMaxRSSI;
	unsigned char	idx_AntDiv_counter_2G;
	unsigned char	idx_AntDiv_counter_5G;
	unsigned char	AntDiv_2G_5G;
	unsigned int    CCK_counter_main;
	unsigned int    CCK_counter_aux;
	unsigned int    OFDM_counter_main;
	unsigned int    OFDM_counter_aux;

	#ifdef ODM_EVM_ENHANCE_ANTDIV
	unsigned int	MainAntEVM_Sum[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	AuxAntEVM_Sum[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	MainAntEVM_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned int	AuxAntEVM_Cnt[ODM_ASSOCIATE_ENTRY_NUM];
	BOOLEAN	EVM_method_enable;
	unsigned char	TargetAnt_EVM;
	unsigned char	TargetAnt_CRC32;
	unsigned char	TargetAnt_enhance;
	unsigned char	pre_TargetAnt_enhance;
	unsigned short	Main_MPDU_OK_cnt;
	unsigned short	Aux_MPDU_OK_cnt;

	unsigned int	CRC32_Ok_Cnt;
	unsigned int	CRC32_Fail_Cnt;
	unsigned int	MainCRC32_Ok_Cnt;
	unsigned int	AuxCRC32_Ok_Cnt;
	unsigned int	MainCRC32_Fail_Cnt;
	unsigned int	AuxCRC32_Fail_Cnt;
	#endif
	#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
	unsigned int    CCK_CtrlFrame_Cnt_main;
	unsigned int    CCK_CtrlFrame_Cnt_aux;
	unsigned int    OFDM_CtrlFrame_Cnt_main;
	unsigned int    OFDM_CtrlFrame_Cnt_aux;
	unsigned int	MainAnt_CtrlFrame_Sum;
	unsigned int	AuxAnt_CtrlFrame_Sum;
	unsigned int	MainAnt_CtrlFrame_Cnt;
	unsigned int	AuxAnt_CtrlFrame_Cnt;
	#endif
	BOOLEAN	fix_ant_bfee;
	BOOLEAN	enable_ctrl_frame_antdiv;
	BOOLEAN	use_ctrl_frame_antdiv;
}FAT_T,*pFAT_T;


//1 ============================================================
//1  enumeration
//1 ============================================================



typedef enum _FAT_STATE
{
	FAT_NORMAL_STATE			= 0,
	FAT_TRAINING_STATE 		= 1,
}FAT_STATE_E, *PFAT_STATE_E;


typedef enum _ANT_DIV_TYPE
{
	NO_ANTDIV			= 0xFF,
	CG_TRX_HW_ANTDIV		= 0x01,
	CGCS_RX_HW_ANTDIV 	= 0x02,
	FIXED_HW_ANTDIV		= 0x03,
	CG_TRX_SMART_ANTDIV	= 0x04,
	CGCS_RX_SW_ANTDIV	= 0x05,
	S0S1_SW_ANTDIV          = 0x06 //8723B intrnal switch S0 S1
}ANT_DIV_TYPE_E, *PANT_DIV_TYPE_E;


//1 ============================================================
//1  function prototype
//1 ============================================================


VOID
ODM_StopAntennaSwitchDm(
	IN	PVOID	pDM_VOID
	);
VOID
ODM_SetAntConfig(
	IN	PVOID	pDM_VOID,
	IN	unsigned char		antSetting	// 0=A, 1=B, 2=C, ....
	);


#define SwAntDivRestAfterLink	ODM_SwAntDivRestAfterLink
VOID ODM_SwAntDivRestAfterLink(
	IN	PVOID	pDM_VOID
	);

#if (defined(CONFIG_HW_ANTENNA_DIVERSITY))

VOID
ODM_UpdateRxIdleAnt(
	IN		PVOID		pDM_VOID,
	IN		 unsigned char		Ant
);

#if (RTL8723B_SUPPORT == 1)||(RTL8821A_SUPPORT == 1)
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
VOID
ODM_SW_AntDiv_Callback(
	IN 	PRT_TIMER		pTimer
	);

VOID
ODM_SW_AntDiv_WorkitemCallback(
	IN 		PVOID            pContext
	);


#elif (DM_ODM_SUPPORT_TYPE == ODM_CE)

VOID
ODM_SW_AntDiv_WorkitemCallback(
	IN PVOID	pContext
);

VOID
ODM_SW_AntDiv_Callback(
	void 		*FunctionContext
	);

#endif

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
VOID
odm_S0S1_SwAntDivByCtrlFrame(
	IN		PVOID			pDM_VOID,
	IN		unsigned char			Step
);

VOID
odm_AntselStatisticsOfCtrlFrame(
	IN		PVOID			pDM_VOID,
	IN		unsigned char			antsel_tr_mux,
	IN		unsigned int			RxPWDBAll
);

VOID
odm_S0S1_SwAntDivByCtrlFrame_ProcessRSSI(
	IN		PVOID				pDM_VOID,
	IN		PVOID		p_phy_info_void,
	IN		PVOID		p_pkt_info_void
);

/*
VOID
odm_S0S1_SwAntDivByCtrlFrame_ProcessRSSI(
	IN		PVOID				pDM_VOID,
	IN		PVOID		p_phy_info_void,
	IN		PVOID		p_pkt_info_void
);
*/

#endif
#endif

#ifdef ODM_EVM_ENHANCE_ANTDIV
VOID
odm_EVM_FastAntTrainingCallback(
	IN		PVOID		pDM_VOID
);
#endif

VOID
odm_HW_AntDiv(
	IN		PVOID		pDM_VOID
);

#if( defined(CONFIG_5G_CG_SMART_ANT_DIVERSITY) ) ||( defined(CONFIG_2G_CG_SMART_ANT_DIVERSITY) )
VOID
odm_FastAntTraining(
	IN		PVOID		pDM_VOID
);

VOID
odm_FastAntTrainingCallback(
	IN		PVOID		pDM_VOID
);

VOID
odm_FastAntTrainingWorkItemCallback(
	IN		PVOID		pDM_VOID
);
#endif


VOID
ODM_AntDivInit(
	IN		PVOID		pDM_VOID
);

VOID
ODM_AntDiv(
	IN		PVOID		pDM_VOID
);

VOID
odm_AntselStatistics(
	IN		PVOID			pDM_VOID,
	IN		unsigned char			antsel_tr_mux,
	IN		unsigned int			MacId,
	IN		unsigned int			utility,
	IN            unsigned char			method
);
/*
VOID
ODM_Process_RSSIForAntDiv(
	IN OUT	PVOID		pDM_VOID,
	IN		PVOID		p_phy_info_void,
	IN		PVOID		p_pkt_info_void
);
*/


VOID
ODM_Process_RSSIForAntDiv(
	IN OUT	PVOID		pDM_VOID,
	IN		PVOID		p_phy_info_void,
	IN		PVOID		p_pkt_info_void
);



#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
VOID
ODM_SetTxAntByTxInfo(
	IN		PVOID			pDM_VOID,
	IN		punsigned char			pDesc,
	IN		unsigned char			macId
);

#elif(DM_ODM_SUPPORT_TYPE == ODM_AP)

VOID
ODM_SetTxAntByTxInfo(
	struct	rtl8192cd_priv		*priv,
	struct 	tx_desc			*pdesc,
	unsigned short			aid
);

#endif


VOID
ODM_AntDiv_Config(
	IN		PVOID		pDM_VOID
);


VOID
ODM_UpdateRxIdleAnt_8723B(
	IN		PVOID			pDM_VOID,
	IN		unsigned char			Ant,
	IN		unsigned int			DefaultAnt,
	IN		unsigned int			OptionalAnt
);

VOID
ODM_AntDivTimers(
	IN		PVOID		pDM_VOID,
	IN 		unsigned char		state
);

#endif //#if (defined(CONFIG_HW_ANTENNA_DIVERSITY))

VOID
ODM_AntDivReset(
	IN		PVOID		pDM_VOID
);

VOID
odm_AntennaDiversityInit(
	IN		PVOID		pDM_VOID
);

VOID
odm_AntennaDiversity(
	IN		PVOID		pDM_VOID
);


#endif //#ifndef	__ODMANTDIV_H__
