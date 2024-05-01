/* Простой пример UDP-сервера для сервиса echo */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sockfd;
    int clilen, n, portno;
    char line[1000];
    struct sockaddr_in servaddr, cliaddr;

    if(argc != 2)
    {
        printf("Port not writed\n");
        exit(1);
    }

    portno = atoi(argv[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(portno);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }

    if(bind(sockfd, (struct sockaddr *) &servaddr, 
    sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    printf("Server started\n");
    while(1) {
        clilen = sizeof(cliaddr);

        if((n = recvfrom(sockfd, line, 999, 0, 
        (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        }

        printf("%s\n", line);

        if(sendto(sockfd, line, strlen(line), 0, 
        (struct sockaddr *) &cliaddr, clilen) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        } 
    }
    return 0;
}