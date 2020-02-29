#ifndef UTILS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h> 

char* concat(char* str, char* str2)
{
    int len = strlen(str) + strlen(str2);
    char* re = (char*)malloc(len);
    strcpy(re, str);
    strcat(re, str2);
    return re;
}

void dataPrint(struct data p)
{
    printf("Név:   %s\n",p.name);
    printf("Cím:   %s\n",p.address);
    int i = 0;
    int reti = regcomp(&regex, "[a-zA-Z]", 0);
    if (reti) 
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    reti = regexec(&regex, p.days[i], 0, NULL, 0);
    while (!reti)
    {
        printf("Nap: %s\n", p.days[i]);
        ++i;
        reti = regexec(&regex, p.days[i], 0, NULL, 0);
    }
}

char** split(char* str, int row, int length)
{
    char** re = (char**)malloc(sizeof(char*) * row);
    for(int i = 0; i < row; ++i)
    {
        re[i] = (char*)malloc(sizeof(char) * length);
        re[i] = "";
    }
    char* split;
    split = strtok (str," ");
    int c = 0;
    while (split != NULL)
    {
        //printf ("%s\n",split);
        re[c] = split;
        split = strtok (NULL, " ");
        ++c;
    }
    return re;
}

void readFile()
{
    FILE* reader;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    reader = fopen("data.txt", "r");
    if (reader == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, reader)) != -1) 
    {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    fclose(reader);
    if (line)
        free(line);
}


void delete() //Create new file trick
{
    printf("Kit szeretne törölni (név)?\nNév:");
    char name[20];
    scanf("%20[^\n]%*c", name);

    FILE* reader;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    reader = fopen("data.txt", "r");
    if (reader == NULL)
        exit(EXIT_FAILURE);

    FILE* write;
    write = fopen("data2.txt", "w");
    if (write == NULL)
        exit(EXIT_FAILURE);

    int found = 0;
    int end = 0;
    while ((read = getline(&line, &len, reader)) != -1) 
    {
        if(strstr(line, name) && strstr(line, "Név:"))
        {
            found = 1;
            end = 1;
        }

        if(end == 0)
        {
            fprintf(write, line);
        }
        else if(strstr(line, "+++"))
        {
            end = 0;
        }        
    }

    if (found == 0)
    {
        printf("Nincs ilyen nevű jelentkező!\n");
        unlink("data2.txt");
    }
    else
    {
        unlink("data.txt");
        rename("data2.txt", "data.txt");
    }
    
    

    fclose(reader);
    fclose(write);
    if (line)
        free(line);
}


// scanf("%20[^\n]%*c", str);

#endif // !UTILS