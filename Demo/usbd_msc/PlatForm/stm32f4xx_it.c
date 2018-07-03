/*
*********************************************************************************************************
*
*	ģ������ : �ж�ģ��
*	�ļ����� : stm32f4xxx_it.c
*	��    �� : V1.1
*	˵    �� : ���ļ�����жϷ�������
*
*			����ֻ��Ҫ������Ҫ���жϺ������ɡ�һ���жϺ������ǹ̶��ģ��������޸��������ļ��еĺ�����
*				\Libraries\CMSIS\Device\ST\STM32F4xx\Source\Templates\arm\startup_stm32f4xx.s
*
*			�����ļ��ǻ�������ļ�������ÿ���жϵķ���������Щ����ʹ����WEAK �ؼ��֣���ʾ�����壬�����
*			��������c�ļ����ض����˸÷��������������ͬ��������ô�����ļ����жϺ������Զ���Ч����Ҳ��
*			�����ض���ĸ��
*
*			Ϊ�˼�ǿģ�黯�����ǽ��齫�жϷ�������ɢ����Ӧ������ģ���ļ��С�����systick�жϷ������
*			���� bsp_timer.c �ļ��С�
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2013-02-01 armfly  �װ�
*		v1.0    2013-06-20 armfly  ��Ӳ���쳣�жϷ�����������Ӵ�ӡ������Ϣ�Ĺ��ܡ�
*
*
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "bsp.h"
#include "usbd_msc_core.h"
/*NVIC���ȼ�����ΪGroup4��0-16��ռʽ���ȼ���ԽС�����ȼ�Խ��
*	�ж���						PreemptionPriority		SubPriority		����
*	TIM1_BRK_TIM9_IRQHandler		9						0			MODBUS�ַ������ʱ
*   USART2_IRQHandler				4						0			RS485����ͨ��
*	USART1_IRQHandler				10						0			printf��ӡ
*/
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/
extern void UartIRQ(void);
void USART2_IRQHandler(void)
{
	UartIRQ();
}

extern void TIM9_IRQ(void);
void TIM1_BRK_TIM9_IRQHandler(void)
{
	TIM9_IRQ();
}
extern void TIM2_IRQ(void);
void TIM2_IRQHandler(void)
{
	TIM2_IRQ();
}

#define CURSOR_STEP    10

static uint8_t *USBD_HID_GetPos (void)
{
    int8_t  x = 0 , y = 0 ;
    uint8_t ucMsg;
    static uint8_t HID_Buffer [4];
    ucMsg = bsp_GetKeyMsg();
    if (ucMsg)
    {
        switch (ucMsg)
        {
            case KEY0_DOWN:
                 x -= CURSOR_STEP;
                break;

            case KEY1_DOWN:
                x += CURSOR_STEP;
                break;

            case KEY2_UP:
                y -= CURSOR_STEP;
                break;

            case KEY_WK_DOWN:
                y += CURSOR_STEP;
                break;

            default:
                break;
        }
    }  
    HID_Buffer[0] = 0;
    HID_Buffer[1] = x;
    HID_Buffer[2] = y;
    HID_Buffer[3] = 0;
  
    return HID_Buffer;
}

/*
*********************************************************************************************************
*	�� �� ��: SysTick_ISR
*	����˵��: SysTick�жϷ������ÿ��1ms����1��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern void SysTick_ISR(void);
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
void SysTick_Handler(void)
{
    static __IO uint32_t counter=0;
    uint8_t *buf;  
    
    SysTick_ISR();
  
  /* check Joystick state every polling interval (10ms) */
  if (counter++ == USBD_HID_GetPollingInterval(&USB_OTG_dev))
  {
    buf = USBD_HID_GetPos();
    if((buf[1] != 0) ||(buf[2] != 0))
    {
      /* send data though IN endpoint*/
      USBD_HID_SendReport (&USB_OTG_dev, 
                           buf,
                           4);
    }
    counter =0;
  }
}
/*
*********************************************************************************************************
*	�� �� ��: NMI_Handler
*	����˵��: ���������жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void NMI_Handler(void)
{
}

/*
*********************************************************************************************************
*	�� �� ��: HardFault_Handler
*	����˵��: Ӳ�������жϷ�����������쳣�������رգ����ַ������쳣���򴥷���
*			  ִ���쳣����ʱ���������쳣���򴥷�����λʱĬ��ʹ�ܡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	�� �� ��: MemManage_Handler
*	����˵��: �ڴ�����쳣�жϷ������Υ��MPU�趨�Ĵ洢�����ʹ���ʱ������ ��λʱĬ��δʹ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	�� �� ��: BusFault_Handler
*	����˵��: ���߷����쳣�жϷ������ȡָ����ݶ�д����ջ���������쳣�� ��λʱĬ��δʹ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	�� �� ��: UsageFault_Handler
*	����˵��: �÷������жϷ������ִ��δ����ָ��Ƕ������������ʱ������ ��λʱĬ��δʹ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/*
*********************************************************************************************************
*	�� �� ��: DebugMon_Handler
*	����˵��: ���Լ������жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DebugMon_Handler(void)
{
}

/*
*********************************************************************************************************
*	�� �� ��: PendSV_Handler
*	����˵��: �ɹ����ϵͳ��������жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void PendSV_Handler(void)
{
}



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/