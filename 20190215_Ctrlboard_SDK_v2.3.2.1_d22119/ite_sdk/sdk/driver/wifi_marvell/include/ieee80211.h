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
#ifndef __IEEE80211_H
#define __IEEE80211_H




	//#include <drv_conf.h>
	#include "osdep_service.h"
	//#include <drv_types.h>
	#include "wifi.h"
        #include "ports/nl80211_copy.h"
	#if defined PLATFORM_OS_XP
	#include <ntstrsafe.h>
	#endif
	#if defined PLATFORM_LINUX
	//#include <linux/wireless.h>
	#endif

#define MGMT_QUEUE_NUM 5

#define ETH_ALEN	6
#define ETH_TYPE_LEN		2
#define PAYLOAD_TYPE_LEN	1

#ifdef CONFIG_AP_MODE

#define RTL_IOCTL_HOSTAPD (SIOCIWFIRSTPRIV + 28)

/* RTL871X_IOCTL_HOSTAPD ioctl() cmd: */
enum {
	RTL871X_HOSTAPD_FLUSH = 1,
	RTL871X_HOSTAPD_ADD_STA = 2,
	RTL871X_HOSTAPD_REMOVE_STA = 3,
	RTL871X_HOSTAPD_GET_INFO_STA = 4,
	/* REMOVED: PRISM2_HOSTAPD_RESET_TXEXC_STA = 5, */
	RTL871X_HOSTAPD_GET_WPAIE_STA = 5,
	RTL871X_SET_ENCRYPTION = 6,
	RTL871X_GET_ENCRYPTION = 7,
	RTL871X_HOSTAPD_SET_FLAGS_STA = 8,
	RTL871X_HOSTAPD_GET_RID = 9,
	RTL871X_HOSTAPD_SET_RID = 10,
	RTL871X_HOSTAPD_SET_ASSOC_AP_ADDR = 11,
	RTL871X_HOSTAPD_SET_GENERIC_ELEMENT = 12,
	RTL871X_HOSTAPD_MLME = 13,
	RTL871X_HOSTAPD_SCAN_REQ = 14,
	RTL871X_HOSTAPD_STA_CLEAR_STATS = 15,
	RTL871X_HOSTAPD_SET_BEACON=16,
	RTL871X_HOSTAPD_SET_WPS_BEACON = 17,
	RTL871X_HOSTAPD_SET_WPS_PROBE_RESP = 18,
	RTL871X_HOSTAPD_SET_WPS_ASSOC_RESP = 19,
	RTL871X_HOSTAPD_SET_HIDDEN_SSID = 20,
	RTL871X_HOSTAPD_SET_MACADDR_ACL = 21,
	RTL871X_HOSTAPD_ACL_ADD_STA = 22,
	RTL871X_HOSTAPD_ACL_REMOVE_STA = 23,
};

/* STA flags */
#define WLAN_STA_AUTH BIT(0)
#define WLAN_STA_ASSOC BIT(1)
#define WLAN_STA_PS BIT(2)
#define WLAN_STA_TIM BIT(3)
#define WLAN_STA_PERM BIT(4)
#define WLAN_STA_AUTHORIZED BIT(5)
#define WLAN_STA_PENDING_POLL BIT(6) /* pending activity poll not ACKed */
#define WLAN_STA_SHORT_PREAMBLE BIT(7)
#define WLAN_STA_PREAUTH BIT(8)
#define WLAN_STA_WME BIT(9)
#define WLAN_STA_MFP BIT(10)
#define WLAN_STA_HT BIT(11)
#define WLAN_STA_WPS BIT(12)
#define WLAN_STA_MAYBE_WPS BIT(13)
#define WLAN_STA_NONERP BIT(31)

#endif

#define IEEE_CMD_SET_WPA_PARAM			1
#define IEEE_CMD_SET_WPA_IE				2
#define IEEE_CMD_SET_ENCRYPTION			3
#define IEEE_CMD_MLME						4

#define IEEE_PARAM_WPA_ENABLED				1
#define IEEE_PARAM_TKIP_COUNTERMEASURES		2
#define IEEE_PARAM_DROP_UNENCRYPTED			3
#define IEEE_PARAM_PRIVACY_INVOKED			4
#define IEEE_PARAM_AUTH_ALGS					5
#define IEEE_PARAM_IEEE_802_1X				6
#define IEEE_PARAM_WPAX_SELECT				7

#define AUTH_ALG_OPEN_SYSTEM			0x1
#define AUTH_ALG_SHARED_KEY			0x2
#define AUTH_ALG_LEAP				0x00000004

#define IEEE_MLME_STA_DEAUTH				1
#define IEEE_MLME_STA_DISASSOC			2

#define IEEE_CRYPT_ERR_UNKNOWN_ALG			2
#define IEEE_CRYPT_ERR_UNKNOWN_ADDR			3
#define IEEE_CRYPT_ERR_CRYPT_INIT_FAILED		4
#define IEEE_CRYPT_ERR_KEY_SET_FAILED			5
#define IEEE_CRYPT_ERR_TX_KEY_SET_FAILED		6
#define IEEE_CRYPT_ERR_CARD_CONF_FAILED		7


#define	IEEE_CRYPT_ALG_NAME_LEN			16

#define WPA_CIPHER_NONE 	BIT(0)
#define WPA_CIPHER_WEP40 	BIT(1)
#define WPA_CIPHER_WEP104 BIT(2)
#define WPA_CIPHER_TKIP 	BIT(3)
#define WPA_CIPHER_CCMP 	BIT(4)

/* cipher suite selectors */
#define WLAN_CIPHER_SUITE_USE_GROUP	0x000FAC00
#define WLAN_CIPHER_SUITE_WEP40		0x000FAC01
#define WLAN_CIPHER_SUITE_TKIP		0x000FAC02
/* reserved: 				0x000FAC03 */
#define WLAN_CIPHER_SUITE_CCMP		0x000FAC04
#define WLAN_CIPHER_SUITE_WEP104	0x000FAC05
#define WLAN_CIPHER_SUITE_AES_CMAC	0x000FAC06

/* AKM suite selectors */
#define WLAN_AKM_SUITE_8021X		0x000FAC01
#define WLAN_AKM_SUITE_PSK		0x000FAC02

#define IEEE80211_NUM_TIDS		16

#define WPA_SELECTOR_LEN 4
extern u8 RTW_WPA_OUI_TYPE[] ;
extern u16 RTW_WPA_VERSION ;
extern u8 WPA_AUTH_KEY_MGMT_NONE[];
extern u8 WPA_AUTH_KEY_MGMT_UNSPEC_802_1X[];
extern u8 WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X[];
extern u8 WPA_CIPHER_SUITE_NONE[];
extern u8 WPA_CIPHER_SUITE_WEP40[];
extern u8 WPA_CIPHER_SUITE_TKIP[];
extern u8 WPA_CIPHER_SUITE_WRAP[];
extern u8 WPA_CIPHER_SUITE_CCMP[];
extern u8 WPA_CIPHER_SUITE_WEP104[];


#define RSN_HEADER_LEN 4
#define RSN_SELECTOR_LEN 4
#define ETHTOOL_FWVERS_LEN	32

extern u16 RSN_VERSION_BSD;
extern u8 RSN_AUTH_KEY_MGMT_UNSPEC_802_1X[];
extern u8 RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X[];
extern u8 RSN_CIPHER_SUITE_NONE[];
extern u8 RSN_CIPHER_SUITE_WEP40[];
extern u8 RSN_CIPHER_SUITE_TKIP[];
extern u8 RSN_CIPHER_SUITE_WRAP[];
extern u8 RSN_CIPHER_SUITE_CCMP[];
extern u8 RSN_CIPHER_SUITE_WEP104[];

typedef enum _RATR_TABLE_MODE{
	RATR_INX_WIRELESS_NGB = 0,	// BGN 40 Mhz 2SS 1SS
	RATR_INX_WIRELESS_NG = 1,		// GN or N
	RATR_INX_WIRELESS_NB = 2,		// BGN 20 Mhz 2SS 1SS  or BN
	RATR_INX_WIRELESS_N = 3,
	RATR_INX_WIRELESS_GB = 4,
	RATR_INX_WIRELESS_G = 5,
	RATR_INX_WIRELESS_B = 6,
	RATR_INX_WIRELESS_MC = 7,
	RATR_INX_WIRELESS_AC_N = 8,
}RATR_TABLE_MODE, *PRATR_TABLE_MODE;

enum NETWORK_TYPE
{
    WIRELESS_INVALID = 0,
    //Sub-Element
    WIRELESS_11B = BIT(0), // tx: cck only , rx: cck only, hw: cck
    WIRELESS_11G = BIT(1), // tx: ofdm only, rx: ofdm & cck, hw: cck & ofdm
    WIRELESS_11A = BIT(2), // tx: ofdm only, rx: ofdm only, hw: ofdm only
    WIRELESS_11_24N = BIT(3), // tx: MCS only, rx: MCS & cck, hw: MCS & cck
    WIRELESS_11_5N = BIT(4), // tx: MCS only, rx: MCS & ofdm, hw: ofdm only
	//WIRELESS_AUTO 	= BIT(5), 
	WIRELESS_AC 		= BIT(6), 

    //Combination
    WIRELESS_11BG = (WIRELESS_11B|WIRELESS_11G), // tx: cck & ofdm, rx: cck & ofdm & MCS, hw: cck & ofdm
    WIRELESS_11G_24N = (WIRELESS_11G|WIRELESS_11_24N), // tx: ofdm & MCS, rx: ofdm & cck & MCS, hw: cck & ofdm
    WIRELESS_11A_5N = (WIRELESS_11A|WIRELESS_11_5N), // tx: ofdm & MCS, rx: ofdm & MCS, hw: ofdm only
    WIRELESS_11BG_24N = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11_24N), // tx: ofdm & cck & MCS, rx: ofdm & cck & MCS, hw: ofdm & cck
    WIRELESS_11AGN = (WIRELESS_11A|WIRELESS_11G|WIRELESS_11_24N|WIRELESS_11_5N), // tx: ofdm & MCS, rx: ofdm & MCS, hw: ofdm only
    WIRELESS_11ABGN = (WIRELESS_11A|WIRELESS_11B|WIRELESS_11G|WIRELESS_11_24N|WIRELESS_11_5N),
};

