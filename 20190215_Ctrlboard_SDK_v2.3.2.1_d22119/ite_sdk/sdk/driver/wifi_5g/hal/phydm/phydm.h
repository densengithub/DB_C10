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


#ifndef	__HALDMOUTSRC_H__
#define __HALDMOUTSRC_H__

//============================================================
// include files
//============================================================
#include "phydm_pre_define.h"
#include "phydm_dig.h"
#include "phydm_edcaturbocheck.h"
#include "phydm_pathdiv.h"
#include "phydm_antdiv.h"
#include "phydm_antdect.h"
#include "phydm_dynamicbbpowersaving.h"
#include "phydm_rainfo.h"
#include "phydm_dynamictxpower.h"
#include "phydm_cfotracking.h"
#include "phydm_acs.h"
#include "phydm_adaptivity.h"
//#include "phydm_types.h"


#if (RTL8814A_SUPPORT == 1)
#include "rtl8814a/phydm_iqk_8814a.h"
#endif


#if (DM_ODM_SUPPORT_TYPE & (ODM_AP))
#include "halphyrf_ap.h"
#include "phydm_powertracking_ap.h"
#endif

#if (DM_ODM_SUPPORT_TYPE & (ODM_CE))
#include "phydm_noisemonitor.h"
#include "halphyrf_ce.h"
#include "phydm_powertracking_ce.h"
#endif

#if(DM_ODM_SUPPORT_TYPE & (ODM_WIN))
#include "phydm_beamforming.h"
#include "phydm_rxhp.h"
#include "halphyrf_win.h"
#include "phydm_powertracking_win.h"
#endif

//============================================================
// Definition
//============================================================
//
// 2011/09/22 MH Define all team supprt ability.
//

//
// 2011/09/22 MH Define for all teams. Please Define the constan in your precomp header.
//
//#define		DM_ODM_SUPPORT_AP			0
//#define		DM_ODM_SUPPORT_ADSL			0
//#define		DM_ODM_SUPPORT_CE			0
//#define		DM_ODM_SUPPORT_MP			1

//
// 2011/09/28 MH Define ODM SW team support flag.
//

//For SW AntDiv, PathDiv, 8192C AntDiv joint use
#define	TP_MODE		0
#define	RSSI_MODE		1

#define	TRAFFIC_LOW	0
#define	TRAFFIC_HIGH	1
#define	TRAFFIC_UltraLOW	2

#define	NONE			0




//8723A High Power IGI Setting
#define		DM_DIG_HIGH_PWR_IGI_LOWER_BOUND	0x22
#define  		DM_DIG_Gmode_HIGH_PWR_IGI_LOWER_BOUND 0x28
#define		DM_DIG_HIGH_PWR_THRESHOLD	0x3a
#define		DM_DIG_LOW_PWR_THRESHOLD	0x14


//============================================================
// structure and define
//============================================================

//
// 2011/09/20 MH Add for AP/ADSLpseudo DM structuer requirement.
// We need to remove to other position???
//
#if(DM_ODM_SUPPORT_TYPE & (ODM_CE|ODM_WIN))
typedef		struct rtl8192cd_priv {
	unsigned char		temp;

}rtl8192cd_priv, *prtl8192cd_priv;
#endif


#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
typedef		struct _ADAPTER{
	unsigned char		temp;
	#ifdef AP_BUILD_WORKAROUND
	HAL_DATA_TYPE*		temp2;
	prtl8192cd_priv		priv;
	#endif
}ADAPTER, *PADAPTER;
#endif

#if (DM_ODM_SUPPORT_TYPE == ODM_AP)

typedef		struct _WLAN_STA{
	unsigned char		temp;
} WLAN_STA, *PRT_WLAN_STA;

#endif

typedef struct _Dynamic_Primary_CCA{
	unsigned char		PriCCA_flag;
	unsigned char		intf_flag;
	unsigned char		intf_type;
	unsigned char		DupRTS_flag;
	unsigned char		Monitor_flag;
	unsigned char		CH_offset;
	unsigned char  	MF_state;
}Pri_CCA_T, *pPri_CCA_T;


#if (DM_ODM_SUPPORT_TYPE & ODM_AP)


#ifdef ADSL_AP_BUILD_WORKAROUND
#define MAX_TOLERANCE			5
#define IQK_DELAY_TIME			1		//ms
#endif
#if 0//defined in 8192cd.h
//
// Indicate different AP vendor for IOT issue.
//
typedef enum _HT_IOT_PEER
{
	HT_IOT_PEER_UNKNOWN 			= 0,
	HT_IOT_PEER_REALTEK 			= 1,
	HT_IOT_PEER_REALTEK_92SE 		= 2,
	HT_IOT_PEER_BROADCOM 		= 3,
	HT_IOT_PEER_RALINK 			= 4,
	HT_IOT_PEER_ATHEROS 			= 5,
	HT_IOT_PEER_CISCO 				= 6,
	HT_IOT_PEER_MERU 				= 7,
	HT_IOT_PEER_MARVELL 			= 8,
	HT_IOT_PEER_REALTEK_SOFTAP 	= 9,// peer is RealTek SOFT_AP, by Bohn, 2009.12.17
	HT_IOT_PEER_SELF_SOFTAP 		= 10, // Self is SoftAP
	HT_IOT_PEER_AIRGO 				= 11,
	HT_IOT_PEER_INTEL 				= 12,
	HT_IOT_PEER_RTK_APCLIENT 		= 13,
	HT_IOT_PEER_REALTEK_81XX 		= 14,
	HT_IOT_PEER_REALTEK_WOW 		= 15,
	HT_IOT_PEER_MAX 				= 16
}HT_IOT_PEER_E, *PHTIOT_PEER_E;
#endif
#endif//#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))

#define		DM_Type_ByFW			0
#define		DM_Type_ByDriver		1

//
// Declare for common info
//

#define IQK_THRESHOLD			8
#define DPK_THRESHOLD			4


#if (DM_ODM_SUPPORT_TYPE &  (ODM_AP))
__PACK typedef struct _ODM_Phy_Status_Info_
{
	unsigned char		RxPWDBAll;
	unsigned char		SignalQuality;					/* in 0-100 index. */
	unsigned char		RxMIMOSignalStrength[4];		/* in 0~100 index */
	signed char		RxMIMOSignalQuality[4];		/* EVM */
	signed char		RxSNR[4];					/* per-path's SNR */
#if (RTL8822B_SUPPORT == 1)
	unsigned char		RxCount;						/* RX path counter---*/
#endif
	unsigned char		BandWidth;

} __WLAN_ATTRIB_PACK__ ODM_PHY_INFO_T, *PODM_PHY_INFO_T;

