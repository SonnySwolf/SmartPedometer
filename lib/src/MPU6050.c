#include<MPU6050.h>
//****************************************
// Update to MPU6050 by shinetop
// MCU: STC89C52
// 2012.3.1
// 功能: 显示加速度计和陀螺仪的10位原始数据
//****************************************
// GY-52 MPU3050 IIC测试程序
// 使用单片机STC89C51 
// 晶振：11.0592M
//****************************************



//**************************************
//I2C起始信号
//**************************************
void I2C_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delaynus(5);                 //延时
    SDA = 0;                    //产生下降沿
    Delaynus(5);                 //延时
    SCL = 0;                    //拉低时钟线
}



//**************************************
//I2C停止信号
//**************************************
void I2C_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delaynus(5);                 //延时
    SDA = 1;                    //产生上升沿
    Delaynus(5);                 //延时
}



//**************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delaynus(5);                 //延时
    SCL = 0;                    //拉低时钟线
    Delaynus(5);                 //延时
}



//**************************************
//I2C接收应答信号
//**************************************
bit I2C_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delaynus(5);                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delaynus(5);                 //延时
    return CY;
}



//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(BYTE dat)
{
    BYTE i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delaynus(5);             //延时
        SCL = 0;                //拉低时钟线
        Delaynus(5);             //延时
    }
    I2C_RecvACK();
}



//**************************************
//从I2C总线接收一个字节数据
//**************************************
BYTE I2C_RecvByte()
{
    BYTE i;
    BYTE dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delaynus(5);             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delaynus(5);             //延时
    }
    return dat;
}



//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(BYTE REG_Address,BYTE REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}



//**************************************
//从I2C设备读取一个字节数据
//**************************************
BYTE Single_ReadI2C(BYTE REG_Address)
{
	BYTE REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}



//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}



//**************************************
//合成数据
//**************************************
int GetData(BYTE REG_Address)
{
	BYTE H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}

