#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hasznos/utils.c"

int main(int argc, char** argv)
{
    if(argc == 3)
    {
        char* conc = concat(argv[1], argv[2]);
        printf("%s\n", conc);
    }
    printf("%s\n", argv[0]);
    int len = 0;
    while (*argv)
    {
        char* out = *argv++;
        printf("%s\n", out);
        len += strlen(out);
    }
    *argv--;
    int c = 0;
    while (c != argc)
    {
        printf("%s\n", *argv--);
        c++;
    }
    printf("Length: %i\n", len);
    if(!strcmp("almaa", "alma"))
        printf("Same\n");
    
    
}