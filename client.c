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
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	serveraddr.sin_port = htons(9111);
	if ( connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )	{
		printf("Connect failed\n"); 
		exit(0); 
	}
	printf("COnnected successfully\n");
	printf("Enter the message to send to server: "); 
	scanf("%s", buffer);
	m = write(sockfd, buffer, sizeof(buffer)); 
	printf("Message sent to server: %s\n", buffer); 
	m = read(newfd, buffer, sizeof(buffer)); 
	printf("Message from server: %s\n", buffer); 
	close(sockfd); 

	return 0;
}
