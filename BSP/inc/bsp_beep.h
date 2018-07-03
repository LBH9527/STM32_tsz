/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : bsp_beep.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*
*********************************************************************************************************
*/

#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include <stdint.h>
/******************************* Beep�����������***************************/
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

/********************************** �������� ***************************************/
void bsp_InitBeep(void);
void bsp_BeepStart(uint8_t BeepTime, uint16_t StopTime, uint16_t Cycle);
void bsp_BeepStop(void);
void bsp_BeepKeyTone(void);
void bsp_BeepTickService(void);

#endif
