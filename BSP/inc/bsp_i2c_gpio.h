/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动模块
*	文件名称 : bsp_i2c_gpio.h
*	版    本 : V1.0
*	说    明 : 头文件。
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

/* 提供给其他C文件调用的函数 */
void bsp_InitI2C(void);
void I2C_Stop(void);
void I2C_Start(void);
u8 I2C_Read_NoACK(void);
u8 I2C_Read_ACK(void);
u8 I2C_Write(u8 data);
u8 I2C_CheckDevice(u8 Address);
#endif

