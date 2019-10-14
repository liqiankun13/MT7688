/*目前可以用作各个进程间共享的变量和状态*/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <osa_shm.h>

union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};


static inline int semaphore_p(OSA_ShmHndl *hndl)
{
    // 对信号量做减1操作，即等待P（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;//P()
    sem_b.sem_flg = SEM_UNDO;
    if (semop(hndl->semId, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
 
    return 1;
}
 
static inline int semaphore_v(OSA_ShmHndl *hndl)
{
    // 这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; // V()
    sem_b.sem_flg = SEM_UNDO;
    if (semop(hndl->semId, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_v failed\n");
        return 0;
    }
 
    return 1;
}

int OSA_shmCreate(OSA_ShmHndl *hndl, int size)
{
	key_t  key;
	union semun tmp_sem;//a传值
	int isAlreadyCreat = 0;
	
	
	if((key = ftok("/dev/null", 1)) < 0)
    {
      printf("ftok error:%s\n", strerror(errno));
      return -1;
    }
	hndl->size = size + sizeof(struct fifo);
	hndl->shmid = shmget(key, size, 0666 | IPC_CREAT|IPC_EXCL);
	if (hndl->shmid == -1)
    {
		if((errno == EEXIST) && ((hndl->shmid = shmget(key, size, 0666)) != -1))
		{
			isAlreadyCreat = 1;
		}
		else
		{
			 printf("shmget error:%s\n", strerror(errno));
			return -1;
		}
    }
	hndl->mem = shmat(hndl->shmid,  (void*)0, 0);
	if(!hndl->mem)
	{
		fprintf(stderr, "shmat fail\n");
        return -1;
	}
	/**/
	hndl->semId = semget(key, 1, 0666 | IPC_CREAT|IPC_EXCL);
	if (hndl->semId == -1)
    {
       	if((errno == EEXIST) && ((hndl->semId = semget(key, 1, 0666)) != -1))
		{
			;
		}
		else
		{
			 printf("semget error:%s\n", strerror(errno));
			return -1;
		}
    }
	else
	{
		
        tmp_sem.val = 1;
        if(semctl(hndl->semId,0,SETVAL,tmp_sem)==-1)//0代表信号量下表
        {
            perror("semctl init error");
			return -1;
        }	
	}
	hndl->fifo = (struct fifo  *)hndl->mem;

	if(!isAlreadyCreat)
	{
		hndl->fifo ->capacity = size;
		hndl->fifo ->isEmpty = 1;
		hndl->fifo ->isFull = 0;
		//hndl->fifo ->addr = hndl->mem+sizeof(struct fifo);
		hndl->fifo ->use = 0;
		hndl->fifo ->curRd = 0;
		hndl->fifo ->curWr = 0;
	}
	return 0;
}
void *OSA_shmGetMem(OSA_ShmHndl *hndl)
{
	return hndl->mem;
}
int OSA_shmWrite(OSA_ShmHndl *hndl, int start, void *addr, int len)
{
	if(start < 0 || addr == NULL)
	{
		return -1;
	}
	if(start+len > hndl->size )
	{
		return -1;
	}
	semaphore_p(hndl);
	memcpy(hndl->mem+start, addr, len);
	semaphore_v(hndl);
	return 0;
}
int OSA_shmRead(OSA_ShmHndl *hndl,int start, void *addr, int len)
{
	if(start < 0 || addr == NULL)
	{
		return -1;
	}
	if(start+len > hndl->size )
	{
		return -1;
	}
	semaphore_p(hndl);
	memcpy(addr, hndl->mem+start, len);
	semaphore_v(hndl);
	return ;
}
int OSA_fifoWrite(OSA_ShmHndl *hndl, void *addr, int len)
{
	struct fifo  *pFifo = hndl->fifo;
	void *fifoAddr = hndl->mem+sizeof(struct fifo);
	int wSize;

	if(addr == NULL || len < 0)
	{
		return -1;
	}
	//printf("%d %d %d %d\r\n",pFifo->curWr, pFifo->use,pFifo->capacity, len);

	semaphore_p(hndl);
	if(pFifo->isFull)
	{
		semaphore_v(hndl);
		return -1;
	}
	wSize = len;
	if(wSize > pFifo->capacity)
		wSize = pFifo->capacity;
	if(pFifo->capacity < (pFifo->curWr + len))
	{
		wSize = pFifo->capacity - pFifo->curWr;//得到可以写的大小
		pFifo->isFull = 1;
		//printf("%d %d\r\n", wSize,pFifo->curWr);
	}
//	printf("===%d %d %x\r\n", wSize, pFifo->curWr, fifoAddr);
	memcpy(fifoAddr + pFifo->curWr,addr, wSize);
//	printf("%d %d\r\n", wSize, pFifo->curWr);
	pFifo->curWr += wSize;
	pFifo->use += wSize;
	pFifo->isEmpty = 0;
	semaphore_v(hndl);
	//printf("write -> %s\r\n", fifoAddr);
	return wSize;
}
int OSA_fifoRead(OSA_ShmHndl *hndl, void *addr, int len)
{
	struct fifo  *pFifo = (struct fifo  *)hndl->mem;
	void *fifoAddr = hndl->mem+sizeof(struct fifo);
	int rSize;
	if(addr == NULL || len < 0)
	{
		return -1;
	}
//	printf("%d %d\r\n", pFifo->use,pFifo->isEmpty);
	
	semaphore_p(hndl);
	if(pFifo->isEmpty)
	{
		semaphore_v(hndl);
		return 0;
	}
	rSize = len;
	if(rSize > pFifo->use)
		rSize = pFifo->use;
	if(pFifo->capacity < (pFifo->curRd + len))
	{
		rSize = pFifo->capacity - pFifo->curWr;
		pFifo->isEmpty = 1;
	}
	
	memcpy(addr, fifoAddr + pFifo->curRd, rSize);
	pFifo->curRd += rSize;
	pFifo->use -= rSize;
	if(pFifo->isEmpty)
	{
		pFifo->curRd = 0;
		pFifo->curWr = 0;
		pFifo->use = 0;
	}
	pFifo->isFull = 0;
	semaphore_v(hndl);
	return rSize;
}
void OSA_shmDelete(OSA_ShmHndl *hndl)
{
	if (shmdt(hndl->mem) == -1)
    {
        fprintf(stderr, "shmdt fail\n");
        return ;
    }
	if (shmctl(hndl->shmid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        return ;
    }
	if(semctl(hndl->semId,0,IPC_RMID)==-1)//0代表信号量集
	{
		fprintf(stderr, "semctl(IPC_RMID) failed\n");
		return ;
	}
}
/*
int main()
{
	OSA_ShmHndl pHndl;
	int ret;
	ret = OSA_shmCreate(&pHndl, 1024);
    if(ret < 0)
	{
		printf("OSA_shmCreate\r\n");
		return 0;
	}
	char buf[32];
	while(1)
	{
		if((ret = OSA_fifoRead(&pHndl,  buf, sizeof(buf) - 1)) > 0)
		{
			buf[ret] = 0;
			printf("%s\r\n",buf);
		}
		sleep(1);
		if(strstr(buf, "quit") != NULL)
			break;
	}
	OSA_shmDelete(&pHndl);
}
int main()
{
	OSA_ShmHndl pHndl;
	int ret;
	ret = OSA_shmCreate(&pHndl, 1024);
    if(ret < 0)
	{
		printf("OSA_shmCreate\r\n");
		return 0;
	}
	char buf[32];
	int n ;
	while(1)
	{
		scanf("%s", buf);
		OSA_fifoWrite(&pHndl,  buf, strlen(buf));
		if(strstr(buf, "quit") != NULL)
			break;
	}
	OSA_shmDelete(&pHndl);
} 

*/

