/** @file moal_cfg80211.h
  *
  * @brief This file contains the CFG80211 specific defines.
  *
  * Copyright (C) 2011-2014, Marvell International Ltd.
  *
  * This software file (the "File") is distributed by Marvell International
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991
  * (the "License").  You may use, redistribute and/or modify this File in
  * accordance with the terms and conditions of the License, a copy of which
  * is available by writing to the Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
  * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
  * this warranty disclaimer.
  *
  */

#ifndef _MOAL_CFG80211_H_
#define _MOAL_CFG80211_H_

#include    "moal_main.h"

/* Clear all key indexes */
#define KEY_INDEX_CLEAR_ALL             (0x0000000F)

/** RTS/FRAG disabled value */
#define MLAN_FRAG_RTS_DISABLED          (0xFFFFFFFF)

#ifndef WLAN_CIPHER_SUITE_WAPI
#define WLAN_CIPHER_SUITE_WAPI          0x00000020
#endif

#define MRVL_PKT_TYPE_MGMT_FRAME 0xE5

/********* Reference by cfg80211.h************/
/* can we porting cfg80211.h ?? */
#if 1
/* define for custom ie operation */
#define MLAN_CUSTOM_IE_AUTO_IDX_MASK    0xffff
#define MLAN_CUSTOM_IE_DELETE_MASK      0x0
#define MLAN_CUSTOM_IE_NEW_MASK         0x8000
#define TLV_TYPE_MGMT_IE                0x0169
#define MGMT_MASK_ASSOC_REQ             0x01
#define MGMT_MASK_REASSOC_REQ           0x04
#define MGMT_MASK_ASSOC_RESP            0x02
#define MGMT_MASK_REASSOC_RESP          0x08
#define MGMT_MASK_PROBE_REQ             0x10
#define MGMT_MASK_PROBE_RESP            0x20
#define MGMT_MASK_BEACON                0x100
#define MGMT_MASK_BEACON_WPS_P2P        0x8000
#define IE_MASK_WPS						0x0001
#define IE_MASK_P2P						0x0002
#define IE_MASK_WFD						0x0004

#define IEEE80211_FCTL_VERS		0x0003
#define IEEE80211_FCTL_FTYPE		0x000c
#define IEEE80211_FCTL_STYPE		0x00f0
#define IEEE80211_FCTL_TODS		0x0100
#define IEEE80211_FCTL_FROMDS		0x0200
#define IEEE80211_FCTL_MOREFRAGS	0x0400
#define IEEE80211_FCTL_RETRY		0x0800
#define IEEE80211_FCTL_PM		0x1000
#define IEEE80211_FCTL_MOREDATA		0x2000
#define IEEE80211_FCTL_PROTECTED	0x4000
#define IEEE80211_FCTL_ORDER		0x8000
#define IEEE80211_FCTL_CTL_EXT		0x0f00

#define IEEE80211_SCTL_FRAG		0x000F
#define IEEE80211_SCTL_SEQ		0xFFF0

#define IEEE80211_FTYPE_MGMT		0x0000
#define IEEE80211_FTYPE_CTL		0x0004
#define IEEE80211_FTYPE_DATA		0x0008
#define IEEE80211_FTYPE_EXT		0x000c


/* management */
#define IEEE80211_STYPE_ASSOC_REQ	0x0000
#define IEEE80211_STYPE_ASSOC_RESP	0x0010
#define IEEE80211_STYPE_REASSOC_REQ	0x0020
#define IEEE80211_STYPE_REASSOC_RESP	0x0030
#define IEEE80211_STYPE_PROBE_REQ	0x0040
#define IEEE80211_STYPE_PROBE_RESP	0x0050
#define IEEE80211_STYPE_BEACON		0x0080
#define IEEE80211_STYPE_ATIM		0x0090
#define IEEE80211_STYPE_DISASSOC	0x00A0
#define IEEE80211_STYPE_AUTH		0x00B0
#define IEEE80211_STYPE_DEAUTH		0x00C0
#define IEEE80211_STYPE_ACTION		0x00D0

