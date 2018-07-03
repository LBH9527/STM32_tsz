/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H

//#include "stm32f4xx.h"
enum {
    LED_RED,
    LED_GREEN,
    LEDn,
};
/* ���ⲿ���õĺ������� */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t no);
void bsp_LedOff(uint8_t no);
void bsp_LedToggle(uint8_t no);
#endif

