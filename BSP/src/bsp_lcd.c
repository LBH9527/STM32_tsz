/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : bsp_tft_lcd.c
*	版    本 : V4.2
*	说    明 : 
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库版本 V3.5.0版本。

*
*********************************************************************************************************
*/
#include "bsp.h"
#include "lcd_nt35510.h"
#include "fonts.h"

static void _LCD_ReadAsciiDot(uint8_t code, uint8_t fontcode, uint8_t *pBuf);
void _LCD_ReadHZDot(uint8_t _code1, uint8_t _code2,  uint8_t _fontcode, uint8_t *_pBuf);
/********************************************************************************************************
*	函 数 名: LCD_CtrlLinesConfig
*	功能说明: 配置LCD控制口线，FSMC管脚设置为复用功能
*	形    参: 无
*	返 回 值: 无
********************************************************************************************************/
void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//使能PD,PE,PF,PG时钟  	
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PB15 推挽输出,控制背光
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 //PB15 推挽输出,控制背光
	
	GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(7<<8)|(3<<14);//PD0,1,4,5,8,9,10,14,15 AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化  

	GPIO_InitStructure.GPIO_Pin = (0X1FF<<7);//PE7~15,AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化 

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
*	函 数 名: LCD_FSMCConfig
*	功能说明: 配置FSMC并口访问时序
*	形    参: 无
*	返 回 值: 无
********************************************************************************************************/
void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
	 RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟 
	
	readWriteTiming.FSMC_AddressSetupTime = 0XF;	 //地址建立时间（ADDSET）为16个HCLK 1/168M=6ns*16=96ns	
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
	readWriteTiming.FSMC_DataSetupTime = 60;			//数据保存时间为60个HCLK	=6*60=360ns
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
	
	writeTiming.FSMC_AddressSetupTime = 3;	      //地址建立时间（ADDSET）为3个HCLK =18ns 
	writeTiming.FSMC_AddressHoldTime = 0x00;
	writeTiming.FSMC_DataSetupTime = 2;				//数据保存时间为6ns*(2+1)个HCLK=18ns
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A   	
		
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;	//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	// 不复用数据地址
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;	//SRAM   	
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//存储器数据宽度为16bit  
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;//读写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;	//写时序
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置
	
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);	// 使能BANK1第四区 		
}

