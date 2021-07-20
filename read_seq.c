#include <stdio.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char** argv)
{
    FILE* fp;
    char buff[250];
    struct timeval start, end, result;
    int cnt = 0;
    fp = fopen(argv[1], "r");

    fread(buff, 4, 1, fp);
    gettimeofday(&start, NULL);
    while (1)
    {
        memset(buff, 0, 250);
        fread(buff, 250, 1, fp);
        if (feof(fp))
        {
            break;
        }
        cnt++;
    }
    gettimeofday(&end, NULL);

    result.tv_sec = end.tv_sec - start.tv_sec;
    result.tv_usec = end.tv_usec - start.tv_usec;

    while (result.tv_sec > 0)
    {
        result.tv_sec--;
        result.tv_usec += 1000000;
    }
    printf("#records: %d elapsed_time: %ld us\n", cnt, result.tv_usec);

    return 0;
}