#include <stdlib.h>
#include <stdio.h>

void value(char** str1, char* str2)
{
    *str1 = str2;
}

int main()
{
    char* c3 = malloc(9 * sizeof(char));
    value(&c3, "123456789");

    printf("%s\n",c3);
    c3 = realloc(c3, 9 * sizeof(char));
    printf("%s\n",c3);
    if(c3)
    {
        printf("%s\n",c3);
    }
    else
    {
        printf("Not enough memory");
    }
}