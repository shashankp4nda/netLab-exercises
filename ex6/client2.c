#include<unistd.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(){
	int sockfd,len,n;
	char buffer[1000];
	struct sockaddr_in myaddr,clnt;

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1){
		printf("Socket creation failed!\n");
		exit(0);
	}
	else{
		printf("Socket created...\n");
	}
	bzero(&myaddr,sizeof(myaddr));

	myaddr.sin_family=AF_INET;
	myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	//127.0.0.1 is the standard address for IPv4
	myaddr.sin_port=htons(8080);

	len=sizeof(clnt);
	getsockname(sockfd, (struct sockaddr *)&clnt,&len);
    n=sizeof(myaddr);
	printf("Domain Name:");
	scanf("%[^\n]",&buffer);
	getchar();
    sendto(sockfd,buffer,1000,0,(struct sockaddr *)&myaddr,n);
    bzero(buffer,1000);
	recvfrom(sockfd,buffer,1000,0,(struct sockaddr *)&myaddr,&n);
	printf("IP Address: %s\n",&buffer);
	close(sockfd);
	return 0;
}

