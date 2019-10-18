#include <stdio.h>
#include <osa_thr.h>
#include <sys/prctl.h>
#include <display.h>
#include <includes.h>
#include <sysCfg.h>
#include <common.h>
#include <sys_ctrl.h>
#include "LCD12864.h"
#include <ipnc_gio_util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <UIDispalyThr.h>


static inline void uiDisplayProc()
{
	sys_state_st sysState = {0};
	static sys_state_st lastSysState = {0};
	char text[64];
	static unsigned int cnt = 0;  
	getSysState(Sys_State_All,&sysState);
	if(cnt%2 == 0)
	{
		showSysTime();
	}
	cnt ++;
	
	if(memcmp(&sysState, &lastSysState,sizeof(sys_state_st))== 0)//没有状态更新
	{
		return ;
	}
	
	if(sysState.runState == MDVR_Sys_AIRKISS)//显示二维码
	{
		showAirkissQRcode(True);
	}
	else if(sysState.runState == MDVR_Sys_IDLE)
	{
		if(lastSysState.runState == MDVR_Sys_AIRKISS)
			showRecord(True);
	}
	showSysNet(sysState.runState ,sysState.wifiLink.linkState);
	
	memcpy(&lastSysState,&sysState,sizeof(sys_state_st));
	printf("run = %d, [wifi] rssi=%d [server] online=%d \r\n", sysState.runState, sysState.wifiLink.linkState, sysState.serverState);
}


static pthread_t ThrId;
static void *UIDisplayThr(void* arg)
{
	prctl(PR_SET_NAME, (unsigned long)__func__, 0, 0, 0);
	GUICleanScreen(0, 0, _LCD_WIDTH,_LCD_HIGHT);
	if(gSYS_cfg_para.sysPara.is1thBoot) //第一次开机
	{
		showAirkissQRcode();
		SetConfigFileIntValue("sys","is1thBoot",False,SYS_CFG_PATHNAME);
	}
	else //非第一次开机，显示上次检查结果
		{
		showRecord(True);
	}
	showCamera();
	while(1)
	{
		msleep(500);
		uiDisplayProc();//1s
	}


}
void UIDisplayThrCreat()
{	
	extern uint8_t wifi_signal_icon[0][24*20*2];
	extern uint8_t server_icon[0][24*20*2];
	extern uint8_t  result_bmp[0][54*18*2];
	 GUILoadBmp("/root/res/images/wifi-0.bmp",24, 20,  wifi_signal_icon[0]);
	 GUILoadBmp("/root/res/images/wifi-4.bmp",24, 20,  wifi_signal_icon[1]);
	 GUILoadBmp("/root/res/images/wifi-3.bmp",24, 20,  wifi_signal_icon[2]);
	 GUILoadBmp("/root/res/images/wifi-2.bmp",24, 20,  wifi_signal_icon[3]);
	 GUILoadBmp("/root/res/images/server-0.bmp",24, 20,  server_icon[0]);
	 GUILoadBmp("/root/res/images/server-1.bmp",24, 20,  server_icon[1]);
	 GUILoadBmp("/root/res/images/result_0.bmp",56, 18,  result_bmp[0]);
	 GUILoadBmp("/root/res/images/result_1.bmp",56, 18,  result_bmp[1]);
	 GUILoadBmp("/root/res/images/result_2.bmp",56, 18,  result_bmp[2]);
	 GUILoadBmp("/root/res/images/result_3.bmp",56, 18,  result_bmp[3]);
	
	// GUILoadBmp("/root/wifi-4.bmp",24, 20,  wifi_signal_icon[4]);
	 pthread_create(&ThrId, NULL, UIDisplayThr, NULL);
}
void UIDisplayThrDelet()
{

	pthread_cancel(ThrId);
	pthread_join(ThrId, NULL);
	LOG_DBG("\r\n")
}


