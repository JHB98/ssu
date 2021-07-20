#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
char matrix[10000][10000];
char nextMatrix[10000][10000];
int line, row, processNum, threadNum, genNum;
void inputMatrix();
void sequential();
void parallelProcess();
void parallelThread();
void inputNum(int input);
void *check(void *lineRange);
void print(int gen);
void calculate(int *workNum, int num);
int main()
{
    int input;
    struct timeval start, end;
    long long result;
    while (1)
    {
        printf("1)Program End 2)Sequential 3)Parallel Process 4)Parallel Thread\n");
        printf("Input number : ");
        scanf("%d", &input);
        inputNum(input);
        gettimeofday(&start, NULL);
        inputMatrix();

        switch (input)
        {
        case 1:
            return 0;
        case 2:
            sequential();
            break;
        case 3:
            parallelProcess();
            break;
        case 4:
            parallelThread();
            break;
        }
        gettimeofday(&end, NULL);
        result = ((end.tv_sec - start.tv_sec) * 1000) + ((end.tv_usec - start.tv_usec) / 1000);
        printf("\nTime : %lld ms\n", result);
        printf("======================\n");
    }
}
void sequential()
{
    int lineRange[2] = {0};
    lineRange[0] = 0;
    lineRange[1] = line;
    for (int j = 1; j <= genNum; j++)
    {
        check(&lineRange);
        print(j);
        if (j != genNum)
        {
            memcpy(matrix, nextMatrix, sizeof(nextMatrix));
        }
    }
}
void parallelProcess()
{
    pid_t pid[processNum];
    int workNum[processNum], start, ref_line, ref_row, fd, spaceNum, lineRange[2] = {0};
    char ch, space = ' ', enter = '\n';
    calculate(workNum, processNum);
    for (int k = 1; k <= genNum; k++)
    {
        fd = open("ref.matrix", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        ref_line = 0, ref_row = 0;
        for (int i = 0; i < processNum; i++)
        {
            if ((pid[i] = fork()) == 0)
            {
                start = 0;
                for (int l = 0; l < i; l++)
                {
                    start += workNum[l];
                }
                lineRange[0] = start;
                lineRange[1] = start + workNum[i] - 1;
                check(&lineRange);
                for (int p = start; p < start + workNum[i]; p++)
                {
                    spaceNum = 0;
                    for (int q = 0; q <= row; q++)
                    {
                        pwrite(fd, &nextMatrix[p][q], 1, (p * ((2 * (row + 1)) + 1)) + q + spaceNum);
                        pwrite(fd, &space, 1, (p * ((2 * (row + 1)) + 1)) + q + spaceNum + 1);
                        spaceNum++;
                    }
                    pwrite(fd, &enter, 1, (p * ((2 * (row + 1)) + 1)) + row + spaceNum + 1);
                }
                close(fd);
                exit(0);
            }
        }
        while (wait(NULL) != -1)
            ;
        fd = open("ref.matrix", O_RDONLY);
        while (read(fd, &ch, 1) != 0)
        {
            switch (ch)
            {
            case ' ':
                ref_row++;
                break;
            case '\n':
                ref_line++;
                ref_row = 0;
                break;
            default:
                nextMatrix[ref_line][ref_row] = ch;
                break;
            }
        }
        close(fd);
        print(k);
        printf("%d Gen pid\n", k);
        for (int m = 0; m < processNum; m++)
        {
            printf("%d pid = %d\n", m + 1, pid[m]);
        }
        if (k != genNum)
        {
            memcpy(matrix, nextMatrix, sizeof(nextMatrix));
        }
    }
    remove("./ref.matrix");
}
void parallelThread()
{
    pthread_t tid[threadNum];
    int workNum[threadNum], start, **lineRange;
    calculate(workNum, threadNum);
    lineRange = (int **)malloc(sizeof(int *) * threadNum);
    for (int p = 0; p < threadNum; p++)
    {
        lineRange[p] = (int *)malloc(sizeof(int) * 2);
    }
    for (int k = 1; k <= genNum; k++)
    {
        for (int i = 0; i < threadNum; i++)
        {
            start = 0;
            for (int l = 0; l < i; l++)
            {
                start += workNum[l];
            }
            lineRange[i][0] = start;
            lineRange[i][1] = start + workNum[i] - 1;
            if (pthread_create(&tid[i], NULL, check, (void *)lineRange[i]) != 0)
            {
                printf("pthread_create error\n");
                exit(0);
            }
        }
        for (int q = 0; q < threadNum; q++)
        {
            pthread_join(tid[q], NULL);
        }
        print(k);
        printf("%d Gen tid\n", k);
        for (int m = 0; m < threadNum; m++)
        {
            printf("%d tid = %lu\n", m + 1, tid[m]);
        }
        if (k != genNum)
        {
            memcpy(matrix, nextMatrix, sizeof(nextMatrix));
        }
    }
}
void *check(void *lineRange)
{
    int start = ((int *)lineRange)[0];
    int end = ((int *)lineRange)[1];
    int cnt;
    for (int checkLine = start; checkLine <= end; checkLine++)
    {
        for (int i = 0; i <= row; i++)
        {
            cnt = 0;
            if (matrix[checkLine][i] - '0') //살아 있을 때
            {
                for (int j = 0; j < 3; j++)
                {
                    if (((checkLine + j - 1) >= 0) && ((checkLine + j - 1) <= line))
                    {
                        for (int k = 0; k < 3; k++)
                        {
                            if (((i + k - 1) >= 0) && ((i + k - 1) <= row))
                            {
                                if (!((j == 1) && (k == 1)))
                                {
                                    if (matrix[checkLine + j - 1][i + k - 1] - '0')
                                    {
                                        cnt++;
                                    }
                                }
                            }
                        }
                    }
                }
                if ((cnt <= 2) || (cnt >= 7))
                {
                    nextMatrix[checkLine][i] = '0';
                }
                else
                {
                    nextMatrix[checkLine][i] = '1';
                }
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    if (((checkLine + j - 1) >= 0) && ((checkLine + j - 1) <= line))
                    {
                        for (int k = 0; k < 3; k++)
                        {
                            if (((i + k - 1) >= 0) && ((i + k - 1) <= row))
                            {
                                if (!((j == 1) && (k == 1)))
                                {
                                    if (matrix[checkLine + j - 1][i + k - 1] - '0')
                                    {
                                        cnt++;
                                    }
                                }
                            }
                        }
                    }
                }
                if (cnt == 4)
                {
                    nextMatrix[checkLine][i] = '1';
                }
                else
                {
                    nextMatrix[checkLine][i] = '0';
                }
            }
        }
    }
    return NULL;
}
void print(int gen)
{
    FILE *fp;
    char fileName[20] = "gen_";
    char gener[15] = {0};
    if (gen == genNum)
    {
        strcpy(fileName, "output.matrix");
    }
    else
    {
        sprintf(gener, "%d", gen);
        strcat(fileName, gener);
        strcat(fileName, ".matrix");
    }
    fp = fopen(fileName, "w");
    for (int i = 0; i <= line; i++)
    {
        for (int j = 0; j <= row; j++)
        {
            fprintf(fp, "%c ", nextMatrix[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
void inputMatrix()
{
    char ch;
    FILE *fp;
    fp = fopen("input.matrix", "r");
    while ((ch = fgetc(fp)) != EOF)
    {
        switch (ch)
        {
        case ' ':
            row++;
            break;
        case '\n':
            line++;
            row = 0;
            break;
        default:
            matrix[line][row] = ch;
            break;
        }
    }
    fclose(fp);
    return;
}
void inputNum(int input)
{
    line = 0;
    row = 0;
    switch (input)
    {
    case 2:
        printf("Generation num : ");
        scanf("%d", &genNum);
        break;
    case 3:
        printf("Process num : ");
        scanf("%d", &processNum);
        printf("Generation num : ");
        scanf("%d", &genNum);
        break;
    case 4:
        printf("Thread num : ");
        scanf("%d", &threadNum);
        printf("Generation num : ");
        scanf("%d", &genNum);
        break;
    }
}
void calculate(int *workNum, int num)
{
    int refLine = line + 1;
    int remain = refLine % num;

    for (int i = 0; i < num; i++)
    {
        workNum[i] = refLine / num;
    }
    if (remain)
    {
        for (int j = 0; j < remain; j++)
        {
            workNum[j]++;
        }
    }
}