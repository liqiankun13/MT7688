#include <stdio.h>
#include <osa_thr.h>
#include <includes.h>
#include <cfg_read_write.h>
#include <common.h>
#include <sys_ctrl.h>
#include "LCD12864.h"
#include <ipnc_gio_util.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static inline void uiDisplayProc()
{
	mdvr_state_st sysState = {0};
	static mdvr_state_st lastSysState = {0};
	char text[64];
	static unsigned int cnt = 0;  
	getSysState(Sys_State_All,&sysState);
	//printf("sysState.runState = %d\r\n", sysState.runState);
		
	if(cnt%2 == 0)
	{
		showSysTime();
	}
	showSysNet(sysState.runState ,sysState.wifiLink.RSSI);
	cnt ++;
	
	if(memcmp(&sysState, &lastSysState,sizeof(mdvr_state_st))!= 0)
		memcpy(&lastSysState,&sysState,sizeof(mdvr_state_st));
	else
	{	
		return ;
	}

	printf("run = %d, [wifi] rssi = %d\r\n", sysState.runState, sysState.wifiLink.RSSI);
}


static pthread_t ThrId;
static void *UIDisplayThr(void* arg)
{
	Clear_Display(0, 0, 128,64);
	while(1)
	{
		msleep(500);
		uiDisplayProc();//1s
	}


}
void UIDisplayThrCreat()
{
	 pthread_create(&ThrId, NULL, UIDisplayThr, NULL);
}
void UIDisplayThrDelet()
{

	pthread_cancel(ThrId);
	pthread_join(ThrId, NULL);
	LOG_DBG("\r\n")
}


