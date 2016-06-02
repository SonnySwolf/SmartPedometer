#ifndef __GPS_H
#define __GPS_H
#include<Config.h> 
#include<uart.h>
#include <delay.h>
typedef struct 
{
	double latitude;
	double longitude;
}itude;
extern char lati[12];
extern char longi[12]; 
extern char GPS_Data[150];
extern itude position;
char* NMEA_Comma_Pos(char *buf,u8 cx);
u32 NMEA_Pow(u8 m,u8 n);
void NMEA_Strlati(char *buf);
void NMEA_Strlongi(char *buf);
void Anylize();
#endif