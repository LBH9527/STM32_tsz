/*
*********************************************************************************************************
*	                                  
*	模块名称 : printf模块    
*	文件名称 : bsp_printf.c
*	版    本 : V2.0
*	说    明 : 实现printf和scanf函数重定向到串口1，即支持printf信息到USART1
*				实现重定向，只需要添加2个函数:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				对于KEIL MDK编译器，编译选项中需要在MicorLib前面打钩，否则不会有数据打印到USART1。
*				uart1配置：115200 无校验，1个停止位
*				
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2016-08-13 9527   只实现发送fputc
*
*
*********************************************************************************************************
*/
#include "bsp.h"

#define USE_UART1
//#define USE_UART2       /* 母头串口GPIO定义 */
//#define USE_UART3       /* 公头串口GPIO定义 */

#if (AM_CFG_DEBUG_UART == 0)
    #define PRINTF_UART     USART1 
#else (AM_CFG_DEBUG_UART == 1)

    #define PRINTF_UART     USART2
#endif

#define PRINTF_BAUD		115200  //串口波特率

/*
*********************************************************************************************************
*	函 数 名: bsp_InitPrintfUart1
*	功能说明: 初始化CPU的USART1串口硬件设备。未启用中断。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitPrintf( void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_UART1	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); /* 使能GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);/* 使能USART时钟 */

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1

	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
#endif
#ifdef USE_UART2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); /* 使能GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);/* 使能USART时钟 */

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2

	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
#endif
#ifdef USE_UART3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); /* 使能GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);/* 使能USART时钟 */

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART1

	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
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

///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口DEBUG_USART */
		USART_SendData(PRINTF_UART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(PRINTF_UART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(PRINTF_UART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(PRINTF_UART);
}
