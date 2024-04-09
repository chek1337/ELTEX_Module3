#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <errno.h>
#include <unistd.h>

void FunPrintExit(void)
{
    printf("Program succes!\n");
}

int main(int argc, char *argv[])
{
    pid_t pid;
    switch (pid  = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_SUCCESS);
    case 0:
        for(int i = 0; i < argc; i++)
            printf("Child process: %s\n", argv[i]);
            exit(EXIT_SUCCESS);
    
    default:
        wait(NULL);
        atexit(FunPrintExit);
        for(int i = 0; i < argc; i++)
            printf("Parent process: %s\n", argv[i]);
            exit(EXIT_SUCCESS);
    }
}