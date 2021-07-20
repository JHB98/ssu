#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int num = atoi(argv[1]), *pid, i;
    pid = (int *)malloc(sizeof(int) * num);
    for (i = 0; i < num; i++)
    {
        if ((pid[i] = fork()) == 0)
        {
            while (1)
            {
                sleep(5);
                printf("%d\n", i);
            }
        }
    }
    sleep(4);
    while (1)
    {
        sleep(1);
        kill(pid[--num], SIGKILL);
        if (num == 0)
        {
            raise(SIGTERM);
        }
    }
    return 0;
}