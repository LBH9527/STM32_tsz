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
#include "bsp.h"
#include "bsp_Gt9147.h"
/* 触屏模块用到的全局变量 */
TOUCH_T g_tTP;
/*
*********************************************************************************************************
*	函 数 名: bsp_InitTouch
*	功能说明: 配置STM32和触摸相关的口线，使能硬件SPI1, 片选由软件控制
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitTouch(void)
{
	g_tTP.Enable = 0;
	GT9147_Init();
	g_tTP.Write = 0;
	g_tTP.Read = 0;
	g_tTP.Enable = 1;	
}
/*
*********************************************************************************************************
*	函 数 名: TOUCH_Scan
*	功能说明: 触摸板事件检测程序。该函数被周期性调用，每ms调用1次. 见 bsp_Timer.c
*	形    参:  lcdDir 屏幕显示方向
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_TOUCHScan(uint16_t lcdDir)
{
	if (g_tTP.Enable == 0)
		return;
	GT9147_Timer1ms();
	GT9147_Scan();
}
/*
*********************************************************************************************************
*	函 数 名: TOUCH_PutKey
*	功能说明: 将1个触摸点坐标值压入触摸FIFO缓冲区。用于电阻触摸屏。
*	形    参: usX, _usY 坐标值
*	返 回 值: 无
*********************************************************************************************************
*/
void TOUCH_PutKey(uint8_t Event, uint16_t usX, uint16_t usY)
{
	g_tTP.Event[g_tTP.Write] = Event;
	g_tTP.XBuf[g_tTP.Write] = usX;
	g_tTP.YBuf[g_tTP.Write] = usY;

	if (++g_tTP.Write  >= TOUCH_FIFO_SIZE)
	{
		g_tTP.Write = 0;
	}
}
/*
*********************************************************************************************************
*	函 数 名: TOUCH_GetKey
*	功能说明: 从触摸FIFO缓冲区读取一个坐标值。
*	形    参:  无
*	返 回 值:
*			TOUCH_NONE      表示无事件
*			TOUCH_DOWN      按下
*			TOUCH_MOVE      移动
*			TOUCH_RELEASE	释放
*********************************************************************************************************
*/
uint8_t TOUCH_GetKey(uint16_t *_pX, uint16_t *_pY)
{
	uint8_t ret;

	if (g_tTP.Read == g_tTP.Write)
	{
		return TOUCH_NONE;
	}
	else
	{
		ret = g_tTP.Event[g_tTP.Read];
		*_pX = g_tTP.XBuf[g_tTP.Read];
		*_pY = g_tTP.YBuf[g_tTP.Read];

		if (++g_tTP.Read >= TOUCH_FIFO_SIZE)
		{
			g_tTP.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_CelarFIFO
*	功能说明: 清除触摸FIFO缓冲区
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void TOUCH_CelarFIFO(void)
{
	g_tTP.Write = g_tTP.Read;
}
/*
*********************************************************************************************************
*	函 数 名: TOUCH_InRect
*	功能说明: 判断当前坐标是否位于矩形框内
*	形    参:  _usX, _usY: 输入坐标
*			_usRectX,_usRectY: 矩形起点
*			_usRectH、_usRectW : 矩形高度和宽度
*	返 回 值: 1 表示在范围内
*********************************************************************************************************
*/
uint8_t TOUCH_InRect(uint16_t _usX, uint16_t _usY,uint16_t _usRectX, uint16_t _usRectY, uint16_t _usRectH, uint16_t _usRectW)
{
	if ((_usX > _usRectX) && (_usX < _usRectX + _usRectW)
		&& (_usY > _usRectY) && (_usY < _usRectY + _usRectH))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

