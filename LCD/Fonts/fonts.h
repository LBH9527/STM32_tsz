#ifndef __FONTS_H__
#define __FONTS_H__	 
#include <stm32f4xx.h>
#include "fontupd.h"

//typedef struct _tFont
//{    
//  const uint16_t *table;
//  uint16_t Width;
//  uint16_t Height;
//  
//} FONT_T;
extern const unsigned char  Ascii_1206[95][12];
extern const unsigned char  Ascii_1608[95][16];
extern const unsigned char  Ascii_2412[95][36];

extern const unsigned char  Hz12[];
extern const unsigned char  Hz16[];
extern const unsigned char  Hz24[];
extern const unsigned char  Hz32[];

//typedef struct 
//{
//	const unsigned char *table;
//	u16 Width;
//	u16 Height;
//	u16 FontSize;
//}_FONT_T;
//extern _FONT_T Ascii1206;
//extern _FONT_T Ascii1608;
//extern _FONT_T Ascii2412;

//extern _FONT_T Hz12;
//extern _FONT_T Hz16;
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//得到汉字的点阵码
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//在指定位置显示一个汉字
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//在指定位置显示一个字符串 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
#endif


