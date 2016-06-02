#include<Uart.h>
bit busy1;
bit busy2;
uchar whichuartsend=1;//Ĭ��ʹ�ô���1����
uchar whichuartreceive=1;//Ĭ��ʹ�ô���1����
bit isreceive=0; 
int uart_rx_cnt=0;
int uart_rx_rp=0;
int uart_rx_wp=0;
char uart_rx[UART_BUF_SIZE];

bit isreceive2=0;    
int uart2_rx_cnt=0;
int uart2_rx_rp=0;
int uart2_rx_wp=0;
char uart2_rx[UART_BUF_SIZE];

void InitUart1(void)
{
	#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8λ�ɱ䲨����
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9λ�ɱ䲨����,У��λ��ʼΪ1
	#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0
	#endif	
	T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
	ES=1;
	EA=1;	
}
void InitUart2(void)
{
	#if (PARITYBIT == NONE_PARITY)
    S2CON = 0x10;               //8λ�ɱ䲨����
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S2CON = 0xda;               //9λ�ɱ䲨����,У��λ��ʼΪ1
	#elif (PARITYBIT == SPACE_PARITY)
    S2CON = 0xd2;               //9λ�ɱ䲨����,У��λ��ʼΪ0
	#endif				//8λ�ɱ䲨����
	T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR |= 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
	IE2|=0x01;					 //���������ж�
    EA=1;
}
void Uart1SendData(BYTE dat)
{
	while (busy1);   
	#if(PARITYBIT != NONE_PARITY)
	{	
		ACC = dat;                  //��ȡУ��λP (PSW.0)
	    if (P)                      //����P������У��λ
	    {
		#if (PARITYBIT == ODD_PARITY)
	        TB8 = 0;                //����У��λΪ0
		#elif (PARITYBIT == EVEN_PARITY)
	        TB8 = 1;                //����У��λΪ1
		#endif
	    }
	    else
	    {
		#if (PARITYBIT == ODD_PARITY)
	        TB8 = 1;                //����У��λΪ1
		#elif (PARITYBIT == EVEN_PARITY)
	        TB8 = 0;                //����У��λΪ0
		#endif
	    }
	}
	#endif
	busy1=1;	
    SBUF = dat;                 //д���ݵ�UART���ݼĴ���
}
void Uart2SendData(BYTE dat)
{
    while (busy2);               //�ȴ�ǰ������ݷ������
	#if(PARITYBIT != NONE_PARITY)
	{
		ACC = dat;                  //��ȡУ��λP (PSW.0)
		if (P)                      //����P������У��λ
	    {
			#if (PARITYBIT == ODD_PARITY)
	        S2CON &= ~S2TB8;        //����У��λΪ0
			#elif (PARITYBIT == EVEN_PARITY)
	        S2CON |= S2TB8;         //����У��λΪ1
			#endif
	   	}
	    else
	    {
			#if (PARITYBIT == ODD_PARITY)
	        S2CON |= S2TB8;         //����У��λΪ1
			#elif (PARITYBIT == EVEN_PARITY)
	        S2CON &= ~S2TB8;        //����У��λΪ0
			#endif
	    }
	}
	#endif
    busy2 = 1;
	S2BUF = dat;                 //д���ݵ�UART���ݼĴ���
}
char putchar(char dat)
{
	if(whichuartsend==1)
		Uart1SendData(dat);	
	if(whichuartsend==2)
		Uart2SendData(dat);
    return dat;
}
char _getkey(void)  
{  
    uchar ch;  
    //Wait until a character is available: 
	if(whichuartreceive==1)  
	    {while(uart_rx_cnt == 0);  
	    ES = 0;  
	    ch = uart_rx[uart_rx_rp];  
	    uart_rx_rp = (uart_rx_rp + 1) % UART_BUF_SIZE;  
	    uart_rx_cnt--;  
	    ES = 1;  
	    return ch;
		}
	else
		{
		while(uart2_rx_cnt == 0);  
	    IE2&=~0x01;  
	    ch = uart2_rx[uart2_rx_rp];  
	    uart2_rx_rp = (uart2_rx_rp + 1) % UART_BUF_SIZE;  
	    uart2_rx_cnt--;  
	    IE2|=0x01;  
	    return ch;	
		} 
} 
void Uart1SendStr(char *str)
{
	while (*str)                  //����ַ���������־
    {
        Uart1SendData(*str++);         //���͵�ǰ�ַ�
    }
}
void Uart2SendStr(char *str)
{
   while (*str)                  //����ַ���������־
    {
        Uart2SendData(*str++);         //���͵�ǰ�ַ�
    }
}