/* control */
#define IEEE80211_STYPE_BACK_REQ	0x0080
#define IEEE80211_STYPE_BACK		0x0090
#define IEEE80211_STYPE_PSPOLL		0x00A0
#define IEEE80211_STYPE_RTS		0x00B0
#define IEEE80211_STYPE_CTS		0x00C0
#define IEEE80211_STYPE_ACK		0x00D0
#define IEEE80211_STYPE_CFEND		0x00E0
#define IEEE80211_STYPE_CFENDACK	0x00F0

/* data */
#define IEEE80211_STYPE_DATA			0x0000
#define IEEE80211_STYPE_DATA_CFACK		0x0010
#define IEEE80211_STYPE_DATA_CFPOLL		0x0020
#define IEEE80211_STYPE_DATA_CFACKPOLL		0x0030
#define IEEE80211_STYPE_NULLFUNC		0x0040
#define IEEE80211_STYPE_CFACK			0x0050
#define IEEE80211_STYPE_CFPOLL			0x0060
#define IEEE80211_STYPE_CFACKPOLL		0x0070
#define IEEE80211_STYPE_QOS_DATA		0x0080
#define IEEE80211_STYPE_QOS_DATA_CFACK		0x0090
#define IEEE80211_STYPE_QOS_DATA_CFPOLL		0x00A0
#define IEEE80211_STYPE_QOS_DATA_CFACKPOLL	0x00B0
#define IEEE80211_STYPE_QOS_NULLFUNC		0x00C0
#define IEEE80211_STYPE_QOS_CFACK		0x00D0
#define IEEE80211_STYPE_QOS_CFPOLL		0x00E0
#define IEEE80211_STYPE_QOS_CFACKPOLL		0x00F0

/* 802.11n HT capabilities masks (for cap_info) */
#define IEEE80211_HT_CAP_LDPC_CODING		0x0001
#define IEEE80211_HT_CAP_SUP_WIDTH_20_40	0x0002
#define IEEE80211_HT_CAP_SM_PS			0x000C
#define	IEEE80211_HT_CAP_SM_PS_SHIFT	2
#define IEEE80211_HT_CAP_GRN_FLD		0x0010
#define IEEE80211_HT_CAP_SGI_20			0x0020
#define IEEE80211_HT_CAP_SGI_40			0x0040
#define IEEE80211_HT_CAP_TX_STBC		0x0080
#define IEEE80211_HT_CAP_RX_STBC		0x0300
#define	IEEE80211_HT_CAP_RX_STBC_SHIFT	8
#define IEEE80211_HT_CAP_DELAY_BA		0x0400
#define IEEE80211_HT_CAP_MAX_AMSDU		0x0800
#define IEEE80211_HT_CAP_DSSSCCK40		0x1000
#define IEEE80211_HT_CAP_RESERVED		0x2000
#define IEEE80211_HT_CAP_40MHZ_INTOLERANT	0x4000
#define IEEE80211_HT_CAP_LSIG_TXOP_PROT		0x8000

/* 802.11n HT capability MSC set */
#define IEEE80211_HT_MCS_RX_HIGHEST_MASK	0x3ff
#define IEEE80211_HT_MCS_TX_DEFINED		0x01
#define IEEE80211_HT_MCS_TX_RX_DIFF		0x02
/* value 0 == 1 stream etc */
#define IEEE80211_HT_MCS_TX_MAX_STREAMS_MASK	0x0C
#define IEEE80211_HT_MCS_TX_MAX_STREAMS_SHIFT	2
#define		IEEE80211_HT_MCS_TX_MAX_STREAMS	4
#define IEEE80211_HT_MCS_TX_UNEQUAL_MODULATION	0x10

/* Information Element IDs */
#define WLAN_EID_SSID 0
#define WLAN_EID_SUPP_RATES 1
#define WLAN_EID_FH_PARAMS 2
#define WLAN_EID_DS_PARAMS 3
#define WLAN_EID_CF_PARAMS 4
#define WLAN_EID_TIM 5
#define WLAN_EID_IBSS_PARAMS 6
#define WLAN_EID_COUNTRY 7
#define WLAN_EID_CHALLENGE 16

