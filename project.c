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


float KalmanGain;//  ����������
float EstimateCovariance;//����Э����
float MeasureCovariance;//����Э����
float EstimateValue;//����ֵ



//******�ǶȲ���************

float Gyro_y;        //Y�������������ݴ�
float Angle_gy;      //�ɽ��ٶȼ������б�Ƕ�
float Accel_x;	     //X����ٶ�ֵ�ݴ�
float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�
float Angle;         //С��������б�Ƕ�
//uchar value;		 //�Ƕ��������Ա��	



//******����������************
		
float code Q_angle=0.001;  
float code Q_gyro=0.003;
float code R_angle=0.5;
float code dt=0.01;	                  //dtΪkalman�˲�������ʱ��;
char  code C_0 = 1;
float xdata Q_bias, Angle_err;
float xdata PCt_0, PCt_1, E;
float xdata K_0, K_1, t_0, t_1;
float xdata Pdot[4] ={0,0,0,0};
float xdata PP[2][2] = { { 1, 0 },{ 0, 1 } };

//*********************************************************
// �������˲�
//*********************************************************

//Kalman�˲���20MHz�Ĵ���ʱ��Լ0.77ms��

void Kalman_Filter(float Accel,float Gyro)		
{
	Angle+=(Gyro - Q_bias) * dt; //�������

	
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	Gyro_y   = Gyro - Q_bias;	 //���ֵ(�������)��΢��=���ٶ�

}



//*********************************************************
// ��Ǽ��㣨�������ںϣ�
//*********************************************************

void Angle_Calcu(void)	 
{
	//------���ٶ�--------------------------

	//��ΧΪ2gʱ�������ϵ��16384 LSB/g
	//�ǶȽ�Сʱ��x=sinx�õ��Ƕȣ����ȣ�, deg = rad*180/3.14
	//��Ϊx>=sinx,�ʳ���1.3�ʵ��Ŵ�

	Accel_x  = GetData(ACCEL_XOUT_H);	  //��ȡX����ٶ�
	Angle_ax = (Accel_x - 1100) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
	Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��,


    //-------���ٶ�-------------------------

	//��ΧΪ2000deg/sʱ�������ϵ��16.4 LSB/(deg/s)

	Gyro_y = GetData(GYRO_YOUT_H);	      //��ֹʱ���ٶ�Y�����Ϊ-30����
	Gyro_y = -(Gyro_y + 30)/16.4;         //ȥ�����ƫ�ƣ�������ٶ�ֵ,����Ϊ������ 
	//Angle_gy = Angle_gy + Gyro_y*0.01;  //���ٶȻ��ֵõ���б�Ƕ�.	

	
	//-------�������˲��ں�-----------------------

	Kalman_Filter(Angle_ax,Gyro_y);       //�������˲��������


	/*//-------�����˲�-----------------------

	//����ԭ����ȡ��ǰ��Ǻͼ��ٶȻ����ǲ�ֵ���зŴ�Ȼ����
    //�����ǽ��ٶȵ��Ӻ��ٻ��֣��Ӷ�ʹ��������Ϊ���ٶȻ�õĽǶ�
	//0.5Ϊ�Ŵ������ɵ��ڲ����ȣ�0.01Ϊϵͳ����10ms	
		
	Angle = Angle + (((Angle_ax-Angle)*0.5 + Gyro_y)*0.01);*/
															  
}  

/////////////////////////////////////////////////
//ƽ���˲�
/////////////////////////////////////////////////
int smooth_filter(int n)
{
	int xdata a[RANGE]={0};
	int rtn;
	unsigned char q,w;
	for(q=0;q<RANGE-1;q++)		//ƽ���˲���Χ
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
        //���㿨��������
        KalmanGain=EstimateCovariance*sqrt(1/(EstimateCovariance*EstimateCovariance+MeasureCovariance*MeasureCovariance));

        //���㱾���˲�����ֵ
        EstimateValue=EstimateValue+KalmanGain * (Measure-EstimateValue);
        
        //���¹���Э����
        EstimateCovariance=sqrt(1-KalmanGain) * EstimateCovariance;
        
        //���²�������
        MeasureCovariance=sqrt(1-KalmanGain) * MeasureCovariance;
        
        //���ع���ֵ
        return EstimateValue;
}



//*********************************************************
//������
//**************************************** *****************
void main()
{ 
	int temp;
	Delaynms(500);		//�ϵ���ʱ	
	SystemInit();
	while(1)
	{
	//x = abs(GetData(ACCEL_XOUT_H)/4);	//��ʾX����ٶ�
	//y = abs(GetData(ACCEL_YOUT_H)/4);	//��ʾY����ٶ�
	//z = abs(GetData(ACCEL_ZOUT_H)/4);	//��ʾZ����ٶ�
	//	Display10BitData(GetData(GYRO_XOUT_H),2,1);	//��ʾX����ٶ�
	//	Display10BitData(GetData(GYRO_YOUT_H),7,1);	//��ʾY����ٶ�
	//	Display10BitData(GetData(GYRO_ZOUT_H),12,1);	//��ʾZ����ٶ�
		//x=188;
		//printf("%d\r\n",x);
		temp=abs(GetData(ACCEL_XOUT_H));
		printf("%d %d;",temp,10*smooth_filter(temp));
		//Angle_Calcu();              //��Ǽ���
		//printf("Angle_ax = %f\r\nGyro_y = %f\r\n\r\n",Angle_ax,Gyro_y);
		Delaynms(100);
	}
}





void Uart1() interrupt 4 using 1
{
	BYTE a;
    if (RI)
    {
        RI = 0;                 //���RIλ
        a = SBUF;              //P0��ʾ��������
		SBUF=a;
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy1 = 0;               //��æ��־
    }
}
/*void Uart2() interrupt 8 using 1
{
	BYTE a;
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //���S2RIλ
        a = S2BUF;             //P0��ʾ��������
		S2BUF=a;
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //���S2TIλ
        busy2=0;               //��æ��־
    }
}
*/