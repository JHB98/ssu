#pragma warning(disable : 4996)
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
typedef enum e
{
    ERROR,
    NUMBER,
    ID,
    PRINT,
    PLUS,
    STAR,
    LP,
    RP,
    RET,
    END,
    EQL
} TOKEN;
TOKEN token;
int num;
int array[26];
char ch = ' ';
TOKEN get_next_token();
void statement();
int expr();
int term();
int factor();
void error();
int main()
{
    printf("�����Ϸ��� �ƹ��͵� �Է����� �ʰ� ENTER�� �Է��ϼ���.\n");
    while (1)
    {
        token = get_next_token();
        statement();
    }
    return 0;
}
TOKEN get_next_token()
{
    char string[32];
    int i = 0;
    while ((ch == ' ') || (ch == '\t'))
    {
        ch = getchar();
    }

    if (ch == '=')
    {
        ch = getchar();
        return EQL;
    }

    if (ch == '+')
    {
        ch = getchar();
        return PLUS;
    }

    if (ch == '*')
    {
        ch = getchar();
        return STAR;
    }

    if (ch == '(')
    {
        ch = getchar();
        return LP;
    }

    if (ch == ')')
    {
        ch = getchar();
        return RP;
    }

    if (ch == '\n')
    {
        ch = ' ';
        return RET;
    }

    if (ch == EOF)
    {
        ch = getchar();
        return END;
    }

    if ((ch >= '0') && (ch <= '9'))
    {
        num = 0;
        do
        {
            num = num * 10 + ch - '0';
            ch = getchar();
        } while ((ch >= '0') && (ch <= '9'));
        return NUMBER;
    }

    if ((ch >= 'a') && (ch <= 'z'))
    {
        do
        {
            string[i++] = ch;
            ch = getchar();
        } while ((ch >= 'a') && (ch <= 'z'));
        string[i] = 0;

        if (strcmp(string, "print") == 0)
        {
            return PRINT;
        }

        if (strlen(string) == 1)
        {
            num = string[0] - 'a';
            return ID;
        }
    }
    return ERROR;
}
void statement()
{
    int idnum;
    if (token == ID)
    {
        idnum = num;
        token = get_next_token();
        if (token == EQL)
        {
            token = get_next_token();
            array[idnum] = expr();
        }
        else
        {
            error();
        }
    }

    else if (token == PRINT)
    {
        token = get_next_token();
        printf("%d\n", expr());
    }

    else if (token == RET)
    {
        printf("finish");
        exit(0);
    }

    else
    {
        error();
    }
}
int expr()
{
    int r;
    r = term();
    while (token == PLUS)
    {
        token = get_next_token();
        r += term();
    }
    return r;
}
int term()
{
    int r;
    r = factor();
    while (token == STAR)
    {
        token = get_next_token();
        r *= factor();
    }

    return r;
}
int factor()
{
    int r;
    if (token == NUMBER)
    {
        r = num;
        token = get_next_token();
    }
    else if (token == ID)
    {
        r = array[num];
        token = get_next_token();
    }
    else if (token == LP)
    {
        token = get_next_token();
        r = expr();
        if (token == RP)
        {
            token = get_next_token();
        }
        else
        {
            error();
        }
    }
    else
    {
        error();
    }

    return r;
}
void error()
{
    printf("ERROR");
    exit(0);
}