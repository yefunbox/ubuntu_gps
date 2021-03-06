#ifndef __NMEA_H
#define __NMEA_H

typedef unsigned char u8;
typedef unsigned short u16;


#define NMEA_NULL   0x00        //GPS�������
#define NMEA_GPGGA  0x01
#define NMEA_GPGSA  0x02
#define NMEA_GPGSV  0x04
#define NMEA_GPRMC  0x08

#define GPS_PARSE_FAILURE           0x00
#define GPS_PARSE_START             0x01
#define GPS_PARSE_CHECKSUM_ERROR    0x02
#define GPS_PARSE_OK                0x03

u8 GPS_Parser(char SBuf);
void GPS_View_Basic(u8 ucLogStatus);
void GPS_View_SatInfo(void);
void GPS_View_Log(void);
void GPS_View_Speed(void);
void GPS_GetLogData(u8 * LogBuf);
void GPS_GetTime(u8* ucDataTime);


#endif
