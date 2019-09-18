/*
 * SPI.c
 *
 *  Created on: 2018年8月2日
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
#include <errno.h>  	// 包含errno所需要的头文件
#include <string.h>  	// 包含strerror所需要的头文件
#include "common.h"



//SPI初始化
int SPI_Init(SPI_HANDLE *pHandle, const char *pSpiDeviceName)
{
	pHandle->fd = -1;

	if(pSpiDeviceName == NULL || pHandle==NULL || strlen(pSpiDeviceName)>SPI_DEVICE_NAME_MAX_LEN)
	{
		printf("%s(%d)Check the input parameters!\r\n",__FILE__ , __LINE__);
		return -1;
	}
	strcpy(pHandle->SpiDeviceName, pSpiDeviceName);					//记录串口设备名称

	//打开SPI
	pHandle->fd = open(pSpiDeviceName, O_RDWR|O_NOCTTY|O_NDELAY);		//读写独占方式打开SPI
	if (pHandle->fd < 0)
	{
		//打印错误信息
		printf("Can't Open SPI(%s) : %s(%d)\n",pSpiDeviceName, strerror(errno), errno);
		return errno;
	}
	else
	{
		printf("Open SPI OK!\r\n");

	}

	return 0;
}

