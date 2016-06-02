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
sbit SCLK=P0^1;   //λ����1302оƬ�Ľӿڣ�ʱ������˿ڶ�����P1.0����
sbit DATA=P0^2;   //λ����1302оƬ�Ľӿڣ���������˶�����P1.1����
sbit RST=P0^0;    //λ����1302оƬ�Ľӿڣ���λ�˿ڶ�����P1.2����
extern Time PresentTime,InitTime;   //�ֱ𴢴浱ǰʱ�估��ʼ��ʱ��


void Write1302(uchar dat);
void WriteSet1302(uchar Cmd,uchar dat);
uchar Read1302();
uchar ReadSet1302(uchar Cmd);
bit CheckDS1302GO(void);
void ResetTime(uchar second,uchar minute,uchar hour,uchar day,uchar month,uchar year);
void InitDS1302();
void ReadTime();

#endif
/**************�û�����****************
if(CheckDS1302GO())
	{ReadTime();
	printf("%u",(unsigned int)PresentTime.second);}
***************************************/