#include <stdio.h>
#include <sys/prctl.h>

#include <includes.h>
#include <sysCfg.h>
#include <common.h>
#include <timerThr.h>


static taskList *pTaskHead = NULL;

/*min 50ms */
static pthread_t ThrId;
static pthread_mutex_t mutex;
static void *timerTaskChkThr(void* arg)
{
	taskList *pTaskListCurNode = pTaskHead;
	struct timespec stCurTime;
	int32_t u32TimeDiff; /**<unit millisecond */
	taskList *pCurTaskNode = pTaskHead->next;
	timerTask *pCurTask;
	
	prctl(PR_SET_NAME, (unsigned long)__func__, 0, 0, 0);
	
	while(1)
	{
		pthread_mutex_lock(&mutex);
		clock_gettime(CLOCK_MONOTONIC, &stCurTime);
		if(pCurTaskNode==NULL)
		{
			pCurTaskNode = pTaskHead->next;
			pthread_mutex_unlock(&mutex);
			if(pCurTaskNode == NULL)//任务为空
			{
				usleep(IDLE_CHECK_INTERVAL);
				continue;
			}
		}
		pCurTask = &pCurTaskNode->task;
		u32TimeDiff = (pCurTask->stCheckTime.tv_sec - stCurTime.tv_sec) * 1000 +
                (pCurTask->stCheckTime.tv_nsec-stCurTime.tv_nsec) / 1000000;
	
		if(u32TimeDiff >= pCurTask->u32Time)
		{
			if(pCurTask->pfnProc)
			{
				//LOG_DBG("exec task %s\r\n", pCurTask->name);
				pCurTask->pfnProc(pCurTask->pPrivData);	
			}
			if(pCurTask->mode == SYS_TIMER_TASK_SINGLE)//释放任务
			{
				delNode(pCurTask->procId);
			}
			else if(pCurTask->mode == SYS_TIMER_TASK_CONTINUE)//更新下次运行的时间
			{
				pCurTask->stCheckTime.tv_sec = stCurTime.tv_sec+pCurTask->u32Time/1000;
				pCurTask->stCheckTime.tv_nsec = stCurTime.tv_sec+pCurTask->u32Time%1000*1000000;
			}
		}
		pCurTaskNode = pCurTaskNode->next;
		pthread_mutex_unlock(&mutex);
		usleep(TASK_CHECK_INTERVAL);
	}
	
}

void delNode(int procId)
{
	taskList *pTmpNode = pTaskHead->next;
	taskList *pLastTaskNode = pTaskHead;
	while(pTmpNode)
	{
		if(pTmpNode->task.procId == procId)
		{
			break;
		}
		pLastTaskNode = pTmpNode;
		pTmpNode = pTmpNode->next;
	
	}
	if(pTmpNode)
	{
		pLastTaskNode->next = pTmpNode->next;
		free(pTmpNode);
	}
	
}
void deInitNode(taskList*pNode)
{
	pNode->task.mode = SYS_TIMER_TASK_SINGLE;
	pNode->task.pfnProc = NULL;
	pNode->task.pPrivData = NULL;
	pNode->next = NULL;
	pNode->task.procId = 0;
}
void timerThrCreat()
{
	pTaskHead = (taskList *)malloc(sizeof(taskList));
	if(pTaskHead == NULL)
	{
		LOG_ERR("malloc fail\r\n");
		return ;
	}
	deInitNode(pTaskHead);
	
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&ThrId, NULL, timerTaskChkThr, NULL);
}
void timerThrDelet()
{

	pthread_cancel(ThrId);
	pthread_join(ThrId, NULL);
	LOG_DBG("\r\n")
}
/*
	mode 
	time： ms min：50ms
*/
int RegisterProc(int mode, uint32_t time,const char *name,timerTaskProc pfnProc,void *pPrivData)
{
	struct timespec stCurTime;
	
	taskList*pNewTaskNode = (taskList *)malloc(sizeof(taskList));
	if(pNewTaskNode==NULL)
	{
		LOG_ERR("\r\n")
		return -1;
	}
	pthread_mutex_lock(&mutex);
	pNewTaskNode->next = pTaskHead->next;
	pTaskHead->next =pNewTaskNode;
	pNewTaskNode->task.procId = ++ pTaskHead->task.procId;
	pNewTaskNode->task.mode = mode;
	pNewTaskNode->task.pfnProc = pfnProc;
	pNewTaskNode->task.pPrivData = pPrivData;
	pNewTaskNode->task.u32Time = time;
	strcpy(pNewTaskNode->task.name,name);
	clock_gettime(CLOCK_MONOTONIC, &stCurTime);
	pNewTaskNode->task.stCheckTime.tv_sec = stCurTime.tv_sec+time/1000;
	pNewTaskNode->task.stCheckTime.tv_nsec = stCurTime.tv_sec+time%1000*1000000;
	pthread_mutex_unlock(&mutex);
	return pNewTaskNode->task.procId;
}	
void UnRegisterProc(int procId)
{
	pthread_mutex_lock(&mutex);
	if(procId > 0)
	{
		delNode(procId);
	}
	pthread_mutex_unlock(&mutex);
}