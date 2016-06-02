#ifndef __Gprs_H_
#define __Gprs_H_
#include<UART.h>
#include<Delay.h>
#include"string.h"
extern char strsend[50];
extern unsigned int step;
char code *str1 = "AT+CDNSORIP=1\r\n";
char code *str2 = "AT+CIPHEAD=1\r\n";
char code *str3 = "AT+CIPSTART=\"TCP\",\"api.yeelink.net\",\"80\"\r\n";
char code *str4 = "AT+CIPSEND\r\n"; //进入编辑状态
char code *str5 = "POST /v1.0/device/18065/sensor/35755/datapoints HTTP/1.1\r\nU-ApiKey:5b26d553d08c0cee6eebc9b0bcae1591\r\nHost: api.yeelink.net\r\nContent-Length:";
char code *str6 = "\r\nConnection: close\r\n\r\n{\"value\": {\"lat\":";
char code *str8 = "\r\nAT+CIPCLOSE\r\n";
char code *str9 = "AT+CIPSHUT\r\n";

char code *str3x = "AT+CIPSTART=\"TCP\",\"www.lewei50.com\",\"80\"\r\n";
char code *str5x = "POST /api/V1/gateway/UpdateSensors/01 HTTP/1.1\r\nuserkey:6ff95182280b468f9998fd1a9239e9a6\r\nHost: www.lewei50.com\r\nContent-Length:";
char code *str6x = "\r\nConnection: Keep-Alive\r\n\r\n[";
char code *str7x = "]";


int stepsensor(char* str,uint value,bit flag)	//0:返回字符个数;1：打印
{
	uchar valuenum = 0,allnum = 0;
	if(flag == 0)
	{	if(value <10)
			valuenum = 1;
		if(value >=10 && value <100)
			valuenum = 2;
		if(value >100)
			valuenum = 3;				
		allnum = 9 + (uchar)strlen(str) + 11 + valuenum + 2 ; 
		return allnum; 
	}
	printf("{\"Name\":\"%s\",\"Value\":\"%d\"}",str,value);
	return 0;
}

void sendsteptoserver(uint para1)
{
	printf("%s",str4);
	Delaynms(50);
	printf("%s",str5x);
	printf("%d%s",stepsensor("hongjing", para1, 0) + 2, str6x);

	stepsensor("hongjing",para1,1);
	printf("%s",str7x);

	Uart1SendData(0x1a);
	printf("\r\n");
}

int gpssensor(bit flag)	//0：返回字符个数；1：打印
{
	if(flag ==0)
	return 17+strlen(lati) + 7 + strlen(longi) + 12;
	printf("%s,\"lng\":%s,\"speed\":0}}",lati,longi);
	return 0;
}
void opengprgps()
{
	printf("%s",str1);
	Delaynms(50);
	printf("%s",str2);
	Delaynms(50);
	printf("%s",str3);
	Delaynms(50);
}
void opengprstep()
{
	printf("%s",str1);
	Delaynms(50);
	printf("%s",str2);
	Delaynms(50);
	printf("%s",str3x);
	Delaynms(50);
}
void sendgpstoserver()
{
	printf("%s",str4);
	Delaynms(50);
	printf("%s",str5);
	printf("%d%s",gpssensor(0), str6);
	gpssensor(1);
	Uart1SendData(0x1a);
	printf("\r\n");
}

void GPRS_Send()
{
	printf("%s",str8);
	Delaynms(50);
	printf("%s",str9);
	Delaynms(1000);
	opengprgps();
	Delaynms(4000);
	sendgpstoserver();
}
void GPRS_SendStep()
{
	printf("%s",str8);
	Delaynms(50);
	printf("%s",str9);
	Delaynms(1000);
	opengprstep();
	Delaynms(4000);
	sendsteptoserver(step);
}
#endif