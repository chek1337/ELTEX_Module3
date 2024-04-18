#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MSG_SIZE 8
#define KEY_PATHNAME "msg"

typedef struct msgbuf
{
    long mtype;
    char mtext[MSG_SIZE];
} msgbuf;

int main()
{

    key_t key = ftok(KEY_PATHNAME, 0);
    if(key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    int msq_id = msgget(key, IPC_CREAT | IPC_EXCL | 0600);
    if(msq_id == -1)
    {
        if(errno == EEXIST)
        {    msq_id = msgget(key, 0);
            if(msq_id == -1)
            {
                perror("msgget");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            perror("msgget");
            exit(EXIT_FAILURE);
        }
    }

    msgbuf *msg_buf_snd = malloc(sizeof(struct msgbuf));

    srand(time(NULL));
    int randNum;
    char strNum[MSG_SIZE];
    int i;
    for ( i = 0; randNum != 255; i++)
    {
        randNum = rand()%255 + 1;
        sprintf(strNum, "%d", randNum);
        strcpy(msg_buf_snd->mtext, strNum);
        msg_buf_snd->mtype = randNum;
        msgsnd(msq_id, msg_buf_snd, MSG_SIZE, IPC_NOWAIT);
        sleep(1);
    }

    // msgbuf *msg_buf_rcv = malloc(sizeof(struct msgbuf));
    // for (int j = 0; msg_buf_rcv->mtype != 255; j++)
    // {
    //     msgrcv(msq_id, msg_buf_rcv, MSG_SIZE, 0, IPC_NOWAIT);
    //     printf("%s\n", msg_buf_rcv->mtext);
    // }
}
