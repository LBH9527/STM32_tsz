/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : bsp_tft_lcd.c
*	��    �� : V4.2
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-08-21 armfly  ST�̼���汾 V3.5.0�汾��

*
*********************************************************************************************************
*/
#include "bsp.h"
#include "lcd_nt35510.h"
#include "fonts.h"

static void _LCD_ReadAsciiDot(uint8_t code, uint8_t fontcode, uint8_t *pBuf);
void _LCD_ReadHZDot(uint8_t _code1, uint8_t _code2,  uint8_t _fontcode, uint8_t *_pBuf);
/********************************************************************************************************
*	�� �� ��: LCD_CtrlLinesConfig
*	����˵��: ����LCD���ƿ��ߣ�FSMC�ܽ�����Ϊ���ù���
*	��    ��: ��
*	�� �� ֵ: ��
********************************************************************************************************/
void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PD,PE,PF,PGʱ��  	
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PB15 �������,���Ʊ���
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� //PB15 �������,���Ʊ���
	
	GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(7<<8)|(3<<14);//PD0,1,4,5,8,9,10,14,15 AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin = (0X1FF<<7);//PE7~15,AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12
 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12
 
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);//PF12,AF12
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);
}

/********************************************************************************************************
*	�� �� ��: LCD_FSMCConfig
*	����˵��: ����FSMC���ڷ���ʱ��
*	��    ��: ��
*	�� �� ֵ: ��
********************************************************************************************************/
void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
	 RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ�� 
	
	readWriteTiming.FSMC_AddressSetupTime = 0XF;	 //��ַ����ʱ�䣨ADDSET��Ϊ16��HCLK 1/168M=6ns*16=96ns	
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	readWriteTiming.FSMC_DataSetupTime = 60;			//���ݱ���ʱ��Ϊ60��HCLK	=6*60=360ns
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
	
	writeTiming.FSMC_AddressSetupTime = 3;	      //��ַ����ʱ�䣨ADDSET��Ϊ3��HCLK =18ns 
	writeTiming.FSMC_AddressHoldTime = 0x00;
	writeTiming.FSMC_DataSetupTime = 2;				//���ݱ���ʱ��Ϊ6ns*(2+1)��HCLK=18ns
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA   	
		
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;	//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	// ���������ݵ�ַ
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;	//SRAM   	
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//�洢�����ݿ��Ϊ16bit  
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;//��дʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;	//дʱ��
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����
	
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);	// ʹ��BANK1������ 		
}