#define WLAN_CAPABILITY_ESS		(1<<0)
#define WLAN_CAPABILITY_IBSS		(1<<1)
#define WLAN_CAPABILITY_CF_POLLABLE	(1<<2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST	(1<<3)
#define WLAN_CAPABILITY_PRIVACY		(1<<4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE	(1<<5)
#define WLAN_CAPABILITY_PBCC		(1<<6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY	(1<<7)

/* 802.11h */
#define WLAN_CAPABILITY_SPECTRUM_MGMT	(1<<8)
#define WLAN_CAPABILITY_QOS		(1<<9)
#define WLAN_CAPABILITY_SHORT_SLOT_TIME	(1<<10)
#define WLAN_CAPABILITY_DSSS_OFDM	(1<<13)

#define IEEE80211_COUNTRY_IE_MIN_LEN	6

enum rate_info_flags {
	RATE_INFO_FLAGS_MCS		= 1<<0,
	RATE_INFO_FLAGS_40_MHZ_WIDTH	= 1<<1,
	RATE_INFO_FLAGS_SHORT_GI	= 1<<2,
};

enum ieee80211_channel_flags {
	IEEE80211_CHAN_DISABLED		= 1<<0,
	IEEE80211_CHAN_PASSIVE_SCAN	= 1<<1,
	IEEE80211_CHAN_NO_IBSS		= 1<<2,
	IEEE80211_CHAN_RADAR		= 1<<3,
	IEEE80211_CHAN_NO_HT40PLUS	= 1<<4,
	IEEE80211_CHAN_NO_HT40MINUS	= 1<<5,
};

enum survey_info_flags {
	SURVEY_INFO_NOISE_DBM = 1<<0,
	SURVEY_INFO_IN_USE = 1<<1,
	SURVEY_INFO_CHANNEL_TIME = 1<<2,
	SURVEY_INFO_CHANNEL_TIME_BUSY = 1<<3,
	SURVEY_INFO_CHANNEL_TIME_EXT_BUSY = 1<<4,
	SURVEY_INFO_CHANNEL_TIME_RX = 1<<5,
	SURVEY_INFO_CHANNEL_TIME_TX = 1<<6,
};

enum station_info_flags {
	STATION_INFO_INACTIVE_TIME	= 1<<0,
	STATION_INFO_RX_BYTES		= 1<<1,
	STATION_INFO_TX_BYTES		= 1<<2,
	STATION_INFO_LLID		= 1<<3,
	STATION_INFO_PLID		= 1<<4,
	STATION_INFO_PLINK_STATE	= 1<<5,
	STATION_INFO_SIGNAL		= 1<<6,
	STATION_INFO_TX_BITRATE		= 1<<7,
	STATION_INFO_RX_PACKETS		= 1<<8,
	STATION_INFO_TX_PACKETS		= 1<<9,
};

enum tx_power_setting {
	TX_POWER_AUTOMATIC,
	TX_POWER_LIMITED,
	TX_POWER_FIXED,
};

struct ieee80211_country_ie_triplet {
	union {
		struct {
			u8 first_channel;
			u8 num_channels;
			s8 max_power;
		} ;
		struct {
			u8 reg_extension_id;
			u8 reg_class;
			u8 coverage_class;
		};
	};
} ;

struct rate_info {
	u8 flags;
	u8 mcs;
	u16 legacy;
};

struct station_info {
	u32 filled;
	u32 inactive_time;
	u32 rx_bytes;
	u32 tx_bytes;
	u16 llid;
	u16 plid;
	u8 plink_state;
	s8 signal;
	s8 signal_avg;
	struct rate_info txrate;
	struct rate_info rxrate;
	u32 rx_packets;
	u32 tx_packets;
	u32 tx_retries;
	u32 tx_failed;
	u32 rx_dropped_misc;

	int generation;
};

