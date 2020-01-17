
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "pthread.h"
#include <sys/socket.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <common.h>
#include <sysCfg.h>
#include <includes.h>
#include "airkissThr.h"
#include <display.h>


#if WIRELESS_EXT <= 11
#ifndef SIOCDEVPRIVATE
#define SIOCDEVPRIVATE      0x8BE0
#endif
#define SIOCIWFIRSTPRIV      SIOCDEVPRIVATE
#endif
#define RTPRIV_IOCTL_GET_80211_DATA	(SIOCIWFIRSTPRIV + 0x1D)
#define RTPRIV_IOCTL_SHOW_CONNSTATUS	(SIOCIWFIRSTPRIV + 0x1B)

#define RX_80211_PKT_SIZE   (1500)
#define RX_BUF_SIZE         (3000)


typedef struct _RX_PKT
{
	unsigned int  length;
	unsigned char  data[RX_80211_PKT_SIZE];
}RX_PKT;


airkiss_context_t akcontext;
uint8_t cur_channel = 1;
airkiss_config_t config;
airkiss_result_t ak_result;

int set_channel(int channel)
{
        char cmd[128] = "\0";
	snprintf(cmd,128,"iwpriv apcli0 elian set_ch=%d>/dev/null",channel);
//	snprintf(cmd,128,"iwpriv apcli0 set Channel=%d",channel);
	system(cmd);
	return 0;
}

void time_callback(int signo)
{
        if(cur_channel >= 13)
	    cur_channel = 1;
	else
	    cur_channel ++;
	set_channel(cur_channel);
	airkiss_change_channel(&akcontext);
}

void exit_airkiss(void)
{
	int ret;
	int    socket_id;
	struct   iwreq wrq;
	char data[64];
	system("iwpriv apcli0 elian monitor_off");
	system("iwpriv apcli0 elian stop");
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id < 0)
	{
		printf("error::Open socket error!\n\n");
		return ;
	}
	memset(data, 0x00, 64);
	strcpy(data,"mangop");
	strcpy(wrq.ifr_name, "apcli0");
	wrq.u.data.length = 64;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_GET_80211_DATA, &wrq);
	if(ret != 0)
	{
		printf("error::stop error\n\n");
		return ;
	}
	close(socket_id);
	return ;
}