enum nl80211_bss_scan_width {
	NL80211_BSS_CHAN_WIDTH_20,
	NL80211_BSS_CHAN_WIDTH_10,
	NL80211_BSS_CHAN_WIDTH_5,
};

enum nl80211_feature_flags {
	NL80211_FEATURE_SK_TX_STATUS			= 1 << 0,
	NL80211_FEATURE_HT_IBSS				= 1 << 1,
	NL80211_FEATURE_INACTIVITY_TIMER		= 1 << 2,
	NL80211_FEATURE_CELL_BASE_REG_HINTS		= 1 << 3,
	NL80211_FEATURE_P2P_DEVICE_NEEDS_CHANNEL	= 1 << 4,
	NL80211_FEATURE_SAE				= 1 << 5,
	NL80211_FEATURE_LOW_PRIORITY_SCAN		= 1 << 6,
	NL80211_FEATURE_SCAN_FLUSH			= 1 << 7,
	NL80211_FEATURE_AP_SCAN				= 1 << 8,
	NL80211_FEATURE_VIF_TXPOWER			= 1 << 9,
	NL80211_FEATURE_NEED_OBSS_SCAN			= 1 << 10,
	NL80211_FEATURE_P2P_GO_CTWIN			= 1 << 11,
	NL80211_FEATURE_P2P_GO_OPPPS			= 1 << 12,
	/* bit 13 is reserved */
	NL80211_FEATURE_ADVERTISE_CHAN_LIMITS		= 1 << 14,
	NL80211_FEATURE_FULL_AP_CLIENT_STATE		= 1 << 15,
	NL80211_FEATURE_USERSPACE_MPM			= 1 << 16,
	NL80211_FEATURE_ACTIVE_MONITOR			= 1 << 17,
	NL80211_FEATURE_AP_MODE_CHAN_WIDTH_CHANGE	= 1 << 18,
	NL80211_FEATURE_DS_PARAM_SET_IE_IN_PROBES	= 1 << 19,
	NL80211_FEATURE_WFA_TPC_IE_IN_PROBES		= 1 << 20,
	NL80211_FEATURE_QUIET				= 1 << 21,
	NL80211_FEATURE_TX_POWER_INSERTION		= 1 << 22,
	NL80211_FEATURE_ACKTO_ESTIMATION		= 1 << 23,
	NL80211_FEATURE_STATIC_SMPS			= 1 << 24,
	NL80211_FEATURE_DYNAMIC_SMPS			= 1 << 25,
	NL80211_FEATURE_SUPPORTS_WMM_ADMISSION		= 1 << 26,
	NL80211_FEATURE_MAC_ON_CREATE			= 1 << 27,
	NL80211_FEATURE_TDLS_CHANNEL_SWITCH		= 1 << 28,
	NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR		= 1 << 29,
	NL80211_FEATURE_SCHED_SCAN_RANDOM_MAC_ADDR	= 1 << 30,
	NL80211_FEATURE_ND_RANDOM_MAC_ADDR		= 1 << 31,
};

enum nl80211_ext_feature_index {
	NL80211_EXT_FEATURE_VHT_IBSS,

	/* add new features before the definition below */
	NUM_NL80211_EXT_FEATURES,
	MAX_NL80211_EXT_FEATURES = NUM_NL80211_EXT_FEATURES - 1
};

enum cfg80211_signal_type {
	CFG80211_SIGNAL_TYPE_NONE,
	CFG80211_SIGNAL_TYPE_MBM,
	CFG80211_SIGNAL_TYPE_UNSPEC,
};


enum nl80211_user_reg_hint_type {
	NL80211_USER_REG_HINT_USER	= 0,
	NL80211_USER_REG_HINT_CELL_BASE = 1,
	NL80211_USER_REG_HINT_INDOOR    = 2,
};

enum nl80211_dfs_regions {
	NL80211_DFS_UNSET	= 0,
	NL80211_DFS_FCC		= 1,
	NL80211_DFS_ETSI	= 2,
	NL80211_DFS_JP		= 3,
};

enum environment_cap {
	ENVIRON_ANY,
	ENVIRON_INDOOR,
	ENVIRON_OUTDOOR,
};

enum nl80211_mesh_power_mode {
	NL80211_MESH_POWER_UNKNOWN,
	NL80211_MESH_POWER_ACTIVE,
	NL80211_MESH_POWER_LIGHT_SLEEP,
	NL80211_MESH_POWER_DEEP_SLEEP,

	__NL80211_MESH_POWER_AFTER_LAST,
	NL80211_MESH_POWER_MAX = __NL80211_MESH_POWER_AFTER_LAST - 1
};

#define SUPPORTED_24G_NETTYPE_MSK (WIRELESS_11B | WIRELESS_11G | WIRELESS_11_24N)
#define SUPPORTED_5G_NETTYPE_MSK (WIRELESS_11A | WIRELESS_11_5N)

#define IsSupported24G(NetType) ((NetType) & SUPPORTED_24G_NETTYPE_MSK ? _TRUE : _FALSE)
#define IsSupported5G(NetType) ((NetType) & SUPPORTED_5G_NETTYPE_MSK ? _TRUE : _FALSE)

#define IsEnableHWCCK(NetType) IsSupported24G(NetType)
#define IsEnableHWOFDM(NetType) ((NetType) & (WIRELESS_11G|WIRELESS_11_24N|SUPPORTED_5G_NETTYPE_MSK) ? _TRUE : _FALSE)

#define IsSupportedRxCCK(NetType) IsEnableHWCCK(NetType)
#define IsSupportedRxOFDM(NetType) IsEnableHWOFDM(NetType)
#define IsSupportedRxMCS(NetType) IsEnableHWOFDM(NetType)

#define IsSupportedTxCCK(NetType) ((NetType) & (WIRELESS_11B) ? _TRUE : _FALSE)
#define IsSupportedTxOFDM(NetType) ((NetType) & (WIRELESS_11G|WIRELESS_11A) ? _TRUE : _FALSE)
#define IsSupportedTxMCS(NetType) ((NetType) & (WIRELESS_11_24N|WIRELESS_11_5N) ? _TRUE : _FALSE) 


typedef struct ieee_param {
	u32 cmd;
	u8 sta_addr[ETH_ALEN];
	union {
		struct {
			u8 name;
			u32 value;
		} wpa_param;
		struct {
			u32 len;
			u8 reserved[32];
			u8 data[0];
		} wpa_ie;
	        struct{
			int command;
    			int reason_code;
		} mlme;
		struct {
			u8 alg[IEEE_CRYPT_ALG_NAME_LEN];
			u8 set_tx;
			u32 err;
			u8 idx;
			u8 seq[8]; /* sequence counter (set: RX, get: TX) */
			u16 key_len;
			u8 key[0];
		} crypt;
#ifdef CONFIG_AP_MODE
		struct {
			u16 aid;
			u16 capability;
			int flags;
			u8 tx_supp_rates[16];		
			struct rtw_ieee80211_ht_cap ht_cap;
		} add_sta;
		struct {
			u8	reserved[2];//for set max_num_sta
			u8	buf[0];
		} bcn_ie;
#endif

	} u;	   
}ieee_param;

#ifdef CONFIG_AP_MODE
typedef struct ieee_param_ex {
	u32 cmd;
	u8 sta_addr[ETH_ALEN];
	u8 data[0];
}ieee_param_ex;

struct sta_data{
	u16 aid;
	u16 capability;
	int flags;
	u32 sta_set;
	u8 tx_supp_rates[16];	
	u32 tx_supp_rates_len;
	struct rtw_ieee80211_ht_cap ht_cap;
	u64	rx_pkts;
	u64	rx_bytes;
	u64	rx_drops;
	u64	tx_pkts;
	u64	tx_bytes;
	u64	tx_drops;
};
#endif


#if WIRELESS_EXT < 17
#define IW_QUAL_QUAL_INVALID   0x10
#define IW_QUAL_LEVEL_INVALID  0x20
#define IW_QUAL_NOISE_INVALID  0x40
#define IW_QUAL_QUAL_UPDATED   0x1
#define IW_QUAL_LEVEL_UPDATED  0x2
#define IW_QUAL_NOISE_UPDATED  0x4
#endif

#define IEEE80211_DATA_LEN		2304
/* Maximum size for the MA-UNITDATA primitive, 802.11 standard section
   6.2.1.1.2.

   The figure in section 7.1.2 suggests a body size of up to 2312
   bytes is allowed, which is a bit confusing, I suspect this
   represents the 2304 bytes of real data, plus a possible 8 bytes of
   WEP IV and ICV. (this interpretation suggested by Ramiro Barreiro) */


#define IEEE80211_HLEN			30
#define IEEE80211_FRAME_LEN		(IEEE80211_DATA_LEN + IEEE80211_HLEN)
#define IEEE80211_MAX_CHAINS	        4

/* this is stolen from ipw2200 driver */
#define IEEE_IBSS_MAC_HASH_SIZE 31

struct ieee_ibss_seq {
	u8 mac[ETH_ALEN];
	u16 seq_num;
	u16 frag_num;
	unsigned long packet_time;
	_list	list;
};

#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW)||defined(PLATFORM_FREEBSD) 

struct rtw_ieee80211_hdr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u8 addr4[ETH_ALEN];
} __attribute__ ((packed));

struct rtw_ieee80211_hdr_3addr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
} __attribute__ ((packed));


struct rtw_ieee80211_hdr_qos {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u8 addr4[ETH_ALEN];
	u16	qc;
}  __attribute__ ((packed));

struct rtw_ieee80211_hdr_3addr_qos {
        u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
       u16     qc;
}  __attribute__ ((packed));

struct eapol {
	u8 snap[6];
	u16 ethertype;
	u8 version;
	u8 type;
	u16 length;
} __attribute__ ((packed));

#endif



#ifdef PLATFORM_WINDOWS

#pragma pack(1)
struct rtw_ieee80211_hdr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u8 addr4[ETH_ALEN];
};

struct rtw_ieee80211_hdr_3addr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
};


struct rtw_ieee80211_hdr_qos {
	struct rtw_ieee80211_hdr wlan_hdr;
	u16	qc;
};

