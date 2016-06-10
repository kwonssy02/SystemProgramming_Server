#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <errno.h>
#include <resolv.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
		a		
	f		b
		g		
	e		c	
		d		dp


*/
#define DRIVER_NAME		"/dev/cnfnd"

#define MAX_FND_NUM		6

#define  DOT_OR_DATA	0x80

#define MAXBUF 1024

//char * servURL = "http://ec2-52-69-0-71.ap-northeast-1.compute.amazonaws.com";
char * servURL = "52.69.0.71";
int serverPort = 5000;
char * dir = "/getDate";

const unsigned short segNum[10] =
{
	0x3F, // 0
	0x06,
	0x5B,
	0x4F,
	0x66,
	0x6D,
	0x7D,
	0x27,
	0x7F,
	0x6F  // 9
};
const unsigned short segSelMask[MAX_FND_NUM] = 
{
	0xFE00,
	0xFD00,
	0xFB00,
	0xF700,
	0xEF00,
	0xDF00
};
static struct termios oldt, newt;
void Changemode(int dir)
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

#define MODE_STATIC_DIS		0
#define MODE_TIME_DIS		1
#define MODE_COUNT_DIS		2
void* fnd()
{
	int fd;
	int mode ;
	int number,counter;
	int durationtime;
	

	int cSelCounter,loopCounter;
        int temp , totalCount, i ;
        unsigned short wdata;
        int dotEnable[MAX_FND_NUM];
        int fndChar[MAX_FND_NUM];
	int num =0;
	
	// open  driver 
	fd = open(DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver open error.\n");
		return;
	}
	
	char date[7];
	getDate(date);
	printf("%s\n", date);

        fndChar[0]= date[0]-'0'; 

        fndChar[1]= date[1]-'0';

        fndChar[2]= date[2]-'0';

        fndChar[3]= date[3]-'0';

        fndChar[4]= date[4]-'0';

        fndChar[5]= date[5]-'0';

        cSelCounter = 0;
        loopCounter = 0;
	Changemode(1);
	dotEnable[0] =0;
	dotEnable[1] =1;
	dotEnable[2] =0;
	dotEnable[3] =1;
	dotEnable[4] =0;
	dotEnable[5] =0;	
	while(1){
		wdata = segNum[fndChar[0]]  | segSelMask[0] ;
	       if (dotEnable[0])
			wdata |= DOT_OR_DATA;
		write(fd,&wdata,2);

		wdata = segNum[fndChar[1]]  | segSelMask[1] ;
		if (dotEnable[1])
			wdata |= DOT_OR_DATA;
		write(fd,&wdata,2);		

		wdata = segNum[fndChar[2]]  | segSelMask[2] ;
		if (dotEnable[2])
			wdata |= DOT_OR_DATA;
		write(fd,&wdata,2);

		wdata = segNum[fndChar[3]]  | segSelMask[3] ;
		if (dotEnable[3])
			wdata |= DOT_OR_DATA;
		write(fd,&wdata,2);

		wdata = segNum[fndChar[4]]  | segSelMask[4] ;
		if (dotEnable[4])
			wdata |= DOT_OR_DATA;
		write(fd,&wdata,2);
	       
		 wdata = segNum[fndChar[5]]  | segSelMask[5] ;
		if (dotEnable[5])
			wdata |= DOT_OR_DATA;
		write(fd,&wdata,2);

        }
	Changemode(0);
	close(fd);

	return;

}
int getDate2(char* ptr) 
{
	ptr[0] = '1';
	ptr[1] = '5';
	ptr[2] = '1';
	ptr[3] = '2';
	ptr[4] = '0';
	ptr[5] = '3';
}

int getDate(char* ptr)
{   
	int ndx;
	int sockfd, bytes_read;
	struct sockaddr_in servAddr;
	char ip[20];
	char buffer[MAXBUF];

	if ( (sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
		printf("Socket");

	struct hostent *host_entry;
	host_entry = gethostbyname(servURL);
	for(ndx = 0; NULL != host_entry->h_addr_list[ndx]; ndx++) {
		strcpy(ip, inet_ntoa(*(struct in_addr*)host_entry->h_addr_list[ndx]));
	}

	/*---Initialize server address/port struct---*/
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(serverPort); /*default HTTP Server port */

	/*---Connect to server---*/
	if ( connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) != 0 )
		printf("Connect");
	
	sprintf(buffer, "GET %s HTTP/1.0\n\n", dir);
	send(sockfd, buffer, strlen(buffer), 0);

	/*---While there's data, read and print it---*/

	char *ptf;
	do
	{
		bzero(buffer, sizeof(buffer));
		bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
		if ( bytes_read > 0 ) {
			printf("buffer: %s\n", buffer);
			ptf = strstr(buffer, "{\"time\":");
			ptr[0] = ptf[9];
			ptr[1] = ptf[10];
			ptr[2] = ptf[11];
			ptr[3] = ptf[12];
			ptr[4] = ptf[13];
			ptr[5] = ptf[14];
			ptr[6] = '\0';
		 }
	    }while ( bytes_read > 0 );

	//printf("%s", ptr);
	/*---Clean up---*/
	close(sockfd);
	return 0;
}

