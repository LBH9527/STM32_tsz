/*
*********************************************************************************************************
*
*	ģ������ : W25Q128оƬ����ģ��
*	�ļ����� : bsp_W25Qxx_Flash.c
*	��    �� : V1.1
*	˵    �� : ʵ�ֶ��ⲿ FLASH�� W25Q128���Ķ�д��
*	�޸ļ�¼ :
*		�汾��  ����       ����    	˵��
*		V1.0   			  9527		��W25Q128��Sector����
*
*********************************************************************************************************
*/

#include "bsp.h"
	/*
		4KbytesΪһ��Sector��16������Ϊ1��Block��1��Block = 64K�ֽ�
		W25Q128����Ϊ16M�ֽ�,����256��Block,4096��Sector .��С������λΪһ��Sector��	
	*/

	/*
		����Flash�ͺ�Ϊ W25Q128BV: 128M-bit/16M-byte (104MHz)
		STM32Ӳ��SPI�ӿ� = SPI3 ���� SPI1

		����SPI1��ʱ��Դ��84M, SPI3��ʱ��Դ��42M��Ϊ�˻�ø�����ٶȣ������ѡ��SPI1��
	*/
	/*
		��SPIʱ�������2��Ƶ����֧�ֲ���Ƶ��
		�����SPI1��2��ƵʱSCKʱ�� = 42M��4��ƵʱSCKʱ�� = 21M
		�����SPI3, 2��ƵʱSCKʱ�� = 21M

		������߹�������SPIʱ�ӱ��뽵�Ͳ��С� 8��Ƶ��Ӧ 10.5M���ٶȡ�
	*/
/* CS ��Ӧ��GPIOʱ�� */
#define	RCC_W25		(RCC_AHB1Periph_GPIOB)
/* ƬѡGPIO�˿�  */
#define W25_CS_GPIO			GPIOB
#define W25_CS_PIN			GPIO_Pin_14
/* Ƭѡ�����õ�ѡ��  */
#define W25_CS_0()      W25_CS_GPIO->BSRRH = W25_CS_PIN	//����͵�ƽ GPIO_ResetBits(PORT_CS, PIN_CS)
/* Ƭѡ�����ø߲�ѡ�� */
#define W25_CS_1()     W25_CS_GPIO->BSRRL = W25_CS_PIN	//����Ϊ�ߵ�ƽ GPIO_SetBits(PORT_CS, PIN_CS)
//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 	/* ��ֹд */
#define W25X_ReadStatusReg		0x05 	/* ��״̬�Ĵ������� */
#define W25X_WriteStatusReg		0x01 	/* д״̬�Ĵ������� */
#define W25X_ReadData			0x03 	/* ������������ */
#define W25X_FastReadData		0x0B 	/* ���ٶ�����������*/
#define W25X_FastReadDual		0x3B 	/* ���ٶ�ȡ˫��� */
#define W25X_PageProgram		0x02 	/* ҳ����*/
#define W25X_BlockErase			0xD8 	/* 64KB�����*/
#define W25X_SectorErase		0x20 	/* ������������ */
#define W25X_ChipErase			0xC7 	/* ������������ */
#define W25X_PowerDown			0xB9 	/* �͹��� */
#define W25X_ReleasePowerDown	0xAB 	/* ȡ���͹��� */
#define W25X_DeviceID			0xAB 	/* ��ȡ�豸ID */
#define W25X_ManufactDeviceID	0x90 	/* ��������ID */
#define W25X_JedecDeviceID		0x9F 	/* ��JEDEC ID */

void W25QXX_Wait_Busy(void);   
#define WIP_Flag                0x01 	/* Write In Progress (WIP) flag */
#define Dummy_Byte              0xFF	/* ���������Ϊ����ֵ�����ڶ����� */

W25_S s_W25;

