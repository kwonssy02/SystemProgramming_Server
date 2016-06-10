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
#include "buzzer.h"


#define DRIVER_NAME		"/dev/cnbuzzer"
#define MAX_BUZZER_NUMBER		36


int buzzer()
{
	int buzzerNumber = 1;
	int fd;
	
	// open  driver 
	fd = open(DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver (//dev//cnbuzzer) open error.\n");
		return 1;
	}
	// control led 
	write(fd, &buzzerNumber, 4);
	close(fd);
	usleep(200000);

	fd = open(DRIVER_NAME,O_RDWR);
	buzzerNumber = 13;
	write(fd, &buzzerNumber, 4);
	close(fd);
	usleep(200000);

	fd = open(DRIVER_NAME,O_RDWR);
	buzzerNumber = 25;
	write(fd, &buzzerNumber, 4);
	close(fd);
	usleep(200000);
	
	fd = open(DRIVER_NAME,O_RDWR);
	buzzerNumber = 0;
	write(fd, &buzzerNumber, 4);
	close(fd);
	
	return 0;
}
