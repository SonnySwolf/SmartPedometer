#include<jiexi.h>
//�� buf����õ��� cx ���������ڵ�λ��
//����ֵ:0~0XFE,����������λ�õ�ƫ��.
// 0XFF,�������ڵ� cx ������
char GPS_Data[150];
char lati[12]={0};
char longi[12]={0};
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

/*double NMEA_Str2num(char *buf)
{
char *p=buf;

double ires=0,fres=0,res;
u8 ilen=0,flen=0,i;
u8 mask=0;	  //����Ƿ��Ǹ�����С��
//p+=1;
while(1) //�õ�������С���ĳ���
{
if(*p=='-'){mask|=0X02;p++;}//�Ǹ���
if(*p==','||(*p=='*')|(*p == '\0'))break;//����������
if(*p=='.'){mask|=0X01;p++;}//����С������
if(mask&0X01)flen++;
else ilen++;
p++;
}
if(mask&0X02)buf++; //ȥ������
for(i=0;i<ilen;i++) ires+=NMEA_Pow(10,ilen-1 -i)*(buf[i]-'0');//�õ�������������
if(flen>6)flen=6; //���ȡ 5 λС��
for(i=0;i<flen;i++) //�õ�С����������
{
fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
}
res=ires+fres/(double)NMEA_Pow(10,flen);
if(mask&0X02)res=-res; 
return res;
}*/
void NMEA_Strlati(char *buf)
{
	char *p = buf;
	uchar i=0;
	while(1)
	{ 
	lati[i] = *p;
		p++;i++;
	 if(*p == ',')break; 
	}
	if(i > 5){
	lati[i] = '\0';
	i = 0;
	while(1)
	{
		if(lati[i] == '.') {lati[i] = lati[i-1];
								lati[i-1] = lati[i-2];
								lati[i-2] = '.';break;}
		i++;
	}}
	else {lati[0] = '1';lati[1] = '\0';}
}
void NMEA_Strlongi(char *buf)
{
	char *p = buf;
	uchar i=0;
	while(1)
	{ 
	longi[i] = *p;
		p++;i++;
	 if(*p == ',')break; 
	}
	if(i > 5)
	{
	longi[i] = '\0';
	i = 0;
	while(1)
	{
		if(longi[i] == '.') {longi[i] = longi[i-1];
								longi[i-1] = longi[i-2];
								longi[i-2] = '.';break;}
		i++;
	}
	}
	else {longi[0] = '1';longi[1] = '\0';}
}
/*void Anylize()
{
	 uchar number=0;
	 char* ptr=NULL;
	 isreceive2=1;
	 while(1)
	 {
	 scanf("%s",GPS_Data);
	 //count=0;
	 if(GPS_Data[3]=='R')break;
	 }
	 isreceive2=0;
	 ptr=NMEA_Comma_Pos(GPS_Data,3);
	 position.latitude=NMEA_Str2num(ptr);
	 ptr=NMEA_Comma_Pos(GPS_Data,5);
	 position.longitude=NMEA_Str2num(ptr);
	 for(number;number<150;number++)
	 {
	 	GPS_Data[number]=0;
	 } 	 
}*/
void Anylize()
{
	 uchar number=0;
	 char* ptr=NULL;
	 for(number;number<150;number++)
	 {
		 	GPS_Data[number]=0;
	 }
	 isreceive2=1;
	 while(1)
	 {
	 scanf("%s",GPS_Data);
	 if(GPS_Data[3]=='R')break;
	 }
	 isreceive2=0;
	 ptr=NMEA_Comma_Pos(GPS_Data,3);
	 NMEA_Strlati(ptr);
	 ptr=NMEA_Comma_Pos(GPS_Data,5);
	 NMEA_Strlongi(ptr);
}