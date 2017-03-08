#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<malloc.h>

#define MAX 1024

int main(int argc, char** argv){
	char IP[20] = {0};
	int SERVERPORT;
	int ser_socketfd, n = 0;
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(struct sockaddr);
	char send_buffer[MAX];
	char recv_buffer[MAX];

	// printf("please input server's IP: ");
	// scanf("%s", IP);
	// setbuf(stdin,NULL);
	// printf("please input server's Port: ");
	// scanf("%d", &SERVERPORT);

	printf("Connecting...\n");

	memset(&remote_addr, 0, sizeof(struct sockaddr_in));
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&remote_addr.sin_addr);


	if((ser_socketfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		perror("Client socket create failed.");
		return 1;
	}
	
	printf("Connecting succeed!\n");

	memset(&send_buffer, 0, MAX);
	memset(&recv_buffer, 0, MAX);

	while(1)
	{
		printf("Client: ");
		//scanf("%s",send_buffer);
		gets(send_buffer);

		if(sendto(ser_socketfd, send_buffer, strlen(send_buffer), 0, (struct sockaddr* )&remote_addr, sizeof(remote_addr))<0)
		{
			perror("send error:");
			continue;
		}

		if(strcmp(send_buffer, "exit")==0)
		{
            		close(ser_socketfd);
            		return 0;
		}
		memset(&send_buffer, 0, MAX);
		if((n = recvfrom(ser_socketfd, recv_buffer, MAX, 0, (struct sockaddr* )&remote_addr, &remote_addr_len))<0)
		{
			perror("recv error:");
			continue;
		}
		else{
            recv_buffer[n] = '\0';
            printf("Server: %s\n", recv_buffer);
		}
        memset(&recv_buffer, 0, MAX);

		//memset(&recv_buffer, 0, MAX);
		//memset(&send_buffer, 0, MAX);
	}
	close(ser_socketfd);

	return 0;
}



