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
    int fd;
    char msgBuf[BUF_SIZE];

    fd = open(NAMEDPIPE_NAME, O_RDONLY);
    if ( fd == -1 ) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("%s is opened to read\n", NAMEDPIPE_NAME);

    // Открытие семафора для считывания данных
    int key1 = ftok(PATHNAME, 0);
    int semRead = semget(key1, 1, IPC_CREAT | 0660);
    if (semRead == -1) {
        perror("semget");
        exit(1);
    }

    // Открытие семафора для записи данных
    int key2 = ftok(PATHNAME, 1);
    int semWrite = semget(key2 , 1, IPC_CREAT | 0660);
    if (semWrite == -1) {
        perror("semget");
        exit(1);
    }

    V(semWrite);
    ssize_t endofile = 1;
    for (int i = 0; ; i++)
    {
        P(semRead);
        endofile = read(fd, msgBuf, BUF_SIZE);
        if(endofile == 0)
            break;
        printf("readProg get: %s\n", msgBuf);
        V(semWrite);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}