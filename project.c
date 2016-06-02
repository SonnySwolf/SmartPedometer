#include <STC15W4K58S4.h>
#include <delay.h>
#include <MPU6050.h>
#include <Uart.h>
#include <Config.h>
#include <stdio.h>

#define RANGE 5

void SystemInit()
{
	InitUart1();
	Delaynms(150);
	//InitUart2();
	Delaynms(150);
	InitMPU6050();
	Delaynms(150);
}


float KalmanGain;//  卡尔曼增益
float EstimateCovariance;//估计协方差
float MeasureCovariance;//测量协方差
float EstimateValue;//估计值



//******角度参数************

float Gyro_y;        //Y轴陀螺仪数据暂存
float Angle_gy;      //由角速度计算的倾斜角度
float Accel_x;	     //X轴加速度值暂存
float Angle_ax;      //由加速度计算的倾斜角度
float Angle;         //小车最终倾斜角度
//uchar value;		 //角度正负极性标记	



//******卡尔曼参数************
		
float code Q_angle=0.001;  
float code Q_gyro=0.003;
float code R_angle=0.5;
float code dt=0.01;	                  //dt为kalman滤波器采样时间;
char  code C_0 = 1;
float xdata Q_bias, Angle_err;
float xdata PCt_0, PCt_1, E;
float xdata K_0, K_1, t_0, t_1;
float xdata Pdot[4] ={0,0,0,0};
float xdata PP[2][2] = { { 1, 0 },{ 0, 1 } };

//*********************************************************
// 卡尔曼滤波
//*********************************************************

//Kalman滤波，20MHz的处理时间约0.77ms；

void Kalman_Filter(float Accel,float Gyro)		
{
	Angle+=(Gyro - Q_bias) * dt; //先验估计

	
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	Gyro_y   = Gyro - Q_bias;	 //输出值(后验估计)的微分=角速度

}



//*********************************************************
// 倾角计算（卡尔曼融合）
//*********************************************************

void Angle_Calcu(void)	 
{
	//------加速度--------------------------

	//范围为2g时，换算关系：16384 LSB/g
	//角度较小时，x=sinx得到角度（弧度）, deg = rad*180/3.14
	//因为x>=sinx,故乘以1.3适当放大

	Accel_x  = GetData(ACCEL_XOUT_H);	  //读取X轴加速度
	Angle_ax = (Accel_x - 1100) /16384;   //去除零点偏移,计算得到角度（弧度）
	Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度,


    //-------角速度-------------------------

	//范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)

	Gyro_y = GetData(GYRO_YOUT_H);	      //静止时角速度Y轴输出为-30左右
	Gyro_y = -(Gyro_y + 30)/16.4;         //去除零点偏移，计算角速度值,负号为方向处理 
	//Angle_gy = Angle_gy + Gyro_y*0.01;  //角速度积分得到倾斜角度.	

	
	//-------卡尔曼滤波融合-----------------------

	Kalman_Filter(Angle_ax,Gyro_y);       //卡尔曼滤波计算倾角


	/*//-------互补滤波-----------------------

	//补偿原理是取当前倾角和加速度获得倾角差值进行放大，然后与
    //陀螺仪角速度叠加后再积分，从而使倾角最跟踪为加速度获得的角度
	//0.5为放大倍数，可调节补偿度；0.01为系统周期10ms	
		
	Angle = Angle + (((Angle_ax-Angle)*0.5 + Gyro_y)*0.01);*/
															  
}  

/////////////////////////////////////////////////
//平滑滤波
/////////////////////////////////////////////////
int smooth_filter(int n)
{
	int xdata a[RANGE]={0};
	int rtn;
	unsigned char q,w;
	for(q=0;q<RANGE-1;q++)		//平滑滤波范围
		{
			a[q]=a[q+1];
			a[RANGE-1]=n;
		
			for(w=0;w<RANGE;w++)
			{
				rtn+=a[w];
				rtn=rtn/RANGE;
			}
			
		}
		return rtn;
}


void KalmanFilterInit(void)
{
        EstimateValue = 0;
        EstimateCovariance = 1;
        MeasureCovariance = 2;
}

float KalmanFilter(float Measure)
{
        //计算卡尔曼增益
        KalmanGain=EstimateCovariance*sqrt(1/(EstimateCovariance*EstimateCovariance+MeasureCovariance*MeasureCovariance));

        //计算本次滤波估计值
        EstimateValue=EstimateValue+KalmanGain * (Measure-EstimateValue);
        
        //更新估计协方差
        EstimateCovariance=sqrt(1-KalmanGain) * EstimateCovariance;
        
        //更新测量方差
        MeasureCovariance=sqrt(1-KalmanGain) * MeasureCovariance;
        
        //返回估计值
        return EstimateValue;
}



//*********************************************************
//主程序
//**************************************** *****************
void main()
{ 
	int temp;
	Delaynms(500);		//上电延时	
	SystemInit();
	while(1)
	{
	//x = abs(GetData(ACCEL_XOUT_H)/4);	//显示X轴加速度
	//y = abs(GetData(ACCEL_YOUT_H)/4);	//显示Y轴加速度
	//z = abs(GetData(ACCEL_ZOUT_H)/4);	//显示Z轴加速度
	//	Display10BitData(GetData(GYRO_XOUT_H),2,1);	//显示X轴角速度
	//	Display10BitData(GetData(GYRO_YOUT_H),7,1);	//显示Y轴角速度
	//	Display10BitData(GetData(GYRO_ZOUT_H),12,1);	//显示Z轴角速度
		//x=188;
		//printf("%d\r\n",x);
		temp=abs(GetData(ACCEL_XOUT_H));
		printf("%d %d;",temp,10*smooth_filter(temp));
		//Angle_Calcu();              //倾角计算
		//printf("Angle_ax = %f\r\nGyro_y = %f\r\n\r\n",Angle_ax,Gyro_y);
		Delaynms(100);
	}
}





void Uart1() interrupt 4 using 1
{
	BYTE a;
    if (RI)
    {
        RI = 0;                 //清除RI位
        a = SBUF;              //P0显示串口数据
		SBUF=a;
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy1 = 0;               //清忙标志
    }
}
/*void Uart2() interrupt 8 using 1
{
	BYTE a;
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //清除S2RI位
        a = S2BUF;             //P0显示串口数据
		S2BUF=a;
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //清除S2TI位
        busy2=0;               //清忙标志
    }
}
*/