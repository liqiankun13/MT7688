#ifndef _INCLUDE_H
#define _INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <syscall.h>
#include <errno.h>
#include <pthread.h>

#include <sys/stat.h>
#include <sys/statfs.h>



#define  gettid() syscall(__NR_gettid)
#define  msleep(x) usleep(x*1000)//x should be less than 1000, or else use sleep

#ifdef DEBUG
#define LOG_INFO(format,...)  \
	do{ \
	fprintf(stderr,TERMINAL_COLOR_GREEN"[INFO:%s -> %s: %d] " format TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__, __LINE__, ##__VA_ARGS__ ); \
	}while(0);

#define LOG_DBG(format,...)  \
	do{ \
	fprintf(stderr, TERMINAL_COLOR_CYAN"[DEBUG:%s -> %s : %d] " format TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__, __LINE__, ##__VA_ARGS__ ); \
	}while(0);

#define LOG_WAR(var,...)	\
	do{	\
	fprintf(stderr, TERMINAL_COLOR_BOLDMAGENTA"[WARING:%s -> %s: %d] " var TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__,__LINE__, ##__VA_ARGS__);	\
	}while(0);

#define LOG_ERR(var,...) 	\
	do{	\
	fprintf(stderr, TERMINAL_COLOR_BOLDRED"[ERROR:%s -> %s: %d] " var TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__,__LINE__, ##__VA_ARGS__); \
	}while(0);

#else
#define LOG_INFO(format,...)  \
	do{ \
	fprintf(stderr,TERMINAL_COLOR_GREEN format TERMINAL_COLOR_TERRESET, ##__VA_ARGS__ ); \
	}while(0);

#define LOG_DBG(format,...)  

#define LOG_WAR(var,...)	\
	do{	\
	fprintf(stderr,TERMINAL_COLOR_BOLDMAGENTA var TERMINAL_COLOR_TERRESET, ##__VA_ARGS__);	\
	}while(0);

#define LOG_ERR(var,...) 	\
	do{	\
	fprintf(stderr,TERMINAL_COLOR_BOLDRED var TERMINAL_COLOR_TERRESET, ##__VA_ARGS__); \
	}while(0);

#endif


//the following are UBUNTU/LINUX ONLY terminal color codes.
#define		TERMINAL_COLOR_TERRESET		"\033[0m"
#define		TERMINAL_COLOR_BLACK		"\033[30m" /* Black */
#define		TERMINAL_COLOR_RED			"\033[31m" /* Red */
#define		TERMINAL_COLOR_GREEN		"\033[32m" /* Green */
#define		TERMINAL_COLOR_YELLOW		"\033[33m" /* Yellow */
#define		TERMINAL_COLOR_BLUE			"\033[34m" /* Blue */
#define		TERMINAL_COLOR_MAGENTA		"\033[35m" /* Magenta */
#define		TERMINAL_COLOR_CYAN			"\033[36m" /* Cyan */
#define		TERMINAL_COLOR_WHITE		"\033[37m" /* White */
#define		TERMINAL_COLOR_BOLDBLACK	"\033[1m\033[30m" /* Bold Black */
#define		TERMINAL_COLOR_BOLDRED		"\033[1m\033[31m" /* Bold Red */
#define		TERMINAL_COLOR_BOLDGREEN	"\033[1m\033[32m" /* Bold Green */
#define		TERMINAL_COLOR_BOLDYELLOW	"\033[1m\033[33m" /* Bold Yellow */
#define		TERMINAL_COLOR_BOLDBLUE		"\033[1m\033[34m" /* Bold Blue */
#define		TERMINAL_COLOR_BOLDMAGENTA	"\033[1m\033[35m" /* Bold Magenta */
#define		TERMINAL_COLOR_BOLDCYAN		"\033[1m\033[36m" /* Bold Cyan */
#define		TERMINAL_COLOR_BOLDWHITE	"\033[1m\033[37m" /* Bold White */









/***************************************视频接口****************************************************************/

typedef enum {
	DVR_INTERFACE_SDI = 0,
	DVR_INTERFACE_YPbPr = 1,
	DVR_INTERFACE_DVI = 2,
	DVR_INTERFACE_HDMI = 3,
	DVR_INTERFACE_VGA = 4,
	DVR_INTERFACE_CVBS = 5,
	DVR_INTERFACE_SVIDEO = 6,
	DVR_INTERFACE_UNKNOWN ,
} DVR_InterfaceType;


/***********************************视频分辨率******************************************************/

typedef enum {
	VIDEO_RES_HD_1080P,
	VIDEO_RES_HD_1080I,
	
	VIDEO_RES_HD_720P,
	
	/**********标清模拟********/
	VIDEO_RES_SD_PAL, //PAL
	VIDEO_RES_SD_NTSC, //NTSC

	/*其他*/
	VIDEO_RES_VGA,//640*480
	VIDEO_RES_SVGA,//800*600
	VIDEO_RES_XGA,//1024*768
	VIDEO_RES_SXGA,//1280*1024
	VIDEO_RES_WSXGA,//1440*900
	VIDEO_RES_WXGA_PLUS,//1600*900
	VIDEO_RES_WSXGA_PLUS,//1680*1050
	VIDEO_RES_UXGA,//1600*1200
	VIDEO_RES_WUXGA,//1920*1200
	VIDEO_RES_QXGA,//2048*1536
	VIDEO_RES_UNKOWN,
} DVR_VideoResolutionType;

typedef enum 
{
	//VIDEO_RES_1920X1200P60
	
	VIDEO_FPS_60HZ = 0,
	VIDEO_FPS_50HZ = 1,
	VIDEO_FPS_30HZ = 2,
	VIDEO_FPS_25HZ = 3,
	VIDEO_FPS_24HZ = 4,
	VIDEO_FPS_UNKOWN,
} DVR_VideoFPSType;

/***********************************音频采样率******************************************************/

typedef enum {
	//采样率+编码率
	AUDIO_SAMP_8K_32K,
	AUDIO_SAMP_16K_64K,
	AUDIO_SAMP_32K_96K,
	AUDIO_SAMP_44_1K_96K,
	AUDIO_SAMP_48K_128K,
	AUDIO_SAMP_UNKOWN,
} DVR_AudioSampleType;

/*************************************图片类型***************************************************/

typedef enum {
	IMAGE_TYPE_BMP = 0,
	IMAGE_TYPE_JPG,
	IMAGE_TYPE_DCIOM,
	IMAGE_TYPE_GIF,
	IMAGE_TYPE_PNG,
	IMAGE_TYPE_UNKOWN,
} DVR_ImageType;

/************************外围设备*************************/

typedef enum {
	DVR_UART_4800B,
	DVR_UART_9600B,
	DVR_UART_19200B,
	DVR_UART_38400,
	DVR_UART_115200B
}DVR_UartBaudRate;



#define CURRENT_LANGUAGE	gDVR_cfg_para.sysPara.language
//#define GENERAL_DVR



#endif

