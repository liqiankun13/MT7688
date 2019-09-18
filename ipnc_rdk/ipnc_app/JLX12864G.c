/*************************************************************************************************************
 * 文件名:		JLX12864G.c
 * 功能:		JLX12864G-0088 JLX12864G液晶驱动
 * 作者:		cp1300@139.com
 * 邮箱:		cp1300@139.com
 * 创建时间:	2012年5月30日20:40
 * 最后修改时间:2012年5月30日
 * 详细:		2016-02-01:增加获取显存函数
*************************************************************************************************************/
#include "ASCII_8x16.h"
#include "JLX12864G.h"
#include <stdio.h>
#include "common.h"


//字模取模方式：阴码,列行式,逆向(低位在前)



//汉字支持
#define CHINESE_ENABLE		0







/*************************************************************************************************************************
* 函数			:	void JLX12864G_WriteCommand(JLX12864G_HANDLE *pHandle, u8 cmd)
* 功能			:	向JLX12864G写入一字节命令
* 参数			:	pHandle:句柄；cmd:命令
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120530
* 最后修改时间 	: 	2018-08-12
* 说明			: 	RS=0,时钟上升沿数据有效,先发送高位
*************************************************************************************************************************/
void JLX12864G_WriteCommand(JLX12864G_HANDLE *pHandle, u8 cmd)
{
	

	pHandle->SetRS(0);				//RS=0
	pHandle->WriteData(&cmd, 1);	//发送数据
	pHandle->SetRS(1);				//RS=1
	
}


/*************************************************************************************************************************
* 函数	:	static void JLX12864G_SetPageAdd(JLX12864G_HANDLE *pHandle, u8 PageAdd)
* 功能	:	设置光标页地址
* 参数	:	pHandle:句柄；PageAdd:页地址,0-7
* 返回	:	无
* 依赖	:	底层宏定义
* 作者	:	cp1300@139.com
* 时间	:	20120531
* 最后修改时间 : 20120531
* 说明	: 	共64行,没8行为一页,共8页
*************************************************************************************************************************/
static void JLX12864G_SetPageAdd(JLX12864G_HANDLE *pHandle, u8 PageAdd)
{
	JLX12864G_WriteCommand(pHandle, 0xb0 + PageAdd);
}


/*************************************************************************************************************************
* 函数	:	static void JLX12864G_SetLineAdd(JLX12864G_HANDLE *pHandle, u8 LineAdd)
* 功能	:	设置光标列地址
* 参数	:	pHandle:句柄；LineAdd:列地址,0-127
* 返回	:	无
* 依赖	:	底层宏定义
* 作者	:	cp1300@139.com
* 时间	:	20120531
* 最后修改时间 : 20120531
* 说明	: 	共128列
*************************************************************************************************************************/
static void JLX12864G_SetLineAdd(JLX12864G_HANDLE *pHandle, u8 LineAdd)
{
	LineAdd += JLX12864G_X_OFFSET;
	JLX12864G_WriteCommand(pHandle, 0x10 + (LineAdd >> 4));		//列地址高4位
	JLX12864G_WriteCommand(pHandle, 0x00 + (LineAdd & 0x0f)); 	//列地址低4位
}




/*************************************************************************************************************************
* 函数			:	void JLX12864G_ClearAll(JLX12864G_HANDLE *pHandle)
* 功能			:	JLX12864G液晶清屏
* 参数			:	pHandle:句柄；
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120530
* 最后修改时间 	: 	20120530
* 说明			: 	无
*************************************************************************************************************************/
void JLX12864G_ClearAll(JLX12864G_HANDLE *pHandle)
{
	u8 i,j;
	u32 data = 0x0;

	for(i = 0;i < 9;i ++)
	{
		JLX12864G_SetPageAdd(pHandle, i);
		JLX12864G_SetLineAdd(pHandle, 0);
		for(j = 0;j < 132/4;j ++)
		{
			//JLX12864G_WriteData((u8 *)&data, 4);
		}
	}
}


/*************************************************************************************************************************
* 函数			:	void JLX12864G_FillAll(JLX12864G_HANDLE *pHandle)
* 功能			:	JLX12864G液晶填充
* 参数			:	pHandle:句柄；
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120530
* 最后修改时间 	: 	20120530
* 说明			: 	无
*************************************************************************************************************************/
void JLX12864G_FillAll(JLX12864G_HANDLE *pHandle)
{
	u8 i,j;
	u32 data = 0xffffffff;

	for(i = 0;i < 9;i ++)
	{
		JLX12864G_SetPageAdd(pHandle, i);
		JLX12864G_SetLineAdd(pHandle, 0);
		for(j = 0;j < 132/4;j ++)
		{
			pHandle->WriteData((u8 *)&data, 4);
		}
	}
}


