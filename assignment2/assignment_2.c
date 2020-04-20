#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <wait.h>
#include <regex.h> 
#include "common.h"

#define name_length 50
const char* data = "data.txt";
const char* pipe_child1 = "/tmp/tavasz_c0s0rj_child1";
const char* pipe_child2 = "/tmp/tavasz_c0s0rj_child2";

pid_t p = -1;
pid_t p2 = -1;

enum Week
{
    monday,
    tuesday,
    wendsday,
    thursday,
    friday,
    saturday,
    sunday
};

enum Dune
{
    Jeno_telek,
    Lovas_dulo,
    Hosszu,
    Selyem_telek,
    Malom_telek,
    Szula
};

enum Work_Types
{
    Metszes,
    Permetezes,
    Nyitas,
    Horolas
};

struct Job
{
    enum Dune today_dune;
    enum Work_Types today_work;
};

struct Job_List
{
    struct Job today_job;
    char workers[100][name_length];
    int worker_number;
};

void transform_Week(char* str, enum Week today)
{
    char re[50];

    switch (today)
    {
    case monday:
        strcpy(re, "hétfő");
        break;
    case tuesday:
        strcpy(re, "kedd");
        break;
    case wendsday:
        strcpy(re, "szerda");
        break;
    case thursday:
        strcpy(re, "csütörtök");
        break;
    case friday:
        strcpy(re, "péntek");
        break;
    case saturday:
        strcpy(re, "szombat");
        break;
    case sunday:
        strcpy(re, "vasárnap");
        break;    
    default:
        break;
    }
    strcpy(str, re);
}

int readNamesForDay(char names[][name_length], enum Week today)
{
    char dayStr[50];
    transform_Week(dayStr, today);

    char tmp[100][name_length];

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
    int pos = 0;
    int tmp_count = 0;
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
        else if(strstr(line, dayStr))
        {
            strcpy(tmp[tmp_count], name);

            tmp_count++;
        }
    }
    
    free(file);

    for (int i = 0; i < tmp_count; i++)
    {
        strcpy(names[i], tmp[i]);
    }
    

    return tmp_count;
}

int data_is_modified(time_t old_time) 
{
    struct stat file_stat;
    int err = stat(data, &file_stat);
    if (err != 0) {
        perror(" [file_is_modified] stat");
        exit(EXIT_FAILURE);
    }
    return file_stat.st_mtime > old_time;
}

void cout(char* str)
{
    printf("%s\n", str);
}

void transform_Dune(char* data, enum Dune dune)
{
    switch (dune)
    {
        case Jeno_telek:
            strcpy(data, "Jenő telek");
            break;
        case Lovas_dulo:
            strcpy(data, "Lovas dűlő");
            break;
        case Hosszu:
            strcpy(data, "Hosszú");
            break;
        case Selyem_telek:
            strcpy(data, "Selyem telek");
            break;
        case Malom_telek:
            strcpy(data, "Majom telek");
            break;   
        case Szula:
            strcpy(data, "Szula");
            break;   
        default:
            break;
    }
}

void transform_Work(char* data, enum Work_Types work)
{
    switch (work)
    {
        case Metszes:
            strcpy(data, "Metszés");
            break;
        case Permetezes:
            strcpy(data, "Rügyfakasztó permetezés");
            break;
        case Nyitas:
            strcpy(data, "Tavaszi nyitás");
            break;
        case Horolas:
            strcpy(data, "Horolás");
            break;
        default:
            break;
    }
}

int Workers_Required(enum Week today)
{
    struct days day_num = GetDays();
    int re = -1;

    switch (today)
    {
    case monday:
        re = day_num.monday;
        break;
    case tuesday:
        re = day_num.tuesday;
        break;
    case wendsday:
        re = day_num.wendsday;
        break;
    case thursday:
        re = day_num.thursday;
        break;
    case friday:
        re = day_num.friday;
        break;
    case saturday:
        re = day_num.saturday;
        break;
    case sunday:
        re = day_num.sunday;
        break;    
    default:
        break;
    }
    return re;
}

void stop_run()
{
    if (0 == p)
    {
        cout("Stopping CHILD1");
    }
    else if (0 == p2)
    {
        cout("Stopping CHILD2");
    }
    else
    {
        cout("Stopping PARENT");
        unlink(pipe_child1);
        unlink(pipe_child2);
    }
    exit(EXIT_SUCCESS);    
}

