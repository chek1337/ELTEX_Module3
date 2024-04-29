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

int main()
{
    int sockfd;
    socklen_t clilen;
    ssize_t n; 
    char sendline[1000], recvline[1000];
    struct sockaddr_in client1, client2;

    bzero(&client1, sizeof(client1));
    client1.sin_family = AF_INET;
    client1.sin_port = htons(2024);
    client1.sin_addr.s_addr = htonl(INADDR_ANY);

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        myperr("socket");

    if(bind(sockfd, (struct sockaddr *) &client1, sizeof(client1)) < 0)
    {
        close(sockfd);
        myperr("bind");
    }

    printf("Server started\n");

    char hostbuffer[256];
	char *IPbuffer;
	struct hostent *host_entry;
	int hostname;

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
	if(hostname == -1)
        myperr("gethostname");

    host_entry = gethostbyname(hostbuffer);
	if( host_entry== NULL)
        myperr("gethostbyname");

	IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

	printf("Hostname: %s\n", hostbuffer);
	printf("Host IP: %s\n", IPbuffer);

    clilen = sizeof(client2);

    while(1) 
    {
        printf("=> ");
        fgets(sendline, 1000, stdin);
        
        
        if((n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &client2, &clilen)) < 0)
        {
            close(sockfd);
            myperr("recvfrom");
        }

        printf("%s\n", recvline);

        if(sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *) &client2, clilen) < 0)
        {
            close(sockfd);
            myperr("sendto");
        }
    }
    return 0;
}