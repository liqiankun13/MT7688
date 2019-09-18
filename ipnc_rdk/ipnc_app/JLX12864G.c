/*************************************************************************************************************
 * �ļ���:		JLX12864G.c
 * ����:		JLX12864G-0088 JLX12864GҺ������
 * ����:		cp1300@139.com
 * ����:		cp1300@139.com
 * ����ʱ��:	2012��5��30��20:40
 * ����޸�ʱ��:2012��5��30��
 * ��ϸ:		2016-02-01:���ӻ�ȡ�Դ溯��
*************************************************************************************************************/
#include "ASCII_8x16.h"
#include "JLX12864G.h"
#include <stdio.h>
#include "common.h"


//��ģȡģ��ʽ������,����ʽ,����(��λ��ǰ)



//����֧��
#define CHINESE_ENABLE		0







/*************************************************************************************************************************
* ����			:	void JLX12864G_WriteCommand(JLX12864G_HANDLE *pHandle, u8 cmd)
* ����			:	��JLX12864Gд��һ�ֽ�����
* ����			:	pHandle:�����cmd:����
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120530
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	RS=0,ʱ��������������Ч,�ȷ��͸�λ
*************************************************************************************************************************/
void JLX12864G_WriteCommand(JLX12864G_HANDLE *pHandle, u8 cmd)
{
	

	pHandle->SetRS(0);				//RS=0
	pHandle->WriteData(&cmd, 1);	//��������
	pHandle->SetRS(1);				//RS=1
	
}


/*************************************************************************************************************************
* ����	:	static void JLX12864G_SetPageAdd(JLX12864G_HANDLE *pHandle, u8 PageAdd)
* ����	:	���ù��ҳ��ַ
* ����	:	pHandle:�����PageAdd:ҳ��ַ,0-7
* ����	:	��
* ����	:	�ײ�궨��
* ����	:	cp1300@139.com
* ʱ��	:	20120531
* ����޸�ʱ�� : 20120531
* ˵��	: 	��64��,û8��Ϊһҳ,��8ҳ
*************************************************************************************************************************/
static void JLX12864G_SetPageAdd(JLX12864G_HANDLE *pHandle, u8 PageAdd)
{
	JLX12864G_WriteCommand(pHandle, 0xb0 + PageAdd);
}


/*************************************************************************************************************************
* ����	:	static void JLX12864G_SetLineAdd(JLX12864G_HANDLE *pHandle, u8 LineAdd)
* ����	:	���ù���е�ַ
* ����	:	pHandle:�����LineAdd:�е�ַ,0-127
* ����	:	��
* ����	:	�ײ�궨��
* ����	:	cp1300@139.com
* ʱ��	:	20120531
* ����޸�ʱ�� : 20120531
* ˵��	: 	��128��
*************************************************************************************************************************/
static void JLX12864G_SetLineAdd(JLX12864G_HANDLE *pHandle, u8 LineAdd)
{
	LineAdd += JLX12864G_X_OFFSET;
	JLX12864G_WriteCommand(pHandle, 0x10 + (LineAdd >> 4));		//�е�ַ��4λ
	JLX12864G_WriteCommand(pHandle, 0x00 + (LineAdd & 0x0f)); 	//�е�ַ��4λ
}




