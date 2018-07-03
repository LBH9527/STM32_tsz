/*
*********************************************************************************************************
*
*	ģ������ : ���ݴ���ģ��
*	�ļ����� : bsp_LcdTouch.c
*	��    �� : 
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		
*********************************************************************************************************
*/
#ifndef BSP_LCDTOUCH_H_
#define BSP_LCDTOUCH_H_

#include "stdint.h"

#define TOUCH_FIFO_SIZE		20
#define TOUCH_MAX_POINT  	5    		//������֧�ֵĵ���,�̶�Ϊ5��
#define TP_PRES_DOWN 0x80               //����������	  
#define TP_CATH_PRES 0x40               //�а��������� 
 
//������������
//typedef struct
//{

//	u16 x[TOUCH_MAX_POINT]; 	//��ǰ����
//	u16 y[TOUCH_MAX_POINT];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
//								//x[4],y[4]�洢��һ�ΰ���ʱ������. 
//	u8  sta;					//�ʵ�״̬ 
//								//b7:����1/�ɿ�0; 
//	                            //b6:0,û�а�������;1,�а�������. 
//								//b5:����
//								//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)

//}_m_tp_dev;
//extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��
typedef struct
{
	u8 TimerCount;				//����ˢ��ʱ��
	
	u16 x[TOUCH_MAX_POINT]; 	//��ǰ����
	u16 y[TOUCH_MAX_POINT];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
								//x[4],y[4]�洢��һ�ΰ���ʱ������. 
}GT9147_T;
extern GT9147_T	g_tGT9147;		


typedef struct
{
	uint8_t Enable;		/* �������ʹ�ܱ�־ */

	uint8_t Event[TOUCH_FIFO_SIZE];	/* �����¼� */
	int16_t XBuf[TOUCH_FIFO_SIZE];	/* �������껺���� */
	int16_t YBuf[TOUCH_FIFO_SIZE];	/* �������껺���� */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;					/* ������дָ�� */
}TOUCH_T;
extern TOUCH_T g_tTP;
/* �����¼� */
enum
{
	TOUCH_NONE = 0,		/* �޴��� */
	TOUCH_DOWN = 1,		/* ���� */
	TOUCH_MOVE = 2,		/* �ƶ� */
	TOUCH_RELEASE = 3	/* �ͷ� */
};

/* �ṩ������C�ļ����õĺ��� */
void bsp_InitTouch(void);
void bsp_TOUCHScan(uint16_t lcdDir);
u8 TOUCH_GetKey(u16 *_pX, u16 *_pY);
void TOUCH_PutKey(u8 Event, u16 usX, u16 usY);
u8 TOUCH_InRect(u16 _usX, u16 _usY,u16 _usRectX, u16 _usRectY, u16 _usRectH, u16 _usRectW);
#endif
