/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶电容触摸驱动模块
*	文件名称 : bsp_Gt9147.c
*	版    本 : 
*	说    明 : 
*	修改记录 :
*		版本号  日期       作者    说明
*		
*********************************************************************************************************
*/
#include "bsp.h"
#include  "bsp_Gt9147.h"
/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C_SCL	GPIOB			/* GPIO端口 */
#define GPIO_PORT_I2C_SDA	GPIOF			/* GPIO端口 */
#define RCC_I2C_SCL_PORT 	RCC_AHB1Periph_GPIOB		/* SCL GPIO 端口时钟 */
#define RCC_I2C_SDA_PORT	RCC_AHB1Periph_GPIOF		/* SDA GPIO 端口时钟 */
#define CT_I2C_SCL_PIN		GPIO_Pin_0			/* 连接到SCL时钟线的GPIO */
#define CT_I2C_SDA_PIN		GPIO_Pin_11			/* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏 */
#define CT_I2C_SCL_1()  GPIO_PORT_I2C_SCL->BSRRL = CT_I2C_SCL_PIN				/* SCL = 1 */
#define CT_I2C_SCL_0()  GPIO_PORT_I2C_SCL->BSRRH = CT_I2C_SCL_PIN				/* SCL = 0 */

#define CT_I2C_SDA_1()  GPIO_PORT_I2C_SDA->BSRRL = CT_I2C_SDA_PIN				/* SDA = 1 */
#define CT_I2C_SDA_0()  GPIO_PORT_I2C_SDA->BSRRH = CT_I2C_SDA_PIN				/* SDA = 0 */

#define CT_I2C_SDA_READ()  ((GPIO_PORT_I2C_SDA->IDR & CT_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#define CT_I2C_SCL_READ()  ((GPIO_PORT_I2C_SCL->IDR & CT_I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */
//IO方向设置
#define CT_SDA_IN()  {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=0<<2*11;}	//PF11输入模式
#define CT_SDA_OUT() {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=1<<2*11;} 	//PF11输出模式
/* 中断信号和复位信号端口 */
#define GPIO_PORT_INT		GPIOB
#define GPIO_PORT_RST		GPIOC
#define RCC_INT_PORT 		RCC_AHB1Periph_GPIOB		/* INT GPIO 端口时钟 */
#define RCC_RST_PORT		RCC_AHB1Periph_GPIOC		/* RST GPIO 端口时钟 */
#define GPIO_PIN_INT		GPIO_Pin_1					/* 连接到INT时钟线的GPIO */
#define GPIO_PIN_RST		GPIO_Pin_13					/* 连接到RST数据线的GPIO */

#define GT_RST_1()		GPIO_PORT_RST->BSRRL = GPIO_PIN_RST					/* RST = 1 */
#define GT_RST_0()		GPIO_PORT_RST->BSRRH = GPIO_PIN_RST					/* RST = 0 */
#define INT_IS_HIGH()	(GPIO_ReadInputDataBit(GPIO_PORT_INT, GPIO_PIN_INT) == Bit_SET)

const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};	//触摸点数据地址
GT9147_T	g_tGT9147;	
uint16_t GT9147_ReadVersion(void);
/*
 *GT9147配置参数表
 *第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
 *flash原有版本号,才会更新配置.
 */
