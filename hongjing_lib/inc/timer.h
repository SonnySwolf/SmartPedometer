#ifndef __timer_H
#define __timer_H
#include<config.h>
#define Timer0_Rate     25500       //�ж�Ƶ��
#define Timer0_Reload   (65535 -(FOSC/Timer0_Rate)) //Timer 0 ��װֵ
#endif