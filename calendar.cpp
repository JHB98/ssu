#pragma warning(disable : 4996)
#include <stdio.h>
#include <time.h>
int main()
{
    int array[42] = {0}, *p, year, daysum, day_of_the_week, date = 1, enter;
    scanf("%d", &year);
    daysum = ((year - 1) * 365) + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400);
    day_of_the_week = (daysum + 1) % 7;
    p = array + day_of_the_week;
    enter = day_of_the_week;
    for (int i = 0; i < 42; i++)
    {
        *p = date++;
        if (date > 31)
        {
            break;
        }
        p++;
    }
    p = array;
    printf("SUN MON TUE WED THU FRI SAT\n");
    for (int i = 0; i < 42; i++)
    {
        if (*p == 0)
        {
            p++;
            printf("%3c ", ' ');
            continue;
        }
        printf("%3d ", *p);
        enter++;
        if (enter == 7)
        {
            printf("\n");
            enter = 0;
        }
        p++;
    }
    printf("\n");
    return 0;
}