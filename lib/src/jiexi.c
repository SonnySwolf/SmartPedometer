#include<jiexi.h>
#include<uart.h>
//从 buf里面得到第 cx 个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
// 0XFF,代表不存在第 cx 个逗号

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
return p;		 //第一个字符为位置1
}
//m^n 函数
//返回值:m^n 次方.
u32 NMEA_Pow(u8 m,u8 n)
{
u32 result=1;
while(n--)result*=m;
return result;
}
//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
double NMEA_Str2num(char *buf)
{
char *p=buf;

double ires=0,fres=0,res;
u8 ilen=0,flen=0,i;
u8 mask=0;	  //标记是否是负数或小数
//p+=1;
while(1) //得到整数和小数的长度
{
if(*p=='-'){mask|=0X02;p++;}//是负数
if(*p==','||(*p=='*'))break;//遇到结束了
if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
if(mask&0X01)flen++;
else ilen++;
p++;
}
if(mask&0X02)buf++; //去掉负号
for(i=0;i<ilen;i++) ires+=NMEA_Pow(10,ilen-1 -i)*(buf[i]-'0');//得到整数部分数据
if(flen>5)flen=5; //最多取 5 位小数
for(i=0;i<flen;i++) //得到小数部分数据
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