/*******************************************************************************
* ������  :  LCD_BacklightInit
* ����    �� �������
* ��������� level ��Χ0-10�� 10���� 0�
* ���ز����� ��
*******************************************************************************/
void bsp_LCDBackLightSet(uint8_t level)
{
	TIM_SetCompare2(TIM12,level*1000);
}
/*******************************************************************************
* ������  :  LCD_BacklightInit
* ����    �� ���⺯����ʼ��
* ��������� 1 �������ȿɵ���0�������Ȳ��ɵ�
* ���ز����� ��
*******************************************************************************/
void LCD_BacklightInit(void )
{
	if (LCD_BACKLIGHT_SET)
	{
		bsp_InitBacklightSetTimer();	//��ʱ��ʱ��Ϊ 84M����Ƶϵ��Ϊ 84�����Լ���Ƶ��
										//Ϊ 84M/84=1Mhz,��װ��ֵ 10000������ PWM Ƶ��Ϊ 1M/10000=100hz.
	}		
	else
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PB15 �������,���Ʊ���
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� //PB15 �������,���Ʊ���
	}							
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLCD
*	����˵��: ��ʼ��LCD
*	��    ��: _backlightSet��0�������Ȳ��ɵ���1�������ȿɵ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLCD(void)
{
	/* ����LCD���ƿ���GPIO */
	LCD_CtrlLinesConfig();
	/* ����FSMC�ӿڣ��������� */
	LCD_FSMCConfig();
	/* ������ʼ�� */
	Init_NT35510();
	/* �������ó�ʼ�� */
	LCD_BacklightInit();
	if (LCD_BACKLIGHT_SET)			
		bsp_LCDBackLightSet(LCD_BACKLIGHT_DEFAULT);
	else
		LCD_LED = 1;					//��������	
	/* ��Ļ���� */
	bsp_LCDDisplayDir(LCD_DIR_DEFAULT);	//Ĭ��Ϊ�������������ϣ�

	/* ���� */
	LCD_Clear(CL_WHITE);
}
/*
*********************************************************************************************************
*��������LCD_ShowChar()
* ���� ����ָ��λ����ʾһ���ַ�
* ���� ��x,y:��ʼ����, code : ASCII�ַ��ı��� size:�����С 12/16/24, mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
* ��� ��
*********************************************************************************************************
*/
void LCD_ShowChar1(uint16_t x,uint16_t y,uint8_t code, FONT_T *Font)
//void LCD_ShowChar1(uint16_t x,uint16_t y,uint8_t code, uint8_t fontcode,uint8_t mode)
{
	uint8_t temp, t1, t;
	uint16_t y0 = y;
//	uint8_t mode = 0;
	uint8_t Height,font_bytes;
	uint8_t buf[36];		/* ���֧��24����ASCII*/	
	
	switch (Font->FontCode)
	{
		case FC_ST_12:		/* 12���� */			
			Height = 12;
			font_bytes = 12;
			break;
		case FC_ST_16:				
			Height = 16;
			font_bytes = 16;
			break;
		case FC_ST_24:		   
			Height = 24;
			font_bytes = 36;
		    break;
		case FC_ST_32:
			break;
	}
	//���ô���		   
	code = code - ' ';//�õ�ƫ�ƺ��ֵ
	/* ��ascii�ַ������Ƶ�buf */
	_LCD_ReadAsciiDot(code, Font->FontCode, buf);		
	for ( t=0; t<font_bytes; t++)
	{
		temp = buf[t];	
		for (t1=0; t1<8;t1++)
		{
			if(temp&0x80)							//���λΪ1
				LCD_Fast_DrawPoint(x, y, Font->FrontColor );
			else
			{
				if (Font->BackColor != CL_MASK)	/* �ж��Ƿ�ı䱳��ɫ */
				{
					LCD_Fast_DrawPoint(x,y, Font->BackColor);/* ����������ɫΪ���ֱ���ɫ */
				}
			}					
			temp<<=1;								//��������
			y++;
			if((y-y0) == Height)
			{
				y = y0;
				x++;	
				break;				
			}
		}
	}
}
void LCD_ShowFont1(uint16_t x,uint16_t y,uint8_t code1, uint8_t code2,FONT_T *Font)
{
	uint8_t temp,t,t1;
	uint16_t y0=y;
	//uint8_t mode = 0;
	uint8_t Height,font_bytes;
	uint8_t buf[72];  
	
	switch (Font->FontCode)
	{
		case FC_ST_12:		/* 12���� */			
			Height = 12;
			font_bytes = 24;
			break;
		case FC_ST_16:				
			Height = 16;
			font_bytes = 32;
			break;
		case FC_ST_24:		   
			Height = 24;
			font_bytes = 72;
		    break;
		case FC_ST_32:
			break;
	}
	/* ����Ӧ�ֵ����Ƶ�buf */
	_LCD_ReadHZDot(code1, code2, Font->FontCode, buf);
	
	for(t=0;t<font_bytes;t++)
	{   												   
		temp=buf[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
				LCD_Fast_DrawPoint(x,y,Font->FrontColor);
			else 
			{
				if (Font->BackColor != CL_MASK)	/* �ж��Ƿ�ı䱳��ɫ */
				{
					LCD_Fast_DrawPoint(x,y, Font->BackColor);/* ����������ɫΪ���ֱ���ɫ */
				}
			}
			temp<<=1;
			y++;
			if((y-y0)==Height)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	} 	
}
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
	uint8_t temp, t1, t;
	uint16_t y0 = y;
	
	uint8_t csize = (size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	

	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	for ( t=0; t<csize; t++)
	{
		if(size==12)temp=Ascii_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=Ascii_1608[num][t];	//����1608����
		else if(size==24)temp=Ascii_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for (t1=0; t1<8;t1++)
		{
			if(temp&0x80)							//���λΪ1
				LCD_Fast_DrawPoint(x, y, POINT_COLOR);
			else if(mode == 0)
				LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;								//��������
			y++;
			if(y>=g_lcddev.height)return;				//��������
			if((y-y0)==size)
			{
				y = y0;
				x++;
				if(x>=g_lcddev.width)return;	//��������
				break;
			}
		}
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_GetFontWidth
*	����˵��: ��ȡ����Ŀ�ȣ����ص�λ)
*	��    ��: tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���		_
*	�� �� ֵ: ����Ŀ�ȣ����ص�λ)
*********************************************************************************************************
*/
uint16_t LCD_GetFontWidth(FONT_T *_tFont)
{
	uint16_t font_width = 16;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:
			font_width = 12;
			break;
		case FC_ST_16:		
			font_width = 16;
			break;					
		case FC_ST_24:
			font_width = 24;
			break;			
		case FC_ST_32:
			font_width = 32;
			break;			
	}
	return font_width;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetFontHeight
*	����˵��: ��ȡ����ĸ߶ȣ����ص�λ)
*	��    ��:_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ����Ŀ�ȣ����ص�λ)
*********************************************************************************************************
*/
uint16_t LCD_GetFontHeight(FONT_T *_tFont)
{
	uint16_t height = 16;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:
			height = 12;
			break;
		case FC_ST_16:		
			height = 16;
			break;		
		case FC_ST_24:
			height = 24;
			break;			
		case FC_ST_32:
			height = 32;
			break;			
	}
	return height;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetStrWidth
