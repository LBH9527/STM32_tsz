/*
*********************************************************************************************************
*
*	ģ������ : TIM������ʱ����ģ��
*	�ļ����� : bsp_tim_fun.c
*	��    �� : V1.1
*	˵    �� : ����STM32F4�ڲ�TIM���PWM�źţ� ��ʵ�ֻ����Ķ�ʱ�жϣ��ȶ�ʱ������.
*               TIM2����Ӳ����ʱ����ʱʱ�䵽��ִ�лص�������
*               TIM9����PWM���������LCD���⡣
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.1	2014-06-15 armfly  ���� bsp_SetTIMOutPWM����ռ�ձ�=0��100%ʱ���رն�ʱ����GPIO����Ϊ���
*		V1.3	2015-07-23 armfly  ��ʼ����ʱ������������ TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;		
*								   TIM1 �� TIM8 �������á�����������Ŀ��Ʋ�������
*
*********************************************************************************************************
*/
#include "bsp.h"
/* ���� TIM��ʱ�жϵ���ִ�еĻص�����ָ�� */
static void (*s_TIM_CallBack1)(void);
static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);
static void (*s_TIM_CallBack4)(void);
static void (*s_ModbusTIM_CallBack)(void);

/*-----------------------------------------------------------------------
	system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

	HCLK = SYSCLK / 1     (AHB1Periph)
	PCLK2 = HCLK / 2      (APB2Periph)
	PCLK1 = HCLK / 4      (APB1Periph)

	��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
	��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

	APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
	APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

----------------------------------------------------------------------- */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHardTimer
*	����˵��: ���� TIM2������us����Ӳ����ʱ��TIM2���������У�����ֹͣ.
*			���� APB1 �ϣ�����ʱ��=SystemCoreClock / 2
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	/* �жϽṹ���� misc.h �ж��� */
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;

  	/* ʹ��TIMʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	uiTIMxCLK = SystemCoreClock / 2;
	usPrescaler = uiTIMxCLK / 1000000 ;		/* ��Ƶ������ 1us */
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIMx enable counter */
	TIM_Cmd(TIM2, ENABLE);

	/* ����TIM��ʱ�ж� (Update) */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHardTimer9
*	����˵��: ���� TIM9������us����Ӳ����ʱ��TIM9��Ϊmodbus rtu 3.5�ַ�ʱ��
*			���� APB2 �ϣ�����ʱ��=SystemCoreClock / 2
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitModbusTimer(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);    //T9ʱ��ʹ��
	
	uiTIMxCLK = SystemCoreClock;
	usPrescaler = uiTIMxCLK / 1000000 ;						// ��Ƶ������ 1us 
	
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;			//�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = usPrescaler; 	//��ʱ����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;//���¼���
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);      //T9��ʼ��    

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;	//��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM9, TIM_IT_Update, DISABLE);   			//��T9�����ж�
    TIM_Cmd(TIM9, DISABLE);         						//��ֹʹ��
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_StartModbusTimer
*	����˵��: ÿ���յ�һ���ַ�����ô˺�����֡������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartModbusTimer(uint16_t TimeOut, void * _pCallBack)
{
	s_ModbusTIM_CallBack = (void (*)(void))_pCallBack;
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update);     		//����жϱ�־λ
    TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);   			//ʹ��T9�����ж�
    TIM_SetCounter(TIM9,TimeOut);    						//�趨��ʼֵ
    TIM_Cmd(TIM9, ENABLE);      							//����T9	
}
	

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartHardTimer
*	����˵��: ʹ��TIM2�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����4����ʱ�����������š�
*             ��ʱ��������10us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣬�����ڲ������˲�����С��
*			 TIM2��32λ��ʱ������ʱ��Χ�ܴ�
*	��    ��: _CC : ����ͨ������1��2��3, 4
*             _uiTimeOut : ��ʱʱ��, ��λ 1us.   ��� 4294��
*             _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
	uint32_t cnt_now;
    uint32_t cnt_tar;

    /*
        ִ�����������䣬ʱ�� = 18us (ͨ���߼������ǲ���IO��ת)
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

    cnt_now = TIM_GetCounter(TIM2);    	/* ��ȡ��ǰ�ļ�����ֵ */
    cnt_tar = cnt_now + _uiTimeOut;			/* ���㲶��ļ�����ֵ */
    if (_CC == 1)
    {
        s_TIM_CallBack1 = (void (*)(void))_pCallBack;

        TIM_SetCompare1(TIM2, cnt_tar);      	/* ���ò���Ƚϼ�����CC1 */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);	/* ʹ��CC1�ж� */

    }
    else if (_CC == 2)
    {
		s_TIM_CallBack2 = (void (*)(void))_pCallBack;

        TIM_SetCompare2(TIM2, cnt_tar);      	/* ���ò���Ƚϼ�����CC2 */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);	/* ʹ��CC2�ж� */
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;

        TIM_SetCompare3(TIM2, cnt_tar);      	/* ���ò���Ƚϼ�����CC3 */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);	/* ʹ��CC3�ж� */
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack4 = (void (*)(void))_pCallBack;

        TIM_SetCompare4(TIM2, cnt_tar);      	/* ���ò���Ƚϼ�����CC4 */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);	/* ʹ��CC4�ж� */
    }
	else
    {
        return;
    }
}

