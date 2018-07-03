/*
*********************************************************************************************************
*
*	模块名称 : 消息处理模块
*	文件名称 : bsp_msg.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_MSG_H
#define __BSP_MSG_H

#define MSG_FIFO_SIZE    40	   		/* 消息个数 */
enum 
{
	MSG_NONE = 0,
	MSG_KEY0_DOWN,      /* 0键按下 */
    MSG_KEY0_LONG,      /* 0键长按 */

    MSG_KEY1_DOWN,      /* 1键按下 */
    MSG_KEY1_LONG,      /* 1键长按 */

    MSG_KEY2_UP,        /* 2键弹起 */
    MSG_KEY2_LONG,      /* 2键长按 */   

	MSG_KEY3_UP,		/* 3键弹起 */
	MSG_KEY3_LONG,		/* 3键长按 */  
	
	MSG_MODBUS_03H,				/* */
	MSG_MODBUS_06H,					/**/
	MSG_MODBUS_10H,					/**/
	
	MSG_NEW_COUNT	,				/* 产量计数脉冲消息 */	
	MSG_SECOND,						/* 时钟秒更新 */	
	MSG_SECOND1,					/* 时钟秒更新,读DS1307后发此消息 */	
	
	MSG_GH1_CHANGE,					/* 工号1表更 */
	MSG_GH2_CHANGE,					/* 工号2表更 */
	MSG_GH3_CHANGE,					/* 工号3表更 */
	
	MSG_COUNT_CHANGE, 				/* 实时产量变更 */
	
	MSG_433M_OK,	/* 433M 通信OK */
};

/* 按键FIFO用到变量 */
typedef struct
{
	uint16_t MsgCode;		/* 消息代码 */
	uint32_t MsgParam;		/* 消息的数据体, 也可以是指针（强制转化） */
}MSG_S;

/* 按键FIFO用到变量 */
typedef struct
{
	MSG_S Buf[MSG_FIFO_SIZE];	/* 消息缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}MSG_FIFO_S;

/* 供外部调用的函数声明 */
void bsp_InitMsg(void);
void bsp_PutMsg(uint16_t _MsgCode, uint32_t _MsgParam);
uint8_t bsp_GetMsg(MSG_S *_pMsg);
uint8_t bsp_GetMsg2(MSG_S *_pMsg);
void bsp_ClearMsg(void);

#endif


