/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 工程配置文件，包括系统，板级和硬件配置
 * \sa am_prj_config.h
 * \internal
 * \par Modification history
 * - 1.00 14-12-23  tee, first implementation.
 * \endinternal
 */
 
#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H


/**
 * \addtogroup am_if_prj_config
 * \copydoc am_prj_config.h
 * @{
 */

/**
 * \brief 一些全局使用的单实例设备，默认在系统启动时初始化，以便全局使用
 * 
 *  如 GPIO、DMA 、INT 等。如果确定应用程序不使用这些资源，可以将对应的宏值修改为0。
 *  如无特殊情况，不建议禁能（将对应宏值修改为0）这些默认初始化的单实例设备。
 *
 * @{
 */
 
/** \brief 为1，启动时，完成时钟初始化  */
#define AM_CFG_CLK_ENABLE                1
 
/** \brief 为1，初始化 GPIO 的相关功能  */
#define AM_CFG_GPIO_ENABLE               1

/** \brief 为1，初始化 INT 相关功能     */
#define AM_CFG_INT_ENABLE                1

/** \brief 为1，初始化 DMA 相关功能     */
#define AM_CFG_DMA_ENABLE                1

/** @} */


/**
 * \name 板级初始化配置
 *
 * 默认可以使用板上的一些资源，如 LED，蜂鸣器等，如果不使用，应禁止相应的宏
 *
 * @{
 */
 
/** 
 * \brief 如果为1，则初始化延时函数
 *
 * 初始化后可以使用am_mdelay() 和 am_udelay()延时函数
 *
 * 注意，默认的延时函数由 am_delay.c文件实现，如有特别需求，可以自行更新为其它实现方式
 *
 */
#define AM_CFG_DELAY_ENABLE          1

/** 
 * \brief 如果为1，则初始化led的相关功能，板上默认有两个LED
 *
 * ID: 0 --- PIO0.20 （需要短接跳线帽 J9）
 * ID: 1 --- PIO0.21 （需要短接跳线帽 J10）
 */
#define AM_CFG_LED_ENABLE            1


/** 
 * \brief 如果为1，则初始化蜂鸣器相关功能
 *
 * 默认使用SCT_OUT （PIO0_24）输出PWM （需要短接跳线帽 J7）
 */
#define AM_CFG_BUZZER_ENABLE         1

/** \brief 蜂鸣器的PWM频率              */
#define AM_CFG_BUZZER_PWM_FREQ       2500
    
/** \brief PWM的占空比，50代表50%       */
#define AM_CFG_BUZZER_PWM_DUTY       50

/** 
 * \brief 如果为1，则初始化软件定时器
 * 
 * 软件定时器默认使用 MRT 定时器 
 * 软件定时器的使用详见 am_softimer.h
 */
#define AM_CFG_SOFTIMER_ENABLE       1

/** \brief 软件定时器使用的MRT通道    */
#define AM_CFG_SOFTIMER_CHAN         0

/** 
 * \brief 如果为1，则初始化串口调试输出，调试串口默认为UART0
 *
 * UART0的TXD引脚默认为 PIO0_4 只需要将PIO0_4连接到串口模块的输入引脚，
 * 即可看到 AM_DBG_INFO() 打印的调试信息
 */
#define AM_CFG_DEBUG_ENABLE          1

/** 
 * \brief 使用的调试串口，默认UART0
 * 0 - UART0; 1 - UART1; 2 - UART2
 */
#define AM_CFG_DEBUG_UART            0

/**
 * \brief 配置调试串口输出的波特率
 */
#define AM_CFG_DEBUG_BAUDRATE        115200

/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_PRJ_CONFIG_H */

/* end of file */
