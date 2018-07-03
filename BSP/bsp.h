/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
*
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H

/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */


/* 这个宏仅用于调试阶段排错 */
//#define BSP_Printf		printf
//#define BSP_Printf(...)
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board_config.h"



/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */

#include "bsp_SysTick.h"
#include "bsp_Userlib.h"
#include "bsp_Msg.h"
#include "bsp_time_fun.h"

#include "bsp_config.h"
#include "bsp_printf.h"
#include "bsp_led.h"
#include "bsp_key.h"
//#include "bsp_beep.h"
//#include "bsp_Rs485.h"
#include "bsp_i2c_gpio.h"
#include "bsp_eeprom_24cxx.h"

#include "lcd_nt35510.h"
#include "bsp_lcd.h"
#include "bsp_lcdTouch.h"

//#include "bsp_ds18b20.h"
#include "bsp_spi_bus.h"
#include "bsp_w25qxx_flash.h"

//#include "bsp_wv51216_sram.h"
////#include "bsp_cpu_flash.h"
//#include "bsp_sdio_sd.h"

#include "bsp_rtc.h"
//#include "bsp_iwdg.h"

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif


