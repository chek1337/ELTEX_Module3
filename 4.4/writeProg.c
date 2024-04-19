#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define NAMEDPIPE_NAME "/tmp/namedpipe"
#define BUF_SIZE 128
#define SEMNAME1 "sem1"
#define SEMNAME2 "sem2"

void myperr(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
int main()
{    
    if(mkfifo(NAMEDPIPE_NAME, 0660) == -1)
        if(errno != EEXIST)
            myperr("mkfifo");
    errno = 0;    

    printf("%s is created\n", NAMEDPIPE_NAME);

    int fd = open(NAMEDPIPE_NAME, 0666);
    if(fd == -1)
        myperr("open");
        
    sem_t *semToRead = sem_open(SEMNAME1, O_CREAT, 0600, 0);
    if(errno == EEXIST)
        semToRead = sem_open(SEMNAME1, 0600);
    if(semToRead == SEM_FAILED)
        myperr("sem_open1");
  

    sem_t *semToWrite = sem_open(SEMNAME2, O_CREAT, 0600, 0);
    if(errno == EEXIST)
        semToWrite = sem_open(SEMNAME2, 0600);
    if(semToWrite == SEM_FAILED)
        myperr("sem_open1");
    
    char msgBuf[BUF_SIZE];
    int randNum;
    for (int i = 0; i < 10; i++)
    {
        sem_wait(semToWrite);
        randNum = rand() % 1000;
        sprintf(msgBuf, "%d", randNum);
        write(fd, msgBuf, BUF_SIZE);
        printf("writeProg send: %s\n", msgBuf);
        sem_post(semToRead);
    }
    
    sem_unlink(SEMNAME1);
    sem_unlink(SEMNAME2);
    close(fd);
    printf("finish\n");
}

