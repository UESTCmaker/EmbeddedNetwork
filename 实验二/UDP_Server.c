#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<malloc.h>

#define MAX 1024

int main(){
	int  cli_socketfd, n;
	socklen_t sin_size =0;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t client_addr_len = sizeof(struct sockaddr);
	char send_buffer[MAX];
	char recv_buffer[MAX];

	printf("Server activating...\n");

	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(10101);

	if((cli_socketfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		perror("Server socket create failed.");
		return 1;
	}

	if(bind(cli_socketfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr))<0)
	{
		perror("Server socket bind failed.");
		return 1;
	}

	printf("Server online now.\n");

	sin_size= sizeof(servaddr);

	memset(send_buffer, 0, MAX);
	memset(recv_buffer, 0, MAX);

	while(1)
	{

		if((n = recvfrom(cli_socketfd, recv_buffer, MAX, 0, (struct sockaddr*)&cliaddr, &client_addr_len))<0)
		{
			perror("recv error:");
			continue;
		}
		else{
            recv_buffer[n] = '\0';
            printf("Client: %s\n",recv_buffer);
		}

		if(strcmp(recv_buffer, "exit")==0)
		{
            		close(cli_socketfd);
            		return 1;
		}
		memset(recv_buffer, 0, MAX);

		printf("Server: ");
		//scanf("%s",send_buffer);
		gets(send_buffer);

		if(sendto(cli_socketfd, send_buffer, strlen(send_buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr))<0)
		{
			perror("send error:");
			continue;
		}
        memset(send_buffer, 0, MAX);

		//memset(send_buffer, 0, MAX);
		//memset(recv_buffer, 0, MAX);
	}
	close(cli_socketfd);

	return 0;
}
