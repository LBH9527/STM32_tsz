/*
*********************************************************************************************************
*
*	模块名称 : 串行EEPROM 24xx驱动模块
*	文件名称 : bsp_eeprom_24cxx.c
*	版    本 : V1.0
*	说    明 : 实现24xx系列EEPROM的读写操作。写操作采用页写模式提高写入效率。
*
*	修改记录 :
*		版本号  日期        作者     说明
*
*********************************************************************************************************
*/

/*
	应用说明：访问串行EEPROM前，请先调用一次 bsp_InitI2C()函数配置好I2C相关的GPIO.
*/

#include "bsp.h"

u8 AT24CXX_CheckOk(void);
/*
*********************************************************************************************************
*	函 数 名: bsp_InitAT24CXX
*	功能说明: 初始化EEPROM
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
void bsp_InitAT24CXX(void)
{
	bsp_InitI2C();
	if (AT24CXX_CheckOk() == 0)
	{
		#ifdef BSP_Printf
		BSP_Printf("EEPROM CheckeEror\r\n");
	#endif
	}
}
/*
*********************************************************************************************************
*	函 数 名: ee_CheckOk
*	功能说明: 判断串行EERPOM是否正常
*	形    参:  无
*	返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
u8 AT24CXX_CheckOk(void)
{
	if (I2C_CheckDevice(EE_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		/* 失败后，切记发送I2C总线停止信号 */
		I2C_Stop();
		return 0;
	}
}
/*
*********************************************************************************************************
* 函数名  :  AT24CXX_WriteByte()
* 描述    ： 24Cxx 写数据
* 输入参数： addr：要写入数据的寄存器地址， data：要写入的数据
* 返回参数：
*********************************************************************************************************
*/
void AT24CXX_WriteByte(u16 addr, u8 data)
{
	I2C_Start();
//	if (EE_TYPE>AT24C16)
//	{

//	}
//	else
	    I2C_Write(EE_DEV_ADDR + ((addr/256)<<1)); //写器件地址和页地址的高 3 位
	I2C_Write(addr%256);    //写页地址的低 4 位和页地址内部偏移量
	I2C_Write(data);
	I2C_Stop();
	bsp_DelayMS(1);
}
/*
*********************************************************************************************************
*	函 数 名: AT24CXX_WriteStr
*	功能说明: 向串行EEPROM指定地址写入若干数据
*	形    参:  addr : 起始地址
*			 len : 数据长度，单位为字节
*			 str : 存放写入数据的缓冲区指针
*	返 回 值:
*********************************************************************************************************
*/
void AT24CXX_WriteStr(u16 addr, u8 *str, u16 len)
{
	/*
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
		对于24xx02，page size = 8;24c16,page size = 16
		简单的处理方法为：按字节写操作模式，每写1个字节，都发送地址
		为了提高连续写的效率可以数采用page wirte操作，但是要注意页写的长度限制，要对跨页进行区分。
		程序采用简单的处理方法，但写入速度会慢
	*/
#if 1
	I2C_Start();
//	if (EE_TYPE>AT24C16)
//	{

//	}
//	else
	    I2C_Write(EE_DEV_ADDR + ((addr/256)<<1));  ////写器件地址和页地址的高 3 位
	I2C_Write(addr%256);    //写页地址的低 4 位和页地址内部偏移量
	while (len--)
	{
		I2C_Write(*str);        
		str++;	
	}
	I2C_Stop();
#endif
#if 0
	while(len--)
	{
		AT24CXX_WriteByte(addr,*str);
		addr++;
		str++;
	}
#endif	
}

/*
*********************************************************************************************************
* 函数名  :  AT24CXX_ReadByte()
* 描述    ： 24Cxx 写数据
* 输入参数： addr：要写入数据的寄存器地址
* 返回参数：
*********************************************************************************************************
*/
u8 AT24CXX_ReadByte(u16 addr)
{
	u8 temp;

	I2C_Start();
//	if (EE_TYPE>AT24C16)
//	{

//	}
//	else
	     I2C_Write(EE_DEV_ADDR + ((addr/256)<<1));  ////写器件地址和页地址的高 3 位
	I2C_Write(addr%256);    //写页地址的低 4 位和页地址内部偏移量
	I2C_Start();
	I2C_Write(EE_DEV_ADDR | I2C_RD);             //进入接收模式	
	temp = I2C_Read_NoACK();    //读一个字节
	I2C_Stop();
	return temp;
}

/*
*********************************************************************************************************
*	函 数 名: AT24CXX_ReadStr
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参:  addr : 起始地址
*			 len : 数据长度，单位为字节
*			 str : 存放读到的数据的缓冲区指针
*	返 回 值:
*********************************************************************************************************
*/
void AT24CXX_ReadStr(u16 addr, u8 *str, u8 len)
{
	I2C_Start();
//	if (EE_TYPE>AT24C16)
//	{

//	}
//	else
	     I2C_Write(EE_DEV_ADDR + ((addr/256)<<1));  ////写器件地址和页地址的高 3 位
	I2C_Write(addr%256);    //写页地址的低 4 位和页地址内部偏移量
	I2C_Start();
	I2C_Write(EE_DEV_ADDR | I2C_RD);
	while (--len)
	{
		*str = I2C_Read_ACK();
		str++;
	}
	*str = I2C_Read_NoACK();
}

/*
*********************************************************************************************************
*	函 数 名: DataSave_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DataSave_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 3000; i++);
}
/*
*********************************************************************************************************
* 函数名  :  U16SaveToE2
* 描述    ： 保存uint16_t型参数到EEPROM中.要把uint16_t型强制转化成两个uint8_t来保存；
* 输入参数： _usAddress：地址 _pWriteBuf：存放写入数据的缓冲区指针 _usSize：写数据个数
* 返回参数： 无
*********************************************************************************************************
*/
void bsp_U16SaveToE2(uint16_t _usAddress, uint16_t *_pWriteBuf, uint16_t _usSize)
{
    uint8_t i;
    
    for (i=0; i<_usSize; i++)
    {
        AT24CXX_WriteStr(_usAddress+2*i, (uint8_t*)(&_pWriteBuf[i]), 2);
        DataSave_Delay();
    }
}
/*
*********************************************************************************************************
* 函数名  :  E2DataReload
* 描述    ： 读出所有的系统参数， 不包括校验值
* 输入参数： _usAddress：读寄存器的首地址 _pWriteBuf：存放写入数据的缓冲区指针 _usSize：写数据个数
* 返回参数： 无
*********************************************************************************************************
*/
void bsp_E2DataReload(uint16_t _usAddress, uint16_t *_pReadBuf, uint16_t _usSize)
{
    AT24CXX_ReadStr(_usAddress,(uint8_t*)_pReadBuf, _usSize*2);    
}
