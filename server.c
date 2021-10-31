#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main() 	{

	int sockfd, newfd, n, m;
	struct sockaddr_in serveraddr, cliaddr;
	char buffer[1024];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sockfd<0 )	{
		printf("Socket Creation Failed\n"); 
		exit(0); 
	}
	printf("Socket created successfully\n"); 
	bzero(&serveraddr, sizeof(serveraddr)); 
	serveraddr.sin_family = AF_INET; 
	serveraddr.sin_addr.s_addr = INADDR_ANY; 
	serveraddr.sin_port = htonl(9111);
	if ( bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )	{
		printf("Bind failed\n"); 
		exit(0); 
	}
	printf("Binded successfully\n"); 
	listen(sockfd, 2);
	n = sizeof(cliaddr);
	newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &n); 
	if (newfd < 0)	{
		printf("Accept failed\n"); 
		exit(0); 
	}
	printf("Accepted successfully\n"); 
	m = read(newfd, buffer, sizeof(buffer)); 
	printf("Message from client: %s\n", buffer); 
	m = write(newfd, buffer, sizeof(buffer)); 
	printf("Message sent to client\n"); 
	close(sockfd); 
	close(newfd); 

	return 0;
}
