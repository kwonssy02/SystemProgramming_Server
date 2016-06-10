main: main.o tlcd.o cled.o dipswitch.o busled.o keymatrix.o touchapp.o camera.o bitmap.o oled.o postFile.o mled.o buzzer.o fnd.o
	arm-none-linux-gnueabi-gcc -o main main.o tlcd.o cled.o dipswitch.o busled.o keymatrix.o touchapp.o camera.o bitmap.o oled.o postFile.o mled.o buzzer.o fnd.o -lpthread
	mv main /tftp
	ifconfig eth0 192.168.1.1

main.o: main.c tlcd.h dipswitch.h keymatrix.h
	arm-none-linux-gnueabi-gcc -c main.c -lpthread

tlcd.o: tlcd.c tlcd.h
	arm-none-linux-gnueabi-gcc -c tlcd.c	

cled.o: cled.c cled.h
	arm-none-linux-gnueabi-gcc -c cled.c

dipswitch.o: dipswitch.c dipswitch.h busled.c busled.h tlcd.c tlcd.h
	arm-none-linux-gnueabi-gcc -c dipswitch.c busled.c tlcd.c -lpthread

busled.o: busled.c busled.h
	arm-none-linux-gnueabi-gcc -c busled.c

keymatrix.o: keymatrix.c keymatrix.h
	arm-none-linux-gnueabi-gcc -c keymatrix.c

touchapp.o: touchapp.c touchapp.h camera.c camera.h bitmap.c bitmap.h keymatrix.c keymatrix.h
	arm-none-linux-gnueabi-gcc -c touchapp.c camera.c bitmap.c keymatrix.c -lpthread

camera.o: camera.c camera.h
	arm-none-linux-gnueabi-gcc -c camera.c -lpthread

bitmap.o: bitmap.c bitmap.h
	arm-none-linux-gnueabi-gcc -c bitmap.c
oled.o: oled.c oled.h
	arm-none-linux-gnueabi-gcc -c oled.c

postFile.o: postFile.c postFile.h
	arm-none-linux-gnueabi-gcc -c postFile.c

mled.o: mled.c mled.h
	arm-none-linux-gnueabi-gcc -c mled.c

buzzer.o: buzzer.c buzzer.h
	arm-none-linux-gnueabi-gcc -c buzzer.c

fnd.o: fnd.c fnd.h
	arm-none-linux-gnueabi-gcc -c fnd.c