/*
*********************************************************************************************************
* ������  :  bsp_InitBacklightSetTimer
*	����˵��: TIM12_PWM_Init�������������PWM�źŵ�Ƶ�ʺ�ռ�ձ�.  ��Ƶ��Ϊ0������ռ��Ϊ0ʱ���رն�ʱ����GPIO���0��
*			  ��Ƶ��Ϊ0��ռ�ձ�Ϊ100%ʱ��GPIO���1.PWM�ź�Ƶ�ʣ���λHz  (ʵ�ʲ��ԣ�������Ƶ��Ϊ 168M / 4 = 42M��. 0 ��ʾ��ֹ���
*	��    ��: 			  
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitBacklightSetTimer(void)
{
/*
	TIM12 ����: ����1·PWM�ź�;
	TIM12CLK = 84 MHz, Prescaler = 0(����Ƶ), TIM12 counter clock = 84 MHz
	���㹫ʽ��
	PWM���Ƶ�� = TIM12 counter clock /(ARR + 1)
	
	�������TIM12CLKԤ��Ƶ����ô�����ܵõ�100Hz��Ƶ��Ҫ�������������Ƶ�����ı�Ԥ��Ƶϵ����
	������Ƶ��Խ��Ԥ��Ƶϵ��ԽС��
	������������Ϊ100Hz��Prescaler = 84-1��ARR = 10000-1
	PWM���Ƶ�� = TIM12 counter clock /(ARR + 1)(Prescaler+1)=100Hz
	PWM�ź�ռ�ձȣ���λ�����֮һ����5000����ʾ50.00%��ռ�ձ�,��ʼֵΪ7500
 */	
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);	//TIM12ʱ��ʹ��    
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);//GPIOB15����Ϊ��ʱ��12
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//PB15 �������,���Ʊ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ�� //PB15 �������,���Ʊ���
	
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1;				//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;			//��ʱ����Ƶ	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseInitStructure);	//��ʼ����ʱ��12
	
	//��ʼ��TIM12 Channel2 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//PWMģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity= TIM_OCPolarity_Low;	//�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_Pulse = 7500;                   //ռ�ձȳ�ʼֵ7500/10000 = 75%
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);				//TIM12OC2
	
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);		//ʹ��TIM12��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM12, ENABLE);					//ARPʹ�� 
	TIM_Cmd(TIM12, ENABLE);									//ʹ��TIM12	
}

/*
*********************************************************************************************************
*	�� �� ��: TIMx_IRQHandler
*	����˵��: TIM �жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM2_IRQ(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);	/* ����CC1�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);	/* ����CC2�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);	/* ����CC3�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);	/* ����CC4�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack4();
    }
}

/*
*********************************************************************************************************
* ������  :  TIM9_IRQ
* ����    �� ֡��ʱ������ T9�жϷ������ ��ʾһ֡���ݽ������
* ��������� ��
* ���ز����� ��
*********************************************************************************************************
*/
void TIM9_IRQ(void)
{
    //�Ѿ���t35��ô����ʱ���û���յ������ֽڣ���ǰ��֡����,���յ�������һ֡Modbus���ݣ�������
    if (TIM_GetITStatus(TIM9, TIM_IT_Update) == 1)		//����ж�
    {       
        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);     //�����־λ
        TIM_ITConfig(TIM9, TIM_IT_Update, DISABLE);   	//��T6�����ж�               
		TIM_Cmd(TIM9, DISABLE);      					//�رն�ʱ��    
		/* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
		s_ModbusTIM_CallBack();
    }
}

