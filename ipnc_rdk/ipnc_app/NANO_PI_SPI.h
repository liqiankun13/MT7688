/*
 * NANO_PI_SPI.h
 *
 *  Created on: 2018��8��2��
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


int setSPIWriteBitsPerWord(int spi_fd, int bits);		//����ÿ�ζ�SPI�豸���ֳ�,��λ�Ǳ���.
int setSPIReadBitsPerWord(int spi_fd, int bits);		//����ÿ��дSPI�豸���ֳ�����λ�Ǳ���
int setSPIBitOrder(int spi_fd, int order);				//�豸SPI����ʱ���ȴ���ͱ���λ���Ǹ߱���λ
int setSPIMaxSpeed(int spi_fd, unsigned int spi_speed);	//�豸SPI�����ٶ�
int setSPIDataMode(int spi_fd, int mode);				//����SPI�豸��ģʽ
//ͬʱ���������һ���ֽڵ�����
int SPItransferOneByte(int spi_fd , unsigned char byteData, int spi_delay, int spi_speed, int spi_bits);
//ͬʱ��������ն���ֽڵ�����
int SPItransferBytes(int spi_fd, unsigned char * writeData, int writeLen, unsigned char * readBuffer, int readLen, int spi_delay,
		int spi_speed, int spi_bits);
//д����ֽڵ����ݵ�SPI�� �� 
int writeBytesToSPI(int spi_fd, unsigned char * writeData, int writeLen, int spi_delay, int spi_speed, int spi_bits);
//��SPI�豸��ȡ����ֽ�
int readBytesFromSPI(int spi_fd, unsigned char * readBuffer, int readLen, int spi_delay, int spi_speed, int spi_bits);


#endif /* NANO_PI_SPI_H_ */
