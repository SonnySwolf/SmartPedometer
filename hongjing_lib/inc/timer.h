#ifndef __timer_H
#define __timer_H
#include<config.h>
#define Timer0_Rate     25500       //中断频率
#define Timer0_Reload   (65535 -(FOSC/Timer0_Rate)) //Timer 0 重装值
#endif