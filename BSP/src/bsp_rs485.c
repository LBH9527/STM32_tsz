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

#define UART_TXBUF_SIZE    128		/* ���ͻ�������С */
#define UART_RXBUF_SIZE    128     	/* ���ջ�������С */
uint8_t UartTxBuf[UART_TXBUF_SIZE];     	/* ���ͻ����� */
uint8_t UartRxBuf[UART_RXBUF_SIZE];     	/* ���ջ����� */
//const u32 BaudTab[] = {4800,9600,19200,38400,57600,115200};
#define UART2_BAUD			9600 	/* Ĭ��Ϊ9600 */
UART_S g_sUart;

static void Uart2_InitVar(void);
static void Uart2_InitHard(void);
void RS485_InitTXE(void);
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
void RS485_InitTXE(void)
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
	g_sUart.pTxBuf = UartTxBuf;					/* ���ͻ�����ָ�� */
	g_sUart.pRxBuf = UartRxBuf;					/* ���ջ�����ָ�� */
	g_sUart.TxBufSize = UART_TXBUF_SIZE;			/* ���ͻ�������С */
	g_sUart.RxBufSize = UART_RXBUF_SIZE;			/* ���ջ�������С */
	g_sUart.TxWrite = 0;							/* ����FIFOд���� */
	g_sUart.TxRead = 0;							/* ����FIFO������ */
	g_sUart.RxWrite = 0;							/* ����FIFOд���� */
	g_sUart.RxRead = 0;							/* ����FIFO������ */
//	g_sUart.RxCount = 0;							/* ���յ��������ݸ��� */
//	g_sUart.TxCount = 0;							/* �����͵����ݸ��� */
	g_sUart.SendBefor = RS485_SendBefor;			/* ��������ǰ�Ļص����� */
	g_sUart.SendOver = RS485_SendOver;			/* ������Ϻ�Ļص����� */
	g_sUart.ReciveNew = RS485_ReciveNew;			/* ���յ������ݺ�Ļص����� */
}
/*
*********************************************************************************************************
*	�� �� ��: comClearTxFifo
*	����˵��: ���㴮�ڷ��ͻ�����
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UartClearTxFifo(void)
{
	g_sUart.TxWrite = 0;
	g_sUart.TxRead = 0;
//	g_sUart.TxCount = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: comClearRxFifo
*	����˵��: ���㴮�ڽ��ջ�����
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UartClearRxFifo(void)
{
	g_sUart.RxWrite = 0;
	g_sUart.RxRead = 0;
//	g_sUart.RxCount = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Set485Baud
*	����˵��: �޸�UART2������
*	��    ��: �����ʶ�Ӧ�±�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Set485Baud(uint16_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_BaudRate = baud;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART2, &USART_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: UartSend
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��:   len �����ַ������ȣ� *s ָ���ַ���ͷ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_UartSend(uint8_t *str, uint8_t len)
{
    uint8_t i;
	#if 0
	/* �� _pUart->usTxBufSize == 1ʱ������ĺ����������������� */
	while (1)
	{
		__IO uint16_t usCount;

		DISABLE_INT();
		usCount = g_sUart.TxCount;
		ENABLE_INT();

		if (usCount < g_sUart.TxBufSize)
		{
			break;
		}
	}
	#endif	
	
	
	for (i = 0; i < len; i++)
	{
	    /* �����������뷢�ͻ����� */ 
	    g_sUart.pTxBuf[g_sUart.TxWrite++] = str[i];
	    if (g_sUart.TxWrite >= UART_TXBUF_SIZE)
	    {
            g_sUart.TxWrite = 0;
	    }
//		g_sUart.TxCount++;
	}
	/* ʹ�ܷ���״̬�Ĵ���Ϊ���жϣ����������ж�*/
//	RS485_SendBefor();	
	g_sUart.SendBefor();	
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);	//ʹ�ܷ������ݼĴ���Ϊ���ж�
}
/*
*********************************************************************************************************
*	�� �� ��: UartGetChar
*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
*	��    ��: pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
*********************************************************************************************************
*/
uint8_t UartGetChar(uint8_t * pByte)
{
	if(g_sUart.RxRead == g_sUart.RxWrite)    
    {        
        return 0;    
    }    
    else    
    {       
        *pByte = g_sUart.pRxBuf[g_sUart.RxRead++];
        if(g_sUart.RxRead >= UART_RXBUF_SIZE)        
        {            
            g_sUart.RxRead = 0;                  
        }       
        return 1;    
    }
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
	uint8_t ch;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//�����ж�
    {
		ch = USART_ReceiveData(USART2); 
        g_sUart.pRxBuf[g_sUart.RxWrite++] = ch;//������յ�������
        if (g_sUart.RxWrite >= g_sUart.RxBufSize)
        {
            g_sUart.RxWrite = 0;		
        }    		
		g_sUart.ReciveNew(ch);
    }
		/* �����ͻ��������ж� */
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
		if (g_sUart.TxRead == g_sUart.TxWrite)
//		if (g_sUart.TxCount == 0)	
        {
           /* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			/* ʹ�����ݷ�������ж� */
			USART_ITConfig(USART2, USART_IT_TC, ENABLE);
        }
        else
        {
           /* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
			USART_SendData(USART2, UartTxBuf[g_sUart.TxRead]);
            if (++g_sUart.TxRead >= UART_TXBUF_SIZE)
                g_sUart.TxRead = 0;
//			g_sUart.TxCount --;
        }
	}/* ����Ƿ�����ɣ����������رշ��ͣ�ʹ�ܽ��� */
	else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)//��������ж�
	{
		if (g_sUart.TxRead == g_sUart.TxWrite)
        {
            USART_ITConfig(USART2, USART_IT_TC, DISABLE);//��ֹ��������ж�
			g_sUart.SendOver();           
        }	
        else
        {
            /* ��������£��������˷�֧ */
			USART_SendData(USART2, UartTxBuf[g_sUart.TxRead++]);
            if (g_sUart.TxRead >= UART_TXBUF_SIZE)
                g_sUart.TxRead = 0;
        }		
	}
}

