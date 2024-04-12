#ifndef ANTHROPONYM_H_
#define ANTHROPONYM_H_

typedef struct	Anthroponym {
    char firstName[20];
    char lastName[20];
    char patronymic[20];
} Anthroponym;

int LastnameComparer(Anthroponym person1, Anthroponym person2);

#endif // !ANTHROPONYM_H_
