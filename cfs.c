#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <sched.h>
#define LOW 250000
#define MID 500000
#define HIGH 750000
void matrix(int num)
{
    int a[5][5] = {{1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}};
    int b[5][5] = {{5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}};
    int c[5][5] = {0};
    for (int l = 0; l < num; l++)
    {
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                for (int k = 0; k < 5; k++)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }
}
void makeChild(int num, bool check, int nice)
{
    int pid;
    if ((pid = fork()) == 0)
    {
        if (check)
        {
            setpriority(PRIO_PROCESS, 0, nice);
        }
        printf("%d process begins / priority : %d\n", getpid(), getpriority(PRIO_PROCESS, 0));
        matrix(num);
        printf("%d process ends / priority : %d\n", getpid(), getpriority(PRIO_PROCESS, 0));
        exit(0);
    }
}
void normal()
{
    for (int i = 0; i < 21; i++)
    {
        makeChild(MID, false, 0);
    }
    while (wait(NULL) != -1)
        ;
}
void mod()
{
    for (int i = 0; i < 21; i++)
    {
        if (i < 7)
        {
            makeChild(LOW, true, 19);
        }
        else if (i < 14)
        {
            makeChild(MID, true, 9);
        }
        else
        {
            makeChild(HIGH, true, 0);
        }
    }
    while (wait(NULL) != -1)
        ;
}
int main()
{
    printf("NORMAL CFS\n----------------------\n");
    normal();
    printf("FINISH\n\n");
    printf("MODIFIED NICE CFS\n----------------------\n");
    mod();
    printf("FINISH\n");
    return 0;
}