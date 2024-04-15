#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define NAMEDPIPE_NAME "/tmp/named_pipe"
#define BUF_SIZE 128
int main()
{
    int fd, randNum;
    char msgBuf[BUF_SIZE];
    if(mkfifo(NAMEDPIPE_NAME, 0770) == -1)
    {
        if(errno != EEXIST)
        {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
        else
            printf("%s is existing, connect to this file\n", NAMEDPIPE_NAME);
        
    }
    printf("%s is created\n", NAMEDPIPE_NAME);

    if ((fd = open(NAMEDPIPE_NAME, O_WRONLY)) <= 0 ) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("%s is opened to write\n", NAMEDPIPE_NAME);

    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        randNum = rand() % 1000;
        sprintf(msgBuf, "%d", randNum);
        write(fd, msgBuf, BUF_SIZE);
        printf("writeProg send: %s\n", msgBuf);
        sleep(1);
    }
    
    close(fd);
    exit(EXIT_SUCCESS);
}

/*

# Что будет если запустить несколько экземпляров каждой программы?

Программа на записывание (writeProg.c) выведет следущие данные

zeuslinux@zeuslinux:~$ ./write
/tmp/named_pipe is existing, connect to this file
/tmp/named_pipe is created
/tmp/named_pipe is opened to write
writeProg send: 320
writeProg send: 996
writeProg send: 70
writeProg send: 667
writeProg send: 543
writeProg send: 186
writeProg send: 587
writeProg send: 290
writeProg send: 954
writeProg send: 371
zeuslinux@zeuslinux:~$

***********************************************************************

Первая программа на считывание (readProg.c) выведет следущие данные

zeuslinux@zeuslinux:~$ ./read
/tmp/named_pipe is opened to read
readProg get: 70
readProg get: 543
readProg get: 587
readProg get: 954
zeuslinux@zeuslinux:~$ 


***********************************************************************

Вторая программа на считывание (readProg.c) выведет следущие данные

zeuslinux@zeuslinux:~$ ./read
/tmp/named_pipe is opened to read
readProg get: 320
readProg get: 996
readProg get: 667
readProg get: 186
readProg get: 290
readProg get: 371
zeuslinux@zeuslinux:~$ 

*/

