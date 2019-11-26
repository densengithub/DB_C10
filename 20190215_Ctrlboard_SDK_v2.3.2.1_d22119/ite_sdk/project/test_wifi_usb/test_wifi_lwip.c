#include <pthread.h>
#include <errno.h>
#include "openrtos/FreeRTOS.h"
#include "openrtos/task.h"
#include "lwip/sockets.h"
#include "ite/ith.h"
#include "ite/itp.h"
#include "lwip/ip.h"
#include "iniparser/iniparser.h"
#include "ctrlboard.h"
#include "wifiMgr.h"

char* SENDER_IP_ADDR;
bool wifi_connect_all_finish = false;

/* Only test by Wifi USB type project */
#define network_ap_layer 1
#if network_ap_layer
#define test_perf		 0
#define tcp_client_test	 0
#define test_scan_ap	 0
#define test_connect	 1

////////////////////test function////////////////////
#if test_scan_ap
static void scan_ap_loop(void)
{
	int AP_count, loop_time = 0;
	WIFI_MGR_SCANAP_LIST pList[64];

	++loop_time;
	if (loop_time > 10){
		printf("========>start scan\n");
		AP_count = wifiMgr_get_scan_ap_info(pList);
		printf("========>found %d AP\n\n", AP_count);
		loop_time = 0;
	}

	sleep(5);
};
#endif


#if test_connect
struct connect_info
{
	char ssid[WIFI_SSID_MAXLEN];
	char password[WIFI_PASSWORD_MAXLEN];
	char secumode[WIFI_SECUMODE_MAXLEN];
};

static struct connect_info _connect_info[2];
static void link_info_setting(void)
{
	snprintf(_connect_info[0].ssid, WIFI_SSID_MAXLEN, "SSID");
	snprintf(_connect_info[0].password, WIFI_PASSWORD_MAXLEN, "Password");
	snprintf(_connect_info[0].secumode, WIFI_SECUMODE_MAXLEN, "6");

	snprintf(_connect_info[1].ssid, WIFI_SSID_MAXLEN, "SSID");
	snprintf(_connect_info[1].password, WIFI_PASSWORD_MAXLEN, "Password");
	snprintf(_connect_info[1].secumode, WIFI_SECUMODE_MAXLEN, "6");
};

static void link_differnet_ap(void)
{
	int loop_time = 0;

	if(wifi_connect_all_finish)
		wifiMgr_clientMode_disconnect();

	usleep(500*1000);

	++loop_time;
	printf("\n========>connect to [%s]\n", _connect_info[loop_time%2].ssid);
	wifiMgr_clientMode_connect_ap(_connect_info[loop_time%2].ssid, _connect_info[loop_time%2].password, _connect_info[loop_time%2].secumode);

	sleep(10);
};
#endif
////////////////////////////////////////
#endif

#if network_ap_layer
static int wifiCallbackFucntion(int nState)
{
    ITPWifiInfo wifiInfo;
    char* wifi_ip;

    switch (nState)
    {
        case WIFIMGR_STATE_CALLBACK_CONNECTION_FINISH:
            printf("[Indoor]WifiCallback connection finish \n");
			wifi_connect_all_finish = true;
#if tcp_client_test
            ioctl(ITP_DEVICE_WIFI, ITP_IOCTL_GET_INFO, &wifiInfo);
            wifi_ip = ipaddr_ntoa((const ip_addr_t*)&wifiInfo.address);
			SENDER_IP_ADDR = wifi_ip;
            printf("====>Wifi IP: %s, Socket sender IP: %s\n", wifi_ip, SENDER_IP_ADDR);
#endif
        break;

        case WIFIMGR_STATE_CALLBACK_CLIENT_MODE_DISCONNECT_30S:
            printf("[Indoor]WifiCallback connection disconnect 30s \n");
        break;

        case WIFIMGR_STATE_CALLBACK_CLIENT_MODE_RECONNECTION:
            printf("[Indoor]WifiCallback connection reconnection \n");
        break;

        case WIFIMGR_STATE_CALLBACK_CLIENT_MODE_TEMP_DISCONNECT:
            printf("[Indoor]WifiCallback connection temp disconnect \n");
        break;

        case WIFIMGR_STATE_CALLBACK_CLIENT_MODE_CONNECTING_FAIL:
            printf("[Indoor]WifiCallback connecting fail, please check ssid,password,secmode \n");
        break;

        default:
            printf("[Indoor]WifiCallback unknown %d state  \n",nState);
        break;

    }
}

