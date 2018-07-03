/*
*********************************************************************************************************
*
*	模块名称 : TIM基本定时功能模块
*	文件名称 : bsp_tim_fun.c
*	版    本 : V1.1
*	说    明 : 利用STM32F4内部TIM输出PWM信号， 并实现基本的定时中断，等定时器功能.
*               TIM2用于硬件定时，定时时间到后执行回调函数。
*               TIM9用于PWM输出，调节LCD背光。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.1	2014-06-15 armfly  完善 bsp_SetTIMOutPWM，当占空比=0和100%时，关闭定时器，GPIO配置为输出
*		V1.3	2015-07-23 armfly  初始化定时器，必须设置 TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;		
*								   TIM1 和 TIM8 必须设置。否则蜂鸣器的控制不正常。
*
*********************************************************************************************************
*/
#include "bsp.h"
/* 保存 TIM定时中断到后执行的回调函数指针 */
static void (*s_TIM_CallBack1)(void);
static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);
static void (*s_TIM_CallBack4)(void);
static void (*s_ModbusTIM_CallBack)(void);

/*-----------------------------------------------------------------------
	system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

	HCLK = SYSCLK / 1     (AHB1Periph)
	PCLK2 = HCLK / 2      (APB2Periph)
	PCLK1 = HCLK / 4      (APB1Periph)

	因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
	因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

	APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
	APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11

----------------------------------------------------------------------- */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitHardTimer
*	功能说明: 配置 TIM2，用于us级别硬件定时。TIM2将自由运行，永不停止.
*			挂在 APB1 上，输入时钟=SystemCoreClock / 2
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	/* 中断结构体在 misc.h 中定义 */
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;

  	/* 使能TIM时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	uiTIMxCLK = SystemCoreClock / 2;
	usPrescaler = uiTIMxCLK / 1000000 ;		/* 分频到周期 1us */
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIMx enable counter */
	TIM_Cmd(TIM2, ENABLE);

	/* 配置TIM定时中断 (Update) */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitHardTimer9
*	功能说明: 配置 TIM9，用于us级别硬件定时。TIM9作为modbus rtu 3.5字符时间
*			挂在 APB2 上，输入时钟=SystemCoreClock / 2
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitModbusTimer(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);    //T9时钟使能
	
	uiTIMxCLK = SystemCoreClock;
	usPrescaler = uiTIMxCLK / 1000000 ;						// 分频到周期 1us 
	
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;			//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = usPrescaler; 	//定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;//向下计数
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);      //T9初始化    

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;	//相应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM9, TIM_IT_Update, DISABLE);   			//关T9更新中断
    TIM_Cmd(TIM9, DISABLE);         						//禁止使能
}
/*
*********************************************************************************************************
*	函 数 名: bsp_StartModbusTimer
*	功能说明: 每接收到一个字符后调用此函数，帧结束符
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartModbusTimer(uint16_t TimeOut, void * _pCallBack)
{
	s_ModbusTIM_CallBack = (void (*)(void))_pCallBack;
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update);     		//清除中断标志位
    TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);   			//使能T9更新中断
    TIM_SetCounter(TIM9,TimeOut);    						//设定初始值
    TIM_Cmd(TIM9, ENABLE);      							//启动T9	
}
	

/*
*********************************************************************************************************
*	函 数 名: bsp_StartHardTimer
*	功能说明: 使用TIM2做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个定时器，互不干扰。
*             定时精度正负10us （主要耗费在调用本函数的执行时间，函数内部进行了补偿减小误差）
*			 TIM2是32位定时器。定时范围很大
*	形    参: _CC : 捕获通道几，1，2，3, 4
*             _uiTimeOut : 超时时间, 单位 1us.   最大 4294秒
*             _pCallBack : 定时时间到后，被执行的函数
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
	uint32_t cnt_now;
    uint32_t cnt_tar;

    /*
        执行下面这个语句，时长 = 18us (通过逻辑分析仪测量IO翻转)
        bsp_StartTimer2(3, 500, (void *)test1);
    */
    if (_uiTimeOut < 5)
    {
        ;
    }


    else
    {
        _uiTimeOut -= 5;
    }

    cnt_now = TIM_GetCounter(TIM2);    	/* 读取当前的计数器值 */
    cnt_tar = cnt_now + _uiTimeOut;			/* 计算捕获的计数器值 */
    if (_CC == 1)
    {
        s_TIM_CallBack1 = (void (*)(void))_pCallBack;

        TIM_SetCompare1(TIM2, cnt_tar);      	/* 设置捕获比较计数器CC1 */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);	/* 使能CC1中断 */

    }
    else if (_CC == 2)
    {
		s_TIM_CallBack2 = (void (*)(void))_pCallBack;

        TIM_SetCompare2(TIM2, cnt_tar);      	/* 设置捕获比较计数器CC2 */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);	/* 使能CC2中断 */
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;

        TIM_SetCompare3(TIM2, cnt_tar);      	/* 设置捕获比较计数器CC3 */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);	/* 使能CC3中断 */
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack4 = (void (*)(void))_pCallBack;

        TIM_SetCompare4(TIM2, cnt_tar);      	/* 设置捕获比较计数器CC4 */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);	/* 使能CC4中断 */
    }
	else
    {
        return;
    }
}

