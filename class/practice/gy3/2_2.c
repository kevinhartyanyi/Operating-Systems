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

    int f = open("hello.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (f == -1)
    {
        printf("Failed to open file.\n");
        return 1;
    }
    fcntl(f, F_SETLKW, &l);

    int len = strlen(getlogin());
    char name[10];
    printf("%d\n", len);
    strcpy(name, getlogin());
    write(f, name, sizeof(name));

    char name2[10];
    getlogin_r(name2, sizeof(name2));
    write(f, name2, sizeof(name2));

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
    for (int i = 0; i < bytes; i++)
    {
        printf("%c %d\n", buffer[i], buffer[i]);
        if((int)buffer[i] == 0)
        {
            printf("End\n");
        }
    }
    
      
}