/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart.c
*	版    本 : V1.0
*	说    明 : 采用串口中断+FIFO模式实现多个串口的同时访问
*	修改记录 :
*		版本号  日期       作者    说明
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "bsp_uart.h"


#define UART_TXBUF_SIZE    12		/* 发送缓冲区大小 */
#define UART_RXBUF_SIZE    12     	/* 接收缓冲区大小 */

//const u32 BaudTab[] = {4800,9600,19200,38400,57600,115200};
#define UART2_BAUD			9600 	/* 默认为9600 */

am_rngbuf write_buffer;
uint8_t write_buffer_data[UART_TXBUF_SIZE];
am_rngbuf read_buffer;
uint8_t read_buffer_data[UART_RXBUF_SIZE];
uart_serve_t g_Uart;

static void Uart2_InitVar(void);
static void Uart2_InitHard(void);
static void RS485_InitTXE(void);
/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	Uart2_InitHard();	/* 配置串口的硬件参数(波特率等) */	
    RS485_InitTXE();	/* 配置RS485芯片的发送使能硬件，配置为推挽输出 */
	Uart2_InitVar();	/* 初始化全局变量 */
}
/*
*********************************************************************************************************
*	函 数 名: Uart2_InitHard
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 
*	返 回 值: 无
*********************************************************************************************************
*/
static void Uart2_InitHard(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能USART2时钟
	
    //USART2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);      

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2);//GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);//GPIOA2复用为USART2
	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//USART_InitStructure.USART_BaudRate = BaudTab[s_Param.Baud485];  
	USART_InitStructure.USART_BaudRate = UART2_BAUD;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART2, &USART_InitStructure);       //初始化串口2
	
	USART_Cmd(USART2, ENABLE);                      //使能串口2
	USART_ClearFlag(USART2, USART_FLAG_TC);         //清除发送完成标志位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //接收中断使能
	
    //Uart2 NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//相应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     

    NVIC_Init(&NVIC_InitStructure);

}
/*
*********************************************************************************************************
*	函 数 名: RS485_InitTXE
*	功能说明: 配置RS485发送使能IO口 PG8 TXE
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void RS485_InitTXE(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);	//使能USART2时钟		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 		//GPIOG8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 	//上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); 			//初始化PG8
}


/*
*********************************************************************************************************
*	函 数 名: RS485_SendBefor
*	功能说明: 发送数据前的准备工作。对于RS485通信，请设置RS485芯片为发送状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_sUart2.SendBefor = RS485_SendBefor
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendBefor(void)
{
	/* 切换RS485收发芯片为发送模式 */	
	GPIO_SetBits(GPIOG, GPIO_Pin_8);         		//高电平为发送模式	
}

/*
*********************************************************************************************************
*	函 数 名: RS485_SendOver
*	功能说明: 发送一串数据结束后的善后处理。对于RS485通信，请设置RS485芯片为接收状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_tUart2.SendOver = RS485_SendOver
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendOver(void)
{
	/* 切换RS485收发芯片为接收模式 */
	GPIO_ResetBits(GPIOG, GPIO_Pin_8);         //低电平接受模式	
}

/*
*********************************************************************************************************
*	函 数 名: RS485_ReciveNew
*	功能说明: 接收到新的数据
*	形    参: _byte 接收到的新数据
*	返 回 值: 无
*********************************************************************************************************
*/
extern void MODBUS_ReciveNew(uint8_t _byte);
void RS485_ReciveNew(uint8_t _byte)
{
//	MODBUS_ReciveNew(_byte);
}
/*
*********************************************************************************************************
*	函 数 名: InitUartVar
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Uart2_InitVar(void)
{	
	g_Uart.write_buffer_data = write_buffer_data;					/* 发送缓冲区指针 */
    g_Uart.read_buffer_data = read_buffer_data;
    g_Uart.write_buffer = &write_buffer;
    g_Uart.read_buffer = &read_buffer;
	g_Uart.SendBefor = RS485_SendBefor;			/* 发送数据前的回调函数 */
	g_Uart.SendOver = RS485_SendOver;			/* 发送完毕后的回调函数 */
	g_Uart.ReciveNew = RS485_ReciveNew;			/* 接收到新数据后的回调函数 */
    
    am_rngbuf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    am_rngbuf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}
//uint32_t uart_write_free(void)
//{
////    return am_rngbuf_flush(&write_buffer);
//}

//int32_t uart2_write_data(uint8_t *data, uint16_t size)
//{
////    uint32_t cnt;

////    cnt = circ_buf_write(&write_buffer, data, size);
//    
//}

int32_t uart2_read_data(uint8_t *data, uint16_t size)
{
    return am_rngbuf_get(&read_buffer, data, size);
    
}

/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
void UartIRQ(void)
{
	//接收到一个数据，保存串口数据，重启定时器
    uint8_t data, data_temp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收中断
    {
        uint32_t free;
        
        
		data = USART_ReceiveData(USART2); 
        if(am_rngbuf_isfull(g_Uart.read_buffer) == TRUE)
        {
            // Drop oldest
            am_rngbuf_getchar(g_Uart.read_buffer,&data_temp);       //读出一个数据并丢弃            
        }
        am_rngbuf_putchar(g_Uart.read_buffer, data);     
    }
		/* 处理发送缓冲区空中断 */
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
        if(am_rngbuf_isempty(g_Uart.write_buffer) == TRUE)
        {
             /* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			/* 使能数据发送完毕中断 */
			USART_ITConfig(USART2, USART_IT_TC, ENABLE);
        }
        else
        {
            /* 从发送FIFO取1个字节写入串口发送数据寄存器 */
            am_rngbuf_getchar(g_Uart.write_buffer, &data);
			USART_SendData(USART2, data);
        }
	}/* 检查是否发送完成，发送完成则关闭发送，使能接收 */
	else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)//发送完成中断
	{		
        if(am_rngbuf_isempty(g_Uart.write_buffer) == TRUE)
        {
            USART_ITConfig(USART2, USART_IT_TC, DISABLE);//禁止发送完成中断
			g_Uart.SendOver();       
        }
	}
}
