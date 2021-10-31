// shashank Panda 195001102

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>

short SocketCreate(void)
{
    short hSocket;
    printf("Create the socket\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}
int SocketConnect(int hSocket)
{
    int iRetval=-1;
    int ServerPort = 90190;
    struct sockaddr_in remote= {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);
    iRetval = connect(hSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
    return iRetval;
}
int SocketSend(int hSocket,char* Rqst,short lenRqst)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;
    if(setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = send(hSocket, Rqst, lenRqst, 0);
    return shortRetval;
}
int SocketReceive(int hSocket,char* Rsp,short RvcSize)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;
    if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = recv(hSocket, Rsp, RvcSize, 0);
    printf("Response %s\n",Rsp);
    return shortRetval;
}

//main driver program
int main(int argc, char *argv[])
{   
    char userName[10]; 
    printf("\nEnter your userName: ");
    gets(userName);
    while(1)    {
        int hSocket, read_size;
        struct sockaddr_in server;
        char SendToServer[100] = {0};
        char server_reply[200] = {0};
        hSocket = SocketCreate();
        if(hSocket == -1)
        {
            printf("Could not create socket\n");
            return 1;
        }
        printf("Socket is created\n");
        //Connect to remote server
        if (SocketConnect(hSocket) < 0)
        {
            perror("connect failed.\n");
            return 1;
        }
        printf("Sucessfully conected with server\n");
        printf("Enter the Message: ");
        gets(SendToServer);
        char *finalMessage = (char*)malloc(50*sizeof(char));
        sprintf(finalMessage, "%s: %s", userName, SendToServer);
        //Send data to the server
        SocketSend(hSocket, finalMessage, strlen(finalMessage));
        close(hSocket);
        shutdown(hSocket,0);
        shutdown(hSocket,1);
        shutdown(hSocket,2);
    }
    return 0;
}