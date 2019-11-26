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

#ifndef	__PHYDMDIG_H__
#define    __PHYDMDIG_H__

#define DIG_VERSION	"1.6"	/*2015.02.09*/

typedef struct _Dynamic_Initial_Gain_Threshold_
{
	BOOLEAN		bStopDIG;		// for debug
	BOOLEAN		bPauseDIG;
	BOOLEAN		bIgnoreDIG;
	BOOLEAN		bPSDInProgress;

	unsigned char		Dig_Enable_Flag;
	unsigned char		Dig_Ext_Port_Stage;

	int		   	RssiLowThresh;
	int		    	RssiHighThresh;

	unsigned int		FALowThresh;
	unsigned int		FAHighThresh;

	unsigned char		CurSTAConnectState;
	unsigned char		PreSTAConnectState;
	unsigned char		CurMultiSTAConnectState;

	unsigned char		PreIGValue;
	unsigned char		CurIGValue;
	unsigned char		BackupIGValue;		//MP DIG
	unsigned char		BT30_CurIGI;
	unsigned char		IGIBackup;

	signed char		BackoffVal;
	signed char		BackoffVal_range_max;
	signed char		BackoffVal_range_min;
	unsigned char		rx_gain_range_max;
	unsigned char		rx_gain_range_min;
	unsigned char		Rssi_val_min;

	unsigned char		PreCCK_CCAThres;
	unsigned char		CurCCK_CCAThres;
	unsigned char		PreCCKPDState;
	unsigned char		CurCCKPDState;
	unsigned char		CCKPDBackup;

	unsigned char		LargeFAHit;
	unsigned char		ForbiddenIGI;
	unsigned int		Recover_cnt;

	unsigned char		DIG_Dynamic_MIN_0;
	unsigned char		DIG_Dynamic_MIN_1;
	BOOLEAN		bMediaConnect_0;
	BOOLEAN		bMediaConnect_1;

	unsigned int		AntDiv_RSSI_max;
	unsigned int		RSSI_max;

	unsigned char		*bP2PInProcess;

#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	BOOLEAN		bTpTarget;
	BOOLEAN		bNoiseEst;
	unsigned int		TpTrainTH_min;
	unsigned char		IGIOffset_A;
	unsigned char		IGIOffset_B;
#endif
}DIG_T,*pDIG_T;

typedef struct _FALSE_ALARM_STATISTICS{
	unsigned int	Cnt_Parity_Fail;
	unsigned int	Cnt_Rate_Illegal;
	unsigned int	Cnt_Crc8_fail;
	unsigned int	Cnt_Mcs_fail;
	unsigned int	Cnt_Ofdm_fail;
	unsigned int	Cnt_Ofdm_fail_pre;	//For RTL8881A
	unsigned int	Cnt_Cck_fail;
	unsigned int	Cnt_all;
	unsigned int	Cnt_Fast_Fsync;
	unsigned int	Cnt_SB_Search_fail;
	unsigned int	Cnt_OFDM_CCA;
	unsigned int	Cnt_CCK_CCA;
	unsigned int	Cnt_CCA_all;
	unsigned int	Cnt_BW_USC;	//Gary
	unsigned int	Cnt_BW_LSC;	//Gary
}FALSE_ALARM_STATISTICS, *PFALSE_ALARM_STATISTICS;

typedef enum tag_Dynamic_Init_Gain_Operation_Type_Definition
{
	DIG_TYPE_THRESH_HIGH	= 0,
	DIG_TYPE_THRESH_LOW	= 1,
	DIG_TYPE_BACKOFF		= 2,
	DIG_TYPE_RX_GAIN_MIN	= 3,
	DIG_TYPE_RX_GAIN_MAX	= 4,
	DIG_TYPE_ENABLE 		= 5,
	DIG_TYPE_DISABLE 		= 6,
	DIG_OP_TYPE_MAX
}DM_DIG_OP_E;

typedef enum tag_ODM_PauseDIG_Type {
	ODM_PAUSE_DIG    		= 	BIT0,
	ODM_RESUME_DIG  		= 	BIT1
} ODM_Pause_DIG_TYPE;

typedef enum tag_ODM_PauseCCKPD_Type {
	ODM_PAUSE_CCKPD    	= 	BIT0,
	ODM_RESUME_CCKPD  	= 	BIT1
} ODM_Pause_CCKPD_TYPE;

/*
typedef enum tag_CCK_Packet_Detection_Threshold_Type_Definition
{
	CCK_PD_STAGE_LowRssi = 0,
	CCK_PD_STAGE_HighRssi = 1,
	CCK_PD_STAGE_MAX = 3,
}DM_CCK_PDTH_E;

typedef enum tag_DIG_EXT_PORT_ALGO_Definition
{
	DIG_EXT_PORT_STAGE_0 = 0,
	DIG_EXT_PORT_STAGE_1 = 1,
	DIG_EXT_PORT_STAGE_2 = 2,
	DIG_EXT_PORT_STAGE_3 = 3,
	DIG_EXT_PORT_STAGE_MAX = 4,
}DM_DIG_EXT_PORT_ALG_E;

typedef enum tag_DIG_Connect_Definition
{
	DIG_STA_DISCONNECT = 0,
	DIG_STA_CONNECT = 1,
	DIG_STA_BEFORE_CONNECT = 2,
	DIG_MultiSTA_DISCONNECT = 3,
	DIG_MultiSTA_CONNECT = 4,
	DIG_CONNECT_MAX
}DM_DIG_CONNECT_E;


#define DM_MultiSTA_InitGainChangeNotify(Event) {DM_DigTable.CurMultiSTAConnectState = Event;}

#define DM_MultiSTA_InitGainChangeNotify_CONNECT(_ADAPTER)	\
	DM_MultiSTA_InitGainChangeNotify(DIG_MultiSTA_CONNECT)

#define DM_MultiSTA_InitGainChangeNotify_DISCONNECT(_ADAPTER)	\
	DM_MultiSTA_InitGainChangeNotify(DIG_MultiSTA_DISCONNECT)
*/
#define		DM_DIG_THRESH_HIGH			40
#define		DM_DIG_THRESH_LOW			35

