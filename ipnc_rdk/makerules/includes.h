#ifndef _INCLUDE_H
#define _INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>

#define debug

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
#define		TERMINAL_COLOR_TERRESET	"\033[0m"
#define		TERMINAL_COLOR_BLACK		"\033[30m" /* Black */
#define		TERMINAL_COLOR_RED			"\033[31m" /* Red */
#define		TERMINAL_COLOR_GREEN		"\033[32m" /* Green */
#define		TERMINAL_COLOR_YELLOW		"\033[33m" /* Yellow */
#define		TERMINAL_COLOR_BLUE		"\033[34m" /* Blue */
#define		TERMINAL_COLOR_MAGENTA	"\033[35m" /* Magenta */
#define		TERMINAL_COLOR_CYAN		"\033[36m" /* Cyan */
#define		TERMINAL_COLOR_WHITE		"\033[37m" /* White */
#define		TERMINAL_COLOR_BOLDBLACK	"\033[1m\033[30m" /* Bold Black */
#define		TERMINAL_COLOR_BOLDRED	"\033[1m\033[31m" /* Bold Red */
#define		TERMINAL_COLOR_BOLDGREEN	"\033[1m\033[32m" /* Bold Green */
#define		TERMINAL_COLOR_BOLDYELLOW	 "\033[1m\033[33m" /* Bold Yellow */
#define		TERMINAL_COLOR_BOLDBLUE	"\033[1m\033[34m" /* Bold Blue */
#define		TERMINAL_COLOR_BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define		TERMINAL_COLOR_BOLDCYAN	"\033[1m\033[36m" /* Bold Cyan */
#define		TERMINAL_COLOR_BOLDWHITE	"\033[1m\033[37m" /* Bold White */

typedef enum {
	OSA_SOK = 0,
	OSA_EFAIL = -1,
	OSA_EPERM = -EPERM,  /* Operation not permitted */ 
	OSA_ENOENT = -ENOENT, /* No such file or directory */ 
	OSA_EIO	= -EIO,			/* I/O error */ 
	OSA_ENOMEM = -ENOMEM,  /* Out of memory */ 
	OSA_EBUSY = -EBUSY,  /* Device or resource busy */ 
	OSA_ENODEV = -ENODEV,  /* No such device */ 
	OSA_EINVAL = -EINVAL,  /* Invalid argument */ 
	OSA_ENOSPC = -ENOSPC,  /* No space left on device */ 
	OSA_ENOMEDIUM = -ENOMEDIUM, /* No medium found */
	OSA_EMEDIUMTYPE = -EMEDIUMTYPE,          /* Wrong medium type */
	OSA_EALREADY = -EALREADY,        /* Operation already in progress */
	OSA_ETIMEDOUT = -ETIMEDOUT,            /* Connection timed out */
	OSA_ENETUNREACH = -ENETUNREACH,     /* Network is unreachable */
}OSA_StatusTypeDef;


#define msleep(ms) usleep(1000*ms)




#endif

