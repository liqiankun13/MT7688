
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <carmera_v4l2.h>
#include <time.h>
#include <signal.h>
#include <includes.h>
#include <common.h>
#include <sys_misc.h>
#include <sysCfg.h>
#include <display.h>


static Bool  isExit = False;

void  Hal_Init()
{
	Init_Dislay();
	Init_Cameral("/dev/video0");
	Start_Cameral();
}
void  Hal_Exit()
{
	Stop_Cameral();
	Exit_Cameral();
}

void requestExit()
{
	isExit = True;
}

static void SignalHander(int signal)
{	static Bool bExit = False;	
	if(signal == SIGKILL || signal == SIGINT)	
	{
		if(!bExit)
			requestExit();	
	}
	else		
	{		
		LOG_WAR("unhandle signal<%d>!", signal);			
	}
}


void CaptureAllSignal()
{	
	signal(SIGPIPE, SIG_IGN);			
	signal(SIGUSR1, SignalHander);	
	signal(SIGKILL, SignalHander);	
	//signal(SIGSEGV, SignalHander);	
	signal(SIGFPE, SignalHander); 
	signal(SIGINT, SignalHander); 
}
#define _download_ 0
#if _download_
int main(void)
{
	char text[64];
	Init_Dislay();
	sprintf(text, "check upgrage...");
	GUIDrawText(20,100,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_BLACK);
	sleep(1);
	download_file("http://115.231.64.178:10088/app.tar.bz2", "app.tar.bz2");
	if(access("app.tar.bz2", F_OK|R_OK) == 0)
	{
		sprintf(text, " unzip app. . .");
		GUIDrawText(20,100,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_BLACK);
		system("tar xvf app.tar.bz2");
		system("chmod a+x * -R");
		sleep(1);
		sprintf(text, " upgrage ok");
		GUIDrawText(20,100,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_BLACK);
		sleep(1);
		//unlink("app.tar.bz2");
	}
	else
		{
		sprintf(text, "upgrage fail...");
		GUIDrawText(20,100,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_BLACK);
		sleep(1);
		Exit_Dislay();
		return -1;
	}
	Exit_Dislay();
	return 0;
	
}
#else
int main(void)
{
	SYS_initALLCfgPara(&gSYS_cfg_para);
	tFileMngInit();
	initLogMng();
	CaptureAllSignal();
	initSysState();
	Hal_Init();
	MiscMngInit();
	SysServiceInit();
	
	while(!isExit)
	{
		sleep(1);
	}
	LOG_INFO("System Exit...\r\n");
	SysServiceExit();
	MiscMngExit();
	Hal_Exit();
	return 0;

 
}
#endif