typedef struct _ODM_Phy_Status_Info_Append_
{
	unsigned char		MAC_CRC32;

}ODM_PHY_INFO_Append_T,*PODM_PHY_INFO_Append_T;

#else

typedef struct _ODM_Phy_Status_Info_
{
	//
	// Be care, if you want to add any element please insert between
	// RxPWDBAll & SignalStrength.
	//
#if (DM_ODM_SUPPORT_TYPE &  (ODM_WIN))
	unsigned int		RxPWDBAll;
#else
	unsigned char		RxPWDBAll;
#endif
	unsigned char		SignalQuality;				/* in 0-100 index. */
	signed char		RxMIMOSignalQuality[4];		/* per-path's EVM */
	unsigned char		RxMIMOEVMdbm[4];			/* per-path's EVM dbm */
	unsigned char		RxMIMOSignalStrength[4];	/* in 0~100 index */
	short		Cfo_short[4];				/* per-path's Cfo_short */
	short		Cfo_tail[4];					/* per-path's Cfo_tail */
	signed char		RxPower;					/* in dBm Translate from PWdB */
	signed char		RecvSignalPower;			/* Real power in dBm for this packet, no beautification and aggregation. Keep this raw info to be used for the other procedures. */
	unsigned char		BTRxRSSIPercentage;
	unsigned char		SignalStrength;				/* in 0-100 index. */
	signed char		RxPwr[4];					/* per-path's pwdb */
	signed char		RxSNR[4];					/* per-path's SNR	*/
#if (RTL8822B_SUPPORT == 1)
	unsigned char		RxCount:2;					/* RX path counter---*/
	unsigned char		BandWidth:2;
	unsigned char		rxsc:4;						/* sub-channel---*/
#else
	unsigned char		BandWidth;
#endif
	unsigned char		btCoexPwrAdjust;
#if (RTL8822B_SUPPORT == 1)
	unsigned char		channel;						/* channel number---*/
	BOOLEAN		bMuPacket;					/* is MU packet or not---*/
	BOOLEAN		bBeamformed;				/* BF packet---*/
#endif
}ODM_PHY_INFO_T,*PODM_PHY_INFO_T;
#endif

typedef struct _ODM_Per_Pkt_Info_
{
	//unsigned char		Rate;
	unsigned char		DataRate;
	unsigned char		StationID;
	BOOLEAN		bPacketMatchBSSID;
	BOOLEAN		bPacketToSelf;
	BOOLEAN		bPacketBeacon;
	BOOLEAN		bToSelf;
}ODM_PACKET_INFO_T,*PODM_PACKET_INFO_T;


typedef struct _ODM_Phy_Dbg_Info_
{
	//ODM Write,debug info
	signed char		RxSNRdB[4];
	unsigned int		NumQryPhyStatus;
	unsigned int		NumQryPhyStatusCCK;
	unsigned int		NumQryPhyStatusOFDM;
#if (RTL8822B_SUPPORT == 1)
	unsigned int		NumQryMuPkt;
	unsigned int		NumQryBfPkt;
#endif
	unsigned char		NumQryBeaconPkt;
	//Others
	s4Byte		RxEVM[4];

}ODM_PHY_DBG_INFO_T;


typedef struct _ODM_Mac_Status_Info_
{
	unsigned char	test;

}ODM_MAC_INFO;

//
// 2011/20/20 MH For MP driver RT_WLAN_STA =  STA_INFO_T
// Please declare below ODM relative info in your STA info structure.
//
#if 1
typedef		struct _ODM_STA_INFO{
	// Driver Write
	BOOLEAN		bUsed;				// record the sta status link or not?
	//unsigned char		WirelessMode;		//
	unsigned char		IOTPeer;			// Enum value.	HT_IOT_PEER_E

	// ODM Write
	//1 PHY_STATUS_INFO
	unsigned char		RSSI_Path[4];		//
	unsigned char		RSSI_Ave;
	unsigned char		RXEVM[4];
	unsigned char		RXSNR[4];

	// ODM Write
	//1 TX_INFO (may changed by IC)
	//TX_INFO_T		pTxInfo;				// Define in IC folder. Move lower layer.
#if 0
	unsigned char		ANTSEL_A;			//in Jagar: 4bit; others: 2bit
	unsigned char		ANTSEL_B;			//in Jagar: 4bit; others: 2bit
	unsigned char		ANTSEL_C;			//only in Jagar: 4bit
	unsigned char		ANTSEL_D;			//only in Jagar: 4bit
	unsigned char		TX_ANTL;			//not in Jagar: 2bit
	unsigned char		TX_ANT_HT;			//not in Jagar: 2bit
	unsigned char		TX_ANT_CCK;			//not in Jagar: 2bit
	unsigned char		TXAGC_A;			//not in Jagar: 4bit
	unsigned char		TXAGC_B;			//not in Jagar: 4bit
	unsigned char		TXPWR_OFFSET;		//only in Jagar: 3bit
	unsigned char		TX_ANT;				//only in Jagar: 4bit for TX_ANTL/TX_ANTHT/TX_ANT_CCK
#endif

	//
	// 	Please use compile flag to disabe the strcutrue for other IC except 88E.
	//	Move To lower layer.
	//
	// ODM Write Wilson will handle this part(said by Luke.Lee)
	//TX_RPT_T		pTxRpt;				// Define in IC folder. Move lower layer.
#if 0
	//1 For 88E RA (don't redefine the naming)
	unsigned char		rate_id;
	unsigned char		rate_SGI;
	unsigned char		rssi_sta_ra;
	unsigned char		SGI_enable;
	unsigned char		Decision_rate;
	unsigned char		Pre_rate;
	unsigned char		Active;

	// Driver write Wilson handle.
	//1 TX_RPT (don't redefine the naming)
	unsigned short		RTY[4];				// ???
	unsigned short		TOTAL;				// ???
	unsigned short		DROP;				// ???
	//
	// Please use compile flag to disabe the strcutrue for other IC except 88E.
	//
#endif

}ODM_STA_INFO_T, *PODM_STA_INFO_T;
#endif

