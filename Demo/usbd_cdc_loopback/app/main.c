/*
*********************************************************************************************************
*
*	模块名称 : 模板工程
*	文件名称 : main.c
*	版    本 : V1.1
*	说    明 : 
*           1、The system Clock is configured as follow : 
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
*           2NVIC优先级分组为Group4：0-15抢占式优先级。
*               
*	修改记录 :
*		版本号   日期        作者      说明

*
*********************************************************************************************************
*/


#include "bsp.h"			/* 底层硬件驱动 */
#include "usb_bsp.h"
#include "usbd_cdc_core_loopback.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "main.h"   
#include "am_common.h"

//#include "usb_bsp_msc.h"		/* usb底层驱动 */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;
uint8_t Rxbuffer[64]; 
__IO uint32_t receive_count =1;
extern __IO uint32_t  data_sent;

static void PrintfLogo(void);
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
    uint32_t file_size;
	MSG_S ucMsg;
	FONT_T tFont;			/* 定义一个字体结构体变量，用于设置字体参数 */
	
	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
	配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM
	*/

  	bsp_Init();				/* 硬件初始化 */
      USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);  
//	LoadParam();			/* 加载参数 读取EEPROM中的全局变量*/
//	MODBUS_Init();			/* modbus初始化 */
	
	PrintfLogo();			/* 打印例程信息到串口1 */

//    SD_FatfsTest();

	/* 设置字体参数 */
	{
		tFont.FontCode = FC_ST_16;		/* 字体代码 16点阵 */
		tFont.FrontColor = CL_WHITE;	/* 字体颜色 */
		tFont.BackColor = CL_BLUE;		/* 文字背景颜色 */
		tFont.Space = 0;				/* 文字间距，单位 = 像素 */
	}
	LCD_Fill_Rect(0, 0, g_lcddev.width,40, CL_BLUE);
	LCD_DispStr(10, 10, (uint8_t *)VER_INFO, &tFont);							/* 显示软件版本信息 */





//    usbd_OpenMassStorage();	/* 初始化USB Device，配置为Mass Storage */
	/* 进入主程序循环体 */
	while (1)
	{
        

        bsp_Idle();		/* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */

          
	}
}


/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* 检测CPU ID */
	{
		/* 参考手册：
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
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */
	printf("* 固件库版本 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* 打印一行空格 */
	printf("*************************************************************\n\r");
}

