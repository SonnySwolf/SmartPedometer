#ifndef __Config_H_
#define __Config_H_
#include<STC15W4K58S4.h>
//#define FOSC 8000000L
#define FOSC 11059200L
#define uchar unsigned char
#define uint unsigned int
#define ulint unsigned long int
#define u8 unsigned char
#define u32 unsigned long int
#define False 0
#define True !False
typedef unsigned char BYTE;
typedef unsigned int WORD;
#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 16
#define BIT5 32
#define BIT6 64
#define BIT7 128

//****************************************
// ���嵥Ƭ���˿�
//****************************************
sbit    SCL=P1^0;			//IICʱ�����Ŷ���
sbit    SDA=P1^1;			//IIC�������Ŷ���

#endif