//
// 2011/10/20 MH Define Common info enum for all team.
//
typedef enum _ODM_Common_Info_Definition
{
//-------------REMOVED CASE-----------//
	//ODM_CMNINFO_CCK_HP,
	//ODM_CMNINFO_RFPATH_ENABLE,		// Define as ODM write???
	//ODM_CMNINFO_BT_COEXIST,				// ODM_BT_COEXIST_E
	//ODM_CMNINFO_OP_MODE,				// ODM_OPERATION_MODE_E
//-------------REMOVED CASE-----------//

	//
	// Fixed value:
	//

	//-----------HOOK BEFORE REG INIT-----------//
	ODM_CMNINFO_PLATFORM = 0,
	ODM_CMNINFO_ABILITY,					// ODM_ABILITY_E
	ODM_CMNINFO_INTERFACE,				// ODM_INTERFACE_E
	ODM_CMNINFO_MP_TEST_CHIP,
	ODM_CMNINFO_IC_TYPE,					// ODM_IC_TYPE_E
	ODM_CMNINFO_CUT_VER,					// ODM_CUT_VERSION_E
	ODM_CMNINFO_FAB_VER,					// ODM_FAB_E
	ODM_CMNINFO_RF_TYPE,					// ODM_RF_PATH_E or ODM_RF_TYPE_E?
	ODM_CMNINFO_RFE_TYPE,
	ODM_CMNINFO_BOARD_TYPE,				// ODM_BOARD_TYPE_E
	ODM_CMNINFO_PACKAGE_TYPE,
	ODM_CMNINFO_EXT_LNA,					// TRUE
	ODM_CMNINFO_5G_EXT_LNA,
	ODM_CMNINFO_EXT_PA,
	ODM_CMNINFO_5G_EXT_PA,
	ODM_CMNINFO_GPA,
	ODM_CMNINFO_APA,
	ODM_CMNINFO_GLNA,
	ODM_CMNINFO_ALNA,
	ODM_CMNINFO_EXT_TRSW,
	ODM_CMNINFO_EXT_LNA_GAIN,
	ODM_CMNINFO_PATCH_ID,				//CUSTOMER ID
	ODM_CMNINFO_BINHCT_TEST,
	ODM_CMNINFO_BWIFI_TEST,
	ODM_CMNINFO_SMART_CONCURRENT,
	ODM_CMNINFO_CONFIG_BB_RF,
	ODM_CMNINFO_DOMAIN_CODE_2G,
	ODM_CMNINFO_DOMAIN_CODE_5G,
	ODM_CMNINFO_IQKFWOFFLOAD,
	//-----------HOOK BEFORE REG INIT-----------//


	//
	// Dynamic value:
	//
//--------- POINTER REFERENCE-----------//
	ODM_CMNINFO_MAC_PHY_MODE,			// ODM_MAC_PHY_MODE_E
	ODM_CMNINFO_TX_UNI,
	ODM_CMNINFO_RX_UNI,
	ODM_CMNINFO_WM_MODE,				// ODM_WIRELESS_MODE_E
	ODM_CMNINFO_BAND,					// ODM_BAND_TYPE_E
	ODM_CMNINFO_SEC_CHNL_OFFSET,		// ODM_SEC_CHNL_OFFSET_E
	ODM_CMNINFO_SEC_MODE,				// ODM_SECURITY_E
	ODM_CMNINFO_BW,						// ODM_BW_E
	ODM_CMNINFO_CHNL,
	ODM_CMNINFO_FORCED_RATE,

	ODM_CMNINFO_DMSP_GET_VALUE,
	ODM_CMNINFO_BUDDY_ADAPTOR,
	ODM_CMNINFO_DMSP_IS_MASTER,
	ODM_CMNINFO_SCAN,
	ODM_CMNINFO_POWER_SAVING,
	ODM_CMNINFO_ONE_PATH_CCA,			// ODM_CCA_PATH_E
	ODM_CMNINFO_DRV_STOP,
	ODM_CMNINFO_PNP_IN,
	ODM_CMNINFO_INIT_ON,
	ODM_CMNINFO_ANT_TEST,
	ODM_CMNINFO_NET_CLOSED,
	//ODM_CMNINFO_RTSTA_AID,				// For win driver only?
	ODM_CMNINFO_FORCED_IGI_LB,
	ODM_CMNINFO_P2P_LINK,
	ODM_CMNINFO_FCS_MODE,
	ODM_CMNINFO_IS1ANTENNA,
	ODM_CMNINFO_RFDEFAULTPATH,
//--------- POINTER REFERENCE-----------//

//------------CALL BY VALUE-------------//
	ODM_CMNINFO_WIFI_DIRECT,
	ODM_CMNINFO_WIFI_DISPLAY,
	ODM_CMNINFO_LINK_IN_PROGRESS,
	ODM_CMNINFO_LINK,
	ODM_CMNINFO_STATION_STATE,
	ODM_CMNINFO_RSSI_MIN,
	ODM_CMNINFO_DBG_COMP,				// unsigned long long
	ODM_CMNINFO_DBG_LEVEL,				// unsigned int
	ODM_CMNINFO_RA_THRESHOLD_HIGH,		// unsigned char
	ODM_CMNINFO_RA_THRESHOLD_LOW,		// unsigned char
	ODM_CMNINFO_RF_ANTENNA_TYPE,		// unsigned char
	ODM_CMNINFO_BT_ENABLED,
	ODM_CMNINFO_BT_HS_CONNECT_PROCESS,
	ODM_CMNINFO_BT_HS_RSSI,
	ODM_CMNINFO_BT_OPERATION,
	ODM_CMNINFO_BT_LIMITED_DIG,					//Need to Limited Dig or not
	ODM_CMNINFO_BT_DIG,
	ODM_CMNINFO_BT_BUSY,					//Check Bt is using or not//neil
	ODM_CMNINFO_BT_DISABLE_EDCA,
#if(DM_ODM_SUPPORT_TYPE & ODM_AP)		// for repeater mode add by YuChen 2014.06.23
#ifdef UNIVERSAL_REPEATER
	ODM_CMNINFO_VXD_LINK,
#endif
#endif
	ODM_CMNINFO_AP_TOTAL_NUM,
//------------CALL BY VALUE-------------//

	//
	// Dynamic ptr array hook itms.
	//
	ODM_CMNINFO_STA_STATUS,
	ODM_CMNINFO_PHY_STATUS,
	ODM_CMNINFO_MAC_STATUS,

	ODM_CMNINFO_MAX,


}ODM_CMNINFO_E;

//
// 2011/10/20 MH Define ODM support ability.  ODM_CMNINFO_ABILITY
//
typedef enum _ODM_Support_Ability_Definition
{
	//
	// BB ODM section BIT 0-19
	//
	ODM_BB_DIG					= BIT0,
	ODM_BB_RA_MASK				= BIT1,
	ODM_BB_DYNAMIC_TXPWR		= BIT2,
	ODM_BB_FA_CNT					= BIT3,
	ODM_BB_RSSI_MONITOR			= BIT4,
	ODM_BB_CCK_PD				= BIT5,
	ODM_BB_ANT_DIV				= BIT6,
	ODM_BB_PWR_SAVE				= BIT7,
	ODM_BB_PWR_TRAIN				= BIT8,
	ODM_BB_RATE_ADAPTIVE			= BIT9,
	ODM_BB_PATH_DIV				= BIT10,
	ODM_BB_PSD					= BIT11,
	ODM_BB_RXHP					= BIT12,
	ODM_BB_ADAPTIVITY				= BIT13,
	ODM_BB_CFO_TRACKING			= BIT14,
	ODM_BB_NHM_CNT				= BIT15,
	ODM_BB_PRIMARY_CCA			= BIT16,
	ODM_BB_TXBF				= BIT17,

	//
	// MAC DM section BIT 20-23
	//
	ODM_MAC_EDCA_TURBO			= BIT20,
	ODM_MAC_EARLY_MODE			= BIT21,

	//
	// RF ODM section BIT 24-31
	//
	ODM_RF_TX_PWR_TRACK			= BIT24,
	ODM_RF_RX_GAIN_TRACK			= BIT25,
	ODM_RF_CALIBRATION			= BIT26,

}ODM_ABILITY_E;

