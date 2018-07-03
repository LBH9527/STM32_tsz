/*
*********************************************************************************************************
*
*	模块名称 : W25Q128芯片驱动模块
*	文件名称 : bsp_W25Qxx_Flash.c
*	版    本 : V1.1
*	说    明 : 实现对外部 FLASH（ W25Q128）的读写，
*	修改记录 :
*		版本号  日期       作者    	说明
*		V1.0   			  9527		对W25Q128按Sector操作
*
*********************************************************************************************************
*/

#include "bsp.h"
	/*
		4Kbytes为一个Sector，16个扇区为1个Block，1个Block = 64K字节
		W25Q128容量为16M字节,共有256个Block,4096个Sector .最小擦除单位为一个Sector。	
	*/

	/*
		串行Flash型号为 W25Q128BV: 128M-bit/16M-byte (104MHz)
		STM32硬件SPI接口 = SPI3 或者 SPI1

		由于SPI1的时钟源是84M, SPI3的时钟源是42M。为了获得更快的速度，软件上选择SPI1。
	*/
	/*
		【SPI时钟最快是2分频，不支持不分频】
		如果是SPI1，2分频时SCK时钟 = 42M，4分频时SCK时钟 = 21M
		如果是SPI3, 2分频时SCK时钟 = 21M

		如果排线过长，则SPI时钟必须降低才行。 8分频对应 10.5M的速度。
	*/
/* CS 对应的GPIO时钟 */
#define	RCC_W25		(RCC_AHB1Periph_GPIOB)
/* 片选GPIO端口  */
#define W25_CS_GPIO			GPIOB
#define W25_CS_PIN			GPIO_Pin_14
/* 片选口线置低选中  */
#define W25_CS_0()      W25_CS_GPIO->BSRRH = W25_CS_PIN	//输出低电平 GPIO_ResetBits(PORT_CS, PIN_CS)
/* 片选口线置高不选中 */
#define W25_CS_1()     W25_CS_GPIO->BSRRL = W25_CS_PIN	//设置为高电平 GPIO_SetBits(PORT_CS, PIN_CS)
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 	/* 禁止写 */
#define W25X_ReadStatusReg		0x05 	/* 读状态寄存器命令 */
#define W25X_WriteStatusReg		0x01 	/* 写状态寄存器命令 */
#define W25X_ReadData			0x03 	/* 读数据区命令 */
#define W25X_FastReadData		0x0B 	/* 快速读数据区命令*/
#define W25X_FastReadDual		0x3B 	/* 快速读取双输出 */
#define W25X_PageProgram		0x02 	/* 页操作*/
#define W25X_BlockErase			0xD8 	/* 64KB快擦除*/
#define W25X_SectorErase		0x20 	/* 擦除扇区命令 */
#define W25X_ChipErase			0xC7 	/* 批量擦除命令 */
#define W25X_PowerDown			0xB9 	/* 低功耗 */
#define W25X_ReleasePowerDown	0xAB 	/* 取消低功耗 */
#define W25X_DeviceID			0xAB 	/* 读取设备ID */
#define W25X_ManufactDeviceID	0x90 	/* 读制造商ID */
#define W25X_JedecDeviceID		0x9F 	/* 读JEDEC ID */

void W25QXX_Wait_Busy(void);   
#define WIP_Flag                0x01 	/* Write In Progress (WIP) flag */
#define Dummy_Byte              0xFF	/* 哑命令，可以为任意值，用于读操作 */

W25_S s_W25;

