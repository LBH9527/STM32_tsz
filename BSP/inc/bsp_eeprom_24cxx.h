/*
*********************************************************************************************************
*
*	ģ������ : ����EEPROM 24xx02����ģ��
*	�ļ����� : bsp_eeprom_24xx.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2012-10-12 armfly  ST�̼���汾 V2.1.0
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_EEPROM_24CXX_H
#define _BSP_EEPROM_24CXX_H

//#define AT24C02
#define AT24C16

#ifdef AT24C02
	#define EE_MODEL_NAME       "AT24C02"
	#define EE_DEV_ADDR         0xA0            /* �豸��ַ */
	#define EE_PAGE_SIZE        8               /* ҳ���С(�ֽ�) */
	#define EE_SIZE             (8*32)          /* ������(�ֽ�) */
	#define EE_TYPE				255				/* ����ַ = ������ - 1 */
#endif

#ifdef AT24C16
	#define EE_MODEL_NAME		"AT24C16"
	#define EE_DEV_ADDR			0xA0			/* �豸��ַ */
	#define EE_PAGE_SIZE		16				/* ҳ���С(�ֽ�) */
	#define EE_SIZE				(16*128)		/* ������(�ֽ�) */
	#define EE_TYPE				2047			/* ����ַ = ������ - 1 */
#endif

/* �ṩ������C�ļ����õĺ��� */
void bsp_InitAT24CXX(void);
void bsp_U16SaveToE2(uint16_t _usAddress, uint16_t *_pWriteBuf, uint16_t _usSize);
void bsp_E2DataReload(uint16_t _usAddress, uint16_t *_pReadBuf, uint16_t _usSize);
#endif
