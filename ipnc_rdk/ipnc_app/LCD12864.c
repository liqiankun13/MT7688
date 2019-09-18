/*
 * LCD12864_Virtual.c
 * 虚拟LCD12864
 *  Created on: 2018年8月12日
 *      Author: cfan
 */
#include "LCD12864.h"
#include "common.h"
#include "ASCII_8x16.h"
#include <stdio.h>
#include <string.h>

/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_Init(LCD12864_HANDLE *pHandle)
* 功能			:	LCD12864显存模式初始化
* 参数			:	pHandle:句柄；
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120531
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_Init(LCD12864_HANDLE *pHandle)
{
	if(pHandle == NULL)
	{
		printf("LCD12864:ERROR *pHandle is NULL!\r\n");
		return;
	}
	memset(pHandle->pFramebuf, 0, 8*128);
}



/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_DrawPoint(LCD12864_HANDLE *pHandle, u8 x,u8 y)
* 功能			:	在显存里面指定位置画点
* 参数			:	pHandle:句柄；x:X坐标,0-127;y:y坐标,0-63
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120531
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_DrawPoint(LCD12864_HANDLE *pHandle, u8 x,u8 y)
{
	if(x > 127 || y > 63)
		return;
	pHandle->pFramebuf[y / 8*128 + x] |= (1 << (y % 8));
}




/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_ClearPoint(LCD12864_HANDLE *pHandle, u8 x,u8 y)
* 功能			:	擦除显存里面指定位置的点
* 参数			:	pHandle:句柄；x:X坐标,0-127;y:y坐标,0-63
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120531
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_ClearPoint(LCD12864_HANDLE *pHandle, u8 x, u8 y)
{
	if(x > 127 || y > 63)
		return;
	pHandle->pFramebuf[y / 8*128 + x] &= ~(1 << (y % 8));
}



/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_ClearAll(LCD12864_HANDLE *pHandle)
* 功能			:	清除全部显存
* 参数			:	pHandle:句柄；
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120531
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_ClearAll(LCD12864_HANDLE *pHandle)
{
	u8 i,j;

	for(i = 0;i < 8;i ++)
	{
		for(j = 0;j < 128;j ++)
		{
			pHandle->pFramebuf[i*128+j] = 0x00;
		}
	}
}


/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_ShowChar(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 CHAR,LCD12864_FONT_MODE FontMode)
* 功能			:	在指定位置显示一个指定大小的字符
* 参数			:	pHandle:句柄；x,y:显示开始坐标,p:汉子点阵缓冲区;FontMode:汉子显示模式,
* 返回			:	无
* 依赖			:	画点函数
* 作者			:	cp1300@139.com
* 时间			:	20120603
* 最后修改时间 	: 	2018-08-12
* 说明			:
*************************************************************************************************************************/
void LCD12864_GRAM_ShowChar(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 CHAR,LCD12864_FONT_MODE FontMode)
{
	u8 i,j;
	u8 FontSize = (u8)FontMode&0x0f;	//获取字体大小
	u8 *p;
	void (*DrawPoint)(LCD12864_HANDLE *pHandle, u8 i,u8 j);
	void (*ClearPoint)(LCD12864_HANDLE *pHandle, u8 i,u8 j);

	CHAR -= 32;
	if(CHAR > 95 - 1) //限制ASCII范围
		return;

	if(FontSize)
	{
		FontSize = 12;
		p = (u8 *)ASCII_8X12[CHAR];		//12号
	}
	else
	{
		FontSize = 16;
		p = (u8 *)ASCII_8X16[CHAR];		//16号
	}

	if(FontMode & 0x40)	//反显
	{
		DrawPoint = LCD12864_GRAM_ClearPoint;
		ClearPoint =  LCD12864_GRAM_DrawPoint;
	}
	else //正常模式
	{
		ClearPoint =  LCD12864_GRAM_ClearPoint;
		DrawPoint =  LCD12864_GRAM_DrawPoint;
	}

	if(FontMode & 0x80)	//叠加显示
	{
		for(j = 0;j < 8;j ++)
		{
		 	for(i = 0;i < 8;i ++)
			{
			 	if(*p & (1 << i))
					(*DrawPoint)(pHandle, x + j,y + i);
			}
			p ++;
		}
		for(j = 0;j < 8;j ++)
		{
		 	for(i = 0;i < FontSize - 8;i ++)
			{
				if(*p & (1 << i))
					(*DrawPoint)(pHandle, x + j,y + 8 + i);
			}
			p ++;
		}
	}
	else	//非叠加显示
	{
		for(j = 0;j < 8;j ++)
		{
		 	for(i = 0;i < 8;i ++)
			{
			 	if(*p & (1 << i))
					(*DrawPoint)(pHandle, x + j,y + i);
				else
					(*ClearPoint)(pHandle, x + j,y + i);
			}
			p ++;
		}
		for(j = 0;j < 8;j ++)
		{
		 	for(i = 0;i < FontSize - 8;i ++)
			{
				if(*p & (1 << i))
					(*DrawPoint)(pHandle, x + j,y + 8 + i);
				else
					(*ClearPoint)(pHandle, x + j,y + 8 + i);
			}
			p ++;
		}
	}
}

