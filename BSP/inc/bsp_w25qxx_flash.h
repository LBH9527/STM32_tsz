/*
*********************************************************************************************************
*
*	ģ������ : SPI��������
*	�ļ����� : bsp_w25qxx_flash.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#ifndef _BSP_W25QXX_FLASH_H_
#define _BSP_W25QXX_FLASH_H_
#include <stdint.h>

typedef struct
{
	uint32_t ChipID;		/* оƬID */
	uint32_t TotalSize;		/* ������ */
	uint16_t PageSize;		/* ҳ���С */
}W25_S;

void bsp_InitW25QxxFlash(void);
uint32_t W25QXX_ReadID(void);  	    		//��ȡFLASH ID
void W25QXX_Write_Enable(void);  		//дʹ�� 
void W25QXX_Write_Disable(void);		//д����
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void W25QXX_Erase_Chip(void);    	  	//��Ƭ����
void W25QXX_Erase_Sector(u32 Dst_Addr);	//��������
void W25QXX_PowerDown(void);        	//�������ģʽ
void W25QXX_WAKEUP(void);				//����
//���Գ���
#if 1
void W25QXXReadTest(void);
void W25QXXTestReadSpeed(void);
void W25QXXWriteTest(void);
 void W25QXXWriteAll(uint8_t _ch);
void W25QXXErase(void);
#endif

#endif

