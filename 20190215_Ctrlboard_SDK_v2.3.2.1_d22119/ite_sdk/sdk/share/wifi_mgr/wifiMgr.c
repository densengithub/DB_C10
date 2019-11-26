
#include <pthread.h>
#include "ite/itp.h"
#include "lwip/netif.h"
#include "wifiMgr.h"
#include "../dhcps/dhcps.h"

extern void dhcps_init(void);
extern void dhcps_deinit(void);

#ifdef CFG_NET_WIFI
#include "ite/ite_wifi.h"

static sem_t                             semConnectStart, semConnectStop;
static pthread_mutex_t                   mutexALWiFi, mutexIni, mutexMode;

static sem_t                             semWPSStart, semWPSStop;
static pthread_mutex_t                   mutexALWiFiWPS;

static pthread_t                         clientModeTask, wpsTask, processTask;

static int                               mpMode = 0;
static int                               al_wifi_init = 0;
static int                               gwifiTerminate = 0;
static int                               gwifiWpaTerminate = 0;
static int                               need_set = false;
static int                               wps_cancel_flag = false;
static int                               soft_ap_init_ready = false;
static int                               connect_cancel_flag = false;
static int                               is_first_connect = true;     // first connect when the system start up
static int                               is_wifi_available = false;
static int                               is_temp_disconnect = false;  // is temporary disconnect
static int                               no_ssid = false;
static int                               no_ini_file = false;
static int                               wifi_client_on_off;
static char                              macSrting[32];
static char                              gSsid[WIFI_SSID_MAXLEN];
static char                              gPassword[WIFI_PASSWORD_MAXLEN];
static char                              gSecumode[WIFI_SECUMODE_MAXLEN];
static WIFIMGR_CONNSTATE_E               gWifi_connstate = WIFIMGR_CONNSTATE_STOP;
static WIFIMGR_CONNSTATE_E               wifi_conn_state = WIFIMGR_CONNSTATE_STOP;
static WIFIMGR_ECODE_E                   wifi_conn_ecode = WIFIMGR_ECODE_SET_DISCONNECT;
static WIFIMGR_MODE_E                    wifi_mode = WIFIMGR_MODE_CLIENT;
static WIFI_MGR_SETTING                  gWifiMgrSetting = {0};
static WIFI_MGR_SCANAP_LIST              gWifiMgrApList[WIFI_SCAN_LIST_NUMBER];
static WIFI_MGR_SCANAP_LIST              gWifiMgrTempApList[WIFI_SCAN_LIST_NUMBER];
static struct net_device_info            apInfo = {0}, gScanApInfo = {0};


static int
WifiMgr_WpaInit(void)
{
    int nRet = WIFIMGR_ECODE_OK;
	struct net_device_config netCfg = {0};

#ifndef CFG_NET_WIFI_WPA
    ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_LINK_AP, &netCfg);
#else
    // start wpa state machine
    ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_ENABLE, NULL);
    usleep(1000*1000);

    iteStartWPACtrl();
    do
    {
        usleep(1000);
    } while (!iteWPACtrlIsReady());
#endif

    return nRet;
}


static int
WifiMgr_WpaTerminate(void)
{
    int nRet = WIFIMGR_ECODE_OK;

    iteWPACtrlDisconnectNetwork();

    printf("WifiMgr_WpaTerminate \n");
    iteStopWPACtrl();
    do
    {
        usleep(1000*100);
        printf("wait WifiMgr_WpaTerminate %d %d \n", iteStopWPADone(),iteWPADeinitDone());
    }while(iteStopWPADone()==0 || iteWPADeinitDone()==0);

    usleep(1000*100);

    printf("WifiMgr_WpaTerminate end %d ,%d  \n",iteStopWPADone(),iteWPADeinitDone());
    return nRet;
}


static int
WifiMgr_HostAPInit(void)
{
    int nRet = WIFIMGR_ECODE_OK;
    ITPWifiInfo wifiInfo;

    ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFIAP_ENABLE, NULL);
    usleep(1000*1000);

    if (mpMode)
    {
        ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_GET_INFO, &wifiInfo);
        snprintf(macSrting, sizeof(macSrting), "_%02X:%02X", wifiInfo.hardwareAddress[4], wifiInfo.hardwareAddress[5]);
        iteHostapdSetSSIDWithMac(macSrting);
    }

    dhcps_init();
    usleep(1000*10);

    iteStartHostapdCtrl();

    do
    {
        usleep(1000);
    } while (!iteHOSTAPDCtrlIsReady());

    usleep(1000);

    return nRet;
}


static int
WifiMgr_HostAPTerminate(void)
{
    int nRet = WIFIMGR_ECODE_OK;

    iteStopHostapdCtrl();
    do
    {
        usleep(1000*200);
    }while(!iteStopHostapdDone());
    usleep(1000*200);

    dhcps_deinit();
    usleep(1000*10);

    ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFIAP_DISABLE, NULL);
    usleep(1000*1000);

    return nRet;
}

static int
WifiMgr_ScanAP(void)
{
    int nRet = WIFIMGR_ECODE_OK;
    int nWifiState = 0;
    int i = 0, j = 0, k = 0;
    struct net_device_info tmpApInfo = {0};
    int found1 = 0;

    memset(&apInfo, 0, sizeof(struct net_device_info));

    if (connect_cancel_flag) {
        goto end;
    }

    for (i = 0; i < 5; i++)
    {
        printf("[WIFIMGR]%s() SCAN Round <%ld> ==========================\r\n", __FUNCTION__, i);
        ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_SCAN, NULL);
        while (1)
        {
            nWifiState = ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_STATE, NULL);
            printf("[WIFIMGR]%s() nWifiState=0x%X\r\n", __FUNCTION__, nWifiState);
            if ((nWifiState & WLAN_SITE_MONITOR) == 0) {
                // scan finish
                printf("[WIFIMGR]%s() Scan AP Finish!\r\n", __FUNCTION__);
                break;
            }

            if (connect_cancel_flag) {
                goto end;
            }

            usleep(1000 * 1000);
        }

        memset(&tmpApInfo, 0, sizeof(struct net_device_info));
        read(ITP_DEVICE_WIFI, &tmpApInfo, (size_t)NULL);
        printf("[WIFIMGR]%s() L#%ld: tmpApInfo.apCnt = %ld\r\n", __FUNCTION__, __LINE__, tmpApInfo.apCnt);
        for (j = 0; j < tmpApInfo.apCnt; j++)
        {
            found1 = 0;
            for (k = 0; k < apInfo.apCnt; k++)
            {
                if (!memcmp(apInfo.apList[k].apMacAddr, tmpApInfo.apList[j].apMacAddr, 6)){
                    found1 = 1;
                    break;
                }
            }

            if (!found1 && apInfo.apCnt < WIFI_SCAN_LIST_NUMBER) {
                memcpy(&apInfo.apList[apInfo.apCnt], &tmpApInfo.apList[j], sizeof(apInfo.apList[0]));
                apInfo.apCnt++;
            }

            if (connect_cancel_flag) {
                goto end;
            }
        }

        if (connect_cancel_flag) {
            goto end;
        }
    }

    if (connect_cancel_flag) {
        goto end;
    }

    printf("[WIFIMGR]%s() L#%ld: apInfo.apCnt = %ld\r\n", __FUNCTION__, __LINE__, apInfo.apCnt);
    for (i = 0; i < apInfo.apCnt; i++)
    {
        printf("[WIFIMGR] ssid = %32s, securityOn = %ld, securityMode = %ld, <%02x:%02x:%02x:%02x:%02x:%02x>\r\n", apInfo.apList[i].ssidName, apInfo.apList[i].securityOn, apInfo.apList[i].securityMode,
            apInfo.apList[i].apMacAddr[0], apInfo.apList[i].apMacAddr[1], apInfo.apList[i].apMacAddr[2], apInfo.apList[i].apMacAddr[3], apInfo.apList[i].apMacAddr[4], apInfo.apList[i].apMacAddr[5]);
    }

end:

    if (connect_cancel_flag) {
        printf("[WIFIMGR WPS]%s() L#%ld: End. wps_cancel_flag is set.\r\n", __FUNCTION__, __LINE__);
    }

    return nRet;
}

int
WifiMgr_Connecting(void)
{
    int nRet = WIFIMGR_ECODE_OK;

    if (!al_wifi_init)
        return WIFIMGR_ECODE_NOT_INIT;

    pthread_mutex_lock(&mutexALWiFi);
    if (wifi_conn_state == WIFIMGR_CONNSTATE_STOP) {
        need_set = false;
        sem_post(&semConnectStart);
    }
    pthread_mutex_unlock(&mutexALWiFi);

    return nRet;
}