*	����˵��: �����ַ����ܿ��(���ص�λ)
*	��    ��:	_ptr  : �ַ���ָ��
*		_		tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD_GetStrWidth(uint8_t *_ptr, FONT_T *_tFont)
{
	uint8_t *p = _ptr;
	uint16_t width = 0;
	uint8_t code1, code2;
	uint16_t font_width;

	//font_width = LCD_GetFontWidth(_tFont);

	while (*p != 0)
	{
		code1 = *p;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
		if (code1 < 0x80)	/* ASCII */
		{	
			switch(_tFont->FontCode)
			{
				case FC_ST_12:
					font_width = 6;
					break;

				case FC_ST_16:		
					font_width = 8;
					break;
					
				case FC_ST_24:			
					font_width = 12;
					break;
					
				case FC_ST_32:
					font_width = 16;
					break;
				
				default:
					font_width = 8;
					break;					
			}			
			
		}
		else	/* ���� */
		{
			code2 = *++p;
			if (code2 == 0)
			{
				break;
			}
			font_width = LCD_GetFontWidth(_tFont);	//���ֿ��
			
		}
		width += font_width;
		p++;
	}
	return width;
}
/*
*********************************************************************************************************
*	�� �� ��: _LCD_ReadAsciiDot
*	����˵��: ��ȡ1��ASCII�ַ��ĵ�������
*	��    ��:
*		code : ASCII�ַ��ı��룬1�ֽڡ�1-128
*		fontcode ���������
*		pBuf : ��Ŷ������ַ���������
*	�� �� ֵ: ���ֿ��
*********************************************************************************************************
*/
static void _LCD_ReadAsciiDot(uint8_t code, uint8_t fontcode, uint8_t *pBuf)
{
	uint8_t *pAscDot;
	uint8_t font_bytes = 0;
	
	switch (fontcode)
	{
		case FC_ST_12:		/* 12���� */
			pAscDot = (uint8_t*)Ascii_1206[code];	
			font_bytes = 12;
			break;
		case FC_ST_16:	
			pAscDot = (uint8_t*)Ascii_1608[code];
			font_bytes = 16;
			break;
		case FC_ST_24:
		    pAscDot = (uint8_t*)Ascii_2412[code];
			font_bytes = 36;
		    break;
		case FC_ST_32:
			break;
	}
	//pAscDot = (uint8_t*)tFont->table;
	memcpy(pBuf, pAscDot, font_bytes);
}

