#include<PCA.h>

void ChangePwm0(uchar pwm)        //pwmµ÷ËÙ³ÌÐò    
{	
	CCAPM1&=~(0x02);
    mt2=1;
	CCON = 0;                       //Initial PCA control register
                                    //PCA timer stop running
                                    //Clear CF flag
                                    //Clear all module interrupt flag
    CL = 0;                         //Reset PCA base timer
    CH = 0;
    CMOD = 0x02;                    //Set PCA timer clock source as Fosc/2
                                    //Disable PCA timer overflow interrupt
    CCAP0H = CCAP0L = pwm;         //PWM0 port output 50% duty cycle square wave
    CCAPM0 = 0x42;                  //PCA module-0 work in 8-bit PWM mode and no PCA interrupt
    CR = 1;   
}
