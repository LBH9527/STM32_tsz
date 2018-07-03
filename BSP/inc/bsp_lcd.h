/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : LCD_tft_lcd.h
*	版    本 : V2.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/


/******************************* LCD相关数据声明***************************/
/*
	LCD 颜色代码，CL_是Color的简写
	16Bit由高位至低位， RRRR RGGG GGGB BBBB

	下面的RGB 宏将24位的RGB值转换为16位格式。
	启动windows的画笔程序，点击编辑颜色，选择自定义颜色，可以获得的RGB值。

	推荐使用迷你取色器软件获得你看到的界面颜色。
*/
#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* 将8位R,G,B转化为 16位RGB565格式 */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)
enum
{
	CL_WHITE        = RGB(255,255,255),	/* 白色 */
	CL_BLACK        = RGB(  0,  0,  0),	/* 黑色 */
	CL_RED          = RGB(255,	0,  0),	/* 红色 */
	CL_GREEN        = RGB(  0,255,  0),	/* 绿色 */
	CL_BLUE         = RGB(  0,	0,255),	/* 蓝色 */
	CL_YELLOW       = RGB(255,255,  0),	/* 黄色 */

	CL_GREY			= RGB( 98, 98, 98), 	/* 深灰色 */
	CL_GREY1		= RGB( 150, 150, 150), 	/* 浅灰色 */
	CL_GREY2		= RGB( 180, 180, 180), 	/* 浅灰色 */
	CL_GREY3		= RGB( 200, 200, 200), 	/* 最浅灰色 */
	CL_GREY4		= RGB( 230, 230, 230), 	/* 最浅灰色 */

	CL_BUTTON_GREY	= RGB( 220, 220, 220), /* WINDOWS 按钮表面灰色 */

	CL_MAGENTA      = 0xF81F,	/* 红紫色，洋红色 */
	CL_CYAN         = 0x7FFF,	/* 蓝绿色，青色 */

	CL_BLUE1        = RGB(  0,  0, 240),		/* 深蓝色 */
	CL_BLUE2        = RGB(  0,  0, 128),		/* 深蓝色 */
	CL_BLUE3        = RGB(  68, 68, 255),		/* 浅蓝色1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* 浅蓝色1 */

	/* UI 界面 Windows控件常用色 */
	CL_BTN_FACE		= RGB(236, 233, 216),	/* 按钮表面颜色(灰) */
	
	CL_BTN_FONT		= CL_BLACK,				/* 按钮字体颜色（黑） */
	
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* 分组框主线颜色 */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* 分组框阴影线颜色 */


	CL_MASK			= 0x9999	/* 颜色掩码，用于文字背景透明 */
};

/* 文字对齐方式 */
enum
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

/* 编辑框风格 */
enum
{
	EDIT_BORDER_COLOR		= CL_BLUE2,		/* 编辑框四个边的颜色 */
	EDIT_BACK_COLOR			= CL_WHITE,			/* 编辑框背景 */
};

/* 按钮风格 */
enum
{
	BUTTON_BORDER_COLOR		= CL_BLUE2,			/* 按钮四个边的颜色 */
	BUTTON_BORDER1_COLOR	= CL_WHITE,			/* 按钮四个边内部2列的颜色 */
	BUTTON_BORDER2_COLOR	= CL_GREY1,			/* 按钮四个边内部2列的颜色 */
	BUTTON_BACK_COLOR		= CL_GREY3,			/* 按钮背景 */
	BUTTON_ACTIVE_COLOR		= CL_CYAN,			/* 激活的按钮背景 */
};

/* 窗口风格 */
enum
{
	WIN_BORDER_COLOR	= CL_BLUE4,		/* 窗口边框 */
	WIN_TITLE_COLOR		= CL_BLUE3,		/* 窗口标题蓝背景颜色 */
	WIN_CAPTION_COLOR	= CL_WHITE,		/* 窗口标题栏文字颜色 */
	WIN_BODY_COLOR		= CL_GREY2,		/* 窗体颜色 */
};

