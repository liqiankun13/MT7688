#ifndef _TIMER_RHREAD_H
#define _TIMER_RHREAD_H

#include <common.h>

typedef int (*timerTaskProc)(void  *pPrivData);


typedef struct _timerTask{
	int procId;
	char name[32];
	int mode;
	uint32_t u32Time;
	struct timespec stCheckTime;
	void *pPrivData;
	timerTaskProc pfnProc;
}timerTask;
typedef struct _taskList{
	timerTask task;
	struct _taskList *next;
} taskList;

/** check interval if none task in checking */
#define IDLE_CHECK_INTERVAL (1000000)
/** check interval if any task in checking */
#define TASK_CHECK_INTERVAL (50000)

enum {
	SYS_TIMER_TASK_SINGLE,
	SYS_TIMER_TASK_CONTINUE
};



void timerThrCreat();
void timerThrDelet();
int RegisterProc(int mode, uint32_t time,const char *name,timerTaskProc pfnProc,void *pPrivData);


void delNode(int procId);

#endif


