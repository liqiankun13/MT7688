#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>



//常见数据定义
typedef signed short int16_t;
typedef signed char  int8_t;

typedef signed long  S32;
typedef signed short S16;
typedef signed char  S8;


typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */


typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef unsigned int  uint32_t;
typedef unsigned long  long uint64_t;

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef unsigned long  long u64;


typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */




typedef enum {
	False = 0,
	True = !False,
} Bool;


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(*a))
#endif

#define _LINUX_OS
#ifdef  _LINUX_OS
#define		RT_SUCCESS		(0)
#define		RT_FAILURE		(-1)
#define	    RT_NOSPACE      (-2)
#define     RT_NOESIST      (-3)
#define     RT_NOACESS      (-4)
#define     RT_INVILDPOINT  (-5)
#define     RT_BREAK        (-6)
#define     RT_EXITED       (-7) 
#define     RT_RUNING       (-8) 
#else
#define		RT_SUCCESS		(0)
#define		RT_FAILURE		(-1)
#define	    RT_NOSPACE      (-2)
#define     RT_NOESIST      (-3)
#define     RT_NOACESS      (-4)
#define     RT_INVILDPOINT  (-5)
#define     RT_BREAK        (-6)
#define     RT_EXIT         (-7) 

#endif 













#endif