/*************************************************************************************************************************
* ����			:	void JLX12864G_ClearAll(JLX12864G_HANDLE *pHandle)
* ����			:	JLX12864GҺ������
* ����			:	pHandle:�����
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120530
* ����޸�ʱ�� 	: 	20120530
* ˵��			: 	��
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
* ����			:	void JLX12864G_FillAll(JLX12864G_HANDLE *pHandle)
* ����			:	JLX12864GҺ�����
* ����			:	pHandle:�����
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120530
* ����޸�ʱ�� 	: 	20120530
* ˵��			: 	��
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
* ����	:	void JLX12864G_ShowOneChar(JLX12864G_HANDLE *pHandle,u8 PageAdd,u8 LineAdd,u8 CHAR,u8 FontSize)
* ����	:	��ָ��λ����ʾһ���ַ�
* ����	:	pHandle:�����PageAdd:ҳ,0~7,��8ҳ;L:0~127��128��,CHAR:��Ҫ��ʾ���ַ�,FontSize:�����С
* ����	:	��
* ����	:	�ײ�궨��
* ����	:	cp1300@139.com
* ʱ��	:	20120530
* ����޸�ʱ�� : 20120530
* ˵��	: 	��ʾһ��ASCII�ַ�
*************************************************************************************************************************/
void JLX12864G_ShowOneChar(JLX12864G_HANDLE *pHandle,u8 PageAdd,u8 LineAdd,u8 CHAR,u8 FontSize)
{

	u8 i,j,k;
	const unsigned char *p;


	CHAR -= 32;
	if(CHAR > ASCII_MAX - 1)
		return;

	if(FontSize == 12)
		p = ASCII_8X12[CHAR];		//12��
	else
		p = ASCII_8X16[CHAR];		//16��

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
* ����	:	void LCD_PrintfChar(JLX12864G_HANDLE *pHandle,u8 PageAdd,u8 LineAdd,const char *p,u8 FontSize)
* ����	:	��ָ��λ����ʾ�ַ���
* ����	:	pHandle:�����PageAdd:ҳ,0~7,��8ҳ;L:0~127��128��;p:�ַ�ָ��,FontSize:�Ӵ�С;16����12
* ����	:	��
* ����	:	JLX12864G_ShowOneChar
* ����	:	cp1300@139.com
* ʱ��	:	20120601
* ����޸�ʱ�� : 20120601
* ˵��	: 	FontSize = 16���� 12
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
* ����	:	void JLX12864G_SetConAdj(JLX12864G_HANDLE *pHandle,u8 cont)
* ����	:	����Һ���ĶԱȶ�
* ����	:	pHandle:�����cont:�Աȶ�ֵ
* ����	:	��
* ����	:	�ײ�궨��
* ����	:	cp1300@139.com
* ʱ��	:	2014-08-24
* ����޸�ʱ�� : 2014-08-24
* ˵��	: 	��Ҫ�ȳ�ʼ��LCD
*************************************************************************************************************************/
void JLX12864G_SetConAdj(JLX12864G_HANDLE *pHandle,u8 cont)
{
	if(cont < 25)cont = 25;
	if(cont > 60)cont = 60;

	JLX12864G_WriteCommand(pHandle, 0x81); /*΢���Աȶ�*/
	JLX12864G_WriteCommand(pHandle, cont); /*΢���Աȶȵ�ֵ�������÷�Χ0��63*/
	pHandle->LCD_Cont = cont;		//���¶Աȶ�
}


/*************************************************************************************************************************
* ����			:	void JLX12864G_Init(JLX12864G_HANDLE *pHandle,
						void (*WriteData)(u8 data,u8 len),		//д���ݽӿ�
						void (*SetRS)(u8 level),		//����RS��ƽ
						void (*SetRST)(u8 level),		//����RST��ƽ
						void (*DelayMS)(u8 ms),			//ms��ʱ
						u8 LCDCont)
* ����			:	��ʼ��JLX12864GҺ��
* ����			:	pHandle:�����WriteByteData:дһ�ֽں�����SetRS������RS��ƽ��SetRST������RST��ƽ��DelayMS��ϵͳms��ʱ��LCDCont���Աȶ�
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120530
* ����޸�ʱ�� 	: 2018-08-12
* ˵��			: 	��ʼ��JLX12864GҺ��
*************************************************************************************************************************/
void JLX12864G_Init(JLX12864G_HANDLE *pHandle,
		void (*WriteData)(u8 *data,u8 len),		//д���ݽӿ�
		void (*SetRS)(u8 level),		//����RS��ƽ
		void (*SetRST)(u8 level),		//����RST��ƽ
		void (*DelayMS)(u8 ms),			//ms��ʱ
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
		SetRST(0);//Һ����λ��ʼ
		DelayMS(2);
		SetRST(1);//Һ����λ����

	}


	JLX12864G_WriteCommand(pHandle, 0xe2); /*��λ*/
	JLX12864G_WriteCommand(pHandle, 0x2c); /*��ѹ����1*/
	JLX12864G_WriteCommand(pHandle, 0x2e); /*��ѹ����2*/
	JLX12864G_WriteCommand(pHandle, 0x2f); /*��ѹ����3*/
	JLX12864G_WriteCommand(pHandle, 0x23); /*�ֵ��Աȶȣ������÷�Χ20��27*/
	JLX12864G_WriteCommand(pHandle, 0x81); /*΢���Աȶ�*/
//	JLX12864G_WriteCommand(0x30); /*΢���Աȶȵ�ֵ�������÷�Χ0��63*/
	//JLX12864G_SetConAdj(pHandle, LCDCont);
	JLX12864G_WriteCommand(pHandle, 0xa2); /*1/9 ƫѹ�ȣ�bias��*/
#if(LCD_ROTATE_180)	//��ת180����ʾ
	JLX12864G_WriteCommand(pHandle, 0xc0); /*��ɨ��˳�򣺴��µ���*/
	JLX12864G_WriteCommand(pHandle, 0xa1); /*��ɨ��˳�򣺴��ҵ���*/
#else
	JLX12864G_WriteCommand(pHandle, 0xc8); /*��ɨ��˳�򣺴��ϵ���*/
	JLX12864G_WriteCommand(pHandle, 0xa0); /*��ɨ��˳�򣺴�����*/
#endif
	JLX12864G_WriteCommand(pHandle, 0x40); //��ʼ����ʾ��Ϊ0

//	JLX12864G_WriteCommand(pHandle, 0xa4); //������ʾ
	JLX12864G_WriteCommand(pHandle, 0xaf); /*����ʾ*/
	JLX12864G_ClearAll(pHandle);
	pHandle->LCD_Cont = LCDCont;
	//isPowerStatus = TRUE;	//�ϵ����
}




