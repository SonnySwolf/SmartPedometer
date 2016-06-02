#ifndef __DS1302_H_
#define __DS1302_H_
#include<delay.h>
#include<Config.h>
typedef struct 
{
	uchar second;
	uchar minute;
	uchar hour;
	uchar day;
	uchar month;
	uchar year;	
}Time;
sbit SCLK=P0^1;   //位定义1302芯片的接口，时钟输出端口定义在P1.0引脚
sbit DATA=P0^2;   //位定义1302芯片的接口，数据输出端定义在P1.1引脚
sbit RST=P0^0;    //位定义1302芯片的接口，复位端口定义在P1.2引脚
extern Time PresentTime,InitTime;   //分别储存当前时间及初始化时间


void Write1302(uchar dat);
void WriteSet1302(uchar Cmd,uchar dat);
uchar Read1302();
uchar ReadSet1302(uchar Cmd);
bit CheckDS1302GO(void);
void ResetTime(uchar second,uchar minute,uchar hour,uchar day,uchar month,uchar year);
void InitDS1302();
void ReadTime();

#endif
/**************用户调用****************
if(CheckDS1302GO())
	{ReadTime();
	printf("%u",(unsigned int)PresentTime.second);}
***************************************/