/*******************************************************************************
* 函数名  :  LCD_BacklightInit
* 描述    ： 背光调节
* 输入参数： level 范围0-10， 10最亮 0最暗
* 返回参数： 无
*******************************************************************************/
void bsp_LCDBackLightSet(uint8_t level)
{
	TIM_SetCompare2(TIM12,level*1000);
}
/*******************************************************************************
* 函数名  :  LCD_BacklightInit
* 描述    ： 背光函数初始化
* 输入参数： 1 背景亮度可调，0背景亮度不可调
* 返回参数： 无
*******************************************************************************/
void LCD_BacklightInit(void )
{
	if (LCD_BACKLIGHT_SET)
	{
		bsp_InitBacklightSetTimer();	//定时器时钟为 84M，分频系数为 84，所以计数频率
										//为 84M/84=1Mhz,重装载值 10000，所以 PWM 频率为 1M/10000=100hz.
	}		
	else
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PB15 推挽输出,控制背光
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 //PB15 推挽输出,控制背光
	}							
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitLCD
*	功能说明: 初始化LCD
*	形    参: _backlightSet：0背光亮度不可调，1背光亮度可调
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLCD(void)
{
	/* 配置LCD控制口线GPIO */
	LCD_CtrlLinesConfig();
	/* 配置FSMC接口，数据总线 */
	LCD_FSMCConfig();
	/* 驱动初始化 */
	Init_NT35510();
	/* 背光设置初始化 */
	LCD_BacklightInit();
	if (LCD_BACKLIGHT_SET)			
		bsp_LCDBackLightSet(LCD_BACKLIGHT_DEFAULT);
	else
		LCD_LED = 1;					//点亮背光	
	/* 屏幕设置 */
	bsp_LCDDisplayDir(LCD_DIR_DEFAULT);	//默认为竖屏（排针在上）

	/* 清屏 */
	LCD_Clear(CL_WHITE);
}
/*
*********************************************************************************************************
*函数名：LCD_ShowChar()
* 描述 ：在指定位置显示一个字符
* 输入 ：x,y:起始坐标, code : ASCII字符的编码 size:字体大小 12/16/24, mode:叠加方式(1)还是非叠加方式(0)
* 输出 ：
*********************************************************************************************************
*/
void LCD_ShowChar1(uint16_t x,uint16_t y,uint8_t code, FONT_T *Font)
//void LCD_ShowChar1(uint16_t x,uint16_t y,uint8_t code, uint8_t fontcode,uint8_t mode)
{
	uint8_t temp, t1, t;
	uint16_t y0 = y;
//	uint8_t mode = 0;
	uint8_t Height,font_bytes;
	uint8_t buf[36];		/* 最大支持24点阵ASCII*/	
	
	switch (Font->FontCode)
	{
		case FC_ST_12:		/* 12点阵 */			
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
	//设置窗口		   
	code = code - ' ';//得到偏移后的值
	/* 将ascii字符点阵复制到buf */
	_LCD_ReadAsciiDot(code, Font->FontCode, buf);		
	for ( t=0; t<font_bytes; t++)
	{
		temp = buf[t];	
		for (t1=0; t1<8;t1++)
		{
			if(temp&0x80)							//最高位为1
				LCD_Fast_DrawPoint(x, y, Font->FrontColor );
			else
			{
				if (Font->BackColor != CL_MASK)	/* 判断是否改变背景色 */
				{
					LCD_Fast_DrawPoint(x,y, Font->BackColor);/* 设置像素颜色为文字背景色 */
				}
			}					
			temp<<=1;								//数据左移
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
		case FC_ST_12:		/* 12点阵 */			
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
	/* 将相应字点阵复制到buf */
	_LCD_ReadHZDot(code1, code2, Font->FontCode, buf);
	
	for(t=0;t<font_bytes;t++)
	{   												   
		temp=buf[t];			//得到点阵数据                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
				LCD_Fast_DrawPoint(x,y,Font->FrontColor);
			else 
			{
				if (Font->BackColor != CL_MASK)	/* 判断是否改变背景色 */
				{
					LCD_Fast_DrawPoint(x,y, Font->BackColor);/* 设置像素颜色为文字背景色 */
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
	
	uint8_t csize = (size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	

	//设置窗口		   
	num=num-' ';//得到偏移后的值
	for ( t=0; t<csize; t++)
	{
		if(size==12)temp=Ascii_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=Ascii_1608[num][t];	//调用1608字体
		else if(size==24)temp=Ascii_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for (t1=0; t1<8;t1++)
		{
			if(temp&0x80)							//最高位为1
				LCD_Fast_DrawPoint(x, y, POINT_COLOR);
			else if(mode == 0)
				LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;								//数据左移
			y++;
			if(y>=g_lcddev.height)return;				//超区域了
			if((y-y0)==size)
			{
				y = y0;
				x++;
				if(x>=g_lcddev.width)return;	//超区域了
				break;
			}
		}
	}
}
/*
*********************************************************************************************************
*	函 数 名: LCD_GetFontWidth
*	功能说明: 读取字体的宽度（像素单位)
*	形    参: tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数		_
*	返 回 值: 字体的宽度（像素单位)
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
*	函 数 名: LCD_GetFontHeight
*	功能说明: 读取字体的高度（像素单位)
*	形    参:_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 字体的宽度（像素单位)
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
*	函 数 名: LCD_GetStrWidth
*	功能说明: 计算字符串总宽度(像素单位)
*	形    参:	_ptr  : 字符串指针
*		_		tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
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
		code1 = *p;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
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
		else	/* 汉字 */
		{
			code2 = *++p;
			if (code2 == 0)
			{
				break;
			}
			font_width = LCD_GetFontWidth(_tFont);	//汉字宽度
			
		}
		width += font_width;
		p++;
	}
	return width;
}
/*
*********************************************************************************************************
*	函 数 名: _LCD_ReadAsciiDot
*	功能说明: 读取1个ASCII字符的点阵数据
*	形    参:
*		code : ASCII字符的编码，1字节。1-128
*		fontcode ：字体代码
*		pBuf : 存放读出的字符点阵数据
*	返 回 值: 文字宽度
*********************************************************************************************************
*/
static void _LCD_ReadAsciiDot(uint8_t code, uint8_t fontcode, uint8_t *pBuf)
{
	uint8_t *pAscDot;
	uint8_t font_bytes = 0;
	
	switch (fontcode)
	{
		case FC_ST_12:		/* 12点阵 */
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
*	函 数 名: _LCD_ReadHZDot
*	功能说明: 读取1个汉字的点阵数据
*	形    参:
*		_code1, _cod2 : 汉字内码. GB2312编码
*		_fontcode ：字体代码
*		_pBuf : 存放读出的字符点阵数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void _LCD_ReadHZDot(uint8_t _code1, uint8_t _code2,  uint8_t _fontcode, uint8_t *_pBuf)
{
	#if USE_SMALL_FONT		/* 使用CPU 内部Flash 小字库 */
		uint8_t *pDot;
		uint8_t font_bytes = 0;
		uint16_t address;
		uint16_t m = 0;

		pDot = 0;	/* 仅仅用于避免告警 */
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12点阵 */
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
				/* 字库搜索完毕，未找到，则填充全FF */
				memset(_pBuf, 0xFF, font_bytes);
				break;
			}
		}
	#else	/* 用全字库 */
		uint8_t font_bytes = 0;	//一个字符对应点阵集所占的字节数
		unsigned long foffset; 	
        
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12点阵 */
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
		if(_code1<0x81||_code2<0x40||_code2==0xff||_code1==0xff)//非 常用汉字
		{ 
			/* 字库搜索完毕，未找到，则填充全FF */
			memset(_pBuf, 0xFF, font_bytes);
			return; //结束访问
		} 
		
		if(_code2 < 0x7f)
			_code2 -= 0x40;//注意!
		else 
			_code2-=0x41;
		_code1 -= 0x81;	
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12点阵 */
				foffset =((unsigned long)190*_code1+_code2)*font_bytes;	//得到字库中的字节偏移量  	
				W25QXX_Read(_pBuf,foffset + ftinfo.f12addr,font_bytes);	 			
				break;			
			case FC_ST_16:
				font_bytes = 32;
				foffset =((unsigned long)190*_code1+_code2)*font_bytes;	//得到字库中的字节偏移量  				
				W25QXX_Read(_pBuf,foffset + ftinfo.f16addr,font_bytes);				
				break;	
			case FC_ST_24:
				font_bytes = 72;
				foffset =((unsigned long)190*_code1+_code2)*font_bytes;	//得到字库中的字节偏移量  		
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
*	函 数 名: LCD_DispStrEx
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串。 增强型函数。支持左\中\右对齐，支持定长清屏。
*	形    参:
*		_usX : X坐标
*		_usY : Y坐标
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数。
*		_Width : 字符串显示区域的宽度. 0 表示不处理留白区域，此时_Align无效
*		_Align :字符串在显示区域的对齐方式，
*				ALIGN_LEFT = 0,
*				ALIGN_CENTER = 1,
*				ALIGN_RIGHT = 2
*	返 回 值: 无
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
	uint16_t str_width;	/* 字符串实际宽度  */

	
	switch (Font->FontCode)
	{
		case FC_ST_12:		/* 12点阵 */
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
	str_width = LCD_GetStrWidth(_ptr, Font);	/* 计算字符串实际总宽度 */
	offset = 0;
	if (_Width > str_width)	/* 显示区域宽度大于字符串实际宽度 */
	{
		if (_Align == ALIGN_RIGHT)	/* 右对齐 */
		{
			offset = _Width - str_width;
		}
		else if (_Align == ALIGN_CENTER)	/* 居中对齐 */
		{
			offset = (_Width - str_width) / 2;
		}
		else	/* 左对齐 不处理 */
		{
			;
		}
	}
	/* 左侧填背景色, 中间对齐和右边对齐  */
	if (offset > 0)
	{
		LCD_Fill_Rect(_usX, _usY, offset, font_height+_usY, Font->BackColor);
		_usX += offset;
	}
	
	/* 右侧填背景色 */
	if (_Width > str_width)
	{
		LCD_Fill_Rect(_usX + str_width, _usY,  _Width , font_height+_usY, Font->BackColor);
	}

	/* 开始循环处理字符 */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)
		{
			width = font_width / 2;
			LCD_ShowChar1(_usX, _usY, code1, Font);//有效部分写入 
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
//			/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
//		}
//			_usX += width + _tFont->Space;	/* 列地址递增 */
		_usX += width;
		_ptr++;			/* 指向下一个字符 */
	}
	
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DispStr
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串
*	形    参:
*		_usX : X坐标
*		_usY : Y坐标
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, uint8_t *_ptr, FONT_T *_tFont)
{
	LCD_DispStrEx(_usX, _usY, _ptr, _tFont, 0, 0);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLine
*	功能说明: 在2点间画一条直线。
*	形    参:
*			_usX1, _usY1 : 起始点坐标
*			_usX2, _usY2 : 终止点Y坐标
*			_usColor     : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x = x2-x1; //计算坐标增量 
	delta_y = y2-y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x>0)
		incx=1; //设置单步方向 
	else if(delta_x == 0)
		incx=0;//垂直线 
	else 
	{
		incx=-1;
		delta_x=-delta_x;
	} 
	if(delta_y>0)
		incy=1; 
	else if(delta_y==0)
		incy=0;//水平线 
	else
	{
		incy=-1;
		delta_y=-delta_y;
	} 
	if( delta_x>delta_y)
		distance = delta_x; //选取基本增量坐标轴 
	else 
		distance = delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_Fast_DrawPoint(uRow,uCol,Color);//画点 
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
*	函 数 名: LCD_DrawPoints
*	功能说明: 绘制一组点，并将这些点连接起来。可用于波形显示。
*	形    参:x, y     : 坐标数组
*			_usSize	  ：点个数
*			_usColor : 颜色
*	返 回 值: 无
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
*	函 数 名: LCD_DrawRect
*	功能说明: 绘制水平放置的矩形。
*	形    参:
*			_usX,_usY: 矩形左上角的坐标
*			_usHeight : 矩形的高度
*			_usWidth  : 矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
		/*
	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	
	LCD_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _usColor);	/* 顶 */
	LCD_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _usColor);	/* 底 */

	LCD_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _usColor);	/* 左 */
	LCD_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _usColor);	/* 右 */
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参:
*			x0,y0  : 圆心的坐标
*			r  : 圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color)
{
	int a,b;
	int di;
	
	a = 0;
	b = r;	  
	di = 3-(r<<1);             //判断下个点位置的标志
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
		//使用Bresenham算法画圆     
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
*	函 数 名: LCD_Blend565
*	功能说明: 对像素透明化 颜色混合。32位真彩色后8位为透明度
*	形    参: src : 原始像素
*			  dst : 混合的颜色
*			  alpha : 透明度 0-32
*	返 回 值: 无
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
*	函 数 名: LCD_DrawIcon32
*	功能说明: 在LCD上绘制一个图标, 带有透明信息的位图(32位， RGBA). 图标下带文字
*	形    参: _pIcon : 图标结构
*			  _tFont : 字体属性
*			  _ucFocusMode : 焦点模式。0 表示正常图标  1表示选中的图标
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawIcon32(const ICON_T *tIcon, FONT_T *tFont, uint8_t FocusMode)
{
	const uint8_t *p;
	uint16_t oldRGB, newRGB;
	uint16_t x, y;			/* 用于记录窗口内的相对坐标 */
	uint8_t R1, G1, B1, A;	/* 新像素色彩分量 */
	uint8_t R0, G0, B0;		/* 旧像素色彩分量 */
	
	p = (const uint8_t *)tIcon->pBmp;
	p += 54;            /* 直接指向图像数据区 */
	/* 按照BMP位图次序，从左至右，从上至下扫描 */
	for (y = tIcon->Height ; y> 0; y--)
	{
        for (x=0; x<tIcon->Width; x++)
        {
 			B1 = *p++;
			G1 = *p++;
			R1 = *p++;
			A = *p++;	/* Alpha 值(透明度)，0-255, 0表示透明，1表示不透明, 中间值表示透明度 */   

			if (A == 0x00)  /* 需要透明,显示背景 */
			{
                ;/* 不用刷新背景 */
			}
			else if (A == 0xFF)/* 完全不透明， 显示新像素 */
			{
                newRGB = RGB(R1, G1, B1);
                if ( FocusMode == 1)
                {
                    newRGB = LCD_Blend565(newRGB, CL_YELLOW, 10);
                }
                LCD_Fast_DrawPoint(x+tIcon->Left, y+tIcon->Top, newRGB);
			}
			else    /* 半透明 */
			{
                /* 计算公式： 实际显示颜色 = 前景颜色 * Alpha / 255 + 背景颜色 * (255-Alpha) / 255 */
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
	/* 绘制图标下的文字 */
	{
        uint16_t len;
        uint16_t width;

        len = strlen(tIcon->Text);
        if (len == 0)
        {
            return;/* 如果图标文本长度为0，则不显示 */
        }
        /* 计算文本的总宽度 */
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
        	/* 水平居中 */
        x = (tIcon->Left + tIcon->Width/2) - width/2;
        y = tIcon->Top + tIcon->Height + 2;
        LCD_DispStr(x, y, (uint8_t *)tIcon->Text, tFont);
	}	
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawBmp32
*	功能说明: 在LCD上绘制一个32位的BMP图, 带有透明信息的位图(32位， RGBA)
*	形    参: usX, usY : 显示坐标
*			  Height, usWidth : 图片高度和宽度
*			  pBmp : 图片数据（带BMP文件头）
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawBmp32(uint16_t usX, uint16_t usY, uint16_t Height, uint16_t Width, uint8_t *pBmp)
	{
	const uint8_t *p;
	uint16_t usOldRGB, usNewRGB;
	uint16_t x, y;		/* 用于记录窗口内的相对坐标 */
	uint8_t R1,G1,B1,A;	/* 新像素色彩分量 */
	uint8_t R0,G0,B0;	/* 旧像素色彩分量 */

	p = (const uint8_t *)pBmp;
	p += 54;		/* 直接指向图像数据区 */

	/* 按照BMP位图次序，从左至右，从上至下扫描 */
	for (y = Height ; y > 0; y--)
	{
		for (x = 0; x < Width; x++)
		{
			B1 = *p++;
			G1 = *p++;
			R1 = *p++;
			A = *p++;	/* Alpha 值(透明度)，0-255, 0表示透明，1表示不透明, 中间值表示透明度 */

			if (A == 0x00)	/* 需要透明,显示背景 */
			{
				;	/* 不用刷新背景 */
			}
			else if (A == 0xFF)	/* 完全不透明， 显示新像素 */
			{
				usNewRGB = RGB(R1, G1, B1);
				//if (_ucFocusMode == 1)
				//{
				//	usNewRGB = Blend565(usNewRGB, CL_YELLOW, 10);
				//}
				LCD_Fast_DrawPoint(x + usX, y + usY, usNewRGB);
			}
			else 	/* 半透明 */
			{
				/* 计算公式： 实际显示颜色 = 前景颜色 * Alpha / 255 + 背景颜色 * (255-Alpha) / 255 */
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
*	函 数 名: LCD_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序: 从左到右，从上到下
*	形    参:
*			_usX, _usY : 图片的坐标
*			_usHeight  : 图片高度
*			_usWidth   : 图片宽度
*			_ptr       : 图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
//void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)











