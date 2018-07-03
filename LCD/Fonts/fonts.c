#include "bsp.h"
#include "fonts.h"
#include "fontupd.h"

/*********************************************
*��������LCD_Pow
* ���� ��m^n����
* ���� ��
* ��� ��:m^n�η�.
*********************************************/
u32 LCD_Pow(u8 m, u8 n)
{
	u32 result = 1;
	while(n--)	result *= m;
	return result;
}

/*********************************************
*��������LCD_ShowNum()
* ���� ����ָ��λ����ʾһ��������λΪ0����ʾ
* ���� ��x,y:��ʼ����, len���ֵĳ��ȣ�size �����С��num:��ֵ(0~4294967295);	
* ��� ��
*********************************************/
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
	u8 enshow = 0;			//��λΪ0 �Ƿ���ʾ��־λ
	
	for (t=0;t<len;t++)
	{
		temp = (num/LCD_Pow(10, len-t-1))%10;
		if (enshow==0 && t<(len-1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}	
}

/*********************************************
*��������LCD_ShowxNum()
* ���� ����ָ��λ����ʾһ��������λΪ0������ʾ
* ���� ��x,y:��ʼ����, len���ֵĳ��ȣ�size �����С��num:��ֵ(0~999999999);	
//mode: ��λΪ0ʱ�Ƿ���0���
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
* ��� ��
*********************************************/
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t=0; t<len; t++)
	{
		temp = (num/LCD_Pow(10, len-t-1))%10;
		if (enshow==0 && t<(len-1))
		{
			if (temp == 0)
			{
				if (mode & 0x80)
					LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else
					LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}	
}	
/*********************************************
*��������LCD_ShowString()
* ���� ����ָ��λ����ʾ�ַ���
* ���� ��x,y:��ʼ����, width,height:�����С , size:�����С 12/16/24, *p:�ַ�����ʼ��ַ		  
* ��� ��
*********************************************/
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width += x;
	height += y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x >= width)
		{
			x=x0;
			y+=size;
		}
        if(y>=height)
			break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
//	unsigned long foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return; //��������
	}          
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
//	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  		  
	switch(size)
	{
		case 12:
//			W25QXX_Read(mat,foffset+ftinfo.f12addr,csize);
			break;
		case 16:
//			W25QXX_Read(mat,foffset+ftinfo.f16addr,csize);
			break;
		case 24:
//			W25QXX_Read(mat,foffset+ftinfo.f24addr,csize);
			break;
			
	}     												    
}  
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+width-size))//����
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//Խ�緵��  						     
	        Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,g_lcddev.width,g_lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,g_lcddev.width,g_lcddev.height,str,size,1);
	}
}   
























		  






