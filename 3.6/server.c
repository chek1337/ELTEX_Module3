#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <limits.h>

#define SERVER_KEY_PATHNAME "/tmp"
#define CLEINTS_KEY_PATHNAME "/home"

#define MSG_SIZE 32

int main ()
{
    char message[MSG_SIZE];
    int num_user = 0;
    int users_id[64];

    struct mq_attr queue_attr;
    queue_attr.mq_flags = 0;
    queue_attr.mq_maxmsg = MSG_SIZE;
    queue_attr.mq_msgsize = MSG_SIZE;
    queue_attr.mq_curmsgs = 0;

    // mq_unlink(SERVER_KEY_PATHNAME);
    mqd_t server_qid = mq_open(SERVER_KEY_PATHNAME,  O_CREAT | O_RDWR, 0666, &queue_attr);
    if(server_qid == -1)
    {
        perror("mq_open1");
        exit(1);
    }

    // mq_unlink(CLEINTS_KEY_PATHNAME);
    mqd_t cleints_qid = mq_open(CLEINTS_KEY_PATHNAME, O_CREAT | O_RDWR, 0666, &queue_attr);
    if(cleints_qid == -1)
    {
        perror("mq_open2");
        exit(1);
    }

    printf ("Server Started!\n");

    while (1) {
        if (mq_receive(server_qid, message, MSG_SIZE, NULL) == -1) 
        {
            perror ("mq_receive1");
            exit (1);
        }

        printf ("Server get: %s\n", message);

        if(strcmp(message, "USERCONNECTED") == 0)
        {
            if (mq_receive(server_qid, message, MSG_SIZE, NULL) == -1) 
            {
                perror ("mq_receive2");
                exit (1);
            }

            users_id[num_user] = atoi(message);
            num_user++;
        }
            
        else if(strcmp(message, "USERDISCONNECTED") == 0)
            num_user--;
        else
        {
            for (int i = 0; i < num_user; i++)
            {
                if (mq_send(cleints_qid, message, MSG_SIZE, 0) == -1) 
                {  
                    perror ("mq_send");
                    exit (1);
                }
            }
        }
    }

    mq_close(server_qid);
    mq_close(cleints_qid);
}