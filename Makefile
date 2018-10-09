main:  main.o nmea.o testNmea.o gps.o uart.o queueManager.o -lpthread

main.o: main.c
	gcc -c main.c

nmea.o: nmea.c nmea.h
	gcc -c nmea.c

testNmea.o: testNmea.c testNmea.h
	gcc -c testNmea.c

gps.o: gps.c gps.h
	gcc -c gps.c

queueManager.o: queueManager.c
	gcc -c queueManager.c

uart.o: uart.c
	gcc -c uart.c

.PHONY:clean
clean:
	rm *.o main
