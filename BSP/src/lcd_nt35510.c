/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : LCD_NT35510.c
*	��    �� : V1.0
*	˵    �� : NT35510 ��ʾ���ֱ���Ϊ 480 * 800,  4.3��
*	�޸ļ�¼ :
*				����             ˵��
*				11.29		LCD_DIR  ���÷���0��3��5��6
*				0������(��������) 3������(��������) 5������(���������) 6������(�������ұ�)
*********************************************************************************************************
*/	 
#include "bsp.h"

//--------------LCD�˿ڶ���--------------


////ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
////ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E			    
//#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
//#define LCD             ((LCD_TypeDef *) LCD_BASE)
//#define LCD_REG
/* 
*	ѡ�� BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ 0X60000000~0X63FFFFFF
* `	FSMC_A16 �� LCD �� DC(�Ĵ���/����ѡ��)��6 bit => FSMC[24:0]��Ӧ HADDR[25:1]
* `	�Ĵ�������ַ = 0X60000000
* `	RAM ����ַ = 0X60020000 = 0X60000000+2^6*2 = 0X60000000 + 0X20000 = 0X60020000
* `	��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼��㡣
*/
#define LCD_BASE        ((u32)(0x6C000000 | 0x00000000))
#define LCD_REG			*(__IO uint16_t *)(LCD_BASE)
#define LCD_RAM			*(__IO uint16_t *)(LCD_BASE + (1 << (6 + 1)))	/* FSMC 16λ����ģʽ�£�FSMC_A6���߶�Ӧ�����ַA7 */


//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint16_t POINT_COLOR = 0x0000;	//������ɫ
uint16_t BACK_COLOR = 0xFFFF;  	//����ɫ 
//����LCD��Ҫ����
//Ĭ��Ϊ����
//lcd ����

s_LCD_DEV		g_lcddev;	//LCD����

/*********************************************
*��������opt_delay
* ���� ����mdk -O1ʱ���Ż�ʱ��Ҫ����.��ʱ
* ���� ��
* ��� ��
*********************************************/
void opt_delay(uint8_t i)
{
	while(i--);
}

/*********************************************
*��������LCD_WR_REG()
* ���� ����LCD������оƬ��������
* ���� ��regval �������
* ��� ��
*********************************************/
void LCD_WR_REG(vu16 regval)
{
	//regval = regval;		//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD_REG = regval;	//д��Ҫд�ļĴ������	 
}

/*********************************************
*��������LCD_WR_DATA()
* ���� ��дLCD����
* ���� ��data Ҫд���ֵ
* ��� ��
*********************************************/
void LCD_WR_DATA(vu16 data)
{
	//data = data;			//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD_RAM = data;
}
/*********************************************
*��������LCD_WriteReg()
* ���� ��д�Ĵ���
* ���� ��LCD_Reg:�Ĵ�����ַ  LCD_RegValue:Ҫд�������
* ��� ��
*********************************************/
void LCD_WriteReg(vu16 LCD_Reg, vu16 LCD_RegValue)
{	
	LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	LCD_RAM = LCD_RegValue;//д������	    		 
}  

/*********************************************
*��������LCD_ReadReg()
* ���� ����ȡLCD����оƬID
* ���� ��
* ��� ��0x008000
*********************************************/
uint16_t LCD_ReadID(void)
{
	uint8_t buf[3];
	
	LCD_WR_REG(0XDA00);	
	buf[0] = LCD_RAM;//����0X00	 
	LCD_WR_REG(0XDB00);	
	buf[1] = LCD_RAM;//����0X80	
	LCD_WR_REG(0XDC00);	
	buf[2] = LCD_RAM;//����0X00	
	
	return (buf[0] << 16) + (buf[1] << 8) + buf[2];
}

/*********************************************
*��������LCD_DisplayOn(void)
* ���� ��LCD������ʾ
* ���� ��
* ��� ��
*********************************************/
void LCD_DisplayOn(void)
{
	LCD_WR_REG(0X2900);	//������ʾ
}

/*********************************************
*��������LCD_DisplayOff(void)
* ���� ��LCD�ر���ʾ
* ���� ��
* ��� ��
*********************************************/
void LCD_DisplayOff(void)
{
	LCD_WR_REG(0X2800);	//�ر���ʾ
} 

