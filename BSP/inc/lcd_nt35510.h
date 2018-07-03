#ifndef __LCD_NT35510_H
#define __LCD_NT35510_H

#include <stdint.h>

#define LCD_LED		PBout(15)		//LCD背光	PB15

typedef struct
{
	uint16_t width;				/* 显示屏分辨率-宽度 */
	uint16_t height;			/* 显示屏分辨率-高度 */
	uint8_t  direction;			/* 常用显示方向.0,3,5,6 */
}s_LCD_DEV;
extern s_LCD_DEV		g_lcddev;	//LCD参数

//lcd的画笔颜色和背景色
extern uint16_t POINT_COLOR;		//默认红色
extern uint16_t BACK_COLOR;			//背景颜色，默认白色

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上	竖屏(排针在上)

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	


void Init_NT35510(void);
void bsp_LCDDisplayDir(uint8_t dir);											//设置屏幕显示方向												  
void LCD_DisplayOn(void);														//开显示
void LCD_DisplayOff(void);														//关显示
void LCD_Clear(uint16_t Color);	 												//清屏
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);								//设置光标
void LCD_DrawPoint(uint16_t x,uint16_t y);										//画点
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);								//快速画点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); 											//读点 

void LCD_Fill_Rect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);		   				//填充单色
void LCD_Fill_ColorRect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);				//填充指定颜色

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,u32 num,uint8_t len,uint8_t size);  						//显示一个数字
void LCD_ShowxNum(uint16_t x,uint16_t y,u32 num,uint8_t len,uint8_t size,uint8_t mode);				//显示 数字
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//显示一个字符串,12/16字体


void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//设置窗口	

void LCD_DrawImage(uint16_t x0, uint16_t y0, uint16_t height, uint16_t width, uint16_t *ptr);

#endif

