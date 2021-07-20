#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SUFFLE_NUM	10000
void GenRecordSequence(int* list, int n);
void swap(int* a, int* b);
int main(int argc, char** argv)
{
    FILE* fp;
    char buff[250];
    struct timeval start, end, result;
    int* read_order_list;
    int num_of_records = 0;
    fp = fopen(argv[1], "r");
    fread(buff, 4, 1, fp);
    while (1)
    {
        memset(buff, 0, 250);
        fread(buff, 250, 1, fp);
        if (feof(fp))
        {
            break;
        }
        num_of_records++;
    }
    read_order_list = (int*)malloc(sizeof(int) * num_of_records);
    GenRecordSequence(read_order_list, num_of_records);
    fseek(fp, 4, SEEK_SET);
    gettimeofday(&start, NULL);
    for (int i = 0; i < num_of_records; i++)
    {
        if (i == 0)
        {
            fseek(fp, 250 * (read_order_list[i]), SEEK_CUR);
            fread(buff, 250, 1, fp);
        }
        else
        {
            fseek(fp, 250 * (read_order_list[i] - read_order_list[i - 1] - 1), SEEK_CUR);
            fread(buff, 250, 1, fp);
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

    printf("#records: %d elapsed_time: %ld us\n", num_of_records, result.tv_usec);
    return 0;
}

void GenRecordSequence(int* list, int n)
{
    int i, j, k;

    srand((unsigned int)time(0));

    for (i = 0; i < n; i++)
    {
        list[i] = i;
    }

    for (i = 0; i < SUFFLE_NUM; i++)
    {
        j = rand() % n;
        k = rand() % n;
        swap(&list[j], &list[k]);
    }
}

void swap(int* a, int* b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}