#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    char buffer[1024];
    char recvdata[20];
    char ipadd[35], macadd[35];
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("\nSocket creation failed!\n");
        exit(0);
    }
    else
    {
        struct sockaddr_in servaddr;
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(atoi(argv[1]));
        int v;
        v = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        if (v == 0)
        {
            char message[1024];
            char add[20];
            printf("\nEnter the ip address:");
            scanf("%s", ipadd);
            printf("\nEnter the Mac address:");
            scanf("%s", macadd);
            printf("\nARP Request Received:\n");
            read(sockfd, buffer, sizeof(buffer));
            printf("%s", buffer);
            int len = strlen(buffer);
            int j = 0;
            for (int i = (len - 1); i >= 0; i--)
            {
                if (buffer[i] == '|')
                    break;
                else
                {
                    add[j] = buffer[i];
                    j++;
                }
            }
            int h = 0;
            int d = strlen(add);
            char add1[d + 1];
            for (int i = (d - 1); i >= 0; i--)
            {
                add1[h] = add[i];
                h++;
            }
            if ((strcmp(ipadd, add1)) == 0)
            {
                printf("\nIP address match");
                char reply[10] = "yes";
                printf("\n");
                write(sockfd, reply, sizeof(reply));
                printf("\nARP Reply Sent:\n");
                message[0] = '2';
                message[1] = '|';
                int p = 2;
                for (int i = 0; i < strlen(macadd); i++)
                {
                    message[p] = macadd[i];
                    p++;
                }
                message[p] = '|';
                ++p;
                for (int i = 0; i < strlen(ipadd); i++)
                {
                    message[p] = ipadd[i];
                    p++;
                }
                message[p] = '|';
                ++p;
                int c = 0;
                for (int i = 2; i < strlen(buffer) && c < 2; i++)
                {
                    if (buffer[i] == '|')
                        c++;
                    if (c == 2)
                        break;
                    message[p] = buffer[i];
                    p++;
                }
                printf("%s", message);
                write(sockfd, message, sizeof(message));
                read(sockfd, recvdata, sizeof(recvdata));
                char k[20];
                int l = 0;
                printf("\nReceived the data packet from:\n");
                for (int i = 2; i < strlen(buffer); i++)
                {
                    if (buffer[i] == '|')
                        break;
                    k[l] = buffer[i];
                    l++;
                }
                k[l] = '\0';
                printf("%s", k);
                printf("\n");
            }
            else
            {
                printf("\nip address does not match\n");
                char reply1[10] = "no";
                write(sockfd, reply1, sizeof(reply1));
                printf("\n");
            }
        }
        else
        {
            printf("\nConnection failed!");
            exit(0);
        }
    }
    return 0;
}
