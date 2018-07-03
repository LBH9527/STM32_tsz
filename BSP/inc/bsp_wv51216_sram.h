/*
*********************************************************************************************************
*	                                  
*	模块名称 : 外部SRAM操作模块头文件
*	文件名称 : bsp_wv51216_sram.h
*	版    本 : V1.0
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
