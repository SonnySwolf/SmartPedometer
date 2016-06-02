#ifndef __GPS_H
#define __GPS_H
#include<Config.h> 

typedef struct 
{
	double latitude;
	double longitude;
}itude;

extern char GPS_Data[150];
extern itude position;
char* NMEA_Comma_Pos(char *buf,u8 cx);
u32 NMEA_Pow(u8 m,u8 n);
double NMEA_Str2num(char *buf);
void Anylize();
#endif