const uint8_t GT9147_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
}; 
void CT_IIC_Stop(void);
//void CT_IIC_Start(void);
/*
*********************************************************************************************************
*	函 数 名: CT_IIC_Init
*	功能说明: 电容触摸芯片IIC接口初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CT_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_I2C_SCL_PORT|RCC_I2C_SDA_PORT, ENABLE);//使能GPIOB,F时钟

	GPIO_InitStructure.GPIO_Pin = CT_I2C_SCL_PIN;//PB0设置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIO_PORT_I2C_SCL, &GPIO_InitStructure);//初始化
		
	GPIO_InitStructure.GPIO_Pin = CT_I2C_SDA_PIN;//PF11设置推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_Init(GPIO_PORT_I2C_SDA, &GPIO_InitStructure);//初始化	

	CT_IIC_Stop();
}
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void I2C_Delay(void)
{
	uint8_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 60; i++);
	//bsp_DelayUS(2);

}
/*
*********************************************************************************************************
*	函 数 名: I2C_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void CT_IIC_Start(void)
{	
    CT_SDA_OUT();

	CT_I2C_SCL_1(); 
	CT_I2C_SDA_1(); 
	I2C_Delay();
	//bsp_DelayUS(30);
	CT_I2C_SDA_0();		//先拉低SDA
	I2C_Delay();
	CT_I2C_SCL_0(); 	//再拉低SCL
	I2C_Delay();
}
/*
*********************************************************************************************************
*	函 数 名: CT_IIC_Stop
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void CT_IIC_Stop(void)
{	
	CT_SDA_OUT();
	CT_I2C_SDA_0();
	CT_I2C_SCL_1();
	I2C_Delay();
	//bsp_DelayUS(30);
	CT_I2C_SDA_0();
	I2C_Delay();
	CT_I2C_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: CT_IIC_Wait_Ack
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t CT_IIC_Wait_Ack(void)
{	
	uint8_t ucErrTime=0;
	CT_SDA_IN();      //SDA设置为输入  
	CT_I2C_SDA_1();	/* CPU释放SDA总线 */
	I2C_Delay();
	CT_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	I2C_Delay();		 
	while(CT_I2C_SDA_READ())      //等待应答
	{
        ucErrTime++;
        if(ucErrTime>=250)
        {
            I2C_Stop();          
            return 1;
        }
	}		
	CT_I2C_SCL_0();//再拉低SCL完成应答位，并保持住总线
	I2C_Delay();
	return 0;
}
/*
*********************************************************************************************************
*   函 数 名: CT_IIC_Ack
*   功能说明: I2C 读数据.发送应答位
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
void CT_IIC_Ack(void)
{
	CT_SDA_OUT();
	CT_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	I2C_Delay();
	CT_I2C_SCL_1();	/* CPU产生1个时钟 */
	I2C_Delay();
	CT_I2C_SCL_0();
	I2C_Delay();
	CT_I2C_SDA_1();	/* CPU释放SDA总线 */
}
	
/*
*********************************************************************************************************
*	函 数 名: CT_IIC_NAck
*	功能说明: I2C 读数据.不发送应答位
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void CT_IIC_NAck(void)
{
	CT_SDA_OUT();
	CT_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	I2C_Delay();
	CT_I2C_SCL_1();	/* CPU产生1个时钟 */
	I2C_Delay();
	CT_I2C_SCL_0();
	I2C_Delay();
}
/*
*********************************************************************************************************
*	函 数 名: i2c_Read
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t CT_IIC_Read_Byte(void)
{
	uint8_t i;
	uint8_t value;
	CT_SDA_IN();//SDA设置为输入
	//bsp_DelayUS(30);
	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		CT_I2C_SCL_1();
		I2C_Delay();
		if (CT_I2C_SDA_READ())
		{
			value++;
		}
		CT_I2C_SCL_0();
		I2C_Delay();
	}
	return value;
}
/*
*********************************************************************************************************
*	函 数 名: CT_IIC_Send_Byte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void CT_IIC_Send_Byte(uint8_t _ucByte)
{
	uint8_t i;
	CT_SDA_OUT(); 	 
	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			CT_I2C_SDA_1();
		}
		else
		{
			CT_I2C_SDA_0();
		}
		I2C_Delay();
		CT_I2C_SCL_1();
		I2C_Delay();
		CT_I2C_SCL_0();
		if (i == 7)
		{
			 CT_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		I2C_Delay();
	}
}

/*
*********************************************************************************************************
*函数名：GT9147_WR_Reg()
* 描述 ：向GT9147写入一次数据
* 输入 ：reg:起始寄存器地址 buf:数据缓缓存区 len:写数据长度
* 输出 ：返回值:0,成功;1,失败.
*********************************************************************************************************
*/
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for (i = 0;i < len; i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret = CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
/*
*********************************************************************************************************
*函数名：GT9147_RD_Reg()
* 描述 ：从GT9147读出一次数据
* 输入 ：reg:起始寄存器地址  buf:数据缓缓存区  len:读数据长度		
* 输出 ：返回值:0,成功;1,失败.
*********************************************************************************************************
*/
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令		   
	CT_IIC_Wait_Ack();	   
	for (i = 0; i < len; i++)
	{
		buf[i] = CT_IIC_Read_Byte();	/* 读1个字节 */

		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != len - 1)
		{
			CT_IIC_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			CT_IIC_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	} 
    CT_IIC_Stop();//产生一个停止条件    
} 
   
