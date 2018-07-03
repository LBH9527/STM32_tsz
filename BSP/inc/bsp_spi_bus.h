/*
*********************************************************************************************************
*
*	ģ������ : SPI��������
*	�ļ����� : bsp_spi_bus.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#ifndef __BSP_SPI_BUS_H_
#define __BSP_SPI_BUS_H_

//#define SOFT_SPI		/* ������б�ʾʹ��GPIOģ��SPI�ӿ� */
#define HARD_SPI		/* ������б�ʾʹ��CPU��Ӳ��SPI�ӿ� */

/*
	��SPIʱ�������2��Ƶ����֧�ֲ���Ƶ��
	�����SPI1��2��ƵʱSCKʱ�� = 42M��4��ƵʱSCKʱ�� = 21M
	�����SPI3, 2��ƵʱSCKʱ�� = 21M
*/
#define SPI_SPEED_42M		SPI_BaudRatePrescaler_2
#define SPI_SPEED_21M		SPI_BaudRatePrescaler_4
#define SPI_SPEED_5_2M		SPI_BaudRatePrescaler_8
#define SPI_SPEED_2_6M		SPI_BaudRatePrescaler_16
#define SPI_SPEED_1_3M		SPI_BaudRatePrescaler_32
#define SPI_SPEED_0_6M		SPI_BaudRatePrescaler_64

void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
void bsp_InitSPIBus(void);
void bsp_SpiBusExit(void);
void bsp_SpiBusEnter(void);
u8 bsp_SPI1ReadWriteByte(u8 TxData);
#endif
