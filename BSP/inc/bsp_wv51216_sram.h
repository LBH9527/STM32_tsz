/*
*********************************************************************************************************
*	                                  
*	ģ������ : �ⲿSRAM����ģ��ͷ�ļ�
*	�ļ����� : bsp_wv51216_sram.h
*	��    �� : V1.0
*
*********************************************************************************************************
*/
#ifndef _BSP_WV51216_SRAM_H
#define _BSP_WV51216_SRAM_H

void bsp_InitFSMC_SRAM(void);
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);
void fsmc_sram_test(void);
void fsmc_sram_WriteTest(void);
void fsmc_sram_ReadTest(void);
#endif