/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_ShowChar(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 CHAR,LCD12864_FONT_MODE FontMode)
* 功能			:	在指定位置显示一个指定大小的字符
* 参数			:	pHandle:句柄；x,y:显示开始坐标,p:汉子点阵缓冲区;FontMode:汉子显示模式,
* 返回			:	无
* 依赖			:	画点函数
* 作者			:	cp1300@139.com
* 时间			:	20120603
* 最后修改时间 	: 	2018-08-12
* 说明			:
*************************************************************************************************************************/
void LCD12864_GRAM_ShowPic(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 w,u8 h, u8 *data, Bool isReverse)
{
	 u8 i,j;


	if(w > 127) 
		w = 127;
	y /= 8;  //计算页地址
	h /= 8;

	for(i = 0;i < h;i ++)
	{
		for(j = 0;j < w;j ++)
		{
			
			if(isReverse)
			{
				pHandle->pFramebuf[(i+y)*128 + x + j] = ~data[i*w+j];
			}
			else
			{
				pHandle->pFramebuf[(i+y)*128 + x + j] = data[i*w+j];
			}
		
		}
	
	}
	
}


/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_Fill(LCD12864_HANDLE *pHandle, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
* 功能			:	指定位置填充
* 参数			:	pHandle:句柄；范围
* 返回			:	无
* 依赖			:	底层函数
* 作者			:	cp1300@139.com
* 时间			:	20110920
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_Fill(LCD12864_HANDLE *pHandle, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{
 	u16 i,j;

	for(i = xStart;i < xEnd; i ++)
	{
		for(j = yStart;j < yEnd;j ++)
		{
			LCD12864_GRAM_DrawPoint(pHandle,i,j);
		}
	}
}


/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_Clear(LCD12864_HANDLE *pHandle, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
* 功能			:	清除指定位置
* 参数			:	pHandle:句柄；范围
* 返回			:	无
* 依赖			:	底层函数
* 作者			:	cp1300@139.com
* 时间			:	20110920
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_Clear(LCD12864_HANDLE *pHandle, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{
 	u16 i,j;

	for(i = xStart;i < xEnd; i ++)
	{
		for(j = yStart;j < yEnd;j ++)
		{
			LCD12864_GRAM_ClearPoint(pHandle,i,j);
		}
	}
}



/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_DrawLine(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
* 功能			:	画线函数
* 参数			:	pHandle:句柄；起点终点坐标
* 返回			:	无
* 依赖			:	画点函数
* 作者			:	cp1300@139.com
* 时间			:	20110920
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_DrawLine(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	//TFT_LCD_SetRamAddr(0,239,0,319);//设置显示窗口

	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		LCD12864_GRAM_DrawPoint(pHandle, uRow,uCol);//画点
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}



/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_DrawRectangle(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
* 功能			:	在指定位置画一个矩形
* 参数			:	pHandle:句柄；多边形的两个坐标
* 返回			:	无
* 依赖			:	画线函数
* 作者			:	cp1300@139.com
* 时间			:	20110920
* 最后修改时间 	: 	2018-08-12
* 说明			: 	无
*************************************************************************************************************************/
void LCD12864_GRAM_DrawRectangle(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD12864_GRAM_DrawLine(pHandle, x1,y1,x2,y1);
	LCD12864_GRAM_DrawLine(pHandle, x1,y1,x1,y2);
	LCD12864_GRAM_DrawLine(pHandle, x1,y2,x2,y2);
	LCD12864_GRAM_DrawLine(pHandle, x2,y1,x2,y2);
}





/*************************************************************************************************************************
* 函数			:	void LCD12864_GRAM_ShowString(LCD12864_HANDLE *pHandle, u16 x,u16 y,const char *pStr,LCD12864_FONT_MODE Font_MODE)
* 功能			:	在显存指定位置显示字符串
* 参数			:	pHandle:句柄；x,y:显示开始坐标,pStr:字符串缓冲区;FontMode:显示模式,
* 返回			:	无
* 依赖			:	画点函数
* 作者			:	cp1300@139.com
* 时间			:	2014-08-20
* 最后修改时间 	: 	2018-08-12
* 说明			:
*************************************************************************************************************************/
void LCD12864_GRAM_ShowString(LCD12864_HANDLE *pHandle, u16 x,u16 y,const char *pStr,LCD12864_FONT_MODE Font_MODE)
{
	u8 Font_Size = Font_MODE & 0x0f;
#if	CHINESE_ENABLE
	u8 buff[32];
#endif

	while(*pStr != 0)
	{
#if	CHINESE_ENABLE
		if(*pStr > 0x80)//汉字
		{
			FONT_GetFontLattice(buff, (u8*)pStr, ST16X16);	//获取汉字点阵
			LCD12864_GRAM_ShowChina(x,y,buff,Font_MODE);	//显示汉字
			pStr += 2;
			if(x > 127 - 16)  //自动换行
			{
			 	x = 0;
				y += 16;
			}
			else
			{
				x += 16;
			}
		}
		else //ASCII
#endif
		{
			LCD12864_GRAM_ShowChar(pHandle, x,y,*pStr,Font_MODE);
			pStr++;
			if(x > 127 - 8)  //自动换行
			{
			 	x = 0;
				y += Font_Size;
			}
			else
			{
				x += 8;
			}
		}

	}
}

