/*************************************************************************************************************
 * �ļ���:		lcd12864.h
 * ����:		JLX12864G-0088 LCD12864Һ������
 * ����:		cp1300@139.com
 * ����:		cp1300@139.com
 * ����ʱ��:	2012��5��30��20:40
 * ����޸�ʱ��:2012��5��30��
 * ��ϸ:		
*************************************************************************************************************/
#ifndef _JLX12864G_H_
#define _JLX12864G_H_

#include "common.h"


//LCD�ͺ�							
#define 	JLX12864G_0088		0
#define 	JLX12864G_1016		1






//��ת180����ʾ
#define 	LCD_ROTATE_180		0
//LCD�ͺ�
#define JLX12864G_MODEL		JLX12864G_0088
							



//LCD ������ƫ��,ĳЩLCD��ת180�Ⱥ�X������Ҫƫ��
#if((JLX12864G_MODEL==JLX12864G_1016)&&LCD_ROTATE_180)
#define JLX12864G_X_OFFSET	4
#else
#define JLX12864G_X_OFFSET	0
#endif



//JLX12864G ���
typedef struct
{
	void (*lock)();
	void (*unlock)();	
	void (*WriteData)(u8 *data,u8 len);	//д���ݽӿ�
	void (*SetRS)(u8 level);			//����RS��ƽ
	void (*SetRST)(u8 level);			//����RST��ƽ
	void (*DelayMS)(u8 ms);				//ms��ʱ
	u8 LCD_Cont;						//��¼���õĶԱȶ�
}JLX12864G_HANDLE;



//��ؽӿ�
//��ʼ��12864
void JLX12864G_Init(JLX12864G_HANDLE *pHandle,
		void (*WriteData)(u8 *data,u8 len),	//д���ݽӿ�
		void (*SetRS)(u8 level),			//����RS��ƽ
		void (*SetRST)(u8 level),			//����RST��ƽ
		void (*DelayMS)(u8 ms),				//ms��ʱ
		u8 LCDCont);
void JLX12864G_ClearAll(JLX12864G_HANDLE *pHandle);	//ȫ�����
void JLX12864G_FillAll(JLX12864G_HANDLE *pHandle);	//���ȫ��
void JLX12864G_PrintfChar(JLX12864G_HANDLE *pHandle, u8 PageAdd,u8 LineAdd,const char *p,u8 FontSize);	//�ַ�����ʾ����
void JLX12864G_SetConAdj(JLX12864G_HANDLE *pHandle, u8 cont);
void JLX12864G_GRAM_Up(JLX12864G_HANDLE *pHandle, u8 LCD_BUFF[8][128], u8 x1,u8 y1,u8 x2,u8 y2);//�����Դ���Һ��

#if LCD_BUFF_ENABLE		//ʹ�����Դ�


typedef enum
{
	FONT16_DEFAULT 	= (0x80+16),	//16��,������ʾ
	FONT12_DEFAULT 	= (0x80+12),	//12��,������ʾ
	FONT16_COVER 	= 	(16),		//16��,������ʾ
	FONT12_COVER 	= 	(12),		//12��,������ʾ
	FONT16_REVERSE 	= (0x40+16),	//16��,������ʾ
	FONT12_REVERSE 	= (0x40+12),	//12��,������ʾ
}FONT_MODE;

void GRAM_DrawPoint(u8 x,u8 y);									//�Դ�д��
void GRAM_ClearPoint(u8 x,u8 y); 								//�Դ����
void GRAM_ClearAll(void);										//����Դ�
void GRAM_Up(u8 x1,u8 y1,u8 x2,u8 y2);							//�����Դ�
#define GRAM_UpAll()	GRAM_Up(0,0,128,64)						//����ȫ���Դ�
void GRAM_ShowChina(u8 x,u8 y,const u8 *p,FONT_MODE FontMode);	//������ʾ
void GRAM_ShowChar(u8 x,u8 y,u8 CHAR,FONT_MODE FontMode);		//ASCII��ʾ
void GRAM_Fill(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd);
void GRAM_Clear(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd);
void GRAM_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void GRAM_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void GRAM_ShowString(u16 x,u16 y,const char *pStr,FONT_MODE Font_MODE);
u8 *LCD12864_GetGramBuff(void);//��ȡ�Դ��ַ
//��ȡ�ϵ�״̬						
bool LCD12864_GetPowerStatus(void);

#endif


#endif //_JLX12864G_H_
