/*
*********************************************************************************************************
*
*	ģ������ : �ⲿSRAM����ģ��
*	�ļ����� : bsp_wv51216_sram.c
*	��    �� : V1.0
*	˵    �� : ����FSMC�������ⲿSRAM IS62WV51216��16 λ�� 512K �� 512*16���� 1M �ֽڣ�������FSMC BANK1������3
*				��ʼ��ַ0x68000000
*
*********************************************************************************************************
*/
#include "bsp.h"
//ʹ��NOR/SRAM�� Bank1.sector3,��ַλHADDR[27,26]=10 
//��IS61LV25616/IS62WV25616,��ַ�߷�ΧΪA0~A17 
//��IS61LV51216/IS62WV51216,��ַ�߷�ΧΪA0~A18
#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	
#define EXT_SRAM_ADDR		((u32)(0x68000000))	

void bsp_InitFSMC_SRAM(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PD,PE,PF,PGʱ��  
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  

	GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(0XFF<<8);//PD0,1,4,5,8~15 AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin = (3<<0)|(0X1FF<<7);//PE0,1,7~15,AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin = (0X3F<<0)|(0XF<<12); 	//PF0~5,12~15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin =(0X3F<<0)| GPIO_Pin_10;//PG0~5,10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 


	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12

	GPIO_PinAFConfig(GPIOF,GPIO_PinSource0,GPIO_AF_FSMC);//PF0,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource1,GPIO_AF_FSMC);//PF1,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource2,GPIO_AF_FSMC);//PF2,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource3,GPIO_AF_FSMC);//PF3,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource4,GPIO_AF_FSMC);//PF4,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource5,GPIO_AF_FSMC);//PF5,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);//PF12,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource13,GPIO_AF_FSMC);//PF13,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource14,GPIO_AF_FSMC);//PF14,AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource15,GPIO_AF_FSMC);//PF15,AF12

	GPIO_PinAFConfig(GPIOG,GPIO_PinSource0,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource1,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource2,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource3,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource4,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource5,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource10,GPIO_AF_FSMC);


	readWriteTiming.FSMC_AddressSetupTime = 0x03;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 1/36M=27ns
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	readWriteTiming.FSMC_DataSetupTime = 0x08;		 ////���ݱ���ʱ�䣨DATAST��Ϊ9��HCLK 6*9=54ns	 	 
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 



	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;//  ��������ʹ��NE3 ��Ҳ�Ͷ�ӦBTCR[4],[5]��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit  
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//�洢��дʹ�� 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;  
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; // ��дʹ����ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming; //��дͬ��ʱ��

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  // ʹ��BANK1����3										  

}

//��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)  
	{										    
		*(vu8*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;	  
		WriteAddr++;
		pBuffer++;
	}   
}																			    
//��ָ����ַ((WriteAddr+Bank1_SRAM3_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)  
	{											    
		*pBuffer++=*(vu8*)(Bank1_SRAM3_ADDR+ReadAddr);    
		ReadAddr++;
	}  
} 
 

/* ���Գ��� */
#if 0

#define EXT_SRAM_SIZE	(1 * 1024 * 1024)
#define BUFFER_SIZE        1024
u8 TxBuffer[BUFFER_SIZE];
u8 RxBuffer[BUFFER_SIZE];
static void Fill_Buffer(u8 *pBuffer, u32 uwBufferLenght, u8 uhOffset)
{
  u32 i = 0;

  /* Put in global buffer different values */
  for (i = 0; i < uwBufferLenght; i++ )
  {
    pBuffer[i] = i + uhOffset;
  }
}

/*
*********************************************************************************************************
*	�� �� ��: WriteTest
*	����˵��: дSRAM
*	��    ��: _Value : д�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void fsmc_sram_WriteTest(void)
{
	u8 i;
	int32_t iTime1, iTime2;
	
	Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x55);
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	for (i=0; i<20; i++)
	{
		FSMC_SRAM_WriteBuffer(TxBuffer, 0, BUFFER_SIZE);	//10M����
	}
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	/* ��ӡ�ٶ� */
	printf("10m����д��ʱ: %dms, д�ٶ�: %dKB/s\r\n", iTime2 - iTime1, (10*1024*1000 / (iTime2 - iTime1)));
}

/*
*********************************************************************************************************
*	�� �� ��: ReadTest
*	����˵��: �����ݣ�����ӡ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void fsmc_sram_ReadTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;


	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */

	FSMC_SRAM_ReadBuffer(RxBuffer,0,BUFFER_SIZE);

	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	printf("�����ַ��0x%08X  ��С: %d�ֽ�,  ��������: \r\n", EXT_SRAM_ADDR , BUFFER_SIZE);

	/* ��ӡ���� */
	for (i = 0; i < BUFFER_SIZE; i++)
	{
		printf(" %02X", RxBuffer[i]);

		if ((i & 31) == 31)
		{
			printf("\r\n");	/* ÿ����ʾ16�ֽ����� */
		}
		else if ((i & 31) == 15)
		{
			printf(" - ");
		}
	}

	/* ��ӡ���ٶ� */
	printf("����ʱ: %dms, ���ٶ�: %dB/s\r\n", iTime2 - iTime1, (BUFFER_SIZE * 1000) / (iTime2 - iTime1));
}
#endif