struct cfg80211_chan_def {
	struct ieee80211_channel *chan;
	enum nl80211_chan_width width;
	u32 center_freq1;
	u32 center_freq2;
};

struct cfg80211_beacon_data {
	const u8 *head, *tail;
	const u8 *beacon_ies;
	const u8 *proberesp_ies;
	const u8 *assocresp_ies;
	const u8 *probe_resp;

	size_t head_len, tail_len;
	size_t beacon_ies_len;
	size_t proberesp_ies_len;
	size_t assocresp_ies_len;
	size_t probe_resp_len;
};

struct cfg80211_acl_data {
	enum nl80211_acl_policy acl_policy;
	int n_acl_entries;

	/* Keep it last */
	struct mac_address mac_addrs[];
};

struct cfg80211_ap_settings {
	struct cfg80211_chan_def chandef;

	struct cfg80211_beacon_data beacon;

	int beacon_interval, dtim_period;
	const u8 *ssid;
	size_t ssid_len;
	enum nl80211_hidden_ssid hidden_ssid;
	struct cfg80211_crypto_settings crypto;
	bool privacy;
	enum nl80211_auth_type auth_type;
	enum nl80211_smps_mode smps_mode;
	int inactivity_timeout;
	u8 p2p_ctwindow;
	bool p2p_opp_ps;
	const struct cfg80211_acl_data *acl;
	bool pbss;
};

struct cfg80211_ssid {
	u8 ssid[IEEE80211_MAX_SSID_LEN];
	u8 ssid_len;
};

struct cfg80211_scan_request {
	struct cfg80211_ssid *ssids;
	int n_ssids;
	u32 n_channels;
	const u8 *ie;
	size_t ie_len;

	/* internal */
	struct wiphy *wiphy;
	struct net_device *dev;
	bool aborted;

	/* keep last */
	struct ieee80211_channel *channels[0];
};

struct beacon_parameters {
	u8 *head, *tail;
	int interval, dtim_period;
	int head_len, tail_len;
	const u8 *ssid;
	size_t ssid_len;
	enum nl80211_hidden_ssid hidden_ssid;
	struct cfg80211_crypto_settings crypto;
	bool privacy;
	enum nl80211_auth_type auth_type;
	const u8 *beacon_ies;
	size_t beacon_ies_len;
	const u8 *proberesp_ies;
	size_t proberesp_ies_len;
	const u8 *assocresp_ies;
	size_t assocresp_ies_len;
	int probe_resp_len;
	u8 *probe_resp;
};

struct cfg80211_ibss_params {
	u8 *ssid;
	u8 *bssid;
	struct ieee80211_channel *channel;
	u8 *ie;
	u8 ssid_len, ie_len;
	u16 beacon_interval;
	bool channel_fixed;
	bool privacy;
};


struct ieee80211_txq_params {
	enum nl80211_txq_q queue;
	u16 txop;
	u16 cwmin;
	u16 cwmax;
	u8 aifs;
};

struct mpath_info {
	u32 filled;
	u32 frame_qlen;
	u32 dsn;
	u32 metric;
	u32 exptime;
	u32 discovery_timeout;
	u8 discovery_retries;
	u8 flags;
};

struct station_parameters {
	u8 *supported_rates;
	struct net_device *vlan;
	u32 station_flags;
	int listen_interval;
	u16 aid;
	u8 supported_rates_len;
	u8 plink_action;
	struct ieee80211_ht_cap *ht_capa;
};

struct bss_parameters {
	int use_cts_prot;
	int use_short_preamble;
	int use_short_slot_time;
	u8 *basic_rates;
	u8 basic_rates_len;
};

struct mesh_config {
	/* Timeouts in ms */
	/* Mesh plink management parameters */
	u16 dot11MeshRetryTimeout;
	u16 dot11MeshConfirmTimeout;
	u16 dot11MeshHoldingTimeout;
	u16 dot11MeshMaxPeerLinks;
	u8  dot11MeshMaxRetries;
	u8  dot11MeshTTL;
	bool auto_open_plinks;
	/* HWMP parameters */
	u8  dot11MeshHWMPmaxPREQretries;
	u32 path_refresh_time;
	u16 min_discovery_timeout;
	u32 dot11MeshHWMPactivePathTimeout;
	u16 dot11MeshHWMPpreqMinInterval;
	u16 dot11MeshHWMPnetDiameterTraversalTime;
};

