/*
*********************************************************************************************************
*
*	ģ������ : ���ֵ����ֿ⡣��Ч��ʾ�� ��11x��11, ����һ������
*	�ļ����� : hz.c
*	˵    �� : ֻ�����������õ������ֿ⣬��Դ��PC2LCD2002ȡģ��ʽ���ã�����+����ʽ+˳��+C51��ʽ 
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-09-08 armfly  ST�̼���V3.5.0�汾��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "fonts.h"
#if 1
/*
	FLASH����ǶС�ֿ⣬ֻ�����������õ��ĺ��ֵ���
	ÿ�е������ݣ�ͷ2�ֽ��Ǻ��ӵ����룬����32�ֽ���16�����ӵ���ģ���ݡ�
*/

 /* 
	ʵ(0) ʱ(1) ��(2) ��(3) ��(4) Ļ(5) ��(6)
 */
 //��������� 24
unsigned char const Hz12[] = {	
0xCA, 0xB5,	0x11,0x00,0x61,0x20,0x45,0x20,0x53,0x20,0x49,0x40,0xC1,0x80,
			0x5F,0x00,0x41,0x80,0x41,0x40,0x51,0x20,0x61,0x00,0x00,0x00,/*"ʵ",0*/
0xCA, 0xB1,	0x7F,0xC0,0x44,0x80,0x44,0x80,0x7F,0xC0,0x20,0x00,0x28,0x00,
			0x26,0x20,0x20,0x20,0xFF,0xE0,0x20,0x00,0x20,0x00,0x00,0x00,/*"ʱ",1*/
0xCE, 0xC2,	0x91,0xE0,0x4E,0x00,0x00,0x20,0x03,0xE0,0xFA,0x20,0xAB,0xE0,
			0xAA,0x20,0xAB,0xE0,0xAA,0x20,0xFB,0xE0,0x00,0x20,0x00,0x00,/*"��",2*/
0xB6, 0xC8,	0x00,0x60,0x7F,0x80,0x50,0x20,0x51,0x20,0x7D,0x20,0x55,0xC0,
			0xD5,0x40,0x55,0xC0,0x7D,0x20,0x50,0x20,0x50,0x20,0x00,0x00,/*"��",3*/
0xC6, 0xC1,	0x00,0x60,0xFF,0x80,0xA1,0x20,0xB5,0x40,0xAF,0x80,0xA5,0x00,
			0xA5,0x00,0xAF,0xE0,0xB5,0x00,0xE5,0x00,0x01,0x00,0x00,0x00,/*"��",4*/
0xC4, 0xBB,	0x82,0x80,0x82,0x80,0xBF,0xE0,0xAA,0x80,0xEA,0x80,0xAB,0xE0,
			0xAA,0x80,0xEA,0xA0,0xBF,0xE0,0x82,0x80,0x82,0x80,0x00,0x00,/*"Ļ",5*/
0xC1, 0xC1,	0x46,0x20,0x44,0x20,0x5C,0x40,0x55,0x80,0x55,0x00,0xD5,0x00,
			0x55,0x00,0x55,0xE0,0x5C,0x20,0x44,0x20,0x46,0xE0,0x00,0x00,/*"��",6*/

	/* ���һ�б�����0xFF,0xFF�����������ֿ����������־ */
0xFF,0xFF

};
 /* 
	ʵ(0) ʱ(1) ��(2) ��(3) ��(4) Ļ(5) ��(6)
 */
