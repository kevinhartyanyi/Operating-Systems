#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argv, char* arg[])
{

    int f = open("hello.txt", O_RDONLY, 0600);
    if (f == -1)
    {
        printf("Failed to open file.\n");
    }

    char b;
    while ()
    {
        read (f, b, 1)
        printf("%c\n", b);
    }
    
    
    close (f);

    
}