static int
WifiMgr_Connect(void)
{
    struct net_device_config netCfg = {0};
    ITPEthernetSetting setting;
    ITPWifiInfo wifiInfo;

    int nRet = WIFIMGR_ECODE_OK;
    int is_connected = 0, is_ssid_match = 0, dhcp_available = 0;
    int phase = 0, lastIdx = 0, triedSecu = 0, nSecurity = -1;
    unsigned long connect_cnt = 0;
    char *ssid, *password, *secumode;


    if (connect_cancel_flag) {
        goto end;
    }

    ssid = gSsid;
    password = gPassword;
    secumode = gSecumode;

    if (mpMode) {
        printf("[WIFIMGR] Is mpMode, connect to default SSID.\r\n");
        // SSID
        snprintf(ssid, WIFI_SSID_MAXLEN, "%s", CFG_NET_WIFI_MP_SSID);
        // Password
        snprintf(password, WIFI_PASSWORD_MAXLEN, "%s", CFG_NET_WIFI_MP_PASSWORD);
#ifdef DTMF_DEC_HAS_SECUMODE
        // Security mode
        snprintf(secumode, WIFI_SECUMODE_MAXLEN, "%s", CFG_NET_WIFI_MP_SECURITY);
#endif
    }


    // dhcp
    setting.dhcp = gWifiMgrSetting.setting.dhcp;

    // autoip
    setting.autoip = gWifiMgrSetting.setting.autoip;

    // ipaddr
    setting.ipaddr[0] = gWifiMgrSetting.setting.ipaddr[0];
    setting.ipaddr[1] = gWifiMgrSetting.setting.ipaddr[1];
    setting.ipaddr[2] = gWifiMgrSetting.setting.ipaddr[2];
    setting.ipaddr[3] = gWifiMgrSetting.setting.ipaddr[3];

    // netmask
    setting.netmask[0] = gWifiMgrSetting.setting.netmask[0];
    setting.netmask[1] = gWifiMgrSetting.setting.netmask[1];
    setting.netmask[2] = gWifiMgrSetting.setting.netmask[2];
    setting.netmask[3] = gWifiMgrSetting.setting.netmask[3];

    // gateway
    setting.gw[0] = gWifiMgrSetting.setting.gw[0];
    setting.gw[1] = gWifiMgrSetting.setting.gw[1];
    setting.gw[2] = gWifiMgrSetting.setting.gw[2];
    setting.gw[3] = gWifiMgrSetting.setting.gw[3];

    printf("[WIFIMGR] ssid     = %s\r\n", ssid);
    printf("[WIFIMGR] password = %s\r\n", password);
    printf("[WIFIMGR] secumode = %s\r\n", secumode);

    if (connect_cancel_flag) {
        goto end;
    }

    if (strlen(ssid) == 0)
    {
        printf("[WIFIMGR]%s() L#%ld: Error! Wifi setting has no SSID\r\n", __FUNCTION__, __LINE__);
        nRet = WIFIMGR_ECODE_NO_SSID;
        goto end;
    }

#if defined(CFG_NET_ETHERNET) && defined(CFG_NET_WIFI)
    printf("[WIFIMGR] check wifi netif %d \n",ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_NETIF_STATUS, NULL));
    // Check if the wifi netif is exist
    if (ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_NETIF_STATUS, NULL) == 0) {
        printf("[WIFIMGR]%s() L#%ld: wifi need to add netif !\r\n", __FUNCTION__, __LINE__);
        ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_ADD_NETIF, NULL);
    }
