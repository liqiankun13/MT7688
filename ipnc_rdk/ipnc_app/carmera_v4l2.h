
#ifndef  _CAMERALOPT_H

#define  _CAMERALOPT_H

 

#include <stdio.h>

#include <linux/videodev2.h>

#include <fcntl.h>

#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

 

#define   COUNT  3

#define    WIDTH   720
#define    HIGHT   560



//初始化摄像头

int Init_Cameral(const char *dev);


int Exit_Cameral(void); //退出摄像头

//摄像头开始采集

int Start_Cameral(void);

int Stop_Cameral(void);//停止摄像头

//获取摄像头的数据

int save_Picture(int (*callBack)( const char*, char*,  size_t), const char *pathName);



 

#endif  //_CAMERALOPT_H
