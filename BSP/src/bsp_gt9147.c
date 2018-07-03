/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ�����ݴ�������ģ��
*	�ļ����� : bsp_Gt9147.c
*	��    �� : 
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		
*********************************************************************************************************
*/
#include "bsp.h"
#include  "bsp_Gt9147.h"
/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GPIO_PORT_I2C_SCL	GPIOB			/* GPIO�˿� */
#define GPIO_PORT_I2C_SDA	GPIOF			/* GPIO�˿� */
#define RCC_I2C_SCL_PORT 	RCC_AHB1Periph_GPIOB		/* SCL GPIO �˿�ʱ�� */
#define RCC_I2C_SDA_PORT	RCC_AHB1Periph_GPIOF		/* SDA GPIO �˿�ʱ�� */
#define CT_I2C_SCL_PIN		GPIO_Pin_0			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define CT_I2C_SDA_PIN		GPIO_Pin_11			/* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ� */
#define CT_I2C_SCL_1()  GPIO_PORT_I2C_SCL->BSRRL = CT_I2C_SCL_PIN				/* SCL = 1 */
#define CT_I2C_SCL_0()  GPIO_PORT_I2C_SCL->BSRRH = CT_I2C_SCL_PIN				/* SCL = 0 */

#define CT_I2C_SDA_1()  GPIO_PORT_I2C_SDA->BSRRL = CT_I2C_SDA_PIN				/* SDA = 1 */
#define CT_I2C_SDA_0()  GPIO_PORT_I2C_SDA->BSRRH = CT_I2C_SDA_PIN				/* SDA = 0 */

#define CT_I2C_SDA_READ()  ((GPIO_PORT_I2C_SDA->IDR & CT_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#define CT_I2C_SCL_READ()  ((GPIO_PORT_I2C_SCL->IDR & CT_I2C_SCL_PIN) != 0)	/* ��SCL����״̬ */
//IO��������
#define CT_SDA_IN()  {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=0<<2*11;}	//PF11����ģʽ
#define CT_SDA_OUT() {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=1<<2*11;} 	//PF11���ģʽ
/* �ж��źź͸�λ�źŶ˿� */
#define GPIO_PORT_INT		GPIOB
#define GPIO_PORT_RST		GPIOC
#define RCC_INT_PORT 		RCC_AHB1Periph_GPIOB		/* INT GPIO �˿�ʱ�� */
#define RCC_RST_PORT		RCC_AHB1Periph_GPIOC		/* RST GPIO �˿�ʱ�� */
#define GPIO_PIN_INT		GPIO_Pin_1					/* ���ӵ�INTʱ���ߵ�GPIO */
#define GPIO_PIN_RST		GPIO_Pin_13					/* ���ӵ�RST�����ߵ�GPIO */

#define GT_RST_1()		GPIO_PORT_RST->BSRRL = GPIO_PIN_RST					/* RST = 1 */
#define GT_RST_0()		GPIO_PORT_RST->BSRRH = GPIO_PIN_RST					/* RST = 0 */
#define INT_IS_HIGH()	(GPIO_ReadInputDataBit(GPIO_PORT_INT, GPIO_PIN_INT) == Bit_SET)

const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};	//���������ݵ�ַ
GT9147_T	g_tGT9147;	
uint16_t GT9147_ReadVersion(void);
/*
 *GT9147���ò�����
 *��һ���ֽ�Ϊ�汾��(0X60),���뱣֤�µİ汾�Ŵ��ڵ���GT9147�ڲ�
 *flashԭ�а汾��,�Ż��������.
 */