static WIFI_MGR_SETTING gWifiSetting;
static void ResetWifi()
{
    char buf[16], *saveptr;

    memset(&gWifiSetting.setting, 0, sizeof (ITPEthernetSetting));
	printf("====>Reset Wifi\n");

    // dhcp
    gWifiSetting.setting.dhcp = 1;

    // autoip
    gWifiSetting.setting.autoip = 0;

    // ipaddr
    gWifiSetting.setting.ipaddr[0] = 192;
    gWifiSetting.setting.ipaddr[1] = 168;
    gWifiSetting.setting.ipaddr[2] = 190;
    gWifiSetting.setting.ipaddr[3] = 101;

    // netmask
    gWifiSetting.setting.netmask[0] = 255;
    gWifiSetting.setting.netmask[1] = 255;
    gWifiSetting.setting.netmask[2] = 255;
    gWifiSetting.setting.netmask[3] = 0;

    // gateway
    gWifiSetting.setting.gw[0] = 192;
    gWifiSetting.setting.gw[1] = 168;
    gWifiSetting.setting.gw[2] = 190;
    gWifiSetting.setting.gw[3] = 1;

}

void NetworkInit(void)
{
	snprintf(gWifiSetting.ssid , WIFI_SSID_MAXLEN, "SSID");
	snprintf(gWifiSetting.password, WIFI_PASSWORD_MAXLEN, "Password");
	snprintf(gWifiSetting.secumode, WIFI_SECUMODE_MAXLEN, "6");
#if test_connect
	link_info_setting();
#endif
	gWifiSetting.wifiCallback = wifiCallbackFucntion;
	ResetWifi();
}
#endif

void* TestAPFunc(void* arg)
{
	int wifi_on_off = 1, AP_count, loop_time = 0;

#if network_ap_layer
	while (loop_time < 1)
    {
    	++loop_time;
		printf("====>wait wlan0...\n");
		sleep(1);
	}

    printf("====>init network\n");
    NetworkInit();

    printf("====>init wifi\n");
    WifiMgr_clientMode_switch(wifi_on_off);
    wifiMgr_init(WIFIMGR_MODE_CLIENT, 0, gWifiSetting);

	loop_time = 0;
#endif

	while (!wifi_connect_all_finish){
		sleep(1);
	}

    for (;;)
    {
#if test_scan_ap
		scan_ap_loop();
#endif

#if test_connect
		link_differnet_ap();
#endif

		if(!(test_scan_ap || test_connect))
			sleep(1);
    }

    return NULL;
}

void* TestFunc(void* arg)
{
    pthread_t task_ap;
    pthread_attr_t attr_ap;

    // init pal
    printf("====>init itp\n");
    itpInit();
    usleep(5*100*1000);

	pthread_attr_init(&attr_ap);
	pthread_create(&task_ap, &attr_ap, TestAPFunc, NULL);


	while (!wifi_connect_all_finish){
		sleep(1);
	}

#if (CFG_NET_HTTP && test_perf && !test_connect)
	sleep(10);

	iperf_test_client();
#endif

	for (;;)
    {
#if (tcp_client_test && !test_connect)
		network_tcp_client_main();
		printf("====>socket connect/recv failed\n");
#endif

        sleep(1);
    }
}


