#ifndef PHONE_LIST_H_
#define PHONE_LIST_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "persinfo.h"

typedef struct PhoneList
{
    PersonalInformation person;
    struct PhoneList* prev;
    struct PhoneList* next;
} PhoneList;

PhoneList* AddInList(PhoneList* head, PersonalInformation newPerson);

PhoneList* FindInList(PhoneList* head, PersonalInformation findPerson);

PhoneList* DeleteInList(PhoneList* head, PersonalInformation deletePerson);

PhoneList* RedactInList(PhoneList* head, PersonalInformation newPerson);

void PrintList(PhoneList* head);

#endif // PHONE_LIST_H_

