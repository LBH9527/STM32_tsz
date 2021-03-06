/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动模块
*	文件名称 : bsp_i2c_gpio.c
*	版    本 : V1.0
*	说    明 : 用gpio模拟i2c总线, 适用于STM32F4系列CPU。该模块不包括应用层命令帧，仅包括I2C总线基本操作函数。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*
*********************************************************************************************************
*/

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
*/
    
#include "bsp.h"

//SDA方向
#define IIC_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define IIC_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

#define IIC_SCL		PBout(8)	//SCL
#define IIC_SDA		PBout(9)	//SDA
#define IIC_SDA_READ	PBin(9) //SDA读

/*
*********************************************************************************************************
*	函 数 名: bsp_InitI2C
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化	

//	IIC_SDA_OUT();//SDA线输出
	IIC_SDA = 1;
	IIC_SCL = 1;	
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
	u8 i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 30; i++);

}
/*
*********************************************************************************************************
*	函 数 名: I2C_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_Start(void)
{
	IIC_SDA_OUT();//SDA线输出
	IIC_SCL = 1;
	IIC_SDA = 1;
	I2C_Delay();
	IIC_SDA = 0;	//先拉低SDA
	I2C_Delay();
	IIC_SCL = 0;	//再拉低SCL
}

/*
*********************************************************************************************************
*	函 数 名: I2C_Stop
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_Stop(void)
{
	IIC_SDA_OUT();//SDA线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	I2C_Delay();
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	I2C_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: I2C_Wait_ACK
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
u8 I2C_Wait_ACK(void)
{
	u8 ucErrTime=0;
	
	IIC_SDA_IN();
	IIC_SDA = 1;
	I2C_Delay();
	IIC_SCL = 1;
	I2C_Delay();		 
	while(IIC_SDA_READ)      //等待应答
	{
        ucErrTime++;
        if(ucErrTime>=250)
        {
            I2C_Stop();   
        
            return 1;
        }
	}	
	IIC_SCL = 0;	//再拉低SCL完成应答位，并保持住总线
	return 0;

}

/*
*********************************************************************************************************
*	函 数 名: i2c_Read
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
u8 I2C_Read(void)
{
	u8 i;
	u8 receive = 0;

	IIC_SDA_IN();//SDA 设置为输入
	for (i=0; i<8; i++)
	{
		IIC_SCL = 0;
		I2C_Delay();
		IIC_SCL = 1;
		receive <<= 1;  //从高位到低位依次进行
		receive |= IIC_SDA_READ;
		I2C_Delay();
	}
	IIC_SCL = 0;
	IIC_SDA_OUT();  //SDA线输出
	return receive; 
}


/*
*********************************************************************************************************
*   函 数 名: I2C_Read_NoACK
*   功能说明: I2C 读数据.不发送应答位
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
u8 I2C_Read_NoACK(void)
{
	u8 rcv;
	rcv = I2C_Read();
	IIC_SDA = 1;		//8位数据发送完后，拉高SDA，发送非应答信号
	I2C_Delay();
	IIC_SCL = 1;
	I2C_Delay();
	IIC_SCL = 0;
	return rcv;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: I2C 读数据.发送应答位
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
u8 I2C_Read_ACK(void)
{
	u8 rcv;
	rcv = I2C_Read();
	IIC_SDA = 0;        //8位数据发送完后，拉低SDA，发送应答信号
	I2C_Delay();
	IIC_SCL = 1;
	I2C_Delay();
	IIC_SCL = 0;
	return rcv;
}

/*
*********************************************************************************************************
*	函 数 名: I2C_Write
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
u8 I2C_Write(u8 data)
{
	u8 i;
	u8 ack;
	
	IIC_SDA_OUT();  //SDA线输出
	IIC_SCL=0;//拉低时钟开始数据传输
	for (i=0; i<8; i++)
	{
		IIC_SDA = (data&0x80) >> 7;
		data <<= 1;
		I2C_Delay();
		IIC_SCL = 1;
		I2C_Delay();
		IIC_SCL = 0;
		I2C_Delay();
	}
	ack = I2C_Wait_ACK();
	return ack;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参:  _Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
u8 I2C_CheckDevice(u8 Address)
{
	u8 Ack;
	
	I2C_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	Ack = I2C_Write(Address | I2C_WR);/* 检测设备的ACK应答 */
	I2C_Stop();			/* 发送停止信号 */
	return Ack;
}






