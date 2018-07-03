/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.h
*	版    本 : V1.0
*	说    明 : 头文件
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
/* 供外部调用的函数声明 */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t no);
void bsp_LedOff(uint8_t no);
void bsp_LedToggle(uint8_t no);
#endif

