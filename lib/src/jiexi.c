#include<jiexi.h>
#include<uart.h>
//�� buf����õ��� cx ���������ڵ�λ��
//����ֵ:0~0XFE,����������λ�õ�ƫ��.
// 0XFF,�������ڵ� cx ������

char GPS_Data[150];
itude position; 
char *NMEA_Comma_Pos(char *buf,u8 cx)
{ 
char *p=buf;
while(cx)
{
if(*p==',')cx--;
p++;
}
return p;		 //��һ���ַ�Ϊλ��1
}
//m^n ����
//����ֵ:m^n �η�.
u32 NMEA_Pow(u8 m,u8 n)
{
u32 result=1;
while(n--)result*=m;
return result;
}
//strת��Ϊ����,��','����'*'����
//buf:���ִ洢��
//dx:С����λ��,���ظ����ú���
//����ֵ:ת�������ֵ
double NMEA_Str2num(char *buf)
{
char *p=buf;

double ires=0,fres=0,res;
u8 ilen=0,flen=0,i;
u8 mask=0;	  //����Ƿ��Ǹ�����С��
//p+=1;
while(1) //�õ�������С���ĳ���
{
if(*p=='-'){mask|=0X02;p++;}//�Ǹ���
if(*p==','||(*p=='*'))break;//����������
if(*p=='.'){mask|=0X01;p++;}//����С������
if(mask&0X01)flen++;
else ilen++;
p++;
}
if(mask&0X02)buf++; //ȥ������
for(i=0;i<ilen;i++) ires+=NMEA_Pow(10,ilen-1 -i)*(buf[i]-'0');//�õ�������������
if(flen>5)flen=5; //���ȡ 5 λС��
for(i=0;i<flen;i++) //�õ�С����������
{
fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
}
res=ires+fres/(double)NMEA_Pow(10,flen);
if(mask&0X02)res=-res; 
return res;
}

void Anylize()
{
	 uchar number=0;
	 char* ptr=NULL;
	 isreceive=1;
	 while(1)
	 {
	 scanf("%s",GPS_Data);
	 count=0;
	 if(GPS_Data[3]=='R')break;
	 }
	 isreceive=0;
	 ptr=NMEA_Comma_Pos(GPS_Data,5);
	 position.latitude=NMEA_Str2num(ptr);
	 ptr=NMEA_Comma_Pos(GPS_Data,7);
	 position.longitude=NMEA_Str2num(ptr);
	 for(number;number<150;number++)
	 {
	 	GPS_Data[number]=0;
	 } 	 
}