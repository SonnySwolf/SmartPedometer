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
char code str10[] = "\r\nAT+CSCS=\"GSM\"\r\n"; // �ַ���          
char code str20[]="AT+CSMP=17,167,0,0\r\n"; 
char code phone[]="AT+CMGS=\"15865281903\"\r\n";
char sec = 0, min = 0;
unsigned int timerflag=0;
unsigned int step = 0;
bit fallstatus = 0;//0û��
bit status1 = 0;	//�ȴ�ȡ��������Ϣ
bit iscancel = 0;    //�Ƿ񰴼�ȡ��
bit certainsend = 0;  //ȷ�Ϸ�����Ϣ
bit beepflag = 0;   //�����־
bit iscount = 0;    //�Ƿ�ʼ�Ʋ�
bit iskeystatuschanged = 0;//�Ƿ�ı䰴��״̬
bit enter = 0;//�Ƿ�ȷ��
char strsend[50] = {0};
sbit beep = P4^5; //0��
sbit key = P0^0;//ȡ������
sbit key2 = P1^2;//
uchar keystatus = 0;//0 �Ʋ�  1 ��ͣ 2 �ϴ�
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
void Timer1Init(void)		//1����@8MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xC0;		//���ö�ʱ��ֵ
	TH1 = 0xE0;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;		//
}
void Timer0Init(void)		//20����@8MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xEB;		//���ö�ʱ��ֵ
	TH0 = 0xCB;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
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
		flag = 1; //ǰһ��һֱС
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
	lcdWriteChar16_16(3, 4, 0);//ȷ��
	lcdWriteChar16_16(4, 4, 1);
	lcdWriteChar16_16(0, 0, 4);//�Ʋ�
	lcdWriteChar16_16(1, 0, 5);
	lcdWriteChar16_16(0, 2, 8);//����
	lcdWriteChar16_16(1, 2, 9);
	lcdWriteChar16_16(0, 4, 6);//�ϴ�
	lcdWriteChar16_16(1, 4, 7);
	lcdWriteChar6_8(5,keystatus * 2,12);//��־
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
	if(fallstatus == 0){status1 = 0; iscancel = 0;beepflag = 0;certainsend = 0;min = 0;sec = 0;}//δˤ��
	if(fallstatus == 1 && status1 == 0 && iscancel == 0){ sec = 0 ;min = 0;status1 = 1;} //��ˤ����
	if(fallstatus == 1 && status1 == 1){iscancel = 0;}
	if(status1 ==1 && sec >5 && sec <20 && min ==0) beepflag = 1;//ˤ����ʱ����δ�ȵ�ȡ���б���
	if(status1 ==1 && key == 0) {beepflag = 0;iscancel = 1; status1 = 0;}//ˤ����ȡ��
	if(status1 ==1 && sec == 20 && min ==0 && iscancel == 0) {beepflag = 0;certainsend = 1;}  //ˤ����ʱ����δ�ȵ�ȡ������
	
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
        RI = 0;                 //���RIλ
		if((isreceive==1)&&(uart_rx_cnt<UART_BUF_SIZE))
		{
			
			uart_rx[uart_rx_wp]=SBUF;
			uart_rx_wp=(uart_rx_wp+1)%UART_BUF_SIZE;
			uart_rx_cnt++;
		}
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy1 = 0;               //��æ��־
    }
	ES=1;
}
void Uart2() interrupt 8 using 1
{
	IE2&=~0x01;		  //�رմ����ж�
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //���S2RIλ
        if((isreceive2==1)&&(uart2_rx_cnt<UART_BUF_SIZE))
		{
			
			uart2_rx[uart2_rx_wp]=S2BUF;
			uart2_rx_wp=(uart2_rx_wp+1)%UART_BUF_SIZE;
			uart2_rx_cnt++;
		}
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //���S2TIλ
        busy2=0;               //��æ��־
    }
	IE2|=0x01;					//���������ж�
}