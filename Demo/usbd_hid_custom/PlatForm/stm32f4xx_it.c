/*
*********************************************************************************************************
*
*	模块名称 : 中断模块
*	文件名称 : stm32f4xxx_it.c
*	版    本 : V1.1
*	说    明 : 本文件存放中断服务函数。
*
*			我们只需要添加需要的中断函数即可。一般中断函数名是固定的，除非您修改了启动文件中的函数名
*				\Libraries\CMSIS\Device\ST\STM32F4xx\Source\Templates\arm\startup_stm32f4xx.s
*
*			启动文件是汇编语言文件，定了每个中断的服务函数，这些函数使用了WEAK 关键字，表示弱定义，因此如
*			果我们在c文件中重定义了该服务函数（必须和它同名），那么启动文件的中断函数将自动无效。这也就
*			函数重定义的概念。
*
*			为了加强模块化，我们建议将中断服务程序分散到对应的驱动模块文件中。比如systick中断服务程序
*			放在 bsp_timer.c 文件中。
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2013-02-01 armfly  首版
*		v1.0    2013-06-20 armfly  在硬件异常中断服务程序中增加打印错误信息的功能。
*
*
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "bsp.h"
#include  "usbd_customhid_core.h"
/*NVIC优先级分组为Group4：0-16抢占式优先级。越小，优先级越高
*	中断名						PreemptionPriority		SubPriority		作用
*	TIM1_BRK_TIM9_IRQHandler		9						0			MODBUS字符间隔计时
*   USART2_IRQHandler				4						0			RS485串口通信
*	USART1_IRQHandler				10						0			printf打印
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
    static uint8_t HID_Buffer [2];
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
    HID_Buffer[0] = x;
    HID_Buffer[1] = y;
//    HID_Buffer[2] = y;
//    HID_Buffer[3] = 0;
  
    return HID_Buffer;
}

/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔1ms进入1次
*	形    参:  无
*	返 回 值: 无
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
  if (counter++ == 10)
  {
    buf = USBD_HID_GetPos();
    if((buf[0] != 0) ||(buf[1] != 0))
    {
      /* send data though IN endpoint*/
      USBD_CUSTOM_HID_SendReport (&USB_OTG_dev, 
                           buf,
                           2);
    }
    counter =0;
  }
}
/*
*********************************************************************************************************
*	函 数 名: NMI_Handler
*	功能说明: 不可屏蔽中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void NMI_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: HardFault_Handler
*	功能说明: 硬件故障中断服务程序。其他异常处理被关闭，而又发生了异常，则触发。
*			  执行异常处理时，发生了异常，则触发。复位时默认使能。
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: MemManage_Handler
*	功能说明: 内存管理异常中断服务程序。违反MPU设定的存储器访问规则时触发。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: BusFault_Handler
*	功能说明: 总线访问异常中断服务程序。取指令、数据读写、堆栈操作出现异常。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: UsageFault_Handler
*	功能说明: 用法错误中断服务程序。执行未定义指令、非对齐操作、除零时触发。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: DebugMon_Handler
*	功能说明: 调试监视器中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DebugMon_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: PendSV_Handler
*	功能说明: 可挂起的系统服务调用中断服务程序。
*	形    参: 无
*	返 回 值: 无
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
