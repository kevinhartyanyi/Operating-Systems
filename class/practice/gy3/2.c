#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{
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
    char a[] = "Hello\n";
    int i = 16;
    write(f, a, sizeof(a));
    write(f, &i, 1);
    write(f, a, sizeof(a));
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
    for (int i = 0; i < 18; i++)
    {
        printf("%c %d\n", buffer[i], buffer[i]);

    }
    
      
}