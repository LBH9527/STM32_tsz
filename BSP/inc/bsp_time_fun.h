/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_Time_fun.h
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*
*
*********************************************************************************************************
*/
#ifndef __BSP_TIME_FUN_H
#define __BSP_TIME_FUN_H

/* �ṩ������C�ļ����õĺ��� */
void bsp_InitHardTimer(void);
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack);
void bsp_InitBacklightSetTimer(void);
void bsp_InitModbusTimer(void);
void bsp_StartModbusTimer(uint16_t TimeOut, void * _pCallBack);
#endif
