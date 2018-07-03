/*
********************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-08-02	9527	 LED灯
*
*

*/

#include "bsp.h"
/*******************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 初始化
*	形    参: 无
*	返 回 值: 无
*******************************************************************************/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	bsp_LedOff(0);
	bsp_LedOff(1);
	//PF9、PF10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;	//LED0、LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//100MHz
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//初始化GPIO
	
}

/*******************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  no : 指示灯序号，范围 0 - 1
*	返 回 值: 无
*******************************************************************************/
void bsp_LedOn(uint8_t no)
{
	if (no < LEDn)
    {
        if (no == 0)
        {
            GPIOF->BSRRH = GPIO_Pin_9;
        }
        else if (no == 1)
        {
            GPIOF->BSRRH = GPIO_Pin_10;
        }        
    }
}

/*******************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 0-1
*	返 回 值: 无
*******************************************************************************/
void bsp_LedOff(uint8_t no)
{
    if (no < LEDn)
    {
        if (no == 0)
        {
            GPIOF->BSRRL = GPIO_Pin_9;
        }
        else if (no == 1)
        {
            GPIOF->BSRRL = GPIO_Pin_10;
        }        
    }

}

/*******************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。库函数GPIO_ToggleBits()功能
*	形    参:  _no : 指示灯序号，范围 0-1
*	返 回 值: 按键代码
*******************************************************************************/
void bsp_LedToggle(u8 no)
{
    if (no < LEDn)
    {
        if (no == 0)
        {
            GPIOF->ODR ^= GPIO_Pin_9;
        }
        else if (no == 1)
        {
            GPIOF->ODR ^= GPIO_Pin_10;
        }        
    }

}