struct rtw_ieee80211_hdr_3addr_qos {
        struct  rtw_ieee80211_hdr_3addr wlan_hdr;
        u16     qc;
};

struct eapol {
	u8 snap[6];
	u16 ethertype;
	u8 version;
	u8 type;
	u16 length;
};
#pragma pack()

#endif



enum eap_type {
	EAP_PACKET = 0,
	EAPOL_START,
	EAPOL_LOGOFF,
	EAPOL_KEY,
	EAPOL_ENCAP_ASF_ALERT
};

#define IEEE80211_3ADDR_LEN 24
#define IEEE80211_4ADDR_LEN 30
#define IEEE80211_FCS_LEN    4

#define MIN_FRAG_THRESHOLD     256U
#define	MAX_FRAG_THRESHOLD     2346U

/* Frame control field constants */
#define RTW_IEEE80211_FCTL_VERS		0x0003
#define RTW_IEEE80211_FCTL_FTYPE		0x000c
#define RTW_IEEE80211_FCTL_STYPE		0x00f0
#define RTW_IEEE80211_FCTL_TODS		0x0100
#define RTW_IEEE80211_FCTL_FROMDS	0x0200
#define RTW_IEEE80211_FCTL_MOREFRAGS	0x0400
#define RTW_IEEE80211_FCTL_RETRY		0x0800
#define RTW_IEEE80211_FCTL_PM		0x1000
#define RTW_IEEE80211_FCTL_MOREDATA	0x2000
#define RTW_IEEE80211_FCTL_PROTECTED	0x4000
#define RTW_IEEE80211_FCTL_ORDER		0x8000
#define RTW_IEEE80211_FCTL_CTL_EXT	0x0f00

#define RTW_IEEE80211_FTYPE_MGMT		0x0000
#define RTW_IEEE80211_FTYPE_CTL		0x0004
#define RTW_IEEE80211_FTYPE_DATA		0x0008
#define RTW_IEEE80211_FTYPE_EXT		0x000c

/* management */
#define RTW_IEEE80211_STYPE_ASSOC_REQ	0x0000
#define RTW_IEEE80211_STYPE_ASSOC_RESP 	0x0010
#define RTW_IEEE80211_STYPE_REASSOC_REQ	0x0020
#define RTW_IEEE80211_STYPE_REASSOC_RESP	0x0030
#define RTW_IEEE80211_STYPE_PROBE_REQ	0x0040
#define RTW_IEEE80211_STYPE_PROBE_RESP	0x0050
#define RTW_IEEE80211_STYPE_BEACON		0x0080
#define RTW_IEEE80211_STYPE_ATIM		0x0090
#define RTW_IEEE80211_STYPE_DISASSOC	0x00A0
#define RTW_IEEE80211_STYPE_AUTH		0x00B0
#define RTW_IEEE80211_STYPE_DEAUTH		0x00C0
#define RTW_IEEE80211_STYPE_ACTION		0x00D0

/* control */
#define RTW_IEEE80211_STYPE_CTL_EXT		0x0060
#define RTW_IEEE80211_STYPE_BACK_REQ		0x0080
#define RTW_IEEE80211_STYPE_BACK		0x0090
#define RTW_IEEE80211_STYPE_PSPOLL		0x00A0
#define RTW_IEEE80211_STYPE_RTS		0x00B0
#define RTW_IEEE80211_STYPE_CTS		0x00C0
#define RTW_IEEE80211_STYPE_ACK		0x00D0
#define RTW_IEEE80211_STYPE_CFEND		0x00E0
#define RTW_IEEE80211_STYPE_CFENDACK		0x00F0

/* data */
#define RTW_IEEE80211_STYPE_DATA		0x0000
#define RTW_IEEE80211_STYPE_DATA_CFACK	0x0010
#define RTW_IEEE80211_STYPE_DATA_CFPOLL	0x0020
#define RTW_IEEE80211_STYPE_DATA_CFACKPOLL	0x0030
#define RTW_IEEE80211_STYPE_NULLFUNC	0x0040
#define RTW_IEEE80211_STYPE_CFACK		0x0050
#define RTW_IEEE80211_STYPE_CFPOLL		0x0060
#define RTW_IEEE80211_STYPE_CFACKPOLL	0x0070
#define RTW_IEEE80211_STYPE_QOS_DATA		0x0080
#define RTW_IEEE80211_STYPE_QOS_DATA_CFACK		0x0090
#define RTW_IEEE80211_STYPE_QOS_DATA_CFPOLL		0x00A0
#define RTW_IEEE80211_STYPE_QOS_DATA_CFACKPOLL	0x00B0
#define RTW_IEEE80211_STYPE_QOS_NULLFUNC	0x00C0
#define RTW_IEEE80211_STYPE_QOS_CFACK		0x00D0
#define RTW_IEEE80211_STYPE_QOS_CFPOLL		0x00E0
#define RTW_IEEE80211_STYPE_QOS_CFACKPOLL	0x00F0

/* sequence control field */
#define RTW_IEEE80211_SCTL_FRAG	0x000F
#define RTW_IEEE80211_SCTL_SEQ	0xFFF0


#define RTW_ERP_INFO_NON_ERP_PRESENT BIT(0)
#define RTW_ERP_INFO_USE_PROTECTION BIT(1)
#define RTW_ERP_INFO_BARKER_PREAMBLE_MODE BIT(2)

/* QoS,QOS */
#define NORMAL_ACK			0
#define NO_ACK				1
#define NON_EXPLICIT_ACK	2
#define BLOCK_ACK			3

#ifndef ETH_P_PAE
#define ETH_P_PAE 0x888E /* Port Access Entity (IEEE 802.1X) */
#endif /* ETH_P_PAE */

#define ETH_P_PREAUTH 0x88C7 /* IEEE 802.11i pre-authentication */

#define ETH_P_ECONET	0x0018

#ifndef ETH_P_80211_RAW
#define ETH_P_80211_RAW (ETH_P_ECONET + 1)
#endif

/* IEEE 802.11 defines */

#define P80211_OUI_LEN 3

#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW) || defined(PLATFORM_FREEBSD)

struct ieee80211_snap_hdr {

        u8    dsap;   /* always 0xAA */
        u8    ssap;   /* always 0xAA */
        u8    ctrl;   /* always 0x03 */
        u8    oui[P80211_OUI_LEN];    /* organizational universal id */

} __attribute__ ((packed));

#endif

#ifdef PLATFORM_WINDOWS

#pragma pack(1)
struct ieee80211_snap_hdr {

        u8    dsap;   /* always 0xAA */
        u8    ssap;   /* always 0xAA */
        u8    ctrl;   /* always 0x03 */
        u8    oui[P80211_OUI_LEN];    /* organizational universal id */

};
#pragma pack()

#endif


#define SNAP_SIZE sizeof(struct ieee80211_snap_hdr)

#define WLAN_FC_GET_TYPE(fc) ((fc) & RTW_IEEE80211_FCTL_FTYPE)
#define WLAN_FC_GET_STYPE(fc) ((fc) & RTW_IEEE80211_FCTL_STYPE)

#define WLAN_QC_GET_TID(qc) ((qc) & 0x0f)

#define WLAN_GET_SEQ_FRAG(seq) ((seq) & RTW_IEEE80211_SCTL_FRAG)
#define WLAN_GET_SEQ_SEQ(seq)  ((seq) & RTW_IEEE80211_SCTL_SEQ)

/* Authentication algorithms */
#define WLAN_AUTH_OPEN 0
#define WLAN_AUTH_SHARED_KEY 1

#define WLAN_AUTH_CHALLENGE_LEN 128

#define WLAN_CAPABILITY_BSS (1<<0)
#define WLAN_CAPABILITY_IBSS (1<<1)
#define WLAN_CAPABILITY_CF_POLLABLE (1<<2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST (1<<3)
#define WLAN_CAPABILITY_PRIVACY (1<<4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE (1<<5)
#define WLAN_CAPABILITY_PBCC (1<<6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY (1<<7)
#define WLAN_CAPABILITY_SHORT_SLOT (1<<10)

/* Status codes */
#define WLAN_STATUS_SUCCESS 0
#define WLAN_STATUS_UNSPECIFIED_FAILURE 1
#define WLAN_STATUS_CAPS_UNSUPPORTED 10
#define WLAN_STATUS_REASSOC_NO_ASSOC 11
#define WLAN_STATUS_ASSOC_DENIED_UNSPEC 12
#define WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG 13
#define WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION 14
#define WLAN_STATUS_CHALLENGE_FAIL 15
#define WLAN_STATUS_AUTH_TIMEOUT 16
#define WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA 17
#define WLAN_STATUS_ASSOC_DENIED_RATES 18
/* 802.11b */
#define WLAN_STATUS_ASSOC_DENIED_NOSHORT 19
#define WLAN_STATUS_ASSOC_DENIED_NOPBCC 20
#define WLAN_STATUS_ASSOC_DENIED_NOAGILITY 21

/* Reason codes */
#define WLAN_REASON_UNSPECIFIED 1
#define WLAN_REASON_PREV_AUTH_NOT_VALID 2
#define WLAN_REASON_DEAUTH_LEAVING 3
#define WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY 4
#define WLAN_REASON_DISASSOC_AP_BUSY 5
#define WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA 6
#define WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA 7
#define WLAN_REASON_DISASSOC_STA_HAS_LEFT 8
#define WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH 9
#define WLAN_REASON_JOIN_WRONG_CHANNEL       65534
#define WLAN_REASON_EXPIRATION_CHK 65535

