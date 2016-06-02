#include<DS1302.h>
Time PresentTime,InitTime;
uchar ReadValue;   //�����1302��ȡ������
/*****************************************************
�������ܣ���1302дһ���ֽ�����
��ڲ�����x
***************************************************/ 
void Write1302(uchar dat)
{
  uchar i; 
  SCLK=0;            //����SCLK��Ϊ����������д����������׼��
  Delaynus(5);       //��΢�ȴ���ʹӲ������׼��
  for(i=0;i<8;i++)      //����д8��������λ����
    {
	     DATA=dat&0x01;    //ȡ��dat�ĵ�0λ����д��1302  ��λ��ǰ����λ�ں�
		 Delaynus(5);       //��΢�ȴ���ʹӲ������׼��
		 SCLK=1;           //������д������
		 Delaynus(5);      //��΢�ȴ���ʹӲ������׼��
		 SCLK=0;           //��������SCLK���γ�����
		 dat>>=1;          //��dat�ĸ�����λ����1λ��׼��д����һ������λ
	  }
	
 }
/*****************************************************
�������ܣ����������֣���1302дһ���ֽ�����
��ڲ�����Cmd�����������֣�dat�������д������
***************************************************/ 
void WriteSet1302(uchar Cmd,uchar dat)
 {	  
        RST=0;           //��ֹ���ݴ���
        SCLK=0;          //ȷ��д����ǰSCLK������
	    RST=1;           //�������ݴ���
	    Delaynus(5);     //��΢�ȴ���ʹӲ������׼��
	    Write1302(Cmd);  //д��������
	    Write1302(dat);  //д����
	    SCLK=1;          //��ʱ�ӵ�ƽ���ڸߵ�ƽ״̬
	    RST=0;           //��ֹ���ݴ���
 }
/*****************************************************
�������ܣ���1302��һ���ֽ�����
��ڲ�����x
***************************************************/ 
uchar Read1302(void)
 {
    uchar i,dat;
	Delaynus(5);       //��΢�ȴ���ʹӲ������׼��
	for(i=0;i<8;i++)   //������8��������λ����
	 {	 dat>>=1;
		 if(DATA==1)    //���������������1
		 dat|=0x80;    //��1ȡ����д��dat�����λ
		 SCLK=1;       //��SCLK���ڸߵ�ƽ��Ϊ�½��ض���
		 Delaynus(5);  //��΢�ȴ�
		 SCLK=0;       //����SCLK���γ������½���
		 Delaynus(5);  //��΢�ȴ�
	  }	 
  return dat;        //�����������ݷ���
}  
/*****************************************************
�������ܣ����������֣���1302��ȡһ���ֽ�����
��ڲ�����Cmd
***************************************************/ 
uchar  ReadSet1302(uchar Cmd)
 {
  uchar dat;
  RST=0;                 //����RST
  SCLK=0;                //ȷ��д����ǰSCLK������
  RST=1;                 //�������ݴ���
  Write1302(Cmd);       //д��������
  dat=Read1302();       //��������
  SCLK=1;              //��ʱ�ӵ�ƽ������֪״̬
  RST=0;               //��ֹ���ݴ���
  return dat;          //�����������ݷ���
}

/*****************************************************
�������ܣ� 1302����ʱ������
***************************************************/
void ResetTime(uchar second,uchar minute,uchar hour,uchar day,uchar month,uchar year)
{
	InitTime.second=second;
	InitTime.minute=minute;
	InitTime.hour=hour;
	InitTime.day=day;
	InitTime.month=month;
	InitTime.year=year;
	WriteSet1302(0x8E,0x00);                 //����д״̬�Ĵ��������֣�д�벻����ָ�� 
   	WriteSet1302(0x80,((InitTime.second/10)<<4|(InitTime.second%10)));   //����д��Ĵ��������֣�д����ĳ�ʼֵ
	WriteSet1302(0x82,((InitTime.minute/10)<<4|(InitTime.minute%10)));   //����д�ּĴ��������֣�д��ֵĳ�ʼֵ
	WriteSet1302(0x84,((InitTime.hour/10)<<4|(InitTime.hour%10))); //����дСʱ�Ĵ��������֣�д��Сʱ�ĳ�ʼֵ
	WriteSet1302(0x86,((InitTime.day/10)<<4|(InitTime.day%10))); //����д�ռĴ��������֣�д���յĳ�ʼֵ
	WriteSet1302(0x88,((InitTime.month/10)<<4|(InitTime.month%10))); //����д�¼Ĵ��������֣�д���µĳ�ʼֵ
	WriteSet1302(0x8c,((InitTime.year/10)<<4|(InitTime.year%10)));  //����д��Ĵ��������֣�д����ĳ�ʼֵ
	WriteSet1302(0x90,0xa5);                //�򿪳�繦�� ѡ��2K�����緽ʽ
	WriteSet1302(0x8E,0x80);			   //����д״̬�Ĵ��������֣�д�뱣��ָ��
}
/*****************************************************
�������ܣ� ���1302�Ƿ���������
***************************************************/ 
bit CheckDS1302GO(void)
{	
  uchar flag;
  flag= ReadSet1302(0x81);
   if(flag&0x80)		 //�ж�ʱ��оƬ�Ƿ�ر�	
   	   return 0;
	else 
		return 1;
}
void ReadTime(void)
{
	  ReadValue = ReadSet1302(0x81);   //����Ĵ���������
     PresentTime.second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//����������ת��

	  ReadValue = ReadSet1302(0x83);  //�ӷּĴ�����
     PresentTime.minute=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

     ReadValue = ReadSet1302(0x85);  //�ӷּĴ�����
     PresentTime.hour=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

	 ReadValue = ReadSet1302(0x87);  //�ӷּĴ�����
     PresentTime.day=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

	 ReadValue = ReadSet1302(0x89);  //�ӷּĴ�����
     PresentTime.month=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

	 ReadValue = ReadSet1302(0x8d);  //�ӷּĴ�����
     PresentTime.year=((ReadValue&0xf0)>>4)*10 + (ReadValue&0x0F); //����������ת��
}