void W25_SetCS(uint8_t state);
/*
*********************************************************************************************************
*	�� �� ��: w25_ConfigGPIO
*	����˵��: ��ʼ������FlashӲ���ӿ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitW25QxxFlash(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

	//GPIOB14
	GPIO_InitStructure.GPIO_Pin = W25_CS_PIN;			//PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(W25_CS_GPIO, &GPIO_InitStructure);		//��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			//PG7
	GPIO_Init(GPIOG, &GPIO_InitStructure);				//��ʼ��
	GPIO_SetBits(GPIOG,GPIO_Pin_7);						//PG7���1,��ֹNRF����SPI FLASH��ͨ��
	
	W25_SetCS(1);										//SPI FLASH��ѡ��
	bsp_InitSPIBus();		   							//��ʼ��SPI
	SPI1_SetSpeed(SPI_SPEED_21M);						//����Ϊ42Mʱ��,����ģʽ 
	s_W25.ChipID = W25QXX_ReadID();						//��ȡFLASH ID.
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CfgSPIForW25
*	����˵��: ����STM32�ڲ�SPIӲ���Ĺ���ģʽ���ٶȵȲ��������ڷ���SPI�ӿڵĴ���Flash��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_CfgSPIForW25(void)
{
	bsp_InitSPIBus();
}

/*
*********************************************************************************************************
*	�� �� ��: w25_SetCS
*	����˵��: ����CS�� ����������SPI����
*	��    ��: ��
	�� �� ֵ: ��
*********************************************************************************************************
*/
void W25_SetCS(uint8_t state)
{
	if (state == 0)
	{
		bsp_SpiBusEnter();	/* ռ��SPI���ߣ� �������߹��� */
		W25_CS_0(); 
	}
	else
	{
		W25_CS_1();
		bsp_SpiBusExit();	/* �ͷ�SPI���ߣ� �������߹��� */
	}
}
/*******************************************************************************
* ������  ;  W25QXX_ReadSR
* ����    �� ��ȡW25QXX��״̬�Ĵ��� 
* ��������� 
* ���ز����� 
*******************************************************************************/
u8 W25QXX_ReadSR(void)   
{
    u8 byte = 0;

    W25_SetCS(0);         //ʹ��Ƭѡ  
    bsp_SPI1ReadWriteByte(W25X_ReadStatusReg);//���Ͷ�ȡ״̬�Ĵ�������    
    byte = bsp_SPI1ReadWriteByte(0xff);         //��ȡһ���ֽ�  
    W25_SetCS(1);         //ȡ��Ƭѡ     
    return byte;    
}

/*******************************************************************************
* ������  ; W25QXX_Write_SR(u8 sr)   
* ����    �� дW25QXX״̬�Ĵ��� ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
* ��������� 
* ���ز����� 
*******************************************************************************/
void W25QXX_Write_SR(u8 sr)
{
    W25_SetCS(0);                 //ʹ������   
	bsp_SPI1ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	bsp_SPI1ReadWriteByte(sr);               //д��һ���ֽ�  
	W25_SetCS(1);                           //ȡ��Ƭѡ     	       
}

/*
*********************************************************************************************************
*	�� �� ��: w25_WaitForWriteEnd
*	����˵��: ����ѭ����ѯ�ķ�ʽ�ȴ������ڲ�д�������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void W25QXX_WaitForWriteEnd(void) 
{    
	W25_CS_0();
	while ((W25QXX_ReadSR() & WIP_Flag) == 0x01); // �ȴ�BUSYλ���
	/*���Լ��ϳ�ʱ����*/
	W25_SetCS(1); 
}
	
/*******************************************************************************
* ������  ; W25QXX_PowerDown
* ����    �� �������ģʽ
* ��������� 
* ���ز����� 
*******************************************************************************/
void W25QXX_PowerDown(void)   
{ 
  	W25_SetCS(0);                           //ʹ������   
    bsp_SPI1ReadWriteByte(W25X_PowerDown);        //���͵�������  
	W25_SetCS(1);                           //ȡ��Ƭѡ     	      
}  

/*******************************************************************************
* ������  ; W25QXX_WAKEUP
* ����    �� ����
* ��������� 
* ���ز����� 
*******************************************************************************/
void W25QXX_WAKEUP(void)  
{
  	W25_SetCS(0);                           //ʹ������   
    bsp_SPI1ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	W25_SetCS(1);                           //ȡ��Ƭѡ     	      
}

/*******************************************************************************
* ������  ; void W25QXX_Write_Enable(void) 
* ����    �� ��WEL��λ  
* ��������� 
* ���ز����� 
*******************************************************************************/
void W25QXX_Write_Enable(void)   
{
	W25_SetCS(0);                           //ʹ������   
    bsp_SPI1ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	W25_SetCS(1);                           //ȡ��Ƭѡ     	      
} 

/*******************************************************************************
* ������  ; W25QXX_Write_Disable(void)	
* ����    �� W25QXXд��ֹ
* ��������� 
* ���ز����� 
*******************************************************************************/
void W25QXX_Write_Disable(void)   
{  
    W25_SetCS(0);                           //ʹ������   
    bsp_SPI1ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��
    W25_SetCS(1);                           //ȡ��Ƭѡ  
}   