//Move some non-DM enum,define, struc. form phydm.h to phydm_types.h by Dino

// ODM_CMNINFO_ONE_PATH_CCA
typedef enum tag_CCA_Path
{
	ODM_CCA_2R			= 0,
	ODM_CCA_1R_A		= 1,
	ODM_CCA_1R_B		= 2,
}ODM_CCA_PATH_E;

//move RAInfo to Phydm_RaInfo.h

//Remove struct  PATHDIV_PARA to odm_PathDiv.h

//Remove struct to odm_PowerTracking.h by YuChen
//
// ODM Dynamic common info value definition
//
//Move AntDiv form phydm.h to Phydm_AntDiv.h by Dino

//move PathDiv to Phydm_PathDiv.h

typedef enum _BASEBAND_CONFIG_PHY_REG_PG_VALUE_TYPE{
	PHY_REG_PG_RELATIVE_VALUE = 0,
	PHY_REG_PG_EXACT_VALUE = 1
} PHY_REG_PG_TYPE;

//
// 2011/09/22 MH Copy from SD4 defined structure. We use to support PHY DM integration.
//
#if(DM_ODM_SUPPORT_TYPE & ODM_WIN)
#if (RT_PLATFORM != PLATFORM_LINUX)
typedef
#endif

struct DM_Out_Source_Dynamic_Mechanism_Structure
#else// for AP,ADSL,CE Team
typedef  struct DM_Out_Source_Dynamic_Mechanism_Structure
#endif
{
	//RT_TIMER 	FastAntTrainingTimer;
	//
	//	Add for different team use temporarily
	//
	PADAPTER		Adapter;		// For CE/NIC team
	prtl8192cd_priv	priv;			// For AP/ADSL team
	// WHen you use Adapter or priv pointer, you must make sure the pointer is ready.
	BOOLEAN			odm_ready;

#if(DM_ODM_SUPPORT_TYPE & (ODM_CE|ODM_WIN))
	rtl8192cd_priv		fake_priv;
#endif
#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	// ADSL_AP_BUILD_WORKAROUND
	ADAPTER			fake_adapter;
#endif

	PHY_REG_PG_TYPE		PhyRegPgValueType;
	unsigned char				PhyRegPgVersion;

	unsigned long long			DebugComponents;
	unsigned int			DebugLevel;

	unsigned int			NumQryPhyStatusAll; 	//CCK + OFDM
	unsigned int			LastNumQryPhyStatusAll;
	unsigned int			RxPWDBAve;
	BOOLEAN			MPDIG_2G; 		//off MPDIG
	unsigned char			Times_2G;

//------ ODM HANDLE, DRIVER NEEDS NOT TO HOOK------//
	BOOLEAN			bCckHighPower;
	unsigned char			RFPathRxEnable;		// ODM_CMNINFO_RFPATH_ENABLE
	unsigned char			ControlChannel;
//------ ODM HANDLE, DRIVER NEEDS NOT TO HOOK------//

//--------REMOVED COMMON INFO----------//
	//unsigned char				PseudoMacPhyMode;
	//BOOLEAN			*BTCoexist;
	//BOOLEAN			PseudoBtCoexist;
	//unsigned char				OPMode;
	//BOOLEAN			bAPMode;
	//BOOLEAN			bClientMode;
	//BOOLEAN			bAdHocMode;
	//BOOLEAN			bSlaveOfDMSP;
//--------REMOVED COMMON INFO----------//


//1  COMMON INFORMATION

	//
	// Init Value
	//
//-----------HOOK BEFORE REG INIT-----------//
    BOOLEAN         NoisyDecision;
    unsigned int          NoisyDecision_Smooth;
	// ODM Platform info AP/ADSL/CE/MP = 1/2/3/4
	unsigned char			SupportPlatform;
	// ODM Support Ability DIG/RATR/TX_PWR_TRACK/ ¡K¡K = 1/2/3/¡K
	unsigned int			SupportAbility;
	// ODM PCIE/USB/SDIO = 1/2/3
	unsigned char			SupportInterface;
	// ODM composite or independent. Bit oriented/ 92C+92D+ .... or any other type = 1/2/3/...
	unsigned int			SupportICType;
	// Cut Version TestChip/A-cut/B-cut... = 0/1/2/3/...
	unsigned char			CutVersion;
	// Fab Version TSMC/UMC = 0/1
	unsigned char			FabVersion;
	// RF Type 4T4R/3T3R/2T2R/1T2R/1T1R/...
	unsigned char			RFType;
	unsigned char			RFEType;
	// Board Type Normal/HighPower/MiniCard/SLIM/Combo/... = 0/1/2/3/4/...
	unsigned char			BoardType;
	unsigned char			PackageType;
	unsigned char			TypeGLNA;
	unsigned char			TypeGPA;
	unsigned char			TypeALNA;
	unsigned char			TypeAPA;
	// with external LNA  NO/Yes = 0/1
	unsigned char			ExtLNA; // 2G
	unsigned char			ExtLNA5G; //5G
	// with external PA  NO/Yes = 0/1
	unsigned char			ExtPA; // 2G
	unsigned char			ExtPA5G; //5G
	// with external TRSW  NO/Yes = 0/1
	unsigned char			ExtTRSW;
	unsigned char			ExtLNAGain; // 2G
	unsigned char			PatchID; //Customer ID
	BOOLEAN			bInHctTest;
	BOOLEAN			bWIFITest;

	BOOLEAN			bDualMacSmartConcurrent;
	unsigned int			BK_SupportAbility;
	unsigned char			AntDivType;
	BOOLEAN			ConfigBBRF;
	unsigned char			odm_Regulation2_4G;
	unsigned char			odm_Regulation5G;
	unsigned char			IQKFWOffload;
//-----------HOOK BEFORE REG INIT-----------//

	//
	// Dynamic Value
	//
//--------- POINTER REFERENCE-----------//

	unsigned char	char_temp;
	BOOLEAN			BOOLEAN_temp;
	PADAPTER		PADAPTER_temp;

	// MAC PHY Mode SMSP/DMSP/DMDP = 0/1/2
	unsigned char			*pMacPhyMode;
	//TX Unicast byte count
	unsigned long long			*pNumTxBytesUnicast;
	//RX Unicast byte count
	unsigned long long			*pNumRxBytesUnicast;
	// Wireless mode B/G/A/N = BIT0/BIT1/BIT2/BIT3
	unsigned char			*pWirelessMode; //ODM_WIRELESS_MODE_E
	// Frequence band 2.4G/5G = 0/1
	unsigned char			*pBandType;
	// Secondary channel offset don't_care/below/above = 0/1/2
	unsigned char			*pSecChOffset;
	// Security mode Open/WEP/AES/TKIP = 0/1/2/3
	unsigned char			*pSecurity;
	// BW info 20M/40M/80M = 0/1/2
	unsigned char			*pBandWidth;
 	// Central channel location Ch1/Ch2/....
	unsigned char			*pChannel;	//central channel number
	BOOLEAN			DPK_Done;
	// Common info for 92D DMSP

	BOOLEAN			*pbGetValueFromOtherMac;
	PADAPTER		*pBuddyAdapter;
	BOOLEAN			*pbMasterOfDMSP; //MAC0: master, MAC1: slave
	// Common info for Status
	BOOLEAN			*pbScanInProcess;
	BOOLEAN			*pbPowerSaving;
	// CCA Path 2-path/path-A/path-B = 0/1/2; using ODM_CCA_PATH_E.
	unsigned char			*pOnePathCCA;
	//pMgntInfo->AntennaTest
	unsigned char			*pAntennaTest;
	BOOLEAN			*pbNet_closed;
	//unsigned char			*pAidMap;
	unsigned char			*pu1ForcedIgiLb;
	BOOLEAN			*pIsFcsModeEnable;
/*--------- For 8723B IQK-----------*/
	BOOLEAN			*pIs1Antenna;
	unsigned char			*pRFDefaultPath;
	// 0:S1, 1:S0

//--------- POINTER REFERENCE-----------//
	unsigned short			pForcedDataRate;
//------------CALL BY VALUE-------------//
	BOOLEAN			bLinkInProcess;
	BOOLEAN			bWIFI_Direct;
	BOOLEAN			bWIFI_Display;
	BOOLEAN			bLinked;
	BOOLEAN			bsta_state;
#if(DM_ODM_SUPPORT_TYPE & ODM_AP)		// for repeater mode add by YuChen 2014.06.23
#ifdef UNIVERSAL_REPEATER
	BOOLEAN			VXD_bLinked;
#endif
#endif									// for repeater mode add by YuChen 2014.06.23
	unsigned char			RSSI_Min;
	unsigned char			InterfaceIndex; /*Add for 92D  dual MAC: 0--Mac0 1--Mac1*/
	BOOLEAN			bIsMPChip;
	BOOLEAN			bOneEntryOnly;
	BOOLEAN			mp_mode;
	unsigned int			OneEntry_MACID;
	unsigned char			pre_number_linked_client;
	unsigned char			number_linked_client;
	unsigned char			pre_number_active_client;
	unsigned char			number_active_client;
	// Common info for BTDM
	BOOLEAN			bBtEnabled;			// BT is enabled
	BOOLEAN			bBtConnectProcess;	// BT HS is under connection progress.
	unsigned char			btHsRssi;				// BT HS mode wifi rssi value.
	BOOLEAN			bBtHsOperation;		// BT HS mode is under progress
	unsigned char			btHsDigVal;			// use BT rssi to decide the DIG value
	BOOLEAN			bBtDisableEdcaTurbo;	// Under some condition, don't enable the EDCA Turbo
	BOOLEAN			bBtBusy;   			// BT is busy.
	BOOLEAN			bBtLimitedDig;   		// BT is busy.
//------------CALL BY VALUE-------------//
	unsigned char			RSSI_A;
	unsigned char			RSSI_B;
	unsigned char			RSSI_C;
	unsigned char			RSSI_D;
	unsigned long long			RSSI_TRSW;
	unsigned long long			RSSI_TRSW_H;
	unsigned long long			RSSI_TRSW_L;
	unsigned long long			RSSI_TRSW_iso;
	unsigned char			TRXAntStatus;
	unsigned char			cck_lna_idx;
	unsigned char			cck_vga_idx;
	unsigned char			ofdm_agc_idx[4];

	unsigned char			RxRate;
	BOOLEAN			bNoisyState;
	unsigned char			TxRate;
	unsigned char			LinkedInterval;
	unsigned char			preChannel;
	unsigned int			TxagcOffsetValueA;
	BOOLEAN			IsTxagcOffsetPositiveA;
	unsigned int			TxagcOffsetValueB;
	BOOLEAN			IsTxagcOffsetPositiveB;
	unsigned long long			lastTxOkCnt;
	unsigned long long			lastRxOkCnt;
	unsigned int			BbSwingOffsetA;
	BOOLEAN			IsBbSwingOffsetPositiveA;
	unsigned int			BbSwingOffsetB;
	BOOLEAN			IsBbSwingOffsetPositiveB;
	unsigned char			antdiv_rssi;
	unsigned char			fat_comb_a;
	unsigned char			fat_comb_b;
	unsigned char			antdiv_intvl;
	unsigned char			AntType;
	unsigned char			pre_AntType;
	unsigned char			antdiv_period;
	unsigned char			antdiv_select;
	unsigned char			path_select;
	unsigned char			antdiv_evm_en;
	unsigned char			bdc_holdstate;
	unsigned char			NdpaPeriod;
	BOOLEAN			H2C_RARpt_connect;
	BOOLEAN			cck_agc_report_type;

	unsigned char			dm_dig_max_TH;
	unsigned char 			dm_dig_min_TH;
	unsigned char 			print_agc;

	//For Adaptivtiy
	unsigned short			NHM_cnt_0;
	unsigned short			NHM_cnt_1;
	signed char			TH_L2H_ini;
	signed char			TH_EDCCA_HL_diff;
	signed char			TH_L2H_ini_mode2;
	signed char			TH_EDCCA_HL_diff_mode2;
	BOOLEAN			Carrier_Sense_enable;
	unsigned char			Adaptivity_IGI_upper;
	BOOLEAN			adaptivity_flag;
	unsigned char			DCbackoff;
	BOOLEAN			Adaptivity_enable;
	unsigned char			APTotalNum;
	BOOLEAN			EDCCA_enable;
	ADAPTIVITY_STATISTICS	Adaptivity;
	//For Adaptivtiy
	unsigned char			LastUSBHub;
	unsigned char			TxBfDataRate;

	unsigned char			c2h_cmd_start;
	unsigned char			fw_debug_trace[60];
	unsigned char			pre_c2h_seq;
	BOOLEAN			fw_buff_is_enpty;
	unsigned int			data_frame_num;

#if (DM_ODM_SUPPORT_TYPE &  (ODM_CE))
	ODM_NOISE_MONITOR noise_level;//[ODM_MAX_CHANNEL_NUM];
#endif
	//
	//2 Define STA info.
	// _ODM_STA_INFO
	// 2012/01/12 MH For MP, we need to reduce one array pointer for default port.??
	PSTA_INFO_T		pODM_StaInfo[ODM_ASSOCIATE_ENTRY_NUM];
	unsigned short			platform2phydm_macid_table[ODM_ASSOCIATE_ENTRY_NUM];		/* platform_macid_table[platform_macid] = phydm_macid */

#if (RATE_ADAPTIVE_SUPPORT == 1)
	unsigned short 			CurrminRptTime;
	ODM_RA_INFO_T   RAInfo[ODM_ASSOCIATE_ENTRY_NUM]; //Use MacID as array index. STA MacID=0, VWiFi Client MacID={1, ODM_ASSOCIATE_ENTRY_NUM-1} //YJ,add,120119
#endif
	//
	// 2012/02/14 MH Add to share 88E ra with other SW team.
	// We need to colelct all support abilit to a proper area.
	//
	BOOLEAN				RaSupport88E;

	// Define ...........

	// Latest packet phy info (ODM write)
	ODM_PHY_DBG_INFO_T	 PhyDbgInfo;
	//PHY_INFO_88E		PhyInfo;

	// Latest packet phy info (ODM write)
	ODM_MAC_INFO		*pMacInfo;
	//MAC_INFO_88E		MacInfo;

	// Different Team independt structure??

	//
	//TX_RTP_CMN		TX_retrpo;
	//TX_RTP_88E		TX_retrpo;
	//TX_RTP_8195		TX_retrpo;

	//
	//ODM Structure
	//
        #if (defined(CONFIG_HW_ANTENNA_DIVERSITY))
	#if (DM_ODM_SUPPORT_TYPE & (ODM_AP))
	BDC_T					DM_BdcTable;
	#endif
        #endif
	FAT_T						DM_FatTable;
	DIG_T						DM_DigTable;
	PS_T						DM_PSTable;
	Pri_CCA_T					DM_PriCCA;
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
	RXHP_T						DM_RXHP_Table;
#endif
	RA_T						DM_RA_Table;
	FALSE_ALARM_STATISTICS		FalseAlmCnt;
	FALSE_ALARM_STATISTICS		FlaseAlmCntBuddyAdapter;
	SWAT_T						DM_SWAT_Table;
	BOOLEAN						RSSI_test;
	CFO_TRACKING    				DM_CfoTrack;
	ACS							DM_ACS;


#if (RTL8814A_SUPPORT == 1)
	IQK_INFO	IQK_info;
#endif /* (RTL8814A_SUPPORT==1) */


#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
	//Path Div Struct
	PATHDIV_PARA	pathIQK;
#endif
#if(defined(CONFIG_PATH_DIVERSITY))
	PATHDIV_T	DM_PathDiv;
#endif

	EDCA_T		DM_EDCA_Table;
	unsigned int		WMMEDCA_BE;

	// Copy from SD4 structure
	//
	// ==================================================
	//

	//common
	//unsigned char		DM_Type;
	//unsigned char    PSD_Report_RXHP[80];   // Add By Gary
	//unsigned char    PSD_func_flag;               // Add By Gary
	//for DIG
	//unsigned char		bDMInitialGainEnable;
	//unsigned char		binitialized; // for dm_initial_gain_Multi_STA use.
	//for Antenna diversity
	//u8	AntDivCfg;// 0:OFF , 1:ON, 2:by efuse
	//PSTA_INFO_T RSSI_target;

	BOOLEAN			*pbDriverStopped;
	BOOLEAN			*pbDriverIsGoingToPnpSetPowerSleep;
	BOOLEAN			*pinit_adpt_in_progress;

	//PSD
	BOOLEAN			bUserAssignLevel;
	RT_TIMER 		PSDTimer;
	unsigned char			RSSI_BT;			//come from BT
	BOOLEAN			bPSDinProcess;
	BOOLEAN			bPSDactive;
	BOOLEAN			bDMInitialGainEnable;

	//MPT DIG
	RT_TIMER 		MPT_DIGTimer;

	//for rate adaptive, in fact,  88c/92c fw will handle this
	unsigned char			bUseRAMask;

	ODM_RATE_ADAPTIVE	RateAdaptive;
//#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN | ODM_CE))
#if(defined(CONFIG_ANT_DETECTION))
	ANT_DETECTED_INFO	AntDetectedInfo; // Antenna detected information for RSSI tool
#endif
	ODM_RF_CAL_T	RFCalibrateInfo;


	//
	// Dynamic ATC switch
	//

#if (DM_ODM_SUPPORT_TYPE &  (ODM_WIN|ODM_CE))
	//
	// Power Training
	//
	BOOLEAN			bDisablePowerTraining;
	unsigned char			ForcePowerTrainingState;
	BOOLEAN			bChangeState;
	unsigned int			PT_score;
	unsigned long long			OFDM_RX_Cnt;
	unsigned long long			CCK_RX_Cnt;
#endif

	//
	// ODM system resource.
	//

	// ODM relative time.
	RT_TIMER 				PathDivSwitchTimer;
	//2011.09.27 add for Path Diversity
	RT_TIMER				CCKPathDiversityTimer;
	RT_TIMER 	FastAntTrainingTimer;
#ifdef ODM_EVM_ENHANCE_ANTDIV
	RT_TIMER 			EVM_FastAntTrainingTimer;
#endif
	RT_TIMER		sbdcnt_timer;

	// ODM relative workitem.
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
#if USE_WORKITEM
	RT_WORK_ITEM			PathDivSwitchWorkitem;
	RT_WORK_ITEM			CCKPathDiversityWorkitem;
	RT_WORK_ITEM			FastAntTrainingWorkitem;
	RT_WORK_ITEM			MPT_DIGWorkitem;
	RT_WORK_ITEM			RaRptWorkitem;
	RT_WORK_ITEM			sbdcnt_workitem;
#endif

#if (BEAMFORMING_SUPPORT == 1)
	RT_BEAMFORMING_INFO BeamformingInfo;
#endif
#endif

#if(DM_ODM_SUPPORT_TYPE & ODM_WIN)

#if (RT_PLATFORM != PLATFORM_LINUX)
} DM_ODM_T, *PDM_ODM_T;		// DM_Dynamic_Mechanism_Structure
#else
};
#endif

