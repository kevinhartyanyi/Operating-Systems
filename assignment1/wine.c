#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


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

void getData(char* data, int size)
{
    char tmp[100];
    fgets(tmp, size, stdin);
    memcpy(data, &tmp[0], strlen(tmp)-1);
	data[strlen(tmp)-1]='\0';
}

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

void dataPrint_file(struct data p)
{
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int f = open("data.txt", O_RDWR | O_APPEND, 0600);
    if (f == -1)
    {
        printf("Fájl megnyítási hiba.\n");
        exit(1);
    }
    fcntl(f, F_SETLKW, &lock);
    char* lineStart = "{\n";
    char* lineEnd = "}\n";
    write(f, lineStart, strlen(lineStart));
    char* name = "Név: ";
    write(f, name, strlen(name));
    write(f, p.name, strlen(p.name));
    write(f, "\n", 1);
    char* address = "Cím: ";
    write(f, address, strlen(address));
    write(f, p.address, strlen(p.address));
    write(f, "\n", 1);
    

    int i = 0;
    while (strlen(p.days[i]) != 0 && i < 7)
    {
        char* day = "Nap: ";
        write(f, day, strlen(day));
        write(f, p.days[i], strlen(p.days[i]));
        write(f, "\n", 1);
        i++;
    }
    write(f, lineEnd, strlen(lineEnd));

    close(f);
    lock.l_type = F_UNLCK;
    fcntl(f,F_SETLKW,&lock); //unlock file
    /*
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
    fprintf(fp, "+++++++++++++++++++++++++++++++++++++\n");*/
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

    scanf(" %c", &re);  
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
    //scanf("%20[^\n]%*c", name);
    getData(name, 20);
    printf("Megadott név: %s\n",name);
    clear();

    printf("\nCím(Maximum 50 karakter): ");
    //scanf("%50[^\n]%*c", address);
    getData(address, 50);
    printf("Megadott cím: %s\n",address);
    clear();

    int right = 0;

    do
    {
        printf("\nNapok(Maximum 100 karakter): ");
        //scanf("%50[^\n]%*c", days);
        getData(days, 50);
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
    printf("Kit szeretne módosítani (név, 20)?\nNév:");
    char name[20];
    //scanf("%20[^\n]%*c", name);
    getData(name, 20);

    printf("Mit szeretne módosítani?\nn - Név\nc - Cím\na - Nap\n");
    char ch;
    scanf(" %c", &ch);
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
        //scanf("%50[^\n]%*c", chData);
        getData(chData, 50);
    }
    else
    {
        printf("Hozzáadni vagy eltávolítani szeretnél napot?\na - Adni\ne - Eltávolítani\n");        
        scanf(" %c", &cm);
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
                //scanf("%50[^\n]%*c", chData);
                getData(chData, 50);
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
                //scanf("%50[^\n]%*c", chData);
                getData(chData,50);
            }          
        }
        
    }


    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int f = open("data.txt", O_RDONLY);
    if (f == -1)
    {
        printf("Hiba a fájl megnyítása közben\n");
        exit(EXIT_FAILURE);
    }
    int fNew = open("data2.txt", O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fNew == -1)
    {
        printf("Hiba a fájl megnyítása közben\n");
        exit(EXIT_FAILURE);
    }

    fcntl(f, F_SETLKW, &lock);
    fcntl(fNew, F_SETLKW, &lock);

    struct stat st;
    stat("data.txt", &st);
    long size = st.st_size;
    char* file = malloc(size);    

    read(f, file, size);

    int found = 0;
    int end = 0;
    int pos = 0;
    char prev[100];
    while (pos < size - 1)
    {
        int l = 0;
        char line[100];
        while (file[pos] != '\n')
        {
            line[l] = file[pos];            
            ++pos; 
            ++l;     
        } 
        line[l] = '\0';
        ++pos;
        
        if(strstr(line, name) && strstr(line, "Név:"))
        {
            found = 1;
            end = 1;
            if (ch == 'n')
            {
                char cFinal[100];
                strcpy(cFinal, prepConcat("Név: ", chData));
                write(fNew, cFinal, strlen(cFinal));
                end = 0;
                continue;
            }
            
        }

        if(end == 0)
        {
            write(fNew, line, strlen(line));
            write(fNew, "\n", 1);
        }
        else if(strstr(line, "}"))
        {
            write(fNew, line, strlen(line));
            write(fNew, "\n", 1);
            end = 0;
        }      
        else if(end == 1)
        {
            if (ch == 'c' && strstr(line, "Cím:"))
            {
                char cFinal[100];
                strcpy(cFinal, prepConcat("Cím: ", chData));
                write(fNew, cFinal, strlen(cFinal));
                end = 0;
            }
            else if(ch == 'a' && cm == 'a' && strstr(line, "Nap:"))
            {
                char cFinal[100];
                strcpy(cFinal, prepConcat("Nap: ", chData));
                write(fNew, cFinal, strlen(cFinal));
                write(fNew, line, strlen(line));
                write(fNew, "\n", 1);
                end = 0;
            }
            else if (ch == 'a' && cm == 'e' && strstr(line, "Nap:") && strstr(line, chData))
            {
                end = 0;
            }
            else
            {
                write(fNew, line, strlen(line));
                write(fNew, "\n", 1);
            }      
        }

    }


    close(f);
    close(fNew);

    if (found == 0)
    {
        printf("Nincs ilyen nevű jelentkező!\n");
        unlink("data2.txt");
    }
    else
    {
        unlink("data.txt");
        rename("data2.txt", "data.txt");
        printf("Sikeres változtatás!\n");
    }

    lock.l_type = F_UNLCK;
    fcntl(f, F_SETLKW, &lock);
    fcntl(fNew, F_SETLKW, &lock);
    free(file);
    //clear();
}

