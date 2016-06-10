

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

#include "busled.h"
#include "tlcd.h"

#define DRIVER_NAME		"/dev/cndipsw"

int formerSwitchState[9];
int switchState[9];

/*
void doHelp(void)
{
	printf("Usage:\n");
	printf("dipsw \n");
	printf("return  int  (0~ 15 bit are valid). \n");
}
*/
void decode(int retvalue) {
	if(retvalue != 0)
		retvalue %= 256;
	int p = 128;
	int i;
	for(i=8; i>=1; i--) {
		if(retvalue >= p) {
			retvalue -= p;
			switchState[i] = 1;
		}else {
			switchState[i] = 0;
		}
		p /= 2;
	}
}
/*
void findToggledSwitch() {
	int i;
	for(i=1; i<=8; i++) {
		//printf("%d: %d %d\n", i, formerSwitchState[i], switchState[i]);
		
		if(formerSwitchState[i] != switchState[i]) {
			if(switchState[i] == 1) { // 스위치가 켜졌을 때
				printf("!!!Switch %d ON\n", i);
			}		
			else if(switchState[i] == 0) { // 스위치가 꺼졌을 때
				printf("!!!Switch %d OFF\n", i);
				
			}
		}	

		formerSwitchState[i] = switchState[i];
	}
}
*/
void *dipswitch(void* fSerial) {
	int j;
	for(j=1; j<=8; j++) formerSwitchState[j] = -1;
	int fd, busLedFd;
	int retvalue;
	//printf("///////////////////////////////////////////////////////");	
	//open driver
	fd = open(DRIVER_NAME, O_RDWR);
	if ( fd < 0 )
	{
		perror("driver open error.\n");
		return;
	}

        busLedFd = open(LED_DRIVER_NAME, O_RDWR);
        if ( fd < 0 )
        {
                perror("driver busled (//dev//cnled) open errorn");
        }

	while(1) {		
		read(fd, &retvalue,4);
		//printf("retorigin:%d",retvalue);
		decode(retvalue);
		//findToggledSwitch();
		int i;
	        for(i=1; i<=8; i++) {
        	        //printf("%d: %d %d\n", i, formerSwitchState[i], switchState[i]);

                	if(formerSwitchState[i] != switchState[i]) {
                        	if(switchState[i] == 1) { // 스위치가 켜졌을 때
					controlBusLed(busLedFd, i, 1);
					if(i == 1 || i == 2) {
						char sendCmd[4] = {'o', ' ', '\n', '\0'};
						sendCmd[1] = '0' + i;	
						int ret = write((*(int*)fSerial), sendCmd, sizeof(sendCmd)-1);
						printf("send result: %d\n", ret);
					}
					printf("Switch %d ON\n", i);
                        	}
	                        else if(switchState[i] == 0) { // 스위치가 꺼졌을 때
					controlBusLed(busLedFd, i, 0);
					if(i == 1 || i == 2) {
						char sendCmd[4] = {'x', ' ', '\n', '\0'};
						sendCmd[1] = '0' + i;	
						int ret = write((*(int*)fSerial), sendCmd, sizeof(sendCmd)-1);
						printf("send result: %d\n", ret);
					
						// 스위치 오프되면 tlcd라인을 지워준다.	
						char *argv[3];
						char sensorNum[2] = {' ', '\0'};
						sensorNum[0] = '0' + i;
						argv[0]= "";
						argv[1]= "r";
						argv[2]= sensorNum;
						tlcd_main(3, &argv);
					}
					printf("Switch %d OFF\n", i);
                	        }
	                }
        	        formerSwitchState[i] = switchState[i];
	        }

/*		int i;
		for(i=1; i<=8; i++) {
			if(switchState[i] == 1) 
				printf("Switch %d ON\n", i);
			else 
				printf("Switch %d OFF\n", i);
//			printf("%d ", switchState[i]);
		}
		printf("\n");
*/		
		//findToggledSwitch();
	}
	//close(fd);	
}
/*
int main(int argc , char **argv)
{
	
	if (argc >1 )
	{
		perror(" Args number is more than 1\n");
		doHelp();
		return 1;
	}
	
	pthread_t thread[1];
	pthread_create(&thread[0], NULL, &dipswitch, NULL);
	
	pthread_join(thread[0], NULL);

	return 1;
}
*/
