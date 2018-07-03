/*
*********************************************************************************************************
*
*	模块名称 : bsp参数设置
*	文件名称 : bsp_config.h
*	版    本 : V2.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/
#ifndef BSP_CONFIG_H_
#define BSP_CONFIG_H_

#define USE_SMALL_FONT 			1			//1：采用Flash内的字库。0：采用全字库
#define BUTTON_BEEP()	BEEP_KeyTone();		/* 按键提示音 */
//#define BUTTON_BEEP()						/* 无按键提示音 */

#define LCD_DIR_DEFAULT			0			//LCD显示方向默认值，方向说明在LCD_NT35510.c中

#define LCD_BACKLIGHT_DEFAULT   5			//背光默认值#ifndef _BSP_CONFIG_H_
#define LCD_BACKLIGHT_SET       0           //LCD背光可以设置1 ， 不可以设置 0


#endif


