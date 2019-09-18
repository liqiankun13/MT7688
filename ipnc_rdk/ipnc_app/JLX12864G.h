/*************************************************************************************************************
 * 文件名:		lcd12864.h
 * 功能:		JLX12864G-0088 LCD12864液晶驱动
 * 作者:		cp1300@139.com
 * 邮箱:		cp1300@139.com
 * 创建时间:	2012年5月30日20:40
 * 最后修改时间:2012年5月30日
 * 详细:		
*************************************************************************************************************/
#ifndef _JLX12864G_H_
#define _JLX12864G_H_

#include "common.h"


//LCD型号							
#define 	JLX12864G_0088		0
#define 	JLX12864G_1016		1






//旋转180度显示
#define 	LCD_ROTATE_180		0
//LCD型号
#define JLX12864G_MODEL		JLX12864G_0088
							



//LCD 列坐标偏移,某些LCD旋转180度后X坐标需要偏移
#if((JLX12864G_MODEL==JLX12864G_1016)&&LCD_ROTATE_180)
#define JLX12864G_X_OFFSET	4
#else
#define JLX12864G_X_OFFSET	0
#endif



//JLX12864G 句柄
typedef struct
{
	void (*lock)();
	void (*unlock)();	
	void (*WriteData)(u8 *data,u8 len);	//写数据接口
	void (*SetRS)(u8 level);			//设置RS电平
	void (*SetRST)(u8 level);			//设置RST电平
	void (*DelayMS)(u8 ms);				//ms延时
	u8 LCD_Cont;						//记录设置的对比度
}JLX12864G_HANDLE;



//相关接口
//初始化12864
void JLX12864G_Init(JLX12864G_HANDLE *pHandle,
		void (*WriteData)(u8 *data,u8 len),	//写数据接口
		void (*SetRS)(u8 level),			//设置RS电平
		void (*SetRST)(u8 level),			//设置RST电平
		void (*DelayMS)(u8 ms),				//ms延时
		u8 LCDCont);
void JLX12864G_ClearAll(JLX12864G_HANDLE *pHandle);	//全屏清除
void JLX12864G_FillAll(JLX12864G_HANDLE *pHandle);	//填充全屏
void JLX12864G_PrintfChar(JLX12864G_HANDLE *pHandle, u8 PageAdd,u8 LineAdd,const char *p,u8 FontSize);	//字符串显示函数
void JLX12864G_SetConAdj(JLX12864G_HANDLE *pHandle, u8 cont);
void JLX12864G_GRAM_Up(JLX12864G_HANDLE *pHandle, u8 LCD_BUFF[8][128], u8 x1,u8 y1,u8 x2,u8 y2);//更新显存至液晶

#if LCD_BUFF_ENABLE		//使能了显存


typedef enum
{
	FONT16_DEFAULT 	= (0x80+16),	//16号,叠加显示
	FONT12_DEFAULT 	= (0x80+12),	//12号,叠加显示
	FONT16_COVER 	= 	(16),		//16号,覆盖显示
	FONT12_COVER 	= 	(12),		//12号,覆盖显示
	FONT16_REVERSE 	= (0x40+16),	//16号,反显显示
	FONT12_REVERSE 	= (0x40+12),	//12号,反显显示
}FONT_MODE;

void GRAM_DrawPoint(u8 x,u8 y);									//显存写点
void GRAM_ClearPoint(u8 x,u8 y); 								//显存擦点
void GRAM_ClearAll(void);										//清除显存
void GRAM_Up(u8 x1,u8 y1,u8 x2,u8 y2);							//更新显存
#define GRAM_UpAll()	GRAM_Up(0,0,128,64)						//更新全部显存
void GRAM_ShowChina(u8 x,u8 y,const u8 *p,FONT_MODE FontMode);	//汉子显示
void GRAM_ShowChar(u8 x,u8 y,u8 CHAR,FONT_MODE FontMode);		//ASCII显示
void GRAM_Fill(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd);
void GRAM_Clear(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd);
void GRAM_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void GRAM_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void GRAM_ShowString(u16 x,u16 y,const char *pStr,FONT_MODE Font_MODE);
u8 *LCD12864_GetGramBuff(void);//获取显存地址
//获取上电状态						
bool LCD12864_GetPowerStatus(void);

#endif


#endif //_JLX12864G_H_
