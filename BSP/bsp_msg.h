/*
*********************************************************************************************************
*
*	ģ������ : ��Ϣ����ģ��
*	�ļ����� : bsp_msg.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_MSG_H
#define __BSP_MSG_H

#define MSG_FIFO_SIZE    40	   		/* ��Ϣ���� */
enum 
{
	MSG_NONE = 0,
	MSG_KEY0_DOWN,      /* 0������ */
    MSG_KEY0_LONG,      /* 0������ */

    MSG_KEY1_DOWN,      /* 1������ */
    MSG_KEY1_LONG,      /* 1������ */

    MSG_KEY2_UP,        /* 2������ */
    MSG_KEY2_LONG,      /* 2������ */   

	MSG_KEY3_UP,		/* 3������ */
	MSG_KEY3_LONG,		/* 3������ */  
	
	MSG_MODBUS_03H,				/* */
	MSG_MODBUS_06H,					/**/
	MSG_MODBUS_10H,					/**/
	
	MSG_NEW_COUNT	,				/* ��������������Ϣ */	
	MSG_SECOND,						/* ʱ������� */	
	MSG_SECOND1,					/* ʱ�������,��DS1307�󷢴���Ϣ */	
	
	MSG_GH1_CHANGE,					/* ����1��� */
	MSG_GH2_CHANGE,					/* ����2��� */
	MSG_GH3_CHANGE,					/* ����3��� */
	
	MSG_COUNT_CHANGE, 				/* ʵʱ������� */
	
	MSG_433M_OK,	/* 433M ͨ��OK */
};

/* ����FIFO�õ����� */
typedef struct
{
	uint16_t MsgCode;		/* ��Ϣ���� */
	uint32_t MsgParam;		/* ��Ϣ��������, Ҳ������ָ�루ǿ��ת���� */
}MSG_S;

/* ����FIFO�õ����� */
typedef struct
{
	MSG_S Buf[MSG_FIFO_SIZE];	/* ��Ϣ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}MSG_FIFO_S;

/* ���ⲿ���õĺ������� */
void bsp_InitMsg(void);
void bsp_PutMsg(uint16_t _MsgCode, uint32_t _MsgParam);
uint8_t bsp_GetMsg(MSG_S *_pMsg);
uint8_t bsp_GetMsg2(MSG_S *_pMsg);
void bsp_ClearMsg(void);

#endif