/*******************************************************************************
* ������  ; W25QXX_ReadID(void)
* ����    �� ��ȡоƬID
* ��������� 
* ���ز�����0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
            0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
            0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
            0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
            0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
*******************************************************************************/
uint32_t W25QXX_ReadID(void)
{
    uint32_t temp = 0;

    W25_SetCS(0);
    bsp_SPI1ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	bsp_SPI1ReadWriteByte(0x00); 	    
	bsp_SPI1ReadWriteByte(0x00); 	    
	bsp_SPI1ReadWriteByte(0x00);
	temp |= bsp_SPI1ReadWriteByte(0xff)<<8;
	temp |= bsp_SPI1ReadWriteByte(0xff);
	W25_SetCS(1);
	return temp;
}

/*******************************************************************************
* ������  ; W25QXX_Read()
* ����    �� ��ָ����ַ��ʼ��ȡָ�����ȵ�����
* ��������� pBuffer:���ݴ洢�� ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit) NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
* ���ز����� 
*******************************************************************************/
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{
    u16 i;

    W25_SetCS(0);
    bsp_SPI1ReadWriteByte(W25X_ReadData);  //���Ͷ�ȡ����  
    bsp_SPI1ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    bsp_SPI1ReadWriteByte((u8)((ReadAddr)>>8));   
    bsp_SPI1ReadWriteByte((u8)ReadAddr);
    for(i=0; i<NumByteToRead;i++)
    {
        pBuffer[i] = bsp_SPI1ReadWriteByte(0xff);   //ѭ������  
    }
    W25_SetCS(1);
}

/*******************************************************************************
* ������  ; void W25QXX_Erase_Sector(u32 Dst_Addr)  
* ����    �� ����һ������.����һ��ɽ��������ʱ��:150ms
* ��������� SectorAddr:������ַ ����ʵ����������
* ���ز����� 
*******************************************************************************/
void W25QXX_Erase_Sector(u32 SectorAddr)  
{
    //����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	 
 	SectorAddr*=4096;
 	W25QXX_Write_Enable();                  //SET WEL
 	W25QXX_WaitForWriteEnd();   
 	W25_SetCS(0);                           //ʹ������   
    bsp_SPI1ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
	/* Send SectorAddr high nibble address byte */
	bsp_SPI1ReadWriteByte((SectorAddr & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	bsp_SPI1ReadWriteByte((SectorAddr & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	bsp_SPI1ReadWriteByte(SectorAddr & 0xFF);
	W25_SetCS(1);                           //ȡ��Ƭѡ     	      
    W25QXX_WaitForWriteEnd();   				   //�ȴ��������
}
/*******************************************************************************
* ������  ; W25QXX_Erase_Chip
* ����    �� ��������оƬ	�ȴ�ʱ�䳬��...
* ��������� 
* ���ز����� 
*******************************************************************************/
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_WaitForWriteEnd();   
  	W25_SetCS(0);                           //ʹ������   
    bsp_SPI1ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	W25_SetCS(1);                           //ȡ��Ƭѡ     	      
	W25QXX_WaitForWriteEnd();   				   //�ȴ�оƬ��������
}  

/*******************************************************************************
* ������  ; W25QXX_Write_Page
* ����    �� ��FLASH��ҳд�����ݣ�һҳ���256���ֽڣ����ñ�����д������ǰ��Ҫ�Ȳ�������.
* ��������� //pBuffer:���ݴ洢��  //WriteAddr:��ʼд��ĵ�ַ(24bit) //NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	
* ���ز����� 
*******************************************************************************/
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u16 i;  
    
    W25QXX_Write_Enable();                  //SET WEL 
	W25_SetCS(0);                           //ʹ������   
    bsp_SPI1ReadWriteByte(W25X_PageProgram);      //����дҳ����   
    bsp_SPI1ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    bsp_SPI1ReadWriteByte((u8)((WriteAddr)>>8));   
    bsp_SPI1ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)bsp_SPI1ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25_SetCS(1);                           //ȡ��Ƭѡ 
	W25QXX_WaitForWriteEnd();					   //�ȴ�д�����
}


/*******************************************************************************
* ������  ; W25QXX_Write_NoCheck
* ����    �� �޼���дSPI FLASH ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
* ��������� pBuffer:���ݴ洢��  WriteAddr:��ʼд��ĵ�ַ(24bit)  NumByteToWrite:Ҫд����ֽ���(���65535)
* ���ز����� 
*******************************************************************************/
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
    u16 pageremain;	
    
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{
        W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)
            break;//д�������
        else//NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	

			NumByteToWrite -= pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite > 256)
			    pageremain = 256;                   //һ�ο���д��256���ֽ�
			else 
			    pageremain = NumByteToWrite; 	   //����256���ֽ���
		}        
	}
}