void delete()
{
    printf("Kit szeretne törölni (név, 20)?\nNév:");
    char name[20];
    //scanf("%20[^\n]%*c", name);
    getData(name, 20);

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int f = open("data.txt", O_RDONLY);
    if (f == -1)
    {
        printf("Hiba a fájl megnyítása közben\n");
        exit(EXIT_FAILURE);
    }
    int fNew = open("data2.txt", O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fNew == -1)
    {
        printf("Hiba a fájl megnyítása közben\n");
        exit(EXIT_FAILURE);
    }

    fcntl(f, F_SETLKW, &lock);
    fcntl(fNew, F_SETLKW, &lock);

    struct stat st;
    stat("data.txt", &st);
    long size = st.st_size;
    char* file = malloc(size);    

    read(f, file, size);

    int found = 0;
    int end = 0;
    int pos = 0;
    char prev[100];
    while (pos < size - 1)
    {
        int l = 0;
        char line[100];
        while (file[pos] != '\n')
        {
            line[l] = file[pos];            
            ++pos; 
            ++l;     
        } 
        line[l] = '\0';
        ++pos;

        if(strstr(line, name) && strstr(line, "Név:"))
        {
            found = 1;
            end = 1;
        }
        else if (strstr(line, "Név:"))
        {
            write(fNew, "{\n", 2);
        }
        

        if(end == 0 && !strstr(line, "{"))
        {
            write(fNew, line, strlen(line));
            write(fNew, "\n", 1);
        }
        else if(strstr(line, "}"))
        {
            end = 0;
        }   
    }
    //write(fNew, "}\n", 2);


    close(f);
    close(fNew);

    if (found == 0)
    {
        printf("Nincs ilyen nevű jelentkező!\n");
        unlink("data2.txt");
    }
    else
    {
        unlink("data.txt");
        rename("data2.txt", "data.txt");
        printf("Sikeres törlés!\n");
    }

    lock.l_type = F_UNLCK;
    fcntl(f, F_SETLKW, &lock);
    fcntl(fNew, F_SETLKW, &lock);
    free(file);

}

