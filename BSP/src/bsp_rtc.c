/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : bsp_rtc.c
*	��    �� : V1.0
*	˵    �� : RTC�ײ�����
*	�޸ļ�¼ :
*		�汾��   ����        ����     ˵��
*		V1.0   
*
*
*********************************************************************************************************
*/
#include "bsp.h"

//#define RTC_Debug   /* ����ѡ�����ģʽ����ʽʹ�ÿ��Խ���ע�͵� */

/* ѡ��RTC��ʱ��Դ */
#define RTC_CLOCK_SOURCE_LSE       /* LSE */
//#define RTC_CLOCK_SOURCE_LSI     /* LSI */ 

RTC_InitTypeDef RTC_InitStructure;

/*
*********************************************************************************************************
*	�� �� ��: RTC_Set_Time()
*	����˵��: RTCʱ������ 
*	��    �Σ�hour,min,sec:Сʱ,����,���� ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
*	�� �� ֵ: SUCEE(1),�ɹ� ERROR(0),�����ʼ��ģʽʧ�� 
*********************************************************************************************************
*/
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours = hour;
	RTC_TimeTypeInitStructure.RTC_Minutes = min;
	RTC_TimeTypeInitStructure.RTC_Seconds = sec;
	RTC_TimeTypeInitStructure.RTC_H12 = ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);	
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_Set_Date()()
*	����˵��: RTC�������� 
*	��    �Σ�year,month,date:��(0~99),��(1~12),��(0~31)  week:����(1~7,0,�Ƿ�!)
*	�� �� ֵ: SUCEE(1),�ɹ� ERROR(0),�����ʼ��ģʽʧ�� 
*********************************************************************************************************
*/
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	
	RTC_DateTypeInitStructure.RTC_Date = date;
	RTC_DateTypeInitStructure.RTC_Month = month;
	RTC_DateTypeInitStructure.RTC_WeekDay = week;
	RTC_DateTypeInitStructure.RTC_Year = year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}
/*
*********************************************************************************************************
*	�� �� ��: RTC_Set_Alarm()
*	����˵��: ��������ʱ��(����������,24Сʱ��)
*	��    �Σ�week:���ڼ�(1~7) @ref  RTC_Alarm_Definitions  hour,min,sec:Сʱ,����,����
*	�� �� ֵ: SUCEE(1),�ɹ� ERROR(0),�����ʼ��ģʽʧ�� 
*********************************************************************************************************
*/
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//�ر�����A 
	
	RTC_TimeTypeInitStructure.RTC_Hours = hour;//Сʱ
	RTC_TimeTypeInitStructure.RTC_Minutes = min;//����
	RTC_TimeTypeInitStructure.RTC_Seconds = sec;//��
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None;	//��ȷƥ�����ڣ�ʱ����	
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;	//����������
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = week;
	RTC_AlarmStructure.RTC_AlarmTime = RTC_TimeTypeInitStructure;
	
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A,&RTC_AlarmStructure);
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);		//���RTC����A�ı�־
	EXTI_ClearITPendingBit(EXTI_Line17);	//���LINE17�ϵ��жϱ�־λ 
	
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);		//��������A 

	//��ʼ�������ж�Line0	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;		//LINE17
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//�����ش��� 
	EXTI_Init(&EXTI_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*
*********************************************************************************************************
*	�� �� ��: RTC_Config
*	����˵��: 1. ѡ��ͬ��RTCʱ��ԴLSI����LSE��
*             2. ����RTCʱ�ӡ�
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
u8 RTC_Config(void)
{
	u32 count = 0;	
	
	/* ʹ��PWRʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* �������RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* ѡ��LSI��Ϊʱ��Դ */
#if defined (RTC_CLOCK_SOURCE_LSI)  
	
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
		count++;
		if(count>5000000)
			return 1;
	}

	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ѡ��LSE��ΪRTCʱ�� */
#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* ʹ��LSE����  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* �ȴ����� */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
		count++;
		if(count>5000000)
			return 1;
	}

	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

#else
#error Please select the RTC Clock source inside the main.c file
#endif 	

	/* ʹ��RTCʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	/* �ȴ�RTC APB�Ĵ���ͬ�� */
	RTC_WaitForSynchro();
	/* ����RTC���ݼĴ����ͷ�Ƶ��  */
	/* ck_spre(1Hz) = RTCCLK(LSE) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
	RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTCͬ����Ƶϵ��(0~7FFF)
	RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
	
	RTC_Set_Time(19,56,56,RTC_H12_AM);	//����ʱ��
	RTC_Set_Date(15,12,9,3);		//��������
	
	RTC_WriteBackupRegister(RTC_BKP_DR0,0x5AA5);	//����Ѿ���ʼ������
	return 0;
}

/* 
*********************************************************************************************************
*  �� �� ��: bsp_InitRTC 
*  ����˵��: ��ʼ��RTC 
*  �� �Σ���  *  �� �� ֵ: ��   
*********************************************************************************************************
*/ 
void bsp_InitRTC(void) 
{
	/* ���ڼ���Ƿ��Ѿ����ù�RTC��������ù��Ļ����������ý���ʱ 
	����RTC���ݼĴ���Ϊ0x5AA5��������RTC���ݼĴ�����0x5AA5   ��ô��ʾû�����ù�����Ҫ����RTC.   */ 
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5) 
	{ 
		/* RTC ���� */ 
		RTC_Config(); 
		/* ��ӡ������Ϣ */ 
		#ifdef RTC_Debug 
			printf("��һ��ʹ��RTC \n\r"); 
		#endif  
	}
	else
	{
		/* ����ϵ縴λ��־�Ƿ����� */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* �����ϵ縴λ */ 
			#ifdef RTC_Debug 
				printf("�����ϵ縴λ \n\r"); 
			#endif 
		} 
		/* ������Ÿ�λ��־�Ƿ����� */ 
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) 
		{ 
			/* �������Ÿ�λ */ 
			#ifdef RTC_Debug 
				printf("�������Ÿ�λ \n\r");     
			#endif    
		} 
		/* ʹ��PWRʱ�� */ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
		/* �������RTC */ 
		PWR_BackupAccessCmd(ENABLE); 
		/* �ȴ� RTC APB �Ĵ���ͬ�� */ 
		RTC_WaitForSynchro(); 
		/* ���RTC���ӱ�־ */ 
	//	RTC_ClearFlag(RTC_FLAG_ALRAF); 
		/* ���RTC�����жϹ����־ */ 
	//	EXTI_ClearITPendingBit(EXTI_Line17); 
	}
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A�ж�?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//����жϱ�־
		printf("ALARM A!\r\n");
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//����ж���17���жϱ�־ 											 
}

