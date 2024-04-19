#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
// #include <sys/sem.h>
// #include <sys/wait.h>
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
    // close(fd);
    printf("finish\n");
}

// int main()
// {
//     int fd, randNum;
//     char msgBuf[BUF_SIZE];
//     if(mkfifo(NAMEDPIPE_NAME, 0660) == -1)
//     {
//         if(errno != EEXIST)
//         {
//             perror("mkfifo");
//             exit(EXIT_FAILURE);
//         }
//         else
//             printf("%s is existing, connect to this file\n", NAMEDPIPE_NAME);
        
//     }
//     printf("%s is created\n", NAMEDPIPE_NAME);

//     fd = open(NAMEDPIPE_NAME, O_RDWR);
//     if ( fd == -1 ) 
//     {
//         perror("open");
//         exit(EXIT_FAILURE);
//     }
//     printf("%s is opened to write\n", NAMEDPIPE_NAME);

//     sem_t *semRead = sem_open(PATHNAME1, O_CREAT | 0666);
//     if(errno == EEXIST)
//         semRead = sem_open(PATHNAME1, 0666);
//     if(semRead == SEM_FAILED)
//     {
//         perror("sem_open1");
//         exit(EXIT_FAILURE);
//     }

//     sem_t *semWrite = sem_open(PATHNAME2, O_CREAT | 0666);
//     if(errno == EEXIST)
//         semWrite = sem_open(PATHNAME2,  0666);
//     if(semWrite == SEM_FAILED)
//     {
//         perror("sem_open2");
//         exit(EXIT_FAILURE);
//     }

//     srand(time(NULL));
//     sem_wait(semWrite);
//     sem_wait(semWrite);
//     sem_wait(semWrite);
//     sem_wait(semWrite);
//     sem_wait(semWrite);
//     sem_wait(semWrite);
//     sem_wait(semWrite);
//     for (int i = 0; i < 10; i++)
//     {
//         sem_wait(semWrite);
//         randNum = rand() % 1000;
//         sprintf(msgBuf, "%d", randNum);
//         write(fd, msgBuf, BUF_SIZE);
//         printf("writeProg send: %s\n", msgBuf);
//         sem_post(semRead);
//     }
//     sem_post(semRead);
    

//     sem_unlink(PATHNAME1);
//     sem_unlink(PATHNAME2);
//     close(fd);
//     printf("finish\n");
// }

// int main()
// {
//     int fd, randNum;
//     char msgBuf[BUF_SIZE];
//     if(mkfifo(NAMEDPIPE_NAME, 0770) == -1)
//     {
//         if(errno != EEXIST)
//         {
//             perror("mkfifo");
//             exit(EXIT_FAILURE);
//         }
//         else
//             printf("%s is existing, connect to this file\n", NAMEDPIPE_NAME);
        
//     }
//     printf("%s is created\n", NAMEDPIPE_NAME);

//     fd = open(NAMEDPIPE_NAME, O_RDWR);
//     if ( fd == -1 ) 
//     {
//         perror("open");
//         exit(EXIT_FAILURE);
//     }
//     printf("%s is opened to write\n", NAMEDPIPE_NAME);

//     // Создание семафора для считывания данных
//     key_t key1 = ftok(PATHNAME, 0);
//     int semRead = semget(key1, 1, IPC_CREAT | 0660);
//     if (semRead == -1) {
//         perror("semget");
//         exit(1);
//     }

//     // Создание семафора для записи данных
//     key_t key2 = ftok(PATHNAME, 1);
//     int semWrite = semget(key2, 1, IPC_CREAT | 0660);
//     if (semWrite == -1) {
//         perror("semget");
//         exit(1);
//     }

//     // Установка начальный значений семафоров
//     if (semctl(semRead, 0, SETVAL, 0) == -1 || semctl(semWrite, 0, SETVAL, 0) == -1) 
//     {
//         perror("semctl");
//         exit(1);
//     }

//     srand(time(NULL));
//     for (int i = 0; i < 10; i++)
//     {
//         P(semWrite);
//         randNum = rand() % 1000;
//         sprintf(msgBuf, "%d", randNum);
//         write(fd, msgBuf, BUF_SIZE);
//         printf("writeProg send: %s\n", msgBuf);
//         V(semRead);
//     }
//     V(semRead);
    
//     close(fd);
//     exit(EXIT_SUCCESS);
// }