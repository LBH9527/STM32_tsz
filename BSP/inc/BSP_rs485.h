/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
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


/* �����豸�ṹ�� */
typedef struct
{	
	uint8_t	*pTxBuf;			/* ���ͻ����� */
	uint8_t	*pRxBuf;			/* ���ջ����� */
	uint16_t TxBufSize;			/* ���ͻ�������С */
	uint16_t RxBufSize;			/* ���ջ�������С*/
	__IO uint16_t TxWrite;		/* ���ͻ�����дָ�� */
	__IO uint16_t TxRead;		/* ���ͻ�������ָ�� */
//	__IO uint16_t TxCount;		/* ���ͻ��������ݸ��� */

	__IO uint16_t RxWrite;		/* ���ջ�����дָ�� */
	__IO uint16_t RxRead;		/* ���ջ�������ָ�� */
//	__IO uint16_t RxCount;		/* ���ջ��������ݸ��� */
	
	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
}UART_S;
//extern UART_S g_sUart;
	
/* ���ⲿ���õĺ������� */
void bsp_InitUart(void);
void bsp_UartSend(uint8_t *str, uint8_t len);
void bsp_Set485Baud(u16 baud);
#endif