const uint8_t GT9147_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
}; 
void CT_IIC_Stop(void);
//void CT_IIC_Start(void);
/*
*********************************************************************************************************
*	�� �� ��: CT_IIC_Init
*	����˵��: ���ݴ���оƬIIC�ӿڳ�ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CT_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_I2C_SCL_PORT|RCC_I2C_SDA_PORT, ENABLE);//ʹ��GPIOB,Fʱ��

	GPIO_InitStructure.GPIO_Pin = CT_I2C_SCL_PIN;//PB0����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIO_PORT_I2C_SCL, &GPIO_InitStructure);//��ʼ��
		
	GPIO_InitStructure.GPIO_Pin = CT_I2C_SDA_PIN;//PF11�����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_Init(GPIO_PORT_I2C_SDA, &GPIO_InitStructure);//��ʼ��	

	CT_IIC_Stop();
}
/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void I2C_Delay(void)
{
	uint8_t i;

	/*��
		CPU��Ƶ168MHzʱ�����ڲ�Flash����, MDK���̲��Ż�����̨ʽʾ�����۲Ⲩ�Ρ�
		ѭ������Ϊ5ʱ��SCLƵ�� = 1.78MHz (����ʱ: 92ms, ��д������������ʾ����̽ͷ���ϾͶ�дʧ�ܡ�ʱ��ӽ��ٽ�)
		ѭ������Ϊ10ʱ��SCLƵ�� = 1.1MHz (����ʱ: 138ms, ���ٶ�: 118724B/s)
		ѭ������Ϊ30ʱ��SCLƵ�� = 440KHz�� SCL�ߵ�ƽʱ��1.0us��SCL�͵�ƽʱ��2us

		��������ѡ��2.2Kŷʱ��SCL������ʱ��Լ0.5us�����ѡ4.7Kŷ����������Լ1us

		ʵ��Ӧ��ѡ��400KHz���ҵ����ʼ���
	*/
	for (i = 0; i < 60; i++);
	//bsp_DelayUS(2);

}
/*
*********************************************************************************************************
*	�� �� ��: I2C_Start
*	����˵��: CPU����I2C���������ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CT_IIC_Start(void)
{	
    CT_SDA_OUT();

	CT_I2C_SCL_1(); 
	CT_I2C_SDA_1(); 
	I2C_Delay();
	//bsp_DelayUS(30);
	CT_I2C_SDA_0();		//������SDA
	I2C_Delay();
	CT_I2C_SCL_0(); 	//������SCL
	I2C_Delay();
}
/*
*********************************************************************************************************
*	�� �� ��: CT_IIC_Stop
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CT_IIC_Stop(void)
{	
	CT_SDA_OUT();
	CT_I2C_SDA_0();
	CT_I2C_SCL_1();
	I2C_Delay();
	//bsp_DelayUS(30);
	CT_I2C_SDA_0();
	I2C_Delay();
	CT_I2C_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: CT_IIC_Wait_Ack
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    ��:  ��
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t CT_IIC_Wait_Ack(void)
{	
	uint8_t ucErrTime=0;
	CT_SDA_IN();      //SDA����Ϊ����  
	CT_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	I2C_Delay();
	CT_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	I2C_Delay();		 
	while(CT_I2C_SDA_READ())      //�ȴ�Ӧ��
	{
        ucErrTime++;
        if(ucErrTime>=250)
        {
            I2C_Stop();          
            return 1;
        }
	}		
	CT_I2C_SCL_0();//������SCL���Ӧ��λ��������ס����
	I2C_Delay();
	return 0;
}
/*
*********************************************************************************************************
*   �� �� ��: CT_IIC_Ack
*   ����˵��: I2C ������.����Ӧ��λ
*   ��    ��:  ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void CT_IIC_Ack(void)
{
	CT_SDA_OUT();
	CT_I2C_SDA_0();	/* CPU����SDA = 0 */
	I2C_Delay();
	CT_I2C_SCL_1();	/* CPU����1��ʱ�� */
	I2C_Delay();
	CT_I2C_SCL_0();
	I2C_Delay();
	CT_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}
	
