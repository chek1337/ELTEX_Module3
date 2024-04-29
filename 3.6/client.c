#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <limits.h>

#define SERVER_KEY_PATHNAME "/tmp"
#define CLEINTS_KEY_PATHNAME "/home"
#define PROJECT_ID 0

#define MSG_SIZE 32

struct message {
    long message_type;
    char message_text[MSG_SIZE];
};

int main ()
{
    char buf[MSG_SIZE];
    char send_message[MSG_SIZE], rcv_message[MSG_SIZE];
    int id;
    struct mq_attr queue_attr;
    queue_attr.mq_flags = 0;
    queue_attr.mq_maxmsg = MSG_SIZE;
    queue_attr.mq_msgsize = MSG_SIZE;
    queue_attr.mq_curmsgs = 0;

    // mq_unlink(SERVER_KEY_PATHNAME);
    mqd_t server_qid = mq_open(SERVER_KEY_PATHNAME, O_RDWR, 0666);
    if(server_qid == -1)
    {
        perror("mq_open1");
        exit(1);
    }

    // mq_unlink(CLEINTS_KEY_PATHNAME);
    mqd_t cleints_qid = mq_open(CLEINTS_KEY_PATHNAME, O_RDWR, 0666);
    if(cleints_qid == -1)
    {
        perror("mq_open2");
        exit(1);
    }

    printf ("Please write USER ID: ");

    scanf("%d", &id);

    strcpy(send_message, "USERCONNECTED");
    if (mq_send(server_qid, send_message, MSG_SIZE, 0) == -1) 
    {
        perror ("client: mq_send1");
        exit (1);
    }

    sprintf(buf, "%d", id);
    strcpy(send_message, buf);
    if (mq_send(server_qid, send_message, MSG_SIZE, 0) == -1) 
    {
        perror ("client: mq_send2");
        exit (1);
    }

    while (1) 
    {
        printf(">> ");
        scanf("%s", buf);

        if(strcmp(buf, "/exit") == 0)
        {
            strcpy(send_message, "USERDISCONNECTED");
            if (mq_send(server_qid, send_message, MSG_SIZE, 0) == -1) 
            {
                perror ("client: mq_send3");
                exit (1);
            }
            break;
        }
            

        sprintf(send_message, "User - %d: %s\n", id, buf);

        // send message to server
        if (mq_send(server_qid, send_message, MSG_SIZE, 0) == -1) {
            perror ("client: mq_send4");
            exit (1);
        }

        // read response from server
        if (mq_receive(server_qid, rcv_message, MSG_SIZE, NULL) == -1) 
        {
            perror ("client: mq_receive");
            exit (1);
        }

        // process return message from server
        printf("%s\n", rcv_message);
    }

    printf ("Client disconected\n");

    exit (0);
}