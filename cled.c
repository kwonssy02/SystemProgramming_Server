

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

#include "cled.h"

#define DRIVER_NAME		"/dev/cncled"


/* void doHelp(void)
{
	printf("Usage:Color LED\n");
	printf("cledtest ledindex  red green blue\n");
	printf("color range 0 ~ 255\n");
	printf("ex) cledtest 0  255 255 255 \n");
	printf(" =>0th led is white color \n");
	printf("ex) cledtest 0 255 0 0 \n");
	printf(" =>0th led is Red color \n");
	printf("ledindex : 0 ~ 3 \n");
} */

#define INDEX_LED		0
#define INDEX_RED_LED		1
#define INDEX_GREEN_LED		2
#define INDEX_BLUE_LED		3
#define INDEX_MAX			4
int cled_main(int* argv)//"r g b"
{
	unsigned short colorArray[INDEX_MAX];
	
	int fd;

	colorArray[INDEX_LED] =(unsigned short) argv[0];
	/*	
	if (colorArray[INDEX_LED] > 3 )
	{
		doHelp();
		perror("led index  over range.\n");
		return 1;
	}
	*/
	colorArray[INDEX_RED_LED] =(unsigned short) argv[1];
	colorArray[INDEX_GREEN_LED] =(unsigned short) argv[2];
	colorArray[INDEX_BLUE_LED] =(unsigned short) argv[3];
	
	// open  driver 
	fd = open(DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver  open error.\n");
		return 1;
	}
/*
	printf("index(%d) r(%d) g(%d) b(%d)\n",colorArray[INDEX_LED],colorArray[INDEX_REG_LED],colorArray[INDEX_GREEN_LED],colorArray[INDEX_BLUE_LED]);
*/
	write(fd,&colorArray,6);
	close(fd);
	
	return 0;
}
