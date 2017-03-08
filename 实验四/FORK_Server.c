#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<malloc.h>

#define MAX 1024

int main(){
	int connfd, listenfd;
	socklen_t sin_size =0;
	struct sockaddr_in servaddr;
	char buffer[MAX];

	printf("Server activating...\n");

	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(10101);

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Server socket create failed.");
		return 1;
	}

	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr))<0)
	{
		perror("Server socket bind failed.");
		return 1;
	}

	listen(listenfd, 5);
	printf("Server online now.\n");

	sin_size= sizeof(servaddr);

	while(1)
	{
		if((connfd = accept(listenfd, (struct sockaddr *)&servaddr, &sin_size))<0)
		{
			perror("Server socket accept failed.");
			return 1;
		}
		
		if(0==fork())break;
		
		close(connfd);
	}
	
	close(listenfd);
	memset(buffer, 0, MAX);

	while(1)
	{		

		if(recv(connfd, buffer, MAX, 0)<0)
		{
			perror("recv error:");
			continue;
		}
		if(strcmp(buffer, "exit")==0)
		{
            		close(connfd);
            		return 1;
		}
		if(send(connfd, buffer, sizeof(buffer), 0)<0)
		{
			perror("send error:");
			continue;
		}
	}
	close(connfd);
	close(listenfd);

	return 0;
}