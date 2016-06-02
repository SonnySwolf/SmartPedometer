#include<Config.h>
#include<MPU6050.h>
#include<delay.h>
#include<UART.h>
#include"stdio.h"
#include<jiexi.h>
#include<lcd5110.h>
#include"string.h"
#include<./gprs.h>
#define RANGE 4
char code str10[] = "\r\nAT+CSCS=\"GSM\"\r\n"; // 字符集          
char code str20[]="AT+CSMP=17,167,0,0\r\n"; 
char code phone[]="AT+CMGS=\"15865281903\"\r\n";
char sec = 0, min = 0;
unsigned int timerflag=0;
unsigned int step = 0;
bit fallstatus = 0;//0没倒
bit status1 = 0;	//等待取消发送信息
bit iscancel = 0;    //是否按键取消
bit certainsend = 0;  //确认发送信息
bit beepflag = 0;   //响铃标志
bit iscount = 0;    //是否开始计步
bit iskeystatuschanged = 0;//是否改变按键状态
bit enter = 0;//是否确定
char strsend[50] = {0};
sbit beep = P4^5; //0响
sbit key = P0^0;//取消警报
sbit key2 = P1^2;//
uchar keystatus = 0;//0 计步  1 暂停 2 上传
int temp1, temp2;
void GSM_Sent_Text_Message(char *str)
{
	Delaynms(350);         
	printf("%s",str10);                 
	Delaynms(350);
	printf("%s",str20);       
	Delaynms(350);
	printf("%s",phone);          
	Delaynms(350);
	printf("%s",str);                
	Delaynms(450);
	Uart1SendData(0x1A);
	Delaynms(350);
}
void Timer1Init(void)		//1毫秒@8MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xC0;		//设置定时初值
	TH1 = 0xE0;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;		//
}
void Timer0Init(void)		//20毫秒@8MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xEB;		//设置定时初值
	TH0 = 0xCB;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}
