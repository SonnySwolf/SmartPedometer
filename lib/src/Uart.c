#include<Uart.h>
bit busy1;
bit busy2;
uchar whichuartsend=1;//默认使用串口1发送
uchar whichuartreceive=1;//默认使用串口1接收
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
    SCON = 0x50;                //8位可变波特率
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9位可变波特率,校验位初始为1
	#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9位可变波特率,校验位初始为0
	#endif	
	T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
	ES=1;
	EA=1;	
}
void InitUart2(void)
{
	#if (PARITYBIT == NONE_PARITY)
    S2CON = 0x10;               //8位可变波特率
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S2CON = 0xda;               //9位可变波特率,校验位初始为1
	#elif (PARITYBIT == SPACE_PARITY)
    S2CON = 0xd2;               //9位可变波特率,校验位初始为0
	#endif				//8位可变波特率
	T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR |= 0x14;                //T2为1T模式, 并启动定时器2
	IE2|=0x01;					 //开启串口中断
    EA=1;
}
void Uart1SendData(BYTE dat)
{
	while (busy1);   
	#if(PARITYBIT != NONE_PARITY)
	{	
		ACC = dat;                  //获取校验位P (PSW.0)
	    if (P)                      //根据P来设置校验位
	    {
		#if (PARITYBIT == ODD_PARITY)
	        TB8 = 0;                //设置校验位为0
		#elif (PARITYBIT == EVEN_PARITY)
	        TB8 = 1;                //设置校验位为1
		#endif
	    }
	    else
	    {
		#if (PARITYBIT == ODD_PARITY)
	        TB8 = 1;                //设置校验位为1
		#elif (PARITYBIT == EVEN_PARITY)
	        TB8 = 0;                //设置校验位为0
		#endif
	    }
	}
	#endif
	busy1=1;	
    SBUF = dat;                 //写数据到UART数据寄存器
}
void Uart2SendData(BYTE dat)
{
    while (busy2);               //等待前面的数据发送完成
	#if(PARITYBIT != NONE_PARITY)
	{
		ACC = dat;                  //获取校验位P (PSW.0)
		if (P)                      //根据P来设置校验位
	    {
			#if (PARITYBIT == ODD_PARITY)
	        S2CON &= ~S2TB8;        //设置校验位为0
			#elif (PARITYBIT == EVEN_PARITY)
	        S2CON |= S2TB8;         //设置校验位为1
			#endif
	   	}
	    else
	    {
			#if (PARITYBIT == ODD_PARITY)
	        S2CON |= S2TB8;         //设置校验位为1
			#elif (PARITYBIT == EVEN_PARITY)
	        S2CON &= ~S2TB8;        //设置校验位为0
			#endif
	    }
	}
	#endif
    busy2 = 1;
	S2BUF = dat;                 //写数据到UART数据寄存器
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
	while (*str)                  //检测字符串结束标志
    {
        Uart1SendData(*str++);         //发送当前字符
    }
}
void Uart2SendStr(char *str)
{
   while (*str)                  //检测字符串结束标志
    {
        Uart2SendData(*str++);         //发送当前字符
    }
}


