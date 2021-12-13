#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
pthread_mutex_t ul = PTHREAD_MUTEX_INITIALIZER; // 중앙도로를 4등분 했을때, 좌측상단
pthread_mutex_t ur = PTHREAD_MUTEX_INITIALIZER; // 중앙도로를 4등분 했을때, 우측상단
pthread_mutex_t dl = PTHREAD_MUTEX_INITIALIZER; // 중앙도로를 4등분 했을때, 좌측하단
pthread_mutex_t dr = PTHREAD_MUTEX_INITIALIZER; // 중앙도로를 4등분 했을때, 우측하단
typedef struct car
{
    int num;
    int time;
} car; // 자동차 구조체
bool check;
int num, mainPointer = -1, p1Pointer, p2Pointer, p3Pointer, p4Pointer, tick, p1Cnt, p2Cnt, p3Cnt, p4Cnt;
car mainWait[15];                                       //main함수 버퍼
car *p1Wait[15], *p2Wait[15], *p3Wait[15], *p4Wait[15]; // 출구 버퍼
void *p1(void *arg)                                     // 출구 함수
{
    int goal, pointer = 0;
    while (1)
    {
        if (check) // 프로그램이 종료되어야 하는지 확인
        {
            break;
        }
        if (p1Wait[pointer]->num) // 이 출구에 차가 들어오면
        {
            pthread_mutex_lock(&ul);     //mutex lock 요청
            pthread_mutex_lock(&dl);     //mutex lock 요청
            p1Wait[pointer++]->time = 1; // 출발했으니 시간 1로 바꿔주고
            goal = tick + 2;             //2 tick 동안 멈춤
            while (goal != tick)
                ;
            p1Cnt++;                   //해당 출구에서 몇대 출발했는지 count
            pthread_mutex_unlock(&ul); //mutex 언락
            pthread_mutex_unlock(&dl);
        }
    }
}
void *p2(void *arg) //p1과 동일
{
    int goal, pointer = 0;
    while (1)
    {
        if (check)
        {
            break;
        }
        if (p2Wait[pointer]->num)
        {
            pthread_mutex_lock(&ul);
            pthread_mutex_lock(&ur);
            p2Wait[pointer++]->time = 1;
            goal = tick + 2;
            while (goal != tick)
                ;
            p2Cnt++;
            pthread_mutex_unlock(&ul);
            pthread_mutex_unlock(&ur);
        }
    }
}
void *p3(void *arg) //p1과 동일
{
    int goal, pointer = 0;
    while (1)
    {
        if (check)
        {
            break;
        }
        if (p3Wait[pointer]->num)
        {
            pthread_mutex_lock(&ur);
            pthread_mutex_lock(&dr);
            p3Wait[pointer++]->time = 1;
            goal = tick + 2;
            while (goal != tick)
                ;
            p3Cnt++;
            pthread_mutex_unlock(&ur);
            pthread_mutex_unlock(&dr);
        }
    }
}
void *p4(void *arg) //p1과 동일
{
    int goal, pointer = 0;
    while (1)
    {
        if (check)
        {
            break;
        }
        if (p4Wait[pointer]->num)
        {
            pthread_mutex_lock(&dr);
            pthread_mutex_lock(&dl);
            p4Wait[pointer++]->time = 1;
            goal = tick + 2;
            while (goal != tick)
                ;
            p4Cnt++;
            pthread_mutex_unlock(&dr);
            pthread_mutex_unlock(&dl);
        }
    }
}
void printHead() // total 자동차 수와 내용 출력
{
    printf("Total number of vehicles : %d\n", num);
    printf("Start point : ");
    for (int i = 0; i < num; i++)
    {
        printf("%d ", mainWait[i].num);
    }
    printf("\n");
}
void printTick() //현재 tick 상황 출력
{
    printf("tick : %d\n", tick);
    printf("=========================\n");
    printf("Passed Vehicle\nCar ");
    for (int i = 0; i < num; i++)
    {
        if (mainWait[i].time == 2)
        {
            printf("%d ", mainWait[i].num);
        }
    }
    printf("\nWaiting Vehicle\nCar ");
    for (int i = 0; i < num; i++)
    {
        if (!mainWait[i].time)
        {
            printf("%d ", mainWait[i].num);
        }
    }
    printf("\n=========================\n");
}
void printTail()
{
    printf("Number of vehicles passed from each start point\n");
    printf("P1 : %d times\n", p1Cnt);
    printf("P2 : %d times\n", p2Cnt);
    printf("P3 : %d times\n", p3Cnt);
    printf("P4 : %d times\n", p4Cnt);
    printf("Total time : %d ticks\n", tick);
}
void increase()
{
    for (int i = 0; i < num; i++)
    {
        if (mainWait[i].time >= 1)
        {
            mainWait[i].time++;
        }
    }
    tick++;
    mainPointer++;
}
int main()
{
    pthread_t tid[4];
    printf("Input num : ");
    scanf("%d", &num); // 자동차 수 입력
    srand(time(NULL));
    for (int i = 0; i < num; i++)
    {
        mainWait[i].num = rand() % 4 + 1; //랜덤 정수 생성해서 리스트에 넣음
        mainWait[i].time = -1;
    }
    for (int i = 0; i < 15; i++) // 각 출발지점의 버퍼 생성
    {
        p1Wait[i] = calloc(1, sizeof(struct car));
        p2Wait[i] = calloc(1, sizeof(struct car));
        p3Wait[i] = calloc(1, sizeof(struct car));
        p4Wait[i] = calloc(1, sizeof(struct car));
    }
    printHead(); //앞부분 출력
    while (1)
    {
        if (!tick) // 0 tick 때, 쓰레드 4개 생성
        {
            pthread_create(&tid[0], NULL, p1, NULL);
            pthread_create(&tid[1], NULL, p2, NULL);
            pthread_create(&tid[2], NULL, p3, NULL);
            pthread_create(&tid[3], NULL, p4, NULL);
        }
        else
        {
            if (mainPointer < num) // tick이 지나면서 추가되는 차량이 있다면
            {
                switch (mainWait[mainPointer].num) // 몇번 출구인지 확인하고 포인터로 연결해줌
                {
                case 1:
                    mainWait[mainPointer].time = 0;
                    p1Wait[p1Pointer++] = &mainWait[mainPointer];
                    break;
                case 2:
                    mainWait[mainPointer].time = 0;
                    p2Wait[p2Pointer++] = &mainWait[mainPointer];
                    break;
                case 3:
                    mainWait[mainPointer].time = 0;
                    p3Wait[p3Pointer++] = &mainWait[mainPointer];
                    break;
                case 4:
                    mainWait[mainPointer].time = 0;
                    p4Wait[p4Pointer++] = &mainWait[mainPointer];
                    break;
                }
            }
            usleep(50000);
            printTick(); //현재 tick상황 출력
        }
        if ((mainPointer >= num) && ((p1Pointer == 0) || (p1Pointer > 0) && (p1Wait[p1Pointer - 1]->time) > 2) &&
            ((p2Pointer == 0) || (p2Pointer > 0) && (p2Wait[p2Pointer - 1]->time) > 2) &&
            ((p3Pointer == 0) || (p3Pointer > 0) && (p3Wait[p3Pointer - 1]->time) > 2) &&
            ((p4Pointer == 0) || (p4Pointer > 0) && (p4Wait[p4Pointer - 1]->time) > 2)) //프로그램 종료조건
        {
            check = true;
            break;
        }
        increase(); // tick과 시간 1씩 추가
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL); // thread join 기다림
    }
    printTail();                // 끝부분 출력
    pthread_mutex_destroy(&ul); //mutex free
    pthread_mutex_destroy(&ur);
    pthread_mutex_destroy(&dl);
    pthread_mutex_destroy(&dr);
    return 0;
}