#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
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