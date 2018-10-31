#include <stdio.h>
#include <stdlib.h>

#include "nmea.h"
#include "nmeaParse.h"
#include "gps.h"

u8 data[13][100] = {
    "$GPRMC,022857.00,A,111111111111111111,N,1331355.659,E,21.372,141.52,090918,,,A*58\r\n",
    "$GPRMC,022857.00,A,2234.40269,N,11355.65973,E,21.372,141.52,090918,,,A*58\r\n",
    "$GPVTG,141.52,T,,M,21.372,N,39.581,K,A*3D\r\n",
    "$GPGGA,022857.00,2234.40269,N,11355.65973,E,1,04,2.18,104.4,M,-2.8,M,,*4C\r\n",  //3
    "$GPGSA,A,3,06,17,03,28,,,,,,,,,5.99,2.18,5.58*0D",
    "$GPGSV,3,1,09,03,29,046,18,06,52,294,38,09,14,127,09,17,52,013,25*71",
    "$GPGSV,3,2,09,19,39,335,20,22,,,08,23,11,099,07,28,63,161,45*42",
    "$GPGSV,3,3,09,41,46,237,31*43",
    "$GPGLL,2234.40269,N,11355.65973,E,022857.00,A,A*60",
    "$GPRMC,022858.00,A,2234.39807,N,11355.66388,E,21.410,143.07,090918,,,A*57",
    "$GPVTG,143.07,T,,M,21.410,N,39.651,K,A*32",                                  //10
    "$GPGGA,022858.00,2234.39807,N,11355.66388,E,1,04,2.18,106.6,M,-2.8,M,,*42",  //11
    "$GPGGA,022858.00,12345,N,678910,E,1,04,2.18,106.6,M,-2.8,M,,*42",   //12  error
};

void testCRC() {
//"$GPGLL,2234.40269,N,11355.65973,E,022857.00,A,A*60",
}

void GPRMC_CallBack(struct_GPSRMC GPS_RMC_Data) {
    //printf("GPRMC_CallBack\n");
}
void GPGGA_CallBack(struct_GPSGGA GPS_GGA_Data) {
    //printf("Altitude:%.1fmeter\n",atof(GPS_GGA_Data.Altitude));
}
void GPGSA_CallBack(struct_GPSGSA GPS_GSA_Data) {
    //printf("=========GPGSA_CallBack=====\n");
    //printf("Mode1 = %c\n",GPS_GSA_Data.Mode);
    //printf("Mode2 = %c\n",GPS_GSA_Data.Mode2);
	
}
void GPGSV_CallBack(struct_GPSGSV GPS_GSV_Data) {
    int i;
#if 0
    printf("=========GPGSV_CallBack=====\n");
    printf("SatInView = %d\n",GPS_GSV_Data.SatInView);
    for(i=0;i < 12;i++) {
       printf("SatID=%02d,Elevation=%02d,Azimuth=%03d,SNR=%02d\n",GPS_GSV_Data.SatInfo[i].SatID,GPS_GSV_Data.SatInfo[i].Elevation,
                                                                  GPS_GSV_Data.SatInfo[i].Azimuth,GPS_GSV_Data.SatInfo[i].SNR);
    }
#endif
}
void GPVTG_CallBack(nmeaGPVTG vtgPack) {
    //printf("dir=%f,dir_t=%c dec=%3.2f,dec_m=%c spn=%f,spn_n=%c spk=%f,spk_k=%c\n",
    //								 vtgPack.dir,vtgPack.dir_t,
	//									 vtgPack.dec,vtgPack.dec_m,
	//									 vtgPack.spn,vtgPack.spn_n,
	//									 vtgPack.spk,vtgPack.spk_k);
}
void GPGLL_CallBack(struct_GPGLL GLLPack) {


}