int udpsend(unsigned char random)
{
}
static int isExit = False;
void *airkissThr(void *arg)
{
	int length;
	RX_PKT * mp;
	int ret= -1;
	int i;
        int socket_id;
        struct iwreq wrq;
	char data[RX_BUF_SIZE];
	struct itimerval tick;
	Bool isGetSSID = False;

	//signal_handle();

	config.memcpy = memcpy;
	config.memset = memset;
	config.memcmp = memcmp;
	config.printf = printf;
	printf("version=%s\n",airkiss_version());
	usleep(100000);
	if(airkiss_init(&akcontext, &config))
	{
		printf("airkiss init fail\n");
		return OSA_EFAIL;
	}

	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id < 0)
	{
	    printf("error::Open socket error!\n");
		return -errno;
	}
	printf("start monitor mode\n");
	system("iwpriv apcli0 elian monitor_on");


	memset(data, 0x00, RX_BUF_SIZE);
	strcpy(data,"mangos");
	strcpy(wrq.ifr_name, "apcli0");
	wrq.u.data.length = RX_BUF_SIZE;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_GET_80211_DATA, &wrq);
	if(ret != 0)
	{
		printf("error::start monitor mode\n\n");
		system("iwpriv apcli0 elian monitor_off");
		system("iwpriv apcli0 elian stop");
		return -errno;
	}

	signal(SIGALRM, time_callback);//

	memset(&tick, 0, sizeof(tick));

	tick.it_interval.tv_sec = 0;
	tick.it_interval.tv_usec = 100000;//100000;
	tick.it_value.tv_sec = 0;
	tick.it_value.tv_usec = 100000;//100000;
	if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
	    printf("Set timer failed!\n");
	isExit  = 0;
    while(!isExit)
    {
		memset(data, 0x00, RX_BUF_SIZE);
		strcpy(data,"mangor");
		strcpy(wrq.ifr_name, "apcli0");
		wrq.u.data.length = RX_BUF_SIZE;
		wrq.u.data.pointer = data;
		wrq.u.data.flags = 0;
		ret = ioctl(socket_id, RTPRIV_IOCTL_GET_80211_DATA, &wrq);
		if(ret != 0)
		{
		                usleep(1000);
		                continue;
		}
		else
		{
		    mp = (RX_PKT *)wrq.u.data.pointer;
#if 0
		    for(i=0;i<((mp->length > 24)? 24:mp->length);i++)
		    {
			printf("%02X ",mp->data[i]);
		    }
		    printf("\n");
#endif
		        ret = airkiss_recv(&akcontext,mp->data,mp->length);
				if(ret == AIRKISS_STATUS_CHANNEL_LOCKED)
				{
				tick.it_value.tv_usec = 0;
				tick.it_interval.tv_usec = 0;
				if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
				    printf("Set timer failed!\n");
					printf("channel locked\n");
				}
				else if(ret == AIRKISS_STATUS_COMPLETE)
				{
				if(airkiss_get_result(&akcontext , &ak_result) < 0)
				    {
				        printf("airkiss get result fail\n");
					break;
				    }
				    else
				    {
				        LOG_INFO("result ok!ssid is %s , key is %s %p\n" , ak_result.ssid , ak_result.pwd, ak_result.random);
					isGetSSID = True;
					break;
				    }
				}

		}
    }
	system("iwpriv apcli0 elian monitor_off");
	system("iwpriv apcli0 elian stop");
	memset(data, 0x00, RX_BUF_SIZE);
	strcpy(data,"mangop");
	strcpy(wrq.ifr_name, "apcli0");
	wrq.u.data.length = RX_BUF_SIZE;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_GET_80211_DATA, &wrq);
	if(ret != 0)
	{
		printf("error::stop error\n\n");
		return -errno;
	}
	close(socket_id);
	char text[32];
	if(isGetSSID == True)
	{
		char cmd[128] = {0};
		LOG_INFO("connect AP...\r\n");
		snprintf(cmd,128,"wifimode apsta %s %s &",ak_result.ssid, ak_result.pwd);
		system(cmd);
		sleep(2);
		uint32_t timeout = 30;
		do {
			int len = sprintf(text, "Connect AP %02ds...", timeout);
			GUIDrawText((240-len*12)/2,220,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_GREEN);
			sleep(1);
			if(check_assoc("apcli0"))
			{
				LOG_INFO("connect AP OK!!!\r\n");				
				break;
			}
			if(-- timeout == 0)
			{
				LOG_WAR("connect AP fail!!!\r\n");
				break;
			}
		} while(1);
		/*³¬Ê±¼ì²âÍøÂçÁ´½Óok*/
		LOG_INFO("connect server...\r\n");
		timeout = 30;
		do {
			int len = sprintf(text, "   Get ip %02ds...  ", timeout);
			LOG_INFO("%s\r\n",text)
			GUIDrawText((240-len*12)/2,220,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_GREEN);
			sleep(1);
			ret = net_get_ifaddr("apcli0");
			if(ret > 0)
			{
				len = sprintf(text, "   Connect AP OK.  ", timeout);
				GUIDrawText((240-len*12)/2,220,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_GREEN);
				sleep(1);
				start_bp(200, 0); //stop flash
				airkiss_replayRandom(ak_result.random);
				LOG_INFO("connect server OK!!!\r\n");
				start_led(1, 1,1); //stop flash
				
				break;
			}
			if(-- timeout == 0)
			{
				LOG_WAR("connect server timeout!!!\r\n");
				start_led(200, 1,1); //stop flash
				break;
			}
		} while(1);
	}
	else
	{
		start_led(1, 1,1); //stop flash
	}
	setSysState(Sys_State_Run, MDVR_Sys_IDLE);
	return 0;
}
static pthread_t thrId;
void airKissThrCreat()
{ 
	pthread_create(&thrId, NULL, airkissThr, NULL);
}
void airKissThrDelete()
{
	isExit = True; 
	pthread_join(thrId, NULL);

}

