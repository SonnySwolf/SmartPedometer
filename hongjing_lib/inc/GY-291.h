#ifndef _GY291_H_
#define _GY291_H_
#include<config.h>
#include<delay.h>
#define	SlaveAddress   0xA6	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
sbit SCL=P0^0;      //IIC时钟引脚定义
sbit SDA=P0^1;      //IIC数据引脚定义
extern float acce[3]; //3轴的加速度
void Init_ADXL345(void);    //初始化ADXL345
void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   //单个写入数据
uchar Single_Read_ADXL345(uchar REG_Address);                   //单个读取内部寄存器数据
void  Multiple_Read_ADXL345();                                  //连续的读取内部寄存器数据
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

