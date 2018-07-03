/*
*********************************************************************************************************
*
*	ģ������ : ������������ģ��
*	�ļ����� : bsp_key.c
*	˵    �� : ɨ�������������������˲����ƣ����а���FIFO�����Լ�������¼���
*				(1) ��������
*				(2) ��������
*				(3) ������
*				(4) ����ʱ�Զ�����
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-08-02	9527	 ����
*
*
*********************************************************************************************************
*/

#include "bsp.h"
/*  �������߷��䣺
        WK_UP      : PA0 (�ߵ�ƽ��ʾ����) 
        K0��       : PE4 (�͵�ƽ��ʾ����)
        K1��       : PE3 (�͵�ƽ��ʾ����)
        K2��       : PE2 (�͵�ƽ��ʾ����)
        
*/
#define RCC_ALL_KEY     (RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA)             /* �����ڶ�Ӧ��RCCʱ�� */

#define GPIO_PORT_WKUP  GPIOA
#define GPIO_PIN_WKUP   GPIO_Pin_0

#define GPIO_PORT_K0    GPIOE
#define GPIO_PIN_K0     GPIO_Pin_4

#define GPIO_PORT_K1    GPIOE
#define GPIO_PIN_K1     GPIO_Pin_3

#define GPIO_PORT_K2    GPIOE
#define GPIO_PIN_K2     GPIO_Pin_2

static KEY_PARAM_S s_KeyParam[KEY_COUNT];      //��������
static KEY_FIFO_S  s_KeyFifo;          //������ϢFIFO

/*******************************************************************************
*��������IsKeyDownX
* ���� ���жϰ����Ƿ���
* ���� ��
* ��� ������ֵ1 ��ʾ���£�0��ʾδ����
*******************************************************************************/
uint8_t IsWKUPDown(void) { if (PAin(0) == 1)  return 1; else	 return 0;}
uint8_t IsKey0Down(void) { if (PEin(4) == 0)  return 1; else	 return 0;}
uint8_t IsKey1Down(void) { if (PEin(3) == 0)  return 1; else	 return 0;}
uint8_t IsKey2Down(void) { if (PEin(2) == 0)  return 1; else	 return 0;}

/*******************************************************************************
*��������Key_InitHard()
* ���� ������Ӳ����ʼ��
* ���� ��
* ��� ��
*******************************************************************************/
static void Key_InitHard(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_ALL_KEY ,ENABLE);	                            //ʱ��GPIOA��GPIOEʱ��
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;								//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;								//����		
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_WKUP;
	GPIO_Init(GPIO_PORT_WKUP, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;								//����ģʽ							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;								//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K0;			
	GPIO_Init(GPIO_PORT_K0, &GPIO_InitStructure);										

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;         
    GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);   

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;         
    GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);	
}

/*******************************************************************************
*��������Key_InitVar()
* ���� ����ʼ����������
* ���� ��
* ��� ��
*******************************************************************************/
static void Key_InitVar(void)
{
    uint8_t i;
    
    /* �԰���FIFO��дָ������ */
    s_KeyFifo.Read = 0;
    s_KeyFifo.Write = 0;

    /* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
    for (i=0; i<KEY_COUNT; i++)
    {
		s_KeyParam[i].FilterTime = KEY_DOWN_PERIOD;
        s_KeyParam[i].LongTime = KEY_LONG_PERIOD;       /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */       
        s_KeyParam[i].Count = 0;
        s_KeyParam[i].RepeatTime = KEY_REPEAT_PERIOD;  /* �����������ٶȣ�0��ʾ��֧��������5��ʾÿ��50ms�Զ����ͼ�ֵ*/
        s_KeyParam[i].State = STATE_KEY_UP;
    }

    /* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ �����糤��������*/
    
    s_KeyParam[KID_K0].KeyCodeDown = KEY0_DOWN;
    s_KeyParam[KID_K0].KeyCodeUp = KEY0_UP;
    s_KeyParam[KID_K0].KeyCodeLong = KEY0_LONG;

    s_KeyParam[KID_K1].KeyCodeDown = KEY1_DOWN;
    s_KeyParam[KID_K1].KeyCodeUp = KEY1_UP;
    s_KeyParam[KID_K1].KeyCodeLong = KEY1_LONG;

    s_KeyParam[KID_K2].KeyCodeDown = KEY2_DOWN;
    s_KeyParam[KID_K2].KeyCodeUp = KEY2_UP;
    s_KeyParam[KID_K2].KeyCodeLong = KEY2_LONG;

    s_KeyParam[KID_WK_UP].KeyCodeDown = KEY_WK_DOWN;
    s_KeyParam[KID_WK_UP].KeyCodeUp = KEY_WK_UP;
    s_KeyParam[KID_WK_UP].KeyCodeLong = KEY_WK_LONG;
    /* �жϰ������µĺ��� */
    s_KeyParam[KID_K0].IsKeyDownFunc = IsKey0Down;
    s_KeyParam[KID_K1].IsKeyDownFunc = IsKey1Down;
    s_KeyParam[KID_K2].IsKeyDownFunc = IsKey2Down;
    s_KeyParam[KID_WK_UP].IsKeyDownFunc = IsWKUPDown;
	
}

