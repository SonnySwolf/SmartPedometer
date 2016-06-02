#ifndef __Uart_H_
#define __Uart_H_
#include<Config.h>
#include<stdio.h>
#define BAUD 9600L
#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��
#define PARITYBIT NONE_PARITY   //����У��λ
#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3

#define UART_BUF_SIZE 100
extern bit busy1;
extern bit busy2;
extern bit isreceive;
extern bit isreceive2;
extern uchar whichuartsend;
extern uchar whichuartreceive;

extern int uart_rx_cnt;
extern int uart_rx_rp;
extern int uart_rx_wp;
extern int uart2_rx_cnt;
extern int uart2_rx_rp;
extern int uart2_rx_wp;
extern char uart_rx[UART_BUF_SIZE];
extern char uart2_rx[UART_BUF_SIZE];

void InitUart1(void);
void InitUart2(void);
void Uart1SendData(BYTE dat);
void Uart2SendData(BYTE dat);
void Uart1SendStr(char *str);
void Uart2SendStr(char *str);
#endif
/***************�����ж�*******************
ע��printfʹ��ʱҪ���жϣ�printf��%u���(unsigned int)ת�����uchar��������
void Uart1() interrupt 4 using 1
{
	BYTE a;
    if (RI)
    {
        RI = 0;                 //���RIλ
        a = SBUF;              //P0��ʾ��������
		SBUF=a;
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy1 = 0;               //��æ��־
    }
}
void Uart2() interrupt 8 using 1
{
	BYTE a;
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //���S2RIλ
        a = S2BUF;             //P0��ʾ��������
		S2BUF=a;
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //���S2TIλ
        busy2=0;               //��æ��־
    }
}
******************************************/