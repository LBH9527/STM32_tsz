/*
*********************************************************************************************************
*
*	ģ������ : SPI��������
*	�ļ����� : bsp_spi_bus.h
*	��    �� : V1.2
*	˵    �� : SPI���ߵײ��������ṩSPI���á��շ����ݡ����豸����SPI֧�֡�
*	�޸ļ�¼ :
*		�汾��  ����        ����    ˵��
*       v1.0    2014-10-24 armfly  �װ档������FLASH��TSC2046��VS1053��AD7705��ADS1256��SPI�豸������
*									���շ����ݵĺ������л��ܷ��ࡣ�������ͬ�ٶȵ��豸��Ĺ������⡣
*		V1.1	2015-02-25 armfly  Ӳ��SPIʱ��û�п���GPIOBʱ�ӣ��ѽ����
*		V1.2	2015-07-23 armfly  �޸� bsp_SPI_Init() ���������ӿ���SPIʱ�ӵ���䡣�淶Ӳ��SPI�����SPI�ĺ궨��.
*
*
*********************************************************************************************************
*/
#include "bsp.h"
/*
	SPI������ģʽ���õĽ϶����ģʽ0��ģʽ3����ͬ��Ϊ���������ز���������ʱSCK�ĵ�ƽ��һ����
	Ҫ���������Ĳ�ͬ��ѡ��SPI�Ĺ���ģʽ��
	1.W25Q128����mode3.
	����SPI1��ʱ��Դ��84M, SPI3��ʱ��Դ��42M��Ϊ�˻�ø�����ٶȣ������ѡ��SPI1��
*/
#ifdef SOFT_SPI		/* ���SPI */
	/* ����GPIO�˿� */
	#define RCC_SCK 	RCC_AHB1Periph_GPIOB
	#define PORT_SCK	GPIOB
	#define PIN_SCK		GPIO_Pin_3

	#define RCC_MOSI 	RCC_AHB1Periph_GPIOB
	#define PORT_MOSI	GPIOB
	#define PIN_MOSI	GPIO_Pin_5

	#define RCC_MISO 	RCC_AHB1Periph_GPIOB
	#define PORT_MISO	GPIOB
	#define PIN_MISO	GPIO_Pin_4 
	
	#define SCK_0()		PORT_SCK->BSRRH = PIN_SCK
	#define SCK_1()		PORT_SCK->BSRRL = PIN_SCK

	#define MOSI_0()	PORT_MOSI->BSRRH = PIN_MOSI
	#define MOSI_1()	PORT_MOSI->BSRRL = PIN_MOSI

	#define MISO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_MISO, PIN_MISO) == Bit_SET)

#endif

#ifdef HARD_SPI
	#define SPI_HARD	SPI1
	#define RCC_SPI		RCC_APB2Periph_SPI1
	
	/* SPI or I2S mode selection masks */
	#define SPI_Mode_Select      ((uint16_t)0xF7FF)
	#define I2S_Mode_Select      ((uint16_t)0x0800) 
	
	/* SPI registers Masks */
	#define CR1_CLEAR_Mask       ((uint16_t)0x3040)
	#define I2SCFGR_CLEAR_Mask   ((uint16_t)0xF040)

	/* SPI SPE mask */
	#define CR1_SPE_Set          ((uint16_t)0x0040)
	#define CR1_SPE_Reset        ((uint16_t)0xFFBF)
#endif

uint8_t g_spi_busy = 0;		/* SPI ���߹����־ */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSPIBus
*	����˵��: ����SPI���ߡ� ֻ���� SCK�� MOSI�� MISO���ߵ����á�������ƬѡCS��Ҳ����������оƬ���е�INT��BUSY��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{
#ifdef SOFT_SPI		/* ���SPI */
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO | RCC_CS, ENABLE);

	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
	GPIO_Init(PORT_MOSI, &GPIO_InitStructure);

	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = PIN_MISO;
	GPIO_Init(PORT_MISO, &GPIO_InitStructure);

#endif

#ifdef HARD_SPI
	/* Ӳ��SPI */
	GPIO_InitTypeDef  GPIO_InitStructure;
	 SPI_InitTypeDef  SPI_InitStructure;
	/* ����GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	/* ��SPIʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* ���� SCK, MISO �� MOSI Ϊ���ù��� */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;    
    SPI_Init(SPI1,&SPI_InitStructure);

	SPI_Cmd(SPI_HARD, DISABLE);			/* �Ƚ�ֹSPI  */

	SPI_Cmd(SPI_HARD, ENABLE);			/* ʹ��SPI  */

#endif	
}

//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
#ifdef HARD_SPI		//Ӳ��SPI
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 
#endif
/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiDelay
*	����˵��: ʱ���ӳ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#ifdef SOFT_SPI		//���SPI	
void bsp_spiDelay(void)
{
#if 1
	uint32_t i;

	/*
		�ӳ�5ʱ�� F407 (168MHz��Ƶ�� GPIOģ�⣬ʵ�� SCK ���� = 480ns (��Լ2M)
	*/
	for (i = 0; i < 5; i++);
#else
	/*
		������ӳ���䣬 F407 (168MHz��Ƶ�� GPIOģ�⣬ʵ�� SCK ���� = 200ns (��Լ5M)
	*/
#endif
}
#endif

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusEnter
*	����˵��: ռ��SPI����
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
void bsp_SpiBusEnter(void)
{
	g_spi_busy = 1;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusExit
*	����˵��: �ͷ�ռ�õ�SPI����
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
void bsp_SpiBusExit(void)
{
	g_spi_busy = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusBusy
*	����˵��: �ж�SPI����æ�������Ǽ������SPIоƬ��Ƭѡ�ź��Ƿ�Ϊ1
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
uint8_t bsp_SpiBusBusy(void)
{
	return g_spi_busy;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_SetSpiSck
*	����˵��: ���շ����ֽ�
*	��    ��: ���͵�����
*	�� �� ֵ: ���յ�������
*********************************************************************************************************
*/
u8 bsp_SPI1ReadWriteByte(u8 TxData)
{
#ifdef SOFT_SPI		//���SPI	
	u8 i;
	
	SPI1_SCK = 1;
	for (i=0; i<8; i++)
	{
		SPI1_SCK = 0;
		if (TxData & 0x80)	//д��ǰ���λ�� MOSI  write
			SPI1_MOSI = 1;	
		else
			SPI1_MOSI = 0;
		TxData <<= 1;		//����һλ�� MSB
		bsp_spiDelay();
		SPI1_SCK = 1;
		if (SPI1_MISO)		//��MISO�ϲ�׽��ǰλ  read
			TxData |= 0x01;
		else
			TxData &= 0xfe;
		bsp_spiDelay();
	}
	return TxData;  
	
#else // HARD_SPI		//Ӳ��SPI	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}     //�ȴ�������Ϊ��
		SPI_I2S_SendData(SPI1, TxData);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}    //�ȴ�������һ��byte 
    return SPI_I2S_ReceiveData(SPI1);                                   //����ͨ��SPIx������յ�����	
 #endif       
}