/*
*********************************************************************************************************
*	�� �� ��: LCD_SetDispWin
*	����˵��: ������ʾ���ڣ����봰����ʾģʽ��TFT����оƬ֧�ִ�����ʾģʽ�����Ǻ�һ���12864����LCD���������
*	��    ��:
*		_usX : ˮƽ����
*		_usY : ��ֱ����
*		_usHeight: ���ڸ߶�
*		_usWidth : ���ڿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{
	LCD_WR_REG(0X2A00); 							/* ����X���� */
	LCD_WR_DATA(_usX >> 8);							/* ��ʼ�� */
	LCD_WR_REG(0x2A01);
	LCD_WR_DATA(_usX);
	LCD_WR_REG(0x2A02);
	LCD_WR_DATA((_usX + _usWidth - 1) >> 8);		/* ������ */
	LCD_WR_REG(0x2A03);
	LCD_WR_DATA(_usX + _usWidth - 1);

	LCD_WR_REG(0X2B00); 				 			/* ����Y����*/	
	LCD_WR_DATA(_usY >> 8);   						/* ��ʼ�� */
	LCD_WR_REG(0x2B01);
	LCD_WR_DATA(_usY);
	LCD_WR_REG(0x2B02);
	LCD_WR_DATA((_usY + _usHeight - 1) >>8);		/* ������ */
	LCD_WR_REG(0x2B03);
	LCD_WR_DATA((_usY + _usHeight - 1));
}
/*
*********************************************************************************************************
*	�� �� ��: ILI9488_QuitWinMode
*	����˵��: �˳�������ʾģʽ����Ϊȫ����ʾģʽ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_QuitWinMode(void)
{
	LCD_SetDispWin(0, 0, g_lcddev.height, g_lcddev.width);
}

/*********************************************
*��������LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
* ���� �����ù��λ�ã����ô��ڵ��������
* ���� ��Xpos�������� Ypos ������
* ��� ��
*********************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_WR_REG(0x2A00);
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_REG(0x2A01);
	LCD_WR_DATA(Xpos&0xFF);
//	LCD_WR_REG(0x2A02);		/*���ϻỨ��*/	
//	LCD_WR_DATA(Xpos>>8);
//	LCD_WR_REG(0x2A03);
//	LCD_WR_DATA(Xpos&0xFF);
	
	LCD_WR_REG(0x2B00);
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_REG(0x2B01);
	LCD_WR_DATA(Ypos&0xFF);	
//	LCD_WR_REG(0x2B02);
//	LCD_WR_DATA(Ypos>>8);
//	LCD_WR_REG(0x2B03);
//	LCD_WR_DATA(Ypos&0xFF);	
}

/*********************************************
*��������LCD_DrawPoint(uint16_t x,uint16_t y)
* ���� ������
* ���� ��x,y����
* ��� ��
*********************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WR_REG(0x2C00);	//��ʼдGRAM
	LCD_WR_DATA(POINT_COLOR);
}

/*********************************************
*��������LCD_Fast_DrawPoint(uint16_t x,uint16_t y, uint16_t color)
* ���� �����ٻ��㣬����Ҫ��������
* ���� ��x,y���꣬color ��ɫ
* ��� ��
*********************************************/
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_WR_REG(0x2A00);
	LCD_WR_DATA(x>>8);  
	LCD_WR_REG(0x2A01);
	LCD_WR_DATA(x&0XFF);	  
	LCD_WR_REG(0x2B00);
	LCD_WR_DATA(y>>8);  
	LCD_WR_REG(0x2B01);
	LCD_WR_DATA(y&0XFF); 	
	LCD_WR_REG(0x2C00); 
	LCD_WR_DATA(color); 
}
/*********************************************
*��������LCD_ReadPoint(uint16_t x,uint16_t y)
* ���� ����ȡ��ĳ�����ɫֵ	 
* ���� ��x,y����
* ��� ���˵���ɫ
*********************************************/
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
	vu16 r = 0, g = 0, b = 0;
	
	if (x >= g_lcddev.width || y >= g_lcddev.height)
		return 0;
	LCD_SetCursor(x, y);
	LCD_WR_REG(0x2E00);		//5510 ���Ͷ�GRAMָ��

	r = LCD_RAM;			//dummy Read	
	opt_delay(2);
	r = LCD_RAM;			//ʵ��������ɫ			
	opt_delay(2);	  
	b = LCD_RAM; 
	g = r&0XFF;				//����5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
	g <<= 8;	 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//NT35510��Ҫ��ʽת��һ��

}	