void W25_SetCS(uint8_t state);
/*
*********************************************************************************************************
*	函 数 名: w25_ConfigGPIO
*	功能说明: 初始化串行Flash硬件接口
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitW25QxxFlash(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

	//GPIOB14
	GPIO_InitStructure.GPIO_Pin = W25_CS_PIN;			//PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(W25_CS_GPIO, &GPIO_InitStructure);		//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			//PG7
	GPIO_Init(GPIOG, &GPIO_InitStructure);				//初始化
	GPIO_SetBits(GPIOG,GPIO_Pin_7);						//PG7输出1,防止NRF干扰SPI FLASH的通信
	
	W25_SetCS(1);										//SPI FLASH不选中
	bsp_InitSPIBus();		   							//初始化SPI
	SPI1_SetSpeed(SPI_SPEED_21M);						//设置为42M时钟,高速模式 
	s_W25.ChipID = W25QXX_ReadID();						//读取FLASH ID.
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CfgSPIForW25
*	功能说明: 配置STM32内部SPI硬件的工作模式、速度等参数，用于访问SPI接口的串行Flash。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_CfgSPIForW25(void)
{
	bsp_InitSPIBus();
}

/*
*********************************************************************************************************
*	函 数 名: w25_SetCS
*	功能说明: 设置CS。 用于运行中SPI共享。
*	形    参: 无
	返 回 值: 无
*********************************************************************************************************
*/
void W25_SetCS(uint8_t state)
{
	if (state == 0)
	{
		bsp_SpiBusEnter();	/* 占用SPI总线， 用于总线共享 */
		W25_CS_0(); 
	}
	else
	{
		W25_CS_1();
		bsp_SpiBusExit();	/* 释放SPI总线， 用于总线共享 */
	}
}
/*******************************************************************************
* 函数名  ;  W25QXX_ReadSR
* 描述    ： 读取W25QXX的状态寄存器 
* 输入参数： 
* 返回参数： 
*******************************************************************************/
u8 W25QXX_ReadSR(void)   
{
    u8 byte = 0;

    W25_SetCS(0);         //使能片选  
    bsp_SPI1ReadWriteByte(W25X_ReadStatusReg);//发送读取状态寄存器命令    
    byte = bsp_SPI1ReadWriteByte(0xff);         //读取一个字节  
    W25_SetCS(1);         //取消片选     
    return byte;    
}

/*******************************************************************************
* 函数名  ; W25QXX_Write_SR(u8 sr)   
* 描述    ： 写W25QXX状态寄存器 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
* 输入参数： 
* 返回参数： 
*******************************************************************************/
void W25QXX_Write_SR(u8 sr)
{
    W25_SetCS(0);                 //使能器件   
	bsp_SPI1ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	bsp_SPI1ReadWriteByte(sr);               //写入一个字节  
	W25_SetCS(1);                           //取消片选     	       
}

/*
*********************************************************************************************************
*	函 数 名: w25_WaitForWriteEnd
*	功能说明: 采用循环查询的方式等待器件内部写操作完成
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void W25QXX_WaitForWriteEnd(void) 
{    
	W25_CS_0();
	while ((W25QXX_ReadSR() & WIP_Flag) == 0x01); // 等待BUSY位清空
	/*可以加上超时处理*/
	W25_SetCS(1); 
}
	
/*******************************************************************************
* 函数名  ; W25QXX_PowerDown
* 描述    ： 进入掉电模式
* 输入参数： 
* 返回参数： 
*******************************************************************************/
void W25QXX_PowerDown(void)   
{ 
  	W25_SetCS(0);                           //使能器件   
    bsp_SPI1ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	W25_SetCS(1);                           //取消片选     	      
}  

/*******************************************************************************
* 函数名  ; W25QXX_WAKEUP
* 描述    ： 唤醒
* 输入参数： 
* 返回参数： 
*******************************************************************************/
void W25QXX_WAKEUP(void)  
{
  	W25_SetCS(0);                           //使能器件   
    bsp_SPI1ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	W25_SetCS(1);                           //取消片选     	      
}

/*******************************************************************************
* 函数名  ; void W25QXX_Write_Enable(void) 
* 描述    ： 将WEL置位  
* 输入参数： 
* 返回参数： 
*******************************************************************************/
void W25QXX_Write_Enable(void)   
{
	W25_SetCS(0);                           //使能器件   
    bsp_SPI1ReadWriteByte(W25X_WriteEnable);      //发送写使能  
	W25_SetCS(1);                           //取消片选     	      
} 

/*******************************************************************************
* 函数名  ; W25QXX_Write_Disable(void)	
* 描述    ： W25QXX写禁止
* 输入参数： 
* 返回参数： 
*******************************************************************************/
void W25QXX_Write_Disable(void)   
{  
    W25_SetCS(0);                           //使能器件   
    bsp_SPI1ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令
    W25_SetCS(1);                           //取消片选  
}   

/*******************************************************************************
* 函数名  ; W25QXX_ReadID(void)
* 描述    ： 读取芯片ID
* 输入参数： 
* 返回参数：0XEF13,表示芯片型号为W25Q80  
            0XEF14,表示芯片型号为W25Q16    
            0XEF15,表示芯片型号为W25Q32  
            0XEF16,表示芯片型号为W25Q64 
            0XEF17,表示芯片型号为W25Q128 	  
*******************************************************************************/
uint32_t W25QXX_ReadID(void)
{
    uint32_t temp = 0;

    W25_SetCS(0);
    bsp_SPI1ReadWriteByte(0x90);//发送读取ID命令	    
	bsp_SPI1ReadWriteByte(0x00); 	    
	bsp_SPI1ReadWriteByte(0x00); 	    
	bsp_SPI1ReadWriteByte(0x00);
	temp |= bsp_SPI1ReadWriteByte(0xff)<<8;
	temp |= bsp_SPI1ReadWriteByte(0xff);
	W25_SetCS(1);
	return temp;
}

