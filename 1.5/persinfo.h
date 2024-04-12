#ifndef PERSONAL_INFORMATION_H_
#define PERSONAL_INFORMATION_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "anthroponym.h"

typedef struct PersonalInformation {
    Anthroponym anthroponym;
    char phoneNumber[12];
} PersonalInformation;

PersonalInformation ReadAnthroponym();

PersonalInformation ReadingData();
#endif // !PERSONAL_INFORMATION_H_

