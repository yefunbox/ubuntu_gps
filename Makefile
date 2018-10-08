main:  main.o nmea.o testNmea.o gps.o

main.o: main.c
	cc -c main.c

nmea.o: nmea.c nmea.h
	cc -c nmea.c

testNmea.o: testNmea.c testNmea.h
	cc -c testNmea.c

gps.o: gps.c gps.h
	cc -c gps.c

.PHONY:clean
clean:
	rm *.o main