/*******************************************************************************
* 函数名  ; W25QXX_Read()
* 描述    ： 在指定地址开始读取指定长度的数据
* 输入参数： pBuffer:数据存储区 ReadAddr:开始读取的地址(24bit) NumByteToRead:要读取的字节数(最大65535)
* 返回参数： 
*******************************************************************************/
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{
    u16 i;

    W25_SetCS(0);
    bsp_SPI1ReadWriteByte(W25X_ReadData);  //发送读取命令  
    bsp_SPI1ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
    bsp_SPI1ReadWriteByte((u8)((ReadAddr)>>8));   
    bsp_SPI1ReadWriteByte((u8)ReadAddr);
    for(i=0; i<NumByteToRead;i++)
    {
        pBuffer[i] = bsp_SPI1ReadWriteByte(0xff);   //循环读数  
    }
    W25_SetCS(1);
}

/*******************************************************************************
* 函数名  ; void W25QXX_Erase_Sector(u32 Dst_Addr)  
* 描述    ： 擦除一个扇区.擦除一个山区的最少时间:150ms
* 输入参数： SectorAddr:扇区地址 根据实际容量设置
* 返回参数： 
*******************************************************************************/
void W25QXX_Erase_Sector(u32 SectorAddr)  
{
    //监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);	 
 	SectorAddr*=4096;
 	W25QXX_Write_Enable();                  //SET WEL
 	W25QXX_WaitForWriteEnd();   
 	W25_SetCS(0);                           //使能器件   
    bsp_SPI1ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令 
	/* Send SectorAddr high nibble address byte */
	bsp_SPI1ReadWriteByte((SectorAddr & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	bsp_SPI1ReadWriteByte((SectorAddr & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	bsp_SPI1ReadWriteByte(SectorAddr & 0xFF);
	W25_SetCS(1);                           //取消片选     	      
    W25QXX_WaitForWriteEnd();   				   //等待擦除完成
}
/*******************************************************************************
* 函数名  ; W25QXX_Erase_Chip
* 描述    ： 擦除整个芯片	等待时间超长...
* 输入参数： 
* 返回参数： 
*******************************************************************************/
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_WaitForWriteEnd();   
  	W25_SetCS(0);                           //使能器件   
    bsp_SPI1ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
	W25_SetCS(1);                           //取消片选     	      
	W25QXX_WaitForWriteEnd();   				   //等待芯片擦除结束
}  

/*******************************************************************************
* 函数名  ; W25QXX_Write_Page
* 描述    ： 对FLASH按页写入数据，一页最多256个字节，调用本函数写入数据前需要先擦除扇区.
* 输入参数： //pBuffer:数据存储区  //WriteAddr:开始写入的地址(24bit) //NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	
* 返回参数： 
*******************************************************************************/
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u16 i;  
    
    W25QXX_Write_Enable();                  //SET WEL 
	W25_SetCS(0);                           //使能器件   
    bsp_SPI1ReadWriteByte(W25X_PageProgram);      //发送写页命令   
    bsp_SPI1ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    bsp_SPI1ReadWriteByte((u8)((WriteAddr)>>8));   
    bsp_SPI1ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)bsp_SPI1ReadWriteByte(pBuffer[i]);//循环写数  
	W25_SetCS(1);                           //取消片选 
	W25QXX_WaitForWriteEnd();					   //等待写入结束
}


/*******************************************************************************
* 函数名  ; W25QXX_Write_NoCheck
* 描述    ： 无检验写SPI FLASH 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
* 输入参数： pBuffer:数据存储区  WriteAddr:开始写入的地址(24bit)  NumByteToWrite:要写入的字节数(最大65535)
* 返回参数： 
*******************************************************************************/
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
    u16 pageremain;	
    
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{
        W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)
            break;//写入结束了
        else//NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	

			NumByteToWrite -= pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite > 256)
			    pageremain = 256;                   //一次可以写入256个字节
			else 
			    pageremain = NumByteToWrite; 	   //不够256个字节了
		}        
	}
}

