#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc, char * argv[]) {
    printf("SERVER :\n");
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd != -1) {
        struct sockaddr_in servaddr, client;
        bzero( & servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(atoi(argv[1]));
        char buffer[1024];
        bzero( & buffer, sizeof(buffer));
        char dip[20], sip[20], smac[20];
        char d[20];
        printf("\nEnter the details of received packet --");
        printf("\nDest IP: ");
        scanf("%s", dip);
        printf("\nSource IP: ");
        scanf("%s", sip);
        printf("\nSource MAC: ");
        scanf("%s", smac);
        printf("\n16 bit d: ");
        scanf("%s", d);
        printf("\nDeveloping ARP Request packet");
        buffer[0] = '1';
        buffer[1] = '|';
        int p = 2;
        for (int j = 0; j < strlen(smac); j++) {
            buffer[p] = smac[j];
            p++;
        }
        buffer[p] = '|';
        ++p;
        for (int j = 0; j < strlen(sip); j++) {
            buffer[p] = sip[j];
            p++;
        }
        buffer[p] = '|';
        ++p;
        char z[20] = "00-00-00-00-00-00";
        for (int j = 0; j < strlen(z); j++) {
            buffer[p] = z[j];
            p++;
        }
        buffer[p] = '|';
        ++p;
        for (int j = 0; j < strlen(dip); j++) {
            buffer[p] = dip[j];
            p++;
        }



        if ((bind(sockfd, (struct sockaddr * ) & servaddr, sizeof(servaddr))) == 0) {
            if ((listen(sockfd, 3)) == 0) {
                int len;
                len = sizeof(client);
                int z = 0;
                char reply[10];
                while (z < 3) {
                    int a;
                    a = accept(sockfd, (struct sockaddr * ) & client, & len);
                    if (a > 0) {
                        char message[1024];
                        printf("\n%s", buffer);
                        printf("\nThe ARP Request packet is broadcasted.");
                        write(a, buffer, sizeof(buffer));
                        printf("\nWaiting for ARP Reply...\n");
                        read(a, reply, sizeof(reply));
                        int g = 0;
                        if ((strcmp(reply, "yes")) == 0) {
                            g = 1;
                        }
                        if (g == 1) {
                            printf("\nARP Reply received.");
                            read(a, message, sizeof(message));
                            printf("\n%s", message);
                            write(a, d, sizeof(d));
                            char to[20];
                            int x = 0;
                            printf("\nSending the packet to:\n");
                            for (int i = 2; message[i] != '|'; i++) {
                                to[x] = message[i];
                                x++;
                            }
                            printf("%s\n", to);
                        } else {
                            printf("\n");
                        }
                    } else {
                        printf("\nAcceptance failed!");
                        exit(0);
                    }
                    printf("\n----------------------------------\n");
                    z++;
                }
            }
        }
    } else {
        printf("socket creation failed");
    }
}