/*
*********************************************************************************************************
*	�� �� ��: CT_IIC_NAck
*	����˵��: I2C ������.������Ӧ��λ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CT_IIC_NAck(void)
{
	CT_SDA_OUT();
	CT_I2C_SDA_1();	/* CPU����SDA = 1 */
	I2C_Delay();
	CT_I2C_SCL_1();	/* CPU����1��ʱ�� */
	I2C_Delay();
	CT_I2C_SCL_0();
	I2C_Delay();
}
/*
*********************************************************************************************************
*	�� �� ��: i2c_Read
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    ��:  ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t CT_IIC_Read_Byte(void)
{
	uint8_t i;
	uint8_t value;
	CT_SDA_IN();//SDA����Ϊ����
	//bsp_DelayUS(30);
	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		CT_I2C_SCL_1();
		I2C_Delay();
		if (CT_I2C_SDA_READ())
		{
			value++;
		}
		CT_I2C_SCL_0();
		I2C_Delay();
	}
	return value;
}
/*
*********************************************************************************************************
*	�� �� ��: CT_IIC_Send_Byte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    ��:  _ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CT_IIC_Send_Byte(uint8_t _ucByte)
{
	uint8_t i;
	CT_SDA_OUT(); 	 
	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			CT_I2C_SDA_1();
		}
		else
		{
			CT_I2C_SDA_0();
		}
		I2C_Delay();
		CT_I2C_SCL_1();
		I2C_Delay();
		CT_I2C_SCL_0();
		if (i == 7)
		{
			 CT_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		I2C_Delay();
	}
}

/*
*********************************************************************************************************
*��������GT9147_WR_Reg()
* ���� ����GT9147д��һ������
* ���� ��reg:��ʼ�Ĵ�����ַ buf:���ݻ������� len:д���ݳ���
* ��� ������ֵ:0,�ɹ�;1,ʧ��.
*********************************************************************************************************
*/
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for (i = 0;i < len; i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret = CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
/*
*********************************************************************************************************
*��������GT9147_RD_Reg()
* ���� ����GT9147����һ������
* ���� ��reg:��ʼ�Ĵ�����ַ  buf:���ݻ�������  len:�����ݳ���		
* ��� ������ֵ:0,�ɹ�;1,ʧ��.
*********************************************************************************************************
*/
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //����д���� 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for (i = 0; i < len; i++)
	{
		buf[i] = CT_IIC_Read_Byte();	/* ��1���ֽ� */

		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != len - 1)
		{
			CT_IIC_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			CT_IIC_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	} 
    CT_IIC_Stop();//����һ��ֹͣ����    
} 
   