/* Information Element IDs */
#define WLAN_EID_SSID 0
#define WLAN_EID_SUPP_RATES 1
#define WLAN_EID_FH_PARAMS 2
#define WLAN_EID_DS_PARAMS 3
#define WLAN_EID_CF_PARAMS 4
#define WLAN_EID_TIM 5
#define WLAN_EID_IBSS_PARAMS 6
#define WLAN_EID_CHALLENGE 16
/* EIDs defined by IEEE 802.11h - START */
#define WLAN_EID_PWR_CONSTRAINT 32
#define WLAN_EID_PWR_CAPABILITY 33
#define WLAN_EID_TPC_REQUEST 34
#define WLAN_EID_TPC_REPORT 35
#define WLAN_EID_SUPPORTED_CHANNELS 36
#define WLAN_EID_CHANNEL_SWITCH 37
#define WLAN_EID_MEASURE_REQUEST 38
#define WLAN_EID_MEASURE_REPORT 39
#define WLAN_EID_QUITE 40
#define WLAN_EID_IBSS_DFS 41
/* EIDs defined by IEEE 802.11h - END */
#define WLAN_EID_ERP_INFO 42
#define WLAN_EID_HT_CAP 45
#define WLAN_EID_RSN 48
#define WLAN_EID_EXT_SUPP_RATES 50
#define WLAN_EID_MOBILITY_DOMAIN 54
#define WLAN_EID_FAST_BSS_TRANSITION 55
#define WLAN_EID_TIMEOUT_INTERVAL 56
#define WLAN_EID_RIC_DATA 57
#define WLAN_EID_HT_OPERATION 61
#define WLAN_EID_SECONDARY_CHANNEL_OFFSET 62
#define WLAN_EID_20_40_BSS_COEXISTENCE 72
#define WLAN_EID_20_40_BSS_INTOLERANT 73
#define WLAN_EID_OVERLAPPING_BSS_SCAN_PARAMS 74
#define WLAN_EID_MMIE 76
#define WLAN_EID_VENDOR_SPECIFIC 221
#define WLAN_EID_GENERIC (WLAN_EID_VENDOR_SPECIFIC)

#define IEEE80211_MGMT_HDR_LEN 24
#define IEEE80211_DATA_HDR3_LEN 24
#define IEEE80211_DATA_HDR4_LEN 30


#define IEEE80211_STATMASK_SIGNAL (1<<0)
#define IEEE80211_STATMASK_RSSI (1<<1)
#define IEEE80211_STATMASK_NOISE (1<<2)
#define IEEE80211_STATMASK_RATE (1<<3)
#define IEEE80211_STATMASK_WEMASK 0x7


#define IEEE80211_CCK_MODULATION    (1<<0)
#define IEEE80211_OFDM_MODULATION   (1<<1)

#define IEEE80211_24GHZ_BAND     (1<<0)
#define IEEE80211_52GHZ_BAND     (1<<1)

#define IEEE80211_CCK_RATE_LEN  		4
#define IEEE80211_NUM_OFDM_RATESLEN	8


#define IEEE80211_CCK_RATE_1MB		        0x02
#define IEEE80211_CCK_RATE_2MB		        0x04
#define IEEE80211_CCK_RATE_5MB		        0x0B
#define IEEE80211_CCK_RATE_11MB		        0x16
#define IEEE80211_OFDM_RATE_LEN 		8
#define IEEE80211_OFDM_RATE_6MB		        0x0C
#define IEEE80211_OFDM_RATE_9MB		        0x12
#define IEEE80211_OFDM_RATE_12MB		0x18
#define IEEE80211_OFDM_RATE_18MB		0x24
#define IEEE80211_OFDM_RATE_24MB		0x30
#define IEEE80211_OFDM_RATE_36MB		0x48
#define IEEE80211_OFDM_RATE_48MB		0x60
#define IEEE80211_OFDM_RATE_54MB		0x6C
#define IEEE80211_BASIC_RATE_MASK		0x80

#define IEEE80211_CCK_RATE_1MB_MASK		(1<<0)
#define IEEE80211_CCK_RATE_2MB_MASK		(1<<1)
#define IEEE80211_CCK_RATE_5MB_MASK		(1<<2)
#define IEEE80211_CCK_RATE_11MB_MASK		(1<<3)
#define IEEE80211_OFDM_RATE_6MB_MASK		(1<<4)
#define IEEE80211_OFDM_RATE_9MB_MASK		(1<<5)
#define IEEE80211_OFDM_RATE_12MB_MASK		(1<<6)
#define IEEE80211_OFDM_RATE_18MB_MASK		(1<<7)
#define IEEE80211_OFDM_RATE_24MB_MASK		(1<<8)
#define IEEE80211_OFDM_RATE_36MB_MASK		(1<<9)
#define IEEE80211_OFDM_RATE_48MB_MASK		(1<<10)
#define IEEE80211_OFDM_RATE_54MB_MASK		(1<<11)

#define IEEE80211_CCK_RATES_MASK	        0x0000000F
#define IEEE80211_CCK_BASIC_RATES_MASK	(IEEE80211_CCK_RATE_1MB_MASK | \
	IEEE80211_CCK_RATE_2MB_MASK)
#define IEEE80211_CCK_DEFAULT_RATES_MASK	(IEEE80211_CCK_BASIC_RATES_MASK | \
        IEEE80211_CCK_RATE_5MB_MASK | \
        IEEE80211_CCK_RATE_11MB_MASK)

#define IEEE80211_OFDM_RATES_MASK		0x00000FF0
#define IEEE80211_OFDM_BASIC_RATES_MASK	(IEEE80211_OFDM_RATE_6MB_MASK | \
	IEEE80211_OFDM_RATE_12MB_MASK | \
	IEEE80211_OFDM_RATE_24MB_MASK)
#define IEEE80211_OFDM_DEFAULT_RATES_MASK	(IEEE80211_OFDM_BASIC_RATES_MASK | \
	IEEE80211_OFDM_RATE_9MB_MASK  | \
	IEEE80211_OFDM_RATE_18MB_MASK | \
	IEEE80211_OFDM_RATE_36MB_MASK | \
	IEEE80211_OFDM_RATE_48MB_MASK | \
	IEEE80211_OFDM_RATE_54MB_MASK)
#define IEEE80211_DEFAULT_RATES_MASK (IEEE80211_OFDM_DEFAULT_RATES_MASK | \
                                IEEE80211_CCK_DEFAULT_RATES_MASK)

#define IEEE80211_NUM_OFDM_RATES	    8
#define IEEE80211_NUM_CCK_RATES	            4
#define IEEE80211_OFDM_SHIFT_MASK_A         4




/* NOTE: This data is for statistical purposes; not all hardware provides this
 *       information for frames received.  Not setting these will not cause
 *       any adverse affects. */
struct ieee80211_rx_stats {
	//u32 mac_time[2];
	s8 rssi;
	u8 signal;
	u8 noise;
	u8 received_channel;
	u16 rate; /* in 100 kbps */
	//u8 control;
	u8 mask;
	u8 freq;
	u16 len;
};

/* IEEE 802.11 requires that STA supports concurrent reception of at least
 * three fragmented frames. This define can be increased to support more
 * concurrent frames, but it should be noted that each entry can consume about
 * 2 kB of RAM and increasing cache size will slow down frame reassembly. */
#define IEEE80211_FRAG_CACHE_LEN 4

struct ieee80211_frag_entry {
	u32 first_frag_time;
	uint seq;
	uint last_frag;
	uint qos;   //jackson
	uint tid;	//jackson
	struct sk_buff *skb;
	u8 src_addr[ETH_ALEN];
	u8 dst_addr[ETH_ALEN];
};

#ifndef PLATFORM_FREEBSD //Baron BSD has already defined
struct ieee80211_stats {
	uint tx_unicast_frames;
	uint tx_multicast_frames;
	uint tx_fragments;
	uint tx_unicast_octets;
	uint tx_multicast_octets;
	uint tx_deferred_transmissions;
	uint tx_single_retry_frames;
	uint tx_multiple_retry_frames;
	uint tx_retry_limit_exceeded;
	uint tx_discards;
	uint rx_unicast_frames;
	uint rx_multicast_frames;
	uint rx_fragments;
	uint rx_unicast_octets;
	uint rx_multicast_octets;
	uint rx_fcs_errors;
	uint rx_discards_no_buffer;
	uint tx_discards_wrong_sa;
	uint rx_discards_undecryptable;
	uint rx_message_in_msg_fragments;
	uint rx_message_in_bad_msg_fragments;
};
#endif //PLATFORM_FREEBSD
struct ieee80211_softmac_stats{
	uint rx_ass_ok;
	uint rx_ass_err;
	uint rx_probe_rq;
	uint tx_probe_rs;
	uint tx_beacons;
	uint rx_auth_rq;
	uint rx_auth_rs_ok;
	uint rx_auth_rs_err;
	uint tx_auth_rq;
	uint no_auth_rs;
	uint no_ass_rs;
	uint tx_ass_rq;
	uint rx_ass_rq;
	uint tx_probe_rq;
	uint reassoc;
	uint swtxstop;
	uint swtxawake;
};

#define SEC_KEY_1         (1<<0)
#define SEC_KEY_2         (1<<1)
#define SEC_KEY_3         (1<<2)
#define SEC_KEY_4         (1<<3)
#define SEC_ACTIVE_KEY    (1<<4)
#define SEC_AUTH_MODE     (1<<5)
#define SEC_UNICAST_GROUP (1<<6)
#define SEC_LEVEL         (1<<7)
#define SEC_ENABLED       (1<<8)

#define SEC_LEVEL_0      0 /* None */
#define SEC_LEVEL_1      1 /* WEP 40 and 104 bit */
#define SEC_LEVEL_2      2 /* Level 1 + TKIP */
#define SEC_LEVEL_2_CKIP 3 /* Level 1 + CKIP */
#define SEC_LEVEL_3      4 /* Level 2 + CCMP */

#define WEP_KEYS 4
#define WEP_KEY_LEN 13



#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW)

struct ieee80211_security {
	u16 active_key:2,
            enabled:1,
	    auth_mode:2,
            auth_algo:4,
            unicast_uses_group:1;
	u8 key_sizes[WEP_KEYS];
	u8 keys[WEP_KEYS][WEP_KEY_LEN];
	u8 level;
	u16 flags;
} __attribute__ ((packed));

#endif

#ifdef PLATFORM_WINDOWS

#pragma pack(1)
struct ieee80211_security {
	u16 active_key:2,
            enabled:1,
	    auth_mode:2,
            auth_algo:4,
            unicast_uses_group:1;
	u8 key_sizes[WEP_KEYS];
	u8 keys[WEP_KEYS][WEP_KEY_LEN];
	u8 level;
	u16 flags;
} ;
#pragma pack()

#endif

/*

 802.11 data frame from AP

      ,-------------------------------------------------------------------.
Bytes |  2   |  2   |    6    |    6    |    6    |  2   | 0..2312 |   4  |
      |------|------|---------|---------|---------|------|---------|------|
Desc. | ctrl | dura |  DA/RA  |   TA    |    SA   | Sequ |  frame  |  fcs |
      |      | tion | (BSSID) |         |         | ence |  data   |      |
      `-------------------------------------------------------------------'

Total: 28-2340 bytes

*/

