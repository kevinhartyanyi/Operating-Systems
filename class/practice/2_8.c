#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    srand(time(0));
    for(int i = 0; i < 6; ++i)
    {
        int a = rand() % 7;
        int b = rand() % 7;
        printf("a: %d\nb: %d\nsum: %d\n\n", a, b, a+b);
    }

    char* s = malloc(20);
    char* s2 = malloc(20);
    char* s3 = malloc(20);
    int a;
    float b;
    scanf("%i%f", &a, &b);
    printf("%i %f", a, b);
    scanf("%20[^\n]%*c", s);
    scanf("%20[^\n]%*c", s2);
    scanf("%20[^\n]%*c", s3);
    printf("%s\n", s);
    printf("%s\n", s2);
    printf("%s\n", s3);
    return 0;
}