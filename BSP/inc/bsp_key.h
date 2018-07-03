/*
********************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_key.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
********************************************************************************
*/
#ifndef __BSP_KEY_H
#define __BSP_KEY_H	

#include <stdbool.h>
#include <stdint.h>

#define KEY_COUNT   4               //��������(����ϰ���ҲҪ���)

#define KEY0 	PEin(4)		        //����ӦGPIOE->IDR�Ĵ���
#define KEY1	PEin(3)
#define KEY2 	PEin(2)
#define WK_UP 	PAin(0)

/*
	�����˲�ʱ��100ms, ��λ10ms��
	ֻ��������⵽100ms״̬�������Ϊ��Ч����������Ͱ��������¼�	
*/
#define KEY_DOWN_PERIOD     10		//��λ10ms�� ����80ms����Ϊ��������
#define KEY_LONG_PERIOD     200		//��λ10ms�� ����2�룬��Ϊ�����¼�
#define KEY_REPEAT_PERIOD   5       //��λ10ms�� ������������Ϊ50ms

/* 
    ����Ӧ�ó���Ĺ���������������,���Ը�����Ҫ����ļ�ֵ�����вü�
*/
#define KEY_DOWN_K0		KEY0_DOWN
#define KEY_LONG_K0		KEY0_LONG

#define KEY_DOWN_K1		KEY1_DOWN
#define KEY_LONG_K1		KEY1_LONG

#define KEY_DOWN_K2		KEY2_DOWN
#define KEY_LONG_K2		KEY2_LONG

#define WK_UP_DOWN      KEY_3_DOWN
#define WK_UP_LONG      KEY_3_LONG
/* 
    ����ID, ��Ҫ����GetKeyState()��������ڲ���
*/
typedef enum
{    
    KID_K0,
	KID_K1,
	KID_K2,
	KID_WK_UP,
}KEY_ID_E;
/* 
    ����״̬
*/
enum
{    
    STATE_KEY_UP,    
    STATE_KEY_DEBOUNCE,    
    STATE_KEY_DOWN,    
    STATE_KEY_LONG,    
    STATE_KEY_REPEAT,    
    STATE_KEY_RELEASE
};

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�
	���Ը���Ҫ��ѡȡ�¼����������
*/
enum
{
    KEY_NONE = 0,   /* 0 ��ʾ�ް����¼� */

    KEY0_DOWN,      /* 0������ */
    KEY0_UP,        /* 0������ */
    KEY0_LONG,      /* 0������ */

    KEY1_DOWN,      /* 1������ */
    KEY1_UP,        /* 1������ */
    KEY1_LONG,      /* 1������ */

    KEY2_DOWN,      /* 2������ */
    KEY2_UP,        /* 2������ */
    KEY2_LONG,      /* 2������ */   

	KEY_WK_DOWN,	    /* 3������ */
	KEY_WK_UP,		/* 3������ */
	KEY_WK_LONG,		/* 3������ */    
};

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */
    uint8_t FilterTime;             /* �˲�ʱ��(���255,��ʾ2550ms) */  
	uint8_t Count;			        /* ���������� */
	uint8_t LongTime;		        /* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t RepeatTime;	            /* ������������ , 0��ʾ����ⳤ��*/
	uint8_t State;                  /*����״̬*/  
     
    uint8_t KeyCodeUp;              /* ��������ļ�ֵ���� */  
    uint8_t KeyCodeDown;            /* �������µļ�ֵ���� */  
    uint8_t KeyCodeLong;            /* ���������ļ�ֵ���� */  

}KEY_PARAM_S;
/* 
    ����FIFO�õ��ı��� 
*/
#define KEY_FIFO_SIZE   10
typedef struct
{
    uint8_t Buf[KEY_FIFO_SIZE];      /* ��ֵ������ */
    uint8_t Read;                    /* ��������ָ��1 */
    uint8_t Write;                   /* ������дָ�� */
}KEY_FIFO_S;

/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan(void);
uint8_t bsp_GetKeyMsg(void);
void bsp_ClearKeyFifo(void);

#endif


