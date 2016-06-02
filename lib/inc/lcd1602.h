#ifndef __1602lcd_H_
#define __1602lcd_H_
#include<config.h>
#include<delay.h>
#define  DatReg  P0
#define  CtrReg  P2
sbit RS=CtrReg^0;
sbit RW=CtrReg^1;
sbit EN=CtrReg^2;
sbit BF=DatReg^7;       //定义lcd功能管脚

bit BusyTest(void);  //检测lcd忙碌
void Write_com(uchar com);  //lcd写命令
void WriteAddress(uchar x); //lcd改变地址 00~15 40~55
void Write_data(uchar dat); //lcd写数据
void InitLcd1602();	 //lcd初始化
void LcdAddrStrp(uchar add,char *str); //在确定地址显示字符串,形参为字符指针
void LcdAddrStr(uchar add,char str[]); //在确定地址显示字符串
void LcdAddrNum(uchar add,uint x);	  //在确定地址显示整型变量
void LcdNum(uint x);	  //显示整型变量
void LcdAddrNum2(uchar add,unsigned long int x);	  //在确定地址显示长整型变量
void LcdNum2(unsigned long int x);	  //显示长整型变量
#endif


