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


struct data add()
{
    char name[20];
    char address[50];
    char days[100];
    printf("\nAdatfelvétel:\nNév(Maximum 20 karakter): ");
    scanf("%20[^\n]%*c", name);
    printf("Megadott név: %s\n",name);
    clear();

    printf("\nCím(Maximum 50 karakter): ");
    scanf("%50[^\n]%*c", address);
    printf("Megadott cím: %s\n",address);
    clear();

    printf("\nNapok(Maximum 100 karakter): ");
    scanf("%50[^\n]%*c", days);
    printf("Megadott napok: %s\n",days);
    clear();

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

void openMenu(char c, FILE* fp)
{
    struct data person;
    switch (c)
    {
    case 'a':
        person = add();
        dataPrint(person);
        dataPrint_file(person, fp);
        fprintf(fp, "korte");
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
    FILE* fp;
    fp = fopen("data.txt", "r+");
    char c = ' ';
    do
    {
        c = menu();
        if (c == '\0')
        {
            printf("\nHibás menüpont! Kérem válasszon a felsorolt menüpontok közül.\n");
        }
        else
        {
            printf("\nVálasztott menüpont: %c\n", c);  
            openMenu(c, fp);          
        }
        
        getchar();
    } while (c != 'q');
    
    printf("%c\n", c);
    fclose(fp);

    return 0;
}
