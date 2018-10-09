#include <stdio.h>
#include <stdlib.h>

#include "nmea.h"
#include "gps.h"

void testNmeaStart() {
    int i = 0,j = 0;
    nmea_msg gpsx;
    u8 data[11][100] = {
    "$GPRMC,022857.00,A,111111111111111111,N,1331355.659,E,21.372,141.52,090918,,,A*58",
    "$GPRMC,022857.00,A,2234.40269,N,11355.65973,E,21.372,141.52,090918,,,A*58",
    "$GPVTG,141.52,T,,M,21.372,N,39.581,K,A*3D",
    "$GPGGA,022857.00,2234.40269,N,11355.65973,E,1,04,2.18,104.4,M,-2.8,M,,*4C",  //3
    "$GPGSA,A,3,06,17,03,28,,,,,,,,,5.99,2.18,5.58*0D",
    "$GPGSV,3,1,09,03,29,046,18,06,52,294,38,09,14,127,09,17,52,013,25*71",
    "$GPGSV,3,2,09,19,39,335,20,22,,,08,23,11,099,07,28,63,161,45*42",
    "$GPGSV,3,3,09,41,46,237,31*43",
    "$GPGLL,2234.40269,N,11355.65973,E,022857.00,A,A*60",
    "$GPRMC,022858.00,A,2234.39807,N,11355.66388,E,21.410,143.07,090918,,,A*57",
    "$GPVTG,143.07,T,,M,21.410,N,39.651,K,A*32",                                  //10
    "$GPGGA,022858.00,2234.39807,N,11355.66388,E,1,04,2.18,106.6,M,-2.8,M,,*42"};

   for(i = 0;i < 11;i++) {
       while(data[i][j]) {
           //GPS_Parser(data[i][j]);
           j++;
           //printf("j = %d\n",j);
       }
   }

   NMEA_GPRMC_Analysis(&gpsx,data[1]);
   printf("%02d:%02d:%02d latitude = %d,longitude = %d\n",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec,
                                                          gpsx.latitude,gpsx.longitude);
   printf("latitudeStr = %s,longitudeStr = %s\n",gpsx.latitudeStr,gpsx.longitudeStr);
   printf("data[0] = %s \n",data[0]);
   //checkCRC(data[8]);
   //NMEA_StrReplace(data[8],3,"012345678910");
   //NMEA_StrReplace(data[8],5,"555");
   NMEA_StrReplace(data[0],3,gpsx.latitudeStr);
   NMEA_StrReplace(data[0],5,gpsx.longitudeStr);
   NMEA_UpdateCRC(data[0]);
   printf("data[0] = %s\n",data[0]);

   NMEA_GPGGA_Analysis(&gpsx,data[3]);
   printf("hdop = %d,gpssta = %d\n",gpsx.hdop,gpsx.gpssta);

}