struct cfg80211_ops {
	int	(*suspend)(struct wiphy *wiphy);
	int	(*resume)(struct wiphy *wiphy);

	int	(*add_virtual_intf)(struct wiphy *wiphy, char *name,
				    enum nl80211_iftype type, u32 *flags,
				    struct vif_params *params);
	int	(*del_virtual_intf)(struct wiphy *wiphy, struct net_device *dev);
	int	(*change_virtual_intf)(struct wiphy *wiphy,
				       struct net_device *dev,
				       enum nl80211_iftype type, u32 *flags,
				       struct vif_params *params);

	int	(*add_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, const u8 *mac_addr,
			   struct key_params *params);
	int	(*get_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, const u8 *mac_addr, void *cookie,
			   void (*callback)(void *cookie, struct key_params*));
	int	(*del_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, const u8 *mac_addr);
	int	(*set_default_key)(struct wiphy *wiphy,
				   struct net_device *netdev,
				   u8 key_index);
	int	(*set_default_mgmt_key)(struct wiphy *wiphy,
					struct net_device *netdev,
					u8 key_index);

	int	(*add_beacon)(struct wiphy *wiphy, struct net_device *dev,
			      struct beacon_parameters *info);
	int	(*set_beacon)(struct wiphy *wiphy, struct net_device *dev,
			      struct beacon_parameters *info);
	int	(*del_beacon)(struct wiphy *wiphy, struct net_device *dev);


	int	(*add_station)(struct wiphy *wiphy, struct net_device *dev,
			       u8 *mac, struct station_parameters *params);
	int	(*del_station)(struct wiphy *wiphy, struct net_device *dev,
			       u8 *mac);
	int	(*change_station)(struct wiphy *wiphy, struct net_device *dev,
				  u8 *mac, struct station_parameters *params);
	int	(*get_station)(struct wiphy *wiphy, struct net_device *dev,
			       u8 *mac, struct station_info *sinfo);
	int	(*dump_station)(struct wiphy *wiphy, struct net_device *dev,
			       int idx, u8 *mac, struct station_info *sinfo);

	int	(*add_mpath)(struct wiphy *wiphy, struct net_device *dev,
			       u8 *dst, u8 *next_hop);
	int	(*del_mpath)(struct wiphy *wiphy, struct net_device *dev,
			       u8 *dst);
	int	(*change_mpath)(struct wiphy *wiphy, struct net_device *dev,
				  u8 *dst, u8 *next_hop);
	int	(*get_mpath)(struct wiphy *wiphy, struct net_device *dev,
			       u8 *dst, u8 *next_hop,
			       struct mpath_info *pinfo);
	int	(*dump_mpath)(struct wiphy *wiphy, struct net_device *dev,
			       int idx, u8 *dst, u8 *next_hop,
			       struct mpath_info *pinfo);
	int	(*get_mesh_params)(struct wiphy *wiphy,
				struct net_device *dev,
				struct mesh_config *conf);
	int	(*set_mesh_params)(struct wiphy *wiphy,
				struct net_device *dev,
				const struct mesh_config *nconf, u32 mask);
	int	(*change_bss)(struct wiphy *wiphy, struct net_device *dev,
			      struct bss_parameters *params);

	int	(*set_txq_params)(struct wiphy *wiphy,
				  struct ieee80211_txq_params *params);

	int	(*set_channel)(struct wiphy *wiphy,
			       struct ieee80211_channel *chan,
			       enum nl80211_channel_type channel_type);

	int	(*scan)(struct wiphy *wiphy, struct net_device *dev,
			struct cfg80211_scan_request *request);

