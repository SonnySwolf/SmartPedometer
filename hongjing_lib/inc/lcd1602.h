#ifndef __1602lcd_H_
#define __1602lcd_H_
#include<config.h>
#include<delay.h>
#define  DatReg  P0
#define  CtrReg  P2
sbit RS=CtrReg^0;
sbit RW=CtrReg^1;
sbit EN=CtrReg^2;
sbit BF=DatReg^7;       //����lcd���ܹܽ�

bit BusyTest(void);  //���lcdæµ
void Write_com(uchar com);  //lcdд����
void WriteAddress(uchar x); //lcd�ı��ַ 00~15 40~55
void Write_data(uchar dat); //lcdд����
void InitLcd1602();	 //lcd��ʼ��
void LcdAddrStrp(uchar add,char *str); //��ȷ����ַ��ʾ�ַ���,�β�Ϊ�ַ�ָ��
void LcdAddrStr(uchar add,char str[]); //��ȷ����ַ��ʾ�ַ���
void LcdAddrNum(uchar add,uint x);	  //��ȷ����ַ��ʾ���ͱ���
void LcdNum(uint x);	  //��ʾ���ͱ���
void LcdAddrNum2(uchar add,unsigned long int x);	  //��ȷ����ַ��ʾ�����ͱ���
void LcdNum2(unsigned long int x);	  //��ʾ�����ͱ���
#endif