/*
*********************************************************************************************************
*	�� �� ��: ILI9488_SetDirection
*	����˵��: ������ʾ����
*	��    ��:  _ucDir : ��ʾ������� 0 ��������, 1=����180�ȷ�ת, 2=����, 3=����180�ȷ�ת
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if 0 	//LCD_Scan_Dir��ͬ����
void LCD_SetDirection(uint8_t _ucDir)
{
	
	/*
		Memory Access Control (3600h)
		This command defines read/write scanning direction of the frame memory.

		These 3 bits control the direction from the MPU to memory write/read.

		Bit  Symbol  Name  Description
		D7   MY  Row Address Order
		D6   MX  Column Address Order
		D5   MV  Row/Column Exchange
		D4   ML  Vertical Refresh Order  LCD vertical refresh direction control. ��

		D3   BGR RGB-BGR Order   Color selector switch control
		     (0 = RGB color filter panel, 1 = BGR color filter panel )
		D2   MH  Horizontal Refresh ORDER  LCD horizontal refreshing direction control.
		D1   RSMX Flip Horizontal
		D0   Flip Vertical
	*/
	LCD_WR_REG(0x3600);	
	/* 0 ��ʾ����(��������)��1��ʾ����(��������), 2��ʾ����(���������)  3��ʾ���� (�������ұ�) */
	if (_ucDir == 0)
	{
		LCD_WR_DATA(0xA8);	/* ����(���������) */
		g_lcddev.width = 480;
		g_lcddev.height= 800;
	}
	else if (_ucDir == 1)
	{
		LCD_WR_DATA(0x68);	/* ���� (�������ұ�) */
		g_lcddev.width = 480;
		g_lcddev.height= 800;
	}
	else if (_ucDir == 2)
	{
		LCD_WR_DATA(0xC8);	/* ����(��������) */
		g_lcddev.width = 800;
		g_lcddev.height= 480;
	}
	else if (_ucDir == 3)
	{
		LCD_WR_DATA(0x08);	/* ����(��������) */
		g_lcddev.width = 800;
		g_lcddev.height= 480;
	}
}
#endif
/*********************************************
*��������void LCD_Scan_Dir(uint8_t dir)
* ���� ������LCD���Զ�ɨ�跽��
* ���� ��dir:0~7,����8������(���嶨���lcd.h)������0��3��5��6
* ��� ��
*********************************************/
void bsp_LCDDisplayDir(uint8_t dir)
{
	uint16_t regval=0;
	
	switch(dir)
	{
		case L2R_U2D://������,���ϵ���	0x08	 //�������� ���K  --> ���ã�����1������(��������)
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://������,���µ��� 0x88	 //�������� ���J  --> ����180����ת�پ��񡾾���Y��
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://���ҵ���,���ϵ��� 0x48	 //�������� ���L  --> ��������������X��
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;		
		case R2L_D2U://���ҵ���,���µ��� 0xc8	//�������� ���I  --> ���ã�����2������180����ת�󡾾���Y������X��  ����(��������)
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		
		case U2D_L2R://���ϵ���,������ 0x28	//�������� ���K  --> ������ʱ��90���پ�����ת����������Y��
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://���ϵ���,���ҵ��� 0x68	//�������� ���L  --> ���ã�����1������(���������)
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://���µ���,������ 0xa8	//�������� ���J  --> ���ã�����2������(�������ұ�)
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://���µ���,���ҵ��� 0xe8	//�������� ���I  --> ����˳ʱ��90���پ�����ת����������X��
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	} 	
	LCD_WR_REG(0X3600);	LCD_WR_DATA(regval);	
	if(regval&0X20)	//����
	{		
		g_lcddev.width = 800;
		g_lcddev.height= 480;		
	}
	else  			//����
	{
		g_lcddev.width = 480;
		g_lcddev.height= 800;		
	} 
	g_lcddev.direction = dir;
	/*������ʾ����*/
	LCD_WR_REG(0x2A00);LCD_WR_DATA(0); 
	LCD_WR_REG(0x2A01);LCD_WR_DATA(0); 
	LCD_WR_REG(0x2A02);LCD_WR_DATA((g_lcddev.width-1)>>8); 
	LCD_WR_REG(0x2A03);LCD_WR_DATA((g_lcddev.width-1)&0XFF); 
	LCD_WR_REG(0x2B00);LCD_WR_DATA(0); 
	LCD_WR_REG(0x2B01);LCD_WR_DATA(0); 
	LCD_WR_REG(0x2B02);LCD_WR_DATA((g_lcddev.height-1)>>8); 
	LCD_WR_REG(0x2B03);LCD_WR_DATA((g_lcddev.height-1)&0XFF);
}


/*********************************************
*�������� LCD_Clear(uint16_t color)
* ���� �������������ɫֵ����
* ���� ��/color:Ҫ���������ɫ
* ��� ��
*********************************************/
void LCD_Clear(uint16_t color)
{
	u32 i=0;      
	u32 n;
	
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WR_REG(0x2C00);     		//��ʼд��GRAM	 
#if 1 /* �Ż�����ִ���ٶ� */
	n = (g_lcddev.width * g_lcddev.height)/8;
	for (i = 0; i < n; i++)
	{
		LCD_RAM = color;
		LCD_RAM = color;
		LCD_RAM = color;
		LCD_RAM = color;
		
		LCD_RAM = color;
		LCD_RAM = color;
		LCD_RAM = color;
		LCD_RAM = color;
		
	}
#else	
	n = g_lcddev.width * g_lcddev.height;	//�ܵ���
	for (i = 0; i < n; i++)	
	{
		LCD_RAM = color;	
	}
#endif
}

