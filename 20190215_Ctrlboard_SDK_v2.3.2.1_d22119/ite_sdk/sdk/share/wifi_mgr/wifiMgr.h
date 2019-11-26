

#if 1//def CFG_NET_WIFI

#include "ite/itp.h"

#define WIFI_SSID_MAXLEN                 64
#define WIFI_PASSWORD_MAXLEN             256
#define WIFI_SECUMODE_MAXLEN             3
#define WIFI_SCAN_LIST_NUMBER     64
#define WIFI_STACK_SIZE                  80000   //(255 * 1024)
#define WIFI_INIFILENAME_MAXLEN          32
#define WIFIMGR_CHECK_WIFI_MSEC          1000

#define WIFI_CONNECT_COUNT               (60 * 10)
#define WIFI_CONNECT_DHCP_COUNT          (10 * 10)
#define WIFI_SET_TIMEMAX                 (2 * 60 * 1000)      // mini second
#define WIFI_SWITCH_MODE_WAIT_TIME       (5 * 1000 * 1000)
#define WIFIMGR_RECONNTECT_MSEC          (60 * 1000)
#define WIFIMGR_TEMPDISCONN_MSEC         (30 * 1000)

#define WIFI_TRIEDSECU_NOSEC             0x1
#define WIFI_TRIEDSECU_WEP               0x2
#define WIFI_TRIEDSECU_WPAPSK            0x4
#define WIFI_TRIEDSECU_WPA2PSK           0x8
#define WIFI_TRIEDSECU_WPAPSK_MIX        0xF //for wpa tool

//Wifi security
#define WIFI_SECUVAL_NOT_AVAILABLE       "NA"
#define WIFI_SECUVAL_NOSEC               "0"
#define WIFI_SECUVAL_WEP                 "1"
#define WIFI_SECUVAL_WPAPSK              "2"
#define WIFI_SECUVAL_WPA2PSK             "3"
#define WIFI_SECUVAL_WPAPSK_MIX          "6" //for wpa tool
#define WIFI_SECUVAL_WPS                 "7" //for wps

typedef enum tagWIFI_TRYAP_PHASE_E
{
    WIFI_TRYAP_PHASE_NONE = 0,
    WIFI_TRYAP_PHASE_SAME_SSID,
    WIFI_TRYAP_PHASE_EMPTY_SSID,
    WIFI_TRYAP_PHASE_FINISH,
    WIFI_TRYAP_PHASE_MAX,
} WIFI_TRYAP_PHASE_E;

typedef enum tagWIFIMGR_ECODE_E
{
    WIFIMGR_ECODE_OK = 0,
    WIFIMGR_ECODE_FAIL,
    WIFIMGR_ECODE_NOT_INIT,
    WIFIMGR_ECODE_ALLOC_MEM,
    WIFIMGR_ECODE_SEM_INIT,
    WIFIMGR_ECODE_MUTEX_INIT,
    WIFIMGR_ECODE_NO_LED,
    WIFIMGR_ECODE_NO_WIFI_DONGLE,
    WIFIMGR_ECODE_NO_INI_FILE,
    WIFIMGR_ECODE_NO_SSID,
    WIFIMGR_ECODE_CONNECT_ERROR,
    WIFIMGR_ECODE_DHCP_ERROR,
    WIFIMGR_ECODE_OPEN_FILE,
    WIFIMGR_ECODE_DTMF_DEC_TIMEOUT,
    WIFIMGR_ECODE_UNKNOWN_SECURITY,
    WIFIMGR_ECODE_SET_DISCONNECT,
    WIFIMGR_ECODE_MAX,
} WIFIMGR_ECODE_E;


typedef enum tagWIFIMGR_CONNSTATE_E
{
    WIFIMGR_CONNSTATE_STOP = 0,
    WIFIMGR_CONNSTATE_SETTING,
    WIFIMGR_CONNSTATE_SCANNING,
    WIFIMGR_CONNSTATE_CONNECTING,
    WIFIMGR_CONNSTATE_MAX,
} WIFIMGR_CONNSTATE_E;

