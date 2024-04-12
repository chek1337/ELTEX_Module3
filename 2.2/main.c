#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Wrong number of arg!\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    
    int fd1[2], fd2[2];
    if(pipe(fd1) == -1 || pipe(fd2) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    char strNum[8];
    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;

    case 0: // Child proccess
        close(fd1[0]);// Close reading
        close(fd2[1]);// Close writing

        srand(time(NULL));
        int randNum;

        for (int i = 0; i < n; i++)
        {
            randNum = rand() % 1000;
            sprintf(strNum, "%d", randNum);
            write(fd1[1], strNum, sizeof(strNum));
        }

        usleep(10);

        for (int i = 0; i < n; i++)
        {
            read(fd2[0], strNum, sizeof(strNum));
            printf("Child get: %s\n", strNum);
        }

        close(fd1[1]);
        close(fd2[0]);
        printf("Child finished!\n");
        exit(EXIT_SUCCESS);

    default: // Parent proccess
        int numx2 = 0;

        close(fd1[1]); // Close writing
        close(fd2[0]); // Close reading

        usleep(10);

        for (int i = 0; i < n; i++)
        {
            read(fd1[0], strNum, sizeof(strNum));
            printf("Parent get: %s\n", strNum);
            numx2 = atoi(strNum);
            numx2 *= 2;
            sprintf(strNum, "%d", numx2);
            write(fd2[1], strNum, sizeof(strNum));
        }

        close(fd1[0]);
        close(fd2[1]);

        wait(NULL);
        printf("Parent finished!\n");
        exit(EXIT_SUCCESS);
    }
}