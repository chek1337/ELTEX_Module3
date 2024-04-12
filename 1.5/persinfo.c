#include "persinfo.h"

PersonalInformation ReadAnthroponym()
{
    PersonalInformation person;
    char buf[20];

    printf("Enter the last name.\n");
    scanf("%s", &buf);
    strncpy(person.anthroponym.lastName, buf, 20);

    printf("Enter the first name.\n");
    scanf("%s", &buf);
    strncpy(person.anthroponym.firstName, buf, 20);

    printf("Enter the patronymic name.\n");
    scanf("%s", &buf);
    strncpy(person.anthroponym.patronymic, buf, 20);
    return person;
}


PersonalInformation ReadingData()
{
    PersonalInformation p;
    char buf[256];
    int scanfRes;

    printf("Enter the last name.\n");
    scanf("%s", &buf);
    strncpy(p.anthroponym.lastName, buf, 20);

    printf("Enter the first name.\n");
    scanf("%s", &buf);
    strncpy(p.anthroponym.firstName, buf, 20);

    printf("Enter the patronymic name.\n");
    scanf("%s", &buf);
    strncpy(p.anthroponym.patronymic, buf, 20);

    printf("Enter the contact's phone number.\n");
    scanf("%s", &buf);
    strncpy(p.phoneNumber, buf, 11);

    return p;
}