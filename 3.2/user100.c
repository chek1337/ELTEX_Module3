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


#define MSG_SIZE 64
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
    msgbuf *msg_buf_rcv = malloc(sizeof(struct msgbuf));

    char text_buf[MSG_SIZE];
    int i;
    printf("User \"100\"\n");
    for ( i = 0; ; i++)
    {
        printf(">> ");
        fgets(text_buf, MSG_SIZE, stdin);
        if(strcmp(text_buf, "/exit\n") == 0)
            break;
        strcpy(msg_buf_snd->mtext, text_buf);
        msg_buf_snd->mtype = 100;
        msgsnd(msq_id, msg_buf_snd, MSG_SIZE, IPC_NOWAIT);

        if(msgrcv(msq_id, msg_buf_rcv, MSG_SIZE, 200, IPC_NOWAIT) != 0)
            puts(msg_buf_rcv->mtext);
    }

    // int msgctl(int msqid, int cmd, struct msqid_ds *buf);
    msgctl(msq_id, IPC_RMID, 0);
    return 0;
}