/*********************************************
*�������� LCD_Fill_Rect()
* ���� ����һ����ɫֵ���һ�����Ρ�
* ���� ��/color:Ҫ������ɫ, (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
* ��� ��
*********************************************/
void LCD_Fill_Rect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	/* ÿ�ζ����õ�ַ*/
#if 0
	uint16_t i,j;
	uint16_t xlen = 0;
	
	xlen = ex-sx+1;	
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WR_REG(0x2C00);     			//��ʼд��GRAM	
		for(j=0;j<xlen;j++)
		{
			LCD_RAM = color;	//��ʾ��ɫ 	
		}			
	}
	/* ����оƬַ������������ʾ*/
#else
	u32 i,total;
	LCD_SetDispWin(sx, sy,ey-sy+1, ex-sx+1);
	LCD_SetCursor(sx, sy);      				//���ù��λ�� 
	LCD_WR_REG(0x2C00);     			//��ʼд��GRAM	
	total = (ex-sx+1)*(ey-sy+1);
	for (i=0; i<total; i++)
	{
		LCD_RAM = color;	//��ʾ��ɫ 	
	}
	
#endif
}

/*********************************************
*�������� LCD_Color_Fill()
* ���� ����ָ���������������ɫ
* ���� ��/color:Ҫ������ɫ, (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
* ��� ��
*********************************************/
void LCD_Fill_ColorRect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WR_REG(0x2C00);     //��ʼд��GRAM
		for(j=0;j<width;j++)
		{
			LCD_RAM = color[i*width+j];//д������
		}			 
	}		  
} 

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawImage
*	����˵��: ��LCD����ʾһ��λͼ��λͼ��Image2Lcd���ɣ�����ɨ�����: �����ң����ϵ���
*	��    ��:
*			x0, y0 : ͼƬ������
*			Height  : ͼƬ�߶�
*			Width   : ͼƬ���
*			_ptr       : ͼƬ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawImage(uint16_t x0, uint16_t y0, uint16_t height, uint16_t width, uint16_t *ptr)
{
	u32 i,total;
	
	LCD_SetDispWin(x0, y0, height, width);
	LCD_SetCursor(x0, y0);      				//���ù��λ�� 
	LCD_WR_REG(0x2C00);     //��ʼд��GRAM
	total = height * width;
	for (i=0; i<total; i++)
	{
		LCD_RAM = *ptr++;	//��ʾ��ɫ 	
	}

}

