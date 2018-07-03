/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ�����ݴ�������ģ��
*	�ļ����� : bsp_Gt9147.h
*	��    �� : 
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		
*********************************************************************************************************
*/
#ifndef BSP_GT9147_H_
#define BSP_GT9147_H_

//I2C��д����	
#define GT_CMD_WR 		0X28     	//д����
#define GT_CMD_RD 		0X29		//������
//GT9147 ���ּĴ������� 
#define GT_CTRL_REG 	0X8040   	//GT9147���ƼĴ���
#define GT_CFGS_REG 	0X8047   	//GT9147������ʼ��ַ�Ĵ���
#define GT_CHECK_REG 	0X80FF   	//GT9147У��ͼĴ���
#define GT_PID_REG 		0X8140   	//GT9147��ƷID�Ĵ���

#define GT_GSTID_REG 	0X814E   	//GT9147��ǰ��⵽�Ĵ������
#define GT_TP1_REG 		0X8150  	//��һ�����������ݵ�ַ
#define GT_TP2_REG 		0X8158		//�ڶ������������ݵ�ַ
#define GT_TP3_REG 		0X8160		//���������������ݵ�ַ
#define GT_TP4_REG 		0X8168		//���ĸ����������ݵ�ַ
#define GT_TP5_REG 		0X8170		//��������������ݵ�ַ 

/* �ṩ������C�ļ����õĺ��� */
uint8_t GT9147_Init(void);
//u8 GT9147_Scan1(u8 mode);
void GT9147_Scan(void);
void GT9147_Timer1ms(void);
#endif
