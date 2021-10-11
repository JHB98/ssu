#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main()
{
    int a, c, *ans, n, r;
    char b;
    printf("Input number of repetitions : "); // 반복횟수 정함
    scanf("%d", &n);
    printf("ex) 1+3 (O), 1 + 3 (X)\n"); // 사용법 출력
    while (n--)
    {
        r = 1;
        printf("Input : ");
        scanf("%d%c%d", &a, &b, &c);
        switch (b) // b로 들어온 연산자에 따라 case문
        {
        case '+':
            syscall(442, ans, a, c);
            break;
        case '-':
            syscall(443, ans, a, c);
            break;
        case '*':
            syscall(444, ans, a, c);
            break;
        case '%':
            r = syscall(445, ans, a, c); // 0으로 나누었을 경우 에러처리를 위해 -1이 시스템콜로부터 return됨
            break;
        default: // 위의 case에 안걸리면 잘못된 연산자
            printf("ERROR : wrong operator");
        }
        if (r == -1) // 위의 에러처리
        {
            printf("ERROR : Mod with 0\n");
        }
        else
        {
            printf("Output : %d\n", *ans);
        }
    }
    return 0;
}