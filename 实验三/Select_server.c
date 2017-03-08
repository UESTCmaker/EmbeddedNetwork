#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<malloc.h>

#define MAX 1024
#define FD_SIZE 100

int main(){
	int listenfd, sockfd, maxfd, connfd, maxi, i, client[FD_SIZE];
	fd_set rset, allset;
	socklen_t sin_size =0;
	struct sockaddr_in servaddr, cliaddr;
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

	sin_size = sizeof(cliaddr);

	FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    maxi=-1;
    for(i=0;i<FD_SIZE;i++){
        client[i]=-1;
    }

	memset(buffer, 0, MAX);

	while(1){
        rset=allset;
		select(maxfd+1, &rset, NULL, NULL, NULL);
		if(FD_ISSET(listenfd, &rset))
		{
			if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &sin_size))<0)
			{
				perror("Server socket accept failed.");
				return 1;
			}
			for(i=0;i<FD_SIZE;i++){
                if(client[i]<0){
                    client[i]=connfd;
                    break;
                }
            }
            if(i==FD_SIZE){
                printf("Server full...");
                close(connfd);
                continue;
            }
            if(i>maxi){
                maxi=i;
            }
			maxfd = connfd > maxfd ? connfd : listenfd;
			FD_SET(connfd, &allset);
			continue;
		}
		for(i=0;i<=maxi;i++){
            if(client[i]>0)
            {
                sockfd=client[i];
                if(FD_ISSET(sockfd, &rset))
                {
                    if(recv(sockfd, buffer, MAX, 0)<0)
                    {
                        perror("recv error:");
                        FD_CLR(sockfd,&allset);
                        close(sockfd);
                        client[i]=-1;
                        continue;
                    }
                    if(strcmp(buffer, "exit")==0)
                    {
                        FD_CLR(sockfd,&allset);
                        close(sockfd);
                        client[i]=-1;
                        continue;
                    }
                    if(send(sockfd, buffer, sizeof(buffer), 0)<0)
                    {
                        perror("send error:");
                        FD_CLR(sockfd,&allset);
                        close(sockfd);
                        client[i]=-1;
                        continue;
                    }
                }
            }
		}

	}

	return 0;
}