#endif

    memset(&netCfg, 0, sizeof(struct net_device_config));

    if (!strcmp(secumode, WIFI_SECUVAL_NOT_AVAILABLE))
    {
        int i = 0, found1 = 0;

        wifi_conn_state = WIFIMGR_CONNSTATE_SCANNING;
        WifiMgr_ScanAP();
        wifi_conn_state = WIFIMGR_CONNSTATE_CONNECTING;

        phase = WIFI_TRYAP_PHASE_SAME_SSID;
        lastIdx = 0;
        triedSecu = 0;

        if (connect_cancel_flag) {
            goto end;
        }

retry:
        nSecurity = -1;
        found1 = 0;
        for (i = lastIdx; i < apInfo.apCnt; i++)
        {
            is_ssid_match = 0;

            if (phase == WIFI_TRYAP_PHASE_SAME_SSID) {
                // search for the same SSID
                if (!strcmp(ssid, apInfo.apList[i].ssidName)) {
                    is_ssid_match = 1;
                }
            } else if (phase == WIFI_TRYAP_PHASE_EMPTY_SSID) {
                // search for the empty SSID
                if (strlen(apInfo.apList[i].ssidName) == 0) {
                    is_ssid_match = 1;
                }
            }

            if (is_ssid_match)
            {
                nSecurity = (apInfo.apList[i].securityOn) ? apInfo.apList[i].securityMode : WLAN_SEC_NOSEC;
                switch (nSecurity)
                {
                case WLAN_SEC_NOSEC:
                    if ((triedSecu & WIFI_TRIEDSECU_NOSEC) == 0)
                    {
                        triedSecu |= WIFI_TRIEDSECU_NOSEC;
                        found1 = 1;
                    }
                    break;
                case WLAN_SEC_WEP:
                    if ((triedSecu & WIFI_TRIEDSECU_WEP) == 0)
                    {
                        triedSecu |= WIFI_TRIEDSECU_WEP;
                        found1 = 1;
                    }
                    break;
                case WLAN_SEC_WPAPSK:
                    if ((triedSecu & WIFI_TRIEDSECU_WPAPSK) == 0)
                    {
                        triedSecu |= WIFI_TRIEDSECU_WPAPSK;
                        found1 = 1;
                    }
                    break;
                case WLAN_SEC_WPA2PSK:
                    if ((triedSecu & WIFI_TRIEDSECU_WPA2PSK) == 0)
                    {
                        triedSecu |= WIFI_TRIEDSECU_WPA2PSK;
                        found1 = 1;
                    }
                    break;
		case WLAN_SEC_WPAPSK_MIX:
                    if ((triedSecu & WIFI_TRIEDSECU_WPAPSK_MIX) == 0)
                    {
                        triedSecu |= WIFI_TRIEDSECU_WPAPSK_MIX;
                        found1 = 1;
                    }
                    break;
                }
            }

            if (found1)
            {
                printf("[WIFIMGR]%s() Found 1 AP matches! ssid = %s, securityOn = %ld, securityMode = %ld, <%02x:%02x:%02x:%02x:%02x:%02x>\r\n",
                    __FUNCTION__, apInfo.apList[i].ssidName, apInfo.apList[i].securityOn, apInfo.apList[i].securityMode,
                    apInfo.apList[i].apMacAddr[0], apInfo.apList[i].apMacAddr[1], apInfo.apList[i].apMacAddr[2], apInfo.apList[i].apMacAddr[3], apInfo.apList[i].apMacAddr[4], apInfo.apList[i].apMacAddr[5]);
                lastIdx = i + 1;
                break;
            }

            if (connect_cancel_flag)
            {
                goto end;
            }
        }

        if ((triedSecu == WIFI_TRIEDSECU_NOSEC | WIFI_TRIEDSECU_WEP | WIFI_TRIEDSECU_WPAPSK | WIFI_TRIEDSECU_WPA2PSK | WIFI_TRIEDSECU_WPAPSK_MIX) ||
            ((phase == WIFI_TRYAP_PHASE_EMPTY_SSID) && (i == apInfo.apCnt)))
        {
            phase = WIFI_TRYAP_PHASE_FINISH;
        }

        if ((phase == WIFI_TRYAP_PHASE_SAME_SSID) && (i == apInfo.apCnt))
        {
            phase = WIFI_TRYAP_PHASE_EMPTY_SSID;
            lastIdx = 0;
        }

        if (connect_cancel_flag)
        {
            goto end;
        }

        if (found1)
        {
            if (!apInfo.apList[i].securityOn || apInfo.apList[i].securityMode == WLAN_SEC_NOSEC)
            {
                netCfg.operationMode = WLAN_MODE_STA;
                snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
                netCfg.securitySuit.securityMode = WLAN_SEC_NOSEC;
            }
            else if (apInfo.apList[i].securityMode == WLAN_SEC_WEP)
            {
                netCfg.operationMode = WLAN_MODE_STA;
                snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
                netCfg.securitySuit.securityMode = WLAN_SEC_WEP;
                netCfg.securitySuit.authMode = WLAN_AUTH_OPENSYSTEM;
                snprintf(netCfg.securitySuit.wepKeys[0], WIFI_PASSWORD_MAXLEN, password);
                snprintf(netCfg.securitySuit.wepKeys[1], WIFI_PASSWORD_MAXLEN, password);
                snprintf(netCfg.securitySuit.wepKeys[2], WIFI_PASSWORD_MAXLEN, password);
                snprintf(netCfg.securitySuit.wepKeys[3], WIFI_PASSWORD_MAXLEN, password);
                netCfg.securitySuit.defaultKeyId = 0; /*From 0 to 3*/
            }
            else if (apInfo.apList[i].securityMode == WLAN_SEC_WPAPSK)
            {
                netCfg.operationMode = WLAN_MODE_STA;
                snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
                netCfg.securitySuit.securityMode = WLAN_SEC_WPAPSK;
                snprintf(netCfg.securitySuit.preShareKey, WIFI_PASSWORD_MAXLEN, password);
            }
            else if (apInfo.apList[i].securityMode == WLAN_SEC_WPA2PSK)
            {
                netCfg.operationMode = WLAN_MODE_STA;
                snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
                netCfg.securitySuit.securityMode = WLAN_SEC_WPA2PSK;
                snprintf(netCfg.securitySuit.preShareKey, WIFI_PASSWORD_MAXLEN, password);
            }
            else if (apInfo.apList[i].securityMode == WLAN_SEC_WPAPSK_MIX)
            {
                netCfg.operationMode = WLAN_MODE_STA;
                snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
                netCfg.securitySuit.securityMode = WLAN_SEC_WPAPSK_MIX;
                snprintf(netCfg.securitySuit.preShareKey, WIFI_PASSWORD_MAXLEN, password);
            }
        }
        else
        {
            if (phase == WIFI_TRYAP_PHASE_FINISH) {
                printf("[WIFIMGR]%s() L#%ld: Cannot find the same SSID on air. Unknown Security! Cannot connect to WiFi AP!\r\n", __FUNCTION__, __LINE__);
                //ioctl(hLed1, ITP_IOCTL_OFF, NULL);
                nRet = WIFIMGR_ECODE_UNKNOWN_SECURITY;
                goto end;
            } else {
                printf("[WIFIMGR]%s() L#%ld: Cannot find the same SSID on air. Goto retry.\r\n", __FUNCTION__, __LINE__);
                goto retry;
            }
        }
    }
    else if (!strcmp(secumode, WIFI_SECUVAL_NOSEC))
    {
        netCfg.operationMode = WLAN_MODE_STA;
        snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
        netCfg.securitySuit.securityMode = WLAN_SEC_NOSEC;
    }
    else if (!strcmp(secumode, WIFI_SECUVAL_WEP))
    {
        netCfg.operationMode = WLAN_MODE_STA;
        snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
        netCfg.securitySuit.securityMode = WLAN_SEC_WEP;
        netCfg.securitySuit.authMode = WLAN_AUTH_OPENSYSTEM;
        snprintf(netCfg.securitySuit.wepKeys[0], WIFI_PASSWORD_MAXLEN, password);
        snprintf(netCfg.securitySuit.wepKeys[1], WIFI_PASSWORD_MAXLEN, password);
        snprintf(netCfg.securitySuit.wepKeys[2], WIFI_PASSWORD_MAXLEN, password);
        snprintf(netCfg.securitySuit.wepKeys[3], WIFI_PASSWORD_MAXLEN, password);
        netCfg.securitySuit.defaultKeyId = 0; /*From 0 to 3*/
    }
    else if (!strcmp(secumode, WIFI_SECUVAL_WPAPSK))
    {
        netCfg.operationMode = WLAN_MODE_STA;
        snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
        netCfg.securitySuit.securityMode = WLAN_SEC_WPAPSK;
        snprintf(netCfg.securitySuit.preShareKey, WIFI_PASSWORD_MAXLEN, password);
    }
    else if (!strcmp(secumode, WIFI_SECUVAL_WPA2PSK))
    {
        netCfg.operationMode = WLAN_MODE_STA;
        snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
        netCfg.securitySuit.securityMode = WLAN_SEC_WPA2PSK;
        snprintf(netCfg.securitySuit.preShareKey, WIFI_PASSWORD_MAXLEN, password);
    }
    else if (!strcmp(secumode, WIFI_SECUVAL_WPAPSK_MIX))
    {
        netCfg.operationMode = WLAN_MODE_STA;
        snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
        netCfg.securitySuit.securityMode = WLAN_SEC_WPAPSK_MIX;
        snprintf(netCfg.securitySuit.preShareKey, WIFI_PASSWORD_MAXLEN, password);
    } else {
        printf("other secumode %s \n",secumode);
        netCfg.operationMode = WLAN_MODE_STA;
        snprintf(netCfg.ssidName, WIFI_SSID_MAXLEN, ssid);
        netCfg.securitySuit.securityMode = WLAN_SEC_WPAPSK_MIX;
        snprintf(netCfg.securitySuit.preShareKey, WIFI_PASSWORD_MAXLEN, password);
    }

    if (connect_cancel_flag) {
        goto end;
    }

    // try to connect to WiFi AP
	if (wifi_client_on_off){
        iteWPACtrlConnectNetwork(&netCfg);
	}

    // Wait for connecting...
    printf("[WIFIMGR] Wait for connecting\n");
    connect_cnt = WIFI_CONNECT_COUNT;
	while (!ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_CONNECTED, NULL)){
        putchar('.');
		//printf("[WIFIMGR] Wait for ITP_IOCTL_IS_CONNECTED \n");
		usleep(100*1000);
        if (connect_cancel_flag) {
            printf("cancel connect................... \n");
            goto end;
        }
	}

    while (connect_cnt)
    {
        if (iteWPAConnectState())
        {
            printf("\r\n[WIFIMGR] WiFi AP is connected!\r\n");
            is_connected = 1;
            break;
        }
        putchar('.');
        fflush(stdout);
        connect_cnt--;
        if (connect_cnt == 0)
        {
            printf("\r\n[WIFIMGR]%s() L#%ld: Timeout! Cannot connect to %s!\r\n", __FUNCTION__, __LINE__, ssid);
            break;
        }

        if (connect_cancel_flag)
        {
            goto end;
        }
        if (gwifiTerminate) {
            printf("[Wifi mgr]terminate connect stop \n");
            goto end;
        }

        usleep(100000);
    }

    if (connect_cancel_flag)
    {
        goto end;
    }

    if (!is_connected)
    {
        if (!strcmp(secumode, WIFI_SECUVAL_NOT_AVAILABLE) &&
            (phase != WIFI_TRYAP_PHASE_FINISH))
        {
            printf("[WIFIMGR]%s() L#%ld: Error! Cannot connect to WiFi AP! Goto retry.\r\n", __FUNCTION__, __LINE__);
            goto retry;
        }
        else
        {
            printf("[WIFIMGR]%s() L#%ld: Error! Cannot connect to WiFi AP!\r\n", __FUNCTION__, __LINE__);
            //ioctl(hLed1, ITP_IOCTL_OFF, NULL);
            nRet = WIFIMGR_ECODE_CONNECT_ERROR;
            gWifiMgrSetting.wifiCallback(WIFIMGR_STATE_CALLBACK_CLIENT_MODE_CONNECTING_FAIL);
            goto end;
        }
    }

    if (setting.dhcp) {
        // Wait for DHCP setting...
        printf("[WIFIMGR] Wait for DHCP setting");

    	ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_START_DHCP, NULL);

        connect_cnt = WIFI_CONNECT_DHCP_COUNT;
        while (connect_cnt)
        {
            if (ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_AVAIL, NULL))
            {
                printf("\r\n[WIFIMGR] DHCP setting OK\r\n");
                dhcp_available = 1;
                break;
            }
            putchar('.');
            fflush(stdout);
            connect_cnt--;
            if (connect_cnt == 0)
            {
                if (!strcmp(secumode, WIFI_SECUVAL_NOT_AVAILABLE) &&
                    (phase != WIFI_TRYAP_PHASE_FINISH))
                {
                    printf("\r\n[WIFIMGR]%s() L#%ld: DHCP timeout! Goto retry.\r\n", __FUNCTION__, __LINE__);
                    goto retry;
                }
                else
                {
                    printf("\r\n[WIFIMGR]%s() L#%ld: DHCP timeout! connect fail!\r\n", __FUNCTION__, __LINE__);
                    nRet = WIFIMGR_ECODE_DHCP_ERROR;
                    goto end;
                }
            }

            if (connect_cancel_flag)
            {
                goto end;
            }

            usleep(100000);
        }

        if (connect_cancel_flag)
        {
            goto end;
        }
    }
    else
    {
        printf("[WIFIMGR] Manual setting IP\n");
        ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_RESET, &setting);
        dhcp_available = 1;
    }

    if (dhcp_available)
    {
        ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_GET_INFO, &wifiInfo);
        printf("[WIFIMGR] wifiInfo.active          = %ld\r\n", wifiInfo.active);
        printf("[WIFIMGR] wifiInfo.address         = 0x%X\r\n", wifiInfo.address);
        printf("[WIFIMGR] wifiInfo.address         = %ld.%ld.%ld.%ld\r\n",
                (wifiInfo.address & 0xFF), (wifiInfo.address & 0xFF00) >> 8, (wifiInfo.address & 0xFF0000) >> 16, (wifiInfo.address & 0xFF000000) >> 24);
        printf("[WIFIMGR] wifiInfo.displayName     = %s\r\n", wifiInfo.displayName);
        printf("[WIFIMGR] wifiInfo.hardwareAddress = %02X:%02X:%02X:%02X:%02X:%02X\r\n", wifiInfo.hardwareAddress[0], wifiInfo.hardwareAddress[1], wifiInfo.hardwareAddress[2], wifiInfo.hardwareAddress[3], wifiInfo.hardwareAddress[4], wifiInfo.hardwareAddress[5]);
        printf("[WIFIMGR] wifiInfo.name            = %s\r\n", wifiInfo.name);

        //usleep(1000*1000*5); //workaround random miss frames issue for cisco router

        if (gWifiMgrSetting.wifiCallback)
            gWifiMgrSetting.wifiCallback(WIFIMGR_STATE_CALLBACK_CONNECTION_FINISH);
    }