/*
*********************************************************************************************************
*函数名：GT9147_Send_Cfg()
* 描述 ：发送GT9147配置参数
* 输入 ：mode:0,参数不保存到flash  1,参数保存到flash
* 输出 ：0,初始化成功;1,初始化失败 
*********************************************************************************************************
*/
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
	uint8_t buf[2];
	uint8_t i=0;
	buf[0]=0;
	buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//计算校验和
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
	return 0;
} 
/*
*********************************************************************************************************
*函数名：GT9147_InitHard()
* 描述 ：初始化GT9147触摸屏硬件
* 输入 ：
* 输出 ：
*********************************************************************************************************
*/
void GT9147_InitHard(void)
{
	uint8_t temp; 
	
	GPIO_InitTypeDef  GPIO_InitStructure;	

	RCC_AHB1PeriphClockCmd(RCC_INT_PORT|RCC_RST_PORT, ENABLE);//使能GPIOB,C时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_INT ;//PB1设置为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIO_PORT_INT, &GPIO_InitStructure);//初始化
		
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RST;//PC13设置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_Init(GPIO_PORT_RST, &GPIO_InitStructure);//初始化	

	CT_IIC_Init();      	//初始化电容屏的I2C总线  
	GT_RST_0();				//复位
	bsp_DelayMS(10);
	GT_RST_1();				//释放复位		    
	bsp_DelayMS(10); 

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_INT ;//PB1设置为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIO_PORT_INT, &GPIO_InitStructure);//初始化
	 
	bsp_DelayMS(100);  
	
	temp = 0X02;			
	GT9147_WR_Reg(GT_CTRL_REG,&temp,1);//软复位GT9147
	GT9147_RD_Reg(GT_CFGS_REG,&temp,1);//读取GT_CFGS_REG寄存器
	if(temp<0X60)//默认版本比较低,需要更新flash配置
	{
	//	printf("Default Ver:%d\r\n",temp[0]);
		GT9147_Send_Cfg(1);//更新并保存配置
	}
	bsp_DelayMS(10);
	temp = 0X00;	 
	GT9147_WR_Reg(GT_CTRL_REG,&temp,1);//结束复位 
}
/*
*********************************************************************************************************
*函数名：GT9147_Init()
* 描述 ：初始化GT9147触摸屏
* 输入 ：
* 输出 ：0,初始化成功;1,初始化失败 
*********************************************************************************************************
*/
uint8_t GT9147_Init(void)
{
	uint16_t ver;
	GT9147_InitHard();
	ver = GT9147_ReadVersion();
	g_tGT9147.TimerCount = 0;
	
	if (ver == 9147)
	{
//		printf("CTP ID:%d\r\n",ver);	//打印ID
		return 0;
	}
	else
		return 1;	
}
/*
*********************************************************************************************************
*	函 数 名: GT811_Scan
*	功能说明: 读取GT811触摸数据。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void GT9147_Scan(void)
{
    uint8_t i;
	uint8_t mode,temp;
	uint8_t buf[4];
    static uint8_t state = TOUCH_NONE;
    uint16_t x, y;
	static uint16_t x_save, y_save;
	
	/* 20ms 执行一次 刷新时间太短,会有误读*/
	if (g_tGT9147.TimerCount < 20)
	    return;
	g_tGT9147.TimerCount = 0;
	
	GT9147_RD_Reg(GT_GSTID_REG,&mode,1);//读取触摸点的状态  
	if ((mode&0X0F)&&((mode&0X0F)<6))	
	{

        for (i=0; i<(mode&0X0f); i++)
        {
			GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值	
			if(g_lcddev.direction > 3)//横屏
			{
				g_tGT9147.y[i]=((uint16_t)buf[1]<<8)+buf[0];
				g_tGT9147.x[i]=g_lcddev.width -(((uint16_t)buf[3]<<8)+buf[2]);
			}else		  //竖屏
			{
				g_tGT9147.x[i]=((uint16_t)buf[1]<<8)+buf[0];
				g_tGT9147.y[i]=((uint16_t)buf[3]<<8)+buf[2];
			} 		
        }
		x = g_tGT9147.x[0];
		y = g_tGT9147.y[0];
		if (state == TOUCH_NONE)
			state = TOUCH_DOWN;
	}

	switch (state)
	{
        case TOUCH_NONE:			
            break;
        case TOUCH_DOWN:
			if (mode&0X8F> 0x80)		//有触摸且触摸点数大于0）
			{
				TOUCH_PutKey(TOUCH_DOWN, x, y);
				state = TOUCH_MOVE;
				//	printf("down\r\n");
			}
			else
			{
				x_save = x;				/* 保存坐标，用于释放事件 */
				y_save = y;
				state = TOUCH_RELEASE;
			}
			break;
		case TOUCH_MOVE:
			if (mode&0X8F> 0x80)		//有触摸且触摸点数大于0）
			{
				TOUCH_PutKey(TOUCH_MOVE, x, y);
				state = TOUCH_MOVE;
				//	printf("move\r\n");
			}
			else
			{
				x_save = x;			/* 保存坐标，用于释放事件 */
				y_save = y;
				state = TOUCH_RELEASE;
			}			
			break;
		case TOUCH_RELEASE:
			TOUCH_PutKey(TOUCH_RELEASE, x_save, y_save);	/* 释放 */
			state = TOUCH_NONE;
			break;
	}
	if(mode&0X80&&((mode&0XF)<6))
	{
		temp=0;
		GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志 		
	}
	
}
/*
*********************************************************************************************************
*	函 数 名: GT811_Timer1ms
*	功能说明: 每隔1ms调用1次
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void GT9147_Timer1ms(void)
{
	g_tGT9147.TimerCount++;
}

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
#if 0
uint8_t GT9147_Scan1(uint8_t mode,uint8_t lcdDir)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	static uint8_t t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{
		GT9147_RD_Reg(GT_GSTID_REG,&mode,1);//读取触摸点的状态  
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
					if(lcdDir>3)//横屏
					{
						tp_dev.y[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.x[i]=g_lcddev.width -(((uint16_t)buf[3]<<8)+buf[2]);
					}else				//竖屏
					{
						tp_dev.x[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((uint16_t)buf[3]<<8)+buf[2];
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据
			t=0;		//触发一次,则会最少连续监测10次,从而提高命中率
		}
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志 		
		}
	}
	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}
#endif
/*
*********************************************************************************************************
*	函 数 名: GT9147_ReadVersion
*	功能说明: 获得GT9147的芯片版本
*	形    参: 无
*	返 回 值: 32位版本
*********************************************************************************************************
*/
static uint16_t GT9147_ReadVersion(void)
{
	uint8_t buf[5]; 
	
	GT9147_RD_Reg(GT_PID_REG,buf,4);//读取产品ID,ASCII码形式
	buf[4]=0;
	buf[0] = buf[0] - 0x30;
	buf[1] = buf[1] - 0x30;
	buf[2] = buf[2] - 0x30;
	buf[3] = buf[3] - 0x30;
	return ((uint16_t)buf[0] *1000 + (uint16_t)buf[1]*100+ buf[2]*10+buf[3]);
}

