/*
*********************************************************************************************************
*	�� �� ��: _LCD_ReadHZDot
*	����˵��: ��ȡ1�����ֵĵ�������
*	��    ��:
*		_code1, _cod2 : ��������. GB2312����
*		_fontcode ���������
*		_pBuf : ��Ŷ������ַ���������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _LCD_ReadHZDot(uint8_t _code1, uint8_t _code2,  uint8_t _fontcode, uint8_t *_pBuf)
{
	#if USE_SMALL_FONT		/* ʹ��CPU �ڲ�Flash С�ֿ� */
		uint8_t *pDot;
		uint8_t font_bytes = 0;
		uint16_t address;
		uint16_t m = 0;

		pDot = 0;	/* �������ڱ���澯 */
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12���� */
				font_bytes = 24;
				pDot = (uint8_t *)Hz12;	
				break;			
			case FC_ST_16:
				font_bytes = 32;
				pDot = (uint8_t *)Hz16;
				break;	
			case FC_ST_24:
				font_bytes = 72;
				pDot = (uint8_t *)Hz24;
				break;					
			case FC_ST_32:	
				font_bytes = 128;
		//		pDot = (uint8_t *)Hz32;
				break;
		}	
		m = 0;
		while(1)
		{
			address = m * (font_bytes + 2);
			m++;
			if ((_code1 == pDot[address + 0]) && (_code2 == pDot[address + 1]))
			{
				address += 2;
				memcpy(_pBuf, &pDot[address], font_bytes);
				break;
			}
			else if ((pDot[address + 0] == 0xFF) && (pDot[address + 1] == 0xFF))
			{
				/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
				memset(_pBuf, 0xFF, font_bytes);
				break;
			}
		}
	#else	/* ��ȫ�ֿ� */
		uint8_t font_bytes = 0;	//һ���ַ���Ӧ������ռ���ֽ���
		unsigned long foffset; 	
        
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12���� */
				font_bytes = 24;				
				break;			
			case FC_ST_16:
				font_bytes = 32;				
				break;	
			case FC_ST_24:
				font_bytes = 72;				
				break;					
//			case FC_ST_32:	
//				font_bytes = 128;
//		//		pDot = (uint8_t *)Hz32;
//				break;
		}
		if(_code1<0x81||_code2<0x40||_code2==0xff||_code1==0xff)//�� ���ú���
		{ 
			/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
			memset(_pBuf, 0xFF, font_bytes);
			return; //��������
		} 
		
		if(_code2 < 0x7f)
			_code2 -= 0x40;//ע��!
		else 
			_code2-=0x41;
		_code1 -= 0x81;	
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12���� */
				foffset =((unsigned long)190*_code1+_code2)*font_bytes;	//�õ��ֿ��е��ֽ�ƫ����  	
				W25QXX_Read(_pBuf,foffset + ftinfo.f12addr,font_bytes);	 			
				break;			
			case FC_ST_16:
				font_bytes = 32;
				foffset =((unsigned long)190*_code1+_code2)*font_bytes;	//�õ��ֿ��е��ֽ�ƫ����  				
				W25QXX_Read(_pBuf,foffset + ftinfo.f16addr,font_bytes);				
				break;	
			case FC_ST_24:
				font_bytes = 72;
				foffset =((unsigned long)190*_code1+_code2)*font_bytes;	//�õ��ֿ��е��ֽ�ƫ����  		
				W25QXX_Read(_pBuf,foffset + ftinfo.f24addr,font_bytes);			
				break;					
//			case FC_ST_32:	
//				font_bytes = 128;		
//				break;						
		}	
	#endif
}
	
