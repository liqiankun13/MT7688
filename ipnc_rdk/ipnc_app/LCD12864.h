/*
 * LCD12864_Virtual.h
 *
 *  Created on: 2018��8��12��
 *      Author: cfan
 */

#ifndef PROGRAM_LCD_LCD12864_H_
#define PROGRAM_LCD_LCD12864_H_
#include "common.h"


typedef enum
{
	FONT16_DEFAULT 	= (0x80+16),	//16��,������ʾ
	FONT12_DEFAULT 	= (0x80+12),	//12��,������ʾ
	FONT16_COVER 	= 	(16),		//16��,������ʾ
	FONT12_COVER 	= 	(12),		//12��,������ʾ
	FONT16_REVERSE 	= (0x40+16),	//16��,������ʾ
	FONT12_REVERSE 	= (0x40+12),	//12��,������ʾ
}LCD12864_FONT_MODE;


//LCD12864 ���
typedef struct
{
	u8 *pFramebuf;//�Դ�

}LCD12864_HANDLE;


void LCD12864_GRAM_Init(LCD12864_HANDLE *pHandle);//LCD12864�Դ�ģʽ��ʼ��
void LCD12864_GRAM_ShowString(LCD12864_HANDLE *pHandle, u16 x,u16 y,const char *pStr,LCD12864_FONT_MODE Font_MODE);	//���Դ�ָ��λ����ʾ�ַ���
void LCD12864_GRAM_ShowPic(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 w,u8 h, u8 *data, Bool isReverse);

#endif /* PROGRAM_LCD_LCD12864_H_ */
