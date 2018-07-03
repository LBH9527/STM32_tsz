/*
*********************************************************************************************************
*
*	模块名称 : 电容触摸模块
*	文件名称 : bsp_LcdTouch.c
*	版    本 : 
*	说    明 : 
*	修改记录 :
*		版本号  日期       作者    说明
*		
*********************************************************************************************************
*/
#ifndef BSP_LCDTOUCH_H_
#define BSP_LCDTOUCH_H_

#include "stdint.h"

#define TOUCH_FIFO_SIZE		20
#define TOUCH_MAX_POINT  	5    		//电容屏支持的点数,固定为5点
#define TP_PRES_DOWN 0x80               //触屏被按下	  
#define TP_CATH_PRES 0x40               //有按键按下了 
 
//触摸屏控制器
//typedef struct
//{

//	u16 x[TOUCH_MAX_POINT]; 	//当前坐标
//	u16 y[TOUCH_MAX_POINT];		//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
//								//x[4],y[4]存储第一次按下时的坐标. 
//	u8  sta;					//笔的状态 
//								//b7:按下1/松开0; 
//	                            //b6:0,没有按键按下;1,有按键按下. 
//								//b5:保留
//								//b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)

//}_m_tp_dev;
//extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义
typedef struct
{
	u8 TimerCount;				//触摸刷新时间
	
	u16 x[TOUCH_MAX_POINT]; 	//当前坐标
	u16 y[TOUCH_MAX_POINT];		//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								//x[4],y[4]存储第一次按下时的坐标. 
}GT9147_T;
extern GT9147_T	g_tGT9147;		


typedef struct
{
	uint8_t Enable;		/* 触摸检测使能标志 */

	uint8_t Event[TOUCH_FIFO_SIZE];	/* 触摸事件 */
	int16_t XBuf[TOUCH_FIFO_SIZE];	/* 触摸坐标缓冲区 */
	int16_t YBuf[TOUCH_FIFO_SIZE];	/* 触摸坐标缓冲区 */
	uint8_t Read;					/* 缓冲区读指针 */
	uint8_t Write;					/* 缓冲区写指针 */
}TOUCH_T;
extern TOUCH_T g_tTP;
/* 触摸事件 */
enum
{
	TOUCH_NONE = 0,		/* 无触摸 */
	TOUCH_DOWN = 1,		/* 按下 */
	TOUCH_MOVE = 2,		/* 移动 */
	TOUCH_RELEASE = 3	/* 释放 */
};

/* 提供给其他C文件调用的函数 */
void bsp_InitTouch(void);
void bsp_TOUCHScan(uint16_t lcdDir);
u8 TOUCH_GetKey(u16 *_pX, u16 *_pY);
void TOUCH_PutKey(u8 Event, u16 usX, u16 usY);
u8 TOUCH_InRect(u16 _usX, u16 _usY,u16 _usRectX, u16 _usRectY, u16 _usRectH, u16 _usRectW);
#endif
