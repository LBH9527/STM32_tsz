/*
*********************************************************************************************************
*
*	模块名称 : DS18B20 驱动模块(1-wire 数字温度传感器）
*	文件名称 : bsp_ds18b20.c
*	版    本 : V1.0
*	说    明 : DS18B20和CPU之间采用1个GPIO接口。
*
*	修改记录 :
*		版本号  日期         作者     说明
*		V1.0    2014-01-24  armfly  正式发布
*
*
*********************************************************************************************************
*/
#include "bsp.h"

//IO反向设置
#define DS18B20_IO_IN()     GPIOG->MODER &= ~(3<<(9*2));GPIOG->MODER |= 0<<9*2	//PG9输入模式
#define DS18B20_IO_OUT()	GPIOG->MODER &= ~(3<<(9*2));GPIOG->MODER |= 1<<9*2	//PG9输出模式
//IO操作函数											   
#define	DS18B20_DQ_OUT      PGout(9) //数据端口	PG9
#define	DS18B20_DQ_IN       PGin(9)  //数据端口	PG9 

//温度小数位对照表,小数部分 0000 ～ 1111 16个数， 最小分辨率为0.0625 ， 保留一位小数对应表
//0000 ～ 0 x 0.0625
//0001 ～ 1 x 0.0625
//????
//1111 ～ 15 x 0.0625 = 0.9375
uc8 DS18b20Code[] = {0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9,9};
/*******************************************************************************
* 函数名  ; DS18B20_Rst
* 描述    ： 复位
* 输入参数： 
* 返回参数： 
*******************************************************************************/
void DS18B20_Rst(void)
{
    /*
		复位时序, 见DS18B20 page 15

		首先主机拉低DQ，持续最少 480us
		然后释放DQ，等待DQ被上拉电阻拉高，约 15-60us
		DS18B20 将驱动DQ为低 60-240us， 这个信号叫 presence pulse  (在位脉冲,表示DS18B20准备就绪 可以接受命令)
		如果主机检测到这个低应答信号，表示DS18B20复位成功
	*/
	DS18B20_IO_OUT();
    DS18B20_DQ_OUT = 0;     //拉低DQ
    bsp_DelayUS(500);          //拉底500us
    DS18B20_DQ_OUT = 1;     //拉高DQ
    bsp_DelayUS(10);
}

/*******************************************************************************
* 函数名  ; DS18B20_Check
* 描述    ： 复位
* 输入参数： 
* 返回参数： 返回1:未检测到DS18B20的存在 返回0:存在
*******************************************************************************/
u8 DS18B20_Check(void)
{
    u8 retry = 0;

    DS18B20_IO_IN();
    while (DS18B20_DQ_IN && retry<200)
    {
        retry++;
        bsp_DelayUS(1);
    };
    if (retry >= 200)
        return 1;
    else 
        retry = 0;
    while (!DS18B20_DQ_IN && retry<240)
    {
        retry++;
        bsp_DelayUS(1);
    }
    if (retry >= 240)
        return 1;
    return 0;
}

/*******************************************************************************
* 函数名  ; DS18B20_ReadBit
* 描述    ： 复位
* 输入参数： 
* 返回参数： 
*******************************************************************************/
u8 DS18B20_ReadBit(void)
{
	u8 data;

	DS18B20_IO_OUT();
	DS18B20_DQ_OUT = 0;
	bsp_DelayUS(5);	
	DS18B20_IO_IN();
	bsp_DelayUS(10);
	if (DS18B20_DQ_IN)
		data = 1;
	else
		data = 0;
	bsp_DelayUS(50);
	return data;
}

/*******************************************************************************
* 函数名  ; DS18B20_ReadByte
* 描述    ： 
* 输入参数： 
* 返回参数： 
*******************************************************************************/
u8 DS18B20_ReadByte(void)
{
	u8 i,temp,dat;

	dat = 0;
	for (i = 1; i<=8; i++)
	{
		temp = DS18B20_ReadBit();
		temp = temp << 7;
		dat = temp | (dat>>1);
	}
	return dat;
}

/*******************************************************************************
* 函数名  ; DS18B20_WriteByte
* 描述    ： 
* 输入参数： 要写入的字节
* 返回参数： 
*******************************************************************************/
void DS18B20_WriteByte(u8 data)
{
	u8 i;

	DS18B20_IO_OUT();
	for (i=0; i<8; i++)
	{
		if (data & 0x01)
		{
			DS18B20_DQ_OUT = 0;
			bsp_DelayUS(2);
			DS18B20_DQ_OUT = 1;
			bsp_DelayUS(60);

		}			
		else
		{
			DS18B20_DQ_OUT = 0;
			bsp_DelayUS(60);
			DS18B20_DQ_OUT = 1;			
			bsp_DelayUS(2);

		}
		data = data>>1;
	}	
}

/*******************************************************************************
* 函数名  ; DS18B20_Init()
* 描述    ： 初始化DS18B20的IO口 DQ 同时检测DS的存在
* 输入参数： 
* 返回参数： 返回1:未检测到DS18B20的存在 返回0:存在
*******************************************************************************/
u8 bsp_InitDS18B20(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

    //GPIOG9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化

	DS18B20_Rst();
	return DS18B20_Check();
}

/*******************************************************************************
* 函数名  ; DS18B20_Start(void)
* 描述    ： 
* 输入参数： 要写入的字节
* 返回参数： 
*******************************************************************************/
void DS18B20_Start(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_WriteByte(0xCC);// skip rom
	DS18B20_WriteByte(0x44);// convert
}

/*******************************************************************************
* 函数名  ; DS18B20_GetTemp(void)
* 描述    ： //精度：0.1C
* 输入参数： 
* 返回参数： //返回值：温度值 （-550~1250)
*******************************************************************************/
u16 DS18B20_GetTemp(void)
{
	u8 TH,TL;
	u8 tempH,tempL;
	
	u8 flag;
	u16 readValue;
	
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_WriteByte(0xCC);// skip rom
	DS18B20_WriteByte(0xbe);// convert
	TL = DS18B20_ReadByte();//LSB
	TH = DS18B20_ReadByte();//MSB
	if (TH > 7)
	{
		TH = ~TH;
		TL = ~TL;
		flag = 0;		//温度为负  
	}
	else
		flag = 1;		//温度为正	 
	readValue = TH;
	readValue<<= 8;
	readValue+= TL;
	tempL = readValue & 0x000f;
	tempL = DS18b20Code[tempL];		////查表得小数部分值
	tempH = readValue>>4;			//整数部分值
	if(flag)
		return(tempH*10+tempL);			//扩大10倍的温度值
	else
		return(~(tempH*10+tempL));
}
/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadID
*	功能说明: 读DS18B20的ROM ID， 总线上必须只有1个芯片
*	形    参: _id 存储ID
*	返 回 值: 0 表示失败， 1表示检测到正确ID
*********************************************************************************************************
*/
uint8_t DS18B20_ReadID(uint8_t *_id)
{
	uint8_t i;

	/* 总线复位 */
	DS18B20_Rst();

	DS18B20_WriteByte(0x33);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		_id[i] = DS18B20_ReadByte();
	}

	DS18B20_Rst();		/* 总线复位 */
	
	return 1;
}