	int	(*auth)(struct wiphy *wiphy, struct net_device *dev,
			struct cfg80211_auth_request *req);
	int	(*assoc)(struct wiphy *wiphy, struct net_device *dev,
			 struct cfg80211_assoc_request *req);
	int	(*deauth)(struct wiphy *wiphy, struct net_device *dev,
			  struct cfg80211_deauth_request *req,
			  void *cookie);
	int	(*disassoc)(struct wiphy *wiphy, struct net_device *dev,
			    struct cfg80211_disassoc_request *req,
			    void *cookie);

	int	(*connect)(struct wiphy *wiphy, struct net_device *dev,
			   struct cfg80211_connect_params *sme);
	int	(*disconnect)(struct wiphy *wiphy, struct net_device *dev,
			      u16 reason_code);

	int	(*join_ibss)(struct wiphy *wiphy, struct net_device *dev,
			     struct cfg80211_ibss_params *params);
	int	(*leave_ibss)(struct wiphy *wiphy, struct net_device *dev);

	int	(*set_wiphy_params)(struct wiphy *wiphy, u32 changed);

	int	(*set_tx_power)(struct wiphy *wiphy,
				enum tx_power_setting type, int dbm);
	int	(*get_tx_power)(struct wiphy *wiphy, int *dbm);

	int	(*set_wds_peer)(struct wiphy *wiphy, struct net_device *dev,
				u8 *addr);

	void	(*rfkill_poll)(struct wiphy *wiphy);

#ifdef CONFIG_NL80211_TESTMODE
	int	(*testmode_cmd)(struct wiphy *wiphy, void *data, int len);
#endif

	int	(*set_bitrate_mask)(struct wiphy *wiphy,
				    struct net_device *dev,
				    const u8 *peer,
				    const struct cfg80211_bitrate_mask *mask);

	int	(*dump_survey)(struct wiphy *wiphy, struct net_device *netdev,
			int idx, struct survey_info *info);

	int	(*set_pmksa)(struct wiphy *wiphy, struct net_device *netdev,
			     struct cfg80211_pmksa *pmksa);
	int	(*del_pmksa)(struct wiphy *wiphy, struct net_device *netdev,
			     struct cfg80211_pmksa *pmksa);
	int	(*flush_pmksa)(struct wiphy *wiphy, struct net_device *netdev);

	/* some temporary stuff to finish wext */
	int	(*set_power_mgmt)(struct wiphy *wiphy, struct net_device *dev,
				  bool enabled, int timeout);
};

static inline bool ieee80211_is_probe_resp(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_RESP);
}


#endif
/********/

/**
 * If multiple wiphys are registered e.g. a regular netdev with
 * assigned ieee80211_ptr and you won't know whether it points
 * to a wiphy your driver has registered or not. Assign this to
 * something global to your driver to help determine whether
 * you own this wiphy or not.
 */
static const void *const mrvl_wiphy_privid = &mrvl_wiphy_privid;

/* Get the private structure from wiphy */
void *woal_get_wiphy_priv(struct wiphy *wiphy);

/* Get the private structure from net device */
void *woal_get_netdev_priv(struct net_device *dev);

t_u8 woal_band_cfg_to_ieee_band(t_u32 band);

int woal_cfg80211_change_virtual_intf(struct wiphy *wiphy,
				      struct net_device *dev,
				      enum nl80211_iftype type,
				      u32 * flags, struct vif_params *params);

int woal_cfg80211_set_wiphy_params(struct wiphy *wiphy, u32 changed);

int woal_cfg80211_add_key(struct wiphy *wiphy,
			  struct net_device *dev, t_u8 key_index,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36) || defined(COMPAT_WIRELESS)
			  bool pairwise,
#endif
			  const t_u8 * mac_addr, struct key_params *params);

int woal_cfg80211_del_key(struct wiphy *wiphy,
			  struct net_device *dev, t_u8 key_index,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36) || defined(COMPAT_WIRELESS)
			  bool pairwise,
#endif
			  const t_u8 * mac_addr);

int woal_cfg80211_set_bitrate_mask(struct wiphy *wiphy,
				   struct net_device *dev,
				   const u8 * peer,
				   const struct cfg80211_bitrate_mask *mask);

