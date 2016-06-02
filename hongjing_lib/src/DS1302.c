#include<DS1302.h>
Time PresentTime,InitTime;
uchar ReadValue;   //储存从1302读取的数据
/*****************************************************
函数功能：向1302写一个字节数据
入口参数：x
***************************************************/ 
void Write1302(uchar dat)
{
  uchar i; 
  SCLK=0;            //拉低SCLK，为脉冲上升沿写入数据做好准备
  Delaynus(5);       //稍微等待，使硬件做好准备
  for(i=0;i<8;i++)      //连续写8个二进制位数据
    {
	     DATA=dat&0x01;    //取出dat的第0位数据写入1302  低位在前，高位在后
		 Delaynus(5);       //稍微等待，使硬件做好准备
		 SCLK=1;           //上升沿写入数据
		 Delaynus(5);      //稍微等待，使硬件做好准备
		 SCLK=0;           //重新拉低SCLK，形成脉冲
		 dat>>=1;          //将dat的各数据位右移1位，准备写入下一个数据位
	  }
	
 }
/*****************************************************
函数功能：根据命令字，向1302写一个字节数据
入口参数：Cmd，储存命令字；dat，储存待写的数据
***************************************************/ 
void WriteSet1302(uchar Cmd,uchar dat)
 {	  
        RST=0;           //禁止数据传递
        SCLK=0;          //确保写数居前SCLK被拉低
	    RST=1;           //启动数据传输
	    Delaynus(5);     //稍微等待，使硬件做好准备
	    Write1302(Cmd);  //写入命令字
	    Write1302(dat);  //写数据
	    SCLK=1;          //将时钟电平置于高电平状态
	    RST=0;           //禁止数据传递
 }
/*****************************************************
函数功能：从1302读一个字节数据
入口参数：x
***************************************************/ 
uchar Read1302(void)
 {
    uchar i,dat;
	Delaynus(5);       //稍微等待，使硬件做好准备
	for(i=0;i<8;i++)   //连续读8个二进制位数据
	 {	 dat>>=1;
		 if(DATA==1)    //如果读出的数据是1
		 dat|=0x80;    //将1取出，写在dat的最高位
		 SCLK=1;       //将SCLK置于高电平，为下降沿读出
		 Delaynus(5);  //稍微等待
		 SCLK=0;       //拉低SCLK，形成脉冲下降沿
		 Delaynus(5);  //稍微等待
	  }	 
  return dat;        //将读出的数据返回
}  
/*****************************************************
函数功能：根据命令字，从1302读取一个字节数据
入口参数：Cmd
***************************************************/ 
uchar  ReadSet1302(uchar Cmd)
 {
  uchar dat;
  RST=0;                 //拉低RST
  SCLK=0;                //确保写数居前SCLK被拉低
  RST=1;                 //启动数据传输
  Write1302(Cmd);       //写入命令字
  dat=Read1302();       //读出数据
  SCLK=1;              //将时钟电平置于已知状态
  RST=0;               //禁止数据传递
  return dat;          //将读出的数据返回
}

/*****************************************************
函数功能： 1302进行时间设置
***************************************************/
void ResetTime(uchar second,uchar minute,uchar hour,uchar day,uchar month,uchar year)
{
	InitTime.second=second;
	InitTime.minute=minute;
	InitTime.hour=hour;
	InitTime.day=day;
	InitTime.month=month;
	InitTime.year=year;
	WriteSet1302(0x8E,0x00);                 //根据写状态寄存器命令字，写入不保护指令 
   	WriteSet1302(0x80,((InitTime.second/10)<<4|(InitTime.second%10)));   //根据写秒寄存器命令字，写入秒的初始值
	WriteSet1302(0x82,((InitTime.minute/10)<<4|(InitTime.minute%10)));   //根据写分寄存器命令字，写入分的初始值
	WriteSet1302(0x84,((InitTime.hour/10)<<4|(InitTime.hour%10))); //根据写小时寄存器命令字，写入小时的初始值
	WriteSet1302(0x86,((InitTime.day/10)<<4|(InitTime.day%10))); //根据写日寄存器命令字，写入日的初始值
	WriteSet1302(0x88,((InitTime.month/10)<<4|(InitTime.month%10))); //根据写月寄存器命令字，写入月的初始值
	WriteSet1302(0x8c,((InitTime.year/10)<<4|(InitTime.year%10)));  //根据写年寄存器命令字，写入年的初始值
	WriteSet1302(0x90,0xa5);                //打开充电功能 选择2K电阻充电方式
	WriteSet1302(0x8E,0x80);			   //根据写状态寄存器命令字，写入保护指令
}
/*****************************************************
函数功能： 检查1302是否已在运行
***************************************************/ 
bit CheckDS1302GO(void)
{	
  uchar flag;
  flag= ReadSet1302(0x81);
   if(flag&0x80)		 //判断时钟芯片是否关闭	
   	   return 0;
	else 
		return 1;
}
void ReadTime(void)
{
	  ReadValue = ReadSet1302(0x81);   //从秒寄存器读数据
     PresentTime.second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//将读出数据转化

	  ReadValue = ReadSet1302(0x83);  //从分寄存器读
     PresentTime.minute=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

     ReadValue = ReadSet1302(0x85);  //从分寄存器读
     PresentTime.hour=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

	 ReadValue = ReadSet1302(0x87);  //从分寄存器读
     PresentTime.day=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

	 ReadValue = ReadSet1302(0x89);  //从分寄存器读
     PresentTime.month=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

	 ReadValue = ReadSet1302(0x8d);  //从分寄存器读
     PresentTime.year=((ReadValue&0xf0)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
}
