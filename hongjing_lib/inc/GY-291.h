#ifndef _GY291_H_
#define _GY291_H_
#include<config.h>
#include<delay.h>
#define	SlaveAddress   0xA6	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
sbit SCL=P0^0;      //IICʱ�����Ŷ���
sbit SDA=P0^1;      //IIC�������Ŷ���
extern float acce[3]; //3��ļ��ٶ�
void Init_ADXL345(void);    //��ʼ��ADXL345
void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   //����д������
uchar Single_Read_ADXL345(uchar REG_Address);                   //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_ADXL345();                                  //�����Ķ�ȡ�ڲ��Ĵ�������
void ADXL345_Start();
void ADXL345_Stop();
void ADXL345_SendACK(bit ack);
bit  ADXL345_RecvACK();
void ADXL345_SendByte(BYTE dat);
BYTE ADXL345_RecvByte();
void ADXL345_ReadPage();
void ADXL345_WritePage();
void FigureAcce();
#endif

