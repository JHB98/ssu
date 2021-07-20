#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
int main()
{
    int N, *num, cnt = 0, index = 1, size;
    struct timeval start, end, result;
    scanf("%d", &N);
    gettimeofday(&start, NULL);
    switch (N)
    {
    case 100:
        size = 550;
        break;
    case 500:
        size = 3580;
        break;
    case 1000:
        size = 7920;
        break;
    case 5000:
        size = 48620;
        break;
    case 10000:
        size = 104750;
        break;
    }
    num = (int *)malloc(sizeof(int) * size);
    for (int i = 1; i <= size; i++)
    {
        num[i - 1] = i;
    }
    for (int i = 2; i <= sqrt(size); i++)
    {
        if (num[i - 1] == 0)
        {
            continue;
        }
        for (int j = i * 2; j <= size; j += i)
        {
            num[j - 1] = 0;
        }
    }
    while (cnt != N)
    {
        if (num[index] == 0)
        {
            index++;
            continue;
        }
        printf("%d ", num[index++]);
        cnt++;
        if (!(cnt % 10))
        {
            printf("\n");
        }
    }
    gettimeofday(&end, NULL);
    result.tv_sec = end.tv_sec - start.tv_sec;
    result.tv_usec = end.tv_usec - start.tv_usec;

    while (result.tv_sec > 0)
    {
        result.tv_sec--;
        result.tv_usec += 1000000;
    }
    printf("time : %ld usec\n", result.tv_usec);
    free(num);
    return 0;
}