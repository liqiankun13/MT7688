/*
 * SPI.h
 *
 *  Created on: 2018年8月2日
 *      Author: cfan
 */

#ifndef HARDWARE_SPI_H_
#define HARDWARE_SPI_H_
#include "termios.h"
#include "common.h"


#define SPI_DEVICE_NAME_MAX_LEN	35	//SPI名称最大长度

//SPI接口句柄
typedef struct
{
	int fd;
	char SpiDeviceName[SPI_DEVICE_NAME_MAX_LEN+1];		//SPI名称
}SPI_HANDLE;



//SPI初始化
int SPI_Init(SPI_HANDLE *pHandle, const char *pSpiDeviceName);


#endif /* HARDWARE_SPI_H_ */
