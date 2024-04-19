#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define NAMEDPIPE_NAME "/tmp/named_pipe"
#define BUF_SIZE 128

#define PATHNAME "/tmp"

void P(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;
    semop(semid, &buf, 1);
}

// V-операция для открытия семафора
void V(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    semop(semid, &buf, 1);
}   

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

    fd = open(NAMEDPIPE_NAME, O_RDWR);
    if ( fd == -1 ) 
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("%s is opened to write\n", NAMEDPIPE_NAME);

    // Создание семафора для считывания данных
    key_t key1 = ftok(PATHNAME, 0);
    int semRead = semget(key1, 1, IPC_CREAT | 0660);
    if (semRead == -1) {
        perror("semget");
        exit(1);
    }

    // Создание семафора для записи данных
    key_t key2 = ftok(PATHNAME, 1);
    int semWrite = semget(key2, 1, IPC_CREAT | 0660);
    if (semWrite == -1) {
        perror("semget");
        exit(1);
    }

    // Установка начальный значений семафоров
    if (semctl(semRead, 0, SETVAL, 0) == -1 || semctl(semWrite, 0, SETVAL, 0) == -1) 
    {
        perror("semctl");
        exit(1);
    }

    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        P(semWrite);
        randNum = rand() % 1000;
        sprintf(msgBuf, "%d", randNum);
        write(fd, msgBuf, BUF_SIZE);
        printf("writeProg send: %s\n", msgBuf);
        V(semRead);
    }
    V(semRead);
    
    close(fd);
    exit(EXIT_SUCCESS);
}