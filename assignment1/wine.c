#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h> 

regex_t regex;

struct data
{
    char name[20];
    char address[50];
    char days[7][15];
};

struct days
{
    int monday;
    int tuesday;
    int wendsday;
    int thursday;
    int friday;
    int saturday;
    int sunday;
};

void printAvailable(struct days a)
{
    printf("Elérhető napok:\n");
    printf("\tHétfő: %d\n\tKedd: %d\n\tSzerda: %d\n\tCsütörtök: %d\n\
        Péntek: %d\n\tSzombat: %d\n\tVasárnap: %d\n\n", a.monday, a.tuesday, a.wendsday, a.thursday, a.friday,\
    a.saturday, a.sunday);
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

void dataPrint_file(struct data p, FILE* fp)
{
    fprintf(fp, "{\n");
    fprintf(fp,"Név:   %s\n",p.name);
    fprintf(fp,"Cím:   %s\n",p.address);
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
        fprintf(fp, "Nap: %s\n", p.days[i]);
        ++i;
        reti = regexec(&regex, p.days[i], 0, NULL, 0);
    }
    fprintf(fp, "}\n");
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

void clear()
{
    while ((getchar()) != '\n');
    // fflush(stdin); 
}

char menu()
{
    char re;
    printf("Jelentkezési Alkalmazás Menü:\n \
    a - adatfelvétel\n \
    m - adatmódosítás\n \
    t - adattörlés\n \
    n - napi lista\n \
    h - heti lista\n \
    q - kilépés\n");
    printf("Add meg a kívánt menüpontot: ");

    scanf("%1[amtnhq]%c", &re);  
    clear();    

    return re;
}

int check(char* in, char* day, int d)
{
    return (strstr(in, day) && d == 0);
}

struct data add(struct days ava)
{
    char name[20];
    char address[50];
    char days[100];
    printf("\nAdatfelvétel:\n");

    printAvailable(ava);    

    printf("Név(Maximum 20 karakter): ");
    scanf("%20[^\n]%*c", name);
    printf("Megadott név: %s\n",name);
    clear();

    printf("\nCím(Maximum 50 karakter): ");
    scanf("%50[^\n]%*c", address);
    printf("Megadott cím: %s\n",address);
    clear();

    int right = 0;

    do
    {
        printf("\nNapok(Maximum 100 karakter): ");
        scanf("%50[^\n]%*c", days);
        printf("Megadott napok: %s\n",days);
        clear();
        right = 0;
        if (check(days, "hétfő", ava.monday) || check(days, "kedd", ava.tuesday) || check(days, "szerda", ava.wendsday)
        || check(days, "csütörtök", ava.thursday) || check(days, "péntek", ava.friday) || check(days, "szombat", ava.saturday) || check(days, "vasárnap", ava.sunday))
        {
            printAvailable(ava); 
            printf("Olyan napot adtál meg, amire már nincs hely! Adj meg egy jó napot!\n");
            right = 1;
            getchar();
        }  
    } while (right);

    char** splitDays = split(days, 7, 15);

    struct data person;
    strcpy(person.name, name);
    strcpy(person.address, address);
    for(int i = 0; i < 7; ++i)
    {
        printf("%s\n", splitDays[i]);
        strcpy(person.days[i], splitDays[i]); 
    }
    
    return person;
}

void change()
{

}

void delete()
{

}


struct days readDays(struct days ava)
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
        if (strstr(line, "hétfő"))
        {
            ava.monday -= 1;
        }
        else if (strstr(line, "kedd"))
        {
            ava.tuesday -= 1;
        }
        else if (strstr(line, "szerda"))
        {
            ava.wendsday -= 1;
        }
        else if (strstr(line, "csütörtök"))
        {
            ava.thursday -= 1;
        }
        else if (strstr(line, "péntek"))
        {
            ava.friday -= 1;
        }
        else if (strstr(line, "szombat"))
        {
            ava.saturday -= 1;
        }
        else if (strstr(line, "vasárnap"))
        {
            ava.sunday -= 1;
        }
    }

    fclose(reader);
    if (line)
        free(line);
        
    return ava;        
}


void openMenu(char c, FILE* fp, struct days ava)
{
    struct data person;
    switch (c)
    {
    case 'a':
        if (ava.monday + ava.tuesday + ava.wendsday + ava.thursday + ava.saturday + ava.sunday <= 0)
        {
            printf("Nincs elérhető nap amire lehetne jelentkezni!\n");
            return;
        }
        person = add(ava);
        dataPrint(person);
        dataPrint_file(person, fp);
        break;
    case 'm':
        /* code */
        break;
    case 't':
        /* code */
        break;
    case 'n':
        /* code */
        break;
    
    default:
        break;
    }
}

int main()
{
    struct days startAvailable = {
        1, 2, 3, 4, 5, 6, 7
    };
    struct days nowAvailable;
    nowAvailable = readDays(startAvailable);
    printAvailable(nowAvailable);
    FILE* writer;
    writer = fopen("data.txt", "a");
    if (writer == NULL)
    {
        printf("Fájl nem található!\n");
        exit(EXIT_FAILURE);
    }
    setlinebuf(writer);
    char c = ' ';
    do
    {
        nowAvailable = readDays(startAvailable);
        c = menu();
        if (c == '\0')
        {
            printf("\nHibás menüpont! Kérem válasszon a felsorolt menüpontok közül.\n");
        }
        else
        {
            printf("\nVálasztott menüpont: %c\n", c);  
            openMenu(c, writer, nowAvailable);          
        }
        
        getchar();
    } while (c != 'q');
    
    fclose(writer);

    return 0;
}