void dayList(char* dayL)
{
    printf("\nA %s-i napra jelentkezők:\n", dayL);

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int f = open("data.txt", O_RDONLY);
    if (f == -1)
    {
        printf("Hiba a fájl megnyítása közben\n");
        exit(EXIT_FAILURE);
    }
    fcntl(f, F_SETLKW, &lock);

    struct stat st;
    stat("data.txt", &st);
    long size = st.st_size;
    char* file = malloc(size);    

    read(f, file, size);

    close(f);
    lock.l_type = F_UNLCK;
    fcntl(f, F_SETLKW, &lock);

    char name[100];
    int any = 0;
    int pos = 0;
    while (pos < size - 10)
    {
        int l = 0;
        char line[100];
        while (file[pos] != '\n')
        {
            line[l] = file[pos];            
            ++pos; 
            ++l;     
        } 
        line[l] = '\0';
        ++pos;
        if(strstr(line, "Név"))
        {
            strcpy(name, line);
        }
        else if(strstr(line, dayL))
        {
            printf("%s\n", name);
            any = 1;
        }
    }


    if (!any)
    {
        printf("Senki\n");
    }
    free(file);
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
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int f = open("data.txt", O_RDONLY);
    if (f == -1)
    {
        printf("Hiba a fájl megnyítása közben\n");
        exit(EXIT_FAILURE);
    }
    fcntl(f, F_SETLKW, &lock);

    struct stat st;
    stat("data.txt", &st);
    long size = st.st_size;
    char* file = malloc(size);
    char entry[200];
    

    read(f, file, size);

    close(f);
    lock.l_type = F_UNLCK;
    fcntl(f, F_SETLKW, &lock);

    int pos = 0;
    while (pos < size - 10)
    {
        memset(entry, 0, 200);
        while (file[pos] != '{')
        {
            ++pos;
        }
        pos += 2; // { \n

        int i = 0;
        while (file[pos] != '}')
        {
            entry[i] = file[pos];
            ++i;
            ++pos;
        }
        int j = 0;
        int l = 0;
        char line[100];
        while (entry[j] != '\0')
        {
            if (entry[j] == '\n')
            {   
                //printf("%s\n", &line[1]);
                if (strstr(&line[1], "hétfő"))
                {
                    ava.monday -= 1;
                }
                else if (strstr(&line[1], "kedd"))
                {
                    ava.tuesday -= 1;
                }
                else if (strstr(&line[1], "szerda"))
                {
                    ava.wendsday -= 1;
                }
                else if (strstr(&line[1], "csütörtök"))
                {
                    ava.thursday -= 1;
                }
                else if (strstr(&line[1], "péntek"))
                {
                    ava.friday -= 1;
                }
                else if (strstr(&line[1], "szombat"))
                {
                    ava.saturday -= 1;
                }
                else if (strstr(&line[1], "vasárnap"))
                {
                    ava.sunday -= 1;
                }
                memset(line, 0, 100);
                l = 0;
            }
            else
            {
                line[l] = entry[j];
            } 
            
            ++j; 
            ++l;     
        } 
    }
    
    free(file);

    return ava;        
}


void openMenu(char c, struct days ava)
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
        dataPrint_file(person);
        break;
    case 'm':
        change(ava);
        break;
    case 't':
        delete();
        break;
    case 'n':
        printf("Melyik nap-ról szeretnél listát?(20)\n");
        char d[20];
        //scanf("%20[^\n]%*c", d);  
        getData(d, 20);  
        //clear();    
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
    int writer = open("data.txt", O_WRONLY | O_CREAT, 0600);
    if (writer == -1)
    {
        printf("Fájl megnyítási hiba1!\n");
        exit(EXIT_FAILURE);
    }

    nowAvailable = readDays(startAvailable);
    printAvailable(nowAvailable);
    char c = ' ';
    do
    {
        nowAvailable = readDays(startAvailable);
        c = menu();
        if (c != 'a' && c != 'm' && c != 't' && c != 'n' && c != 'h' && c != 'q')
        {
            printf("\nHibás menüpont! Kérem válasszon a felsorolt menüpontok közül.\n");
        }
        else
        {
            printf("\nVálasztott menüpont: %c\n", c);  
            openMenu(c, nowAvailable);  
            clear();        
        }
        
        getchar();
    } while (c != 'q');
    

    return 0;
}
