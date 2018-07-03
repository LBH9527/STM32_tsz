#ifndef __LCD_NT35510_H
#define __LCD_NT35510_H

#include <stdint.h>

#define LCD_LED		PBout(15)		//LCD����	PB15

typedef struct
{
	uint16_t width;				/* ��ʾ���ֱ���-��� */
	uint16_t height;			/* ��ʾ���ֱ���-�߶� */
	uint8_t  direction;			/* ������ʾ����.0,3,5,6 */
}s_LCD_DEV;
extern s_LCD_DEV		g_lcddev;	//LCD����

//lcd�Ļ�����ɫ�ͱ���ɫ
extern uint16_t POINT_COLOR;		//Ĭ�Ϻ�ɫ
extern uint16_t BACK_COLOR;			//������ɫ��Ĭ�ϰ�ɫ

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���	����(��������)

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	


void Init_NT35510(void);
void bsp_LCDDisplayDir(uint8_t dir);											//������Ļ��ʾ����												  
void LCD_DisplayOn(void);														//����ʾ
void LCD_DisplayOff(void);														//����ʾ
void LCD_Clear(uint16_t Color);	 												//����
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);								//���ù��
void LCD_DrawPoint(uint16_t x,uint16_t y);										//����
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);								//���ٻ���
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); 											//���� 

void LCD_Fill_Rect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);		   				//��䵥ɫ
void LCD_Fill_ColorRect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);				//���ָ����ɫ

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//��ʾһ���ַ�
void LCD_ShowNum(uint16_t x,uint16_t y,u32 num,uint8_t len,uint8_t size);  						//��ʾһ������
void LCD_ShowxNum(uint16_t x,uint16_t y,u32 num,uint8_t len,uint8_t size,uint8_t mode);				//��ʾ ����
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//��ʾһ���ַ���,12/16����


void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//���ô���	

void LCD_DrawImage(uint16_t x0, uint16_t y0, uint16_t height, uint16_t width, uint16_t *ptr);

#endif

