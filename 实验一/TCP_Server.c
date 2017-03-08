#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<malloc.h>

#define MAX 1024

int main(){
	int listenfd, cli_socketfd;
	socklen_t sin_size =0;
	struct sockaddr_in servaddr;
	//char* send_buffer = (char *)malloc(MAX);
	//char* recv_buffer = (char *)malloc(MAX);
	char send_buffer[MAX];
	char recv_buffer[MAX];

	printf("Server activating...\n");

	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(10101);

	if((cli_socketfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Server socket create failed.");
		return 1;
	}

	if(bind(cli_socketfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr))<0)
	{
		perror("Server socket bind failed.");
		return 1;
	}

	listen(cli_socketfd, 5);
	printf("Server online now.\n");

	sin_size= sizeof(servaddr);

	if((listenfd = accept(cli_socketfd, (struct sockaddr *)&servaddr, &sin_size))<0)
	{
		perror("Server socket accept failed.");
		return 1;
	}

	memset(send_buffer, 0, MAX);
	memset(recv_buffer, 0, MAX);

	while(1)
	{		

		if(recv(listenfd, recv_buffer, MAX, 0)<0)
		{
			printf("recv error, please check your connection.\n");
			continue;
		}
		printf("Client: %s\n",recv_buffer);

		if(strcmp(recv_buffer, "exit")==0)
		{
            		close(cli_socketfd);
            		close(listenfd);
            		return 1;
		}

		printf("Server: ");
		//scanf("%s",send_buffer);
		gets(send_buffer);

		if(send(listenfd, send_buffer, sizeof(send_buffer), 0)<0)
		{
			printf("send error, please try again later.\n");
			continue;
		}


		//memset(send_buffer, 0, MAX);
		//memset(recv_buffer, 0, MAX);
	}
	close(cli_socketfd);
	close(listenfd);

	return 0;
}