int woal_cfg80211_set_antenna(struct wiphy *wiphy, u32 tx_ant, u32 rx_ant);

#ifdef STA_CFG80211
#ifdef STA_SUPPORT
int woal_set_rf_channel(moal_private * priv,
			struct ieee80211_channel *chan,
			enum nl80211_channel_type channel_type,
			t_u8 wait_option);
mlan_status woal_inform_bss_from_scan_result(moal_private * priv,
					     mlan_ssid_bssid * ssid_bssid,
					     t_u8 wait_option);
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 5, 0)
int woal_cfg80211_set_channel(struct wiphy *wiphy,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 34) || defined(COMPAT_WIRELESS)
			      struct net_device *dev,
#endif
			      struct ieee80211_channel *chan,
			      enum nl80211_channel_type channel_type);
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 37) || defined(COMPAT_WIRELESS)
int woal_cfg80211_set_default_key(struct wiphy *wiphy,
				  struct net_device *dev, t_u8 key_index,
				  bool ucast, bool mcast);
#else
int woal_cfg80211_set_default_key(struct wiphy *wiphy,
				  struct net_device *dev, t_u8 key_index);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30) || defined(COMPAT_WIRELESS)
int woal_cfg80211_set_default_mgmt_key(struct wiphy *wiphy,
				       struct net_device *netdev,
				       t_u8 key_index);
#endif

void woal_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
				       struct wireless_dev *wdev,
#else
				       struct net_device *dev,
#endif
				       t_u16 frame_type, bool reg);

int woal_cfg80211_mgmt_tx(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
			  struct wireless_dev *wdev,
#else
			  struct net_device *dev,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
			  struct cfg80211_mgmt_tx_params *params,
#else
			  struct ieee80211_channel *chan, bool offchan,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
			  enum nl80211_channel_type channel_type,
			  bool channel_type_valid,
#endif
			  unsigned int wait, const u8 * buf, size_t len,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0) || defined(COMPAT_WIRELESS)
			  bool no_cck,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)
			  bool dont_wait_for_ack,
#endif
#endif
			  u64 * cookie);

mlan_status woal_register_cfg80211(moal_private * priv);

extern struct ieee80211_supported_band cfg80211_band_2ghz;
extern struct ieee80211_supported_band cfg80211_band_5ghz;
extern const u32 cfg80211_cipher_suites[10];

#if defined(STA_SUPPORT) && defined(UAP_SUPPORT)
int woal_cfg80211_bss_role_cfg(moal_private * priv, t_u16 action,
			       t_u8 * bss_role);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
struct wireless_dev *woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
						    const char *name,
						    enum nl80211_iftype type,
						    u32 * flags,
						    struct vif_params *params);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
struct wireless_dev *woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
						    char *name,
						    enum nl80211_iftype type,
						    u32 * flags,
						    struct vif_params *params);
#else
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 37) || defined(COMPAT_WIRELESS)
struct net_device *woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
						  char *name,
						  enum nl80211_iftype type,
						  u32 * flags,
						  struct vif_params *params);
#else
int woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
				   char *name, enum nl80211_iftype type,
				   u32 * flags, struct vif_params *params);
#endif
#endif
#endif
int woal_cfg80211_del_virt_if(struct wiphy *wiphy, struct net_device *dev);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
int woal_cfg80211_del_virtual_intf(struct wiphy *wiphy,
				   struct wireless_dev *wdev);
#else
int woal_cfg80211_del_virtual_intf(struct wiphy *wiphy, struct net_device *dev);
#endif

