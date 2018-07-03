/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : RTC底层驱动
*	修改记录 :
*		版本号   日期        作者     说明
*		V1.0   
*
*
*********************************************************************************************************
*/
#include "bsp.h"

//#define RTC_Debug   /* 用于选择调试模式，正式使用可以将其注释掉 */

/* 选择RTC的时钟源 */
#define RTC_CLOCK_SOURCE_LSE       /* LSE */
//#define RTC_CLOCK_SOURCE_LSI     /* LSI */ 

RTC_InitTypeDef RTC_InitStructure;

/*
*********************************************************************************************************
*	函 数 名: RTC_Set_Time()
*	功能说明: RTC时间设置 
*	形    参：hour,min,sec:小时,分钟,秒钟 ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
*	返 回 值: SUCEE(1),成功 ERROR(0),进入初始化模式失败 
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
*	函 数 名: RTC_Set_Date()()
*	功能说明: RTC日期设置 
*	形    参：year,month,date:年(0~99),月(1~12),日(0~31)  week:星期(1~7,0,非法!)
*	返 回 值: SUCEE(1),成功 ERROR(0),进入初始化模式失败 
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
*	函 数 名: RTC_Set_Alarm()
*	功能说明: 设置闹钟时间(按星期闹铃,24小时制)
*	形    参：week:星期几(1~7) @ref  RTC_Alarm_Definitions  hour,min,sec:小时,分钟,秒钟
*	返 回 值: SUCEE(1),成功 ERROR(0),进入初始化模式失败 
*********************************************************************************************************
*/
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//关闭闹钟A 
	
	RTC_TimeTypeInitStructure.RTC_Hours = hour;//小时
	RTC_TimeTypeInitStructure.RTC_Minutes = min;//分钟
	RTC_TimeTypeInitStructure.RTC_Seconds = sec;//秒
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None;	//精确匹配星期，时分秒	
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;	//按星期闹铃
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = week;
	RTC_AlarmStructure.RTC_AlarmTime = RTC_TimeTypeInitStructure;
	
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A,&RTC_AlarmStructure);
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);		//清除RTC闹钟A的标志
	EXTI_ClearITPendingBit(EXTI_Line17);	//清除LINE17上的中断标志位 
	
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);		//开启闹钟A 

	//初始化线上中断Line0	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;		//LINE17
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿触发 
	EXTI_Init(&EXTI_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*
*********************************************************************************************************
*	函 数 名: RTC_Config
*	功能说明: 1. 选择不同的RTC时钟源LSI或者LSE。
*             2. 配置RTC时钟。
*	形    参：无
*	返 回 值: 错误类型
*********************************************************************************************************
*/
u8 RTC_Config(void)
{
	u32 count = 0;	
	
	/* 使能PWR时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* 允许访问RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* 选择LSI作为时钟源 */
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

	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* 选择LSE作为RTC时钟 */
#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* 使能LSE振荡器  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* 等待就绪 */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
		count++;
		if(count>5000000)
			return 1;
	}

	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

#else
#error Please select the RTC Clock source inside the main.c file
#endif 	

	/* 使能RTC时钟 */
	RCC_RTCCLKCmd(ENABLE);
	/* 等待RTC APB寄存器同步 */
	RTC_WaitForSynchro();
	/* 配置RTC数据寄存器和分频器  */
	/* ck_spre(1Hz) = RTCCLK(LSE) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
	RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC同步分频系数(0~7FFF)
	RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
	RTC_Init(&RTC_InitStructure);
	
	RTC_Set_Time(19,56,56,RTC_H12_AM);	//设置时间
	RTC_Set_Date(15,12,9,3);		//设置日期
	
	RTC_WriteBackupRegister(RTC_BKP_DR0,0x5AA5);	//标记已经初始化过了
	return 0;
}

/* 
*********************************************************************************************************
*  函 数 名: bsp_InitRTC 
*  功能说明: 初始化RTC 
*  形 参：无  *  返 回 值: 无   
*********************************************************************************************************
*/ 
void bsp_InitRTC(void) 
{
	/* 用于检测是否已经配置过RTC，如果配置过的话，会在配置结束时 
	设置RTC备份寄存器为0x5AA5。如果检测RTC备份寄存器不0x5AA5   那么表示没有配置过，需要配置RTC.   */ 
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5) 
	{ 
		/* RTC 配置 */ 
		RTC_Config(); 
		/* 打印调试信息 */ 
		#ifdef RTC_Debug 
			printf("第一次使用RTC \n\r"); 
		#endif  
	}
	else
	{
		/* 检测上电复位标志是否设置 */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* 发生上电复位 */ 
			#ifdef RTC_Debug 
				printf("发生上电复位 \n\r"); 
			#endif 
		} 
		/* 检测引脚复位标志是否设置 */ 
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) 
		{ 
			/* 发生引脚复位 */ 
			#ifdef RTC_Debug 
				printf("发生引脚复位 \n\r");     
			#endif    
		} 
		/* 使能PWR时钟 */ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
		/* 允许访问RTC */ 
		PWR_BackupAccessCmd(ENABLE); 
		/* 等待 RTC APB 寄存器同步 */ 
		RTC_WaitForSynchro(); 
		/* 清除RTC闹钟标志 */ 
	//	RTC_ClearFlag(RTC_FLAG_ALRAF); 
		/* 清除RTC闹钟中断挂起标志 */ 
	//	EXTI_ClearITPendingBit(EXTI_Line17); 
	}
}

//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A中断?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//清除中断标志
		printf("ALARM A!\r\n");
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//清除中断线17的中断标志 											 
}

