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

struct days GetDays()
{
    struct days worker_num = {
        1, 2, 3, 4, 5, 6, 7
    };

    return worker_num;
}