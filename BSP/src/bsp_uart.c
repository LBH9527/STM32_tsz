/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart.c
*	��    �� : V1.0
*	˵    �� : ���ô����ж�+FIFOģʽʵ�ֶ�����ڵ�ͬʱ����
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "bsp_uart.h"


#define UART_TXBUF_SIZE    12		/* ���ͻ�������С */
#define UART_RXBUF_SIZE    12     	/* ���ջ�������С */

//const u32 BaudTab[] = {4800,9600,19200,38400,57600,115200};
#define UART2_BAUD			9600 	/* Ĭ��Ϊ9600 */

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
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	Uart2_InitHard();	/* ���ô��ڵ�Ӳ������(�����ʵ�) */	
    RS485_InitTXE();	/* ����RS485оƬ�ķ���ʹ��Ӳ��������Ϊ������� */
	Uart2_InitVar();	/* ��ʼ��ȫ�ֱ��� */
}
/*
*********************************************************************************************************
*	�� �� ��: Uart2_InitHard
*	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ��ʺ���STM32-F4������
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Uart2_InitHard(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//ʹ��USART2ʱ��
	
    //USART2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);      

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2);//GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);//GPIOA2����ΪUSART2
	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//USART_InitStructure.USART_BaudRate = BaudTab[s_Param.Baud485];  
	USART_InitStructure.USART_BaudRate = UART2_BAUD;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART2, &USART_InitStructure);       //��ʼ������2
	
	USART_Cmd(USART2, ENABLE);                      //ʹ�ܴ���2
	USART_ClearFlag(USART2, USART_FLAG_TC);         //���������ɱ�־λ
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //�����ж�ʹ��
	
    //Uart2 NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     

    NVIC_Init(&NVIC_InitStructure);

}
/*
*********************************************************************************************************
*	�� �� ��: RS485_InitTXE
*	����˵��: ����RS485����ʹ��IO�� PG8 TXE
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void RS485_InitTXE(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);	//ʹ��USART2ʱ��		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 		//GPIOG8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 	//����
	GPIO_Init(GPIOG,&GPIO_InitStructure); 			//��ʼ��PG8
}


/*
*********************************************************************************************************
*	�� �� ��: RS485_SendBefor
*	����˵��: ��������ǰ��׼������������RS485ͨ�ţ�������RS485оƬΪ����״̬��
*			  ���޸� UartVarInit()�еĺ���ָ����ڱ������������� g_sUart2.SendBefor = RS485_SendBefor
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_SendBefor(void)
{
	/* �л�RS485�շ�оƬΪ����ģʽ */	
	GPIO_SetBits(GPIOG, GPIO_Pin_8);         		//�ߵ�ƽΪ����ģʽ	
}

/*
*********************************************************************************************************
*	�� �� ��: RS485_SendOver
*	����˵��: ����һ�����ݽ�������ƺ�������RS485ͨ�ţ�������RS485оƬΪ����״̬��
*			  ���޸� UartVarInit()�еĺ���ָ����ڱ������������� g_tUart2.SendOver = RS485_SendOver
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_SendOver(void)
{
	/* �л�RS485�շ�оƬΪ����ģʽ */
	GPIO_ResetBits(GPIOG, GPIO_Pin_8);         //�͵�ƽ����ģʽ	
}

/*
*********************************************************************************************************
*	�� �� ��: RS485_ReciveNew
*	����˵��: ���յ��µ�����
*	��    ��: _byte ���յ���������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern void MODBUS_ReciveNew(uint8_t _byte);
void RS485_ReciveNew(uint8_t _byte)
{
//	MODBUS_ReciveNew(_byte);
}
/*
*********************************************************************************************************
*	�� �� ��: InitUartVar
*	����˵��: ��ʼ��������صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Uart2_InitVar(void)
{	
	g_Uart.write_buffer_data = write_buffer_data;					/* ���ͻ�����ָ�� */
    g_Uart.read_buffer_data = read_buffer_data;
    g_Uart.write_buffer = &write_buffer;
    g_Uart.read_buffer = &read_buffer;
	g_Uart.SendBefor = RS485_SendBefor;			/* ��������ǰ�Ļص����� */
	g_Uart.SendOver = RS485_SendOver;			/* ������Ϻ�Ļص����� */
	g_Uart.ReciveNew = RS485_ReciveNew;			/* ���յ������ݺ�Ļص����� */
    
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
*	�� �� ��: UartIRQ
*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
*	��    ��: _pUart : �����豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UartIRQ(void)
{
	//���յ�һ�����ݣ����洮�����ݣ�������ʱ��
    uint8_t data, data_temp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//�����ж�
    {
        uint32_t free;
        
        
		data = USART_ReceiveData(USART2); 
        if(am_rngbuf_isfull(g_Uart.read_buffer) == TRUE)
        {
            // Drop oldest
            am_rngbuf_getchar(g_Uart.read_buffer,&data_temp);       //����һ�����ݲ�����            
        }
        am_rngbuf_putchar(g_Uart.read_buffer, data);     
    }
		/* �����ͻ��������ж� */
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
        if(am_rngbuf_isempty(g_Uart.write_buffer) == TRUE)
        {
             /* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			/* ʹ�����ݷ�������ж� */
			USART_ITConfig(USART2, USART_IT_TC, ENABLE);
        }
        else
        {
            /* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
            am_rngbuf_getchar(g_Uart.write_buffer, &data);
			USART_SendData(USART2, data);
        }
	}/* ����Ƿ�����ɣ����������رշ��ͣ�ʹ�ܽ��� */
	else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)//��������ж�
	{		
        if(am_rngbuf_isempty(g_Uart.write_buffer) == TRUE)
        {
            USART_ITConfig(USART2, USART_IT_TC, DISABLE);//��ֹ��������ж�
			g_Uart.SendOver();       
        }
	}
}
