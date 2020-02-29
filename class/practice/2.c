#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int same(const char *str1, const char *str2)
{
    printf("%p\n%p\n", &(*str1), &(*str2));
    return &(*str1) == &(*str2);
}

int main()
{
    char *str = malloc(10);
    char *str2 = malloc(10);
    strcpy(str, "alma\0");
    strcpy(str2, "alma\0");
    printf("%d\n", same(str, "alma\0"));

    char *s1 = "Hajrá ";
    char *s2;
    char *s3;

    s2 = "Alma!";
    s3 = s1;
    int len = strlen(s1) + strlen(s2) + strlen(s3);
    char* ss = malloc(len);
    strcpy(ss, s1);
    ss = strcat(ss, s2);
    printf("s1: %s\ns2: %s\ns3: %s\n",s1, s2,s3);
    ss = strcat(ss, s3);
    s1 = ss;
    printf("s1: %s\ns2: %s\ns3: %s\n",s1, s2,s3);
}