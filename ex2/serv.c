#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()  {

    // socket creation

    int hsocket;
    if (hsocket = socket(AF_INET, SOCK_STREAM, 0) == -1)  {
        printf("Could not create the socket. Exiting...");
        return 0;
    }

    // Binding

    int PORT = 91042;
    struct sockaddr_in remote = {0};
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(hsocket, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
        printf("Could not bind. Exitting...");
        return 0;
    }

    char client_message[200] = {0};
    char message[100] = {0};
    listen(hsocket, 3);
    int sock, clientLen = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    while(1)    {
        sock = accept(hsocket, (struct sockaddr *)&client, (socklen_t*)&clientLen);
        if (sock < 0)   {
            printf("accept failed");
            return 0;
        }
        printf("Connection accepted\n");
        memset(client_message, '\0', sizeof(client_message));
        memset(message, '\0', sizeof(message));
        if (recv(sock, client_message, 200, 0) < 0) {
            printf("recv failed. ");
            break;
        }
        printf("Client reply: %s\n", client_message);
        strcpy(message, client_message);
        if (send(sock, message, strlen(message), 0) < 0)    {
            printf("Message send failed.");
            break;
        }
        close(sock);
        sleep(1);
    }
    return 0;
}