end:

    if (connect_cancel_flag)
    {
        printf("[WIFIMGR]%s() L#%ld: End. connect_cancel_flag is set.\r\n", __FUNCTION__, __LINE__);
    }

    return nRet;
}


static int
WifiMgr_WPS(void)
{
    int nRet = WIFIMGR_ECODE_OK;
    struct net_device_config netCfg = {0};
    unsigned long connect_cnt = 0;
    int is_connected = 0, dhcp_available = 0;
    ITPWifiInfo wifiInfo;
    ITPEthernetSetting setting;

    struct net_device_config wpsNetCfg = {0};
    int len = 0;
    char ssid[WIFI_SSID_MAXLEN];
    char password[WIFI_PASSWORD_MAXLEN];

    if (wps_cancel_flag) {
        goto end;
    }

    netCfg.operationMode = WLAN_MODE_STA;
    memset(netCfg.ssidName, 0, sizeof(netCfg.ssidName));
    netCfg.securitySuit.securityMode = WLAN_SEC_WPS;

    if (ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_CONNECTED, NULL)) {

        iteWPACtrlDisconnectNetwork();
        usleep(1000*100);
        // dhcp
        setting.dhcp = 0;

        // autoip
        setting.autoip = 0;

        // ipaddr
        setting.ipaddr[0] =0;
        setting.ipaddr[1] = 0;
        setting.ipaddr[2] = 0;
        setting.ipaddr[3] = 0;

        // netmask
        setting.netmask[0] = 0;
        setting.netmask[1] = 0;
        setting.netmask[2] = 0;
        setting.netmask[3] = 0;

        // gateway
        setting.gw[0] = 0;
        setting.gw[1] = 0;
        setting.gw[2] = 0;
        setting.gw[3] = 0;

        ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_RESET, &setting);

    }

    iteWPACtrlConnectNetwork(&netCfg);

    if (wps_cancel_flag)
    {
        goto end;
    }

    // Wait for connecting...
    printf("[WIFIMGR WPS] Wait for connecting");
    connect_cnt = WIFI_CONNECT_COUNT;
    while (connect_cnt)
    {
        if (iteWPACtrlWpsIsComplete()) {
            printf("\r\n[WIFIMGR WPS] WiFi AP is connected!\r\n");
            wifiMgr_CancelConnect();
            is_connected = 1;
            break;
        }
        putchar('.');
        fflush(stdout);
        connect_cnt--;
        if (connect_cnt == 0) {
            printf("\r\n[WIFIMGR WPS]%s() L#%ld: Timeout! Cannot connect to WIFI AP!\r\n", __FUNCTION__, __LINE__);
            break;
        }

        if (wps_cancel_flag) {
            goto end;
        }

        usleep(100000);
    }

    if (!is_connected) {
        printf("[WIFIMGR WPS]%s() L#%ld: Error! Cannot connect to WiFi AP!\r\n", __FUNCTION__, __LINE__);
        nRet = WIFIMGR_ECODE_CONNECT_ERROR;
        goto end;
    }

    if (wps_cancel_flag) {
        goto end;
    }

    // Wait for DHCP setting...
    printf("[WIFIMGR WPS] Wait for DHCP setting");
    ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_START_DHCP, NULL);
    connect_cnt = WIFI_CONNECT_COUNT;
    while (connect_cnt)
    {
        if (ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_AVAIL, NULL)) {
            printf("\r\n[WIFIMGR WPS] DHCP setting OK\r\n");
            dhcp_available = 1;
            break;
        }
        putchar('.');
        fflush(stdout);
        connect_cnt--;
        if (connect_cnt == 0) {
            printf("\r\n[WIFIMGR WPS]%s() L#%ld: DHCP timeout! connect fail!\r\n", __FUNCTION__, __LINE__);
            nRet = WIFIMGR_ECODE_DHCP_ERROR;
            goto end;
        }

        if (wps_cancel_flag) {
            goto end;
        }
        usleep(100000);
    }

    if (dhcp_available)
    {
        // Get WPS info
        iteWPACtrlGetNetwork(&wpsNetCfg);
        // trim the " char
        memset(ssid, 0, WIFI_SSID_MAXLEN);
        len = strlen(wpsNetCfg.ssidName);
        memcpy(ssid, wpsNetCfg.ssidName + 1, len - 2);
        memset(password, 0, WIFI_PASSWORD_MAXLEN);
        len = strlen(wpsNetCfg.securitySuit.preShareKey);
        memcpy(password, wpsNetCfg.securitySuit.preShareKey + 1, len - 2);

        printf("[WIFIMGR WPS] WPS Info:\r\n");
        printf("[WIFIMGR WPS] WPS SSID     = %s\r\n", ssid);
        printf("[WIFIMGR WPS] WPS Password = %s\r\n", password);
        printf("[WIFIMGR WPS] WPS Security = %ld\r\n", wpsNetCfg.securitySuit.securityMode);
    }

    end:

    if (wps_cancel_flag)
    {
        printf("[WIFIMGR WPS]%s() L#%ld: End. wps_cancel_flag is set.\r\n", __FUNCTION__, __LINE__);
    }

    return nRet;
}

static void*
WifiMgrWPSThreadFunc(void* arg)
{
    int nRet = WIFIMGR_ECODE_OK;

    while (1)
    {
        sem_wait(&semWPSStart);
        if (gwifiTerminate) {
            printf("[Wifi mgr]terminate WifiMgrWPSThreadFunc \n");
            break;
        }

        printf("[WIFIMGR] START to Connect WPS!\r\n");
        wifi_conn_ecode = WifiMgr_WPS();
        printf("[WIFIMGR] Connect WPS finish!\r\n");


        usleep(1000);
    }

    return NULL;
}