/*
*********************************************************************************************************
*	�� �� ��: LCD_DispStrEx
*	����˵��: ��LCDָ�����꣨���Ͻǣ���ʾһ���ַ����� ��ǿ�ͺ�����֧����\��\�Ҷ��룬֧�ֶ���������
*	��    ��:
*		_usX : X����
*		_usY : Y����
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ�����
*		_Width : �ַ�����ʾ����Ŀ��. 0 ��ʾ�������������򣬴�ʱ_Align��Ч
*		_Align :�ַ�������ʾ����Ķ��뷽ʽ��
*				ALIGN_LEFT = 0,
*				ALIGN_CENTER = 1,
*				ALIGN_RIGHT = 2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispStrEx(uint16_t _usX, uint16_t _usY, uint8_t *_ptr, FONT_T *Font, uint16_t _Width,uint8_t _Align)
{
	uint8_t code1;
	uint8_t code2;
	uint8_t width;	
	uint8_t font_width = 0;
	uint8_t font_height = 0;
	uint16_t offset;
	uint16_t str_width;	/* �ַ���ʵ�ʿ��  */

	
	switch (Font->FontCode)
	{
		case FC_ST_12:		/* 12���� */
			font_height = 12;
			font_width = 12;
			break;
		
		case FC_ST_16:
			font_height = 16;
			font_width = 16;
			break;

		case FC_ST_24:
			font_height = 24;
			font_width = 24;
			break;
						
		case FC_ST_32:	
			font_height = 32;
			font_width = 32;
			break;					
	}
	str_width = LCD_GetStrWidth(_ptr, Font);	/* �����ַ���ʵ���ܿ�� */
	offset = 0;
	if (_Width > str_width)	/* ��ʾ�����ȴ����ַ���ʵ�ʿ�� */
	{
		if (_Align == ALIGN_RIGHT)	/* �Ҷ��� */
		{
			offset = _Width - str_width;
		}
		else if (_Align == ALIGN_CENTER)	/* ���ж��� */
		{
			offset = (_Width - str_width) / 2;
		}
		else	/* ����� ������ */
		{
			;
		}
	}
	/* ������ɫ, �м������ұ߶���  */
	if (offset > 0)
	{
		LCD_Fill_Rect(_usX, _usY, offset, font_height+_usY, Font->BackColor);
		_usX += offset;
	}
	
	/* �Ҳ����ɫ */
	if (_Width > str_width)
	{
		LCD_Fill_Rect(_usX + str_width, _usY,  _Width , font_height+_usY, Font->BackColor);
	}

	/* ��ʼѭ�������ַ� */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
		if (code1 < 0x80)
		{
			width = font_width / 2;
			LCD_ShowChar1(_usX, _usY, code1, Font);//��Ч����д�� 
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}
			LCD_ShowFont1(_usX, _usY,code1, code2,Font);
			width = font_width;
		}
//		if (Font->Space > 0)
//		{
//			/* ������ֵ�ɫ��_tFont->usBackColor�������ּ����ڵ���Ŀ�ȣ���ô��Ҫ������֮�����(��ʱδʵ��) */
//		}
//			_usX += width + _tFont->Space;	/* �е�ַ���� */
		_usX += width;
		_ptr++;			/* ָ����һ���ַ� */
	}
	
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_DispStr
*	����˵��: ��LCDָ�����꣨���Ͻǣ���ʾһ���ַ���
*	��    ��:
*		_usX : X����
*		_usY : Y����
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, uint8_t *_ptr, FONT_T *_tFont)
{
	LCD_DispStrEx(_usX, _usY, _ptr, _tFont, 0, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawLine
*	����˵��: ��2��仭һ��ֱ�ߡ�
*	��    ��:
*			_usX1, _usY1 : ��ʼ������
*			_usX2, _usY2 : ��ֹ��Y����
*			_usColor     : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x = x2-x1; //������������ 
	delta_y = y2-y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x>0)
		incx=1; //���õ������� 
	else if(delta_x == 0)
		incx=0;//��ֱ�� 
	else 
	{
		incx=-1;
		delta_x=-delta_x;
	} 
	if(delta_y>0)
		incy=1; 
	else if(delta_y==0)
		incy=0;//ˮƽ�� 
	else
	{
		incy=-1;
		delta_y=-delta_y;
	} 
	if( delta_x>delta_y)
		distance = delta_x; //ѡȡ�������������� 
	else 
		distance = delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_Fast_DrawPoint(uRow,uCol,Color);//���� 
		xerr += delta_x ; 
		yerr += delta_y ; 
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
/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawPoints
*	����˵��: ����һ��㣬������Щ�����������������ڲ�����ʾ��
*	��    ��:x, y     : ��������
*			_usSize	  �������
*			_usColor : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		LCD_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _usColor);
	}
}