/*
*********************************************************************************************************
*��������GT9147_Send_Cfg()
* ���� ������GT9147���ò���
* ���� ��mode:0,���������浽flash  1,�������浽flash
* ��� ��0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
*********************************************************************************************************
*/
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
	uint8_t buf[2];
	uint8_t i=0;
	buf[0]=0;
	buf[1]=mode;	//�Ƿ�д�뵽GT9147 FLASH?  ���Ƿ���籣��
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//����У���
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//���ͼĴ�������
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//д��У���,�����ø��±��
	return 0;
} 
/*
*********************************************************************************************************
*��������GT9147_InitHard()
* ���� ����ʼ��GT9147������Ӳ��
* ���� ��
* ��� ��
*********************************************************************************************************
*/
void GT9147_InitHard(void)
{
	uint8_t temp; 
	
	GPIO_InitTypeDef  GPIO_InitStructure;	

	RCC_AHB1PeriphClockCmd(RCC_INT_PORT|RCC_RST_PORT, ENABLE);//ʹ��GPIOB,Cʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_INT ;//PB1����Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIO_PORT_INT, &GPIO_InitStructure);//��ʼ��
		
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RST;//PC13����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_Init(GPIO_PORT_RST, &GPIO_InitStructure);//��ʼ��	

	CT_IIC_Init();      	//��ʼ����������I2C����  
	GT_RST_0();				//��λ
	bsp_DelayMS(10);
	GT_RST_1();				//�ͷŸ�λ		    
	bsp_DelayMS(10); 

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_INT ;//PB1����Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIO_PORT_INT, &GPIO_InitStructure);//��ʼ��
	 
	bsp_DelayMS(100);  
	
	temp = 0X02;			
	GT9147_WR_Reg(GT_CTRL_REG,&temp,1);//��λGT9147
	GT9147_RD_Reg(GT_CFGS_REG,&temp,1);//��ȡGT_CFGS_REG�Ĵ���
	if(temp<0X60)//Ĭ�ϰ汾�Ƚϵ�,��Ҫ����flash����
	{
	//	printf("Default Ver:%d\r\n",temp[0]);
		GT9147_Send_Cfg(1);//���²���������
	}
	bsp_DelayMS(10);
	temp = 0X00;	 
	GT9147_WR_Reg(GT_CTRL_REG,&temp,1);//������λ 
}
/*
*********************************************************************************************************
*��������GT9147_Init()
* ���� ����ʼ��GT9147������
* ���� ��
* ��� ��0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
*********************************************************************************************************
*/
uint8_t GT9147_Init(void)
{
	uint16_t ver;
	GT9147_InitHard();
	ver = GT9147_ReadVersion();
	g_tGT9147.TimerCount = 0;
	
	if (ver == 9147)
	{
//		printf("CTP ID:%d\r\n",ver);	//��ӡID
		return 0;
	}
	else
		return 1;	
}
/*
*********************************************************************************************************
*	�� �� ��: GT811_Scan
*	����˵��: ��ȡGT811�������ݡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT9147_Scan(void)
{
    uint8_t i;
	uint8_t mode,temp;
	uint8_t buf[4];
    static uint8_t state = TOUCH_NONE;
    uint16_t x, y;
	static uint16_t x_save, y_save;
	
	/* 20ms ִ��һ�� ˢ��ʱ��̫��,�������*/
	if (g_tGT9147.TimerCount < 20)
	    return;
	g_tGT9147.TimerCount = 0;
	
	GT9147_RD_Reg(GT_GSTID_REG,&mode,1);//��ȡ�������״̬  
	if ((mode&0X0F)&&((mode&0X0F)<6))	
	{

        for (i=0; i<(mode&0X0f); i++)
        {
			GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//��ȡXY����ֵ	
			if(g_lcddev.direction > 3)//����
			{
				g_tGT9147.y[i]=((uint16_t)buf[1]<<8)+buf[0];
				g_tGT9147.x[i]=g_lcddev.width -(((uint16_t)buf[3]<<8)+buf[2]);
			}else		  //����
			{
				g_tGT9147.x[i]=((uint16_t)buf[1]<<8)+buf[0];
				g_tGT9147.y[i]=((uint16_t)buf[3]<<8)+buf[2];
			} 		
        }
		x = g_tGT9147.x[0];
		y = g_tGT9147.y[0];
		if (state == TOUCH_NONE)
			state = TOUCH_DOWN;
	}

	switch (state)
	{
        case TOUCH_NONE:			
            break;
        case TOUCH_DOWN:
			if (mode&0X8F> 0x80)		//�д����Ҵ�����������0��
			{
				TOUCH_PutKey(TOUCH_DOWN, x, y);
				state = TOUCH_MOVE;
				//	printf("down\r\n");
			}
			else
			{
				x_save = x;				/* �������꣬�����ͷ��¼� */
				y_save = y;
				state = TOUCH_RELEASE;
			}
			break;
		case TOUCH_MOVE:
			if (mode&0X8F> 0x80)		//�д����Ҵ�����������0��
			{
				TOUCH_PutKey(TOUCH_MOVE, x, y);
				state = TOUCH_MOVE;
				//	printf("move\r\n");
			}
			else
			{
				x_save = x;			/* �������꣬�����ͷ��¼� */
				y_save = y;
				state = TOUCH_RELEASE;
			}			
			break;
		case TOUCH_RELEASE:
			TOUCH_PutKey(TOUCH_RELEASE, x_save, y_save);	/* �ͷ� */
			state = TOUCH_NONE;
			break;
	}
	if(mode&0X80&&((mode&0XF)<6))
	{
		temp=0;
		GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//���־ 		
	}
	
}
/*
*********************************************************************************************************
*	�� �� ��: GT811_Timer1ms
*	����˵��: ÿ��1ms����1��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT9147_Timer1ms(void)
{
	g_tGT9147.TimerCount++;
}

//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
#if 0
uint8_t GT9147_Scan1(uint8_t mode,uint8_t lcdDir)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	static uint8_t t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{
		GT9147_RD_Reg(GT_GSTID_REG,&mode,1);//��ȡ�������״̬  
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//������Ч?
				{
					GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
					if(lcdDir>3)//����
					{
						tp_dev.y[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.x[i]=g_lcddev.width -(((uint16_t)buf[3]<<8)+buf[2]);
					}else				//����
					{
						tp_dev.x[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((uint16_t)buf[3]<<8)+buf[2];
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//���������ݶ���0,����Դ˴�����
			t=0;		//����һ��,��������������10��,�Ӷ����������
		}
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//���־ 		
		}
	}
	if((mode&0X8F)==0X80)//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	 
	} 	
	if(t>240)t=10;//���´�10��ʼ����
	return res;
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: GT9147_ReadVersion
*	����˵��: ���GT9147��оƬ�汾
*	��    ��: ��
*	�� �� ֵ: 32λ�汾
*********************************************************************************************************
*/
static uint16_t GT9147_ReadVersion(void)
{
	uint8_t buf[5]; 
	
	GT9147_RD_Reg(GT_PID_REG,buf,4);//��ȡ��ƷID,ASCII����ʽ
	buf[4]=0;
	buf[0] = buf[0] - 0x30;
	buf[1] = buf[1] - 0x30;
	buf[2] = buf[2] - 0x30;
	buf[3] = buf[3] - 0x30;
	return ((uint16_t)buf[0] *1000 + (uint16_t)buf[1]*100+ buf[2]*10+buf[3]);
}

















