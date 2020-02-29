#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

struct tet
{
    int a;
    int b;
};

int main(int argv, char* arg[])
{
    if(argv < 2)
    {
        printf("Add an argument!\n");
        return(1);
    }
    printf("%s\n", arg[1]);
    struct stat statRes;
    if(stat(arg[1], &statRes) < 0){
        printf("Hiba\n");
        return 1;
    }
    int c;
    //while ((c = getchar()) != '\n' && c != EOF) { }
    char ch = getchar();
    printf("Adott: %c\n", ch);

    struct flock l;
    l.l_type = F_WRLCK;
    l.l_whence = SEEK_SET;
    l.l_start = 0;
    l.l_len = 0;
    l.l_pid = getpid();

    int f = open("hello.txt", O_RDWR | O_CREAT, 0600);
    if (f == -1)
    {
        printf("Failed to open file.\n");
        return 1;
    }
    fcntl(f, F_SETLKW, &l);
    lseek(f,-1, SEEK_END);
    write(f, &ch, 1);

    close(f);
    l.l_type = F_UNLCK;
    fcntl(f, F_SETLK, &l);


    f = open("hello.txt", O_RDONLY, 0600);
    if (f == -1)
    {
        printf("Failed to open file.\n");
    }

    char buffer[512];
    int bytes =  read (f, buffer, sizeof(buffer));
    close (f);

    printf("\nSuccessfully read %d bytes.\n", bytes);
    printf("%s\n", buffer);
    int a = buffer[0];
    int b = buffer[1];
    printf("Összeg: %d", a+b);
    for (int i = 0; i < bytes; i++)
    {
        printf("%c %d\n", buffer[i], buffer[i]);
        if((int)buffer[i] == 0)
        {
            printf("End\n");
        }
    }
    
}