/*******************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��: ��
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_InitKey(void)
{
	Key_InitVar();		/* ��ʼ���������� */
	Key_InitHard();		/* ��ʼ������Ӳ�� */
}

/*******************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ��ʼ��������������ð�������
*	��    �Σ�KeyID : ����ID����0��ʼ
*			  LongTime : �����¼�ʱ��
*			 RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_SetKeyParam(KEY_ID_E KeyID, u16 LongTime, uint8_t RepeatSpeed)
{
	s_KeyParam[KeyID].LongTime = LongTime;			        /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_KeyParam[KeyID].RepeatTime = RepeatSpeed;				/* �����������ٶȣ�0��ʾ��֧������ */
}

/*******************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*******************************************************************************/
void bsp_ClearKeyFifo(void)
{
	s_KeyFifo.Read = s_KeyFifo.Write;
}

/********************************************************************************
*	�� �� ��: bsp_bsp_PutKeyMsg
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  KeyCode : ��������
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_PutKeyMsg(uint8_t KeyCode)
{
	s_KeyFifo.Buf[s_KeyFifo.Write] = KeyCode;

	if (++s_KeyFifo.Write  >= KEY_FIFO_SIZE)
	{
		s_KeyFifo.Write = 0;
	}
}
/*******************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��:  ��
*	�� �� ֵ: ��������
*******************************************************************************/
uint8_t bsp_GetKeyMsg(void)
{
	uint8_t ret;

	if (s_KeyFifo.Read == s_KeyFifo.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_KeyFifo.Buf[s_KeyFifo.Read];

		if (++s_KeyFifo.Read >= KEY_FIFO_SIZE)
		{
			s_KeyFifo.Read = 0;
		}
		return ret;
	}
}

/*******************************************************************************
*	�� �� ��: bsp_KeyTickService()
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��:  �����ṹ����ָ��
*	�� �� ֵ: ��
*******************************************************************************/
static void KeyTickService(uint8_t i)
{
    KEY_PARAM_S *_pBtn;

    _pBtn = &s_KeyParam[i];
    switch (_pBtn->State)
    {
        case STATE_KEY_UP:
            if (_pBtn->IsKeyDownFunc())
            {
                 _pBtn->State = STATE_KEY_DEBOUNCE;
                 _pBtn->Count = 0;
            }
            break;
        case STATE_KEY_DEBOUNCE:            
            if (_pBtn->IsKeyDownFunc())         
            {                
                _pBtn->State = STATE_KEY_DOWN;    
				_pBtn->Count = 0;				
            }            
            else            
            {                
               _pBtn->State = STATE_KEY_UP;            
            }            
            break;
        case STATE_KEY_DOWN:            
            if (_pBtn->IsKeyDownFunc())  
            {                
                _pBtn->Count++;                            
                if (_pBtn->Count >= _pBtn->FilterTime)             
                {                   
                    /* 100ms ���Ͱ�ť���µ���Ϣ */                    
                    bsp_PutKeyMsg(_pBtn->KeyCodeDown);
					if (_pBtn->LongTime > 0)    	 	//��ⳤ����
					{
						_pBtn->State = STATE_KEY_LONG;   
					}
					else							//����ⳤ����
					{
						_pBtn->State = STATE_KEY_RELEASE;   
					}                                 
                }            
            }            
            else          
            {                
                _pBtn->State = STATE_KEY_UP;           
             }
             break;
         case STATE_KEY_LONG:
            if (_pBtn->IsKeyDownFunc())  
            {               
				_pBtn->Count++;  
				if (_pBtn->Count >= _pBtn->LongTime)
				{
					/* 2s�����Ͱ�ť�����µ���Ϣ */ 
					bsp_PutKeyMsg(_pBtn->KeyCodeLong);
					if (_pBtn->RepeatTime> 0)     	//�����������
					{
						_pBtn->State = STATE_KEY_REPEAT;
						_pBtn->Count = 0;						
					}
					else
					{
						 _pBtn->State = STATE_KEY_RELEASE;
					}

				}              
            }
            else          
            {                
               _pBtn->State = STATE_KEY_RELEASE; 
            }
            break;
        case  STATE_KEY_REPEAT:
            if (_pBtn->IsKeyDownFunc())  
            {                
				 _pBtn->Count++;  
				 if (_pBtn->Count >= _pBtn->RepeatTime)
				 {
					 /* ��������ÿ��KEY_REPEAT_PERIOD����1��������Ϣ */
					 bsp_PutKeyMsg(_pBtn->KeyCodeDown);
					 _pBtn->State = STATE_KEY_REPEAT;
					 _pBtn->Count = 0;
				 }                 
            }
            else
            {
                _pBtn->State = STATE_KEY_RELEASE;
            }
            break;
        case STATE_KEY_RELEASE:
            /* ���Ͱ�ť�������Ϣ */
            bsp_PutKeyMsg(_pBtn->KeyCodeUp);
            _pBtn->State = STATE_KEY_UP;
            break;
        default:
            _pBtn->State = STATE_KEY_UP;
            break;
                
    }
}
/*******************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*******************************************************************************/
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		KeyTickService(i);
	}
}


