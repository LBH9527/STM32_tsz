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

#ifndef _BSP_USART485_H_
#define _BSP_USART485_H_


//#define FALSE	0
//#define TRUE	1

extern const u32 BaudTab[];
//extern u8 UsartRxBuf[UART_RXBUF_SIZE];


/* 串口设备结构体 */
typedef struct
{	
	uint8_t	*pTxBuf;			/* 发送缓冲区 */
	uint8_t	*pRxBuf;			/* 接收缓冲区 */
	uint16_t TxBufSize;			/* 发送缓冲区大小 */
	uint16_t RxBufSize;			/* 接收缓冲区大小*/
	__IO uint16_t TxWrite;		/* 发送缓冲区写指针 */
	__IO uint16_t TxRead;		/* 发送缓冲区读指针 */
//	__IO uint16_t TxCount;		/* 发送缓冲区数据个数 */

	__IO uint16_t RxWrite;		/* 接收缓冲区写指针 */
	__IO uint16_t RxRead;		/* 接收缓冲区读指针 */
//	__IO uint16_t RxCount;		/* 接收缓冲区数据个数 */
	
	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_S;
//extern UART_S g_sUart;
	
/* 供外部调用的函数声明 */
void bsp_InitUart(void);
void bsp_UartSend(uint8_t *str, uint8_t len);
void bsp_Set485Baud(u16 baud);
#endif