void systemInit(void)
{
	InitUart2();
	InitUart1();
	Timer0Init();
	Timer1Init();
	PS = 1;
	InitMPU6050();
	P4M1 &=~BIT5;
	P4M0 |= BIT5;
	lcd5110Init();
}
void beepnms(uint time)
{
	beep = 0;
	Delaynms(time);
	beep = 1;
}
void findstep(void)
{
	static int temppre = 1500, maxpre = 1500, minpre = 1500;
	static unsigned char flag = 0;
	if(temppre < temp1)
	{
		if(flag == 2)
			minpre = temppre;
		temppre = temp1;
		flag = 1; //前一个一直小
	}
	if(temppre>temp1)
	{
		if(flag == 1)
		{
			maxpre = temppre;
			if(maxpre - minpre >2000 && timerflag > 150)
				{
					step ++;
					timerflag = 0;
					beepnms(50);
					lcdClearAny(2,6,13);
					lcdShowDouble(2,7,(double)step,0);
				}
		}
		temppre = temp1;
		flag = 2;						
	}	
}
int smooth_filter(int n)
{
	int xdata a[RANGE]={0};
	int rtn;
	unsigned char q,w;
	for(q=0;q<RANGE-1;q++)		//
		{
			a[q]=a[q+1];
			a[RANGE-1]=n;
		
			for(w=0;w<RANGE;w++)
			{
				rtn+=a[w];
				rtn=rtn/RANGE;
			}	
		}
		return rtn;
}
void interface()
{
	lcdWriteChar16_16(3, 4, 0);//确定
	lcdWriteChar16_16(4, 4, 1);
	lcdWriteChar16_16(0, 0, 4);//计步
	lcdWriteChar16_16(1, 0, 5);
	lcdWriteChar16_16(0, 2, 8);//清零
	lcdWriteChar16_16(1, 2, 9);
	lcdWriteChar16_16(0, 4, 6);//上传
	lcdWriteChar16_16(1, 4, 7);
	lcdWriteChar6_8(5,keystatus * 2,12);//标志
	lcdWriteChar6_8(5,keystatus * 2 + 1,12);
}
void main(void)
{
	uchar i = 0;
	Delaynms(13000);
	systemInit();
	whichuartreceive = 2;
	whichuartsend = 1;
	interface();
	while(1)
	{	
		temp1=abs(GetData(ACCEL_XOUT_H));
		temp2 =smooth_filter(temp1);
		if(temp2 < 3000)
		 	fallstatus = 1;
		else 
			fallstatus = 0;
		if(fallstatus == 0 && keystatus == 0 && enter == 1)findstep();
		if(iskeystatuschanged == 1)
		{
			for(i = 0; i<6; i++)
			lcdClearAny(i,5,6);
			lcdWriteChar6_8(5,keystatus * 2,12);
			lcdWriteChar6_8(5,keystatus * 2 + 1,12);
			Delaynms(500);
			iskeystatuschanged = 0;
		}
		if(enter == 1 && keystatus != 0)
		{
			if(keystatus == 1)
			{	
				step = 0;
				lcdClearAny(2,6,13);
			}
			else if(keystatus == 2)
			{	
				GPRS_SendStep();
				Delaynms(4000);
				lcd5110Init();
				interface();
			}
			enter = 0;
		}
		if(beepflag==1)
		{
			beepnms(100);
		}
		if(certainsend == 1)
		{
			IE2 |= 0x01;	
			Anylize();
			IE2 &=~ 0x01;	
			sprintf(strsend,"HELP!!\r\nLOCATION:N:%s,E:%s.",lati,longi);
			GSM_Sent_Text_Message(strsend);
			GPRS_Send();
			certainsend = 0;
		}
		Delaynms(200);
	}
}
void timer0() interrupt 1 
{
	static uchar num = 0;
	num ++;
	if(num == 50){num = 0;sec++;}
	if(fallstatus == 0){status1 = 0; iscancel = 0;beepflag = 0;certainsend = 0;min = 0;sec = 0;}//未摔倒
	if(fallstatus == 1 && status1 == 0 && iscancel == 0){ sec = 0 ;min = 0;status1 = 1;} //刚摔倒后
	if(fallstatus == 1 && status1 == 1){iscancel = 0;}
	if(status1 ==1 && sec >5 && sec <20 && min ==0) beepflag = 1;//摔倒后时间内未等到取消有报警
	if(status1 ==1 && key == 0) {beepflag = 0;iscancel = 1; status1 = 0;}//摔倒后取消
	if(status1 ==1 && sec == 20 && min ==0 && iscancel == 0) {beepflag = 0;certainsend = 1;}  //摔倒后时间内未等到取消则发送
	
	if(key == 0 && iskeystatuschanged == 0 && status1 == 0)
	{
		enter = 0;
		iskeystatuschanged = 1;
		keystatus ++;
		if(keystatus == 3) keystatus = 0;
	}
	if(key2 == 0)
	{
		enter = 1;
	}
}

void timer1() interrupt 3 using 1
{
	timerflag++;
	if(timerflag >= 30000)
		timerflag = 0;
}
void Uart1() interrupt 4 using 1
{
	ES=0;
    if (RI)
    {
        RI = 0;                 //清除RI位
		if((isreceive==1)&&(uart_rx_cnt<UART_BUF_SIZE))
		{
			
			uart_rx[uart_rx_wp]=SBUF;
			uart_rx_wp=(uart_rx_wp+1)%UART_BUF_SIZE;
			uart_rx_cnt++;
		}
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy1 = 0;               //清忙标志
    }
	ES=1;
}
void Uart2() interrupt 8 using 1
{
	IE2&=~0x01;		  //关闭串口中断
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //清除S2RI位
        if((isreceive2==1)&&(uart2_rx_cnt<UART_BUF_SIZE))
		{
			
			uart2_rx[uart2_rx_wp]=S2BUF;
			uart2_rx_wp=(uart2_rx_wp+1)%UART_BUF_SIZE;
			uart2_rx_cnt++;
		}
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //清除S2TI位
        busy2=0;               //清忙标志
    }
	IE2|=0x01;					//开启串口中断
}