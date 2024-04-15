#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define NAMEDPIPE_NAME "/tmp/named_pipe"
#define BUF_SIZE 128
int main()
{
    int fd;
    char msgBuf[BUF_SIZE];

    if ((fd = open(NAMEDPIPE_NAME, O_RDONLY)) <= 0 ) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("%s is opened to read\n", NAMEDPIPE_NAME);

    ssize_t endofile = 1;
    for (int i = 0; ; i++)
    {
        endofile = read(fd, msgBuf, BUF_SIZE);
        if(endofile == 0)
            break;
        printf("readProg get: %s\n", msgBuf);
        sleep(2);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}