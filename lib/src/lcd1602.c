#include<lcd1602.h>
bit BusyTest(void)			//检测lcd忙碌
{
	bit result;
	RS=0;
	RW=1;
	EN=1;
	Delaynus(4);//_nop_();_nop_();_nop_();_nop_();
	result=BF;	 
  	EN=0;	
  	return result;   
}
void Write_com (uchar com)                             //lcd写命令
{
	while(BusyTest()==1);
 	RS=0; 
	RW=0; 
	EN=0;
	Delaynus(4);//_nop_(); _nop_();  
  	DatReg=com;
	Delaynus(4);//_nop_();_nop_();_nop_();_nop_();             
	EN=1; 
	Delaynus(4);//_nop_(); _nop_(); _nop_(); _nop_();
	EN=0; 
}
void WriteAddress(uchar x)	                       //lcd改变地址
{   
 	Write_com(x|0x80); 
}
void Write_data(uchar dat)	                      //lcd写数据
{   
	while(BusyTest()==1); 
	RS=1; 
	RW=0; 
	EN=0;            
   	DatReg=dat;
	Delaynus(4);//_nop_(); _nop_(); _nop_(); _nop_();      
	EN=1;
	Delaynus(4);//_nop_(); _nop_(); _nop_(); _nop_();
	EN=0;
}
void InitLcd1602()	 //lcd初始化
{ 	Delaynms(15);            
   	Write_com(0x38);Delaynms(5);  
	Write_com(0x38);Delaynms(5);
	Write_com(0x38);Delaynms(5);
	Write_com(0x0C);Delaynms(5);
	Write_com(0x06);Delaynms(5);			  
	Write_com(0x01);Delaynms(5);    
}
void LcdAddrStrp(uchar add,char *str)	 //在确定地址显示字符串,形参为字符指针
{
	char *strp=str;
	while(*strp=='0')strp++;
	if(*strp=='.')strp--;
	WriteAddress(add);
	while(*strp)
	{
		Write_data(*strp++);
	}	
}
void LcdAddrStr(uchar add,char str[])	 //在确定地址显示字符串
{
	char *strp=str;
	while(*strp=='0')strp++;
	if(*strp=='.')strp--;
	WriteAddress(add);
	while(*strp)
	{
		Write_data(*strp++);
	}
}
void LcdAddrNum(uchar add,uint x)	  //在确定地址显示整型变量
{   
	uint a[10],b,w=x;
	uchar i=0,j;
	WriteAddress(add);
	while(1)
	{
		b=w/10;
		i++;
		if(b==0)break;
		w=w/10;				
	}
	w=x;
	j=i;
	while(i)
	{
		a[i-1]=w%10;
		w=w/10;
		i--;
	}
	
	while(i!=j)
	{
		Write_data(0x30+a[i]);
		i++;
	}
}
void LcdNum(uint x)	  //显示整型变量
{   
	uint a[10],b,w=x;
	uchar i=0,j;
	while(1)
	{
		b=w/10;
		i++;
		if(b==0)break;
		w=w/10;				
	}
	w=x;
	j=i;
	while(i)
	{
		a[i-1]=w%10;
		w=w/10;
		i--;
	}
	
	while(i!=j)
	{
		Write_data(0x30+a[i]);
		i++;
	}
}
void  LcdAddrNum2(uchar add,unsigned long int x)	  //在确定地址显示长整型变量
{   
	ulint b,w=x;
	uchar i=0,j,a[10];
	WriteAddress(add);
	while(1)
	{
		b=w/10;
		i++;
		if(b==0)break;
		w=w/10;				
	}
	w=x;
	j=i;
	while(i)
	{
		a[i-1]=w%10;
		w=w/10;
		i--;
	}
	
	while(i!=j)
	{
		Write_data(0x30+a[i]);
		i++;
	}
}
void LcdNumum2(unsigned long int x)	  //显示长整型变量
{   
	ulint b,w=x;
	uchar a[10],i=0,j;
	while(1)
	{
		b=w/10;
		i++;
		if(b==0)break;
		w=w/10;				
	}
	w=x;
	j=i;
	while(i)
	{
		a[i-1]=w%10;
		w=w/10;
		i--;
	}	
	while(i!=j)
	{
		Write_data(0x30+a[i]);
		i++;
	}
}