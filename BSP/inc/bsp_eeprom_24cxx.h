/*
*********************************************************************************************************
*
*	模块名称 : 串行EEPROM 24xx02驱动模块
*	文件名称 : bsp_eeprom_24xx.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2012-10-12 armfly  ST固件库版本 V2.1.0
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_EEPROM_24CXX_H
#define _BSP_EEPROM_24CXX_H

//#define AT24C02
#define AT24C16

#ifdef AT24C02
	#define EE_MODEL_NAME       "AT24C02"
	#define EE_DEV_ADDR         0xA0            /* 设备地址 */
	#define EE_PAGE_SIZE        8               /* 页面大小(字节) */
	#define EE_SIZE             (8*32)          /* 总容量(字节) */
	#define EE_TYPE				255				/* 最大地址 = 总容量 - 1 */
#endif

#ifdef AT24C16
	#define EE_MODEL_NAME		"AT24C16"
	#define EE_DEV_ADDR			0xA0			/* 设备地址 */
	#define EE_PAGE_SIZE		16				/* 页面大小(字节) */
	#define EE_SIZE				(16*128)		/* 总容量(字节) */
	#define EE_TYPE				2047			/* 最大地址 = 总容量 - 1 */
#endif

/* 提供给其他C文件调用的函数 */
void bsp_InitAT24CXX(void);
void bsp_U16SaveToE2(uint16_t _usAddress, uint16_t *_pWriteBuf, uint16_t _usSize);
void bsp_E2DataReload(uint16_t _usAddress, uint16_t *_pReadBuf, uint16_t _usSize);
#endif
