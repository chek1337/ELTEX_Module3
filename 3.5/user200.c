#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <string.h>

#define PATHNAME1 "/user100"
#define PATHNAME2 "/user200"

#define MSG_SIZE 64

void myperr(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
int main()
{   
    struct mq_attr queue_attr;
    queue_attr.mq_flags = O_NONBLOCK;
    queue_attr.mq_maxmsg = 1000;
    queue_attr.mq_msgsize = MSG_SIZE;
    queue_attr.mq_curmsgs  = 0;

    mqd_t mq_id_user100 = mq_open(PATHNAME1, O_CREAT | O_RDWR, 0660, &queue_attr);
    if(mq_id_user100 == -1)
        myperr("mq_open1");

    mqd_t mq_id_user200 = mq_open(PATHNAME2, O_CREAT | O_RDWR, 0660,  &queue_attr);
    if(mq_id_user200 == -1)
        myperr("mq_open2");

    char snd_buf[MSG_SIZE];
    char rcv_buf[MSG_SIZE];
    printf("User \"200\"\n");
    for ( int i = 0; ; i++)
    {
        printf(">> ");
        fgets(snd_buf, MSG_SIZE, stdin);
        if(strcmp(snd_buf, "/exit") == 0)
            break;
        if(mq_send(mq_id_user100, snd_buf, MSG_SIZE, 200) == -1)
            myperr("mq_send");
        if(mq_receive(mq_id_user200, rcv_buf, MSG_SIZE, NULL) == -1)
            myperr("mq_receive");

        puts(rcv_buf);
    }

    mq_unlink(PATHNAME1);
    mq_unlink(PATHNAME2);
}