#include <UIDispalyThr.h>
#include <netLinkChkThr.h>
#include <keyChkThr.h>
#include <timerThr.h>
#include <includes.h>

void callback(void *data)
{
	
	LOG_WAR("\r\n");
}

void SysServiceInit()
{
	printf("==============SysServiceInit==================\r\n");
	timerThrCreat();
	UIDisplayThrCreat();
	netLinkThrCreat();
	keyChkThrCreat();
	RegisterProc(SYS_TIMER_TASK_SINGLE, 2000, "LED", callback, NULL);
}
void SysServiceExit()
{
	netLinkThrDelet();
	keyChkThrDelet();
	UIDisplayThrDelet();
	timerThrDelet();
}