u8 W25QXX_BUFFER[4096];	
/*******************************************************************************
* ������  ; W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite) 
* ����    �� ��ָ����ַ��ʼд��ָ�����ȵ�����
* ��������� pBuffer:���ݴ洢��  WriteAddr:��ʼд��ĵ�ַ(24bit) NumByteToWrite:Ҫд����ֽ���(���65535)  
* ���ز����� 
*******************************************************************************/
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite) 
{
#if 1 //���Գ�����д��4096���ֽڻ�������1024���ֽ�д����ȷ
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	}
#else	//���ñ�����д������ǰ��Ҫ�Ȳ�������
	  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % 256;
  count = 256 - Addr;
  NumOfPage =  NumByteToWrite / 256;
  NumOfSingle = NumByteToWrite % 256;

  if (Addr == 0) /* ����ַ�� SPI_FLASH_PageSize ����  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      W25QXX_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, 256);
        WriteAddr +=  256;
        pBuffer += 256;
      }

      W25QXX_Write_Page(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* ����ַ�� SPI_FLASH_PageSize ������  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        W25QXX_Write_Page(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        W25QXX_Write_Page(pBuffer, WriteAddr, temp);
      }
      else
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / 256;
      NumOfSingle = NumByteToWrite % 256;

      W25QXX_Write_Page(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, 256);
        WriteAddr +=  256;
        pBuffer += 256;
      }

      if (NumOfSingle != 0)
      {
        W25QXX_Write_Page(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
#endif
}


//����Ϊ���Գ���W25Q128
/*ȫ������ʱ�� 27571ms*/
#if 1
#define EXT_FLASH_SIZE  16*1024*1024    //FLASH ��СΪ16M�ֽ�
#define TEST_ADDR		0			/* ��д���Ե�ַ */
#define TEST_SIZE		2048		/* ��д�������ݴ�С */
/*
*********************************************************************************************************
*	�� �� ��: sfReadTest
*	����˵��: ������Flash����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void W25QXXReadTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* ��ʼ��ַ = 0�� ���ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	W25QXX_Read(buf, TEST_ADDR, TEST_SIZE);
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */
	printf("������Flash�ɹ����������£�\r\n");

	/* ��ӡ���� */
	for (i = 0; i < TEST_SIZE; i++)
	{
		printf(" %02X", buf[i]);

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
	printf("���ݳ���: %d�ֽ�, ����ʱ: %dms, ���ٶ�: %d Bytes/s\r\n", TEST_SIZE, iTime2 - iTime1, (TEST_SIZE * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	�� �� ��: sfTestReadSpeed
*	����˵��: ���Դ���Flash���ٶȡ���ȡ��������Flash�����ݣ�����ӡ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void W25QXXTestReadSpeed(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];
	uint32_t uiAddr;

	/* ��ʼ��ַ = 0�� ���ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	uiAddr = 0;
	for (i = 0; i < EXT_FLASH_SIZE / TEST_SIZE; i++, uiAddr += TEST_SIZE)
	{
		W25QXX_Read(buf, uiAddr, TEST_SIZE);
	}
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	printf("���ݳ���: %d�ֽ�, ����ʱ: %dms\r\n", EXT_FLASH_SIZE, iTime2 - iTime1);
}

/*
*********************************************************************************************************
*	�� �� ��: sfWriteTest
*	����˵��: д����Flash����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void W25QXXWriteTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* �����Ի����� */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = 0x55;
	}

	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
    W25QXX_Write(buf,TEST_ADDR,TEST_SIZE);
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */
	printf("д����Flash�ɹ���\r\n");

	/* ��ӡ���ٶ� */
	printf("���ݳ���: %d�ֽ�, д��ʱ: %dms\r\n", TEST_SIZE, iTime2 - iTime1);
}

/*
*********************************************************************************************************
*	�� �� ��: sfWriteAll
*	����˵��: д����EEPROMȫ������
*	��    �Σ�_ch : д�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void W25QXXWriteAll(uint8_t _ch)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[4 * 1024];

	/* �����Ի����� */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = _ch;
	}

	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	for (i = 0; i < EXT_FLASH_SIZE/ 4096; i++)
	{
		 W25QXX_Write(buf, i * 4096, 4096) ;	
	}
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	printf("���ݳ���: %dK�ֽ�, д��ʱ: %dms\r\n", EXT_FLASH_SIZE / 1024, iTime2 - iTime1);
}

/*
*********************************************************************************************************
*	�� �� ��: sfErase
*	����˵��: ��������Flash
*	��    �Σ���
*	�� �� ֵ:  
*********************************************************************************************************
*/
 void W25QXXErase(void)
{
	int32_t iTime1, iTime2;

	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	W25QXX_Erase_Chip();
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	printf("��������Flash��ɣ�, ��ʱ: %dms\r\n", iTime2 - iTime1);
	return;
}

#endif

