typedef enum tagWIFIMGR_STATE_CALLBACK_E
{
    WIFIMGR_STATE_CALLBACK_CONNECTION_FINISH = 0,
    WIFIMGR_STATE_CALLBACK_CLIENT_MODE_DISCONNECT_30S,
    WIFIMGR_STATE_CALLBACK_CLIENT_MODE_RECONNECTION,
    WIFIMGR_STATE_CALLBACK_CLIENT_MODE_TEMP_DISCONNECT,
    WIFIMGR_STATE_CALLBACK_CLIENT_MODE_CONNECTING_FAIL,
    WIFIMGR_STATE_CALLBACK_SWITCH_CLIENT_SOFTAP_FINISH,
    WIFIMGR_STATE_CALLBACK_MAX,
} WIFIMGR_STATE_CALLBACK_E;


typedef enum tagWIFIMGR_MODE_E
{
    WIFIMGR_MODE_CLIENT = 0,
    WIFIMGR_MODE_SOFTAP,
    WIFIMGR_MODE_MAX,
} WIFIMGR_MODE_E;

typedef enum tagWIFIMGR_SWTICH_ON_OFF_E
{
    WIFIMGR_SWITCH_OFF = 0,
    WIFIMGR_SWITCH_ON,
} WIFIMGR_SWTICH_ON_OFF_E;


typedef struct WIFI_MGR_SETTING_TAG
{
    int  (*wifiCallback)(int nCondition);
    char ssid[WIFI_SSID_MAXLEN];
    char password[WIFI_PASSWORD_MAXLEN];
    char secumode[WIFI_SECUMODE_MAXLEN];
    ITPEthernetSetting setting;    
}WIFI_MGR_SETTING;

typedef struct WIFI_MGR_SCANAP_LIST_TAG
{
    unsigned char name[16];
    unsigned char apMacAddr[6+2];
    int channelId;
    unsigned char ssidName[32];
    int operationMode;
    int securityOn;
    unsigned char rfQualityQuant; //Percent : 0~100
    signed char  rfQualityRSSI; //RSSI
    unsigned char reserved[2];
    int   bitrate;
    int   securityMode;  /*Sec. Mode*/
} WIFI_MGR_SCANAP_LIST; 



int
wifiMgr_is_wifi_available(int *is_available);

int
wifiMgr_get_connect_state(int *conn_state, int *e_code);


// 0 : not ready , 1:ready
int wifiMgr_get_APMode_Ready();

int
WifiMgr_Connecting(void);

int
wifiMgr_set_wps_connect(void);

// 0: client mode, 1:softAP mode 
int wifiMgr_get_wifi_mode(void);

// int wifi mode
int wifiMgr_init(WIFIMGR_MODE_E init_mode, int mp_mode,WIFI_MGR_SETTING wifiSetting);

// cMac : mac address, 6 bytes
int wifiMgr_get_Mac_address(char cMac[6]);

// get all of  WIFI_MGR_SCANAP_LIST, return ap list's conut
int wifiMgr_get_scan_ap_info(WIFI_MGR_SCANAP_LIST* pList);

// terminate wifi mode
int wifiMgr_terminate(void);

// get number of connecting device to ap
int wifiMgr_get_softap_device_number(void);

int wifiMgr_clientMode_sleep_disconnect(void);

int wifiMgr_clientMode_disconnect(void);

int wifiMgr_clientMode_connect_ap(char* ssid,char* password,char* secumode);

// check wifi status; 0: WIFI is turned close, 1: WIFI is turned open
void WifiMgr_clientMode_switch(int status);

// switch Client to soft ap  or  soft ap to Client 
int WifiMgr_Switch_ClientSoftAP_Mode(WIFI_MGR_SETTING wifiSetting);

void wifiMgr_CancelConnect(void);

void wifiMgr_Not_CancelConnect(void);

int WifiMgr_Is_Wpa_Wifi_Terminating(void);

#endif