/*************************************************************************************************************************
* 函数	:	void JLX12864G_ShowOneChar(JLX12864G_HANDLE *pHandle,u8 PageAdd,u8 LineAdd,u8 CHAR,u8 FontSize)
* 功能	:	在指定位置显示一个字符
* 参数	:	pHandle:句柄；PageAdd:页,0~7,共8页;L:0~127共128列,CHAR:需要显示的字符,FontSize:字体大小
* 返回	:	无
* 依赖	:	底层宏定义
* 作者	:	cp1300@139.com
* 时间	:	20120530
* 最后修改时间 : 20120530
* 说明	: 	显示一个ASCII字符
*************************************************************************************************************************/
void JLX12864G_ShowOneChar(JLX12864G_HANDLE *pHandle,u8 PageAdd,u8 LineAdd,u8 CHAR,u8 FontSize)
{

	u8 i,j,k;
	const unsigned char *p;


	CHAR -= 32;
	if(CHAR > ASCII_MAX - 1)
		return;

	if(FontSize == 12)
		p = ASCII_8X12[CHAR];		//12号
	else
		p = ASCII_8X16[CHAR];		//16号

	for(i = 0;i < 2;i ++)
	{
		JLX12864G_SetPageAdd(pHandle, PageAdd + i);
		JLX12864G_SetLineAdd(pHandle, LineAdd);
		k = i * 8;
		pHandle->WriteData((u8 *)&p[k+j], 8);
		/*for(j = 0;j < 8;j ++)
		{
			pHandle->WriteByteData(p[k+j]);
		}*/
	}
}



/*************************************************************************************************************************
* 函数	:	void LCD_PrintfChar(JLX12864G_HANDLE *pHandle,u8 PageAdd,u8 LineAdd,const char *p,u8 FontSize)
* 功能	:	在指定位置显示字符串
* 参数	:	pHandle:句柄；PageAdd:页,0~7,共8页;L:0~127共128列;p:字符指针,FontSize:子大小;16或者12
* 返回	:	无
* 依赖	:	JLX12864G_ShowOneChar
* 作者	:	cp1300@139.com
* 时间	:	20120601
* 最后修改时间 : 20120601
* 说明	: 	FontSize = 16或者 12
*************************************************************************************************************************/
void JLX12864G_PrintfChar(JLX12864G_HANDLE *pHandle,u8 PageAdd,u8 LineAdd,const char *p,u8 FontSize)
{
	while(*p != 0)
	{
		JLX12864G_ShowOneChar(pHandle, PageAdd,LineAdd,*p,FontSize);
		p ++;
		LineAdd += 8;
	}
}


/*************************************************************************************************************************
* 函数	:	void JLX12864G_SetConAdj(JLX12864G_HANDLE *pHandle,u8 cont)
* 功能	:	设置液晶的对比度
* 参数	:	pHandle:句柄；cont:对比度值
* 返回	:	无
* 依赖	:	底层宏定义
* 作者	:	cp1300@139.com
* 时间	:	2014-08-24
* 最后修改时间 : 2014-08-24
* 说明	: 	需要先初始化LCD
*************************************************************************************************************************/
void JLX12864G_SetConAdj(JLX12864G_HANDLE *pHandle,u8 cont)
{
	if(cont < 25)cont = 25;
	if(cont > 60)cont = 60;

	JLX12864G_WriteCommand(pHandle, 0x81); /*微调对比度*/
	JLX12864G_WriteCommand(pHandle, cont); /*微调对比度的值，可设置范围0～63*/
	pHandle->LCD_Cont = cont;		//更新对比度
}


