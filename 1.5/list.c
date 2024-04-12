#include "list.h"

PhoneList* AddInList(PhoneList* head, PersonalInformation newPerson)
{
    PhoneList* pTmp = (PhoneList*)malloc(sizeof(PhoneList));

    pTmp->next = NULL;
    pTmp->prev = NULL;

    pTmp->person.anthroponym.firstName;
    strncpy(pTmp->person.anthroponym.firstName, newPerson.anthroponym.firstName, 20);
    strncpy(pTmp->person.anthroponym.lastName, newPerson.anthroponym.lastName, 20);
    strncpy(pTmp->person.anthroponym.patronymic, newPerson.anthroponym.patronymic, 20);
    strncpy(pTmp->person.phoneNumber, newPerson.phoneNumber, 12);
    
    if (head == NULL)
    {
        head = pTmp;
        return head;
    }

    PhoneList* curElem = head;
    PhoneList* prevElem = head->prev;

    for (; curElem != NULL; )
    {
        if (LastnameComparer(pTmp->person.anthroponym, curElem->person.anthroponym))
        {
            if (head == curElem)
            {
                pTmp->next = curElem;
                curElem->prev = pTmp;
                head = pTmp;
                return head;
            }
            else
            {
                pTmp->next = curElem;
                pTmp->prev = prevElem;
                curElem->prev = pTmp;
                prevElem->next = pTmp;
                return head;
            }
        }
        prevElem = curElem;
        curElem = curElem->next;
    }

    prevElem->next = pTmp;
    pTmp->prev = prevElem;
    return head;
}

PhoneList* FindInList(PhoneList* head, PersonalInformation findPerson)
{
    PhoneList* curContact = head;
    for (; curContact != NULL; )
    {
        if (strcmp(findPerson.anthroponym.lastName, curContact->person.anthroponym.lastName) == 0)
            if (strcmp(findPerson.anthroponym.firstName, curContact->person.anthroponym.firstName) == 0)
                if (strcmp(findPerson.anthroponym.patronymic, curContact->person.anthroponym.patronymic) == 0)
                    return curContact;
        curContact = curContact->next;
    }
    return NULL;
}

PhoneList* DeleteInList(PhoneList* head, PersonalInformation deletePerson)
{
    PhoneList* contactToDelete = FindInList(head, deletePerson);

    if (contactToDelete != NULL)
    {
        if (head == contactToDelete)
        {
            head = head->next;
            head->prev = NULL;
            free(contactToDelete);
            return head;
        }
        else
        {
            if (contactToDelete->next != NULL)
            {
                contactToDelete->prev->next = contactToDelete->next;
                contactToDelete->next->prev = contactToDelete->prev;
                free(contactToDelete);
                return head;
            }
            else
            {
                contactToDelete->prev->next = NULL;
                free(contactToDelete->prev->next);
                return head;
            }
        }
    }
    return NULL;
}

PhoneList* RedactInList(PhoneList* head, PersonalInformation redPerson)
{
    PhoneList* contactToRedact = FindInList(head, redPerson);
    if (contactToRedact != NULL)
    {
        PersonalInformation person;
        char buf[20];
        printf("Enter new first name.\n");
        scanf("%s", &buf);
        strncpy(contactToRedact->person.anthroponym.firstName, buf, 20);

        printf("Enter new last name.\n");
        scanf("%s", &buf);
        strncpy(contactToRedact->person.anthroponym.lastName, buf, 20);

        printf("Enter new patronymic name.\n");
        scanf("%s", &buf);
        strncpy(contactToRedact->person.anthroponym.patronymic, buf, 20);

        printf("Enter new phone number.\n");
        scanf("%s", &buf);
        strncpy(contactToRedact->person.phoneNumber, buf, 12);
        return head;
    }
    return NULL;
}

void PrintList(PhoneList* head)
{
    PhoneList* curElem = head;
    for (int i = 1; curElem != NULL; i++)
    {
        printf("#%d Name: %s %s %s ", i, curElem->person.anthroponym.lastName, curElem->person.anthroponym.firstName, curElem->person.anthroponym.patronymic);
        printf("Phone Number: %s\n", curElem->person.phoneNumber);

        curElem = curElem->next;
    }
    printf("\n");
}

