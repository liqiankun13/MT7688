/*
 * LCD12864_Virtual.c
 * ����LCD12864
 *  Created on: 2018��8��12��
 *      Author: cfan
 */
#include "LCD12864.h"
#include "common.h"
#include "ASCII_8x16.h"
#include <stdio.h>
#include <string.h>

/*************************************************************************************************************************
* ����			:	void LCD12864_GRAM_Init(LCD12864_HANDLE *pHandle)
* ����			:	LCD12864�Դ�ģʽ��ʼ��
* ����			:	pHandle:�����
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120531
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
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
* ����			:	void LCD12864_GRAM_DrawPoint(LCD12864_HANDLE *pHandle, u8 x,u8 y)
* ����			:	���Դ�����ָ��λ�û���
* ����			:	pHandle:�����x:X����,0-127;y:y����,0-63
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120531
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
*************************************************************************************************************************/
void LCD12864_GRAM_DrawPoint(LCD12864_HANDLE *pHandle, u8 x,u8 y)
{
	if(x > 127 || y > 63)
		return;
	pHandle->pFramebuf[y / 8*128 + x] |= (1 << (y % 8));
}




/*************************************************************************************************************************
* ����			:	void LCD12864_GRAM_ClearPoint(LCD12864_HANDLE *pHandle, u8 x,u8 y)
* ����			:	�����Դ�����ָ��λ�õĵ�
* ����			:	pHandle:�����x:X����,0-127;y:y����,0-63
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120531
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
*************************************************************************************************************************/
void LCD12864_GRAM_ClearPoint(LCD12864_HANDLE *pHandle, u8 x, u8 y)
{
	if(x > 127 || y > 63)
		return;
	pHandle->pFramebuf[y / 8*128 + x] &= ~(1 << (y % 8));
}



/*************************************************************************************************************************
* ����			:	void LCD12864_GRAM_ClearAll(LCD12864_HANDLE *pHandle)
* ����			:	���ȫ���Դ�
* ����			:	pHandle:�����
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120531
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
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
* ����			:	void LCD12864_GRAM_ShowChar(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 CHAR,LCD12864_FONT_MODE FontMode)
* ����			:	��ָ��λ����ʾһ��ָ����С���ַ�
* ����			:	pHandle:�����x,y:��ʾ��ʼ����,p:���ӵ��󻺳���;FontMode:������ʾģʽ,
* ����			:	��
* ����			:	���㺯��
* ����			:	cp1300@139.com
* ʱ��			:	20120603
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			:
*************************************************************************************************************************/
void LCD12864_GRAM_ShowChar(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 CHAR,LCD12864_FONT_MODE FontMode)
{
	u8 i,j;
	u8 FontSize = (u8)FontMode&0x0f;	//��ȡ�����С
	u8 *p;
	void (*DrawPoint)(LCD12864_HANDLE *pHandle, u8 i,u8 j);
	void (*ClearPoint)(LCD12864_HANDLE *pHandle, u8 i,u8 j);

	CHAR -= 32;
	if(CHAR > 95 - 1) //����ASCII��Χ
		return;

	if(FontSize)
	{
		FontSize = 12;
		p = (u8 *)ASCII_8X12[CHAR];		//12��
	}
	else
	{
		FontSize = 16;
		p = (u8 *)ASCII_8X16[CHAR];		//16��
	}

	if(FontMode & 0x40)	//����
	{
		DrawPoint = LCD12864_GRAM_ClearPoint;
		ClearPoint =  LCD12864_GRAM_DrawPoint;
	}
	else //����ģʽ
	{
		ClearPoint =  LCD12864_GRAM_ClearPoint;
		DrawPoint =  LCD12864_GRAM_DrawPoint;
	}

	if(FontMode & 0x80)	//������ʾ
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
	else	//�ǵ�����ʾ
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
* ����			:	void LCD12864_GRAM_ShowChar(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 CHAR,LCD12864_FONT_MODE FontMode)
* ����			:	��ָ��λ����ʾһ��ָ����С���ַ�
* ����			:	pHandle:�����x,y:��ʾ��ʼ����,p:���ӵ��󻺳���;FontMode:������ʾģʽ,
* ����			:	��
* ����			:	���㺯��
* ����			:	cp1300@139.com
* ʱ��			:	20120603
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			:
*************************************************************************************************************************/
void LCD12864_GRAM_ShowPic(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 w,u8 h, u8 *data, Bool isReverse)
{
	 u8 i,j;


	if(w > 127) 
		w = 127;
	y /= 8;  //����ҳ��ַ
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
* ����			:	void LCD12864_GRAM_Fill(LCD12864_HANDLE *pHandle, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
* ����			:	ָ��λ�����
* ����			:	pHandle:�������Χ
* ����			:	��
* ����			:	�ײ㺯��
* ����			:	cp1300@139.com
* ʱ��			:	20110920
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
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
* ����			:	void LCD12864_GRAM_Clear(LCD12864_HANDLE *pHandle, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
* ����			:	���ָ��λ��
* ����			:	pHandle:�������Χ
* ����			:	��
* ����			:	�ײ㺯��
* ����			:	cp1300@139.com
* ʱ��			:	20110920
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
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
* ����			:	void LCD12864_GRAM_DrawLine(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
* ����			:	���ߺ���
* ����			:	pHandle:���������յ�����
* ����			:	��
* ����			:	���㺯��
* ����			:	cp1300@139.com
* ʱ��			:	20110920
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
*************************************************************************************************************************/
void LCD12864_GRAM_DrawLine(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	//TFT_LCD_SetRamAddr(0,239,0,319);//������ʾ����

	delta_x=x2-x1; //������������
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //���õ�������
	else if(delta_x==0)incx=0;//��ֱ��
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//ˮƽ��
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//�������
	{
		LCD12864_GRAM_DrawPoint(pHandle, uRow,uCol);//����
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
* ����			:	void LCD12864_GRAM_DrawRectangle(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
* ����			:	��ָ��λ�û�һ������
* ����			:	pHandle:���������ε���������
* ����			:	��
* ����			:	���ߺ���
* ����			:	cp1300@139.com
* ʱ��			:	20110920
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	��
*************************************************************************************************************************/
void LCD12864_GRAM_DrawRectangle(LCD12864_HANDLE *pHandle,u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD12864_GRAM_DrawLine(pHandle, x1,y1,x2,y1);
	LCD12864_GRAM_DrawLine(pHandle, x1,y1,x1,y2);
	LCD12864_GRAM_DrawLine(pHandle, x1,y2,x2,y2);
	LCD12864_GRAM_DrawLine(pHandle, x2,y1,x2,y2);
}





/*************************************************************************************************************************
* ����			:	void LCD12864_GRAM_ShowString(LCD12864_HANDLE *pHandle, u16 x,u16 y,const char *pStr,LCD12864_FONT_MODE Font_MODE)
* ����			:	���Դ�ָ��λ����ʾ�ַ���
* ����			:	pHandle:�����x,y:��ʾ��ʼ����,pStr:�ַ���������;FontMode:��ʾģʽ,
* ����			:	��
* ����			:	���㺯��
* ����			:	cp1300@139.com
* ʱ��			:	2014-08-20
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			:
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
		if(*pStr > 0x80)//����
		{
			FONT_GetFontLattice(buff, (u8*)pStr, ST16X16);	//��ȡ���ֵ���
			LCD12864_GRAM_ShowChina(x,y,buff,Font_MODE);	//��ʾ����
			pStr += 2;
			if(x > 127 - 16)  //�Զ�����
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
			if(x > 127 - 8)  //�Զ�����
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