struct ieee80211_header_data {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[6];
	u8 addr2[6];
	u8 addr3[6];
	u16 seq_ctrl;
};

#define BEACON_PROBE_SSID_ID_POSITION 12

/* Management Frame Information Element Types */
#define MFIE_TYPE_SSID       0
#define MFIE_TYPE_RATES      1
#define MFIE_TYPE_FH_SET     2
#define MFIE_TYPE_DS_SET     3
#define MFIE_TYPE_CF_SET     4
#define MFIE_TYPE_TIM        5
#define MFIE_TYPE_IBSS_SET   6
#define MFIE_TYPE_CHALLENGE  16
#define MFIE_TYPE_ERP        42
#define MFIE_TYPE_RSN	     48
#define MFIE_TYPE_RATES_EX   50
#define MFIE_TYPE_GENERIC    221

#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW)

struct ieee80211_info_element_hdr {
	u8 id;
	u8 len;
} __attribute__ ((packed));

struct ieee80211_info_element {
	u8 id;
	u8 len;
	u8 data[0];
} __attribute__ ((packed));
#endif

#ifdef PLATFORM_WINDOWS

#pragma pack(1)
struct ieee80211_info_element_hdr {
	u8 id;
	u8 len;
} ;

struct ieee80211_info_element {
	u8 id;
	u8 len;
	u8 data[0];
} ;
#pragma pack()

#endif


/*
 * These are the data types that can make up management packets
 *
	u16 auth_algorithm;
	u16 auth_sequence;
	u16 beacon_interval;
	u16 capability;
	u8 current_ap[ETH_ALEN];
	u16 listen_interval;
	struct {
		u16 association_id:14, reserved:2;
	} __attribute__ ((packed));
	u32 time_stamp[2];
	u16 reason;
	u16 status;
*/

#define IEEE80211_DEFAULT_TX_ESSID "Penguin"
#define IEEE80211_DEFAULT_BASIC_RATE 10


#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW)


struct ieee80211_authentication {
	struct ieee80211_header_data header;
	u16 algorithm;
	u16 transaction;
	u16 status;
	//struct ieee80211_info_element_hdr info_element;
} __attribute__ ((packed));


struct ieee80211_probe_response {
	struct ieee80211_header_data header;
	u32 time_stamp[2];
	u16 beacon_interval;
	u16 capability;
	struct ieee80211_info_element info_element;
} __attribute__ ((packed));

struct ieee80211_probe_request {
	struct ieee80211_header_data header;
	/*struct ieee80211_info_element info_element;*/
} __attribute__ ((packed));

struct ieee80211_assoc_request_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 listen_interval;
	//u8 current_ap[ETH_ALEN];
	struct ieee80211_info_element_hdr info_element;
} __attribute__ ((packed));

struct ieee80211_assoc_response_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 status;
	u16 aid;
//	struct ieee80211_info_element info_element; /* supported rates */
} __attribute__ ((packed));
#endif



#ifdef PLATFORM_WINDOWS

#pragma pack(1)

struct ieee80211_authentication {
	struct ieee80211_header_data header;
	u16 algorithm;
	u16 transaction;
	u16 status;
	//struct ieee80211_info_element_hdr info_element;
} ;


struct ieee80211_probe_response {
	struct ieee80211_header_data header;
	u32 time_stamp[2];
	u16 beacon_interval;
	u16 capability;
	struct ieee80211_info_element info_element;
} ;

struct ieee80211_probe_request {
	struct ieee80211_header_data header;
	/*struct ieee80211_info_element info_element;*/
} ;

struct ieee80211_assoc_request_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 listen_interval;
	//u8 current_ap[ETH_ALEN];
	struct ieee80211_info_element_hdr info_element;
} ;

struct ieee80211_assoc_response_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 status;
	u16 aid;
//	struct ieee80211_info_element info_element; /* supported rates */
};

#pragma pack()

#endif




struct ieee80211_txb {
	u8 nr_frags;
	u8 encrypted;
	u16 reserved;
	u16 frag_size;
	u16 payload_size;
	struct sk_buff *fragments[0];
};


/* SWEEP TABLE ENTRIES NUMBER*/
#define MAX_SWEEP_TAB_ENTRIES		  42
#define MAX_SWEEP_TAB_ENTRIES_PER_PACKET  7
/* MAX_RATES_LENGTH needs to be 12.  The spec says 8, and many APs
 * only use 8, and then use extended rates for the remaining supported
 * rates.  Other APs, however, stick all of their supported rates on the
 * main rates information element... */
#define MAX_RATES_LENGTH                  ((u8)12)
#define MAX_RATES_EX_LENGTH               ((u8)16)
#define MAX_NETWORK_COUNT                  128
#define MAX_CHANNEL_NUMBER                 161
#define IEEE80211_SOFTMAC_SCAN_TIME	  400
//(HZ / 2)
#define IEEE80211_SOFTMAC_ASSOC_RETRY_TIME (HZ * 2)

#define CRC_LENGTH                 4U

#define MAX_WPA_IE_LEN (256)
#define MAX_WPS_IE_LEN (512)
#define MAX_P2P_IE_LEN (256)
#define MAX_WFD_IE_LEN (128)

#define NETWORK_EMPTY_ESSID (1<<0)
#define NETWORK_HAS_OFDM    (1<<1)
#define NETWORK_HAS_CCK     (1<<2)

#define IEEE80211_DTIM_MBCAST 4
#define IEEE80211_DTIM_UCAST 2
#define IEEE80211_DTIM_VALID 1
#define IEEE80211_DTIM_INVALID 0

#define IEEE80211_PS_DISABLED 0
#define IEEE80211_PS_UNICAST IEEE80211_DTIM_UCAST
#define IEEE80211_PS_MBCAST IEEE80211_DTIM_MBCAST
#define IW_ESSID_MAX_SIZE 32
#if 0
struct ieee80211_network {
	/* These entries are used to identify a unique network */
	u8 bssid[ETH_ALEN];
	u8 channel;
	/* Ensure null-terminated for any debug msgs */
	u8 ssid[IW_ESSID_MAX_SIZE + 1];
	u8 ssid_len;
	u8	rssi;	//relative signal strength
	u8	sq;		//signal quality

	/* These are network statistics */
	//struct ieee80211_rx_stats stats;
	u16 capability;
	u16	aid;
	u8 rates[MAX_RATES_LENGTH];
	u8 rates_len;
	u8 rates_ex[MAX_RATES_EX_LENGTH];
	u8 rates_ex_len;
	
	u8 edca_parmsets[18];
		
	u8 mode;
	u8 flags;
	u8 time_stamp[8];
	u16 beacon_interval;
	u16 listen_interval;
	u16 atim_window;
	u8 wpa_ie[MAX_WPA_IE_LEN];
	size_t wpa_ie_len;
	u8 rsn_ie[MAX_WPA_IE_LEN];
	size_t rsn_ie_len;
	u8 country[6];
	u8 dtim_period;
	u8 dtim_data;
	u8 power_constraint;
	u8 qosinfo;
	u8 qbssload[5];
	u8 network_type;
	int join_res;
	unsigned long	last_scanned;	
};
#endif
/*
join_res:
-1: authentication fail
-2: association fail
> 0: TID
*/

#ifndef PLATFORM_FREEBSD //Baron BSD has already defined

enum ieee80211_state {

	/* the card is not linked at all */
	IEEE80211_NOLINK = 0,
	
	/* IEEE80211_ASSOCIATING* are for BSS client mode
	 * the driver shall not perform RX filtering unless
	 * the state is LINKED.
	 * The driver shall just check for the state LINKED and
	 * defaults to NOLINK for ALL the other states (including
	 * LINKED_SCANNING)
	 */
	
	/* the association procedure will start (wq scheduling)*/
	IEEE80211_ASSOCIATING,
	IEEE80211_ASSOCIATING_RETRY,
	
	/* the association procedure is sending AUTH request*/
	IEEE80211_ASSOCIATING_AUTHENTICATING,
	
	/* the association procedure has successfully authentcated
	 * and is sending association request
	 */
	IEEE80211_ASSOCIATING_AUTHENTICATED,
	
	/* the link is ok. the card associated to a BSS or linked
	 * to a ibss cell or acting as an AP and creating the bss
	 */
	IEEE80211_LINKED,
	
	/* same as LINKED, but the driver shall apply RX filter
	 * rules as we are in NO_LINK mode. As the card is still
	 * logically linked, but it is doing a syncro site survey
	 * then it will be back to LINKED state.
	 */
	IEEE80211_LINKED_SCANNING,
	
};
#endif //PLATFORM_FREEBSD

#define DEFAULT_MAX_SCAN_AGE (15 * HZ)
#define DEFAULT_FTS 2346
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ARG(x) ((u8*)(x))[0],((u8*)(x))[1],((u8*)(x))[2],((u8*)(x))[3],((u8*)(x))[4],((u8*)(x))[5]

#ifdef PLATFORM_FREEBSD //Baron change func to macro
#define is_multicast_mac_addr(Addr) ((((Addr[0]) & 0x01) == 0x01) && ((Addr[0]) != 0xff))
#define is_broadcast_mac_addr(Addr) ((((Addr[0]) & 0xff) == 0xff) && (((Addr[1]) & 0xff) == 0xff) && \
(((Addr[2]) & 0xff) == 0xff) && (((Addr[3]) & 0xff) == 0xff) && (((Addr[4]) & 0xff) == 0xff) && \
(((Addr[5]) & 0xff) == 0xff))
#else
extern __inline int is_multicast_mac_addr(const u8 *addr)
{
        return ((addr[0] != 0xff) && (0x01 & addr[0]));
}

extern __inline int is_broadcast_mac_addr(const u8 *addr)
{
	return ((addr[0] == 0xff) && (addr[1] == 0xff) && (addr[2] == 0xff) &&   \
		(addr[3] == 0xff) && (addr[4] == 0xff) && (addr[5] == 0xff));
}
#endif //PLATFORM_FREEBSD

