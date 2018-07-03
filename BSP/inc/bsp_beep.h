/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器模块
*	文件名称 : bsp_beep.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*
*********************************************************************************************************
*/

#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include <stdint.h>
/******************************* Beep相关数据声明***************************/
typedef struct _BEEP_T
{
	uint8_t Enalbe;
	uint8_t State;
	uint16_t BeepTime;
	uint16_t StopTime;
	uint16_t Cycle;
	uint16_t Count;
	uint16_t CycleCount;
}BEEP_T;

/********************************** 声明函数 ***************************************/
void bsp_InitBeep(void);
void bsp_BeepStart(uint8_t BeepTime, uint16_t StopTime, uint16_t Cycle);
void bsp_BeepStop(void);
void bsp_BeepKeyTone(void);
void bsp_BeepTickService(void);

#endif