static void*
WiFiMgrProcessThreadFunc(void *arg)
{
    int nRet = 0;
    int bIsAvail = 0, nWiFiConnState = 0, nWiFiConnEcode = 0;
    int nPlayState = 0;

    int nCheckCnt = WIFIMGR_CHECK_WIFI_MSEC;
    static struct timeval tv1 = {0, 0}, tv2 = {0, 0};
    static struct timeval tv3_temp = {0, 0}, tv4_temp = {0, 0};
    long temp_disconn_time = 0;
    int wifi_mode_now = 0, is_softap_ready = 0;

    is_first_connect = true;
    is_temp_disconnect = false;
    no_ini_file = false;
    no_ssid = false;

    usleep(20000);

    while (1)
    {
        nCheckCnt--;
        if (gwifiTerminate) {
            printf("[Wifi mgr]terminate WiFiMgrProcessThreadFunc \n");
            break;
        }

        usleep(1000);
        if (nCheckCnt == 0) {
            wifi_mode_now = wifiMgr_get_wifi_mode();

            if (wifi_mode_now == WIFIMGR_MODE_SOFTAP){
				#ifdef CFG_NET_WIFI_HOSTAPD
                // Soft AP mode
                if (!soft_ap_init_ready) {
                    is_softap_ready = iteHOSTAPDCtrlIsReady();
                    printf("[Main]%s() L#%ld: is_softap_ready=%ld\r\n", __FUNCTION__, __LINE__, is_softap_ready);
                    if (is_softap_ready) {
                        soft_ap_init_ready = true;
                        gWifi_connstate = WIFIMGR_CONNSTATE_STOP;
                        if (gWifiMgrSetting.wifiCallback)
                            gWifiMgrSetting.wifiCallback(WIFIMGR_STATE_CALLBACK_CONNECTION_FINISH);
                    }
                }
				#endif
            }
            else
            {
                // Client mode
                if (is_first_connect) {
                    // first time connect when the system start up
                    nRet = WifiMgr_Connecting();
                    if (nRet == WIFIMGR_ECODE_OK) {
                        gWifi_connstate = WIFIMGR_CONNSTATE_CONNECTING;
                    }
                    is_first_connect = false;

                    goto end;
                }

                if (gWifi_connstate == WIFIMGR_CONNSTATE_SETTING ||
                    gWifi_connstate == WIFIMGR_CONNSTATE_CONNECTING)
                {
                    nRet = wifiMgr_get_connect_state(&nWiFiConnState, &nWiFiConnEcode);
                    if (nWiFiConnState == WIFIMGR_CONNSTATE_STOP) {
                        gWifi_connstate = WIFIMGR_CONNSTATE_STOP;
                        // the connecting was finish
                        if (nWiFiConnEcode == WIFIMGR_ECODE_OK) {
                            nRet = wifiMgr_is_wifi_available(&bIsAvail);
                            if (!bIsAvail) {
                                // fail, restart the timer
                                gettimeofday(&tv1, NULL);
                            }
                        } else {
                            printf("[WIFIMGR]%s() L#%ld: Error! nWiFiConnEcode = 0%ld\r\n", __FUNCTION__, __LINE__, nWiFiConnEcode);

                            // connection has error
                            if (nWiFiConnEcode == WIFIMGR_ECODE_NO_INI_FILE) {
                                no_ini_file = true;
                            }
                            if (nWiFiConnEcode == WIFIMGR_ECODE_NO_SSID) {
                                no_ssid = true;
                            } else {
                                // fail, restart the timer
                                gettimeofday(&tv1, NULL);
                            }
                        }
                    }
                    goto end;
                }

                nRet = wifiMgr_is_wifi_available(&bIsAvail);
                nRet = wifiMgr_get_connect_state(&nWiFiConnState, &nWiFiConnEcode);

				if ((ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_SLEEP_STATUS, NULL) && is_temp_disconnect)
					is_temp_disconnect = false;

                if (bIsAvail)
                {

                    if (is_temp_disconnect) {
                        is_temp_disconnect = false;     // reset
                        printf("[WIFIMGR]%s() L#%ld: WiFi auto re-connected!\r\n", __FUNCTION__, __LINE__);
                        if (gWifiMgrSetting.wifiCallback)
                            gWifiMgrSetting.wifiCallback(WIFIMGR_STATE_CALLBACK_CLIENT_MODE_RECONNECTION);
                    }

                    if (!is_wifi_available) {
                        // prev is not available, curr is available
                        is_wifi_available = true;
                        no_ini_file = false;
                        no_ssid = false;
                        printf("[WIFIMGR]%s() L#%ld: WiFi auto re-connected!\r\n", __FUNCTION__, __LINE__);
                    }
                } else {
                    if (is_wifi_available){
                        if (!is_temp_disconnect  && nWiFiConnEcode == WIFIMGR_ECODE_OK)
                        {
                            // first time detect
                            is_temp_disconnect = true;
                            gettimeofday(&tv3_temp, NULL);
                            printf("[WIFIMGR]%s() L#%ld: WiFi temporary disconnected!%d %d\r\n", __FUNCTION__, __LINE__,nWiFiConnState,nWiFiConnEcode);
                            if (gWifiMgrSetting.wifiCallback)
                                gWifiMgrSetting.wifiCallback(WIFIMGR_STATE_CALLBACK_CLIENT_MODE_TEMP_DISCONNECT);
                        } else if (nWiFiConnEcode == WIFIMGR_ECODE_OK){
                            gettimeofday(&tv4_temp, NULL);
                            temp_disconn_time = itpTimevalDiff(&tv3_temp, &tv4_temp);
                            printf("[WIFIMGR]%s() L#%ld: temp disconnect time = %ld sec. %d %d\r\n", __FUNCTION__, __LINE__, temp_disconn_time / 1000 , nWiFiConnState,nWiFiConnEcode);
                            if (temp_disconn_time >= WIFIMGR_TEMPDISCONN_MSEC) {
                                printf("[WIFIMGR]%s() L#%ld: WiFi temporary disconnected over %ld sec. Services should be shut down.\r\n", __FUNCTION__, __LINE__, temp_disconn_time / 1000);
                                is_temp_disconnect = false;     // reset

                                if (gWifiMgrSetting.wifiCallback)
                                    gWifiMgrSetting.wifiCallback(WIFIMGR_STATE_CALLBACK_CLIENT_MODE_DISCONNECT_30S);

                                // prev is available, curr is not available
                                is_wifi_available = false;
                            }
                        }
                    }
                    else
                    {
                        // prev is not available, curr is not available
                        if (no_ini_file || no_ssid) {
                            // has no data to connect, skip
                            goto end;
                        }

                        nRet = wifiMgr_get_connect_state(&nWiFiConnState, &nWiFiConnEcode);
                        switch (nWiFiConnState)
                        {
                        case WIFIMGR_CONNSTATE_STOP:
                            gettimeofday(&tv2, NULL);
                            if (itpTimevalDiff(&tv1, &tv2) >= WIFIMGR_RECONNTECT_MSEC) {
                                nRet = WifiMgr_Connecting();
                                if (nRet == WIFIMGR_ECODE_OK) {
                                    gWifi_connstate = WIFIMGR_CONNSTATE_CONNECTING;
                                }
                            }
                            break;
                        case WIFIMGR_CONNSTATE_SETTING:
                            break;
                        case WIFIMGR_CONNSTATE_CONNECTING:
                            break;
                        }
                    }
                }
            }

    end:
            nCheckCnt = WIFIMGR_CHECK_WIFI_MSEC;
        }
    }
    return NULL;
}

static void*
WifiMgrClientModeThreadFunc(void* arg)
{
    int nRet = WIFIMGR_ECODE_OK;

    while (1)
    {
        sem_wait(&semConnectStart);

        if (gwifiTerminate) {
            printf("[Wifi mgr]terminate WifiMgrClientModeThreadFunc(0) \n");
            break;
        }

        if (need_set){
            wifi_conn_state = WIFIMGR_CONNSTATE_SETTING;
            printf("[WIFIMGR] START to Set!\r\n");
            wifi_conn_ecode = nRet = WIFIMGR_ECODE_OK;

            need_set = false;
            printf("[WIFIMGR] Set finish!\r\n");
        }
        usleep(1000);

        if (nRet == WIFIMGR_ECODE_OK) {
            wifi_conn_state = WIFIMGR_CONNSTATE_CONNECTING;

            printf("[WIFIMGR] START to Connect!\r\n");
            iteWPACtrlWpsCancel();

			/* Wait Wifi turn on at UI */
			while (!wifi_client_on_off){
				memset(gSsid    , 0, sizeof(gSsid));
				memset(gPassword, 0, sizeof(gPassword));
				memset(gSecumode, 0, sizeof(gSecumode));

				usleep(100*1000);
                if(gwifiTerminate)
                {
                    printf("[Wifi mgr]terminate WifiMgrClientModeThreadFunc(1) \n");
                    wifi_conn_state = WIFIMGR_CONNSTATE_STOP;
                    goto end;
                }
			}

            wps_cancel_flag = true;
            wifi_conn_ecode = WifiMgr_Connect();
            wps_cancel_flag = false;
            printf("[WIFIMGR] Connect finish!\r\n");

        }
        wifi_conn_state = WIFIMGR_CONNSTATE_STOP;
        usleep(1000);
    }
end:
    wifi_conn_state = WIFIMGR_CONNSTATE_STOP;
    return NULL;
}

