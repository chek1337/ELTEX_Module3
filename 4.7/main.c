#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
// #include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>
#include <limits.h>
#include <signal.h>

#define PATHNAME "/tmp"
#define SHMEM_MAXMSG 128
#define MSG_SIZE 5

volatile pid_t pid;

typedef struct shared_memory {
    int buf [SHMEM_MAXMSG];
    int n_msg;
    int pos_minmax;
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
    int k = 0;
    while(1)
    {
        for (int i = 0; i < 10; i++)
        {
            randNum = rand()%1000;
            p_shmem->buf[p_shmem->n_msg] = randNum;
            printf("Parent write: %d\n", p_shmem->buf[p_shmem->n_msg]);
            p_shmem->n_msg++;
        }

        sleep(1);

        printf("Parent MIN = %d\n", p_shmem->buf[p_shmem->n_msg - 2]);
        printf("Parent MAX = %d\n\n", p_shmem->buf[p_shmem->n_msg - 1]);

        k++;
    }
}

void Child(shared_memory *p_shmem)
{   
    printf("Child PID %d\n", getpid());
    int min = INT_MAX, max = INT_MIN;

    int k = 0;
    while(1)
    {
        sleep(1);

        for (int i = p_shmem->pos_minmax; i < p_shmem->n_msg; i++)
        {
            int curValue = p_shmem->buf[i];

            if(curValue > max)
                max = curValue;
            if(curValue < min)
                min = curValue;
        }

        printf("Child MIN = %d\n", min);
        printf("Child MAX = %d\n\n", max);


        p_shmem->pos_minmax = p_shmem->n_msg;

        p_shmem->buf[p_shmem->n_msg] = min;
        p_shmem->n_msg++;

        p_shmem->buf[p_shmem->n_msg] = max;
        p_shmem->n_msg++;

        k++;
    }
}

int main()
{

    key_t key_shm = ftok(PATHNAME, 0);
    if(key_shm == -1)
        myperr("ftok");
    
    int shm_id = shmget(key_shm, sizeof(struct shared_memory), IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        shmctl(shm_id, IPC_RMID, NULL);
        myperr("shmem");
    }
       
    shared_memory *p_shmem = (shared_memory *)shmat(shm_id, NULL,  0);
    if(p_shmem == NULL)
    {
        shmctl(shm_id, IPC_RMID, NULL);
        myperr("shmat");
    }
    p_shmem->n_msg = 0;
    p_shmem->pos_minmax = 0;
    
    switch (pid = fork())
    {
    case  -1:
        myperr("fork");
        break;
    
    case 0:
        Child(p_shmem);
        break;

    default:
        Parent(p_shmem);
        wait(NULL);
        shmctl(shm_id, IPC_RMID, NULL);
        break;
    }
    shmdt(p_shmem);
}
