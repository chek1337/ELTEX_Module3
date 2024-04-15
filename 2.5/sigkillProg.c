#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Wrong number of args!\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = atoi(argv[1]);

    kill(pid, SIGKILL);
    exit(EXIT_SUCCESS);
}