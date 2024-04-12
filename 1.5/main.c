#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include"persinfo.h"
#include "list.h"


PhoneList* ReadFromFile(PhoneList* head)
{
    char file[] = "database.txt";
    int fd = open(file, O_RDONLY);
    ssize_t eof = 10;
    PersonalInformation p;
    eof = read(fd, p.anthroponym.firstName, 20);
    for(; eof != 0; )
    { 
        eof = read(fd, p.anthroponym.lastName, 20);
        eof = read(fd, p.anthroponym.patronymic, 20);
        eof = read(fd, p.phoneNumber, 12);
        head = AddInList(head, p);
        eof = read(fd, p.anthroponym.firstName, 20);
    }
    
    close(fd);

    return head;
    
}

void WriteInFile(PhoneList* head)
{
    char file[] = "database.txt";
    int fd = open(file, O_WRONLY | O_CREAT);
    if(head == NULL)
        return;

    PhoneList* curElem = head;
    for(;curElem != NULL; curElem = curElem->next)
    {
        write(fd, curElem->person.anthroponym.firstName, 20);
        write(fd, curElem->person.anthroponym.lastName, 20);
        write(fd, curElem->person.anthroponym.patronymic, 20);
        write(fd, curElem->person.phoneNumber, 12);
    }
    
    close(fd);
}

void Interface()
{
    PhoneList* head = NULL;
    PhoneList* tmpHead = NULL;
    int selectedOption;
    PersonalInformation p;
    for (int statusExit = 0; statusExit == 0; )
    {
        printf("Select a menu option:\n");
        printf("\t0) Add a contact to the database.\n");
        printf("\t1) Read file to the database.\n");
        printf("\t2) Redact a contact from the databsase.\n");
        printf("\t3) Delete a contact from the databsase.\n");
        printf("\t4) Output database in file.\n");
        printf("\t5) Output database.\n");
        printf("\t6) Exit.\n");

        scanf("%d", &selectedOption);
        switch (selectedOption)
        {
        case 0:
            p = ReadingData();
            head = AddInList(head, p);
            printf("Contact is added!\n");
            break;
        case 1:
            head = ReadFromFile(head);
            break;
        case 2:
            p = ReadAnthroponym();
            tmpHead = RedactInList(head, p);
            if (tmpHead != NULL)
            {
                head = tmpHead;
                printf("Contact is redacted!\n");
            }
            else
                printf("Contact not found!\n");
               
            break;
        case 3:
            p = ReadAnthroponym();
            tmpHead = DeleteInList(head, p);
            if (tmpHead != NULL)
            {
                head = tmpHead;
                printf("Contact is deleted!\n");
            }
            else
                printf("Contact not found!\n");
            break;
        case 4:
            WriteInFile(head);
            break;
        case 5:
            PrintList(head);
            break;
        case 6:
            statusExit = 1;
            break;
        default:
            printf("Wrong action.\n");
        }
    }
}

void main()
{
    Interface();
}