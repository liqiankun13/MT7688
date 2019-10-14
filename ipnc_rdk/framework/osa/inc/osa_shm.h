

#ifndef _OSA_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _OSA_SHM_H_

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <includes.h>

struct fifo {
	int curRd;
	int curWr;
	int use;
	int capacity;
	int isFull;
	int isEmpty;
	void *addr;

};

typedef struct {
	int shmid;
	void *mem;
	int size;
	int semId;
	struct fifo *fifo;
}OSA_ShmHndl;

int OSA_shmCreate(OSA_ShmHndl *hndl, int size);
void *OSA_shmGetMem(OSA_ShmHndl *hndl);
int OSA_shmWrite(OSA_ShmHndl *hndl, int start, void *addr, int len);
int OSA_shmRead(OSA_ShmHndl *hndl,int start, void *addr, int len);
int OSA_fifoWrite(OSA_ShmHndl *hndl, void *addr, int len);
int OSA_fifoRead(OSA_ShmHndl *hndl, void *addr, int len);
void OSA_shmDelete(OSA_ShmHndl *hndl);



#ifdef __cplusplus
}
#endif /* _OSA_SHM_H_ */
#endif /* _OSA_H_ */



