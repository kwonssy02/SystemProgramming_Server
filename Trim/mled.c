

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
#include <termios.h>
#include "mled.h"

#define DRIVER_NAME		"/dev/cnmled"
/*
void doHelp(void)
{
	printf("Usage:\n");
	printf("mledtest option displayTime number\n");
	printf("exit 'q' \n");
	printf("ex) mledtest s 3 71 ; display '71' for 3 Seconds\n");
	printf("ex) mledtest c 3 71 ; counting  from 0 to 71  with 3 Seconds interval.\n");

}
*/

#define MAX_COLUMN_NUM	5

// 0 ~ 9
const unsigned short NumData[10][MAX_COLUMN_NUM]=
{
	{0xfe00,0xfd7F,0xfb41,0xf77F,0xef00}, // 0
	{0xfe00,0xfd42,0xfb7F,0xf740,0xef00}, // 1
	{0xfe00,0xfd79,0xfb49,0xf74F,0xef00}, // 2
	{0xfe00,0xfd49,0xfb49,0xf77F,0xef00}, // 3
	{0xfe00,0xfd0F,0xfb08,0xf77F,0xef00}, // 4
	{0xfe00,0xfd4F,0xfb49,0xf779,0xef00}, // 5
	{0xfe00,0xfd7F,0xfb49,0xf779,0xef00}, // 6
	{0xfe00,0xfd07,0xfb01,0xf77F,0xef00}, // 7
	{0xfe00,0xfd7F,0xfb49,0xf77F,0xef00}, // 8
	{0xfe00,0xfd4F,0xfb49,0xf77F,0xef00}  // 9
};

static struct termios oldt, newt;
void changemode(int dir)
{
	if( dir == 1)
	{
		tcgetattr(STDIN_FILENO , &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO );
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	}
	else
	{
		tcsetattr(STDIN_FILENO , TCSANOW, &oldt);

	}
}

#define ONE_LINE_TIME_U 	1000
/*
// exit return => 0 , success return => 1
int displayDotLed(int driverfile , int* num)
{
	int highChar , lowChar;
	int temp;
	unsigned short wdata[2];

	while(1) {		

		temp = num % 100;
		highChar = temp / 10;
		lowChar = temp % 10;

		wdata[0] = NumData[highChar][0];
		wdata[1] = NumData[lowChar][0];
		write(driverfile,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][1];
		wdata[1] = NumData[lowChar][1];
		write(driverfile,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][2];
		wdata[1] = NumData[lowChar][2];
		write(driverfile,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][3];
		wdata[1] = NumData[lowChar][3];
		write(driverfile,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][4];
		wdata[1] = NumData[lowChar][4];
		write(driverfile,(unsigned char*)wdata,4);
	}

	while(1)
	{
		// high byte display 
		wdata[0] = NumData[highChar][cSelCounter];

		// low byte display 
		wdata[1] = NumData[lowChar][cSelCounter];

		write(driverfile,(unsigned char*)wdata,4);

		cSelCounter++;
		if ( cSelCounter >= (MAX_COLUMN_NUM-1))
			cSelCounter = 1;

		usleep(ONE_LINE_TIME_U);

		loopCounter++;
		if ( loopCounter > totalCount )
			break;

	}
	
	//wdata[0]= 0;
	//wdata[1]= 0;
	//write(driverfile,(unsigned char*)wdata,4);

	return 1;
}
*/

void* mled(void *Num)
{
	printf("mled!!\n");
	int fd;
	int highChar , lowChar;
	int temp;
	unsigned short wdata[2];
	int* num = (int*)Num;
	printf("num=%d\n", *num);
	changemode(1);
	// open  driver 
	fd = open(DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver  open error.\n");
		return;
	}

	while(1) {		

		temp = *num % 100;
		highChar = temp / 10;
		lowChar = temp % 10;

		wdata[0] = NumData[highChar][0];
		wdata[1] = NumData[lowChar][0];
		write(fd,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][1];
		wdata[1] = NumData[lowChar][1];
		write(fd,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][2];
		wdata[1] = NumData[lowChar][2];
		write(fd,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][3];
		wdata[1] = NumData[lowChar][3];
		write(fd,(unsigned char*)wdata,4);
		wdata[0] = NumData[highChar][4];
		wdata[1] = NumData[lowChar][4];
		write(fd,(unsigned char*)wdata,4);
	}
	
	changemode(0);
	close(fd);
	
	return;
}
/*
int main() {
	int num = 10;
	pthread_t thread[1];
	printf("beforecreate\n");
	pthread_create(&thread[0], NULL,  &mled, (void*)&num);
	
	printf("aftercreate\n");
	while(1) {
		scanf("%d", &num);
		printf("num is : %d\n", num);
	}
	return 1;
}
*/
