
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
