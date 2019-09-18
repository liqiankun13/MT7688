/*
 * SPI.c
 *
 *  Created on: 2018��8��2��
 *      Author: cfan
 */
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include "SPI.h"
#include <errno.h>  	// ����errno����Ҫ��ͷ�ļ�
#include <string.h>  	// ����strerror����Ҫ��ͷ�ļ�
#include "common.h"



//SPI��ʼ��
int SPI_Init(SPI_HANDLE *pHandle, const char *pSpiDeviceName)
{
	pHandle->fd = -1;

	if(pSpiDeviceName == NULL || pHandle==NULL || strlen(pSpiDeviceName)>SPI_DEVICE_NAME_MAX_LEN)
	{
		printf("%s(%d)Check the input parameters!\r\n",__FILE__ , __LINE__);
		return -1;
	}
	strcpy(pHandle->SpiDeviceName, pSpiDeviceName);					//��¼�����豸����

	//��SPI
	pHandle->fd = open(pSpiDeviceName, O_RDWR|O_NOCTTY|O_NDELAY);		//��д��ռ��ʽ��SPI
	if (pHandle->fd < 0)
	{
		//��ӡ������Ϣ
		printf("Can't Open SPI(%s) : %s(%d)\n",pSpiDeviceName, strerror(errno), errno);
		return errno;
	}
	else
	{
		printf("Open SPI OK!\r\n");

	}

	return 0;
}

