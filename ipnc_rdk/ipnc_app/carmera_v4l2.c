
#include "carmera_v4l2.h"
#include <includes.h>
#include <sysCfg.h>

struct buffer {
        void *                  start;
        size_t                  length;
};

int video_fd = -1; 

int length ; 

char *yuv[COUNT] ; 

struct v4l2_buffer  enqueue  , dequeue ;  //å®šä¹‰å‡ºå…¥é˜Ÿçš„æ“ä½œç»“æž„ä½“æˆå‘˜

struct buffer *buffers;


int Init_Cameral(const char *dev)
{
	int i ; 
	int ret ; 

	video_fd = open(dev , O_RDWR);
	if(0 > video_fd )
	{
		setSysState(sys_dev_Camera,SYS_DEV_Camera_Disconnect);
		perror("open video device fail");
		return -1 ; 
	}

	struct v4l2_capability cap;
	
	ioctl(video_fd,VIDIOC_QUERYCAP, &cap);
	
	printf("DriverName:%s\nCard Name:%s\nBus info:%s\nDriverVersion:%u.%u.%u\nCapabilities:0x%x%\n",cap.driver,cap.card,cap.bus_info,(cap.version>>16)&0XFF,(cap.version>>8)&0XFF,cap.version&0xFF, \
		cap.capabilities);
	struct v4l2_format format;
	
	struct v4l2_fmtdesc formatdesc;
	
	formatdesc.index=0;
	
	formatdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	printf("Supportformat:\n");
	
	while(ioctl(video_fd,VIDIOC_ENUM_FMT,&formatdesc)!=-1)
	{
		printf("\t%d.%s\n",formatdesc.index+1,formatdesc.description);
		formatdesc.index++;
	}

	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	format.fmt.pix.width  = WIDTH; 
	format.fmt.pix.height = HIGHT; 
	format.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG ;  

	ret = ioctl(video_fd , VIDIOC_S_FMT , &format);
	if(ret != 0)
	{
		perror("set video format fail\r\n");
		return -2 ; 

	}
	
	memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(video_fd, VIDIOC_G_FMT, &format);
    if(ret < 0)
    {
        printf("VIDIOC_G_FMT fail\n");
        return -1;
    }
    printf("-----------------VIDIOC_G_FMT----------------------\n");
    printf("width:%d   \nheight:%d   \ntype:%x   pixelformat:%c%c%c%c\n",format.fmt.pix.width,format.fmt.pix.height,
        format.type,format.fmt.pix.pixelformat&0xff,(format.fmt.pix.pixelformat>>8)&0xff,(format.fmt.pix.pixelformat>>16)&0xff,
        (format.fmt.pix.pixelformat>>24)&0xff);


	struct v4l2_requestbuffers  requestbuffer ; 
	requestbuffer.count = COUNT ; 
	requestbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	requestbuffer.memory = V4L2_MEMORY_MMAP ; 
	ret = ioctl(video_fd , VIDIOC_REQBUFS , &requestbuffer);
	if(ret != 0)
	{
		perror("request buffer fail\r\n");
		return -3  ;
	}

	//querybuffer

	buffers = calloc (requestbuffer.count, sizeof (*buffers));
	struct v4l2_buffer buf; 

	  
	for (i = 0; i < requestbuffer.count; i ++)
	{ 
			   /*»ñÈ¡Í¼Ïñ»º³åÇøµÄÐÅÏ¢*/
			buf.type 	   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory	   = V4L2_MEMORY_MMAP;
			buf.index	   = i;
	 
			 ioctl (video_fd, VIDIOC_QUERYBUF, &buf); 
				 
			 buffers[i].length = buf.length; 
			   
			   // °ÑÄÚºË¿Õ¼äÖÐµÄÍ¼Ïñ»º³åÇøÓ³Éäµ½ÓÃ»§¿Õ¼ä
			  buffers[i].start = mmap (NULL ,	 //Í¨¹ýmmap½¨Á¢Ó³Éä¹ØÏµ
											buf.length,
											PROT_READ | PROT_WRITE ,
											MAP_SHARED ,
											video_fd,
											buf.m.offset);
	}
			
		 /*Í¼Ïñ»º³åÈë¶Ó*/ 
		   
	for (i = 0; i < requestbuffer.count; i ++)
	{
			buf.type 	   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory	   = V4L2_MEMORY_MMAP;
			buf.index	   = i; 
			ioctl(video_fd, VIDIOC_QBUF, &buf);
				   
	}
	
	return 0 ; 

}

 

int Exit_Cameral(void)
{
	int i ;
	
	if(0 > video_fd )
	{
		return -1 ; 
	}
	for(i = 0 ; i < COUNT ; i++)
		munmap(yuv+i , length);
	close(video_fd);
	return 0 ; 

}

 

int Start_Cameral(void)
{
	int ret ; 
	
	if(0 > video_fd )
	{
		return -1 ; 
	}
	int on = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(video_fd , VIDIOC_STREAMON , &on);
	if(ret != 0)
	{
		setSysState(sys_dev_Camera,SYS_DEV_Camera_Err);
		perror("start Cameral fail");
		return -1 ; 
	}
	setSysState(sys_dev_Camera,SYS_DEV_Camera_Normal);
	return 0 ; 

}

int Stop_Cameral(void)
{

	int ret ; 
	int off= 1 ; 
	if(0 > video_fd )
	{
		return -1 ; 
	}
	ret = ioctl(video_fd , VIDIOC_STREAMOFF, &off);
	if(ret != 0)
	{
		perror("stop Cameral fail");
		return -1 ; 
	}
	return 0 ;

}

#if 0
int save_Picture(int (*callBack)( const char*, char*,  size_t), const char *pathName)
{

	int ret = 0;
	struct v4l2_buffer buf;
	if(0 > video_fd )
	{
		return OSA_EFAIL; 
	}

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(video_fd , VIDIOC_DQBUF , &buf);
	if(ret != 0)
	{
		gSYS_cfg_para.report.errCode = OSA_EIO;
		perror("dequeue fail");
		return OSA_EFAIL; 
	}
//	memcpy(buffer , buffers[buf.index].start , buffers[buf.index].length);
	//saveJpeg("test.jpg",buffers[buf.index].start, buffers[buf.index].length);
	if(callBack)
		ret = callBack(pathName, buffers[buf.index].start, buffers[buf.index].length);
	ret = ioctl(video_fd , VIDIOC_QBUF , &buf);
	if(ret != 0)
	{
		gSYS_cfg_para.report.errCode = OSA_EIO;
		perror("enqueue fail");
		return -1 ; 
	}

	return ret; 
}
#else
int save_Picture(int (*callBack)( const char*, char*,  size_t), const char *pathName)
{

	int ret ;
	struct v4l2_buffer buf;
	int count = 2;
	while(count --)
	{
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(video_fd , VIDIOC_DQBUF , &buf);
		if(ret != 0)
		{
			perror("dequeue fail");
			return 0 ; 
		}
	//	memcpy(buffer , buffers[buf.index].start , buffers[buf.index].length);
		//saveJpeg("test.jpg",buffers[buf.index].start, buffers[buf.index].length);
		
		ret = ioctl(video_fd , VIDIOC_QBUF , &buf);
		if(ret != 0)
		{
			perror("enqueue fail");
			return 0 ; 
		}

	}
	if(callBack)
		callBack(pathName, buffers[buf.index].start, buffers[buf.index].length);
	return ret; 
}
#endif
