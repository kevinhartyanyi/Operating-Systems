#include <stdio.h>
#include <stdlib.h>

int length(char* str)
{
    int s = 0;
    int i = 0;
    for(char c = ' '; c != '\0'; i++)
    {
        s++;
        c = str[i];
    }
    return s - 1;
}

int same(char* str1, char*str2)
{
    int i = 0;
    char a = str1[i];
    char b = str2[i];
    while (a != '\0' && b != '\0')
    {
        i++;
        if (a != b)
        {
            return 0;
        }
        a = str1[i];
        b = str2[i];
    }
    return 1;    
}

char* cut(char* str, int c)
{
    char* re = malloc(length(str)-c * sizeof(char));
    char ch = str[c];
    int ind = 0;
    re[ind] = ch;
    for(int i = c+1; ch != '\0'; i++)
    {
        ind++;
        ch = str[i];
        re[ind] = ch;
    }
    return re;
}

void change(char* str,char** str2, char c1, char c2)
{
    char ch = str[0];
    for(int i = 0; ch != '\0'; i++)
    {
        printf("%c\n", ch);
        ch = str[i];
        (*str2)[i] = ch;
        if (ch == c1)
        {
            printf("%c %c\n", str[i], c2);
            (*str2)[i] = c2;
        }
        
    }
}

void value(char** str1, char* str2)
{
    *str1 = str2;
}

int main()
{
    int h = length("alma\0");
    printf("%d\n", h);
    if (same("alma\0","alma\0"))
    {
        printf("Same\n");
    }
    printf("%s\n",cut("alma\0",2));
    char* c;
    change("alma\0",&c,'a','b');
    printf("%s\n",c);
    char* c2;
    value(&c2,"city\0");
    printf("%s\n",c2);
    char* c3 = malloc(9 * sizeof(char));
    value(&c3, "123456789");

    printf("%s\n",c3);
    c3 = realloc(c3, 80);
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