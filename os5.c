#include <stdio.h>
#include <stdbool.h>
#include <string.h>
int frame[4], ref[30], maxFrame, maxRef, fifoPointer, scPointer;
bool chance[30];           // second-chance 표시, true면 기회 한번 더 있음
void printHead(char *name) // 결과 앞부분 5줄 출력
{
    printf("Used method : %s\n", name);
    printf("page reference string : ");
    for (int i = 0; i < maxRef; i++)
    {
        printf("%d ", ref[i]);
    }
    printf("\n\n\tframe\t");
    for (int i = 0; i < maxFrame; i++)
    {
        printf("%d\t", i + 1);
    }
    printf("page fault\ntime\n");
}
int check(int pageNum) // 비어있는 frame이 있거나 이미 들어있는 page인지 확인
{
    int index = -1;
    for (int i = 0; i < maxFrame; i++)
    {
        if ((!frame[i]) || (frame[i] == pageNum))
        {
            index = i;
            break;
        }
    }
    return index;
}
void printNow(int time, bool fault) // 현재 time, frame 상태 및 page fault가 났는지 출력
{
    printf("%d\t\t", time);
    for (int i = 0; i < maxFrame; i++)
    {
        if (frame[i])
        {
            printf("%d\t", frame[i]);
        }
        else
        {
            printf(" \t");
        }
    }
    if (fault)
    {
        printf("F\n");
    }
    else
    {
        printf("\n");
    }
}
void opt(int pageNum, int index) // opt방식, 현재 index 기준으로 각각의 frame안의 page 중 가장 오랫동안
{                                //참조 안될 page를 찾음
    int cnt, ans = -1, ansIndex;
    for (int i = 0; i < maxFrame; i++)
    {
        cnt = 0;
        for (int j = index; j < maxRef; j++)
        {
            if (ref[j] == frame[i])
            {
                break;
            }
            cnt++;
        }
        if (cnt > ans) // 가장 큰 (가장 오래 참조안될) cnt를 가진 page가 바뀜
        {
            ans = cnt;
            ansIndex = i;
        }
    }
    frame[ansIndex] = pageNum;
}
void fifo(int pageNum) //fifo 방식, fifo 포인터를 두고 frame을 돌면서 다음 victim을 찾음
{
    frame[fifoPointer++] = pageNum;
    if (fifoPointer == maxFrame)
    {
        fifoPointer = 0;
    }
}
void lru(int pageNum, int index) // lru방식, 현재 index 기준으로 각각의 frame안의 page 중 가장 오랫동안
{                                //참조 안된 page를 찾음
    int cnt, ans = -1, ansIndex;
    for (int i = 0; i < maxFrame; i++)
    {
        cnt = 0;
        for (int j = index; j >= 0; j--)
        {
            if (ref[j] == frame[i])
            {
                break;
            }
            cnt++;
        }
        if (cnt > ans) // 가장 큰 (가장 오래 참조안된) cnt를 가진 page가 바뀜
        {
            ans = cnt;
            ansIndex = i;
        }
    }
    frame[ansIndex] = pageNum;
}
void second(int pageNum) //second-chance 방식, fifo방식과 동일하나 chance배열로 한번 더 기회를 주는지 확인
{
    while (chance[scPointer])
    {
        chance[scPointer++] = false;
        if (scPointer == maxFrame)
        {
            scPointer = 0;
        }
    }
    frame[scPointer++] = pageNum;
    if (scPointer == maxFrame)
    {
        scPointer = 0;
    }
}
void base()
{
    int time, index, cnt;
    bool fault;
    char name[4][20] = {"OPT", "FIFO", "LRU", "Second-Chance"}; // 4가지 경우를 반복문 돌리기 위해 필요
    for (int j = 0; j < 4; j++)
    {
        printHead(name[j]); // head 부분 출력
        time = 1;           // 4가지 경우 할때 마다 time은 1부터
        cnt = 0;
        memset(frame, 0, sizeof(frame)); // frame안의 값이 0이면 비어있음
        for (int i = 0; i < maxRef; i++)
        {
            fault = false;                     // page fault가 났는지 확인
            if ((index = check(ref[i])) != -1) // frame들 중에 비어있는 값이 있거나 이미 들어있는 page라면
            {
                if (!frame[index]) // frame[index]값이 0이면 비어있다는 뜻
                {
                    frame[index] = ref[i];
                    fault = true;
                    cnt++;
                }
                else // frame[index]값이 0이 아닌 다른 수라면 해당 page는 이미 있다는 뜻
                {
                    if (!strcmp(name[j], "Second-Chance"))
                    {
                        chance[index] = true; // 관리 기법이 second-chance라면 기회를 한번 더 주게 설정
                    }
                }
            }
            else // frame들 중에 비어있는 값이 없으면, 아래 기법들중 현재 설정된 기법으로 돌아가게함
            {
                if (!strcmp(name[j], "OPT"))
                {
                    opt(ref[i], i + 1);
                }
                else if (!strcmp(name[j], "FIFO"))
                {
                    fifo(ref[i]);
                }
                else if (!strcmp(name[j], "LRU"))
                {
                    lru(ref[i], i - 1);
                }
                else // second-chance
                {
                    second(ref[i]);
                }
                fault = true;
                cnt++;
            }
            printNow(time++, fault); // 한번 끝날때 마다 현재 상황 출력
        }
        printf("Number of page faults : %d times\n\n", cnt); // 마지막 총 page fault 출력
    }
}
int main()
{
    FILE *fp;
    char fileName[50], buf;
    printf("Input file name : "); // 파일 이름 입력받음
    scanf("%s", fileName);
    fp = fopen(fileName, "r");  // 파일 read로 오픈
    maxFrame = fgetc(fp) - '0'; // frame size 얻어옴
    fgetc(fp);
    while ((buf = fgetc(fp)) != EOF)
    {
        if (buf != ' ')
        {
            ref[maxRef++] = buf - '0'; // reference string 하나씩 얻어오면서 size측정
        }
    }
    fclose(fp);
    base(); // 기본 프로그램 실행
    return 0;
}