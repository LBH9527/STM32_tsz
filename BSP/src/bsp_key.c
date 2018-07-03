/*
*********************************************************************************************************
*
*	模块名称 : 独立按键驱动模块
*	文件名称 : bsp_key.c
*	说    明 : 扫描独立按键，具有软件滤波机制，具有按键FIFO。可以检测如下事件：
*				(1) 按键按下
*				(2) 按键弹起
*				(3) 长按键
*				(4) 长按时自动连发
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-08-02	9527	 按键
*
*
*********************************************************************************************************
*/

#include "bsp.h"
/*  按键口线分配：
        WK_UP      : PA0 (高电平表示按下) 
        K0键       : PE4 (低电平表示按下)
        K1键       : PE3 (低电平表示按下)
        K2键       : PE2 (低电平表示按下)
        
*/
#define RCC_ALL_KEY     (RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA)             /* 按键口对应的RCC时钟 */

#define GPIO_PORT_WKUP  GPIOA
#define GPIO_PIN_WKUP   GPIO_Pin_0

#define GPIO_PORT_K0    GPIOE
#define GPIO_PIN_K0     GPIO_Pin_4

#define GPIO_PORT_K1    GPIOE
#define GPIO_PIN_K1     GPIO_Pin_3

#define GPIO_PORT_K2    GPIOE
#define GPIO_PIN_K2     GPIO_Pin_2

static KEY_PARAM_S s_KeyParam[KEY_COUNT];      //按键参数
static KEY_FIFO_S  s_KeyFifo;          //按键消息FIFO

/*******************************************************************************
*函数名：IsKeyDownX
* 描述 ：判断按键是否按下
* 输入 ：
* 输出 ：返回值1 表示按下，0表示未按下
*******************************************************************************/
uint8_t IsWKUPDown(void) { if (PAin(0) == 1)  return 1; else	 return 0;}
uint8_t IsKey0Down(void) { if (PEin(4) == 0)  return 1; else	 return 0;}
uint8_t IsKey1Down(void) { if (PEin(3) == 0)  return 1; else	 return 0;}
uint8_t IsKey2Down(void) { if (PEin(2) == 0)  return 1; else	 return 0;}

/*******************************************************************************
*函数名：Key_InitHard()
* 描述 ：按键硬件初始化
* 输入 ：
* 输出 ：
*******************************************************************************/
static void Key_InitHard(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_ALL_KEY ,ENABLE);	                            //时能GPIOA，GPIOE时钟
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;								//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;								//下拉		
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_WKUP;
	GPIO_Init(GPIO_PORT_WKUP, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;								//输入模式							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;								//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K0;			
	GPIO_Init(GPIO_PORT_K0, &GPIO_InitStructure);										

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;         
    GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);   

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;         
    GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);	
}

/*******************************************************************************
*函数名：Key_InitVar()
* 描述 ：初始化按键变量
* 输入 ：
* 输出 ：
*******************************************************************************/
static void Key_InitVar(void)
{
    uint8_t i;
    
    /* 对按键FIFO读写指针清零 */
    s_KeyFifo.Read = 0;
    s_KeyFifo.Write = 0;

    /* 给每个按键结构体成员变量赋一组缺省值 */
    for (i=0; i<KEY_COUNT; i++)
    {
		s_KeyParam[i].FilterTime = KEY_DOWN_PERIOD;
        s_KeyParam[i].LongTime = KEY_LONG_PERIOD;       /* 长按时间 0 表示不检测长按键事件 */       
        s_KeyParam[i].Count = 0;
        s_KeyParam[i].RepeatTime = KEY_REPEAT_PERIOD;  /* 按键连发的速度，0表示不支持连发，5表示每个50ms自动发送键值*/
        s_KeyParam[i].State = STATE_KEY_UP;
    }

    /* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 ，比如长按键检测等*/
    
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
    /* 判断按键按下的函数 */
    s_KeyParam[KID_K0].IsKeyDownFunc = IsKey0Down;
    s_KeyParam[KID_K1].IsKeyDownFunc = IsKey1Down;
    s_KeyParam[KID_K2].IsKeyDownFunc = IsKey2Down;
    s_KeyParam[KID_WK_UP].IsKeyDownFunc = IsWKUPDown;
	
}

/*******************************************************************************
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参: 无
*	返 回 值: 无
*******************************************************************************/
void bsp_InitKey(void)
{
	Key_InitVar();		/* 初始化按键变量 */
	Key_InitHard();		/* 初始化按键硬件 */
}

/*******************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 初始化后可以重新设置按键参数
*	形    参：KeyID : 按键ID，从0开始
*			  LongTime : 长按事件时间
*			 RepeatSpeed : 连发速度
*	返 回 值: 无
*******************************************************************************/
void bsp_SetKeyParam(KEY_ID_E KeyID, u16 LongTime, uint8_t RepeatSpeed)
{
	s_KeyParam[KeyID].LongTime = LongTime;			        /* 长按时间 0 表示不检测长按键事件 */
	s_KeyParam[KeyID].RepeatTime = RepeatSpeed;				/* 按键连发的速度，0表示不支持连发 */
}

/*******************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*******************************************************************************/
void bsp_ClearKeyFifo(void)
{
	s_KeyFifo.Read = s_KeyFifo.Write;
}

/********************************************************************************
*	函 数 名: bsp_bsp_PutKeyMsg
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  KeyCode : 按键代码
*	返 回 值: 无
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
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
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
*	函 数 名: bsp_KeyTickService()
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参:  按键结构变量指针
*	返 回 值: 无
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
                    /* 100ms 发送按钮按下的消息 */                    
                    bsp_PutKeyMsg(_pBtn->KeyCodeDown);
					if (_pBtn->LongTime > 0)    	 	//检测长按键
					{
						_pBtn->State = STATE_KEY_LONG;   
					}
					else							//不检测长按键
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
					/* 2s，发送按钮长按下的消息 */ 
					bsp_PutKeyMsg(_pBtn->KeyCodeLong);
					if (_pBtn->RepeatTime> 0)     	//检测连续按键
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
					 /* 常按键后，每隔KEY_REPEAT_PERIOD发送1个按键消息 */
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
            /* 发送按钮弹起的消息 */
            bsp_PutKeyMsg(_pBtn->KeyCodeUp);
            _pBtn->State = STATE_KEY_UP;
            break;
        default:
            _pBtn->State = STATE_KEY_UP;
            break;
                
    }
}
/*******************************************************************************
*	函 数 名: bsp_KeyScan
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
*	形    参:  无
*	返 回 值: 无
*******************************************************************************/
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		KeyTickService(i);
	}
}