#else// for AP,ADSL,CE Team
} DM_ODM_T, *PDM_ODM_T;		// DM_Dynamic_Mechanism_Structure
#endif


typedef enum _PHYDM_STRUCTURE_TYPE{
	PHYDM_FALSEALMCNT,
	PHYDM_CFOTRACK,
	PHYDM_ADAPTIVITY,
	PHYDM_ROMINFO,

}PHYDM_STRUCTURE_TYPE;



 typedef enum _ODM_RF_CONTENT{
	odm_radioa_txt = 0x1000,
	odm_radiob_txt = 0x1001,
	odm_radioc_txt = 0x1002,
	odm_radiod_txt = 0x1003
} ODM_RF_CONTENT;

typedef enum _ODM_BB_Config_Type{
	CONFIG_BB_PHY_REG,
	CONFIG_BB_AGC_TAB,
	CONFIG_BB_AGC_TAB_2G,
	CONFIG_BB_AGC_TAB_5G,
	CONFIG_BB_PHY_REG_PG,
	CONFIG_BB_PHY_REG_MP,
	CONFIG_BB_AGC_TAB_DIFF,
} ODM_BB_Config_Type, *PODM_BB_Config_Type;

typedef enum _ODM_RF_Config_Type{
	CONFIG_RF_RADIO,
    CONFIG_RF_TXPWR_LMT,
} ODM_RF_Config_Type, *PODM_RF_Config_Type;

