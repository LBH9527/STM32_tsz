/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_Time_fun.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
*
*
*********************************************************************************************************
*/
#ifndef __BSP_TIME_FUN_H
#define __BSP_TIME_FUN_H

/* 提供给其他C文件调用的函数 */
void bsp_InitHardTimer(void);
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack);
void bsp_InitBacklightSetTimer(void);
void bsp_InitModbusTimer(void);
void bsp_StartModbusTimer(uint16_t TimeOut, void * _pCallBack);
#endif
