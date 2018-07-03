/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动
*	文件名称 : bsp_w25qxx_flash.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#ifndef _BSP_W25QXX_FLASH_H_
#define _BSP_W25QXX_FLASH_H_
#include <stdint.h>

typedef struct
{
	uint32_t ChipID;		/* 芯片ID */
	uint32_t TotalSize;		/* 总容量 */
	uint16_t PageSize;		/* 页面大小 */
}W25_S;

void bsp_InitW25QxxFlash(void);
uint32_t W25QXX_ReadID(void);  	    		//读取FLASH ID
void W25QXX_Write_Enable(void);  		//写使能 
void W25QXX_Write_Disable(void);		//写保护
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void W25QXX_Erase_Chip(void);    	  	//整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void W25QXX_PowerDown(void);        	//进入掉电模式
void W25QXX_WAKEUP(void);				//唤醒
//测试程序
#if 1
void W25QXXReadTest(void);
void W25QXXTestReadSpeed(void);
void W25QXXWriteTest(void);
 void W25QXXWriteAll(uint8_t _ch);
void W25QXXErase(void);
#endif

#endif

