/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_SysTick
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*
*********************************************************************************************************
*/

#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

/*
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/
//#define TMR_COUNT	4		/* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 3) */
enum {
    UserTim1,
//    UserTim2,
//    UserTim3,
//    UserTim4,
    TMR_COUNT,			/* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 ) */
};
/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;

/* �����ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint32_t Count;	    /* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
}SOFT_TMR_S;

/* �ṩ������C�ļ����õĺ��� */
void bsp_InitSysTick(void);
void SysTick_ISR(void);
void bsp_DelayMS(__IO uint32_t Delay);
void bsp_DelayUS(uint32_t n);
void bsp_StartSoftTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoSoftTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
int32_t bsp_GetRunTime(void);
int32_t bsp_CheckRunTime(int32_t _LastTime);


#endif

