
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <display.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <includes.h>
#include <common.h>



#define fontmem_big_size 312*72*2
static uint16_t font_big_mem[fontmem_big_size];
#define fontmem_midlle_size 502*100*2
static uint16_t font_midlle_mem[fontmem_midlle_size];


static uint16_t  *pDisplayMem = NULL;
static int fd = -1;

int Init_Dislay()
{
	char buff[32];
	struct fb_var_screeninfo fb_var;

	/* big font */
	fd = open("/root/res/images/font_12x24.bmp",O_RDONLY);
	if(fd<0)
	{		
		printf("load font font_12x24.bmp failed\n");		
		return -1;	
	}	
	read(fd,font_big_mem,72);	
	read(fd,font_big_mem,fontmem_big_size);	
	close(fd);
	/* big font */
	fd = open("/root/res/images/font_50x100.bmp",O_RDONLY);
	if(fd<0)
	{		
		printf("load font font_50x100.bmp failed\n");		
		return -1;	
	}	
	read(fd,font_midlle_mem,72);	
	read(fd,font_midlle_mem,fontmem_midlle_size);	
	close(fd);

	
	fd = open("/dev/fb0",O_RDWR);
	if(fd < 0){
		LOG_ERR("open file error\n");
		return -1;
	}

	if(-1 == ioctl(fd,FBIOGET_VSCREENINFO,&fb_var)){
		LOG_ERR("ioctl FBIOGET_FSCREENINFI fail \n");
		return -1;
	}
	LOG_DBG("%d*%d %dbpp\r\n", fb_var.xres, fb_var.yres, fb_var.bits_per_pixel);
	pDisplayMem = (uint16_t *)mmap(NULL,_LCD_WIDTH*_LCD_HIGHT*_LCD_BPP/8,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(pDisplayMem == NULL)
	{
		close(fd);
		fd = -1;
		LOG_ERR("framebuf mmap fail \r\n");
		return -1;
	}

}

void Exit_Dislay()
{
	if(pDisplayMem != NULL)
	{
		memset(pDisplayMem, _LCD_WIDTH*_LCD_HIGHT*_LCD_BPP/8, 0);
		munmap(pDisplayMem, _LCD_WIDTH*_LCD_HIGHT*_LCD_BPP/8);
	}
	if(fd > 0 )
		close(fd);
}



void  GUICleanScreen(uint16_t x, uint16_t y, uint16_t w,uint16_t h)
{
	uint16_t i,j;	
	for(i=0;i<h;i++)
	{		
		for(j=0;j<w;j++)
		{			
			pDisplayMem[i*_LCD_HIGHT+j+x+y*_LCD_HIGHT] = 0;				
		}	
	}
}
void  GUIFullScreen(uint16_t x, uint16_t y, uint16_t w,uint16_t h, uint16_t corlor)
{
	uint16_t i,j;	
	for(i=0;i<h;i++)
	{		
		for(j=0;j<w;j++)
		{			
			pDisplayMem[i*_LCD_HIGHT+j+x+y*_LCD_HIGHT] = corlor;				
		}	
	}
}

void GUIDrawText(uint16_t x, uint16_t y, const char *text, FontSizeTypeDef fontSize, CorlorTypeDef fontCorlor, CorlorTypeDef BacKCorlor)
{

	uint16_t len;
	uint16_t font_posx,font_posy,font_charw,font_charh, columnSpace, font_BMP_width;
	uint16_t i,j, k= 0;
	uint16_t *pFontAddr;
	uint16_t rgb656 = 0x0000, bk_rgb656;
	switch(fontSize)
	{
		case LCD_FONT_BIG:
			font_charw = 12;font_charh = 24;pFontAddr=font_big_mem;
			font_BMP_width = 312;
			break;
		case LCD_FONT_MIDLLE:
			font_charw = 50;font_charh = 100;pFontAddr=font_midlle_mem;
			font_BMP_width = 502;
			break;	
		case LCD_FONT_SMALL:
		default:
			font_charw = 18;font_charh = 11;pFontAddr=font_midlle_mem;
			break;	
	}
	switch(fontCorlor)
	{
		case LCD_FILL_WHITE:
			rgb656 = RGB(255,255,255);
			break;
		case LCD_FILL_YELLOW:

			break;
		case LCD_FILL_RED:
			rgb656 = RGB(239,131,123);
			break;
		case LCD_FILL_GREEN:
			break;
		default:
			break;
	}
	switch(BacKCorlor)
	{
		case LCD_FILL_WHITE:
			bk_rgb656 = RGB(255,255,255);
			break;
		case LCD_FILL_YELLOW:

			break;
		case LCD_FILL_RED:
			break;
		case LCD_FILL_GREEN:
			bk_rgb656 = RGB(41,40,25);
			break;
		case LCD_FILL_BLUE:
			bk_rgb656 = RGB(255,255,255);
			break;
		case LCD_FILL_BLACK:
		default:
			bk_rgb656 = RGB(0,0,0);
			break;
	}
	char *pChar = text;
	for(; pChar != 0; pChar ++ )
	{	
		if(*pChar>='a' && *pChar<='z'){
			font_posx = (*pChar-'a')*font_charw;
			font_posy = font_charh*0;
		}else if(*pChar>='A' && *pChar<='Z'){
			font_posx = (*pChar-'A')*font_charw;
			font_posy = font_charh*1;
		}else if(*pChar>='0' && *pChar<='9'){
			font_posx = (*pChar-'0')*font_charw;
			font_posy = font_charh*2;
		}else if(*pChar=='.'){
			font_posx = 10*font_charw;font_posy = font_charh*2;
		}else if(*pChar==':'){
			font_posx = 11*font_charw+2;font_posy = font_charh*2;
		}else if(*pChar=='-'){
			font_posx = 12*font_charw;font_posy = font_charh*2;
		}else if(*pChar=='_'){
			font_posx = 13*font_charw;font_posy = font_charh*2;
		}else if(*pChar=='/'){
			font_posx = 14*font_charw;font_posy = font_charh*2;
		}else if(*pChar=='?'){
			font_posx = 15*font_charw;font_posy = font_charh*2;
		}else if(*pChar=='!'){
			font_posx = 16*font_charw;font_posy = font_charh*2;
		}else if(*pChar==' '){
			font_posx = 17*font_charw;font_posy = font_charh*2;
		}else if(*pChar=='/'){
			font_posx = 18*font_charw;font_posy = font_charh*2;
		}else if(*pChar=='@'){
			font_posx = 25*font_charw;font_posy = font_charh*2;
		}else if(*pChar=='#'){
			font_posx = 24*font_charw;font_posy = font_charh*2;
		}
		else
			return ;
		if(LCD_FONT_MIDLLE == fontSize)
		{
			font_posx = (*pChar-'0')*font_charw;
			font_posy = font_charh*0;

		}
		for(i=0;i<font_charh;i++)
		{
			for(j=0;j<font_charw;j++)
			{
				if(LCD_FONT_MIDLLE == fontSize)
				{
					pDisplayMem[(x+y*_LCD_WIDTH+j+i*_LCD_WIDTH+k *font_charw)] = pFontAddr[(font_posx+font_posy*font_BMP_width+j+i*font_BMP_width)];
					continue;
				}
				uint16_t pixel;
				pixel = pFontAddr[(font_posx+font_posy*font_BMP_width+j+i*font_BMP_width)];
				if(pixel == 0xffff)
				{
					pDisplayMem[(x+y*_LCD_WIDTH+j+i*_LCD_WIDTH+k *font_charw)] = rgb656;
				}
				else//下面不运行就是透明
					{
					pDisplayMem[(x+y*_LCD_WIDTH+j+i*_LCD_WIDTH+k *font_charw)] = bk_rgb656;	
				}
				
				
			}
		}
		k ++;
	}
	
	
	
}

void GUIFullScreenImageDraw(const char *path)
{
	int img_fd;	
	int ret=0;	
	char buf[66];	
	img_fd = open(path,O_RDONLY);	
	if(img_fd<0)
	{		
		LOG_ERR("open(%s) error!\n",path);		
		return 0;	
	}	
	read(img_fd,buf,72);	
	ret = read(img_fd,(uint8_t *)pDisplayMem,_LCD_WIDTH*_LCD_HIGHT*2);	
	close(img_fd);	
	
}
/*图像长宽必须是偶数*/
int GUILoadBmp(char *pathname,uint16_t w,uint16_t h,unsigned char *rect)
{
	int img_fd;
	int ret=0;
	img_fd = open(pathname,O_RDONLY);
	if(img_fd<0){
		LOG_ERR("open(%s) error! %s\n",pathname, strerror(errno));
		return 0;
	}
	read(img_fd,rect,72);
	ret = read(img_fd,rect,w*h*2);
	close(img_fd);
	return ret;
}
void GUIImageDraw(uint16_t x,uint16_t y,uint16_t w,uint16_t h, uint16_t *rect)
{
	int i,j;

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			pDisplayMem[i*_LCD_WIDTH+j+x + y*_LCD_WIDTH] = rect[i*w+j];
			
		}
	}
}