typedef enum _ODM_FW_Config_Type{
    CONFIG_FW_NIC,
    CONFIG_FW_NIC_2,
    CONFIG_FW_AP,
    CONFIG_FW_AP_2,
    CONFIG_FW_MP,
    CONFIG_FW_WoWLAN,
    CONFIG_FW_WoWLAN_2,
    CONFIG_FW_AP_WoWLAN,
    CONFIG_FW_BT,
} ODM_FW_Config_Type;

// Status code
#if (DM_ODM_SUPPORT_TYPE != ODM_WIN)
typedef enum _RT_STATUS{
	RT_STATUS_SUCCESS,
	RT_STATUS_FAILURE,
	RT_STATUS_PENDING,
	RT_STATUS_RESOURCE,
	RT_STATUS_INVALID_CONTEXT,
	RT_STATUS_INVALID_PARAMETER,
	RT_STATUS_NOT_SUPPORT,
	RT_STATUS_OS_API_FAILED,
}RT_STATUS,*PRT_STATUS;
#endif // end of RT_STATUS definition

#ifdef REMOVE_PACK
#pragma pack()
#endif

//#include "odm_function.h"

//3===========================================================
//3 DIG
//3===========================================================

//Remove DIG by Yuchen

//3===========================================================
//3 AGC RX High Power Mode
//3===========================================================
#define          LNA_Low_Gain_1                      0x64
#define          LNA_Low_Gain_2                      0x5A
#define          LNA_Low_Gain_3                      0x58

