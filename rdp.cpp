#pragma warning(disable : 4996)
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
enum
{
    PLUS,
    STAR,
    NUMBER,
    LP,
    RP,
    NN,
    END
} token;
char str[1000];
int i = 0;
void get_next_token();
void expr();
void term();
void factor();
void error();
char getChar();
int main()
{
    gets_s(str);
    get_next_token();
    expr();
    if (token == END)
    {
        printf("OK");
    }

    return 0;
}
char getChar()
{
    return str[i++];
}
void get_next_token()
{
    if (isdigit(getChar()))
    {
        while (isdigit(getChar()))
        {
        }
        token = NUMBER;
        i--;
    }
    else
    {
        i--;
        switch (getChar())
        {
        case '+':
            token = PLUS;
            break;
        case '*':
            token = STAR;
            break;
        case '(':
            token = LP;
            break;
        case ')':
            token = RP;
            break;
        case '\0':
            token = END;
            break;
        default:
            token = NN;
        }
    }
}
void expr()
{
    term();
    while (token == PLUS)
    {
        get_next_token();
        term();
    }
}
void term()
{
    factor();
    while (token == STAR)
    {
        get_next_token();
        factor();
    }
}
void factor()
{
    if (token == NUMBER)
    {
        get_next_token();
    }
    else if (token == LP)
    {
        get_next_token();
        expr();
        if (token == RP)
        {
            get_next_token();
        }
        else
        {
            error();
        }
    }
    else
        error();
}
void error()
{
    printf("ERROR");
    exit(0);
}