/*************************************************************************************************************************
* ����			:	void JLX12864G_GRAM_Up(JLX12864G_HANDLE *pHandle, u8 LCD_BUFF[8][128], u8 x1,u8 y1,u8 x2,u8 y2)
* ����			:	�����Դ���Һ��
* ����			:	pHandle�������LCD_BUFF���Դ��ַ��x1,y1:��ʼ����;x2,y2:�յ�����
* ����			:	��
* ����			:	�ײ�궨��
* ����			:	cp1300@139.com
* ʱ��			:	20120531
* ����޸�ʱ�� 	: 	2018-08-12
* ˵��			: 	y�����ҳ����
*************************************************************************************************************************/

void JLX12864G_GRAM_Up(JLX12864G_HANDLE *pHandle, u8 LCD_BUFF[8][128], u8 x,u8 y,u8 w,u8 h)
{
 	u8 i,j;


	if(w > 127) 
		w = 127;
	y /= 8;  //����ҳ��ַ
	h /= 8;
	
	for(i = 0;i < h;i ++)
	{
		JLX12864G_SetPageAdd(pHandle, y + i);	//д��ҳ��ַ
		JLX12864G_SetLineAdd(pHandle, x);  		//д���е�ַ
	//	pHandle->WriteData(&LCD_BUFF[y1 + i][x1 + 0], 36);
		for(j = 0;j < w;j ++)
		{
			//printf("0x%x ", LCD_BUFF[y + i][x + j]);
	
			pHandle->WriteData(&LCD_BUFF[y + i][x + j], 1);
		
		}
	
	}
	
	
}
