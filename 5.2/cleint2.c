//  Здесь client2 выступает в роли клиента, который полключается к серверу client1

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <ifaddrs.h>
#include <netdb.h>

void myperr(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) 
{
    int sockfd;
    ssize_t n; 
    char sendline[1000], recvline[1000];
    struct sockaddr_in cleint1, cleint2;

    if(argc != 2)
        myperr("Wrong nu,ber of arg!");

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        myperr("socket");

    bzero(&cleint2, sizeof(cleint2));
    cleint2.sin_family = AF_INET;
    cleint2.sin_port = htons(1024);
    cleint2.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *) &cleint2, sizeof(cleint2)) < 0)
    {
        close(sockfd);
        myperr("bind");
    }

    bzero(&cleint1, sizeof(cleint1));
    cleint1.sin_family = AF_INET;
    cleint1.sin_port = htons(2024);

    if(inet_aton(argv[1], &cleint1.sin_addr) == 0) 
    {
        close(sockfd);
        myperr("inet_aton");
    }

    while(1)
    {
        printf("=> ");
        fgets(sendline, 1000, stdin);
        
        if(sendto(sockfd, sendline, strlen(sendline)+1, 0, (struct sockaddr *) &cleint1, sizeof(cleint1)) < 0)
        {
            close(sockfd);
            myperr("sendto");
        }

        if((n = recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr *) NULL, NULL)) < 0)
        {
            close(sockfd);
            myperr("recvfrom");
        }

        printf("%s\n", recvline);
    }
}