/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawRect
*	����˵��: ����ˮƽ���õľ��Ρ�
*	��    ��:
*			_usX,_usY: �������Ͻǵ�����
*			_usHeight : ���εĸ߶�
*			_usWidth  : ���εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
		/*
	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	
	LCD_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _usColor);	/* �� */
	LCD_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _usColor);	/* �� */

	LCD_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _usColor);	/* �� */
	LCD_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _usColor);	/* �� */
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawCircle
*	����˵��: ����һ��Բ���ʿ�Ϊ1������
*	��    ��:
*			x0,y0  : Բ�ĵ�����
*			r  : Բ�İ뾶
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color)
{
	int a,b;
	int di;
	
	a = 0;
	b = r;	  
	di = 3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_Fast_DrawPoint(x0+a, y0-b, color);             //5  
 		LCD_Fast_DrawPoint(x0+b, y0-a, color);             //0           
		LCD_Fast_DrawPoint(x0+b, y0+a, color);             //4               
		LCD_Fast_DrawPoint(x0+a, y0+b, color);             //6 
		LCD_Fast_DrawPoint(x0-a, y0+b, color);             //1       
 		LCD_Fast_DrawPoint(x0-b, y0+a, color);             
		LCD_Fast_DrawPoint(x0-a, y0-b, color);             //2             
  		LCD_Fast_DrawPoint(x0-b, y0-a, color);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di += 10+4*(a-b);   
			b--;
		} 						    
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_Blend565
*	����˵��: ������͸���� ��ɫ��ϡ�32λ���ɫ��8λΪ͸����
*	��    ��: src : ԭʼ����
*			  dst : ��ϵ���ɫ
*			  alpha : ͸���� 0-32
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD_Blend565(uint16_t src, uint16_t dst, uint8_t alpha)
{
	uint32_t src2;
	uint32_t dst2;
	//Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
	src2 = ((src << 16) |src) & 0x07E0F81F;
	dst2 = ((dst << 16) | dst) & 0x07E0F81F;
	//Perform blending R:G:B with alpha in range 0..32
	//Note that the reason that alpha may not exceed 32 is that there are only
	//5bits of space between each R:G:B value, any higher value will overflow
	//into the next component and deliver ugly result.
	dst2 = ((((dst2 - src2) * alpha) >> 5) + src2) & 0x07E0F81F;
	return (dst2 >> 16) | dst2;
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawIcon32
*	����˵��: ��LCD�ϻ���һ��ͼ��, ����͸����Ϣ��λͼ(32λ�� RGBA). ͼ���´�����
*	��    ��: _pIcon : ͼ��ṹ
*			  _tFont : ��������
*			  _ucFocusMode : ����ģʽ��0 ��ʾ����ͼ��  1��ʾѡ�е�ͼ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawIcon32(const ICON_T *tIcon, FONT_T *tFont, uint8_t FocusMode)
{
	const uint8_t *p;
	uint16_t oldRGB, newRGB;
	uint16_t x, y;			/* ���ڼ�¼�����ڵ�������� */
	uint8_t R1, G1, B1, A;	/* ������ɫ�ʷ��� */
	uint8_t R0, G0, B0;		/* ������ɫ�ʷ��� */
	
	p = (const uint8_t *)tIcon->pBmp;
	p += 54;            /* ֱ��ָ��ͼ�������� */
	/* ����BMPλͼ���򣬴������ң���������ɨ�� */
	for (y = tIcon->Height ; y> 0; y--)
	{
        for (x=0; x<tIcon->Width; x++)
        {
 			B1 = *p++;
			G1 = *p++;
			R1 = *p++;
			A = *p++;	/* Alpha ֵ(͸����)��0-255, 0��ʾ͸����1��ʾ��͸��, �м�ֵ��ʾ͸���� */   

			if (A == 0x00)  /* ��Ҫ͸��,��ʾ���� */
			{
                ;/* ����ˢ�±��� */
			}
			else if (A == 0xFF)/* ��ȫ��͸���� ��ʾ������ */
			{
                newRGB = RGB(R1, G1, B1);
                if ( FocusMode == 1)
                {
                    newRGB = LCD_Blend565(newRGB, CL_YELLOW, 10);
                }
                LCD_Fast_DrawPoint(x+tIcon->Left, y+tIcon->Top, newRGB);
			}
			else    /* ��͸�� */
			{
                /* ���㹫ʽ�� ʵ����ʾ��ɫ = ǰ����ɫ * Alpha / 255 + ������ɫ * (255-Alpha) / 255 */
                oldRGB = LCD_ReadPoint(x+tIcon->Left, y+tIcon->Top);
                R0 = RGB565_R(oldRGB);
                G0 = RGB565_G(oldRGB);
				B0 = RGB565_B(oldRGB);

				R1 = (R1 * A) / 255 + R0 * (255 - A) / 255;
				G1 = (G1 * A) / 255 + G0 * (255 - A) / 255;
				B1 = (B1 * A) / 255 + B0 * (255 - A) / 255;
				newRGB = RGB(R1, G1, B1);
				if (FocusMode == 1)
				{
                    newRGB = LCD_Blend565(newRGB, CL_YELLOW, 10);
				}
				LCD_Fast_DrawPoint(x+tIcon->Left, y+tIcon->Top, newRGB);
			}
        }
	}
	/* ����ͼ���µ����� */
	{
        uint16_t len;
        uint16_t width;

        len = strlen(tIcon->Text);
        if (len == 0)
        {
            return;/* ���ͼ���ı�����Ϊ0������ʾ */
        }
        /* �����ı����ܿ�� */
        if(tFont->FontCode == FC_ST_12)
        {
            width = 6 * (len + tFont->Space);
        }
        else if(tFont->FontCode == FC_ST_16)
        {
            width = 8 * (len + tFont->Space);
        }
        else if(tFont->FontCode == FC_ST_24)
        {
            width = 12 * (len + tFont->Space);
        }
        	/* ˮƽ���� */
        x = (tIcon->Left + tIcon->Width/2) - width/2;
        y = tIcon->Top + tIcon->Height + 2;
        LCD_DispStr(x, y, (uint8_t *)tIcon->Text, tFont);
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawBmp32
*	����˵��: ��LCD�ϻ���һ��32λ��BMPͼ, ����͸����Ϣ��λͼ(32λ�� RGBA)
*	��    ��: usX, usY : ��ʾ����
*			  Height, usWidth : ͼƬ�߶ȺͿ��
*			  pBmp : ͼƬ���ݣ���BMP�ļ�ͷ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawBmp32(uint16_t usX, uint16_t usY, uint16_t Height, uint16_t Width, uint8_t *pBmp)
	{
	const uint8_t *p;
	uint16_t usOldRGB, usNewRGB;
	uint16_t x, y;		/* ���ڼ�¼�����ڵ�������� */
	uint8_t R1,G1,B1,A;	/* ������ɫ�ʷ��� */
	uint8_t R0,G0,B0;	/* ������ɫ�ʷ��� */

	p = (const uint8_t *)pBmp;
	p += 54;		/* ֱ��ָ��ͼ�������� */

	/* ����BMPλͼ���򣬴������ң���������ɨ�� */
	for (y = Height ; y > 0; y--)
	{
		for (x = 0; x < Width; x++)
		{
			B1 = *p++;
			G1 = *p++;
			R1 = *p++;
			A = *p++;	/* Alpha ֵ(͸����)��0-255, 0��ʾ͸����1��ʾ��͸��, �м�ֵ��ʾ͸���� */

			if (A == 0x00)	/* ��Ҫ͸��,��ʾ���� */
			{
				;	/* ����ˢ�±��� */
			}
			else if (A == 0xFF)	/* ��ȫ��͸���� ��ʾ������ */
			{
				usNewRGB = RGB(R1, G1, B1);
				//if (_ucFocusMode == 1)
				//{
				//	usNewRGB = Blend565(usNewRGB, CL_YELLOW, 10);
				//}
				LCD_Fast_DrawPoint(x + usX, y + usY, usNewRGB);
			}
			else 	/* ��͸�� */
			{
				/* ���㹫ʽ�� ʵ����ʾ��ɫ = ǰ����ɫ * Alpha / 255 + ������ɫ * (255-Alpha) / 255 */
				usOldRGB =  LCD_ReadPoint(x + usX, y + usY);
				R0 = RGB565_R(usOldRGB);
				G0 = RGB565_G(usOldRGB);
				B0 = RGB565_B(usOldRGB);

				R1 = (R1 * A) / 255 + R0 * (255 - A) / 255;
				G1 = (G1 * A) / 255 + G0 * (255 - A) / 255;
				B1 = (B1 * A) / 255 + B0 * (255 - A) / 255;
				usNewRGB = RGB(R1, G1, B1);
				//if (_ucFocusMode == 1)
				//{
				//	usNewRGB = Blend565(usNewRGB, CL_YELLOW, 10);
				//}
				LCD_Fast_DrawPoint(x + usX, y + usY, usNewRGB);
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawBMP
*	����˵��: ��LCD����ʾһ��BMPλͼ��λͼ����ɨ�����: �����ң����ϵ���
*	��    ��:
*			_usX, _usY : ͼƬ������
*			_usHeight  : ͼƬ�߶�
*			_usWidth   : ͼƬ���
*			_ptr       : ͼƬ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)











