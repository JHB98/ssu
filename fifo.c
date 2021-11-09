#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <sched.h>
#define MID 500000
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
void makeChild(int num)
{
    int pid;
    if ((pid = fork()) == 0)
    {
        printf("%d process begins\n", getpid());
        matrix(num);
        printf("%d process ends\n", getpid());
        exit(0);
    }
}
void normal()
{
    for (int i = 0; i < 21; i++)
    {
        makeChild(MID);
    }
    while (wait(NULL) != -1)
        ;
}
int main()
{
    printf("FIFO\n----------------------\n");
    normal();
    printf("FINISH\n\n");
    return 0;
}