

#include <sys/ioctl.h>
#include "NANO_PI_SPI.h"
#include <errno.h>
#include <stdio.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

//开发板官方提供的SPI驱动接口


#define SPI_MAX_SPEED 25000000		//最大时钟速度


/*************************************************************************************************************************
*函数        			:	int setSPIWriteBitsPerWord(int spi_fd, int bits)
*功能        			:	设置每次读SPI设备的字长,单位是比特.
*参数        			:	spi_fd: SPI设备的文件描述符;bits: 字长，单位是比特
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:	虽然大部分SPI接口的字长是8或者16，仍然会有一些特殊的例子。
        			需要说明的是，如果这个成员为零的话，默认使用8作为字长(ioctl SPI_IOC_WR_BITS_PER_WORD)
*************************************************************************************************************************/
int setSPIWriteBitsPerWord(int spi_fd, int bits)
{
    clearLastError();
	int ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_WR_BITS_PER_WORD");
	}
	return ret;
}


/*************************************************************************************************************************
*函数        			:	int setSPIReadBitsPerWord(int spi_fd, int bits)
*功能        			:	设置每次写SPI设备的字长，单位是比特
*参数        			:	spi_fd: SPI设备的文件描述符;bits: 字长，单位是比特
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:	虽然大部分SPI接口的字长是8或者16，仍然会有一些特殊的例子。
        			需要说明的是，如果这个成员为零的话，默认使用8作为字长(ioctl SPI_IOC_WR_BITS_PER_WORD)
*************************************************************************************************************************/
int setSPIReadBitsPerWord(int spi_fd, int bits)
{
	int ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    clearLastError();
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_RD_BITS_PER_WORD\r\n");
	}
	return ret;
}


/*************************************************************************************************************************
*函数        			:	int setSPIBitOrder(int spi_fd, int order)
*功能        			:	设备SPI传输时是先传输低比特位还是高比特位
*参数        			:	spi_fd: SPI设备的文件描述符;order: 传SPIEnum.MSBFIRST或SPIEnum.LSBFIRST
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:	可选的参数有SPIEnum.MSBFIRST和SPIEnum.LSBFIRST
*************************************************************************************************************************/
int setSPIBitOrder(int spi_fd, int order)
{
	int ret;
	int spi_mode = 0;
    clearLastError();
	if(order == LSBFIRST) {
		spi_mode |=  SPI_LSB_FIRST;
	} else {
		spi_mode &= ~SPI_LSB_FIRST;
	}
	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_WR_MODE");
		return ret;
	}
	return ret;
}


/*************************************************************************************************************************
*函数        			:	int setSPIMaxSpeed(int spi_fd, unsigned int spi_speed)
*功能        			:	设备SPI传输速度
*参数        			:	spi_fd: SPI设备的文件描述符;spi_speed: 速度（分频，越小速度越高）
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:
*************************************************************************************************************************/
int setSPIMaxSpeed(int spi_fd, unsigned int spi_speed)
{
	int ret;
	unsigned int realSpeed;
    clearLastError();
    if (spi_speed<0 || spi_speed>SPI_MAX_SPEED) {
        printf("invalid spi speed %d", spi_speed);
    }
	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_WR_MAX_SPEED_HZ");
		return ret;
	}
	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &realSpeed);
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_RD_MAX_SPEED_HZ");
		return ret;
	}
	if(realSpeed != spi_speed)
	{
		printf("spi real speed %d\r\n", realSpeed);
	}
	return ret;
}

/*************************************************************************************************************************
*函数        			:	int setSPIDataMode(int spi_fd, int mode)
*功能        			:	设置SPI设备的模式
*参数        			:	spi_fd: SPI设备的文件描述符;mode: SPI设备的模式，可传入SPI_MODE0 ~ SPI_MODE3
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:
*************************************************************************************************************************/
int setSPIDataMode(int spi_fd, int mode)
{
	int ret;
	int spi_mode = 0;
    clearLastError();
	switch(mode) {
		case SPI_MODE0:
			spi_mode &= ~(SPI_CPHA|SPI_CPOL);
			break;
		case SPI_MODE1:
			spi_mode &= ~(SPI_CPOL);
			spi_mode |= (SPI_CPHA);
			break;
		case SPI_MODE2:
			spi_mode |= (SPI_CPOL);
			spi_mode &= ~(SPI_CPHA);
			break;
		case SPI_MODE3:
			spi_mode |= (SPI_CPHA|SPI_CPOL);
			break;
		default:
			printf("error SPIDataMode");
			return -1;
	}

	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_WR_MODE");
		return ret;
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &mode);
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_RD_MODE");
		return ret;
	}

	return ret;
}



