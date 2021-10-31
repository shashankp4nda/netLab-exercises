#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<ctype.h>
  
#define PORT     8080
#define MAXLINE 1024
  
#define DELIM "."

struct DNSInfo  {
    char IP[20]; 
    char URL[20];
};
 
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

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *messageToClient;
    struct sockaddr_in servaddr, cliaddr;
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    } 
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT); 
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    struct DNSInfo DNStable[10] = {
        (struct DNSInfo){.IP = "10.2.45.67", .URL = "www.yahoo.com"},
        (struct DNSInfo){.IP = "197.34.53.122",.URL =  "www.annauniv.com"}, 
        (struct DNSInfo){.IP = "142.89.78.66",.URL = "www.google.com"}
    };
    int len, n, counter = 3; 
    char rep, reqIP[20];
    char IP[20], URL[20];
    for(int x = 0; x < 20; x++)   {
        rep = 'n';
        printf("\n\n--------DNS Menu--------");  
        int choice; 
        printf("\n\nEnter choice:\n1) Display DNS Table\n2) Create new Entry for DNS Table\n3) Accept request\n");
        scanf("%d", &choice);
        switch(choice)  {
            case 1: 
                printf("\n\nServer Name\t\tIP Address\n-----------------------------------");
                for(int i = 0; i < counter; i++ )   {
                    printf("\n%s    \t%s", DNStable[i].URL, DNStable[i].IP);
                }
                break;
            case 2: 
                printf("\nEnter the Domain Name: "); 
                scanf("%s", URL);
                printf("Enter the IP Address: "); 
                scanf("%s", IP);
                if(!is_valid_ip(IP))
                    printf("\nError: Invalid IP, registration failed.\n");
                else    {
                    struct DNSInfo newEntry;
                    strcpy(newEntry.IP, IP);
                    strcpy(newEntry.URL, URL);
                    DNStable[counter++] = newEntry;
                    printf("\nDNS registered successfully\n");
                }
                break;
            case 3: 
                len = sizeof(cliaddr);  //len is value/resuslt
                n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
                // buffer[n] = '\0';
                // printf("\nClient Request: ");  
                // puts(buffer);
                for(int i = 0; i < n-4; i++)   
                    reqIP[i] = buffer[i];
                printf("Client request: %s", reqIP);
                messageToClient = "No domain was found with the given name";
                for(int i = 0; i < counter; i++ )   {
                    if(strcmp(DNStable[i].URL, buffer)==0)   {
                        strcpy(messageToClient, DNStable[i].IP);
                        break;
                    }
                }
                printf("Message to client: %s", messageToClient);
                sendto(sockfd, (char *)messageToClient, strlen(messageToClient), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len); 
                break;
            default: 
                printf("\nPlease enter a valid option.\n");
        }
    }    
    return 0;
}
