/*
*********************************************************************************************************
*
*	模块名称 : 字符串操作\数值转换
*	文件名称 : bsp_user_lib.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/
#include <stdint.h>
#ifndef __BSP_USERLIB_H
#define __BSP_USERLIB_H

//位带操作。IO操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO口操作，只对单一的IO口
//确保n小于16
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //?? 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //?? 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //?? 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //?? 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //?? 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //?? 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //?? 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //?? 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //?? 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //??

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //?? 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //??

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //?? 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //??

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //?? 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //??

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //?? 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //??


#define     _HIGHBYTE(c)			(INT8U)((c) >> 8)
#define     _LOWBYTE(c)				(INT8U)((c))
#define     _HIGHWORD(c)			(INT16U)((c) >> 16)	
#define     _LOWWORD(c)				(INT16U)((c))	
#define 	   _BYTE2BCD(c)			((((c)/10)<<4) + (c % 10))
#define     _BCD2BYTE(c)			(((c) >> 4)*10 + (c & 0x0f))

#define      mGetMax(a,b)			(((a)>(b))?(a):(b))
#define      mGetMin(a,b)			(((a)<(b))?(a):(b))

#define     mMoreThan(a,b)			((a)>(b))
#define	    mLessThan(a,b)			((a)<(b))

#define 	mMemBitSet(c,b)			(c) |= (0x01 << (b))
#define 	mMemBitClr(c,b)			(c) &= ~(0x01 << (b))
#define     mMemBitGet(c,b)			((c & (0x01 << (b)))>0)
#define     mMemBitInv(c,b)			((c) ^= (0x01 << (b)))
#define     mMemWrite (c,a)			(c)= (a)

typedef __packed union 
{
    uint8_t  B08[2];
    uint16_t  B16;
} B16_B08 ;
typedef __packed union
{
    uint8_t  B08[4];
    uint32_t B32;
} B32_B08;
typedef __packed union
{
    uint16_t  B16[2];
    uint32_t B32;
} B32_B16;

/* 提供给其他C文件调用的函数 */
int str_len(char *_str);
void str_cpy(char *_tar, char *_src);
int str_cmp(char * s1, char * s2);
void _mem_set(char *_tar, char _data, int _len);
void int_to_str(int _iNumber, char *_pBuf, unsigned char _len);
int str_to_int(char *_pStr);

uint16_t BEBufToUint16(uint8_t *_pBuf);
uint16_t LEBufToUint16(uint8_t *_pBuf);

uint32_t BEBufToUint32(uint8_t *_pBuf);
uint32_t LEBufToUint32(uint8_t *_pBuf);

uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);

//int32_t  CaculTwoPoint(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x);

#endif