/*********************************************
*��������LCD_Init()
* ���� ��LCD ��ʼ�� 
* ���� ��
* ��� ��
*********************************************/
void Init_NT35510(void)
{ 	
	
	bsp_DelayMS(1); // delay 1 ms 
 	LCD_WriteReg(0x0000,0x0001);
	bsp_DelayMS(5); // delay 5 ms  
//	
	LCD_WriteReg(0xF000,0x55);
	LCD_WriteReg(0xF001,0xAA);
	LCD_WriteReg(0xF002,0x52);
	LCD_WriteReg(0xF003,0x08);
	LCD_WriteReg(0xF004,0x01);
	//AVDD Set AVDD 5.2V
	LCD_WriteReg(0xB000,0x0D);
	LCD_WriteReg(0xB001,0x0D);
	LCD_WriteReg(0xB002,0x0D);
	//AVDD ratio
	LCD_WriteReg(0xB600,0x34);
	LCD_WriteReg(0xB601,0x34);
	LCD_WriteReg(0xB602,0x34);
	//AVEE -5.2V
	LCD_WriteReg(0xB100,0x0D);
	LCD_WriteReg(0xB101,0x0D);
	LCD_WriteReg(0xB102,0x0D);
	//AVEE ratio
	LCD_WriteReg(0xB700,0x34);
	LCD_WriteReg(0xB701,0x34);
	LCD_WriteReg(0xB702,0x34);
	//VCL -2.5V
	LCD_WriteReg(0xB200,0x00);
	LCD_WriteReg(0xB201,0x00);
	LCD_WriteReg(0xB202,0x00);
	//VCL ratio
	LCD_WriteReg(0xB800,0x24);
	LCD_WriteReg(0xB801,0x24);
	LCD_WriteReg(0xB802,0x24);
	//VGH 15V (Free pump)
	LCD_WriteReg(0xBF00,0x01);
	LCD_WriteReg(0xB300,0x0F);
	LCD_WriteReg(0xB301,0x0F);
	LCD_WriteReg(0xB302,0x0F);
	//VGH ratio
	LCD_WriteReg(0xB900,0x34);
	LCD_WriteReg(0xB901,0x34);
	LCD_WriteReg(0xB902,0x34);
	//VGL_REG -10V
	LCD_WriteReg(0xB500,0x08);
	LCD_WriteReg(0xB501,0x08);
	LCD_WriteReg(0xB502,0x08);
	LCD_WriteReg(0xC200,0x03);
	//VGLX ratio
	LCD_WriteReg(0xBA00,0x24);
	LCD_WriteReg(0xBA01,0x24);
	LCD_WriteReg(0xBA02,0x24);
	//VGMP/VGSP 4.5V/0V
	LCD_WriteReg(0xBC00,0x00);
	LCD_WriteReg(0xBC01,0x78);
	LCD_WriteReg(0xBC02,0x00);
	//VGMN/VGSN -4.5V/0V
	LCD_WriteReg(0xBD00,0x00);
	LCD_WriteReg(0xBD01,0x78);
	LCD_WriteReg(0xBD02,0x00);
	//VCOM
	LCD_WriteReg(0xBE00,0x00);
	LCD_WriteReg(0xBE01,0x64);
	//Gamma Setting
	LCD_WriteReg(0xD100,0x00);
	LCD_WriteReg(0xD101,0x33);
	LCD_WriteReg(0xD102,0x00);
	LCD_WriteReg(0xD103,0x34);
	LCD_WriteReg(0xD104,0x00);
	LCD_WriteReg(0xD105,0x3A);
	LCD_WriteReg(0xD106,0x00);
	LCD_WriteReg(0xD107,0x4A);
	LCD_WriteReg(0xD108,0x00);
	LCD_WriteReg(0xD109,0x5C);
	LCD_WriteReg(0xD10A,0x00);
	LCD_WriteReg(0xD10B,0x81);
	LCD_WriteReg(0xD10C,0x00);
	LCD_WriteReg(0xD10D,0xA6);
	LCD_WriteReg(0xD10E,0x00);
	LCD_WriteReg(0xD10F,0xE5);
	LCD_WriteReg(0xD110,0x01);
	LCD_WriteReg(0xD111,0x13);
	LCD_WriteReg(0xD112,0x01);
	LCD_WriteReg(0xD113,0x54);
	LCD_WriteReg(0xD114,0x01);
	LCD_WriteReg(0xD115,0x82);
	LCD_WriteReg(0xD116,0x01);
	LCD_WriteReg(0xD117,0xCA);
	LCD_WriteReg(0xD118,0x02);
	LCD_WriteReg(0xD119,0x00);
	LCD_WriteReg(0xD11A,0x02);
	LCD_WriteReg(0xD11B,0x01);
	LCD_WriteReg(0xD11C,0x02);
	LCD_WriteReg(0xD11D,0x34);
	LCD_WriteReg(0xD11E,0x02);
	LCD_WriteReg(0xD11F,0x67);
	LCD_WriteReg(0xD120,0x02);
	LCD_WriteReg(0xD121,0x84);
	LCD_WriteReg(0xD122,0x02);
	LCD_WriteReg(0xD123,0xA4);
	LCD_WriteReg(0xD124,0x02);
	LCD_WriteReg(0xD125,0xB7);
	LCD_WriteReg(0xD126,0x02);
	LCD_WriteReg(0xD127,0xCF);
	LCD_WriteReg(0xD128,0x02);
	LCD_WriteReg(0xD129,0xDE);
	LCD_WriteReg(0xD12A,0x02);
	LCD_WriteReg(0xD12B,0xF2);
	LCD_WriteReg(0xD12C,0x02);
	LCD_WriteReg(0xD12D,0xFE);
	LCD_WriteReg(0xD12E,0x03);
	LCD_WriteReg(0xD12F,0x10);
	LCD_WriteReg(0xD130,0x03);
	LCD_WriteReg(0xD131,0x33);
	LCD_WriteReg(0xD132,0x03);
	LCD_WriteReg(0xD133,0x6D);
	LCD_WriteReg(0xD200,0x00);
	LCD_WriteReg(0xD201,0x33);
	LCD_WriteReg(0xD202,0x00);
	LCD_WriteReg(0xD203,0x34);
	LCD_WriteReg(0xD204,0x00);
	LCD_WriteReg(0xD205,0x3A);
	LCD_WriteReg(0xD206,0x00);
	LCD_WriteReg(0xD207,0x4A);
	LCD_WriteReg(0xD208,0x00);
	LCD_WriteReg(0xD209,0x5C);
	LCD_WriteReg(0xD20A,0x00);

	LCD_WriteReg(0xD20B,0x81);
	LCD_WriteReg(0xD20C,0x00);
	LCD_WriteReg(0xD20D,0xA6);
	LCD_WriteReg(0xD20E,0x00);
	LCD_WriteReg(0xD20F,0xE5);
	LCD_WriteReg(0xD210,0x01);
	LCD_WriteReg(0xD211,0x13);
	LCD_WriteReg(0xD212,0x01);
	LCD_WriteReg(0xD213,0x54);
	LCD_WriteReg(0xD214,0x01);
	LCD_WriteReg(0xD215,0x82);
	LCD_WriteReg(0xD216,0x01);
	LCD_WriteReg(0xD217,0xCA);
	LCD_WriteReg(0xD218,0x02);
	LCD_WriteReg(0xD219,0x00);
	LCD_WriteReg(0xD21A,0x02);
	LCD_WriteReg(0xD21B,0x01);
	LCD_WriteReg(0xD21C,0x02);
	LCD_WriteReg(0xD21D,0x34);
	LCD_WriteReg(0xD21E,0x02);
	LCD_WriteReg(0xD21F,0x67);
	LCD_WriteReg(0xD220,0x02);
	LCD_WriteReg(0xD221,0x84);
	LCD_WriteReg(0xD222,0x02);
	LCD_WriteReg(0xD223,0xA4);
	LCD_WriteReg(0xD224,0x02);
	LCD_WriteReg(0xD225,0xB7);
	LCD_WriteReg(0xD226,0x02);
	LCD_WriteReg(0xD227,0xCF);
	LCD_WriteReg(0xD228,0x02);
	LCD_WriteReg(0xD229,0xDE);
	LCD_WriteReg(0xD22A,0x02);
	LCD_WriteReg(0xD22B,0xF2);
	LCD_WriteReg(0xD22C,0x02);
	LCD_WriteReg(0xD22D,0xFE);
	LCD_WriteReg(0xD22E,0x03);
	LCD_WriteReg(0xD22F,0x10);
	LCD_WriteReg(0xD230,0x03);
	LCD_WriteReg(0xD231,0x33);
	LCD_WriteReg(0xD232,0x03);
	LCD_WriteReg(0xD233,0x6D);
	LCD_WriteReg(0xD300,0x00);
	LCD_WriteReg(0xD301,0x33);
	LCD_WriteReg(0xD302,0x00);
	LCD_WriteReg(0xD303,0x34);
	LCD_WriteReg(0xD304,0x00);
	LCD_WriteReg(0xD305,0x3A);
	LCD_WriteReg(0xD306,0x00);
	LCD_WriteReg(0xD307,0x4A);
	LCD_WriteReg(0xD308,0x00);
	LCD_WriteReg(0xD309,0x5C);
	LCD_WriteReg(0xD30A,0x00);

	LCD_WriteReg(0xD30B,0x81);
	LCD_WriteReg(0xD30C,0x00);
	LCD_WriteReg(0xD30D,0xA6);
	LCD_WriteReg(0xD30E,0x00);
	LCD_WriteReg(0xD30F,0xE5);
	LCD_WriteReg(0xD310,0x01);
	LCD_WriteReg(0xD311,0x13);
	LCD_WriteReg(0xD312,0x01);
	LCD_WriteReg(0xD313,0x54);
	LCD_WriteReg(0xD314,0x01);
	LCD_WriteReg(0xD315,0x82);
	LCD_WriteReg(0xD316,0x01);
	LCD_WriteReg(0xD317,0xCA);
	LCD_WriteReg(0xD318,0x02);
	LCD_WriteReg(0xD319,0x00);
	LCD_WriteReg(0xD31A,0x02);
	LCD_WriteReg(0xD31B,0x01);
	LCD_WriteReg(0xD31C,0x02);
	LCD_WriteReg(0xD31D,0x34);
	LCD_WriteReg(0xD31E,0x02);
	LCD_WriteReg(0xD31F,0x67);
	LCD_WriteReg(0xD320,0x02);
	LCD_WriteReg(0xD321,0x84);
	LCD_WriteReg(0xD322,0x02);
	LCD_WriteReg(0xD323,0xA4);
	LCD_WriteReg(0xD324,0x02);
	LCD_WriteReg(0xD325,0xB7);
	LCD_WriteReg(0xD326,0x02);
	LCD_WriteReg(0xD327,0xCF);
	LCD_WriteReg(0xD328,0x02);
	LCD_WriteReg(0xD329,0xDE);
	LCD_WriteReg(0xD32A,0x02);
	LCD_WriteReg(0xD32B,0xF2);
	LCD_WriteReg(0xD32C,0x02);
	LCD_WriteReg(0xD32D,0xFE);
	LCD_WriteReg(0xD32E,0x03);
	LCD_WriteReg(0xD32F,0x10);
	LCD_WriteReg(0xD330,0x03);
	LCD_WriteReg(0xD331,0x33);
	LCD_WriteReg(0xD332,0x03);
	LCD_WriteReg(0xD333,0x6D);
	LCD_WriteReg(0xD400,0x00);
	LCD_WriteReg(0xD401,0x33);
	LCD_WriteReg(0xD402,0x00);
	LCD_WriteReg(0xD403,0x34);
	LCD_WriteReg(0xD404,0x00);
	LCD_WriteReg(0xD405,0x3A);
	LCD_WriteReg(0xD406,0x00);
	LCD_WriteReg(0xD407,0x4A);
	LCD_WriteReg(0xD408,0x00);
	LCD_WriteReg(0xD409,0x5C);
	LCD_WriteReg(0xD40A,0x00);
	LCD_WriteReg(0xD40B,0x81);

	LCD_WriteReg(0xD40C,0x00);
	LCD_WriteReg(0xD40D,0xA6);
	LCD_WriteReg(0xD40E,0x00);
	LCD_WriteReg(0xD40F,0xE5);
	LCD_WriteReg(0xD410,0x01);
	LCD_WriteReg(0xD411,0x13);
	LCD_WriteReg(0xD412,0x01);
	LCD_WriteReg(0xD413,0x54);
	LCD_WriteReg(0xD414,0x01);
	LCD_WriteReg(0xD415,0x82);
	LCD_WriteReg(0xD416,0x01);
	LCD_WriteReg(0xD417,0xCA);
	LCD_WriteReg(0xD418,0x02);
	LCD_WriteReg(0xD419,0x00);
	LCD_WriteReg(0xD41A,0x02);
	LCD_WriteReg(0xD41B,0x01);
	LCD_WriteReg(0xD41C,0x02);
	LCD_WriteReg(0xD41D,0x34);
	LCD_WriteReg(0xD41E,0x02);
	LCD_WriteReg(0xD41F,0x67);
	LCD_WriteReg(0xD420,0x02);
	LCD_WriteReg(0xD421,0x84);
	LCD_WriteReg(0xD422,0x02);
	LCD_WriteReg(0xD423,0xA4);
	LCD_WriteReg(0xD424,0x02);
	LCD_WriteReg(0xD425,0xB7);
	LCD_WriteReg(0xD426,0x02);
	LCD_WriteReg(0xD427,0xCF);
	LCD_WriteReg(0xD428,0x02);
	LCD_WriteReg(0xD429,0xDE);
	LCD_WriteReg(0xD42A,0x02);
	LCD_WriteReg(0xD42B,0xF2);
	LCD_WriteReg(0xD42C,0x02);
	LCD_WriteReg(0xD42D,0xFE);
	LCD_WriteReg(0xD42E,0x03);
	LCD_WriteReg(0xD42F,0x10);
	LCD_WriteReg(0xD430,0x03);
	LCD_WriteReg(0xD431,0x33);
	LCD_WriteReg(0xD432,0x03);
	LCD_WriteReg(0xD433,0x6D);
	LCD_WriteReg(0xD500,0x00);
	LCD_WriteReg(0xD501,0x33);
	LCD_WriteReg(0xD502,0x00);
	LCD_WriteReg(0xD503,0x34);
	LCD_WriteReg(0xD504,0x00);
	LCD_WriteReg(0xD505,0x3A);
	LCD_WriteReg(0xD506,0x00);
	LCD_WriteReg(0xD507,0x4A);
	LCD_WriteReg(0xD508,0x00);
	LCD_WriteReg(0xD509,0x5C);
	LCD_WriteReg(0xD50A,0x00);
	LCD_WriteReg(0xD50B,0x81);

	LCD_WriteReg(0xD50C,0x00);
	LCD_WriteReg(0xD50D,0xA6);
	LCD_WriteReg(0xD50E,0x00);
	LCD_WriteReg(0xD50F,0xE5);
	LCD_WriteReg(0xD510,0x01);
	LCD_WriteReg(0xD511,0x13);
	LCD_WriteReg(0xD512,0x01);
	LCD_WriteReg(0xD513,0x54);
	LCD_WriteReg(0xD514,0x01);
	LCD_WriteReg(0xD515,0x82);
	LCD_WriteReg(0xD516,0x01);
	LCD_WriteReg(0xD517,0xCA);
	LCD_WriteReg(0xD518,0x02);
	LCD_WriteReg(0xD519,0x00);
	LCD_WriteReg(0xD51A,0x02);
	LCD_WriteReg(0xD51B,0x01);
	LCD_WriteReg(0xD51C,0x02);
	LCD_WriteReg(0xD51D,0x34);
	LCD_WriteReg(0xD51E,0x02);
	LCD_WriteReg(0xD51F,0x67);
	LCD_WriteReg(0xD520,0x02);
	LCD_WriteReg(0xD521,0x84);
	LCD_WriteReg(0xD522,0x02);
	LCD_WriteReg(0xD523,0xA4);
	LCD_WriteReg(0xD524,0x02);
	LCD_WriteReg(0xD525,0xB7);
	LCD_WriteReg(0xD526,0x02);
	LCD_WriteReg(0xD527,0xCF);
	LCD_WriteReg(0xD528,0x02);
	LCD_WriteReg(0xD529,0xDE);
	LCD_WriteReg(0xD52A,0x02);
	LCD_WriteReg(0xD52B,0xF2);
	LCD_WriteReg(0xD52C,0x02);
	LCD_WriteReg(0xD52D,0xFE);
	LCD_WriteReg(0xD52E,0x03);
	LCD_WriteReg(0xD52F,0x10);
	LCD_WriteReg(0xD530,0x03);
	LCD_WriteReg(0xD531,0x33);
	LCD_WriteReg(0xD532,0x03);
	LCD_WriteReg(0xD533,0x6D);
	LCD_WriteReg(0xD600,0x00);
	LCD_WriteReg(0xD601,0x33);
	LCD_WriteReg(0xD602,0x00);
	LCD_WriteReg(0xD603,0x34);
	LCD_WriteReg(0xD604,0x00);
	LCD_WriteReg(0xD605,0x3A);
	LCD_WriteReg(0xD606,0x00);
	LCD_WriteReg(0xD607,0x4A);
	LCD_WriteReg(0xD608,0x00);
	LCD_WriteReg(0xD609,0x5C);
	LCD_WriteReg(0xD60A,0x00);
	LCD_WriteReg(0xD60B,0x81);

	LCD_WriteReg(0xD60C,0x00);
	LCD_WriteReg(0xD60D,0xA6);
	LCD_WriteReg(0xD60E,0x00);
	LCD_WriteReg(0xD60F,0xE5);
	LCD_WriteReg(0xD610,0x01);
	LCD_WriteReg(0xD611,0x13);
	LCD_WriteReg(0xD612,0x01);
	LCD_WriteReg(0xD613,0x54);
	LCD_WriteReg(0xD614,0x01);
	LCD_WriteReg(0xD615,0x82);
	LCD_WriteReg(0xD616,0x01);
	LCD_WriteReg(0xD617,0xCA);
	LCD_WriteReg(0xD618,0x02);
	LCD_WriteReg(0xD619,0x00);
	LCD_WriteReg(0xD61A,0x02);
	LCD_WriteReg(0xD61B,0x01);
	LCD_WriteReg(0xD61C,0x02);
	LCD_WriteReg(0xD61D,0x34);
	LCD_WriteReg(0xD61E,0x02);
	LCD_WriteReg(0xD61F,0x67);
	LCD_WriteReg(0xD620,0x02);
	LCD_WriteReg(0xD621,0x84);
	LCD_WriteReg(0xD622,0x02);
	LCD_WriteReg(0xD623,0xA4);
	LCD_WriteReg(0xD624,0x02);
	LCD_WriteReg(0xD625,0xB7);
	LCD_WriteReg(0xD626,0x02);
	LCD_WriteReg(0xD627,0xCF);
	LCD_WriteReg(0xD628,0x02);
	LCD_WriteReg(0xD629,0xDE);
	LCD_WriteReg(0xD62A,0x02);
	LCD_WriteReg(0xD62B,0xF2);
	LCD_WriteReg(0xD62C,0x02);
	LCD_WriteReg(0xD62D,0xFE);
	LCD_WriteReg(0xD62E,0x03);
	LCD_WriteReg(0xD62F,0x10);
	LCD_WriteReg(0xD630,0x03);
	LCD_WriteReg(0xD631,0x33);
	LCD_WriteReg(0xD632,0x03);
	LCD_WriteReg(0xD633,0x6D);
	//LV2 Page 0 enable
	LCD_WriteReg(0xF000,0x55);
	LCD_WriteReg(0xF001,0xAA);
	LCD_WriteReg(0xF002,0x52);
	LCD_WriteReg(0xF003,0x08);
	LCD_WriteReg(0xF004,0x00);
	//Display control
	LCD_WriteReg(0xB100, 0xCC);
	LCD_WriteReg(0xB101, 0x00);
	//Source hold time
	LCD_WriteReg(0xB600,0x05);
	//Gate EQ control
	LCD_WriteReg(0xB700,0x70);
	LCD_WriteReg(0xB701,0x70);
	//Source EQ control (Mode 2)
	LCD_WriteReg(0xB800,0x01);
	LCD_WriteReg(0xB801,0x03);
	LCD_WriteReg(0xB802,0x03);
	LCD_WriteReg(0xB803,0x03);
	//Inversion mode (2-dot)
	LCD_WriteReg(0xBC00,0x02);
	LCD_WriteReg(0xBC01,0x00);
	LCD_WriteReg(0xBC02,0x00);
	//Timing control 4H w/ 4-delay
	LCD_WriteReg(0xC900,0xD0);
	LCD_WriteReg(0xC901,0x02);
	LCD_WriteReg(0xC902,0x50);
	LCD_WriteReg(0xC903,0x50);
	LCD_WriteReg(0xC904,0x50);
	LCD_WriteReg(0x3500,0x00);
	LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
	LCD_WR_REG(0x1100);
	bsp_DelayUS(120);
	LCD_WR_REG(0x2900);
} 
