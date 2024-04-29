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
    key_t msg_queue_key, client_msg_queue_key;
    int server_qid, cleints_qid;
    struct message message;
    int num_user = 0;
    int users_id[64];

    if ((msg_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1) {
        perror ("ftok");
        exit (1);
    }

    if ((server_qid = msgget (msg_queue_key, IPC_CREAT | 0666)) == -1) {
        perror ("msgget");
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

    printf ("Server Started!\n");

    while (1) {
        // read an incoming message
        if (msgrcv (server_qid, &message, MSG_SIZE, 0, 0) == -1) 
        {
            perror ("msgrcv");
            exit (1);
        }

        printf ("Server get: %s\n", message.message_text);

        if(strcmp(message.message_text, "USERCONNECTED") == 0)
        {
            if (msgrcv (server_qid, &message, MSG_SIZE, 0, 0) == -1) 
            {
                perror ("msgrcv");
                exit (1);
            }

            users_id[num_user] = atoi(message.message_text);
            num_user++;
        }
            
        else if(strcmp(message.message_text, "USERDISCONNECTED") == 0)
            num_user--;
        else
        {
            message.message_type = LONG_MAX;

            for (int i = 0; i < num_user; i++)
            {
                if (msgsnd(cleints_qid, &message, MSG_SIZE, 0) == -1) 
                {  
                    perror ("msgget");
                    exit (1);
                }
            }
        }
    }

    if (msgctl (server_qid, IPC_RMID, NULL) == -1)
    {
            perror ("client: msgctl");
            exit (1);
    }

    if (msgctl (cleints_qid, IPC_RMID, NULL) == -1)
    {
            perror ("client: msgctl");
            exit (1);
    }

}