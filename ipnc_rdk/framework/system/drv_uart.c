#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include<fcntl.h>
#include<drv_uart.h>

/*
	*************************************************************
	@Decr:打开串口
	@Para1: 设备路径
	@Ret:-1=失败,其他= 文件描述符
	*************************************************************
*/

int open_port(const char *dev)
{
	 int fd;
     /*  O_RDWR 读写方式打开；
          O_NOCTTY 不允许进程管理串口（不太理解，一般都选上）；
          O_NDELAY 非阻塞（默认为阻塞，打开后也可以使用fcntl()重新设置）*/
	 if((fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	 {
		  perror("open serial port");

		  return -1;
	 }


	/*	fcntl(fd, F_SETFL, 0) //阻塞状态
		fcntl(fd, F_SETFL, FNDELAY)//非阻塞	
	*/
	 if(fcntl(fd, F_SETFL, FNDELAY) < 0)/*回复串口为阻塞状态*/
	 {
		 perror("fcntl F_SETFL");
		 return -1;
	 }
	 if(isatty(fd) == 0)/*测试打开是否为设备*/
	 {
	
	 	//perror("isatty");
	 }
	 return fd;
}


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if (tcgetattr( fd,&oldtio)  !=  0) //tcgetattr(fd,&options); //获取当前设置
    { 
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );//
    
    newtio.c_cflag  |=  CLOCAL | CREAD; /* 将本地模式(CLOCAL)和串行数据接收(CREAD)设置为有效 */
    newtio.c_cflag &= ~CSIZE; /* 关闭字体设置，下面设置数据位之前必须关闭这项 */

    switch(nBits)//数据位
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }
    switch( nEvent )
    {
    case 'O':                     //奇校验
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':                     //偶校验
        newtio.c_iflag |= (INPCK | ISTRIP); // 
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':                    //无校验
        newtio.c_cflag &= ~PARENB;
        break;
    }

switch( nSpeed ) //收发波特率
    {
     case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 19200:
		cfsetispeed(&newtio, B19200);
		cfsetospeed(&newtio, B19200);
		break;
	case 38400:
		cfsetispeed(&newtio, B38400);
		cfsetospeed(&newtio, B38400);
		break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
switch(nStop) //停止位
	{
	case 2:
		newtio.c_cflag |= CSTOPB;
		break;
	default:
		newtio.c_cflag &= ~CSTOPB;
		break;
	}

	/* 发送字符0X0d的时候，往往接收端得到的字符是0X0a，
	    原因是因为在串口设置中c_iflag和c_oflag中存在从NL-CR和
	    CR-NL的映射，即串口能把回车和换行当成同一个字符*/
	newtio.c_iflag &= ~ (INLCR | ICRNL | IGNCR);
	newtio.c_oflag &= ~(ONLCR | OCRNL);
    newtio.c_cc[VTIME]  = 2;//VTIME ：非 canonical 模式读时的延时，以十分之一秒为单位。 
    newtio.c_cc[VMIN] = 0;// 最小字节数 （读）"这个只能在阻塞条件下有效"
    tcflush(fd,TCIFLUSH); //TCIFLUSH :刷清输入队列;TCOFLUSH:刷清输出队列;TCIOFLUSH :刷清输入、输出队列
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)// // TCSANOW立即生效；
    {
        perror("com set error");
        return -1;
    }
    printf(("set done\n"));
    return 0;
}

