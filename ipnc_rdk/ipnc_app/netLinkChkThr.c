#include <stdio.h>
#include <osa_thr.h>
#include <sys/prctl.h>


#include <includes.h>
#include <sysCfg.h>
#include <common.h>
#include <wifi.h>
#include <netLinkChkThr.h>


#define SERVER_HOSTNAME "115.231.64.178"

static pthread_t ThrId;
static void *netLinkChkThr(void* arg)
{
	int wifi_DB;
	Bool  serverConnetState = False;
	prctl(PR_SET_NAME, (unsigned long)__func__, 0, 0, 0);
	while(1)
	{
		wifi_DB = getWifiRSSI();
		if(wifi_DB >= 0)
		{
			wifi_DB = WIRELESS_RSSI_DISCONMECT;
		}
		else if(wifi_DB >= -58)
		{
			wifi_DB = WIRELESS_RSSI_LEVEL_1;
		}
		else if((wifi_DB<-58) && (wifi_DB>=-80))
		{
			wifi_DB = WIRELESS_RSSI_LEVEL_2;
		}
		else if(wifi_DB < -80)
		{
			wifi_DB = WIRELESS_RSSI_LEVEL_3;
		}
		setSysState(sys_net_wireless_wifi,wifi_DB);
		if(wifi_DB > WIRELESS_RSSI_DISCONMECT)
		{
		#if 1
			serverConnetState = isLive("apcli0",SERVER_HOSTNAME)==False?isLive("apcli0",SERVER_HOSTNAME):True;//需要多次判断
		//#else
			//serverConnetState = isLive(SERVER_HOSTNAME);
			if(serverConnetState == False)
			{
				sleep(1);
				serverConnetState = isLive("apcli0",SERVER_HOSTNAME);
			}
		#endif
		}
		setSysState(sys_net_server_link,serverConnetState);
		sleep(1);
	}
	
}
void netLinkThrCreat()
{
	 pthread_create(&ThrId, NULL, netLinkChkThr, NULL);
}
void netLinkThrDelet()
{

	pthread_cancel(ThrId);
	pthread_join(ThrId, NULL);
	LOG_DBG("\r\n")
}


