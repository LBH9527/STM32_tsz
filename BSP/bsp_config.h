/*
*********************************************************************************************************
*
*	ģ������ : bsp��������
*	�ļ����� : bsp_config.h
*	��    �� : V2.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/
#ifndef BSP_CONFIG_H_
#define BSP_CONFIG_H_

#define USE_SMALL_FONT 			1			//1������Flash�ڵ��ֿ⡣0������ȫ�ֿ�
#define BUTTON_BEEP()	BEEP_KeyTone();		/* ������ʾ�� */
//#define BUTTON_BEEP()						/* �ް�����ʾ�� */

#define LCD_DIR_DEFAULT			0			//LCD��ʾ����Ĭ��ֵ������˵����LCD_NT35510.c��

#define LCD_BACKLIGHT_DEFAULT   5			//����Ĭ��ֵ#ifndef _BSP_CONFIG_H_
#define LCD_BACKLIGHT_SET       0           //LCD�����������1 �� ���������� 0


#endif


