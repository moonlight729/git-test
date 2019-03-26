#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9999
#define IP   "192.168.9.86"

int main()
{
	/*窗帘action_id = 25*/
	char * str = "{\"action_id\":1,\"msg_type\":105,\"action_params\":\"AQ\\u003d\\u003d\",\"dev_id\":25}";
	printf("%s\n strlen(str) = %d\n",str,strlen(str));


	int sockfd = -1;
	int payload_len = 74;
	unsigned char sendBuf[128] = {'\0'};
	unsigned char payloadBuf[4] = {0};
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( -1 == sockfd ) {
		perror( "sock created" );
		exit( -1 );
	}

	struct sockaddr_in server;    
	memset( &server, 0, sizeof( struct sockaddr_in ) );
	server.sin_family = AF_INET;
	server.sin_port =htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);

	int res = -1;    
	res = connect( sockfd, (struct sockaddr*)&server, sizeof( server ) );
	if( -1 == res ){
		perror( "sock connect" );
		exit( -1 );
	}else{
		printf("connect OK...\n");
	}
	// 填充sendBuf
	// 大端 
	memcpy(sendBuf,"##",2);
	sendBuf[2] = (char)((payload_len >> 24) & 0xFF);
	sendBuf[3] = (char)((payload_len >> 16) & 0xFF);
	sendBuf[4] = (char)((payload_len >> 8) & 0xFF);
	sendBuf[5] = (char)(payload_len & 0XFF);
	memcpy(sendBuf+2+4,str,strlen(str));
	memcpy(sendBuf +2+4+strlen(str),"!!",2);
	printf("strlen sendBuf is %d\n",strlen(sendBuf));
	for(int i = 0; i < 128;i++){
		printf("%02x ",sendBuf[i]);
	}
	printf("\n");
	printf("head: %s\n",sendBuf);
	printf("payload_len: %02x %02x %02x %02x\n",sendBuf[2],sendBuf[3],sendBuf[4],sendBuf[5]);
	printf("payload: %s\n",sendBuf+6);
	printf("end: %s\n",sendBuf+6+strlen(str));
	while(1){
		sleep(2);
		write(sockfd,sendBuf,6+74+2);
		printf("send...\n");
	}
	close(sockfd);
	return 0;
}
