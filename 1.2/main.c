#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    double num;

    switch (pid  = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        for (int i = 1; i < argc; i += 2)
        {
            num = atof(argv[i]);
            printf("Child process: %.3lf\n", num*num);
        }
        exit(EXIT_SUCCESS);
        
    default:
        wait(NULL);
        for (int i = 2; i < argc; i += 2)
        {
            num = atof(argv[i]);
            printf("Child process: %.3lf\n", num*num);
        }
        exit(EXIT_SUCCESS);
    }
}