#define CFG_IEEE80211_RESERVE_FCS (1<<0)
#define CFG_IEEE80211_COMPUTE_FCS (1<<1)

typedef struct tx_pending_t{
	int frag;
	struct ieee80211_txb *txb;
}tx_pending_t;



#define MAXTID	16

#define IEEE_A            (1<<0)
#define IEEE_B            (1<<1)
#define IEEE_G            (1<<2)
#define IEEE_MODE_MASK    (IEEE_A|IEEE_B|IEEE_G)

//Baron move to ieee80211.c
int ieee80211_is_empty_essid(const char *essid, int essid_len);
int ieee80211_get_hdrlen(u16 fc);

#if 0
/* Action frame categories (IEEE 802.11-2007, 7.3.1.11, Table 7-24) */
#define WLAN_ACTION_SPECTRUM_MGMT 0
#define WLAN_ACTION_QOS 1
#define WLAN_ACTION_DLS 2
#define WLAN_ACTION_BLOCK_ACK 3
#define WLAN_ACTION_RADIO_MEASUREMENT 5
#define WLAN_ACTION_FT 6
#define WLAN_ACTION_SA_QUERY 8
#define WLAN_ACTION_WMM 17
#endif

 enum ieee80211_band {
	IEEE80211_BAND_2GHZ = NL80211_BAND_2GHZ,
	IEEE80211_BAND_5GHZ = NL80211_BAND_5GHZ,
	//IEEE80211_BAND_60GHZ = NL80211_BAND_60GHZ,

	/* keep last */
	IEEE80211_NUM_BANDS
};
/* Action category code */
enum rtw_ieee80211_category {
	RTW_WLAN_CATEGORY_SPECTRUM_MGMT = 0,
	RTW_WLAN_CATEGORY_QOS = 1,
	RTW_WLAN_CATEGORY_DLS = 2,
	RTW_WLAN_CATEGORY_BACK = 3,
	RTW_WLAN_CATEGORY_PUBLIC = 4, //IEEE 802.11 public action frames
	RTW_WLAN_CATEGORY_RADIO_MEASUREMENT  = 5,
	RTW_WLAN_CATEGORY_FT = 6,
	RTW_WLAN_CATEGORY_HT = 7,
	RTW_WLAN_CATEGORY_SA_QUERY = 8,
	RTW_WLAN_CATEGORY_TDLS = 12,
	RTW_WLAN_CATEGORY_WMM = 17,
	RTW_WLAN_CATEGORY_P2P = 0x7f,//P2P action frames
};

/* SPECTRUM_MGMT action code */
enum rtw_ieee80211_spectrum_mgmt_actioncode {
	RTW_WLAN_ACTION_SPCT_MSR_REQ = 0,
	RTW_WLAN_ACTION_SPCT_MSR_RPRT = 1,
	RTW_WLAN_ACTION_SPCT_TPC_REQ = 2,
	RTW_WLAN_ACTION_SPCT_TPC_RPRT = 3,
	RTW_WLAN_ACTION_SPCT_CHL_SWITCH = 4,
	RTW_WLAN_ACTION_SPCT_EXT_CHL_SWITCH = 5,
};

enum _PUBLIC_ACTION{
	ACT_PUBLIC_BSSCOEXIST = 0, // 20/40 BSS Coexistence
	ACT_PUBLIC_DSE_ENABLE = 1,
	ACT_PUBLIC_DSE_DEENABLE = 2,
	ACT_PUBLIC_DSE_REG_LOCATION = 3,
	ACT_PUBLIC_EXT_CHL_SWITCH = 4,
	ACT_PUBLIC_DSE_MSR_REQ = 5,
	ACT_PUBLIC_DSE_MSR_RPRT = 6,
	ACT_PUBLIC_MP = 7, // Measurement Pilot
	ACT_PUBLIC_DSE_PWR_CONSTRAINT = 8,
	ACT_PUBLIC_VENDOR = 9, // for WIFI_DIRECT
	ACT_PUBLIC_GAS_INITIAL_REQ = 10,
	ACT_PUBLIC_GAS_INITIAL_RSP = 11,
	ACT_PUBLIC_GAS_COMEBACK_REQ = 12,
	ACT_PUBLIC_GAS_COMEBACK_RSP = 13,
	ACT_PUBLIC_TDLS_DISCOVERY_RSP = 14,
	ACT_PUBLIC_LOCATION_TRACK = 15,
	ACT_PUBLIC_MAX
};

#ifdef CONFIG_TDLS
enum TDLS_ACTION_FIELD{
	TDLS_SETUP_REQUEST = 0,
	TDLS_SETUP_RESPONSE = 1,
	TDLS_SETUP_CONFIRM = 2,
	TDLS_TEARDOWN = 3,
	TDLS_PEER_TRAFFIC_INDICATION = 4,
	TDLS_CHANNEL_SWITCH_REQUEST = 5,
	TDLS_CHANNEL_SWITCH_RESPONSE = 6,
	TDLS_PEER_PSM_REQUEST = 7,
	TDLS_PEER_PSM_RESPONSE = 8,
	TDLS_PEER_TRAFFIC_RESPONSE = 9,
	TDLS_DISCOVERY_REQUEST = 10,
	TDLS_DISCOVERY_RESPONSE = 14,	//it's used in public action frame
};

#define	TUNNELED_PROBE_REQ	15
#define	TUNNELED_PROBE_RSP	16
#endif //CONFIG_TDLS

/* BACK action code */
enum rtw_ieee80211_back_actioncode {
	RTW_WLAN_ACTION_ADDBA_REQ = 0,
	RTW_WLAN_ACTION_ADDBA_RESP = 1,
	RTW_WLAN_ACTION_DELBA = 2,
};

/* HT features action code */
enum rtw_ieee80211_ht_actioncode {
	RTW_WLAN_ACTION_NOTIFY_CH_WIDTH = 0,
       RTW_WLAN_ACTION_SM_PS = 1,
       RTW_WLAN_ACTION_PSPM = 2,
       RTW_WLAN_ACTION_PCO_PHASE = 3,
       RTW_WLAN_ACTION_MIMO_CSI_MX = 4,
       RTW_WLAN_ACTION_MIMO_NONCP_BF = 5,
       RTW_WLAN_ACTION_MIMP_CP_BF = 6,
       RTW_WLAN_ACTION_ASEL_INDICATES_FB = 7,
       RTW_WLAN_ACTION_HI_INFO_EXCHG = 8,
};

/* BACK (block-ack) parties */
enum rtw_ieee80211_back_parties {
	RTW_WLAN_BACK_RECIPIENT = 0,
	RTW_WLAN_BACK_INITIATOR = 1,
	RTW_WLAN_BACK_TIMER = 2,
};


#define OUI_MICROSOFT 0x0050f2 /* Microsoft (also used in Wi-Fi specs)
				* 00:50:F2 */
#ifndef PLATFORM_FREEBSD //Baron BSD has defined
#define WME_OUI_TYPE 2
#endif //PLATFORM_FREEBSD
#define WME_OUI_SUBTYPE_INFORMATION_ELEMENT 0
#define WME_OUI_SUBTYPE_PARAMETER_ELEMENT 1
#define WME_OUI_SUBTYPE_TSPEC_ELEMENT 2
#define WME_VERSION 1

#define WME_ACTION_CODE_SETUP_REQUEST 0
#define WME_ACTION_CODE_SETUP_RESPONSE 1
#define WME_ACTION_CODE_TEARDOWN 2

#define WME_SETUP_RESPONSE_STATUS_ADMISSION_ACCEPTED 0
#define WME_SETUP_RESPONSE_STATUS_INVALID_PARAMETERS 1
#define WME_SETUP_RESPONSE_STATUS_REFUSED 3

#define WME_TSPEC_DIRECTION_UPLINK 0
#define WME_TSPEC_DIRECTION_DOWNLINK 1
#define WME_TSPEC_DIRECTION_BI_DIRECTIONAL 3


#define OUI_BROADCOM 0x00904c /* Broadcom (Epigram) */

#define VENDOR_HT_CAPAB_OUI_TYPE 0x33 /* 00-90-4c:0x33 */

/**
 * enum rtw_ieee80211_channel_flags - channel flags
 *
 * Channel flags set by the regulatory control code.
 *
 * @RTW_IEEE80211_CHAN_DISABLED: This channel is disabled.
 * @RTW_IEEE80211_CHAN_PASSIVE_SCAN: Only passive scanning is permitted
 *      on this channel.
 * @RTW_IEEE80211_CHAN_NO_IBSS: IBSS is not allowed on this channel.
 * @RTW_IEEE80211_CHAN_RADAR: Radar detection is required on this channel.
 * @RTW_IEEE80211_CHAN_NO_HT40PLUS: extension channel above this channel
 *      is not permitted.
 * @RTW_IEEE80211_CHAN_NO_HT40MINUS: extension channel below this channel
 *      is not permitted.
 */
  enum rtw_ieee80211_channel_flags {
          RTW_IEEE80211_CHAN_DISABLED         = 1<<0,
          RTW_IEEE80211_CHAN_PASSIVE_SCAN     = 1<<1,
          RTW_IEEE80211_CHAN_NO_IBSS          = 1<<2,
          RTW_IEEE80211_CHAN_RADAR            = 1<<3,
          RTW_IEEE80211_CHAN_NO_HT40PLUS      = 1<<4,
          RTW_IEEE80211_CHAN_NO_HT40MINUS     = 1<<5,
  };

enum wiphy_params_flags {
	WIPHY_PARAM_RETRY_SHORT		= 1 << 0,
	WIPHY_PARAM_RETRY_LONG		= 1 << 1,
	WIPHY_PARAM_FRAG_THRESHOLD	= 1 << 2,
	WIPHY_PARAM_RTS_THRESHOLD	= 1 << 3,
	WIPHY_PARAM_COVERAGE_CLASS	= 1 << 4,
	WIPHY_PARAM_DYN_ACK		= 1 << 5,
};
 
  #define RTW_IEEE80211_CHAN_NO_HT40 \
          (RTW_IEEE80211_CHAN_NO_HT40PLUS | RTW_IEEE80211_CHAN_NO_HT40MINUS)

#define IEEE80211_MAX_DATA_LEN_DMG	7920
#define IEEE80211_HT_MCS_MASK_LEN		10
#define IEEE80211_MAX_SSID_LEN		32
#define WLAN_MAX_KEY_LEN		32

