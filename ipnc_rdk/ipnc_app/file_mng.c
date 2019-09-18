
#include <file_mng.h>
#include <pthread.h>
#include <list_head.h>
#include <includes.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdarg.h>


static int IsFileQuit = 0,bNeedWrite = 0;
static pthread_mutex_t mutex;
pthread_t tFileMngThread;
static LIST_HEAD(gLogList);  
static pthread_mutex_t  LogMng_Mutex;  
static int isLog = 0;

//声明链表节点  
typedef struct {  
    int id;  //4//
    char text[128];
	unsigned long timeTamp;
    struct list_head _list;  //8
}logListNode;  
  
//遍历函数指针  
typedef void (*list_foreach_callBack)(logListNode *node,void *data);  



int IsFileThreadQuit()
{	
	int ret;	
	pthread_mutex_lock(&mutex);
	ret = IsFileQuit;	
	pthread_mutex_unlock(&mutex);	
	return ret;
}
/** * @brief	set file thread quit * 
	@param	none * 
	@retval	none 
*/
static void SetFileQuit()
{	
	pthread_mutex_lock(&mutex);
	IsFileQuit = 1;	
	pthread_mutex_unlock(&mutex);
}
/** * @brief	check if file need write * @param	none * @retval	ret = IsFileQuit */
static int FileNeedWrite()
{	
	int ret;	
	pthread_mutex_lock(&mutex);
	ret = bNeedWrite;	
	pthread_mutex_unlock(&mutex);
	return ret;
}
/** * @brief	set file to be updated * @param	none * @retval	none */
void SetFileUpdate(int flag)
{	
	pthread_mutex_lock(&mutex);
	bNeedWrite |= flag;	
	pthread_mutex_unlock(&mutex);
}
/** * @brief	file has been updated * @param	none * @retval	none */
static void FileUpdated(int flag)
{	
	pthread_mutex_lock(&mutex);
	bNeedWrite &= ~flag;	
	pthread_mutex_unlock(&mutex);
}





//添加节点  
int addListNode(struct list_head *list,logListNode *node)  
{  
	pthread_mutex_lock(&LogMng_Mutex); 
    list_add_tail(&node->_list,list);  
	pthread_mutex_unlock(&LogMng_Mutex);
}  
  
//节点遍历,并删除
void list_foreach(struct list_head *list,list_foreach_callBack fun,void *data)  
{  
    struct list_head *entry=NULL,*pos=NULL;  
    logListNode *stu=NULL;
	
    list_for_each_safe(entry,pos,list){  
		
        stu = list_entry(entry,logListNode,_list);  
        fun(stu,data);
		pthread_mutex_lock(&LogMng_Mutex);
		list_del(&(stu->_list)); 
		pthread_mutex_unlock(&LogMng_Mutex);
            //释放节点空间  
        free(stu);  
        stu = NULL;   
    }  
  
}  
static FILE *logFp = NULL;
void savelog_callBack(logListNode *list,void *data)  
{  
  //  LOG_ERR("type:%d,text:%s",list->id,list->text); 

	struct tm *pTm = localtime(&list->timeTamp);
	if(logFp == NULL)
	{
		char pathName[32];
	
		sprintf(pathName,"/root/log/%04d_%02d_%02d.txt",pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday);
		logFp = fopen(pathName, "a+");
		if(logFp == NULL)
		{
			LOG_WAR("savelog_callBack:fopen %s\n", strerror(errno));
			return ;
		}
	}
		fprintf(logFp,"[%02d:%02d:%02d]", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
		if(fputs((const char *)list->text,logFp) == EOF)
		{
				LOG_WAR("savelog_callBack:fputs\n");
				fclose(logFp);
				logFp= NULL;
		}
		
} 


void WriteLogToFile()
{
	if(isLog == 0)
			return 0;
	
	list_foreach(&gLogList,savelog_callBack,(void *)0);
	if(logFp != NULL)
	{
		fflush(logFp);
		fclose(logFp);
		
		logFp = NULL;
	}
} 

void initLogMng()
{
	isLog = 0;
		if(isLog == 0)
			return 0;
		else
			isLog = 20;
	pthread_mutex_init(&LogMng_Mutex, NULL);
}

void exitLogMng()
{
	if(isLog == 0)
			return 0;
	pthread_mutex_destroy(&LogMng_Mutex);
}



//打印字符串长度是52字节
int printToLog(int logType, const char* format, ...)
{
	
	if(isLog == 0)
		return 0;

	logListNode *node;
	node = (logListNode *)calloc(1,sizeof(logListNode));
	if(node == NULL)
	{
		LOG_WAR("addLogToList calloc ERR!\n");
		return -1;
	}
	va_list args;
    va_start(args, format);
    vsnprintf (node->text,128, format, args);
    va_end(args);
	node->timeTamp = time(NULL);
	node->id = logType;
	addListNode(&gLogList,node);
}



static void *ProcFileThread(void *arg)
{	
	static int timeout = 5;	
	while(IsFileThreadQuit() == 0)
	{		
		int flag = 0;		
		if(flag = FileNeedWrite())
		{		
			if(flag & FILE_SYS_ENV_UPDATE_MASK)			
			{		
			  //FileUpdated();				
						
				FileUpdated(FILE_SYS_ENV_UPDATE_MASK);//清除升级完成标志			
			}		
			if(flag & FILE_SYS_LOG_UPDATE_MASK)			
			{				//printf("----------FILE_SYS_LOG_UPDATE_MASK\n");				
				WriteLogToFile();//2s更新一次日志文件				FileUpdated(FILE_SYS_LOG_UPDATE_MASK);							}					
			}		
				
		}
		if(timeout<1)		
		{					
			SetFileUpdate(FILE_SYS_LOG_UPDATE_MASK);			
			timeout = 50;		
		}		
		timeout --;	
		usleep(200000);	
	}
	return NULL;
}



int tFileMngInit()
{	
	/* Initialize the mutex which protects the global data */
    if( pthread_mutex_init(&mutex, NULL) != 0 )
    {
    	printf("Sem_Creat init faill!!\n");
		return -1;
    }
	
	if(pthread_create(&tFileMngThread, NULL, ProcFileThread, NULL))
	{		
		printf("FileMngThread create fail\n");		
		return 0;	
	}		/* Create communication between file manager and system server */	

}
void tFileMngExit()
{
	SetFileQuit();
	pthread_mutex_destroy(&mutex);

}



