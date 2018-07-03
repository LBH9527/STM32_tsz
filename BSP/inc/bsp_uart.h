/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "am_common.h"
#include "am_rngbuf.h"

/* 串口设备结构体 */
typedef struct
{	
	uint8_t	    *write_buffer_data;		/* 发送缓冲区 */
	uint8_t	    *read_buffer_data;		/* 接收缓冲区 */
    am_rngbuf   *write_buffer;          /* 接收环形缓冲区 */
    am_rngbuf   *read_buffer;           /* 发送环形缓冲区 */


    /**\brief 启动UART发送函数 */
    int (*pfn_uart_tx_startup)(void *p_drv);

    /**\brief 设置串口回调函数 */
    int (*pfn_uart_callback_set)(void  *p_drv,
                                 int    callback_type,
                                 int  (*pfn_callback)(void *),
                                 void  *p_callback_arg);

	
	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}uart_serve_t;
//extern UART_S g_sUart;
	
/* 供外部调用的函数声明 */
void bsp_InitUart(void);
void bsp_UartSend(uint8_t *str, uint8_t len);
void bsp_Set485Baud(u16 baud);
#endif

