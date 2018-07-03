/*
*********************************************************************************************************
*
*	ģ������ : I2C��������ģ��
*	�ļ����� : bsp_i2c_gpio.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ���
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

/* �ṩ������C�ļ����õĺ��� */
void bsp_InitI2C(void);
void I2C_Stop(void);
void I2C_Start(void);
u8 I2C_Read_NoACK(void);
u8 I2C_Read_ACK(void);
u8 I2C_Write(u8 data);
u8 I2C_CheckDevice(u8 Address);
#endif

