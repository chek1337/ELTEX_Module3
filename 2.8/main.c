#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

volatile int canRead = 0;

void WaitToRead(int sig)
{
    canRead = 0;
}

void StartToRead(int sig)
{
    canRead = 1;
}

void Child()
{
    usleep(100);
    int fd = open("file.txt", O_RDONLY);
    char strNum[4];

    for (int i = 0; ; i++)
    {
        if(canRead == 1)
        {
            read(fd, strNum, sizeof(strNum));
            printf("Child get: %s\n", strNum);
            canRead = 0;
        }
    }
    
}

void Parent(pid_t childPid, int n)
{
    kill(childPid, SIGUSR1);
    kill(childPid, SIGUSR2);

    int fd = open("file.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    char strNum[4];
    int randNum;
    
    srand(time(NULL));

    for (int i = 0; i < n; i++)
    {
        usleep(100);
        kill(childPid, SIGUSR1);
        randNum = rand() % 1000;
        sprintf(strNum, "%d", randNum);
        write(fd, strNum, sizeof(strNum));
        kill(childPid, SIGUSR2);
        
    }
    
    usleep(100000);
    kill(childPid, SIGKILL);
}
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Wrong number of arg!\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGUSR1, WaitToRead);
    signal(SIGUSR2, StartToRead);

    int n = atoi(argv[1]);

    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;

    case 0: // Child proccess
        Child();
        exit(EXIT_SUCCESS);

    default: // Parent proccess
        Parent(pid, n);
        wait(NULL);
        printf("Program finished!\n");
        exit(EXIT_SUCCESS);
    }
}