unsigned char const Hz16[] = {	
0xCA, 0xB5,	0x00,0x00,0x08,0x41,0x30,0x41,0x20,0x42,0x32,0x42,0x2D,0xC4,0x29,0x48,0xA0,0x50,
			0x6F,0xE0,0x20,0x50,0x20,0x48,0x20,0x47,0x28,0x42,0x30,0x40,0x20,0x40,0x00,0x00,/*0*/
0xCA, 0xB1, 0x00,0x00,0x3F,0xE0,0x22,0x20,0x22,0x20,0x22,0x20,0x3F,0xE0,0x08,0x00,0x09,0x00,
			0x08,0xC0,0x08,0x02,0x08,0x01,0xFF,0xFE,0x08,0x00,0x08,0x00,0x08,0x00,0x00,0x00,/*1*/
0xCE, 0xC2, 0x08,0x40,0x84,0x7F,0x61,0x80,0x0E,0x02,0x00,0xFE,0x7E,0x82,0x52,0x82,0x52,0xFE,
			0x52,0x82,0x52,0x82,0x52,0xFE,0x7E,0x82,0x00,0x82,0x00,0xFE,0x00,0x02,0x00,0x00,/*2*/
0xB6, 0xC8, 0x00,0x01,0x00,0x06,0x3F,0xF8,0x20,0x01,0x24,0x01,0x24,0x42,0x3F,0x62,0xA5,0x54,
			0x65,0x48,0x25,0x48,0x3F,0x54,0x24,0x64,0x24,0x42,0x24,0x03,0x20,0x02,0x00,0x00,/*3*/
0xC6, 0xC1, 0x00,0x02,0x00,0x0C,0xFF,0xF0,0x90,0x41,0x92,0x42,0x9A,0x4C,0x97,0xF0,0x92,0x40,
			0x92,0x40,0x92,0x40,0x97,0xFF,0x9A,0x40,0x92,0x40,0xF2,0x40,0x00,0x40,0x00,0x00,/*4*/
0xC4, 0xBB, 0x40,0x44,0x40,0x44,0x40,0x48,0x5F,0x50,0x55,0x7E,0xF5,0x50,0x55,0xD0,0x55,0x7F,
			0x55,0x50,0xF5,0x52,0x55,0x5E,0x5F,0x60,0x40,0x50,0x40,0x4C,0x40,0x48,0x00,0x00,/*5*/
0xC1, 0xC1, 0x00,0x40,0x41,0x82,0x41,0x02,0x41,0x04,0x5D,0x08,0x55,0x70,0x55,0x40,0xD5,0x40,
			0x55,0x40,0x55,0x7C,0x5D,0x02,0x41,0x02,0x41,0x42,0x43,0x82,0x01,0x0E,0x00,0x00,/*6*/
0xBF, 0xAA, 0x01,0x00,0x41,0x01,0x41,0x02,0x41,0x0C,0x7F,0xF0,0x41,0x00,0x41,0x00,0x41,0x00,
			0x41,0x00,0x41,0x00,0x7F,0xFF,0x41,0x00,0x41,0x00,0x41,0x00,0x01,0x00,0x00,0x00,/*"?",0*/
0xB7, 0xA2, 0x00,0x00,0x00,0x04,0x18,0x08,0x68,0x31,0x08,0xC1,0x0B,0x01,0x1D,0x82,0xE9,0x62,
			0x09,0x14,0x09,0x08,0x09,0x14,0x49,0x22,0x29,0xC2,0x08,0x01,0x00,0x01,0x00,0x00,/*"?",1*/
0xB0, 0xE5, 0x08,0x20,0x08,0xC0,0x0B,0x00,0xFF,0xFF,0x09,0x00,0x08,0xC1,0x00,0x06,0x3F,0xF8,
			0x24,0x01,0x27,0x82,0x24,0x64,0x44,0x18,0xC4,0x64,0x47,0x82,0x00,0x01,0x00,0x00,/*"?",2*/
	/* ���һ�б�����0xFF,0xFF�����������ֿ����������־ */
0xFF,0xFF

};
unsigned char const Hz24[] = {
0xCA, 0xB5,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x07,0x80,0x81,0x03,0x00,0x82,0x02,
			0x10,0x82,0x02,0x08,0x82,0x02,0x0E,0x84,0x02,0xC4,0x84,0x02,0x60,0x88,0x42,0x00,
			0x98,0x22,0x00,0xF0,0x3A,0x07,0xC0,0x12,0xFF,0x20,0x02,0x80,0xA0,0x02,0x00,0x90,
			0x02,0x00,0x90,0x02,0x00,0x98,0x02,0x80,0x8C,0x03,0x01,0x0E,0x06,0x03,0x02,0x06,
			0x01,0x00,0x02,0x00,0x00,0x00,0x00,0x00,/*0*/
0xCA, 0xB1, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xF0,0x04,0x10,0x20,0x04,
			0x10,0x20,0x04,0x10,0x20,0x04,0x10,0x20,0x0F,0xFF,0xF0,0x04,0x80,0x00,0x00,0x80,
			0x00,0x00,0x90,0x00,0x00,0x98,0x00,0x00,0x8E,0x00,0x00,0x84,0x04,0x00,0x80,0x04,
			0x00,0x80,0x06,0x3F,0xFF,0xFE,0x3F,0xFF,0xF8,0x00,0x80,0x00,0x01,0x00,0x00,0x01,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*1*/
0xCE, 0xC2, 0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x40,0x8C,0x10,0x70,0xFC,0x18,0x03,0x80,0x0C,
			0x1C,0x02,0x00,0xE0,0x02,0x01,0x04,0x02,0x00,0x03,0xFC,0x1F,0xFA,0x02,0x11,0x12,
			0x02,0x11,0x12,0x02,0x11,0x13,0xFC,0x11,0x12,0x02,0x11,0x12,0x02,0x11,0x13,0xFC,
			0x11,0x12,0x02,0x11,0x12,0x02,0x3F,0xF2,0x02,0x10,0x03,0xFC,0x00,0x02,0x02,0x00,
			0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x00,/*2*/
0xB6, 0xC8, 0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x18,0x00,0x07,0xE0,0x0F,0xFE,0x00,0x04,
			0x40,0x02,0x04,0x40,0x02,0x04,0x42,0x02,0x04,0x42,0x02,0x04,0x46,0x04,0x07,0xFB,
			0x04,0x46,0x4A,0xC8,0x38,0x4A,0x28,0x14,0x4A,0x10,0x04,0x4A,0x38,0x04,0x4A,0x68,
			0x07,0xFB,0xCC,0x06,0x43,0x04,0x04,0x42,0x06,0x04,0x40,0x06,0x08,0x80,0x06,0x08,
			0x80,0x04,0x00,0x00,0x00,0x00,0x00,0x00,/*3*/
0xC6, 0xC1, 0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x18,0x00,0x00,0xE0,0x3F,0xFF,0x80,0x1F,
			0xE0,0x80,0x12,0x00,0x82,0x12,0x10,0x82,0x12,0x10,0x84,0x13,0x10,0x98,0x12,0x9F,
			0xF0,0x12,0x7F,0x80,0x12,0x10,0x80,0x12,0x10,0x80,0x12,0x10,0x80,0x12,0x30,0x80,
			0x13,0xDF,0xFE,0x12,0x90,0x80,0x12,0x10,0x80,0x1F,0x30,0x80,0x1E,0x11,0x00,0x00,
			0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*4*/
0xC4, 0xBB, 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x04,0x40,0x08,0x04,0x40,0x08,0x04,0x80,0x08,
			0x04,0x80,0x0B,0xF5,0xFC,0x0A,0x96,0x7C,0x0A,0x96,0x40,0x3E,0x9C,0x40,0x0A,0x9C,
			0x40,0x0A,0x97,0xFE,0x0A,0x95,0x46,0x0A,0x94,0x40,0x3E,0x94,0x40,0x2A,0x96,0x44,
			0x0A,0x95,0xFC,0x0F,0xF5,0x78,0x0A,0x04,0x80,0x08,0x04,0x80,0x10,0x04,0xC0,0x10,
			0x0C,0xC0,0x00,0x04,0x00,0x00,0x00,0x00,/*5*/
0xC1, 0xC1, 0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x07,0x01,0x04,0x04,0x02,0x04,0x04,0x02,0x04,
			0x04,0x04,0x07,0xF4,0x0C,0x05,0xE4,0x78,0x05,0x24,0xE0,0x05,0x24,0x80,0x45,0x24,
			0x80,0x39,0x24,0x80,0x15,0x24,0x80,0x05,0x24,0x80,0x05,0x25,0xFC,0x05,0x24,0x84,
			0x05,0xE4,0x04,0x05,0xE4,0x04,0x04,0x05,0x04,0x08,0x06,0x04,0x08,0x0C,0x04,0x00,
			0x04,0x3C,0x00,0x00,0x04,0x00,0x00,0x00,/*6*/ 

	/* ���һ�б�����0xFF,0xFF�����������ֿ����������־ */
0xFF,0xFF

};
unsigned char const Hz32[] = {
	
0xCA, 0xB5,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x04,0x01,
			0x00,0xF0,0x04,0x02,0x03,0xF0,0x04,0x02,0x01,0x00,0x04,0x02,0x01,0x01,0x04,0x04,
			0x01,0x01,0x84,0x04,0x01,0x20,0xE4,0x04,0x01,0x30,0x74,0x08,0x01,0x18,0x04,0x08,
			0x01,0x1E,0x04,0x10,0x01,0x0E,0x04,0x30,0x21,0x00,0x04,0x60,0x39,0x00,0x05,0xC0,
			0x1F,0x00,0x1F,0x80,0x0D,0x3F,0xFE,0x80,0x01,0x3F,0xC4,0x80,0x01,0x10,0x04,0xC0,
			0x01,0x00,0x04,0x40,0x01,0x00,0x04,0x60,0x01,0x00,0x04,0x20,0x01,0x00,0x04,0x30,
			0x01,0x00,0x04,0x38,0x01,0x20,0x08,0x1C,0x01,0xC0,0x18,0x0E,0x03,0xC0,0x18,0x00,
			0x01,0x80,0x08,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*0*/
0xCA, 0xB1,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xF0,
			0x03,0xFF,0xFF,0xE0,0x01,0x01,0x00,0x80,0x01,0x01,0x00,0x80,0x01,0x01,0x00,0x80,
			0x01,0x01,0x00,0x80,0x01,0x01,0x00,0x80,0x03,0xFF,0xFF,0xE0,0x03,0xFF,0xFF,0xC0,
			0x01,0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x22,0x00,0x00,
			0x00,0x21,0x80,0x00,0x00,0x20,0xF0,0x00,0x00,0x20,0x70,0x00,0x00,0x20,0x00,0x08,
			0x00,0x20,0x00,0x08,0x00,0x20,0x00,0x0C,0x00,0x20,0x00,0x0E,0x3F,0xFF,0xFF,0xFC,
			0x1F,0xFF,0xFF,0xF8,0x10,0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x60,0x00,0x00,
			0x00,0x60,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*1*/
0xCE, 0xC2,	0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x10,0x02,0x00,0x00,0x0C,0x02,0x18,
			0x10,0x0F,0x07,0xFC,0x0C,0x06,0x0F,0xFC,0x07,0x00,0x78,0x04,0x07,0x01,0xC0,0x04,
			0x00,0x0E,0x00,0x04,0x00,0x30,0x00,0x04,0x00,0xC0,0x3F,0xFC,0x00,0x00,0x1F,0xFC,
			0x1F,0xFF,0x90,0x04,0x0F,0xFF,0x90,0x04,0x04,0x42,0x10,0x04,0x04,0x42,0x1F,0xFC,
			0x04,0x42,0x1F,0xFC,0x04,0x42,0x10,0x04,0x04,0x42,0x10,0x04,0x04,0x42,0x10,0x04,
			0x04,0x42,0x1F,0xFC,0x04,0x42,0x1F,0xFC,0x04,0x42,0x10,0x04,0x08,0x42,0x10,0x04,
			0x1F,0xFF,0x90,0x04,0x08,0x00,0x10,0x04,0x00,0x00,0x3F,0xFC,0x00,0x00,0x10,0x04,
			0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,/*2*/
0xB6, 0xC8,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x70,
			0x00,0x00,0x0F,0xC0,0x07,0xFF,0xFE,0x00,0x03,0xFF,0x80,0x02,0x02,0x08,0x00,0x02,
			0x02,0x08,0x00,0x02,0x02,0x08,0x00,0x02,0x02,0x08,0x10,0x04,0x02,0x08,0x10,0x04,
			0x03,0x08,0x10,0x04,0x02,0xFF,0xD8,0x08,0x02,0xC8,0x96,0x08,0x22,0x08,0x93,0x10,
			0x3A,0x08,0x91,0xB0,0x1E,0x08,0x90,0xE0,0x02,0x08,0x90,0x60,0x02,0x08,0x90,0xF0,
			0x03,0x08,0x91,0x90,0x02,0xE8,0x97,0x18,0x02,0xFF,0x9E,0x18,0x02,0x08,0x3C,0x0C,
			0x02,0x08,0x18,0x0C,0x02,0x08,0x00,0x0C,0x02,0x10,0x00,0x0E,0x06,0x30,0x00,0x06,
			0x06,0x10,0x00,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*3*/
0xC6, 0xC1,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x30,
			0x00,0x00,0x01,0xC0,0x00,0x00,0xFF,0x00,0x1F,0xFF,0xF0,0x00,0x08,0x80,0x00,0x00,
			0x08,0x80,0x04,0x02,0x08,0x81,0x04,0x04,0x08,0x81,0x04,0x04,0x08,0x81,0x04,0x08,
			0x08,0x81,0x04,0x30,0x08,0xB1,0x05,0xE0,0x08,0x99,0xFF,0xC0,0x08,0x8F,0x04,0x00,
			0x08,0x81,0x04,0x00,0x08,0x81,0x04,0x00,0x08,0x81,0x04,0x00,0x08,0x81,0x04,0x00,
			0x08,0x83,0x04,0x00,0x08,0x9D,0xFF,0xFC,0x08,0xB9,0xFF,0xFC,0x08,0x91,0x04,0x00,
			0x08,0x81,0x04,0x00,0x08,0x83,0x04,0x00,0x1F,0xC3,0x04,0x00,0x09,0xC1,0x0C,0x00,
			0x00,0x00,0x0C,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*4*/
0xC4, 0xBB,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x04,0x00,0x21,0x00,
			0x04,0x00,0x21,0x00,0x04,0x00,0x22,0x00,0x04,0x00,0x26,0x00,0x04,0x00,0x24,0x00,
			0x04,0xFF,0x2B,0xFC,0x04,0x92,0x3B,0xF8,0x04,0x92,0x32,0x00,0x05,0x92,0x62,0x00,
			0x3E,0x92,0xE2,0x00,0x34,0x93,0xC2,0x00,0x04,0x93,0xA2,0x00,0x04,0x92,0x3F,0xFE,
			0x04,0x92,0x2F,0xFE,0x04,0x92,0x22,0x00,0x04,0x92,0x22,0x10,0x3F,0x92,0x22,0x00,
			0x34,0x92,0x22,0x08,0x04,0x92,0x32,0x0C,0x04,0xFF,0x2F,0xFC,0x05,0xFF,0x2E,0x00,
			0x04,0x80,0x24,0x00,0x04,0x00,0x26,0x00,0x04,0x00,0x22,0x00,0x08,0x00,0x43,0x00,
			0x08,0x00,0xC3,0x00,0x00,0x00,0x42,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,/*5*/
0xC1, 0xC1,	0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x18,0x00,0x02,0x00,0x78,0x02,
			0x02,0x00,0xE0,0x02,0x02,0x00,0x40,0x04,0x02,0x00,0x40,0x04,0x02,0x00,0x40,0x0C,
			0x02,0xFE,0x40,0x18,0x02,0x7E,0x40,0x30,0x02,0x42,0x4F,0xE0,0x02,0x42,0x47,0x80,
			0x02,0x42,0x44,0x00,0x02,0x42,0x44,0x00,0x32,0x42,0x44,0x00,0x1E,0x42,0x44,0x00,
			0x1E,0x42,0x44,0x00,0x02,0x42,0x44,0x00,0x02,0x42,0x44,0x00,0x02,0x42,0x4F,0xF8,
			0x02,0x42,0x44,0x1C,0x02,0x42,0x40,0x0C,0x02,0xFE,0x40,0x0C,0x02,0x40,0x40,0x0C,
			0x02,0x00,0x40,0x0C,0x02,0x00,0x58,0x0C,0x06,0x00,0x70,0x0C,0x06,0x00,0xE0,0x0C,
			0x02,0x00,0x40,0x7C,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*6*/
	/* ���һ�б�����0xFF,0xFF�����������ֿ����������־ */
0xFF,0xFF
};

#else
	const unsigned char Hz12[] = {0xFF, 0xFF};
	const unsigned char Hz16[] = {0xFF, 0xFF};
	const unsigned char Hz24[] = {0xFF, 0xFF};
#endif


























