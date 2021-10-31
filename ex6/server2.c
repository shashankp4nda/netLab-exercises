#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<sys/time.h>
#include<arpa/inet.h>

#define DELIM "."

typedef struct dns{
    char domain[30];
    char address[3][30];
}dns;

// To Check if a string is a valid IP address

/* return 1 if string contain only digits, else return 0 */
int valid_digit(char *ip_str)
{
    while (*ip_str) {
        if (*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else
            return 0;
    }
    return 1;
}
 
/* return 1 if IP string is valid, else return 0 */
int is_valid_ip(char *ip_str)
{
    int i, num, dots = 0;
    char *ptr;
 
    if (ip_str == NULL)
        return 0;
 
    ptr = strtok(ip_str, DELIM);
 
    if (ptr == NULL)
        return 0;
 
    while (ptr) {
 
        /* after parsing string, it must contain only digits */
        if (!valid_digit(ptr))
            return 0;
 
        num = atoi(ptr);
 
        /* check for valid IP */
        if (num >= 0 && num <= 255) {
            /* parse remaining string */
            ptr = strtok(NULL, DELIM);
            if (ptr != NULL)
                ++dots;
        } else
            return 0;
    }
 
    /* valid IP string must contain 3 dots */
    if (dots != 3)
        return 0;
    return 1;
}


int main(){
	int sockfd,len,recvfd=1,a,b,inc=1, counter=3;
	char rep;
	dns table[10];
	bzero(&table,10*sizeof(dns));

	struct sockaddr_in myaddr,clnt;

	//creating socket
	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	if(sockfd==-1){
		printf("Socket creation failed!\n");
		exit(0);
	}
	else{
		printf("Socket created...\n");
	}
	struct timeval tv;
	tv.tv_sec = 10;
	fd_set readfds;
	FD_ZERO(&readfds);
    //clear address pointer
	bzero(&myaddr,sizeof(myaddr));
	//assign IP,port
	myaddr.sin_family=AF_INET;
	//assign IPv4
	myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//htonl->translate address host to network long
	//INADDR_ANY->allows server to recieve packets from any of the targeted interfaces
	myaddr.sin_port=htons(8080);
	//translate unsigned short integer to network byte order

	//bind newly created socket to given IP
    if(bind(sockfd,(struct sockaddr *)&myaddr,sizeof(myaddr)) !=0){
    	printf("Binding failed!\n");
		exit(0);
    }
    else{
    	printf("Binding...\n");
    }
    strcpy(table[0].domain,"www.yahoo.com");
    strcpy(table[0].address[0],"10.2.45.67");
    strcpy(table[1].domain,"www.annauniv.edu");
    strcpy(table[1].address[0],"197.34.53.122");
    strcpy(table[2].domain,"www.google.com");
    strcpy(table[2].address[0],"142.89.78.66");

    printf("\nDomain Name\t\t|\tIP Address\t|\n");

    for(int i=0;i<3;i++){
    	printf("%-15s   \t|%-20s\t|\n",table[i].domain,table[i].address);
    }
	if(fork()==0)	{	
		char dn[20], ip[20];
		printf("\ndo you want to modify? (y/n): ");
		scanf(" %c", &rep);
		while(rep == 'y')	{
			printf("\nEnter the Domain name for new entry: ");
			scanf("%s", dn);
			printf("Enter the IP Address for \'%s\': ", dn);
			scanf("%s", ip);

			if(is_valid_ip(ip))		{
				strcpy(table[counter].domain, dn);
				strcpy(table[counter].address[0], ip);
				counter++;
				printf("\nDomain Name\t\t|\tIP Address\t|\n");
				for(int i=0;i<counter;i++){
					printf("%-15s   \t|%-20s\t|\n",table[i].domain,table[i].address[0]);
				}
			}

			else
				printf("\nPlease enter a valid IP address.\n");

			printf("\ndo you want to modify? (y/n): ");
			scanf(" %c", &rep);

		}
	}
	else	{

		len=sizeof(clnt);
		while(recvfd!=0){
		char buffer[1000];
		FD_ZERO(&readfds);
		FD_SET(sockfd,&readfds);
		recvfd=select(5,&readfds,NULL,NULL,&tv);

		for(int i=0;i<5;i++){
			if(FD_ISSET(i,&readfds)){
				int flag=0;
				tv.tv_sec=6;
				bzero(&clnt,len);
				a=recvfrom(i,buffer,1000,0,(struct sockaddr *)&clnt,&len);
				b=ntohs(clnt.sin_port);

					for(int j=0;j<4;j++){
						if(strcmp(buffer,table[j].domain)==0){
							bzero(buffer,1000);
							strcpy(buffer,table[j].address[0]);
							sendto(i, buffer, 1000, MSG_CONFIRM, (struct sockaddr *) &clnt, len);
							flag=1;
							break;
						}
					}
					if(flag==0){
						bzero(buffer,1000);
						strcpy(buffer," couldn't find any valid domain.");
						sendto(i, buffer, 1000, MSG_CONFIRM, (struct sockaddr *) &clnt, len);
					}

					FD_CLR(sockfd, &readfds);
				}
			}
		}
	}
	close(sockfd);
	return 0;
}

