/*
*********************************************************************************************************
*	                                  
*	ģ������ : printfģ��    
*	�ļ����� : bsp_printf.c
*	��    �� : V2.0
*	˵    �� : ʵ��printf��scanf�����ض��򵽴���1����֧��printf��Ϣ��USART1
*				ʵ���ض���ֻ��Ҫ���2������:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				����KEIL MDK������������ѡ������Ҫ��MicorLibǰ��򹳣����򲻻������ݴ�ӡ��USART1��
*				uart1���ã�115200 ��У�飬1��ֹͣλ
*				
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2016-08-13 9527   ֻʵ�ַ���fputc
*
*
*********************************************************************************************************
*/
#include "bsp.h"

#define USE_UART1
//#define USE_UART2       /* ĸͷ����GPIO���� */
//#define USE_UART3       /* ��ͷ����GPIO���� */

#if (AM_CFG_DEBUG_UART == 0)
    #define PRINTF_UART     USART1 
#else (AM_CFG_DEBUG_UART == 1)

    #define PRINTF_UART     USART2
#endif

#define PRINTF_BAUD		115200  //���ڲ�����

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitPrintfUart1
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸��δ�����жϡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitPrintf( void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_UART1	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); /* ʹ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);/* ʹ��USARTʱ�� */

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1

	/* ����GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
#endif
#ifdef USE_UART2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); /* ʹ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);/* ʹ��USARTʱ�� */

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2

	/* ����GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
#endif
#ifdef USE_UART3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); /* ʹ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);/* ʹ��USARTʱ�� */

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART1

	/* ����GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 


#endif
	/* USARTx configuration ----------------------------------------------------*/
	/* USARTx configured as follows:
		- BaudRate = 5250000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 8
			 is: (USART APB Clock / 8) 
			 Example: 
				- (USART1 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
				- (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 16
			 is: (USART APB Clock / 16) 
			 Example: (USART1 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
			 Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
		- Word Length = 8 Bits
		- one Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/ 
	USART_InitStructure.USART_BaudRate = PRINTF_BAUD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(PRINTF_UART, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(PRINTF_UART, ENABLE);
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DEBUG_USART */
		USART_SendData(PRINTF_UART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(PRINTF_UART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(PRINTF_UART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(PRINTF_UART);
}
