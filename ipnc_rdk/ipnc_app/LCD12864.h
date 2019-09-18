/*
 * LCD12864_Virtual.h
 *
 *  Created on: 2018年8月12日
 *      Author: cfan
 */

#ifndef PROGRAM_LCD_LCD12864_H_
#define PROGRAM_LCD_LCD12864_H_
#include "common.h"


typedef enum
{
	FONT16_DEFAULT 	= (0x80+16),	//16号,叠加显示
	FONT12_DEFAULT 	= (0x80+12),	//12号,叠加显示
	FONT16_COVER 	= 	(16),		//16号,覆盖显示
	FONT12_COVER 	= 	(12),		//12号,覆盖显示
	FONT16_REVERSE 	= (0x40+16),	//16号,反显显示
	FONT12_REVERSE 	= (0x40+12),	//12号,反显显示
}LCD12864_FONT_MODE;


//LCD12864 句柄
typedef struct
{
	u8 *pFramebuf;//显存

}LCD12864_HANDLE;


void LCD12864_GRAM_Init(LCD12864_HANDLE *pHandle);//LCD12864显存模式初始化
void LCD12864_GRAM_ShowString(LCD12864_HANDLE *pHandle, u16 x,u16 y,const char *pStr,LCD12864_FONT_MODE Font_MODE);	//在显存指定位置显示字符串
void LCD12864_GRAM_ShowPic(LCD12864_HANDLE *pHandle, u8 x,u8 y,u8 w,u8 h, u8 *data, Bool isReverse);

#endif /* PROGRAM_LCD_LCD12864_H_ */
