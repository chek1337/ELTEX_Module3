#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <bits/sigaction.h>

void SIGINT_SIGQUIT_handler(int sig)
{
    if(sig == SIGINT)
        printf("Program receive SIGINT\n");
    else if(sig == SIGQUIT)
        printf("Program receive SIGQUIT\n");  
}
int main()
{
    struct sigaction sig;
    sig.sa_handler = &SIGINT_SIGQUIT_handler;
    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGQUIT, &sig, NULL);

    printf("\nProgram launched with PID: %d\n", getpid());

    int fd = open("file.txt", O_WRONLY | O_APPEND | O_CREAT, 0660);

    char buf[8];
    for (int i = 1; ; i++)
    {
        sprintf(buf, "%d\n", i);
        write(fd, buf, sizeof(buf));
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}