#define		DM_FALSEALARM_THRESH_LOW	400
#define		DM_FALSEALARM_THRESH_HIGH	1000

#define		DM_DIG_MAX_NIC				0x3e
#define		DM_DIG_MIN_NIC				0x1e //0x22//0x1c
#define		DM_DIG_MAX_OF_MIN_NIC		0x3e

#define		DM_DIG_MAX_AP					0x3e
#define		DM_DIG_MIN_AP					0x1c
#define		DM_DIG_MAX_OF_MIN			0x2A	//0x32
#define		DM_DIG_MIN_AP_DFS				0x20

#define		DM_DIG_MAX_NIC_HP			0x46
#define		DM_DIG_MIN_NIC_HP				0x2e

#define		DM_DIG_MAX_AP_HP				0x42
#define		DM_DIG_MIN_AP_HP				0x30

#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
#define		DM_DIG_MAX_AP_COVERAGR		0x26
#define		DM_DIG_MIN_AP_COVERAGE		0x1c
#define		DM_DIG_MAX_OF_MIN_COVERAGE	0x22

#define		DM_DIG_TP_Target_TH0			500
#define		DM_DIG_TP_Target_TH1			1000
#define		DM_DIG_TP_Training_Period		10
#endif

//vivi 92c&92d has different definition, 20110504
//this is for 92c
#if (DM_ODM_SUPPORT_TYPE & ODM_CE)
	#ifdef CONFIG_SPECIAL_SETTING_FOR_FUNAI_TV
	#define		DM_DIG_FA_TH0				0x80//0x20
	#else
	#define		DM_DIG_FA_TH0				0x200//0x20
	#endif
#else
	#define		DM_DIG_FA_TH0				0x200//0x20
#endif

#define		DM_DIG_FA_TH1					0x300
#define		DM_DIG_FA_TH2					0x400
//this is for 92d
#define		DM_DIG_FA_TH0_92D				0x100
#define		DM_DIG_FA_TH1_92D				0x400
#define		DM_DIG_FA_TH2_92D				0x600

#define		DM_DIG_BACKOFF_MAX			12
#define		DM_DIG_BACKOFF_MIN			-4
#define		DM_DIG_BACKOFF_DEFAULT		10

#define 		DM_DIG_FA_TH0_LPS				4 //-> 4 in lps
#define 		DM_DIG_FA_TH1_LPS				15 //-> 15 lps
#define 		DM_DIG_FA_TH2_LPS				30 //-> 30 lps
#define 		RSSI_OFFSET_DIG				0x05

VOID
ODM_ChangeDynamicInitGainThresh(
	IN		PVOID					pDM_VOID,
	IN		unsigned int  					DM_Type,
	IN		unsigned int 					DM_Value
	);

VOID
ODM_Write_DIG(
	IN		PVOID					pDM_VOID,
	IN		unsigned char					CurrentIGI
	);

VOID
odm_PauseDIG(
	IN		PVOID					pDM_VOID,
	IN		ODM_Pause_DIG_TYPE		PauseType,
	IN		unsigned char					IGIValue
	);

VOID
odm_DIGInit(
	IN		PVOID					pDM_VOID
	);

VOID
odm_DIG(
	IN		PVOID					pDM_VOID
	);

VOID
odm_DIGbyRSSI_LPS(
	IN		PVOID					pDM_VOID
	);

VOID
odm_FalseAlarmCounterStatistics(
	IN		PVOID					pDM_VOID
	);

VOID
odm_PauseCCKPacketDetection(
	IN		PVOID					pDM_VOID,
	IN		ODM_Pause_CCKPD_TYPE	PauseType,
	IN		unsigned char					CCKPDThreshold
	);

VOID
odm_CCKPacketDetectionThresh(
	IN		PVOID					pDM_VOID
	);

VOID
ODM_Write_CCK_CCA_Thres(
	IN		PVOID					pDM_VOID,
	IN		unsigned char					CurCCK_CCAThres
	);

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
VOID
odm_MPT_DIGCallback(
	PRT_TIMER						pTimer
);

VOID
odm_MPT_DIGWorkItemCallback(
    IN 		PVOID            			pContext
    );

#endif

#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
VOID
odm_MPT_DIGCallback(
	IN		PVOID					pDM_VOID
);
#endif

#if (DM_ODM_SUPPORT_TYPE != ODM_CE)
VOID
ODM_MPT_DIG(
	IN		PVOID					pDM_VOID
);
#endif


#endif
