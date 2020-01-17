#include <includes.h>
#include <ipnc_gio_util.h>
#include <sysCfg.h>
#include <sys_ctrl.h>
#include <common.h>
#include <display.h>
#include "LCD12864.h"
#include <wifi.h>
#include <httpThread.h>
#include <signal.h>
#include <airkissThr.h>
#include <display.h>
#include <timerThr.h>


uint8_t wifi_signal_icon[4][24*20*2];
uint8_t		server_icon[2][24*20*2];
volatile uint8_t		result_bmp[4][56*18*2];

void showRecord(Bool isShow)
{
	char text[16];
	if(!isShow)
	{
		GUICleanScreen(0,25,240,216);
		return ;
	}

	uint8_t *pAddr  = (uint8_t *)malloc(240*216*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp("/root/res/images/record.bmp",240, 216,  pAddr);
	GUIImageDraw(0,25,240,216, pAddr);
	sprintf(text, "%02d", gSYS_cfg_para.report.percent);
	GUIDrawText(64,80,text, LCD_FONT_MIDLLE, LCD_FILL_WHITE, LCD_FILL_NONE);
	int len = sprintf(text, "%s", gSYS_cfg_para.report.date);
	GUIDrawText((240-len*12)/2,172,text, LCD_FONT_BIG, LCD_FILL_RED, LCD_FILL_GREEN);
	free(pAddr);
	
}
void showReport(Bool isShow)
{
	char text[16];
	if(!isShow)
	{
		GUICleanScreen(0,25,240,216);
		return ;
	}

	uint8_t *pAddr  = (uint8_t *)malloc(240*216*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp("/root/res/images/report.bmp",240, 216,  pAddr);
	GUIImageDraw(0,25,240,216, pAddr);
	sprintf(text, "%02d", gSYS_cfg_para.report.percent);
	GUIDrawText(64,74,text, LCD_FONT_MIDLLE, LCD_FILL_WHITE, LCD_FILL_NONE);
	int result =gSYS_cfg_para.report.result;
	if( result <4 &&result>=0)
	{
		char tmp[32];
		sprintf(tmp, "/root/res/images/result_%d.bmp", result);
		GUILoadBmp(tmp,56, 18,  pAddr);
		GUIImageDraw(120,172,56,18, pAddr);
	}
	time_t nowT = time(NULL);		
	struct tm *tm_now = localtime(&nowT);	
	
	sprintf(gSYS_cfg_para.report.date, "%02d#%02d@ %02d:%02d",tm_now->tm_mon+1, tm_now->tm_mday,tm_now->tm_hour,tm_now->tm_min);
	GUIDrawText(98,197,gSYS_cfg_para.report.date, LCD_FONT_BIG, LCD_FILL_RED, LCD_FILL_GREEN);
	free(pAddr);
	SetConfigFileIntValue("report","result",gSYS_cfg_para.report.result,SYS_CFG_PATHNAME);
	SetConfigFileIntValue("report","Percent",gSYS_cfg_para.report.percent,SYS_CFG_PATHNAME);
	SetConfigFileStringValue("report","date",gSYS_cfg_para.report.date,SYS_CFG_PATHNAME);
	
}

void showCamera()
{
	char *pCameraPath;
	uint32_t ret = getSysState(sys_dev_Camera, NULL);
	if(ret == SYS_DEV_Camera_Normal)
	{
		pCameraPath = "/root/res/images/camera_0.bmp";
	}
	else if(ret == SYS_DEV_Camera_Err)
	{
		pCameraPath = "/root/res/images/camera_1.bmp";
	}
	else
		{
		pCameraPath = "/root/res/images/camera_1.bmp";
	}
		

	uint8_t *pAddr  = (uint8_t *)malloc(24*20*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp(pCameraPath,24, 20,  pAddr);
	GUIImageDraw(180,0,24,20, pAddr);
	free(pAddr);
}

void showAirkissQRcode(Bool isShow)
{
	if(!isShow)
	{
		GUICleanScreen(41,65,160,158);
		return ;
	}

	uint8_t *pAddr  = (uint8_t *)malloc(160*158*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp("/root/res/images/airKiss.bmp",160, 158,  pAddr);
	GUIImageDraw(41,65,160,158, pAddr);
	free(pAddr);
}
void showWelcom(Bool isShow)
{
	if(!isShow)
	{
		GUICleanScreen(0,25,240,216);
		return ;
	}

	uint8_t *pAddr  = (uint8_t *)malloc(240*216*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp("/root/res/images/welcom.bmp",240, 216,  pAddr);
	GUIImageDraw(0,25,240,216, pAddr);
	free(pAddr);
}
void showWarning(int select)
{
	uint8_t *pAddr  = (uint8_t *)malloc(240*216*2);
	char *path = NULL;
	if(select == 0)
		path = "/root/res/images/net_err.bmp";
	else if(select == 1)
		path = "/root/res/images/slide_err.bmp";
	else
		path = "/root/res/images/welcom.bmp";
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp(path,240, 216,  pAddr);
	GUIImageDraw(0,25,240,216, pAddr);
	free(pAddr);
}
void showSysInfo()
{
	char text[32];
	
	//GUIFullScreen(0,25,240,216, RGB(41,40,25));
	sprintf(text,"version:%s", "1.2");
	GUIDrawText(8,100,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_NONE);
	in_addr_t IP_i = net_get_ifaddr("apcli0");
	sprintf(text,"wlan:%s", inet_ntoa(*((struct in_addr*)&IP_i)));
	GUIDrawText(8,130,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_NONE);
}

void showSysTime()
{
	char text[32];
	int range;
	time_t nowT = time(NULL);		
	struct tm *tm_now = localtime(&nowT);	
#if 0
	sprintf(text, "%04d-%02d-%02d",tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday);
	GUIDrawText(0,0,text, LCD_FONT_MIDLLE);
	sprintf(text, "%02d:%02d:%02d", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	GUIDrawText(0,18,text, LCD_FONT_BIG);
#else
	sprintf(text, "%02d#%02d@ %02d:%02d",tm_now->tm_mon+1, tm_now->tm_mday,tm_now->tm_hour,tm_now->tm_min);
	GUIDrawText(0,0,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_BLACK);
#endif 

}
void showSysNet( int runState, int range)
{
	Bool isOnline = False;
	if(runState == MDVR_Sys_AIRKISS)
	{
		GUIImageDraw(210, 0, 24,20, wifi_signal_icon[0]);
		return ;
	}
	/*wifi显示*/
	GUIImageDraw(210, 0, 24,20, wifi_signal_icon[range-WIRELESS_RSSI_DISCONMECT]);
	isOnline = getSysState(sys_net_server_link, NULL);
	GUIImageDraw(150, 0, 24,20, server_icon[isOnline]);
}

int saveJpeg_CallBack(const char *name, char *buf, size_t len)
{
	int fd = open(name, O_WRONLY|O_CREAT|O_EXCL, 0777);
	if(fd < 0)
		return -1;
	if(write(fd, buf, len) < 0)
	{	
		return -1;
	}
	close(fd);
	uploadPicture(name);//上传图片文件
	return 0;
}

void saveJpeg()
{	
	char pathName[64];
	time_t nowT = time(NULL);
	struct tm *tm_now = localtime(&nowT);
	
	sprintf(pathName, "/tmp/%04d%02d%02d_%02d%02d%02d.jpg", tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	LOG_INFO("Capture %s\r\n", pathName);
	save_Picture(saveJpeg_CallBack,pathName);
}



/*这是一个使用前准备操作*/
/*Bool execReadyTask()
{
	int timeOut = 500; //5s如果还不能开启，测提示出错
	start_MOTOR();//开启陀机
	while(timeOut --)
	{
		usleep(10000);
		if(True == getGateState())//检测们开启
		{
			stop_MOTOR();
			return True;
		}
	}
	return False; //开启超时
}*/
void execHeat()
{
	char text[32];
	uint8_t *pAddr	= (uint8_t *)malloc(240*216*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp("/root/res/images/heat.bmp",240, 216,  pAddr);
	GUIImageDraw(0,25,240,216, pAddr);
	
	free(pAddr);
}
void execCheck()
{
	char text[32];
	uint8_t *pAddr	= (uint8_t *)malloc(240*216*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp("/root/res/images/check.bmp",240, 216,  pAddr);
	GUIImageDraw(0,25,240,216, pAddr);
	
	free(pAddr);
}
void execNetAbort()
{
	char text[32];
	uint8_t *pAddr	= (uint8_t *)malloc(240*216*2);
	if(pAddr == NULL)
	{
		LOG_ERR("malloc <%s>\r\n", strerror(errno));
		return ;
	}
	GUILoadBmp("/root/res/images/net_abort.bmp",240, 216,  pAddr);
	GUIImageDraw(0,25,240,216, pAddr);
	
	free(pAddr);
}

void *taskThr(void *arg)
{
	execHeat();//加热执行
	
}

static int thrId;
static int state; 
static int procId; 

void execTaskStateMachine(void *arg)
{
	static int heatCount = 50, retryCount;
	char text[32];
	int ret;
	LOG_INFO("\r\n");
	switch(state)
	{
		case 0:
			ipnc_gio_write(GPIO_FLASH_LIGTHT_EN,GPIO_HIGH);//开启摄像头补光灯
			ipnc_gio_write(GPIO_HEAT_EN,GPIO_HIGH);
			execHeat();
			heatCount = 50;
			gSYS_cfg_para.report.isValid = False;
			state = 1;
			break;
		case 1:
			sprintf(text, "%02d", heatCount);
			heatCount -- ;
			GUIDrawText(160,169,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_GREEN);
			if(heatCount == 0)
			{
				ipnc_gio_write(GPIO_HEAT_EN,GPIO_LOW);//停止加热
				ret = getSysState(sys_net_server_link,NULL);
				if(ret == True)//网络链接ok
					state = 2;
				else
					{
					state = 3;
					execNetAbort();
					retryCount = 30;
				}	
			}
			break;
		case 2:
			execCheck();
			state = 4;
			break;
		case 3://网络异常
			ret = getSysState(sys_net_server_link,NULL);
			if(ret == True)
			{
				state = 2;
			}
			else
			{
				sprintf(text, "retry %02d ...", retryCount);
				retryCount --;
				GUIDrawText(160,169,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_GREEN);
				if(retryCount == 0)//30s后链接不到网络退出
				{
					state = 5;
				}
			}
			break;
		case 4://上传图片获取结果
			state = 5;
			usleep(1000000);
			saveJpeg();
			usleep(100000);
			ipnc_gio_write(GPIO_FLASH_LIGTHT_EN,GPIO_LOW);
			break;
		case 5:
			start_bp(200,1);
			if(gSYS_cfg_para.report.isValid)
			{
				showReport(True);
			}
			else
				{
				showReport(True);
				int len = sprintf(text,"Check Error.");
				GUIDrawText((240-len*12)/2,74,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_NONE);
			}
			state = 6;
			sleep(1);
			start_bp(200,1);
			sleep(1);
			start_bp(200,1);
			break;
		case 6:
			break;
		default:
			break;
	
	}
}
 
Bool execTask()
{
	state = 0;
	
	procId = RegisterProc(SYS_TIMER_TASK_CONTINUE, 1000, "task", execTaskStateMachine, NULL);
	if(procId>0)
	{
		start_led(2,-1,1); //slow flash
		setSysState(Sys_State_Run,MDVR_Sys_EXE_TASK);
	}
	return False; //开启超时
}
void ExitTask()
{
	start_led(1,1,1);
	UnRegisterProc(procId);
	setSysState(Sys_State_Run,MDVR_Sys_IDLE);
}


void func()
{
	printf("timer is arrivered\r\n");
	ipnc_gio_write(GPIO_FLASH_LIGTHT_EN,GPIO_LOW);

}
void startFlashTimerEvent(int timeVal)
{
	ipnc_gio_write(GPIO_FLASH_LIGTHT_EN,GPIO_HIGH);
	//RegisterProc(SYS_TIMER_TASK_SINGLE, timeVal, "flash LED", func, NULL);
	usleep(timeVal*1000);
	ipnc_gio_write(GPIO_FLASH_LIGTHT_EN,GPIO_LOW);
	printf("timer is arrivered\r\n");
	
}


void power_Off()
{
	LOG_INFO("System is ready poweroff!!!\r\n");
	sync();
	sleep(1);
	ipnc_gio_write(GPIO_SYS_KEY_POWEROFF,GPIO_LOW);
}

void enterAirkissConfigNet()
{
	
	if(getSysState(Sys_State_Run, NULL)  != MDVR_Sys_IDLE)
	{
		LOG_WAR("\r\n");
		return ;
	}
	start_led(8,-1, 1); //fast flash
	setSysState(Sys_State_Run, MDVR_Sys_AIRKISS);
	airKissThrCreat();
}
void exitAirkissConfigNet()
{
	airKissThrDelete();
	setSysState(Sys_State_Run, MDVR_Sys_IDLE);
}