struct survey_info {
	struct ieee80211_channel *channel;
	u64 time;
	u64 time_busy;
	u64 time_ext_busy;
	u64 time_rx;
	u64 time_tx;
	u64 time_scan;
	u32 filled;
	s8 noise;
};

struct ieee80211_rate {
	u32 flags;
	u16 bitrate;
	u16 hw_value, hw_value_short;
};

struct ieee80211_mcs_info {
	u8 rx_mask[IEEE80211_HT_MCS_MASK_LEN];
	__le16 rx_highest;
	u8 tx_params;
	u8 reserved[3];
};

struct cfg80211_crypto_settings {
	u32 wpa_versions;
	u32 cipher_group;
	int n_ciphers_pairwise;
	u32 ciphers_pairwise[NL80211_MAX_NR_CIPHER_SUITES];
	int n_akm_suites;
	u32 akm_suites[NL80211_MAX_NR_AKM_SUITES];
	bool control_port;
	__be16 control_port_ethertype;
	bool control_port_no_encrypt;
};

struct ieee80211_channel {
	//enum ieee80211_band band;
        int band;
	u16 center_freq;
	u16 hw_value;
	u32 flags;
	int max_antenna_gain;
	int max_power;
	int max_reg_power;
	bool beacon_found;
	u32 orig_flags;
	int orig_mag, orig_mpwr;
	//enum nl80211_dfs_state dfs_state;
        int dfs_state;
	unsigned long dfs_state_entered;
	unsigned int dfs_cac_ms;
};

struct ieee80211_vht_mcs_info {
	__le16 rx_mcs_map;
	__le16 rx_highest;
	__le16 tx_mcs_map;
	__le16 tx_highest;
};

struct ieee80211_vht_cap {
	__le32 vht_cap_info;
	struct ieee80211_vht_mcs_info supp_mcs;
} ;
struct ieee80211_ht_cap {
	__le16 cap_info;
	u8 ampdu_params_info;

	/* 16 bytes MCS information */
	struct ieee80211_mcs_info mcs;

	__le16 extended_ht_cap_info;
	__le32 tx_BF_cap_info;
	u8 antenna_selection_info;
} ;

struct cfg80211_connect_params {
	struct ieee80211_channel *channel;
	struct ieee80211_channel *channel_hint;
	const u8 *bssid;
	const u8 *bssid_hint;
	const u8 *ssid;
	size_t ssid_len;
	enum nl80211_auth_type auth_type;
	const u8 *ie;
	size_t ie_len;
	bool privacy;
	enum nl80211_mfp mfp;
	struct cfg80211_crypto_settings crypto;
	const u8 *key;
	u8 key_len, key_idx;
	u32 flags;
	int bg_scan_period;
	struct ieee80211_ht_cap ht_capa;
	struct ieee80211_ht_cap ht_capa_mask;
	struct ieee80211_vht_cap vht_capa;
	struct ieee80211_vht_cap vht_capa_mask;
};

struct ieee80211_sta_ht_cap {
	u16 cap; /* use IEEE80211_HT_CAP_ */
	bool ht_supported;
	u8 ampdu_factor;
	u8 ampdu_density;
	struct ieee80211_mcs_info mcs;
};

struct ieee80211_sta_vht_cap {
	bool vht_supported;
	u32 cap; /* use IEEE80211_VHT_CAP_ */
	struct ieee80211_vht_mcs_info vht_mcs;
};

struct ieee80211_supported_band {
	struct ieee80211_channel *channels;
	struct ieee80211_rate *bitrates;
	enum ieee80211_band band;
	int n_channels;
	int n_bitrates;
	struct ieee80211_sta_ht_cap ht_cap;
	struct ieee80211_sta_vht_cap vht_cap;
};
struct key_params {
	const u8 *key;
	const u8 *seq;
	int key_len;
	int seq_len;
	u32 cipher;
};

struct cfg80211_cached_keys {
	struct key_params params[6];
	u8 data[6][WLAN_MAX_KEY_LEN];
	int def, defmgmt;
};

struct rb_node {
	unsigned long  __rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} ;

struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *head);
};
#define rcu_head callback_head

struct cfg80211_bss_ies {
	u64 tsf;
	struct rcu_head rcu_head;
	int len;
	bool from_beacon;
	u8 data[];
};

#define __rcu

struct cfg80211_bss { //linux 2.6.35
	struct ieee80211_channel *channel;

	u8 bssid[ETH_ALEN];
	u64 tsf;
	u16 beacon_interval;
	u16 capability;
	u8 *information_elements;
	size_t len_information_elements;
	u8 *beacon_ies;
	size_t len_beacon_ies;
	u8 *proberesp_ies;
	size_t len_proberesp_ies;

	s32 signal;

	void (*free_priv)(struct cfg80211_bss *bss);
	u8 priv[0] __attribute__((__aligned__(sizeof(void *))));
};

struct cfg80211_internal_bss {
	struct list_head list;
	struct list_head hidden_list;
	struct rb_node rbn;
	unsigned long ts;
	unsigned long refcount;
	atomic_t hold;

	/* must be last because of priv member */
	struct cfg80211_bss pub;
};

struct mac_address {
	u8 addr[ETH_ALEN];
};

struct ieee80211_iface_limit {
	u16 max;
	u16 types;
};

struct ieee80211_iface_combination {
	const struct ieee80211_iface_limit *limits;
	u32 num_different_channels;
	u16 max_interfaces;
	u8 n_limits;
	bool beacon_int_infra_match;
	u8 radar_detect_widths;
	u8 radar_detect_regions;
};

struct ieee80211_txrx_stypes {
	u16 tx, rx;
};

struct regulatory_request {
	struct rcu_head rcu_head;
	int wiphy_idx;
	enum nl80211_reg_initiator initiator;
	enum nl80211_user_reg_hint_type user_reg_hint_type;
	char alpha2[2];
	enum nl80211_dfs_regions dfs_region;
	bool intersect;
	bool processed;
	enum environment_cap country_ie_env;
	struct list_head list;
};

typedef struct {
#ifdef CONFIG_NET_NS
	struct net *net;
#endif
} possible_net_t;

struct wiphy {
	/* assign these fields before you register the wiphy */

	/* permanent MAC address(es) */
	u8 perm_addr[ETH_ALEN];
	u8 addr_mask[ETH_ALEN];

	struct mac_address *addresses;

	const struct ieee80211_txrx_stypes *mgmt_stypes;

	const struct ieee80211_iface_combination *iface_combinations;
	int n_iface_combinations;
	u16 software_iftypes;

	u16 n_addresses;

	/* Supported interface modes, OR together BIT(NL80211_IFTYPE_...) */
	u16 interface_modes;

	u16 max_acl_mac_addrs;

	u32 flags, regulatory_flags, features;
	u8 ext_features[DIV_ROUND_UP(NUM_NL80211_EXT_FEATURES, 8)];

	u32 ap_sme_capa;

	enum cfg80211_signal_type signal_type;

	int bss_priv_size;
	u8 max_scan_ssids;
	u8 max_sched_scan_ssids;
	u8 max_match_sets;
	u16 max_scan_ie_len;
	u16 max_sched_scan_ie_len;

	int n_cipher_suites;
	const u32 *cipher_suites;

	u8 retry_short;
	u8 retry_long;
	u32 frag_threshold;
	u32 rts_threshold;
	u8 coverage_class;

	char fw_version[ETHTOOL_FWVERS_LEN];
	u32 hw_version;

#ifdef CONFIG_PM
	const struct wiphy_wowlan_support *wowlan;
	struct cfg80211_wowlan *wowlan_config;
#endif

	u16 max_remain_on_channel_duration;

	u8 max_num_pmkids;

	u32 available_antennas_tx;
	u32 available_antennas_rx;

	/*
	 * Bitmap of supported protocols for probe response offloading
	 * see &enum nl80211_probe_resp_offload_support_attr. Only valid
	 * when the wiphy flag @WIPHY_FLAG_AP_PROBE_RESP_OFFLOAD is set.
	 */
	u32 probe_resp_offload;

	const u8 *extended_capabilities, *extended_capabilities_mask;
	u8 extended_capabilities_len;

	/* If multiple wiphys are registered and you're handed e.g.
	 * a regular netdev with assigned ieee80211_ptr, you won't
	 * know whether it points to a wiphy your driver has registered
	 * or not. Assign this to something global to your driver to
	 * help determine whether you own this wiphy or not. */
	const void *privid;

	struct ieee80211_supported_band *bands[IEEE80211_NUM_BANDS];

	/* Lets us get back the wiphy on the callback */
	void (*reg_notifier)(struct wiphy *wiphy,
			     struct regulatory_request *request);

	/* fields below are read-only, assigned by cfg80211 */

	const struct ieee80211_regdomain __rcu *regd;

	/* the item in /sys/class/ieee80211/ points to this,
	 * you need use set_wiphy_dev() (see below) */
	struct device dev;

	/* protects ->resume, ->suspend sysfs callbacks against unregister hw */
	bool registered;

	/* dir in debugfs: ieee80211/<wiphyname> */
	struct dentry *debugfsdir;

	const struct ieee80211_ht_cap *ht_capa_mod_mask;
	const struct ieee80211_vht_cap *vht_capa_mod_mask;

	/* the network namespace this phy lives in currently */
	possible_net_t _net;

#ifdef CONFIG_CFG80211_WEXT
	const struct iw_handler_def *wext;
#endif

	const struct wiphy_coalesce_support *coalesce;

	const struct wiphy_vendor_command *vendor_commands;
	const struct nl80211_vendor_cmd_info *vendor_events;
	int n_vendor_commands, n_vendor_events;

	u16 max_ap_assoc_sta;

	u8 max_num_csa_counters;
	u8 max_adj_channel_rssi_comp;

	char priv[0] ;
};


struct wireless_dev {
	struct wiphy *wiphy;
	enum nl80211_iftype iftype;

	/* the remainder of this struct should be private to cfg80211 */
	struct list_head list;
	struct net_device *netdev;

	u32 identifier;

	struct list_head mgmt_registrations;
	spinlock_t mgmt_registrations_lock;

	struct mutex mtx;

	bool use_4addr, p2p_started;