int
wifiMgr_is_wifi_available(int *is_available)
{
    int nRet = WIFIMGR_ECODE_OK;
    int is_connected = 0, is_avail = 0;

    if (!al_wifi_init) {
        *is_available = 0;
        return WIFIMGR_ECODE_NOT_INIT;
    }

    is_connected = ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_CONNECTED, NULL);
    is_avail = ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_AVAIL, NULL);
    *is_available = is_connected && is_avail && iteWPAConnectState();

    return nRet;
}

int
wifiMgr_set_wps_connect(void)
{
    int nRet = WIFIMGR_ECODE_OK;

    if (!al_wifi_init) {
        return WIFIMGR_ECODE_NOT_INIT;
    }

    pthread_mutex_lock(&mutexALWiFi);
    if (wifi_conn_state == WIFIMGR_CONNSTATE_STOP) {
        need_set = true;
        sem_post(&semWPSStart);
//        sem_post(&semConnectStart);
    }
    pthread_mutex_unlock(&mutexALWiFi);

    return nRet;
}

int
wifiMgr_get_connect_state(int *conn_state, int *e_code)
{
    int nRet = WIFIMGR_ECODE_OK;

    if (!al_wifi_init) {
        *conn_state = 0;
        *e_code = 0;
        return WIFIMGR_ECODE_NOT_INIT;
    }

    pthread_mutex_lock(&mutexALWiFi);
    *conn_state = wifi_conn_state;
    *e_code = wifi_conn_ecode;
    pthread_mutex_unlock(&mutexALWiFi);

    return nRet;
}

int wifiMgr_get_APMode_Ready()
{
    return soft_ap_init_ready;
}

int
wifiMgr_get_wifi_mode(void)
{
    return wifi_mode;
}

int wifiMgr_get_softap_device_number(void)
{
    int stacount = 0;
    if (wifi_mode ==WIFIMGR_MODE_SOFTAP) {
        stacount = ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_STANUM, NULL);
        printf("sta number = %d\n",stacount);
    }
	return stacount;
}

// get mac addresss
int wifiMgr_get_Mac_address(char cMac[6])
{
    ITPWifiInfo wifiInfo;

    ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_GET_INFO, &wifiInfo);
    cMac[0] = wifiInfo.hardwareAddress[0];
    cMac[1] = wifiInfo.hardwareAddress[1];
    cMac[2] = wifiInfo.hardwareAddress[2];
    cMac[3] = wifiInfo.hardwareAddress[3];
    cMac[4] = wifiInfo.hardwareAddress[4];
    cMac[5] = wifiInfo.hardwareAddress[5];

    return 0;
}

static int wifiMgr_clientMode_setting(char* ssid,char* password,char* secumode)
{

    if (ssid){
        // SSID
        snprintf(gSsid, WIFI_SSID_MAXLEN, ssid);
    }
    if (password){
        // Password
        snprintf(gPassword, WIFI_PASSWORD_MAXLEN, password);
    }
    if (secumode){
        // Security mode
        snprintf(gSecumode, WIFI_SECUMODE_MAXLEN, secumode);
    }

    return 0;
}

int wifiMgr_clientMode_sleep_disconnect(void)
{

    ITPEthernetSetting setting;

    int nRet = WIFIMGR_ECODE_OK;

    if (!al_wifi_init) {
        return WIFIMGR_ECODE_NOT_INIT;
    }

	printf("wifiMgr_clientMode_sleep_disconnect \n");
    wifi_conn_state = WIFIMGR_CONNSTATE_STOP;
    wifi_conn_ecode = WIFIMGR_ECODE_SET_DISCONNECT;
    usleep(1000*100);

    return WIFIMGR_ECODE_OK;
}

int wifiMgr_clientMode_disconnect(void)
{

    ITPEthernetSetting setting;

    int nRet = WIFIMGR_ECODE_OK;

    if (!al_wifi_init) {
        return WIFIMGR_ECODE_NOT_INIT;
    }

    if (wifi_conn_state == WIFIMGR_CONNSTATE_CONNECTING){
        wifiMgr_CancelConnect();
	}

	printf("wifiMgr_clientMode_disconnect \n");

    if (ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_CONNECTED, NULL)) {

        iteWPACtrlDisconnectNetwork();
        usleep(1000*100);
        // dhcp
        setting.dhcp = 0;

        // autoip
        setting.autoip = 0;

        // ipaddr
        setting.ipaddr[0] = 0;
        setting.ipaddr[1] = 0;
        setting.ipaddr[2] = 0;
        setting.ipaddr[3] = 0;

        // netmask
        setting.netmask[0] = 0;
        setting.netmask[1] = 0;
        setting.netmask[2] = 0;
        setting.netmask[3] = 0;

        // gateway
        setting.gw[0] = 0;
        setting.gw[1] = 0;
        setting.gw[2] = 0;
        setting.gw[3] = 0;

        //ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_RESET, &setting);
    }

    printf("wifiMgr_clientMode_disconnect end \n");
    wifi_conn_state = WIFIMGR_CONNSTATE_STOP;
    wifi_conn_ecode = WIFIMGR_ECODE_SET_DISCONNECT;
    usleep(1000*100);
    wifiMgr_Not_CancelConnect();
    return WIFIMGR_ECODE_OK;
}

int wifiMgr_clientMode_connect_ap(char* ssid,char* password,char* secumode)
{
    int nRet = WIFIMGR_ECODE_OK;

    if (!al_wifi_init) {
        return WIFIMGR_ECODE_NOT_INIT;
    }

    wifiMgr_clientMode_setting(ssid,password,secumode);

    if (wifi_conn_state == WIFIMGR_CONNSTATE_STOP) {
        need_set = false;
        sem_post(&semConnectStart);
    }

    return WIFIMGR_ECODE_OK;
}

static void Swap(int x, int y)
{
  //    int temp = array[x];
    //array[x] = array[y];
    //array[y] = temp;
    WIFI_MGR_SCANAP_LIST temp;
// printf("%d %d 0x%x 0x%x \n",gWifiMgrApList[x].rfQualityRSSI,gWifiMgrApList[y].rfQualityRSSI, &gWifiMgrApList[x],&gWifiMgrApList[y]);

    memcpy(&temp,&gWifiMgrApList[x],sizeof(WIFI_MGR_SCANAP_LIST));
    memcpy(&gWifiMgrApList[x],&gWifiMgrApList[y],sizeof(WIFI_MGR_SCANAP_LIST));
    memcpy(&gWifiMgrApList[y],&temp,sizeof(WIFI_MGR_SCANAP_LIST));
}


static void InsertSortWifi(int size)
{
    int i,j;
    for(i = 0; i < size; i++){
        for(j = i; j > 0; j--){
            if(gWifiMgrApList[j].rfQualityRSSI > gWifiMgrApList[j - 1].rfQualityRSSI){
                Swap(j, j-1);
            }
        }
    }
}


static int RemoveSameSsid(int size)
{
    int i,j;
    if (size < 1){
        return size;
    }

    for (i=size-1 ; i>0 ; i--){
        for (j = i ; j >=0 ; j --){
            if (strcmp(gWifiMgrApList[i].ssidName , gWifiMgrApList[j].ssidName)==0 && gWifiMgrApList[i].rfQualityRSSI < gWifiMgrApList[j].rfQualityRSSI){
                //set rssi =0 , if the same ssid
                gWifiMgrApList[i].rfQualityRSSI = 0;
            }
        }
    }

    for (i = 0 , j =0 ; i < size ; i ++){
        if (gWifiMgrApList[i].rfQualityRSSI > 0){
            memcpy(&gWifiMgrTempApList[j],&gWifiMgrApList[i],sizeof(WIFI_MGR_SCANAP_LIST));
            j++;
        }

    }

#if 0
    printf("RemoveSameSsid %d \n",j);
    for (i = 0; i < j; i++)
    {
        printf("[Wifi mgr] ssid = %32s, securityOn = %ld, securityMode = %ld, avgQuant = %d, avgRSSI = %d , <%02x:%02x:%02x:%02x:%02x:%02x>\r\n", gWifiMgrTempApList[i].ssidName, gWifiMgrTempApList[i].securityOn, gWifiMgrTempApList[i].securityMode,gWifiMgrTempApList[i].rfQualityQuant, gWifiMgrTempApList[i].rfQualityRSSI,
        gWifiMgrTempApList[i].apMacAddr[0], gWifiMgrTempApList[i].apMacAddr[1], gWifiMgrTempApList[i].apMacAddr[2], gWifiMgrTempApList[i].apMacAddr[3], gWifiMgrTempApList[i].apMacAddr[4], gWifiMgrTempApList[i].apMacAddr[5]);
    }
    printf("RemoveSameSsid -----\n");
#endif
    memcpy(&gWifiMgrApList[0],&gWifiMgrTempApList[0],sizeof(WIFI_MGR_SCANAP_LIST)*WIFI_SCAN_LIST_NUMBER);
    return j;

}

