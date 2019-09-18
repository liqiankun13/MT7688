/*
 * SPI.h
 *
 *  Created on: 2018��8��2��
 *      Author: cfan
 */

#ifndef HARDWARE_SPI_H_
#define HARDWARE_SPI_H_
#include "termios.h"
#include "common.h"


#define SPI_DEVICE_NAME_MAX_LEN	35	//SPI������󳤶�

//SPI�ӿھ��
typedef struct
{
	int fd;
	char SpiDeviceName[SPI_DEVICE_NAME_MAX_LEN+1];		//SPI����
}SPI_HANDLE;



//SPI��ʼ��
int SPI_Init(SPI_HANDLE *pHandle, const char *pSpiDeviceName);


#endif /* HARDWARE_SPI_H_ */