/*
*********************************************************************************************************
* 函数名  :  bsp_InitBacklightSetTimer
*	功能说明: TIM12_PWM_Init设置引脚输出的PWM信号的频率和占空比.  当频率为0，并且占空为0时，关闭定时器，GPIO输出0；
*			  当频率为0，占空比为100%时，GPIO输出1.PWM信号频率，单位Hz  (实际测试，最大输出频率为 168M / 4 = 42M）. 0 表示禁止输出
*	形    参: 			  
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitBacklightSetTimer(void)
{
/*
	TIM12 配置: 产生1路PWM信号;
	TIM12CLK = 84 MHz, Prescaler = 0(不分频), TIM12 counter clock = 84 MHz
	计算公式：
	PWM输出频率 = TIM12 counter clock /(ARR + 1)
	
	如果不对TIM12CLK预分频，那么不可能得到100Hz低频。要根据期望的输出频率来改变预分频系数。
	期望的频率越大，预分频系数越小。
	我们期望设置为100Hz，Prescaler = 84-1，ARR = 10000-1
	PWM输出频率 = TIM12 counter clock /(ARR + 1)(Prescaler+1)=100Hz
	PWM信号占空比，单位：万分之一。如5000，表示50.00%的占空比,初始值为7500
 */	
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);	//TIM12时钟使能    
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);//GPIOB15复用为定时器12
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//PB15 推挽输出,控制背光
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化 //PB15 推挽输出,控制背光
	
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1;				//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;			//定时器分频	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
	
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseInitStructure);	//初始化定时器12
	
	//初始化TIM12 Channel2 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//PWM模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity= TIM_OCPolarity_Low;	//输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse = 7500;                   //占空比初始值7500/10000 = 75%
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);				//TIM12OC2
	
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);		//使能TIM12在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM12, ENABLE);					//ARP使能 
	TIM_Cmd(TIM12, ENABLE);									//使能TIM12	
}

/*
*********************************************************************************************************
*	函 数 名: TIMx_IRQHandler
*	功能说明: TIM 中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_IRQ(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);	/* 禁能CC1中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);	/* 禁能CC2中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);	/* 禁能CC3中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);	/* 禁能CC4中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack4();
    }
}

/*
*********************************************************************************************************
* 函数名  :  TIM9_IRQ
* 描述    ： 帧超时计数器 T9中断服务程序 表示一帧数据接收完成
* 输入参数： 无
* 返回参数： 无
*********************************************************************************************************
*/
void TIM9_IRQ(void)
{
    //已经有t35这么长的时间里，没有收到任新字节，当前的帧结束,接收到完整的一帧Modbus数据，处理发送
    if (TIM_GetITStatus(TIM9, TIM_IT_Update) == 1)		//溢出中断
    {       
        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);     //清除标志位
        TIM_ITConfig(TIM9, TIM_IT_Update, DISABLE);   	//关T6更新中断               
		TIM_Cmd(TIM9, DISABLE);      					//关闭定时器    
		/* 先关闭中断，在执行回调函数，因为回掉函数可能需要重启定时器 */
		s_ModbusTIM_CallBack();
    }
}