static int
scanWifiAp(struct net_device_info *apInfo)
{
    int nRet = 0;
    int nWifiState = 0;
    int i = 0;
    int nHideSsid = 0;

    memset(apInfo, 0, sizeof(struct net_device_info));

    printf("[Wifi mgr]%s() Start to SCAN AP ==========================\r\n", __FUNCTION__);
    ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_SCAN, NULL);
    while (1)
    {
        nWifiState = ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_STATE, NULL);
        //printf("[Presentation]%s() nWifiState=0x%X\r\n", __FUNCTION__, nWifiState);
        if ((nWifiState & WLAN_SITE_MONITOR) == 0)
        {
            // scan finish
            printf("[Wifi mgr]%s() Scan AP Finish!\r\n", __FUNCTION__);
            break;
        }
        usleep(100 * 1000);
    }

    read(ITP_DEVICE_WIFI, apInfo, (size_t)NULL);
    printf("[Wifi mgr]%s() ScanApInfo.apCnt = %ld\r\n", __FUNCTION__, apInfo->apCnt);

	#if 0
    for (i = 0; i < apInfo->apCnt; i++)
    {
        printf("[Main] ssid = %32s (%d), securityOn = %ld, securityMode = %ld, avgQuant = %d, avgRSSI = %d , <%02x:%02x:%02x:%02x:%02x:%02x>\r\n", apInfo->apList[i].ssidName, strlen(apInfo->apList[i].ssidName),apInfo->apList[i].securityOn, apInfo->apList[i].securityMode,apInfo->apList[i].rfQualityQuant, apInfo->apList[i].rfQualityRSSI,
        apInfo->apList[i].apMacAddr[0], apInfo->apList[i].apMacAddr[1], apInfo->apList[i].apMacAddr[2], apInfo->apList[i].apMacAddr[3], apInfo->apList[i].apMacAddr[4], apInfo->apList[i].apMacAddr[5]);
    }
	#endif

    for (i = 0; i < apInfo->apCnt; i++)
    {
        if (strlen(apInfo->apList[i].ssidName)>0)
        {
        gWifiMgrApList[i].channelId = apInfo->apList[i].channelId;
        gWifiMgrApList[i].operationMode = apInfo->apList[i].operationMode ;
        gWifiMgrApList[i].rfQualityQuant = apInfo->apList[i].rfQualityQuant;
        gWifiMgrApList[i].rfQualityRSSI = apInfo->apList[i].rfQualityRSSI;
        gWifiMgrApList[i].securityMode = apInfo->apList[i].securityMode;
        memcpy(gWifiMgrApList[i].apMacAddr,apInfo->apList[i].apMacAddr,8);
        memcpy(gWifiMgrApList[i].ssidName,apInfo->apList[i].ssidName,32);
        } else {
            nHideSsid ++;
    }
    }
    apInfo->apCnt = apInfo->apCnt -nHideSsid;

    InsertSortWifi(apInfo->apCnt);

    apInfo->apCnt = RemoveSameSsid(apInfo->apCnt);

#if 0
    for (i = 0; i < apInfo->apCnt; i++)
    {
        printf("[Wifi mgr] ssid = %32s, securityOn = %ld, securityMode = %ld, avgQuant = %d, avgRSSI = %d , <%02x:%02x:%02x:%02x:%02x:%02x>\r\n", gWifiMgrApList[i].ssidName, gWifiMgrApList[i].securityOn, gWifiMgrApList[i].securityMode,gWifiMgrApList[i].rfQualityQuant, gWifiMgrApList[i].rfQualityRSSI,
        gWifiMgrApList[i].apMacAddr[0], gWifiMgrApList[i].apMacAddr[1], gWifiMgrApList[i].apMacAddr[2], gWifiMgrApList[i].apMacAddr[3], gWifiMgrApList[i].apMacAddr[4], gWifiMgrApList[i].apMacAddr[5]);
    }
#endif

    printf("[Wifi mgr]%s() End to SCAN AP ============================\r\n", __FUNCTION__);
    return apInfo->apCnt;
}

int wifiMgr_get_scan_ap_info(WIFI_MGR_SCANAP_LIST* pList)
{
    int nApCount;

    if (!al_wifi_init) {
        printf("wifiMgr_get_scan_ap_info  !al_wifi_init \n ");
        return WIFIMGR_ECODE_NOT_INIT;
    }


    pthread_mutex_lock(&mutexMode);

	if (wifi_client_on_off){
            nApCount = scanWifiAp(&gScanApInfo);
            memcpy(pList,gWifiMgrApList,sizeof(WIFI_MGR_SCANAP_LIST)*WIFI_SCAN_LIST_NUMBER);
	}

    pthread_mutex_unlock(&mutexMode);

    printf("wifiMgr_get_scan_ap_info %d  \n",nApCount);

    return nApCount;

}


int
wifiMgr_init(WIFIMGR_MODE_E init_mode, int mp_mode,WIFI_MGR_SETTING wifiSetting)
{
    int nRet = WIFIMGR_ECODE_OK;
    pthread_attr_t attr, attr1,attr2;

    printf("[WifiMgr] wifiMgr_init %d , %s %s %s \n",init_mode,wifiSetting.ssid,wifiSetting.password,wifiSetting.secumode);
    while(gwifiTerminate){
         printf("WifiMgr not finished yet \n");
         usleep(200*1000);
    }

    wifi_conn_state = WIFIMGR_CONNSTATE_STOP;
    wifi_conn_ecode = WIFIMGR_ECODE_SET_DISCONNECT;
    need_set = false;
    mpMode = mp_mode;

    gWifiMgrSetting.wifiCallback = wifiSetting.wifiCallback;

    if (wifi_mode ==WIFIMGR_MODE_CLIENT){
        wifiMgr_clientMode_setting(wifiSetting.ssid,wifiSetting.password,wifiSetting.secumode);
    }

    // default select dhcp
    gWifiMgrSetting.setting.dhcp = 1;
    if (wifiSetting.setting.ipaddr[0]>0){
        memcpy(&gWifiMgrSetting.setting,&wifiSetting.setting,sizeof(ITPEthernetSetting));
    }

#ifndef CFG_NET_WIFI_SDIO
    // Check if the wifi driver is exist
    if (ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_WIFI_GET_NET_DEVICE, NULL) == 0) {
        // has no wifi driver!
        printf("[WIFIMGR]%s() L#%ld: Error! Has no WiFi driver!!\r\n", __FUNCTION__, __LINE__);
        nRet = WIFIMGR_ECODE_NO_WIFI_DONGLE;
        goto end;
    }

    // Check if the wifi dongle is exist
    if (ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_IS_DEVICE_READY, NULL) == 0) {
        // has no wifi device!
        printf("[WIFIMGR]%s() L#%ld: Error! Has no WiFi device!!\r\n", __FUNCTION__, __LINE__);
        nRet = WIFIMGR_ECODE_NO_WIFI_DONGLE;
        goto end;
    }
