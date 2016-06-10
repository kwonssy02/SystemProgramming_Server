

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
#include <pthread.h>

#define DRIVER_NAME		"/dev/cnkey"

/*
void doHelp(void)
{
	printf("Usage:\n");
	printf("key \n");
	printf("return   0 ~15 (push button nubmer)\n"); 
}
*/

void* keymatrix(void* fdArdu){
	int rdata;
	int fd;
	int pushedBtn = -200;
	char temp;
  	fd = open(DRIVER_NAME, O_RDWR);
	if(fd < 0){
		perror("driver open erro.\n");
		return;
	}
	while(1){
		pushedBtn = rdata;
		read(fd, &rdata, 4);
		//printf("%d\n", rdata);
			
		if((rdata <= 0 || pushedBtn == rdata)|| ((rdata != 8) && (rdata != 3) && (rdata != 7) && (rdata != 11))) {		
			continue;	
		}
		
		switch(rdata){
			case 8:
				temp = 'u';
				break;
			case 3:
				temp = 'l';
				break;
			case 7:
				temp = 'd';
				break;
			case 11:
				temp = 'r';
				break;
		}
		char sendCmd[2] = {' ', '\n'};
		sendCmd[0] = temp;
		int test = write((*(int*)fdArdu), sendCmd, sizeof(sendCmd));
		printf("button %d %d\n", rdata, test);
		
		//DotMatrix('s', 0, pushedBtn);		
	//Sleep(100);
		}
	close(fd);
	
}

/*
int keymatrix(int argc , char **argv)
{
	if (argc > 1 )
	{
		perror(" Args number is more than 1\n");
		doHelp();
		return 1;
	}
	
	pthread_t thread[1];
	pthread_create(&thread[0], NULL, &ReadKey, NULL);
	
	pthread_join(thread[0], NULL);
	
	return 0;
}*/