u8 W25QXX_BUFFER[4096];	
/*******************************************************************************
* 函数名  ; W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite) 
* 描述    ： 在指定地址开始写入指定长度的数据
* 输入参数： pBuffer:数据存储区  WriteAddr:开始写入的地址(24bit) NumByteToWrite:要写入的字节数(最大65535)  
* 返回参数： 
*******************************************************************************/
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite) 
{
#if 1 //测试程序中写入4096个字节会死机，1024个字节写入正确
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	}
#else	//调用本函数写入数据前需要先擦除扇区
	  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % 256;
  count = 256 - Addr;
  NumOfPage =  NumByteToWrite / 256;
  NumOfSingle = NumByteToWrite % 256;

  if (Addr == 0) /* 若地址与 SPI_FLASH_PageSize 对齐  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      W25QXX_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, 256);
        WriteAddr +=  256;
        pBuffer += 256;
      }

      W25QXX_Write_Page(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* 若地址与 SPI_FLASH_PageSize 不对齐  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        W25QXX_Write_Page(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        W25QXX_Write_Page(pBuffer, WriteAddr, temp);
      }
      else
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / 256;
      NumOfSingle = NumByteToWrite % 256;

      W25QXX_Write_Page(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, 256);
        WriteAddr +=  256;
        pBuffer += 256;
      }

      if (NumOfSingle != 0)
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
#endif
}


//以下为测试程序，W25Q128
/*全部擦除时间 27571ms*/
#if 1
#define EXT_FLASH_SIZE  16*1024*1024    //FLASH 大小为16M字节
#define TEST_ADDR		0			/* 读写测试地址 */
#define TEST_SIZE		2048		/* 读写测试数据大小 */
/*
*********************************************************************************************************
*	函 数 名: sfReadTest
*	功能说明: 读串行Flash测试
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void W25QXXReadTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* 起始地址 = 0， 数据长度为 256 */
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	W25QXX_Read(buf, TEST_ADDR, TEST_SIZE);
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */
	printf("读串行Flash成功，数据如下：\r\n");

	/* 打印数据 */
	for (i = 0; i < TEST_SIZE; i++)
	{
		printf(" %02X", buf[i]);

		if ((i & 31) == 31)
		{
			printf("\r\n");	/* 每行显示16字节数据 */
		}
		else if ((i & 31) == 15)
		{
			printf(" - ");
		}
	}

	/* 打印读速度 */
	printf("数据长度: %d字节, 读耗时: %dms, 读速度: %d Bytes/s\r\n", TEST_SIZE, iTime2 - iTime1, (TEST_SIZE * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	函 数 名: sfTestReadSpeed
*	功能说明: 测试串行Flash读速度。读取整个串行Flash的数据，最后打印结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void W25QXXTestReadSpeed(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];
	uint32_t uiAddr;

	/* 起始地址 = 0， 数据长度为 256 */
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	uiAddr = 0;
	for (i = 0; i < EXT_FLASH_SIZE / TEST_SIZE; i++, uiAddr += TEST_SIZE)
	{
		W25QXX_Read(buf, uiAddr, TEST_SIZE);
	}
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	printf("数据长度: %d字节, 读耗时: %dms\r\n", EXT_FLASH_SIZE, iTime2 - iTime1);
}

/*
*********************************************************************************************************
*	函 数 名: sfWriteTest
*	功能说明: 写串行Flash测试
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void W25QXXWriteTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* 填充测试缓冲区 */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = 0x55;
	}

	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
    W25QXX_Write(buf,TEST_ADDR,TEST_SIZE);
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */
	printf("写串行Flash成功！\r\n");

	/* 打印读速度 */
	printf("数据长度: %d字节, 写耗时: %dms\r\n", TEST_SIZE, iTime2 - iTime1);
}

/*
*********************************************************************************************************
*	函 数 名: sfWriteAll
*	功能说明: 写串行EEPROM全部数据
*	形    参：_ch : 写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
 void W25QXXWriteAll(uint8_t _ch)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[4 * 1024];

	/* 填充测试缓冲区 */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = _ch;
	}

	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	for (i = 0; i < EXT_FLASH_SIZE/ 4096; i++)
	{
		 W25QXX_Write(buf, i * 4096, 4096) ;	
	}
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	printf("数据长度: %dK字节, 写耗时: %dms\r\n", EXT_FLASH_SIZE / 1024, iTime2 - iTime1);
}

/*
*********************************************************************************************************
*	函 数 名: sfErase
*	功能说明: 擦除串行Flash
*	形    参：无
*	返 回 值:  
*********************************************************************************************************
*/
 void W25QXXErase(void)
{
	int32_t iTime1, iTime2;

	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	W25QXX_Erase_Chip();
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	printf("擦除串行Flash完成！, 耗时: %dms\r\n", iTime2 - iTime1);
	return;
}

#endif

























