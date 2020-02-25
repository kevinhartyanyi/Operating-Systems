#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h> 
#include <unistd.h>

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
    fprintf(fp, "+++++++++++++++++++++++++++++++++++++\n");
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

    scanf("%c", &re);  
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

char* prepConcat(char* addBegin, char* conCat)
{
    char tmp[100];
    strcpy(tmp, addBegin);
    char concat[100];
    strcpy(concat, strcat(tmp, conCat));
    return strcat(concat, "\n");
}

void change(struct days ava)
{
    printf("Kit szeretne módosítani (név)?\nNév:");
    char name[20];
    scanf("%20[^\n]%*c", name);

    printf("Mit szeretne módosítani?\nn - Név\nc - Cím\na - Nap\n");
    char ch;
    scanf("%c", &ch);
    clear();
    if (ch != 'n' && ch != 'c' && ch != 'a')
    {
        printf("Rossz betűt adtál meg!\n");
        getchar();
        return;
    }
    char chData[100];
    char cm;
    if (ch == 'n' || ch == 'c')
    {
        printf("Mire szeretnéd módosítani?\nÚj: ");
        scanf("%50[^\n]%*c", chData);
    }
    else
    {
        printf("Hozzáadni vagy eltávolítani szeretnél napot?\na - Adni\ne - Eltávolítani\n");        
        scanf("%c", &cm);
        clear();
        if (cm != 'a' && cm != 'e')
        {
            printf("Rossz betűt adtál meg!\n");
            getchar();
            return;
        }
        else
        {
            if (cm == 'a')
            {
                printf("Mit szeretnél hozzáadni?\nÚj: ");
                scanf("%50[^\n]%*c", chData);
                if (check(chData, "hétfő", ava.monday) || check(chData, "kedd", ava.tuesday) || check(chData, "szerda", ava.wendsday)
                || check(chData, "csütörtök", ava.thursday) || check(chData, "péntek", ava.friday) || check(chData, "szombat", ava.saturday) || check(chData, "vasárnap", ava.sunday))
                {
                    printAvailable(ava); 
                    printf("Olyan napot adtál meg, amire már nincs hely!\n");
                    return;
                }        
            }
            else
            {
                printf("Mit szeretnél eltávolítani?\nNap: ");
                scanf("%50[^\n]%*c", chData);
            }          
        }
        
    }
    
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
            if (ch == 'n')
            {
                char cFinal[100];
                strcpy(cFinal, prepConcat("Név: ", chData));
                fprintf(write, cFinal);
                end = 0;
                continue;
            }
            
        }

        if(end == 0)
        {
            fprintf(write, line);
        }
        else if(strstr(line, "+++"))
        {
            fprintf(write, line);
            end = 0;
        }      
        else if(end == 1)
        {
            if (ch == 'c' && strstr(line, "Cím:"))
            {
                char cFinal[100];
                strcpy(cFinal, prepConcat("Cím: ", chData));
                fprintf(write, cFinal);
                end = 0;
            }
            else if(ch == 'a' && cm == 'a' && strstr(line, "Nap:"))
            {
                char cFinal[100];
                strcpy(cFinal, prepConcat("Nap: ", chData));
                fprintf(write, cFinal);
                fprintf(write, line);
                end = 0;
            }
            else if (ch == 'a' && cm == 'e' && strstr(line, "Nap:") && strstr(line, chData))
            {
                end = 0;
            }
            else
            {
                fprintf(write, line);
            }      
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

void delete()
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

void dayList(char* dayL)
{
    printf("\nA %s-i napra jelentkezők:\n", dayL);

    FILE* reader;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    reader = fopen("data.txt", "r");
    if (reader == NULL)
        exit(EXIT_FAILURE);

    char name[100];
    int any = 0;
    while ((read = getline(&line, &len, reader)) != -1) 
    {
        if(strstr(line, "Név"))
        {
            strcpy(name, line);
        }
        else if(strstr(line, dayL))
        {
            printf("%s", name);
            any = 1;
        }
    }

    if (!any)
    {
        printf("Senki\n");
    }
    

    fclose(reader);
    if (line)
        free(line);
}

void weekList()
{
    char checkDays[7][20] = {"hétfő", "kedd", "szerda", "csütörtök", "péntek", "szombat", "vasárnap"};
    for (int i = 0; i < 7; i++)
    {       
        dayList(checkDays[i]);
    }
}


struct days readDays(struct days ava)
{
    FILE* reader;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    reader = fopen("data.txt", "r");
    if (reader == NULL)
    {
        printf("Fájl: data.txt nem található!\n");
        exit(EXIT_FAILURE);
    }

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
        change(ava);
        break;
    case 't':
        delete();
        break;
    case 'n':
        printf("Melyik nap-ról szeretnél listát?\n");
        char d[20];
        scanf("%20[^\n]%*c", d);        
        dayList(d);
        break;
    case 'h':
        weekList();
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
