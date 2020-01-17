#include <stdio.h>
#include <osa_thr.h>
#include <includes.h>
#include <sysCfg.h>
#include <common.h>
#include <sys_ctrl.h>
#include "LCD12864.h"
#include <ipnc_gio_util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <keyChkThr.h>
#include <netconfig.h>
#include <sys/prctl.h>
#include <display.h>



const int flashOnTimeArry[] = {20,40,80,160,200,400,600,800};
static int  nFlashOnTimer = 0;

static pthread_t ThrId;
static void *keyChkThr(void* arg)
{
	int timeOut = 0;
	Bool isShortPress = False;
	Bool isLongPress = False;
	int cnt = 0, cnt1=0, cnt2 = 0;
	int ret;
	int len;
	setSysState(Sys_State_Run, MDVR_Sys_IDLE);
	prctl(PR_SET_NAME, (unsigned long)__func__, 0, 0, 0);
	start_bp(20, 1);
	char text[32];
	while(1)
	{
		if(ipnc_gio_read(GPIO_SYSTEM_KEY) == GPIO_HIGH)
		{
			cnt ++;
		}
		else
		{	
			timeOut = 0;
			if(cnt > 5 && cnt< 30)
				isShortPress = True;
			else if(cnt >= 30)
				isLongPress = True;
			cnt = 0;
		}
		if(isShortPress)
		{
			LOG_DBG("power key short press!!\r\n");
			start_bp(20, 1);
#if 0	
			startFlashTimerEvent(flashOnTimeArry[nFlashOnTimer]);
			char text[32];
			in_addr_t IP_i = net_get_ifaddr("eth0.2");
			if(IP_i > 0)
			{
				sprintf(text,"eth0.2 : %s", inet_ntoa(*((struct in_addr*)&IP_i)));
				GUIDrawText(0,80,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_NONE);
			}
			else
			{
				IP_i = net_get_ifaddr("apcli0");
				sprintf(text,"apcli0 : %s", inet_ntoa(*((struct in_addr*)&IP_i)));
				GUIDrawText(0,80,text,LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_NONE);
			}
#else
			ret = getSysState(Sys_State_Run,NULL);
			if(ret == MDVR_Sys_IDLE)
			{
				if(getSysState(sys_net_server_link,NULL) == True)
				{
					if(ipnc_gio_read(GPIO_LIGHT_DETECT)== GPIO_LOW)
					{
						execTask();
					}
					else//载玻片没有到位
						{
						setSysState(Sys_State_Run, MDVR_Sys_WARNING);
						showWarning(1);
						sleep(2);
						setSysState(Sys_State_Run, MDVR_Sys_IDLE);
					}
				}
				else//网络错误
				{
					
					setSysState(Sys_State_Run, MDVR_Sys_WARNING);
					showWarning(0);
					sleep(2);
					setSysState(Sys_State_Run, MDVR_Sys_IDLE);
				}
			}
			else if(ret == MDVR_Sys_EXE_TASK)
			{
				ExitTask();
				sleep(1);//等待退出
				showRecord(True);
			}
#endif		
			//saveJpeg();
			isShortPress = False;
		}
		
		if(isLongPress)
		{
			start_bp(200, 1);
			power_Off(); 
			isLongPress = False;
		}
		usleep(20000);
	/*	if(ipnc_gio_read(GPIO_LIGHT_DETECT) == GPIO_LOW)
		{
			cnt1 ++;
		}
		else
		{
			
			timeOut = 0;
			if(cnt1 > 5 && cnt1< 30)
			{
				start_bp(20, 1);
				LOG_DBG("gate key short press!!\r\n");
				start_led(8,-1,1);
				
			}
			cnt1 = 0;
		}
		*/
#if 0
		if(ipnc_gio_read(GPIO_GATE_DETECT) == GPIO_HIGH)
		{
				cnt1 ++;
		}
		else
		{
			if(cnt1 > 5 && cnt1< 30)
			{
				LOG_DBG("pos key short press!!\r\n");
				nFlashOnTimer ++;
				if(nFlashOnTimer>ARRAY_SIZE(flashOnTimeArry)-1)
				{	
					nFlashOnTimer = 0;
				}
				char text[16];
				sprintf(text,"LED %ds", flashOnTimeArry[nFlashOnTimer]);
				GUIDrawText(0,28,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_NONE);
				
			}
			cnt1 = 0;
		}
		if(ipnc_gio_read(GPIO_SYS_KEY_WPS) == GPIO_LOW)
		{
			cnt2 ++;
		}
		else
		{
			if(cnt2 > 5 && cnt2< 30)
			{
				LOG_DBG("wps key short press!!\r\n");
				startFlashTimerEvent(flashOnTimeArry[nFlashOnTimer]);
			}	
			else if(cnt2 > 50)// long press 10s
			{
				LOG_DBG("wps long press!!!\r\n");
				/*enter airkiss config net*/
				ret = getSysState(Sys_State_Run,NULL);
				if(ret == MDVR_Sys_IDLE)
					enterAirkissConfigNet();
			}
			cnt2 = 0;
		}
#else
		if(ipnc_gio_read(GPIO_SYS_KEY_WPS) == GPIO_LOW)
		{
			cnt2 ++;
		}
		else
		{
			timeOut = 0;
			if(cnt2 > 5 && cnt2<= 30)
			{
				start_bp(20, 1);
				LOG_DBG("wps short press!!!\r\n");
				ret = getSysState(Sys_State_Run,NULL);
				if(ret == MDVR_Sys_AIRKISS)
					exitAirkissConfigNet();
				else if(ret == MDVR_Sys_IDLE)
				{
					setSysState(Sys_State_Run, MDVR_Sys_WARNING);
					showSysInfo();
					sleep(2);
					setSysState(Sys_State_Run, MDVR_Sys_IDLE);
					
						
				}
				
			}	
			else if(cnt2 > 50)// long press 10s
			{
				start_bp(20, 1);
				LOG_DBG("wps long press!!!\r\n");
				/*enter airkiss config net*/
				ret = getSysState(Sys_State_Run,NULL);
				if(ret == MDVR_Sys_IDLE)
					enterAirkissConfigNet();
					
			}
			
			cnt2 = 0;
		}
#endif
		if(timeOut ++ > 1400)//about 5min
		{
			power_Off();
		}
			

	}
}
void keyChkThrCreat()
{
	 pthread_create(&ThrId, NULL, keyChkThr, NULL);
}
void keyChkThrDelet()
{

	pthread_cancel(ThrId);
	pthread_join(ThrId, NULL);
	LOG_DBG("\r\n")
}