#define          FA_RXHP_TH1                           5000
#define          FA_RXHP_TH2                           1500
#define          FA_RXHP_TH3                             800
#define          FA_RXHP_TH4                             600
#define          FA_RXHP_TH5                             500

//3===========================================================
//3 EDCA
//3===========================================================

//3===========================================================
//3 Dynamic Tx Power
//3===========================================================
//Dynamic Tx Power Control Threshold

//Remove By YuChen

//3===========================================================
//3 Tx Power Tracking
//3===========================================================



//3===========================================================
//3 Rate Adaptive
//3===========================================================
//Remove to odm_RaInfo.h by RS_James

//3===========================================================
//3 BB Power Save
//3===========================================================

typedef enum tag_1R_CCA_Type_Definition
{
	CCA_1R =0,
	CCA_2R = 1,
	CCA_MAX = 2,
}DM_1R_CCA_E;

typedef enum tag_RF_Type_Definition
{
	RF_Save =0,
	RF_Normal = 1,
	RF_MAX = 2,
}DM_RF_E;


//
// Extern Global Variables.
//
//PowerTracking move to odm_powerTrakcing.h by YuChen
//
// check Sta pointer valid or not
//
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
#define IS_STA_VALID(pSta)		(pSta && pSta->expire_to)
#elif (DM_ODM_SUPPORT_TYPE & ODM_WIN)
#define IS_STA_VALID(pSta)		(pSta && pSta->bUsed)
#else
#define IS_STA_VALID(pSta)		(pSta)
#endif

//Remove DIG by yuchen

//Remove BB power saving by Yuchen

//remove PT by yuchen

//ODM_RAStateCheck() Remove by RS_James

#if(DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_AP|ODM_ADSL))
//============================================================
// function prototype
//============================================================
//#define DM_ChangeDynamicInitGainThresh		ODM_ChangeDynamicInitGainThresh
//void	ODM_ChangeDynamicInitGainThresh(IN	PADAPTER	pAdapter,
//											IN	INT32		DM_Type,
//											IN	INT32		DM_Value);

//Remove DIG by yuchen


BOOLEAN
ODM_CheckPowerStatus(
	IN	PADAPTER		Adapter
	);


//Remove ODM_RateAdaptiveStateApInit() by RS_James

//Remove Edca by YuChen

#endif



unsigned int odm_ConvertTo_dB(unsigned int Value);

unsigned int odm_ConvertTo_linear(unsigned int Value);

#if((DM_ODM_SUPPORT_TYPE==ODM_WIN)||(DM_ODM_SUPPORT_TYPE==ODM_CE))

unsigned int
GetPSDData(
	PDM_ODM_T	pDM_Odm,
	unsigned int 	point,
	unsigned char initial_gain_psd);

#endif

#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
VOID
ODM_DMWatchdog_LPS(
	IN		PDM_ODM_T		pDM_Odm
);
#endif


s4Byte
ODM_PWdB_Conversion(
    IN  s4Byte X,
    IN  unsigned int TotalBit,
    IN  unsigned int DecimalBit
    );

s4Byte
ODM_SignConversion(
    IN  s4Byte value,
    IN  unsigned int TotalBit
    );

VOID
ODM_DMInit(
 IN	PDM_ODM_T	pDM_Odm
);

VOID
ODM_DMReset(
	IN	PDM_ODM_T	pDM_Odm
	);

VOID
phydm_support_ablity_debug(
	IN		PVOID		pDM_VOID,
	IN		unsigned int		*const dm_value,
	IN		unsigned int			*_used,
	OUT		char				*output,
	IN		unsigned int			*_out_len
	);

VOID
ODM_DMWatchdog(
	IN		PDM_ODM_T			pDM_Odm			// For common use in the future
	);

VOID
ODM_CmnInfoInit(
	IN		PDM_ODM_T		pDM_Odm,
	IN		ODM_CMNINFO_E	CmnInfo,
	IN		unsigned int			Value
	);