#if defined(WIFI_DIRECT_SUPPORT)
/* Group Owner Negotiation Req */
#define P2P_GO_NEG_REQ         0
/* Group Owner Negotiation Rsp */
#define P2P_GO_NEG_RSP         1
/* Group Owner Negotiation Confirm */
#define P2P_GO_NEG_CONF        2
/* P2P Invitation Request */
#define P2P_INVITE_REQ         3
/* P2P Invitation Response */
#define P2P_INVITE_RSP         4
/* Device Discoverability Request */
#define P2P_DEVDIS_REQ         5
/* Device Discoverability Response */
#define P2P_DEVDIS_RSP         6
/* Provision Discovery Request */
#define P2P_PROVDIS_REQ        7
/* Provision Discovery Response */
#define P2P_PROVDIS_RSP        8
/** P2P category */
#define P2P_ACT_FRAME_CATEGORY            0x04
/** P2P oui offset */
#define P2P_ACT_FRAME_OUI_OFFSET          26
/** P2P subtype offset */
#define P2P_ACT_FRAME_OUI_SUBTYPE_OFFSET  30
void woal_cfg80211_display_p2p_actframe(const t_u8 * buf, int len,
					struct ieee80211_channel *chan,
					const t_u8 flag);

/** Define kernel version for wifi direct */
#if !defined(COMPAT_WIRELESS)
#define WIFI_DIRECT_KERNEL_VERSION          KERNEL_VERSION(2, 6, 39)
#else
#define WIFI_DIRECT_KERNEL_VERSION          KERNEL_VERSION(2, 6, 33)
#endif /* COMPAT_WIRELESS */
#if LINUX_VERSION_CODE >= WIFI_DIRECT_KERNEL_VERSION
/** Define for remain on channel duration timer */
#define MAX_REMAIN_ON_CHANNEL_DURATION      (1000)

int woal_cfg80211_init_p2p_client(moal_private * priv);

int woal_cfg80211_init_p2p_go(moal_private * priv);

int woal_cfg80211_deinit_p2p(moal_private * priv);

int woal_cfg80211_remain_on_channel_cfg(moal_private * priv,
					t_u8 wait_option, t_u8 remove,
					t_u8 * status,
					struct ieee80211_channel *chan,
					enum nl80211_channel_type channel_type,
					t_u32 duration);
int woal_uap_cfg80211_get_station(struct wiphy *wiphy, struct net_device *dev,
				  u8 * mac, struct station_info *stainfo);

void woal_remove_virtual_interface(moal_handle * handle);

#endif /* KERNEL_VERSION */
#endif /* WIFI_DIRECT_SUPPORT && V14_FEATURE */

#ifdef UAP_CFG80211
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
int woal_cfg80211_add_beacon(struct wiphy *wiphy,
			     struct net_device *dev,
			     struct cfg80211_ap_settings *params);

int woal_cfg80211_set_beacon(struct wiphy *wiphy,
			     struct net_device *dev,
			     struct cfg80211_beacon_data *params);
#else
int woal_cfg80211_add_beacon(struct wiphy *wiphy,
			     struct net_device *dev,
			     struct beacon_parameters *params);

int woal_cfg80211_set_beacon(struct wiphy *wiphy,
			     struct net_device *dev,
			     struct beacon_parameters *params);
#endif

int woal_cfg80211_del_beacon(struct wiphy *wiphy, struct net_device *dev);
int woal_cfg80211_del_station(struct wiphy *wiphy,
			      struct net_device *dev, u8 * mac_addr);
#endif
void woal_clear_all_mgmt_ies(moal_private * priv);
int woal_cfg80211_mgmt_frame_ie(moal_private * priv,
				const t_u8 * beacon_ies, size_t beacon_ies_len,
				const t_u8 * proberesp_ies,
				size_t proberesp_ies_len,
				const t_u8 * assocresp_ies,
				size_t assocresp_ies_len,
				const t_u8 * probereq_ies,
				size_t probereq_ies_len, t_u16 mask);

t_u8 woal_is_any_interface_active(moal_handle * handle);

int woal_get_active_intf_freq(moal_private * priv);

void woal_cfg80211_setup_ht_cap(struct ieee80211_sta_ht_cap *ht_info,
				t_u32 dev_cap, t_u8 * mcs_set);
int woal_cfg80211_assoc(moal_private * priv, void *sme, t_u8 wait_option);

#endif /* _MOAL_CFG80211_H_ */
