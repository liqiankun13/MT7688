#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <common.h>
#include "LCD12864.h"

#define RGB(r,g,b) ( (r>>3)<<11|(g>>2)<<5|(b>>3) ) 

typedef enum{
	LCD_CLEAN 			= 0x0000,
	LCD_FILL_BLACK 	= LCD_CLEAN,
	LCD_FILL_RED 		= 0xF800,
	LCD_FILL_YELLOW 	= 0xFFE0,
	LCD_FILL_GREEN 	= 0x07E0,
	LCD_FILL_BLUE 		= 0x001F,
	LCD_FILL_WHITE 	,
	LCD_FILL_NONE,
	
}CorlorTypeDef;
typedef enum{

	LCD_FONT_BIG,
	LCD_FONT_MIDLLE,
	LCD_FONT_SMALL,
}FontSizeTypeDef;


#define _LCD_WIDTH 240
#define _LCD_HIGHT 240
#define _LCD_BPP   16








int Init_Dislay();
void Exit_Dislay();

void GUIDrawText(u16 x, u16 y, const char *text,FontSizeTypeDef fontSize, CorlorTypeDef fontCorlor, CorlorTypeDef BackCorlor);
void  GUICleanScreen(u16 x, u16 y, u16 w,u16 h);
int GUILoadBmp(char *pathname,uint16_t w,uint16_t h,unsigned char *rect);
void GUIImageDraw(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t *rect);
void  GUIFullScreen(uint16_t x, uint16_t y, uint16_t w,uint16_t h, uint16_t corlor);



#endif
