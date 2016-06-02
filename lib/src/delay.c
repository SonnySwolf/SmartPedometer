#include<delay.h>
void Delay10us()		//@8.000MHz
{
	uchar i;
	_nop_();
	_nop_();
	i = 17;
	while (--i);
}
void Delay10nus(uchar i)
{
	 while(i--)
		Delay1ms();
}
void Delay1ms()		//@8.000MHz
{
	uchar i,j;
	i = 8;
	j = 197;
	do
	{
		while (--j);
	} while (--i);
}
void Delaynus(uchar a)  //a<65
{
    uchar i;
	i = 2*a-1;
	while (--i);
}
void Delaynms(uint a)
{	uint i=a;
	while(i--)
		Delay1ms();
}
