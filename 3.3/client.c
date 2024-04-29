#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <limits.h>

#define SERVER_KEY_PATHNAME "/tmp"
#define CLEINTS_KEY_PATHNAME "/lib"
#define PROJECT_ID 0

#define MSG_SIZE 64

struct message {
    long message_type;
    char message_text[MSG_SIZE];
};

int main ()
{
    char buf[MSG_SIZE];
    key_t server_key, client_msg_queue_key;
    int server_qid, cleints_qid, id;
    struct message send_message, rcv_message;

    server_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID);
    if (server_key == -1) {
        perror ("ftok");
        exit (1);
    }

    if ((server_qid = msgget (server_key, 0)) == -1) {
        perror ("msgget: server_qid");
        exit (1);
    }

    if ((client_msg_queue_key = ftok (CLEINTS_KEY_PATHNAME, PROJECT_ID)) == -1) {
        perror ("ftok");
        exit (1);
    }

    if ((cleints_qid = msgget (client_msg_queue_key, IPC_CREAT | 0666)) == -1) {
        perror ("msgget");
        exit (1);
    }

    printf ("Please write USER ID: ");

    scanf("%d", &id);
    send_message.message_type = id;

    strcpy(send_message.message_text, "USERCONNECTED");
    if (msgsnd (server_qid, &send_message, MSG_SIZE, 0) == -1) 
    {
        perror ("client: msgsnd");
        exit (1);
    }

    sprintf(buf, "%d", id);
    strcpy(send_message.message_text, buf);
    if (msgsnd (server_qid, &send_message, MSG_SIZE, 0) == -1) 
    {
        perror ("client: msgsnd");
        exit (1);
    }

    while (1) 
    {
        printf(">> ");
        scanf("%s", buf);

        if(strcmp(buf, "/exit") == 0)
        {
            strcpy(send_message.message_text, "USERDISCONNECTED");
            if (msgsnd (server_qid, &send_message, MSG_SIZE, 0) == -1) 
            {
                perror ("client: msgsnd");
                exit (1);
            }
            break;
        }
            

        sprintf(send_message.message_text, "User - %d: %s\n", id, buf);

        // send message to server
        if (msgsnd (server_qid, &send_message, MSG_SIZE, 0) == -1) {
            perror ("client: msgsnd");
            exit (1);
        }

        // read response from server
        if (msgrcv (cleints_qid, &rcv_message, MSG_SIZE, LONG_MAX, 0) == -1) {
            perror ("client: msgrcv");
            exit (1);
        }

        // process return message from server
        printf("%s\n", rcv_message.message_text);
    }

    printf ("Client disconected\n");

    exit (0);
}