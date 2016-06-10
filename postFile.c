#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl

#define DATA_SIZE 1024*1024*10 //10MB
#define MAXLINE 409600
 
 
//char * servIP = "ec2-52-69-0-71.ap-northeast-1.compute.amazonaws.com";
char * servIP = "52.69.0.71";
int servPort = 5000;
 
 
int makeSocket(){
	int ndx;
	struct sockaddr_in servAddr;
	int sock;
	char ip[20];
	struct hostent *host_entry;

	/* Create areliable, stream socket using TCP */
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        	printf("socket create failed\n");
	
	host_entry = gethostbyname(servIP);

	for(ndx = 0; NULL != host_entry->h_addr_list[ndx]; ndx++){
		char *temp = inet_ntoa(*(struct in_addr*)host_entry->h_addr_list[ndx]);
		printf("%s\n", temp);
		strcpy(ip, inet_ntoa(*(struct in_addr*)host_entry->h_addr_list[ndx]));
	}
	
	//struct hostent *host_entry2;
	memset(&servAddr, 0, sizeof(servAddr)); //Zero ou structure
	servAddr.sin_family = AF_INET; //Internet address family
	servAddr.sin_addr.s_addr = inet_addr(ip); //Server IP address
	servAddr.sin_port = htons(servPort);//Server port

	/* Establish the connection to the web server */
	if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	    printf("connet() failed\n");

	return sock;
}

int postFile(char sensorNum)
{
	char *fileDir ="./picture.img";
	char *filename ="picture.img";
	char *packet;
	char pre_body[1024], post_body[1024];
	char sendline[MAXLINE + 1];
	char boundary[] ="----WebKitFormBoundaryu8FzpUGNDgydoA4z";

	char *bodyline = calloc(sizeof(char), DATA_SIZE);
	char *fileBuf = calloc(sizeof(char), DATA_SIZE);
	int sock = makeSocket();
	printf("send file name : %s\n", filename);

	FILE *fp = fopen(fileDir, "r");
	if(fp == NULL){
	printf("file can't read\n");
	fclose(fp);
	return -1;
	}

	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if(fread(fileBuf, 1, file_size, fp) == -1){
	printf("fread error\n");
	}
	// make body
	sprintf(pre_body, "--%s\r\nContent-Disposition: form-data; name=\"pdata\"\r\n\r\n"
	"%s\r\n--%s\r\nContent-Disposition: form-data; name=\"flag\"\r\n\r\n"
	"%s\r\n--%s\r\nContent-Disposition: form-data; name=\"upfile\";filename=\"%s\"\r\n"
	"Content-Type:application/octect-stream\r\n\r\n"
	,  boundary, "postdata", boundary,"no", boundary, filename);

	sprintf(post_body, "\r\n--%s--\r\n", boundary);

	int pre_len = strlen(pre_body);
	int post_len = strlen(post_body);
	int body_len = pre_len + file_size + post_len;

	memcpy(bodyline, pre_body, pre_len);
	memcpy(bodyline+pre_len, fileBuf, file_size);
	memcpy(bodyline+pre_len+file_size, post_body, post_len);

	//make header
	sprintf(sendline, "POST /postImage/%c HTTP/1.1\r\n"
	"Host: %s\r\nConnection: keep-alive\r\n"
	"Content-Length: %d\r\n"
	"Content-Type: multipart/form-data; boundary=%s\r\n\r\n", sensorNum, servIP, body_len, boundary);
	int head_len = strlen(sendline);
	int packet_len = head_len+body_len;

	//join header + body
	packet=calloc(1, head_len+body_len+1);
	memcpy(packet, sendline, head_len);
	memcpy(packet+head_len, bodyline, body_len);


	write(sock, packet, packet_len);

	fclose(fp);
	free(packet);
	close(sock);
	return 0;
}