	u8 address[ETH_ALEN] __aligned(sizeof(u16));

	/* currently used for IBSS and SME - might be rearranged later */
	u8 ssid[IEEE80211_MAX_SSID_LEN];
	u8 ssid_len, mesh_id_len, mesh_id_up_len;
	//struct cfg80211_conn *conn;
	struct cfg80211_cached_keys *connect_keys;

	struct list_head event_list;
	spinlock_t event_lock;

	struct cfg80211_internal_bss *current_bss; /* associated / joined */
	///struct cfg80211_chan_def preset_chandef;
	//struct cfg80211_chan_def chandef;

	bool ibss_fixed;
	bool ibss_dfs_possible;

	bool ps;
	int ps_timeout;

	int beacon_interval;

	u32 ap_unexpected_nlportid;

	bool cac_started;
	unsigned long cac_start_time;
	unsigned int cac_time_ms;

	u32 owner_nlportid;

#ifdef CONFIG_CFG80211_WEXT
	/* wext data */
	struct {
		struct cfg80211_ibss_params ibss;
		struct cfg80211_connect_params connect;
		struct cfg80211_cached_keys *keys;
		const u8 *ie;
		size_t ie_len;
		u8 bssid[ETH_ALEN], prev_bssid[ETH_ALEN];
		u8 ssid[IEEE80211_MAX_SSID_LEN];
		s8 default_key, default_mgmt_key;
		bool prev_bssid_valid;
	} wext;
#endif
};

/* Represent channel details, subset of ieee80211_channel */
struct rtw_ieee80211_channel {
	//enum ieee80211_band band;
	//u16 center_freq;
	u16 hw_value;
	u32 flags;
	//int max_antenna_gain;
	//int max_power;
	//int max_reg_power;
	//bool beacon_found;
	//u32 orig_flags;
	//int orig_mag;
	//int orig_mpwr;
}; 

#define CHAN_FMT \
	/*"band:%d, "*/ \
	/*"center_freq:%u, "*/ \
	"hw_value:%u, " \
	"flags:0x%08x" \
	/*"max_antenna_gain:%d\n"*/ \
	/*"max_power:%d\n"*/ \
	/*"max_reg_power:%d\n"*/ \
	/*"beacon_found:%u\n"*/ \
	/*"orig_flags:0x%08x\n"*/ \
	/*"orig_mag:%d\n"*/ \
	/*"orig_mpwr:%d\n"*/ 

#define CHAN_ARG(channel) \
	/*(channel)->band*/ \
	/*, (channel)->center_freq*/ \
	(channel)->hw_value \
	, (channel)->flags \
	/*, (channel)->max_antenna_gain*/ \
	/*, (channel)->max_power*/ \
	/*, (channel)->max_reg_power*/ \
	/*, (channel)->beacon_found*/ \
	/*, (channel)->orig_flags*/ \
	/*, (channel)->orig_mag*/ \
	/*, (channel)->orig_mpwr*/ \

/* Parsed Information Elements */
struct rtw_ieee802_11_elems {
	u8 *ssid;
	u8 ssid_len;
	u8 *supp_rates;
	u8 supp_rates_len;
	u8 *fh_params;
	u8 fh_params_len;
	u8 *ds_params;
	u8 ds_params_len;
	u8 *cf_params;
	u8 cf_params_len;
	u8 *tim;
	u8 tim_len;
	u8 *ibss_params;
	u8 ibss_params_len;
	u8 *challenge;
	u8 challenge_len;
	u8 *erp_info;
	u8 erp_info_len;
	u8 *ext_supp_rates;
	u8 ext_supp_rates_len;
	u8 *wpa_ie;
	u8 wpa_ie_len;
	u8 *rsn_ie;
	u8 rsn_ie_len;
	u8 *wme;
	u8 wme_len;
	u8 *wme_tspec;
	u8 wme_tspec_len;
	u8 *wps_ie;
	u8 wps_ie_len;
	u8 *power_cap;
	u8 power_cap_len;
	u8 *supp_channels;
	u8 supp_channels_len;
	u8 *mdie;
	u8 mdie_len;
	u8 *ftie;
	u8 ftie_len;
	u8 *timeout_int;
	u8 timeout_int_len;
	u8 *ht_capabilities;
	u8 ht_capabilities_len;
	u8 *ht_operation;
	u8 ht_operation_len;
	u8 *vendor_ht_cap;
	u8 vendor_ht_cap_len;
};

typedef enum { ParseOK = 0, ParseUnknown = 1, ParseFailed = -1 } ParseRes;

ParseRes rtw_ieee802_11_parse_elems(u8 *start, uint len,
				struct rtw_ieee802_11_elems *elems,
				int show_errors);

u8 *rtw_set_fixed_ie(unsigned char *pbuf, unsigned int len, unsigned char *source, unsigned int *frlen);
u8 *rtw_set_ie(u8 *pbuf, sint index, uint len, u8 *source, uint *frlen);

enum secondary_ch_offset {
	SCN = 0, /* no secondary channel */
	SCA = 1, /* secondary channel above */
	SCB = 3,  /* secondary channel below */
};
u8 secondary_ch_offset_to_hal_ch_offset(u8 ch_offset);
u8 hal_ch_offset_to_secondary_ch_offset(u8 ch_offset);
u8 *rtw_set_ie_ch_switch(u8 *buf, u32 *buf_len, u8 ch_switch_mode, u8 new_ch, u8 ch_switch_cnt);
u8 *rtw_set_ie_secondary_ch_offset(u8 *buf, u32 *buf_len, u8 secondary_ch_offset);
u8 *rtw_set_ie_mesh_ch_switch_parm(u8 *buf, u32 *buf_len, u8 ttl, u8 flags, u16 reason, u16 precedence);

u8 *rtw_get_ie(u8*pbuf, sint index, sint *len, sint limit);
u8 *rtw_get_ie_ex(u8 *in_ie, uint in_len, u8 eid, u8 *oui, u8 oui_len, u8 *ie, uint *ielen);
int rtw_ies_remove_ie(u8 *ies, uint *ies_len, uint offset, u8 eid, u8 *oui, u8 oui_len);

void rtw_set_supported_rate(u8* SupportedRates, uint mode) ;

unsigned char *rtw_get_wpa_ie(unsigned char *pie, int *wpa_ie_len, int limit);
unsigned char *rtw_get_wpa2_ie(unsigned char *pie, int *rsn_ie_len, int limit);
int rtw_get_wpa_cipher_suite(u8 *s);
int rtw_get_wpa2_cipher_suite(u8 *s);
int rtw_get_wapi_ie(u8 *in_ie,uint in_len,u8 *wapi_ie,u16 *wapi_len);
int rtw_parse_wpa_ie(u8* wpa_ie, int wpa_ie_len, int *group_cipher, int *pairwise_cipher, int *is_8021x);
int rtw_parse_wpa2_ie(u8* wpa_ie, int wpa_ie_len, int *group_cipher, int *pairwise_cipher, int *is_8021x);

int rtw_get_sec_ie(u8 *in_ie,uint in_len,u8 *rsn_ie,u16 *rsn_len,u8 *wpa_ie,u16 *wpa_len);

u8 rtw_is_wps_ie(u8 *ie_ptr, uint *wps_ielen);
u8 *rtw_get_wps_ie(u8 *in_ie, uint in_len, u8 *wps_ie, uint *wps_ielen);
u8 *rtw_get_wps_attr(u8 *wps_ie, uint wps_ielen, u16 target_attr_id ,u8 *buf_attr, u32 *len_attr);
u8 *rtw_get_wps_attr_content(u8 *wps_ie, uint wps_ielen, u16 target_attr_id ,u8 *buf_content, uint *len_content);

/**
 * for_each_ie - iterate over continuous IEs
 * @ie:
 * @buf:
 * @buf_len:
 */
#define for_each_ie(ie, buf, buf_len) \
	for (ie = (void*)buf; (((u8*)ie) - ((u8*)buf) + 1) < buf_len; ie = (void*)(((u8*)ie) + *(((u8*)ie)+1) + 2))

void dump_ies(u8 *buf, u32 buf_len);
void dump_wps_ie(u8 *ie, u32 ie_len);

#ifdef CONFIG_P2P
void dump_p2p_ie(u8 *ie, u32 ie_len);
u8 *rtw_get_p2p_ie(u8 *in_ie, int in_len, u8 *p2p_ie, uint *p2p_ielen);
u8 *rtw_get_p2p_attr(u8 *p2p_ie, uint p2p_ielen, u8 target_attr_id ,u8 *buf_attr, u32 *len_attr);
u8 *rtw_get_p2p_attr_content(u8 *p2p_ie, uint p2p_ielen, u8 target_attr_id ,u8 *buf_content, uint *len_content);
u32 rtw_set_p2p_attr_content(u8 *pbuf, u8 attr_id, u16 attr_len, u8 *pdata_attr);
void rtw_WLAN_BSSID_EX_remove_p2p_attr(WLAN_BSSID_EX *bss_ex, u8 attr_id);
#endif

#ifdef CONFIG_WFD
int rtw_get_wfd_ie(u8 *in_ie, int in_len, u8 *wfd_ie, uint *wfd_ielen);
int rtw_get_wfd_attr_content(u8 *wfd_ie, uint wfd_ielen, u8 target_attr_id ,u8 *attr_content, uint *attr_contentlen);
#endif // CONFIG_WFD

uint	rtw_get_rateset_len(u8	*rateset);

struct registry_priv;
int rtw_generate_ie(struct registry_priv *pregistrypriv);


int rtw_get_bit_value_from_ieee_value(u8 val);

uint	rtw_is_cckrates_included(u8 *rate);

uint	rtw_is_cckratesonly_included(u8 *rate);

int rtw_check_network_type(unsigned char *rate, int ratelen, int channel);

void rtw_get_bcn_info(struct wlan_network *pnetwork);

void rtw_macaddr_cfg(u8 *mac_addr);

u16 rtw_mcs_rate(u8 rf_type, u8 bw_40MHz, u8 short_GI_20, u8 short_GI_40, unsigned char * MCS_rate);

int rtw_action_frame_parse(const u8 *frame, u32 frame_len, u8* category, u8 *action);
const char *action_public_str(u8 action);

#endif /* IEEE80211_H */

