#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>
#include <limits.h>
#include <signal.h>

#define PATHNAME "/tmp"
#define SHMEM_MAXMSG 128
#define MSG_SIZE 5

typedef struct shared_memory {
    int buf [SHMEM_MAXMSG];
    int n_msg;
} shared_memory;

void myperr(char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void handler_SIGINT(int sig)
{
    printf("Parent process get SIGINT\n");
    exit(EXIT_SUCCESS);
}

void Parent(shared_memory *p_shmem)
{
    signal(SIGINT, handler_SIGINT);

    printf("Parent PID %d\n", getpid());
    srand((unsigned int)time(NULL));

    int randNum;
    // char strNum[MSG_SIZE];
    while(1)
    {
        for (int i = 0; i < 10; i++)
        {
            randNum = rand()%1000;
            p_shmem->buf[p_shmem->n_msg] = randNum;
            printf("Parent write: %d\n", p_shmem->buf[p_shmem->n_msg]);
            p_shmem->n_msg++;
        }

        sleep(2);
    }
}

void ChildMin(shared_memory *p_shmem)
{   
    printf("ChildMIN PID %d\n", getpid());
    int min = INT_MAX;

    while(1)
    {
        sleep(2);

        for (int i = 0; i < p_shmem->n_msg; i++)
        {
            int curValue = p_shmem->buf[i];

            if(curValue < min)
                min = curValue;
        }

        printf("-\tChildMIN = %d\n", min);
    }
}

void ChildSum(shared_memory *p_shmem)
{   
    printf("ChildSUM PID %d\n", getpid());

    while(1)
    {
        sleep(2);
        unsigned long sum = 0;
        for (int i = 0; i < p_shmem->n_msg; i++)
        {
            int curValue = p_shmem->buf[i];
            
            sum += (unsigned long)curValue;
        }

        printf("+\tChildSUM = %ld\n", sum);
    }
}

void ChildAvg(shared_memory *p_shmem)
{   
    printf("ChildAVG PID %d\n", getpid());

    while(1)
    {
        sleep(2);
        unsigned long sum = 0;
        for (int i = 0; i < p_shmem->n_msg; i++)
        {
            int curValue = p_shmem->buf[i];
            
            sum += (unsigned long)curValue;
        }

        printf("~\tChildAVG = %ld\n", sum / (unsigned long)p_shmem->n_msg);
    }
}

int main()
{
    int shm_id = shm_open(PATHNAME, O_CREAT | O_RDWR, 0666);
    if (shm_id == -1)
        myperr("shm_open");

    if(ftruncate (shm_id, sizeof(shared_memory)) == -1)
        myperr("ftruncate");

    shared_memory *p_shmem = mmap(NULL, sizeof(shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    if(p_shmem == NULL)
    {
        shm_unlink(PATHNAME);
        myperr("mmap");
    }

    p_shmem->n_msg = 0;
    
    pid_t pid1, pid2, pid3;
    switch (pid1 = fork())
    {
    case  -1:
        myperr("fork");
        break;
    
    case 0:
        ChildMin(p_shmem);
        break;

    default:
       
        switch (pid2 = fork())
        {
        case -1:
            myperr("fork");
            break;

        case 0:
            ChildSum(p_shmem);
            break;

        default:

            switch (pid3 = fork())
            {
            case -1:
                myperr("fork");
                break;
            
            case 0:
                ChildAvg(p_shmem);
                break;

            default:
                Parent(p_shmem);
                wait(NULL);
                wait(NULL);
                wait(NULL);
                break;
            }
            break;
        }
        break;
    }

    shm_unlink(PATHNAME);
}
