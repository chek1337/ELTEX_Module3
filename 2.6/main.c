#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <bits/sigaction.h>

volatile int SIGINT_count = 0;

void SIGINTCountIncrease(int sig)
{
    SIGINT_count++;
    printf("You need to send sigint %d times.\n", 3-SIGINT_count);
}
int main()
{
    struct sigaction sig;
    sig.sa_handler = &SIGINTCountIncrease;
    sigaction(SIGINT, &sig, NULL);

    printf("\nProgram launched with PID: %d\n", getpid());

    int fd = open("file.txt", O_WRONLY | O_APPEND | O_CREAT, 0660);

    char buf[8];
    for (int i = 1; SIGINT_count != 3; i++)
    {
        sprintf(buf, "%d\n", i);
        write(fd, buf, sizeof(buf));
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}