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
    
    int fd[2];
    if(pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    char strNum[3];
    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;

    case 0: // Child proccess write
        close(fd[0]);// Close reading;
        srand(time(NULL));
        int randNum;
        for (int i = 0; i < n; i++)
        {
            randNum = rand()%1000;
            sprintf(strNum, "%d", randNum);
            write(fd[1], strNum, sizeof(strNum));
        }
        
        close(fd[1]);
        printf("Child finished!\n");
        exit(EXIT_SUCCESS);

    default: // Parent proccess read
        usleep(10);
        close(fd[1]); // Close writing
        wait(NULL);

        for (int i = 0; i < n; i++)
        {
            read(fd[0], strNum, sizeof(strNum));
            printf("%s\n", strNum);
        }
        
        close(fd[0]);
        printf("Parent finished!\n");
        exit(EXIT_SUCCESS);
    }
}
