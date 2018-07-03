#include "bsp.h"
#include "fonts.h"
#include "fontupd.h"

/*********************************************
*函数名：LCD_Pow
* 描述 ：m^n函数
* 输入 ：
* 输出 ：:m^n次方.
*********************************************/
u32 LCD_Pow(u8 m, u8 n)
{
	u32 result = 1;
	while(n--)	result *= m;
	return result;
}

/*********************************************
*函数名：LCD_ShowNum()
* 描述 ：在指定位置显示一个数，高位为0则不显示
* 输入 ：x,y:起始坐标, len数字的长度，size 字体大小，num:数值(0~4294967295);	
* 输出 ：
*********************************************/
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
	u8 enshow = 0;			//首位为0 是否显示标志位
	
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
*函数名：LCD_ShowxNum()
* 描述 ：在指定位置显示一个数，高位为0还是显示
* 输入 ：x,y:起始坐标, len数字的长度，size 字体大小，num:数值(0~999999999);	
//mode: 高位为0时是否用0填充
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
* 输出 ：
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
*函数名：LCD_ShowString()
* 描述 ：在指定位置显示字符串
* 输入 ：x,y:起始坐标, width,height:区域大小 , size:字体大小 12/16/24, *p:字符串起始地址		  
* 输出 ：
*********************************************/
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width += x;
	height += y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x >= width)
		{
			x=x0;
			y+=size;
		}
        if(y>=height)
			break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
//	unsigned long foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}          
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
//	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		  
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
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据                          
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
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(x0+width-size/2))//换行
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }else//中文 
        {     
            bHz=0;//有汉字库    
            if(x>(x0+width-size))//换行
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//越界返回  						     
	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
}  			 		 
//在指定宽度的中间显示字符串
//如果字符长度超过了len,则用Show_Str显示
//len:指定要显示的宽度			  
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
























		  