void initNmeaParserCallBack() {
    initParserCallBack(GPRMC_CallBack,GPGGA_CallBack,
	                   GPGSA_CallBack,GPGSV_CallBack,
	                   GPVTG_CallBack,GPGLL_CallBack);
}
//B6 -> 'B6'
char hexToBCD(char hex) {
    char decimal;
    decimal = hex >> 4;
    decimal = decimal * 10 + (hex &= 0x0F);
    printf(" bcd= %x ",decimal);
    return decimal;
}

static u8 crcResult=0;
void doCRC(char c,u8 flag) {
    flag = flag & 0x70;
    if(flag == GPS_PARSE_STATUS_CRC_START) {
        crcResult ^= c;
    } else if(flag == GPS_PARSE_STATUS_CRC_END) {
        //result = hexToBCD(result);
         //printf(" BCD=%x\n",crcResult);
    }
    //printf(" c=%c 0x%x,crc=0x%x   ",c,c,result);
}
#if 0
char lat[20]="2234.39807";
char lon[20]="11355.66388";
#else
char lat[10]="2234.40269";
char lon[11]="11355.65973";
#endif
char sended = 0;
void outputNmea(char c,char flag) {
      char i = 0;
      if((flag & 0x03) == GPS_PARSE_STATUS_LATITUDE) {
           if(sended == 0) {
              for(i = 0;i < sizeof(lat);i++) {
                 c = lat[i];
                 doCRC(c,flag);
                 printf("%c",c);
              }
              sended = 1;
           }
      } else if((flag & 0x03) == GPS_PARSE_STATUS_LONGTITUDE) {
           if(sended == 0) {
              for(i = 0;i < sizeof(lon);i++) {
                 c = lon[i];
                 doCRC(c,flag);
                 printf("%c",c);
              }
              sended = 1;
           }
      } else {
           sended = 0;
           doCRC(c,flag);
           if((flag&0xf0) == GPS_PARSE_STATUS_CRC_CRC1) {
              printf("%x\n",crcResult);
              crcResult = 0;
           } else if((flag&0xf0) == GPS_PARSE_STATUS_CRC_CRC2) {

           } else {
              printf("%c",c);
           }
      }
}

void testNMeaParse() {
    int i = 0,j = 0,k;
    int flag;
    char c;

    printf("testNMeaParse\n");
    for(i = 0;i < 6;i++) {
       j = 0;
       while(data[i][j]) {
           c = data[i][j];
           j++;
           flag = NMEA_Parser(c);
           //printf("\n c=%c,flag = 0x%x \n",c,flag);
           if(c == ',') {
                printf(",");
                doCRC(',',flag);
           } else {
                outputNmea(c,flag);
           }
       }
    }

}
void testNmeaStart() {
    int i = 0,j = 0;
    nmea_msg gpsx;

    initNmeaParserCallBack();
    testNMeaParse();

   for(i = 0;i < 1;i++) {
       while(data[0][j]) {
           //GPS_Parser(data[i][j]);
           j++;
           //printf("j = %d\n",j);
       }
   }
#if 0
   NMEA_GPRMC_Analysis(&gpsx,data[1]);
   printf("%02d:%02d:%02d latitude = %d,longitude = %d\n",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec,
                                                          gpsx.latitude,gpsx.longitude);
   printf("latitudeStr = %s,longitudeStr = %s\n",gpsx.latitudeStr,gpsx.longitudeStr);
   printf("data[0] = %s \n",data[0]);
   char CRC[2];
   checkCRC(data[1],CRC);
   //NMEA_StrReplace(data[8],3,"012345678910");
   //NMEA_StrReplace(data[8],5,"555");
   NMEA_StrReplace(data[0],3,gpsx.latitudeStr);
   NMEA_StrReplace(data[0],5,gpsx.longitudeStr);
   NMEA_UpdateCRC(data[0]);
   printf("data[0] = %s\n",data[0]);

   NMEA_GPGGA_Analysis(&gpsx,data[3]);
   printf("hdop = %d,gpssta = %d\n",gpsx.hdop,gpsx.gpssta);

   uartMain();
   testQueue();
#endif
}

