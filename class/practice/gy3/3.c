#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

struct tet
{
    int a;
    int b;
};

int main()
{
    struct flock l;
    l.l_type = F_WRLCK;
    l.l_whence = SEEK_SET;
    l.l_start = 0;
    l.l_len = 0;
    l.l_pid = getpid();

    int f = open("hello.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (f == -1)
    {
        printf("Failed to open file.\n");
        return 1;
    }
    fcntl(f, F_SETLKW, &l);

    struct tet t;

    scanf("%d", &t.a);
    scanf("%d", &t.b);


    write(f,&t.a,1);
    write(f,&t.b,1);

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