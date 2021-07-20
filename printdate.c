#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int timelimit;
    if (argc == 1)
    {
        while (1)
        {
            if (vfork() == 0)
            {
                sleep(1);
                execl("/bin/date", "date", NULL);
            }
        }
    }
    else if (argc == 2)
    {
        timelimit = atoi(argv[1]);
        while (timelimit-- > 0)
        {
            if (vfork() == 0)
            {
                execl("/bin/date", "date", NULL);
            }
            sleep(1);
            if (timelimit == 0)
            {
                raise(SIGTERM);
            }
            else if (timelimit <= 5)
            {
                printf("Ends in %d second\n", timelimit);
            }
        }
    }
    return 0;
}