/*************************************************************************************************************************
* 函数			:	void JLX12864G_Init(JLX12864G_HANDLE *pHandle,
						void (*WriteData)(u8 data,u8 len),		//写数据接口
						void (*SetRS)(u8 level),		//设置RS电平
						void (*SetRST)(u8 level),		//设置RST电平
						void (*DelayMS)(u8 ms),			//ms延时
						u8 LCDCont)
* 功能			:	初始化JLX12864G液晶
* 参数			:	pHandle:句柄；WriteByteData:写一字节函数；SetRS：设置RS电平；SetRST：设置RST电平；DelayMS：系统ms延时；LCDCont：对比度
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120530
* 最后修改时间 	: 2018-08-12
* 说明			: 	初始化JLX12864G液晶
*************************************************************************************************************************/
void JLX12864G_Init(JLX12864G_HANDLE *pHandle,
		void (*WriteData)(u8 *data,u8 len),		//写数据接口
		void (*SetRS)(u8 level),		//设置RS电平
		void (*SetRST)(u8 level),		//设置RST电平
		void (*DelayMS)(u8 ms),			//ms延时
		u8 LCDCont)
{
	if(pHandle == NULL)
	{
		printf("JLX12864G:ERROR *pHandle is NULL!\r\n");
		return;
	}
	if(WriteData == NULL)
	{
		printf("JLX12864G:ERROR *WriteData is NULL!\r\n");
		return;
	}
	if(SetRS == NULL)
	{
		printf("JLX12864G:ERROR *SetRS is NULL!\r\n");
		return;
	}
	if(DelayMS == NULL)
	{
		printf("JLX12864G:ERROR *DelayMS is NULL!\r\n");
		return;
	}

	pHandle->WriteData = WriteData;
	pHandle->SetRS = SetRS;
	pHandle->SetRST = SetRST;
	pHandle->DelayMS = DelayMS;

	if(pHandle->SetRST != NULL)
	{
		SetRST(0);//液晶复位开始
		DelayMS(2);
		SetRST(1);//液晶复位结束

	}


	JLX12864G_WriteCommand(pHandle, 0xe2); /*软复位*/
	JLX12864G_WriteCommand(pHandle, 0x2c); /*升压步聚1*/
	JLX12864G_WriteCommand(pHandle, 0x2e); /*升压步聚2*/
	JLX12864G_WriteCommand(pHandle, 0x2f); /*升压步聚3*/
	JLX12864G_WriteCommand(pHandle, 0x23); /*粗调对比度，可设置范围20～27*/
	JLX12864G_WriteCommand(pHandle, 0x81); /*微调对比度*/
//	JLX12864G_WriteCommand(0x30); /*微调对比度的值，可设置范围0～63*/
	//JLX12864G_SetConAdj(pHandle, LCDCont);
	JLX12864G_WriteCommand(pHandle, 0xa2); /*1/9 偏压比（bias）*/
#if(LCD_ROTATE_180)	//旋转180度显示
	JLX12864G_WriteCommand(pHandle, 0xc0); /*行扫描顺序：从下到上*/
	JLX12864G_WriteCommand(pHandle, 0xa1); /*列扫描顺序：从右到左*/
#else
	JLX12864G_WriteCommand(pHandle, 0xc8); /*行扫描顺序：从上到下*/
	JLX12864G_WriteCommand(pHandle, 0xa0); /*列扫描顺序：从左到右*/
#endif
	JLX12864G_WriteCommand(pHandle, 0x40); //初始化显示行为0

//	JLX12864G_WriteCommand(pHandle, 0xa4); //常规显示
	JLX12864G_WriteCommand(pHandle, 0xaf); /*开显示*/
	JLX12864G_ClearAll(pHandle);
	pHandle->LCD_Cont = LCDCont;
	//isPowerStatus = TRUE;	//上电完成
}




/*************************************************************************************************************************
* 函数			:	void JLX12864G_GRAM_Up(JLX12864G_HANDLE *pHandle, u8 LCD_BUFF[8][128], u8 x1,u8 y1,u8 x2,u8 y2)
* 功能			:	更新显存至液晶
* 参数			:	pHandle：句柄；LCD_BUFF：显存地址；x1,y1:起始坐标;x2,y2:终点坐标
* 返回			:	无
* 依赖			:	底层宏定义
* 作者			:	cp1300@139.com
* 时间			:	20120531
* 最后修改时间 	: 	2018-08-12
* 说明			: 	y坐标会页对齐
*************************************************************************************************************************/

void JLX12864G_GRAM_Up(JLX12864G_HANDLE *pHandle, u8 LCD_BUFF[8][128], u8 x,u8 y,u8 w,u8 h)
{
 	u8 i,j;


	if(w > 127) 
		w = 127;
	y /= 8;  //计算页地址
	h /= 8;
	
	for(i = 0;i < h;i ++)
	{
		JLX12864G_SetPageAdd(pHandle, y + i);	//写入页地址
		JLX12864G_SetLineAdd(pHandle, x);  		//写入行地址
	//	pHandle->WriteData(&LCD_BUFF[y1 + i][x1 + 0], 36);
		for(j = 0;j < w;j ++)
		{
			//printf("0x%x ", LCD_BUFF[y + i][x + j]);
	
			pHandle->WriteData(&LCD_BUFF[y + i][x + j], 1);
		
		}
	
	}
	
	
}