/*************************************************************************************************************************
*函数        			:	int SPItransferOneByte(int spi_fd, unsigned char byteData, int spi_delay, int spi_speed, int spi_bits)
*功能        			:	同时发送与接收一个字节的数据
*参数        			:	spi_fd: SPI设备的文件描述符;byteData:要写入SPI设备的数据;spi_delay:延时;spi_speed:传输速度;spi_bits:字长，单位是比特
*返回        			:	成功返回读到的数据，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:
*************************************************************************************************************************/
int SPItransferOneByte(int spi_fd , unsigned char byteData, int spi_delay, int spi_speed, int spi_bits)
{
	int ret;
	unsigned char tx[1] = {0};
	unsigned char rx[1] = {0};
	tx[0] = byteData;

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx;
	tr.len = 1;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

    clearLastError();
	ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_MESSAGE");
		return ret;
	}
	return rx[0];
}


/*************************************************************************************************************************
*函数        			:	int SPItransferBytes(int spi_fd, unsigned char * writeData, int writeLen, unsigned char * readBuffer,
*函数        					int readLen, int spi_delay, int spi_speed, int spi_bits)
*功能        			:	同时发送与接收多个字节的数据
*参数        			:	spi_fd: SPI设备的文件描述符;writeData:要写入的数据;readBuff: 存放读取数据的缓冲区;spi_delay:延时;spi_speed:传输速度;
        			spi_bits:字长，单位是比特
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:
*************************************************************************************************************************/
int SPItransferBytes(int spi_fd, unsigned char * writeData, int writeLen, unsigned char * readBuffer, int readLen, int spi_delay,
		int spi_speed, int spi_bits)
{
	unsigned int len = writeLen;
	if (len > readLen) {
		len = readLen;
	}

	unsigned char * pWriteData = writeData;
	unsigned char * pReadBuffer = readBuffer;

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)pWriteData;
	tr.rx_buf = (unsigned long)pReadBuffer;
	tr.len = len;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

	int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);

    clearLastError();
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_MESSAGE [%d]\r\n", errno);
	}
	return ret;
}


/*************************************************************************************************************************
*函数        			:	int writeBytesToSPI(int spi_fd, unsigned char * writeData, int writeLen, int spi_delay, int spi_speed,
*函数        					int spi_bits)
*功能        			:	写多个字节的数据到SPI设 备
*参数        			:	spi_fd: SPI设备的文件描述符;writeData:要写入的数据;spi_delay:延时;spi_speed:传输速度;spi_bits:字长，单位是比特
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:
*************************************************************************************************************************/
int writeBytesToSPI(int spi_fd, unsigned char * writeData, int writeLen, int spi_delay, int spi_speed, int spi_bits)
{
	int ret;	
	struct spi_ioc_transfer tr = {		
		.tx_buf = (unsigned long)writeData,		
			.rx_buf = (unsigned long)0,		
			.len = writeLen,		
			.delay_usecs = spi_delay,		
			.speed_hz = spi_speed,		
			.bits_per_word = spi_bits,	
	};	
	ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);	
	if (ret < 1)		
		printf("can't send spi message");

	
	return ret;
}

/*************************************************************************************************************************
*函数        			:	int readBytesFromSPI(int spi_fd, unsigned char * readBuffer, int readLen, int spi_delay, int spi_speed,
*函数        					int spi_bits)
*功能        			:	从SPI设备读取多个字节
*参数        			:	spi_fd: SPI设备的文件描述符;readBuff: 存放读取数据的缓冲区;readLen:读取长度（不能超过缓冲区大小）
       				spi_delay:延时;spi_speed:传输速度;spi_bits:字长，单位是比特
*返回        			:	成功返回0，失败返回负数
*依赖        			:	无
*作者        			:	Friendly NanoPI-NEO(cp1300@139.com整理)
*时间        			:	2018-08-12
*最后修改时间	:	2018-08-12
*说明        			:
*************************************************************************************************************************/
int readBytesFromSPI(int spi_fd, unsigned char * readBuffer, int readLen, int spi_delay, int spi_speed, int spi_bits)
{
	unsigned int len = readLen;

	unsigned char * pReadBuffer = readBuffer;

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)0;
	tr.rx_buf = (unsigned long)pReadBuffer;
	tr.len = len;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

	int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    clearLastError();
	if (ret < 0) {
		printf("Can't ioctl SPI_IOC_MESSAGE");
	}
	return ret;
}

