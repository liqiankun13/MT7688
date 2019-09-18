

#include <sys/ioctl.h>
#include "NANO_PI_SPI.h"
#include <errno.h>
#include <stdio.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

//������ٷ��ṩ��SPI�����ӿ�


#define SPI_MAX_SPEED 25000000		//���ʱ���ٶ�


/*************************************************************************************************************************
*����        			:	int setSPIWriteBitsPerWord(int spi_fd, int bits)
*����        			:	����ÿ�ζ�SPI�豸���ֳ�,��λ�Ǳ���.
*����        			:	spi_fd: SPI�豸���ļ�������;bits: �ֳ�����λ�Ǳ���
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:	��Ȼ�󲿷�SPI�ӿڵ��ֳ���8����16����Ȼ����һЩ��������ӡ�
        			��Ҫ˵�����ǣ���������ԱΪ��Ļ���Ĭ��ʹ��8��Ϊ�ֳ�(ioctl SPI_IOC_WR_BITS_PER_WORD)
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
*����        			:	int setSPIReadBitsPerWord(int spi_fd, int bits)
*����        			:	����ÿ��дSPI�豸���ֳ�����λ�Ǳ���
*����        			:	spi_fd: SPI�豸���ļ�������;bits: �ֳ�����λ�Ǳ���
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:	��Ȼ�󲿷�SPI�ӿڵ��ֳ���8����16����Ȼ����һЩ��������ӡ�
        			��Ҫ˵�����ǣ���������ԱΪ��Ļ���Ĭ��ʹ��8��Ϊ�ֳ�(ioctl SPI_IOC_WR_BITS_PER_WORD)
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
*����        			:	int setSPIBitOrder(int spi_fd, int order)
*����        			:	�豸SPI����ʱ���ȴ���ͱ���λ���Ǹ߱���λ
*����        			:	spi_fd: SPI�豸���ļ�������;order: ��SPIEnum.MSBFIRST��SPIEnum.LSBFIRST
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:	��ѡ�Ĳ�����SPIEnum.MSBFIRST��SPIEnum.LSBFIRST
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
*����        			:	int setSPIMaxSpeed(int spi_fd, unsigned int spi_speed)
*����        			:	�豸SPI�����ٶ�
*����        			:	spi_fd: SPI�豸���ļ�������;spi_speed: �ٶȣ���Ƶ��ԽС�ٶ�Խ�ߣ�
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:
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
*����        			:	int setSPIDataMode(int spi_fd, int mode)
*����        			:	����SPI�豸��ģʽ
*����        			:	spi_fd: SPI�豸���ļ�������;mode: SPI�豸��ģʽ���ɴ���SPI_MODE0 ~ SPI_MODE3
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:
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
*����        			:	int SPItransferOneByte(int spi_fd, unsigned char byteData, int spi_delay, int spi_speed, int spi_bits)
*����        			:	ͬʱ���������һ���ֽڵ�����
*����        			:	spi_fd: SPI�豸���ļ�������;byteData:Ҫд��SPI�豸������;spi_delay:��ʱ;spi_speed:�����ٶ�;spi_bits:�ֳ�����λ�Ǳ���
*����        			:	�ɹ����ض��������ݣ�ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:
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
*����        			:	int SPItransferBytes(int spi_fd, unsigned char * writeData, int writeLen, unsigned char * readBuffer,
*����        					int readLen, int spi_delay, int spi_speed, int spi_bits)
*����        			:	ͬʱ��������ն���ֽڵ�����
*����        			:	spi_fd: SPI�豸���ļ�������;writeData:Ҫд�������;readBuff: ��Ŷ�ȡ���ݵĻ�����;spi_delay:��ʱ;spi_speed:�����ٶ�;
        			spi_bits:�ֳ�����λ�Ǳ���
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:
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
*����        			:	int writeBytesToSPI(int spi_fd, unsigned char * writeData, int writeLen, int spi_delay, int spi_speed,
*����        					int spi_bits)
*����        			:	д����ֽڵ����ݵ�SPI�� ��
*����        			:	spi_fd: SPI�豸���ļ�������;writeData:Ҫд�������;spi_delay:��ʱ;spi_speed:�����ٶ�;spi_bits:�ֳ�����λ�Ǳ���
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:
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
*����        			:	int readBytesFromSPI(int spi_fd, unsigned char * readBuffer, int readLen, int spi_delay, int spi_speed,
*����        					int spi_bits)
*����        			:	��SPI�豸��ȡ����ֽ�
*����        			:	spi_fd: SPI�豸���ļ�������;readBuff: ��Ŷ�ȡ���ݵĻ�����;readLen:��ȡ���ȣ����ܳ�����������С��
       				spi_delay:��ʱ;spi_speed:�����ٶ�;spi_bits:�ֳ�����λ�Ǳ���
*����        			:	�ɹ�����0��ʧ�ܷ��ظ���
*����        			:	��
*����        			:	Friendly NanoPI-NEO(cp1300@139.com����)
*ʱ��        			:	2018-08-12
*����޸�ʱ��	:	2018-08-12
*˵��        			:
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

