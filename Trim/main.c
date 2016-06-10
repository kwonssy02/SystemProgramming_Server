#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "oled.h"
#include "tlcd.h"
#include "cled.h"
#include "dipswitch.h"
#include "busled.h"
#include "keymatrix.h"
#include "bitmap.h"
#include "camera.h"
#include "touchapp.h"
#include "postFile.h"
#include "mled.h"
#include "buzzer.h"
#include "fnd.h"

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B115200            
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyACM0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

volatile int STOP = FALSE;


void* writeArdu(void* fd){
	char val = 'v';
	int fdVal = 0;

	printf("fd value : %d\n",*((int*)fd));
	while(1){
		fdVal = write((*(int*)fd), &val, 1);	
		printf("%d\n", fdVal);
		sleep(1);
	}
}

void main()
{
	int fd, c, res;
	struct termios oldtio, newtio;
	char buf[255];
	char temp[255];
	int numOfShots = 0;	
//	camera_main();
	/*
	Open modem device for reading and writing and not as controlling tty
	because we don't want to get killed if linenoise sends CTRL-C.
	*/
	//OPEN ARDUINO DEVICE
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
	if (fd <0) { perror(MODEMDEVICE); exit(-1); }

	tcgetattr(fd, &oldtio); /* save current serial port settings */
	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

	//SETTING COMMUNICATION PARAMETERS

	/*
	BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	CRTSCTS : output hardware flow control (only used if the cable has
	all necessary lines. See sect. 7 of Serial-HOWTO)
	CS8     : 8n1 (8bit,no parity,1 stopbit)
	CLOCAL  : local connection, no modem contol
	CREAD   : enable receiving characters
	*/
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

	/*
	IGNPAR  : ignore bytes with parity errors
	ICRNL   : map CR to NL (otherwise a CR input on the other computer
	will not terminate input)
	otherwise make device raw (no other input processing)
	*/
	newtio.c_iflag = IGNPAR | ICRNL;

	/*
	Raw output.
	*/
	newtio.c_oflag = 0;

	/*
	ICANON  : enable canonical input
	disable all echo functionality, and don't send signals to calling program
	*/
	newtio.c_lflag = ICANON;

	/*
	initialize all control characters
	default values can be found in /usr/include/termios.h, and are given
	in the comments, but we don't need them here
	*/
	newtio.c_cc[VINTR] = 0;     /* Ctrl-c */
	newtio.c_cc[VQUIT] = 0;     /* Ctrl-\ */
	newtio.c_cc[VERASE] = 0;     /* del */
	newtio.c_cc[VKILL] = 0;     /* @ */
	newtio.c_cc[VEOF] = 4;     /* Ctrl-d */
	newtio.c_cc[VTIME] = 0;     /* inter-character timer unused */
	newtio.c_cc[VMIN] = 1;     /* blocking read until 1 character arrives */
	newtio.c_cc[VSWTC] = 0;     /* '\0' */
	newtio.c_cc[VSTART] = 0;     /* Ctrl-q */
	newtio.c_cc[VSTOP] = 0;     /* Ctrl-s */
	newtio.c_cc[VSUSP] = 0;     /* Ctrl-z */
	newtio.c_cc[VEOL] = 0;     /* '\0' */
	newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	newtio.c_cc[VWERASE] = 0;     /* Ctrl-w */
	newtio.c_cc[VLNEXT] = 0;     /* Ctrl-v */
	newtio.c_cc[VEOL2] = 0;     /* '\0' */

	/*
	now clean the modem line and activate the settings for the port
	*/
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	/*
	terminal settings done, now handle input
	In this example, inputting a 'z' at the beginning of a line will
	exit the program.
	*/
	/* dipswitch thread start */
	oled();	
	pthread_t thread[4];
	//test
//	pthread_create(&thread[1], NULL, &keymatrix, (void*)&fd);
	//pthread_join(&thread[2], NULL);
	//printf("keymatrix thread was dead\n");
	sleep(3);  // 아두이노와 연결 initilizing 시간을 법니다.
	pthread_create(&thread[0], NULL, &dipswitch, (void*)&fd); //딥스위치
	//pthread_create(&thread[1], NULL, &writeArdu, (void*)&fd);
	//pthread_create(&thread[1], NULL, &writeArdu, (void*)&fd);	 
	pthread_create(&thread[1], NULL, &fnd, NULL); //7세그먼트
	pthread_create(&thread[2], NULL, &touchPad, (void*)&fd); //세팅화면
	pthread_create(&thread[3], NULL, &mled, (void*)&numOfShots); //mled

	//tlcd_main('c', 2, "");
	
	char *argvc[6];
	argvc[0]= "./tlcd";
	argvc[1]= "c";
	argvc[2]= "1";
	argvc[3]= "1";
	argvc[4]= "2";
	argvc[5]= "12";

	tlcd_main(6, &argvc);
	
	char *argvc2[6];
	argvc2[0]= "./tlcd";
	argvc2[1]= "c";
	argvc2[2]= "1";
	argvc2[3]= "0";
	argvc2[4]= "2";
	argvc2[5]= "12";

	tlcd_main(6, &argvc2);
	//system("./tlcd c 1 1 2 12");
	sleep(1);
	int maxCO;
	while (STOP == FALSE) {     /* loop until we have a terminating condition */
		/* read blocks program execution until a line terminating character is
		input, even if more than 255 chars are input. If the number
		of characters read is smaller than the number of chars available,
		subsequent reads will return the remaining chars. res will be set
		to the actual number of characters actually read */
		res = read(fd, buf, 255);
			buf[res] = 0;   
		if(strcmp(buf,"0") >= 0){ 

			char cmd[4];
			char cmd2[255];
			strncpy(cmd, buf, 4);
			
			/* set end of string, so we can printf */	
			int i;
			for(i=4; i<255; i++){
				cmd2[i-4] = buf[i];
				if(buf[i] == '\0')
					break;	
			}
			
			printf("cmd=%s, cmd2=%s\n", cmd, cmd2);
			if(strstr(cmd, "sen") != NULL) {
				char sensorNum[2];
				sensorNum[0] =  cmd[3];
				sensorNum[1] = '\0';
				//printf("sensorNum= %c\n", sensorNum);	
				char tlcd_content[14] = "Sensor";
				tlcd_content[6] = sensorNum[0];
				tlcd_content[7] = ':';

				sprintf(temp, "%d", atoi(cmd2));
				strcat(tlcd_content, temp);
					
				char *argv[3];
				argv[0]= "";
				argv[1]= "r";
				argv[2]= sensorNum;
				tlcd_main(3, &argv);
				/*
				char removeStatement[100] = "./tlcd r ";
				strcat(removeStatement, sensorNum);
				char writeStatement[100] = "./tlcd w ";
				strcat(writeStatement, sensorNum);
				
				strcat(writeStatement, tlcd_content);
				system(removeStatement);
				sleep(1);
				system(writeStatement);	
				sleep(1);
				*/
				sleep(1);		
				char *argv2[4];
				argv2[0]= "";
				argv2[1]= "w";
				argv2[2]= sensorNum;
				argv2[3]= tlcd_content;
				tlcd_main(4, &argv2);
				
				maxCO = maxCO >= atoi(cmd2) ? maxCO : atoi(cmd2); // 두 센서중 가장 큰 센서 CO값 저장.
				if(maxCO < 200){
					int param[4] ={0, 0, 0, 50}; 

					cled_main(param);
				}
				else if(maxCO > 200){

					int param[4] ={0, 50, 0, 50}; 
					
					cled_main(param);
				}
			}
			else if(strstr(cmd, "cam") != NULL) {
				char * ptr = strstr(cmd, "cam");
				char sensorNum = ptr[3];
				printf("!!!!!!!!!!!!!!!!!%s!!!!!!!!!!!!!!\n",cmd);
				screenshot(); //스크린샷 찍고 파일로 저장
				postFile(sensorNum); //url로 파일 전송
				numOfShots++; //mled에 보여주는 사진찍은 횟수 증가
				char tempo = 'k';
				int ret = write(fd, &tempo, 1);	//사진 다 찍었다 보내줌.		
				printf("send result:%d\n", ret);
				buzzer(); //사진찍었음을 알리는 버저 울림
			}

			if (buf[0] == 'z') STOP = TRUE;
		}
	
	}

	/* restore the old port settings */
	tcsetattr(fd, TCSANOW, &oldtio);
}
