/*
 * NANO_PI_SPI.h
 *
 *  Created on: 2018年8月2日
 *      Author: cfan
 */

#ifndef NANO_PI_SPI_H_
#define NANO_PI_SPI_H_
#include "termios.h"
#include "common.h"

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3
#define MSBFIRST 0
#define LSBFIRST 1

#define clearLastError()  //


int setSPIWriteBitsPerWord(int spi_fd, int bits);		//设置每次读SPI设备的字长,单位是比特.
int setSPIReadBitsPerWord(int spi_fd, int bits);		//设置每次写SPI设备的字长，单位是比特
int setSPIBitOrder(int spi_fd, int order);				//设备SPI传输时是先传输低比特位还是高比特位
int setSPIMaxSpeed(int spi_fd, unsigned int spi_speed);	//设备SPI传输速度
int setSPIDataMode(int spi_fd, int mode);				//设置SPI设备的模式
//同时发送与接收一个字节的数据
int SPItransferOneByte(int spi_fd , unsigned char byteData, int spi_delay, int spi_speed, int spi_bits);
//同时发送与接收多个字节的数据
int SPItransferBytes(int spi_fd, unsigned char * writeData, int writeLen, unsigned char * readBuffer, int readLen, int spi_delay,
		int spi_speed, int spi_bits);
//写多个字节的数据到SPI设 备 
int writeBytesToSPI(int spi_fd, unsigned char * writeData, int writeLen, int spi_delay, int spi_speed, int spi_bits);
//从SPI设备读取多个字节
int readBytesFromSPI(int spi_fd, unsigned char * readBuffer, int readLen, int spi_delay, int spi_speed, int spi_bits);


#endif /* NANO_PI_SPI_H_ */