VOID
ODM_CmnInfoHook(
	IN		PDM_ODM_T		pDM_Odm,
	IN		ODM_CMNINFO_E	CmnInfo,
	IN		PVOID			pValue
	);

VOID
ODM_CmnInfoPtrArrayHook(
	IN		PDM_ODM_T		pDM_Odm,
	IN		ODM_CMNINFO_E	CmnInfo,
	IN		unsigned short			Index,
	IN		PVOID			pValue
	);

VOID
ODM_CmnInfoUpdate(
	IN		PDM_ODM_T		pDM_Odm,
	IN		unsigned int			CmnInfo,
	IN		unsigned long long			Value
	);

#if(DM_ODM_SUPPORT_TYPE==ODM_AP)
VOID
ODM_InitAllThreads(
    IN PDM_ODM_T	pDM_Odm
    );

VOID
ODM_StopAllThreads(
	IN PDM_ODM_T	pDM_Odm
	);
#endif

VOID
ODM_InitAllTimers(
    IN PDM_ODM_T	pDM_Odm
    );

VOID
ODM_CancelAllTimers(
    IN PDM_ODM_T    pDM_Odm
    );

VOID
ODM_ReleaseAllTimers(
    IN PDM_ODM_T	pDM_Odm
    );




#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
VOID ODM_InitAllWorkItems(IN PDM_ODM_T	pDM_Odm );
VOID ODM_FreeAllWorkItems(IN PDM_ODM_T	pDM_Odm );



unsigned long long
PlatformDivision64(
	IN unsigned long long	x,
	IN unsigned long long	y
);

//====================================================
//3 PathDiV End
//====================================================


#define DM_ChangeDynamicInitGainThresh		ODM_ChangeDynamicInitGainThresh
//void	ODM_ChangeDynamicInitGainThresh(IN	PADAPTER	pAdapter,
//											IN	INT32		DM_Type,
//											IN	INT32		DM_Value);
//
// PathDiveristy Remove by RS_James

typedef enum tag_DIG_Connect_Definition
{
	DIG_STA_DISCONNECT = 0,
	DIG_STA_CONNECT = 1,
	DIG_STA_BEFORE_CONNECT = 2,
	DIG_MultiSTA_DISCONNECT = 3,
	DIG_MultiSTA_CONNECT = 4,
	DIG_CONNECT_MAX
}DM_DIG_CONNECT_E;


//
// 2012/01/12 MH Check afapter status. Temp fix BSOD.
//
#define	HAL_ADAPTER_STS_CHK(pDM_Odm)\
	if (pDM_Odm->Adapter == NULL)\
	{\
		return;\
	}\


//
// For new definition in MP temporarily fro power tracking,
//
/*
#define odm_TXPowerTrackingDirectCall(_Adapter)	\
	IS_HARDWARE_TYPE_8192D(_Adapter) ? odm_TXPowerTrackingCallback_ThermalMeter_92D(_Adapter) : \
	IS_HARDWARE_TYPE_8192C(_Adapter) ? odm_TXPowerTrackingCallback_ThermalMeter_92C(_Adapter) : \
	IS_HARDWARE_TYPE_8723A(_Adapter) ? odm_TXPowerTrackingCallback_ThermalMeter_8723A(_Adapter) :\
	ODM_TXPowerTrackingCallback_ThermalMeter(_Adapter)
*/


#endif	// #if (DM_ODM_SUPPORT_TYPE == ODM_WIN)

VOID
ODM_AsocEntry_Init(
	IN		PDM_ODM_T		pDM_Odm
	);

//Remove ODM_DynamicARFBSelect() by RS_James

PVOID
PhyDM_Get_Structure(
	IN		PDM_ODM_T		pDM_Odm,
	IN		unsigned char			Structure_Type
);

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN) ||(DM_ODM_SUPPORT_TYPE == ODM_CE)
/*===========================================================*/
/* The following is for compile only*/
/*===========================================================*/

#define	IS_HARDWARE_TYPE_8723A(_Adapter)			FALSE
#define IS_HARDWARE_TYPE_8723AE(_Adapter)			FALSE
#define	IS_HARDWARE_TYPE_8192C(_Adapter)			FALSE
#define	IS_HARDWARE_TYPE_8192D(_Adapter)			FALSE
#define	RF_T_METER_92D					0x42


#define SET_TX_DESC_ANTSEL_A_92C(__pTxDesc, __Value) SET_BITS_TO_LE_1BYTE(__pTxDesc+8+3, 0, 1, __Value)
#define SET_TX_DESC_TX_ANTL_92C(__pTxDesc, __Value) SET_BITS_TO_LE_1BYTE(__pTxDesc+8+3, 4, 2, __Value)
#define SET_TX_DESC_TX_ANT_HT_92C(__pTxDesc, __Value) SET_BITS_TO_LE_1BYTE(__pTxDesc+8+3, 6, 2, __Value)
#define SET_TX_DESC_TX_ANT_CCK_92C(__pTxDesc, __Value) SET_BITS_TO_LE_1BYTE(__pTxDesc+8+3, 2, 2, __Value)

#define GET_RX_STATUS_DESC_RX_MCS(__pRxStatusDesc)				LE_BITS_TO_1BYTE( __pRxStatusDesc+12, 0, 6)

#define		RX_HAL_IS_CCK_RATE_92C(pDesc)\
			(GET_RX_STATUS_DESC_RX_MCS(pDesc) == DESC_RATE1M ||\
			GET_RX_STATUS_DESC_RX_MCS(pDesc) == DESC_RATE2M ||\
			GET_RX_STATUS_DESC_RX_MCS(pDesc) == DESC_RATE5_5M ||\
			GET_RX_STATUS_DESC_RX_MCS(pDesc) == DESC_RATE11M)

#define		H2C_92C_PSD_RESULT				16

#define		rConfig_ram64x16				0xb2c

#define TARGET_CHNL_NUM_2G_5G	59
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)

VOID
FillH2CCmd92C(
	IN	PADAPTER		Adapter,
	IN	unsigned char 	ElementID,
	IN	unsigned int 	CmdLen,
	IN	punsigned char	pCmdBuffer
);
VOID
PHY_SetTxPowerLevel8192C(
	IN	PADAPTER		Adapter,
	IN	unsigned char			channel
	);
unsigned char GetRightChnlPlaceforIQK(unsigned char chnl);

#endif

//===========================================================
#endif //#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)

#if (DM_ODM_SUPPORT_TYPE == ODM_CE)
void odm_dtc(PDM_ODM_T pDM_Odm);
#endif /* #if (DM_ODM_SUPPORT_TYPE == ODM_CE) */


VOID phydm_NoisyDetection(IN	PDM_ODM_T	pDM_Odm	);


#endif

