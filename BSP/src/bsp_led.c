/*
********************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-08-02	9527	 LED��
*
*

*/

#include "bsp.h"
/*******************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	bsp_LedOff(0);
	bsp_LedOff(1);
	//PF9��PF10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;	//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//100MHz
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//��ʼ��GPIO
	
}

/*******************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  no : ָʾ����ţ���Χ 0 - 1
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 0-1
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ��⺯��GPIO_ToggleBits()����
*	��    ��:  _no : ָʾ����ţ���Χ 0-1
*	�� �� ֵ: ��������
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