/* 检查框风格 */
enum
{
	CHECK_BOX_BORDER_COLOR	= CL_BLUE2,		/* 检查框四个边的颜色 */
	CHECK_BOX_BACK_COLOR	= CL_GREY3,		/* 检查框背景 */
	CHECK_BOX_CHECKED_COLOR	= CL_RED,		/* 检查框打勾的颜色 */

	CHECK_BOX_H			= 24,				/* 检查框高度 */
	CHECK_BOX_W			= 24,				/* 检查框高度 */
};

/* 字体代码 */
typedef enum
{
	FC_ST_12 = 0,		/* 宋体12x12点阵 （宽x高） */
	FC_ST_16,			/* 宋体15x16点阵 （宽x高） */
	FC_ST_24,			/* 宋体24x24点阵 （宽x高） -- 暂时未支持 */
	FC_ST_32,			/* 宋体32x32点阵 （宽x高） -- 暂时未支持 */	
}FONT_CODE_E;

/* 字体属性结构, 用于LCD_DispStr() */
/* 字体属性结构, 用于LCD_DispStr() */
typedef struct
{
	FONT_CODE_E FontCode;	/* 字体代码 FONT_CODE_E  */
	uint16_t FrontColor;/* 字体颜色 */
	uint16_t BackColor;	/* 文字背景颜色，透明 */
	uint16_t Space;		/* 文字间距，单位 = 像素 */
}FONT_T;

/* 控件ID */
typedef enum
{
	ID_ICON		= 1,
	ID_WIN		= 2,
	ID_LABEL	= 3,
	ID_BUTTON	= 4,
	ID_CHECK 	= 5,
	ID_EDIT 	= 6,
	ID_GROUP 	= 7,
}CONTROL_ID_T;

/* 图标结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;		/* 左上角X坐标 */
	uint16_t Top;		/* 左上角Y坐标 */
	uint16_t Height;	/* 图标高度 */
	uint16_t Width;		/* 图标宽度 */
	uint16_t *pBmp;		/* 指向图标图片数据 */
	char  Text[16];	/* 图标文本, 最多显示5个汉字16点阵 */
}ICON_T;

/* 窗体结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;
	char *pCaption;
}WIN_T;

/* 文本标签结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	uint16_t MaxLen;		/* 字符串长度 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
}LABEL_T;

/* 按钮结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	/* 按钮的颜色，由底层自动管理 */
	FONT_T *Font;			/* 字体 */
	char *pCaption;
	uint8_t Focus;			/* 焦点 */
}BUTTON_T;

/* 编辑框结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;			/* 字体 */
	char   *pCaption;
	char Text[32];			/* 用于存放编辑内容 */
}EDIT_T;

/* 检查框 CHECK BOX 结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	uint16_t Color;			/* 颜色 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
	uint8_t Checked;		/* 1表示打勾 */
}CHECK_T;

/* 分组框GROUP BOX 结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
}GROUP_T;



#ifndef _BSP_TFT_LCD_H
#define _BSP_TFT_LCD_H
/* 可供外部模块调用的函数 */
void bsp_InitLCD(void);
void bsp_LCDBackLightSet(uint8_t level);														//背光调节
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);							//画线
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);							//画波形
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);		//画矩形
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);									//画
void LCD_DispStrEx(uint16_t _usX, uint16_t _usY, uint8_t *_ptr, FONT_T *_tFont, uint16_t _Width,uint8_t _Align);
void LCD_DispStr(uint16_t _usX, uint16_t _usY, uint8_t *_ptr, FONT_T *_tFont);

void LCD_DrawIcon32(const ICON_T *tIcon, FONT_T *tFont, uint8_t FocusMode);
void LCD_DrawBmp32(uint16_t usX, uint16_t usY, uint16_t Height, uint16_t Width, uint8_t *pBmp);
#endif
