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

#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "am_common.h"
#include "am_rngbuf.h"

/* �����豸�ṹ�� */
typedef struct
{	
	uint8_t	    *write_buffer_data;		/* ���ͻ����� */
	uint8_t	    *read_buffer_data;		/* ���ջ����� */
    am_rngbuf   *write_buffer;          /* ���ջ��λ����� */
    am_rngbuf   *read_buffer;           /* ���ͻ��λ����� */


    /**\brief ����UART���ͺ��� */
    int (*pfn_uart_tx_startup)(void *p_drv);

    /**\brief ���ô��ڻص����� */
    int (*pfn_uart_callback_set)(void  *p_drv,
                                 int    callback_type,
                                 int  (*pfn_callback)(void *),
                                 void  *p_callback_arg);

	
	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
}uart_serve_t;
//extern UART_S g_sUart;
	
/* ���ⲿ���õĺ������� */
void bsp_InitUart(void);
void bsp_UartSend(uint8_t *str, uint8_t len);
void bsp_Set485Baud(u16 baud);
#endif

