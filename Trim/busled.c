#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "busled.h"

#define LED_ON	1
#define LED_OFF	0
#define MAX_LED_NO 8
#define LED_DRIVER_NAME		"/dev/cnled"

void controlBusLed(int fd, int ledNo, int ledControl) {

	int wdata ,rdata,temp;

	read(fd,&rdata,4);
        temp = 1;

        if ( ledControl ==1 )
        {
        	temp <<=(ledNo-1);
                wdata = rdata | temp;
        }
        else
        {
                temp =  ~(temp<<(ledNo-1));
                wdata = rdata & temp;
        }
        
	write(fd,&wdata,4);
}
