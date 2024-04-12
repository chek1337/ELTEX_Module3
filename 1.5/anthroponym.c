#include "anthroponym.h"

// person1 < person2
int LastnameComparer(Anthroponym person1, Anthroponym person2)
{
    int n1_lastname = strlen(person1.lastName);
    int n2_lastname = strlen(person2.lastName);
    if (n1_lastname == n2_lastname)
    {
        for (int i = 0; i < n1_lastname; i++)
        {
            if (person1.lastName[i] < person2.lastName[i])
                return 1;
            else if (person1.lastName[i] > person2.lastName[i])
                return 0;
        }
        return 0;
    }
    else
    {
        if (n1_lastname < n2_lastname)
        {
            for (int i = 0; i < n1_lastname; i++)
            {
                if (person1.lastName[i] < person2.lastName[i])
                    return 1;
                else if (person1.lastName[i] > person2.lastName[i])
                    return 0;
            }
            return 1;
        }
        else
        {
            for (int i = 0; i < n2_lastname; i++)
            {
                if (person1.lastName[i] < person2.lastName[i])
                    return 1;
                else if (person1.lastName[i] > person2.lastName[i])
                    return 0;
            }
            return 0;
        }
    }
}