int main()
{
    

    if( access( pipe_child1, F_OK ) != -1 ) 
    {
        cout("Pipe_child1 file already exists => unlink it");
        unlink(pipe_child1);
    }

    if( access( pipe_child2, F_OK ) != -1 ) 
    {
        cout("Pipe_child2 file already exists => unlink it");
        unlink(pipe_child2);
    }

    int f = mkfifo(pipe_child1, S_IRUSR | S_IWUSR);
    cout("Created Pipe_child1 in PARENT");        
    if (f < 0) { perror("error"); exit(EXIT_FAILURE); }

    int f2 = mkfifo(pipe_child2, S_IRUSR | S_IWUSR);
    cout("Created Pipe_child2 in PARENT");        
    if (f2 < 0) { perror("error"); exit(EXIT_FAILURE); }

    sigset_t signal_block;
    sigemptyset(&signal_block);
    sigaddset(&signal_block, SIGUSR1);
    sigprocmask(SIG_SETMASK, &signal_block, NULL);

    signal(SIGINT, stop_run);

    enum Week today = monday;

    while (1)
    {
        char day_name[30];

        transform_Week(day_name, today);
        printf("\nToday is %s\n\n", day_name);
        p = fork();
        if (p == -1) 
        {
            perror("Fork hiba");
            exit(EXIT_FAILURE);
        }
        
        if(p == 0)
        { // Gyerek
            srand(time(0));
            int fid = open(pipe_child1, O_WRONLY);
            if (fid < 0){ perror("Error at opening the file in CHILD\n"); exit(EXIT_FAILURE);}
            printf("Opened Pipe_child1 in CHILD\n");

            cout("CHILD finding job for today...");
            sleep(3);
            
            struct Job today_job;
            today_job.today_dune = (enum Dune) (rand() % 6);
            today_job.today_work = (enum Work_Types) (rand() % 4);

            write(fid, &today_job, sizeof(today_job));
            cout("Found job. Sending it to PARENT");
            close(fid);
            exit(EXIT_SUCCESS);
        }
        else
        { // Szülő

            int fid = open(pipe_child1, O_RDWR);
            printf("Opened Pipe_child1 in PARENT\n");
            if (fid < 0){ perror("Error at opening the file in PARENT\n"); exit(1);}

            
            struct Job today_job;

            cout("Waiting for job in PARENT");
            read(fid, &today_job, sizeof(today_job));
            cout("Received job from CHILD");

            p2 = fork();

            if (p2 == 0)
            { // Gyerek 2
                int fid2 = open(pipe_child2, O_RDONLY);
                if (fid2 < 0){ perror("Error at opening the file in CHILD2\n"); exit(EXIT_FAILURE);}
                printf("Opened Pipe_child2 in CHILD2\n");

                struct Job_List job_list;

                cout("Waiting for job list in CHILD2");
                read(fid2, &job_list, sizeof(job_list));
                cout("Received job list from PARENT");

                printf("Workers for today:\n");
                
                for (int i = 0; i < job_list.worker_number; i++)
                {
                    printf("%s\n", job_list.workers[i]);
                }
                char today_dune[20];
                char today_work[20];

                transform_Dune(today_dune, job_list.today_job.today_dune);
                transform_Work(today_work, job_list.today_job.today_work);
                
                printf("Today's job is %s at %s\n", today_work, today_dune);

                int workers_required = Workers_Required(today);

                printf("Number of workers we have: %d, number of workers we need: %d\n", job_list.worker_number, workers_required);

                if (job_list.worker_number < workers_required)
                {
                    cout("Don't have enough workers\nWaiting for more");
                    int current_worker_count = job_list.worker_number;
                    char names[100][name_length];
                    // time_t last_modified = time(0);
                    while (current_worker_count < workers_required)
                    {
                        sleep(3);
                        int new_worker_count = readNamesForDay(names, today);
                        if (new_worker_count != current_worker_count)
                        {
                            current_worker_count = new_worker_count;
                            cout("Workers changed");
                            printf("New worker count: %d\n", current_worker_count);
                            for (int i = 0; i < current_worker_count; i++)
                            {
                                cout(names[i]);
                            }                            
                        }
                    }
                    
                }

                cout("We have enough workers");  
                
                union sigval sv; 
                sv.sival_int = workers_required;
                cout("CHILD2 sending SIGUSR1 signal to PARENT");
                sigqueue(getppid(), SIGUSR1, sv); 
                close(fid2);
                exit(EXIT_SUCCESS);
            }
            else
            { // Szülö

                char names[100][name_length];
                int worker_number = readNamesForDay(names, today);       

                struct Job_List job_list;
                job_list.today_job = today_job;

                for (int i = 0; i < worker_number; i++)
                {
                    strcpy(job_list.workers[i], names[i]);                 
                }
                job_list.worker_number = worker_number;

                int fid2 = open(pipe_child2, O_RDWR);
                printf("Opened Pipe_child2 in PARENT\n");
                if (fid2 < 0){ perror("Error at opening the file in PARENT\n"); exit(1);}

                write(fid2, &job_list, sizeof(job_list));
                cout("Sending job list from PARENT");

                cout("PARENT waiting for signal from CHILD2");
                sigset_t t;
                sigemptyset(&t);
                sigaddset(&t, SIGUSR1);
                siginfo_t info;

                sigwaitinfo(&t, &info);
                
                printf("PARENT received workers number: %d from SIGUSR1 signal\n", info.si_value);

                waitpid(p2, NULL, 0);
                printf("PARENT Finished\n");
                close(fid);
                close(fid2);
            }
        }    
        today++;
        if (today > sunday)
        {
            today = monday;
        }
    }
    


}