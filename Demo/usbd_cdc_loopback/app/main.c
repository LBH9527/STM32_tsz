/*
*********************************************************************************************************
*
*	ģ������ : ģ�幤��
*	�ļ����� : main.c
*	��    �� : V1.1
*	˵    �� : 
*           1��The system Clock is configured as follow : 
*            System Clock source            = PLL (HSE)
*            SYSCLK(Hz)                     = 168000000
*            HCLK(Hz)                       = 168000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 4
*            APB2 Prescaler                 = 2
*            HSE Frequency(Hz)              = 25000000
*            PLL_M                          = 8
*            PLL_N                          = 336
*            PLL_P                          = 2
*            PLL_Q                          = 7
*            VDD(V)                         = 3.3
*           2�NNVIC���ȼ�����ΪGroup4��0-15��ռʽ���ȼ���
*               
*	�޸ļ�¼ :
*		�汾��   ����        ����      ˵��

*
*********************************************************************************************************
*/


#include "bsp.h"			/* �ײ�Ӳ������ */
#include "usb_bsp.h"
#include "usbd_cdc_core_loopback.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "main.h"   
#include "am_common.h"

//#include "usb_bsp_msc.h"		/* usb�ײ����� */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;
uint8_t Rxbuffer[64]; 
__IO uint32_t receive_count =1;
extern __IO uint32_t  data_sent;

static void PrintfLogo(void);
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
    uint32_t file_size;
	MSG_S ucMsg;
	FONT_T tFont;			/* ����һ������ṹ���������������������� */
	
	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM
	*/

  	bsp_Init();				/* Ӳ����ʼ�� */
      USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);  
//	LoadParam();			/* ���ز��� ��ȡEEPROM�е�ȫ�ֱ���*/
//	MODBUS_Init();			/* modbus��ʼ�� */
	
	PrintfLogo();			/* ��ӡ������Ϣ������1 */

//    SD_FatfsTest();

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = CL_BLUE;		/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}
	LCD_Fill_Rect(0, 0, g_lcddev.width,40, CL_BLUE);
	LCD_DispStr(10, 10, (uint8_t *)VER_INFO, &tFont);							/* ��ʾ����汾��Ϣ */





//    usbd_OpenMassStorage();	/* ��ʼ��USB Device������ΪMass Storage */
	/* ����������ѭ���� */
	while (1)
	{
        

        bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */

          
	}
}


/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* ���CPU ID */
	{
		/* �ο��ֲ᣺
			32.6.1 MCU device ID code
			33.1 Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("\r\n  CPU : STM32F407ZGT6, LQFP144, UID = %08X %08X %08X\n\r"
			, CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("\n\r");
	printf("************************************************************* \r\n");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */
	printf("* �̼���汾 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* ��ӡһ�пո� */
	printf("*************************************************************\n\r");
}

