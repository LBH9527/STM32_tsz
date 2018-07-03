/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器驱动模块
*	文件名称 : bsp_beep.c
*	版    本 : V1.0
*	说    明 : 驱动蜂鸣器.
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    			9527	有源蜂鸣器
*
*	
*
*********************************************************************************************************
*/

#include "bsp.h"

#define BEEP_ENABLE()   GPIO_SetBits(GPIOF, GPIO_Pin_8)       	/* 使能蜂鸣器鸣叫 */
#define BEEP_DISABLE()  GPIO_ResetBits(GPIOF, GPIO_Pin_8)		/* 禁止蜂鸣器鸣叫 */
static BEEP_T s_sBeep;		/* 定义蜂鸣器全局结构体变量 */


/*******************************************************************************
*函数名：InitKeyHard()
* 描述 ：蜂鸣器硬件初始化
* 输入 ：
* 输出 ：
*******************************************************************************/
static void InitBeepHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	//使能GPIOF时钟
	//蜂鸣器对应GPIOF8 初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_100MHz;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);						//蜂鸣器对应IO口清零
}

/*******************************************************************************
*	函 数 名: bsp_InitBeep
*	功能说明: 初始化蜂鸣器. 该函数被 bsp_Init() 调用。
*	形    参: 无
*	返 回 值: 无
*******************************************************************************/
void bsp_InitBeep(void)
{
	InitBeepHard();
}	

/********************************************************************************
*	函 数 名: BEEP_Start
*	功能说明: 启动蜂鸣音。
*	形    参：BeepTime : 蜂鸣时间，单位10ms; 0 表示不鸣叫
*			  StopTime : 停止时间，单位10ms; 0 表示持续鸣叫
*			 Cycle : 鸣叫次数， 0 表示持续鸣叫
*	返 回 值: 无
*******************************************************************************/
void bsp_BeepStart(uint8_t BeepTime, uint16_t StopTime, uint16_t Cycle)
{
	if (BeepTime == 0)
	{
		return;
	}

	s_sBeep.BeepTime = BeepTime;
	s_sBeep.StopTime = StopTime;
	s_sBeep.Cycle = Cycle;
	s_sBeep.Count = 0;
	s_sBeep.CycleCount = 0;
	s_sBeep.State = 0;
	s_sBeep.Enalbe = 1;	/* 设置完全局参数后再使能发声标志 */

	BEEP_ENABLE();			/* 开始发声 */
}

/*******************************************************************************
*	函 数 名: BEEP_Stop
*	功能说明: 停止蜂鸣音。
*	形    参：无
*	返 回 值: 无
*******************************************************************************/
void bsp_BeepStop(void)
{
	s_sBeep.Enalbe = 0;

	if ((s_sBeep.StopTime == 0) || (s_sBeep.Cycle == 0))
	{
		BEEP_DISABLE();	/* 必须在清控制标志后再停止发声，避免停止后在中断中又开启 */
	}
}

/********************************************************************************
*	函 数 名: BEEP_KeyTone
*	功能说明: 发送按键音
*	形    参：无
*	返 回 值: 无
*******************************************************************************/
void bsp_BeepKeyTone(void)
{
	bsp_BeepStart(5, 1, 1);	/* 鸣叫50ms，停10ms， 1次 */
}

/*******************************************************************************
*	函 数 名: bsp_BeepTickService
*	功能说明: 每隔10ms调用1次该函数，用于控制蜂鸣器发声。该函数在 bsp.c 的bsp_RunPer10ms() 中被调用。
*	形    参：无
*	返 回 值: 无
*******************************************************************************/
void bsp_BeepTickService(void)
{
	if ((s_sBeep.Enalbe == 0) || (s_sBeep.StopTime == 0))
	{
		return;
	}

	if (s_sBeep.State == 0)
	{
		if (s_sBeep.StopTime > 0)	/* 间断发声 */
		{
			if (++s_sBeep.Count >= s_sBeep.BeepTime)
			{
				BEEP_DISABLE();		/* 停止发声 */
				s_sBeep.Count = 0;
				s_sBeep.State = 1;
			}
		}
		else
		{
			;	/* 不做任何处理，连续发声 */
		}
	}
	else if (s_sBeep.State == 1)
	{
		if (++s_sBeep.Count >= s_sBeep.StopTime)
		{
			/* 连续发声时，直到调用stop停止为止 */
			if (s_sBeep.Cycle > 0)
			{
				if (++s_sBeep.CycleCount >= s_sBeep.Cycle)
				{
					/* 循环次数到，停止发声 */
					s_sBeep.Enalbe = 0;
				}

				if (s_sBeep.Enalbe == 0)
				{
					s_sBeep.StopTime = 0;
					return;
				}
			}

			s_sBeep.Count = 0;
			s_sBeep.State = 0;

			BEEP_ENABLE();			/* 开始发声 */
		}
	}
}
//void bsp_BeepTickService(void)
//{
//	switch ()
//	{
//		case BEEP_START:
//			if (++s_sBeep.Count >= s_sBeep.StopTime)
//			{
//				
//			}
//	}
//}
