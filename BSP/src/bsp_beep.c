/*
*********************************************************************************************************
*
*	ģ������ : ����������ģ��
*	�ļ����� : bsp_beep.c
*	��    �� : V1.0
*	˵    �� : ����������.
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    			9527	��Դ������
*
*	
*
*********************************************************************************************************
*/

#include "bsp.h"

#define BEEP_ENABLE()   GPIO_SetBits(GPIOF, GPIO_Pin_8)       	/* ʹ�ܷ��������� */
#define BEEP_DISABLE()  GPIO_ResetBits(GPIOF, GPIO_Pin_8)		/* ��ֹ���������� */
static BEEP_T s_sBeep;		/* ���������ȫ�ֽṹ����� */


/*******************************************************************************
*��������InitKeyHard()
* ���� ��������Ӳ����ʼ��
* ���� ��
* ��� ��
*******************************************************************************/
static void InitBeepHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	//ʹ��GPIOFʱ��
	//��������ӦGPIOF8 ��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_100MHz;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);						//��������ӦIO������
}

/*******************************************************************************
*	�� �� ��: bsp_InitBeep
*	����˵��: ��ʼ��������. �ú����� bsp_Init() ���á�
*	��    ��: ��
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_InitBeep(void)
{
	InitBeepHard();
}	

/********************************************************************************
*	�� �� ��: BEEP_Start
*	����˵��: ������������
*	��    �Σ�BeepTime : ����ʱ�䣬��λ10ms; 0 ��ʾ������
*			  StopTime : ֹͣʱ�䣬��λ10ms; 0 ��ʾ��������
*			 Cycle : ���д����� 0 ��ʾ��������
*	�� �� ֵ: ��
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
	s_sBeep.Enalbe = 1;	/* ������ȫ�ֲ�������ʹ�ܷ�����־ */

	BEEP_ENABLE();			/* ��ʼ���� */
}

/*******************************************************************************
*	�� �� ��: BEEP_Stop
*	����˵��: ֹͣ��������
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_BeepStop(void)
{
	s_sBeep.Enalbe = 0;

	if ((s_sBeep.StopTime == 0) || (s_sBeep.Cycle == 0))
	{
		BEEP_DISABLE();	/* ����������Ʊ�־����ֹͣ����������ֹͣ�����ж����ֿ��� */
	}
}

/********************************************************************************
*	�� �� ��: BEEP_KeyTone
*	����˵��: ���Ͱ�����
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_BeepKeyTone(void)
{
	bsp_BeepStart(5, 1, 1);	/* ����50ms��ͣ10ms�� 1�� */
}

/*******************************************************************************
*	�� �� ��: bsp_BeepTickService
*	����˵��: ÿ��10ms����1�θú��������ڿ��Ʒ������������ú����� bsp.c ��bsp_RunPer10ms() �б����á�
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_BeepTickService(void)
{
	if ((s_sBeep.Enalbe == 0) || (s_sBeep.StopTime == 0))
	{
		return;
	}

	if (s_sBeep.State == 0)
	{
		if (s_sBeep.StopTime > 0)	/* ��Ϸ��� */
		{
			if (++s_sBeep.Count >= s_sBeep.BeepTime)
			{
				BEEP_DISABLE();		/* ֹͣ���� */
				s_sBeep.Count = 0;
				s_sBeep.State = 1;
			}
		}
		else
		{
			;	/* �����κδ����������� */
		}
	}
	else if (s_sBeep.State == 1)
	{
		if (++s_sBeep.Count >= s_sBeep.StopTime)
		{
			/* ��������ʱ��ֱ������stopֹͣΪֹ */
			if (s_sBeep.Cycle > 0)
			{
				if (++s_sBeep.CycleCount >= s_sBeep.Cycle)
				{
					/* ѭ����������ֹͣ���� */
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

			BEEP_ENABLE();			/* ��ʼ���� */
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