#endif

    // init semaphore
    nRet = sem_init(&semConnectStart, 0, 0);
    if (nRet == -1) {
        printf("[WIFIMGR] ERROR, semConnectStart sem_init() fail!\r\n");
        nRet = WIFIMGR_ECODE_SEM_INIT;
        goto err_end;
    }

    nRet = sem_init(&semConnectStop, 0, 0);
    if (nRet == -1) {
        printf("[WIFIMGR] ERROR, semConnectStop sem_init() fail!\r\n");
        nRet = WIFIMGR_ECODE_SEM_INIT;
        goto err_end;
    }

    nRet = sem_init(&semWPSStart, 0, 0);
    if (nRet == -1) {
        printf("[WIFIMGR] ERROR, semWPSStart sem_init() fail!\r\n");
        nRet = WIFIMGR_ECODE_SEM_INIT;
        goto err_end;
    }

    nRet = sem_init(&semWPSStop, 0, 0);
    if (nRet == -1) {
        printf("[WIFIMGR] ERROR, semWPSStop sem_init() fail!\r\n");
        nRet = WIFIMGR_ECODE_SEM_INIT;
        goto err_end;
    }

    // init mutex
    nRet = pthread_mutex_init(&mutexALWiFi, NULL);
    if (nRet != 0) {
        printf("[WIFIMGR] ERROR, mutexALWiFi pthread_mutex_init() fail! nRet = %ld\r\n", nRet);
        nRet = WIFIMGR_ECODE_MUTEX_INIT;
        goto err_end;
    }

    nRet = pthread_mutex_init(&mutexALWiFiWPS, NULL);
    if (nRet != 0) {
        printf("[WIFIMGR] ERROR, mutexALWiFiWPS pthread_mutex_init() fail! nRet = %ld\r\n", nRet);
        nRet = WIFIMGR_ECODE_MUTEX_INIT;
        goto err_end;
    }

    nRet = pthread_mutex_init(&mutexIni, NULL);
    if (nRet != 0) {
        printf("[WIFIMGR] ERROR, mutexIni pthread_mutex_init() fail! nRet = %ld\r\n", nRet);
        nRet = WIFIMGR_ECODE_MUTEX_INIT;
        goto err_end;
    }

    nRet = pthread_mutex_init(&mutexMode, NULL);
    if (nRet != 0) {
        printf("[WIFIMGR] ERROR, mutexMode pthread_mutex_init() fail! nRet = %ld\r\n", nRet);
        nRet = WIFIMGR_ECODE_MUTEX_INIT;
        goto err_end;
    }

    // create thread
    printf("[WifiMgr] create thread \n");

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, WIFI_STACK_SIZE);
    pthread_create(&clientModeTask, &attr, WifiMgrClientModeThreadFunc, NULL);

    pthread_attr_init(&attr1);
    pthread_attr_setstacksize(&attr1, WIFI_STACK_SIZE);
    pthread_create(&wpsTask, &attr1, WifiMgrWPSThreadFunc, NULL);

    pthread_attr_init(&attr2);
    pthread_attr_setstacksize(&attr2, WIFI_STACK_SIZE);
    pthread_create(&processTask, &attr2, WiFiMgrProcessThreadFunc, NULL);


    wifi_mode = init_mode;
    printf("[WIFIMGR]%s() L#%ld: init_mode = %ld, wifi_mode = %ld\r\n", __FUNCTION__, __LINE__, init_mode, wifi_mode);

    if (wifi_mode == WIFIMGR_MODE_SOFTAP){
#ifdef CFG_NET_WIFI_HOSTAPD
		WifiMgr_HostAPInit();
#endif
	}else{
		WifiMgr_WpaInit();
	}

    wifiMgr_Not_CancelConnect();
    wps_cancel_flag = false;

    al_wifi_init = 1;
    gwifiTerminate = 0;
end:
    return nRet;

err_end:
    pthread_mutex_destroy(&mutexMode);
    pthread_mutex_destroy(&mutexIni);
    pthread_mutex_destroy(&mutexALWiFiWPS);
    sem_destroy(&semWPSStop);
    sem_destroy(&semWPSStart);
    pthread_mutex_destroy(&mutexALWiFi);
    sem_destroy(&semConnectStop);
    sem_destroy(&semConnectStart);

    return nRet;
}



int
wifiMgr_terminate(void)
{
    int nRet = WIFIMGR_ECODE_OK;

    printf("wifiMgr_terminate \n");
    if (!al_wifi_init) {
        return WIFIMGR_ECODE_NOT_INIT;
    }

    gwifiWpaTerminate = 1;
    if (wifi_mode == WIFIMGR_MODE_SOFTAP) {
#ifdef CFG_NET_WIFI_HOSTAPD
        WifiMgr_HostAPTerminate();
#endif
    } else {
        wps_cancel_flag = true;
        printf("wifiMgr_terminate  WifiMgr_WpaTerminate \n");

        WifiMgr_WpaTerminate();
    }

    gwifiTerminate = 1;

    sem_post(&semWPSStart);
    sem_post(&semConnectStart);

    pthread_join(clientModeTask, NULL);
    pthread_join(wpsTask, NULL);
    pthread_join(processTask, NULL);

    pthread_mutex_destroy(&mutexMode);
    pthread_mutex_destroy(&mutexIni);
    pthread_mutex_destroy(&mutexALWiFiWPS);
    sem_destroy(&semWPSStop);
    sem_destroy(&semWPSStart);
    pthread_mutex_destroy(&mutexALWiFi);
    sem_destroy(&semConnectStop);
    sem_destroy(&semConnectStart);

    al_wifi_init = 0;
printf("wifiMgr_terminate ~~~~~\n");
    gwifiTerminate = 0;
    gwifiWpaTerminate = 0;
    wifiMgr_Not_CancelConnect();

    return nRet;
}

void
WifiMgr_clientMode_switch(int status)
{
	wifi_client_on_off = status;
}

static int gCountT = 0;
static WIFI_MGR_SETTING gWifiSetting;
static  struct timeval gSwitchStart, gSwitchEnd;

static  struct timeval gSwitchCount;
int calculateTime()
{
    if (itpTimevalDiff(&gSwitchCount,&gSwitchEnd)> 1000){
        printf(" %d , %d  \n",itpTimevalDiff(&gSwitchCount,&gSwitchEnd)+(gCountT*1000),itpTimevalDiff(&gSwitchStart,&gSwitchEnd));
        gCountT++;
        gettimeofday(&gSwitchCount, NULL);
    }
}

static void*
switchThread(void *arg)
{
    int nTemp;

    printf("WifiMgr_Switch_ClientSoftAP_Mode \n");

    if (!al_wifi_init) {
        //return WIFIMGR_ECODE_NOT_INIT;
        return NULL;
    }

    wifi_mode = wifiMgr_get_wifi_mode();

    wifiMgr_terminate();

    gCountT = 0;
   gettimeofday(&gSwitchCount, NULL);

    if (wifi_mode == WIFIMGR_MODE_SOFTAP){
        // init client mode
        printf("WifiMgr_Switch_ClientSoftAP_Mode init client  \n");
        nTemp = wifiMgr_init(WIFIMGR_MODE_CLIENT, 0, gWifiSetting);
    } else {
        // init softap mode
        printf("WifiMgr_Switch_ClientSoftAP_Mode init softap  \n");
        do
        {
            usleep(1000*400);
            gettimeofday(&gSwitchEnd, NULL);

            calculateTime();
        }while(!iteWPADeinitDone() && (itpTimevalDiff(&gSwitchStart,&gSwitchEnd)<15000));
        printf("WifiMgr_Switch_ClientSoftAP_Mode deinit done  \n");

        nTemp = wifiMgr_init(WIFIMGR_MODE_SOFTAP, 0, gWifiSetting);
    }

    if (gWifiMgrSetting.wifiCallback)
        gWifiMgrSetting.wifiCallback(WIFIMGR_STATE_CALLBACK_SWITCH_CLIENT_SOFTAP_FINISH);

    return NULL;

}


int WifiMgr_Switch_ClientSoftAP_Mode(WIFI_MGR_SETTING wifiSetting)
{

    pthread_t task;
    pthread_attr_t attr;
#ifdef CFG_NET_WIFI_HOSTAPD

#else

    return 0;
#endif
    gettimeofday(&gSwitchStart, NULL);

    memcpy(&gWifiSetting,&wifiSetting,sizeof(WIFI_MGR_SETTING));

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, WIFI_STACK_SIZE);
    pthread_create(&task, &attr, switchThread, NULL);

}

void wifiMgr_CancelConnect(void)
{
    connect_cancel_flag = true;
}

void wifiMgr_Not_CancelConnect(void)
{
    connect_cancel_flag = false;
}

int WifiMgr_Is_Wpa_Wifi_Terminating(void)
{

    return (gwifiTerminate ||gwifiWpaTerminate);
}

#elif defined(_WIN32)

int wifiMgr_get_scan_ap_info(WIFI_MGR_SCANAP_LIST* pList)
{
    int nApCount = 0;

    return nApCount;
}

int
wifiMgr_get_connect_state(int *conn_state, int *e_code)
{
    int nRet = WIFIMGR_ECODE_OK;

    *conn_state = 0;
    *e_code = 0;


    return nRet;
}

int wifiMgr_clientMode_connect_ap(char* ssid,char* password,char* secumode)
{
    int nRet = WIFIMGR_ECODE_OK;

    return WIFIMGR_ECODE_OK;
}

int wifiMgr_clientMode_disconnect()
{
    return WIFIMGR_ECODE_OK;
}

void wifiMgr_CancelConnect()
{
    return;
}
#endif

