#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 5000
typedef char element;
typedef struct stack
{
    int data[MAX_STACK_SIZE];
    int top;
} StackType;

typedef struct var
{
    char name;
    int value;
} var;

//2+3*5+9
void init_stack(StackType *s);
int is_empty(StackType *s);
int is_full(StackType *s);
void push(StackType *s, element item);
int valid_exp(element *exp);
element peek(StackType *s);
int pop(StackType *s);
element *infix_to_postfix(element infix[]);
int eval(element *exp);
int prec(element op);
int is_digit(element item);

int main(void)
{
    var arr[100];
    int i = 0;
    int j = 0;
    int len = 0;
    char varname = '\0';
    char *exp = NULL;
    int type = -1;
    while (1)
    {
        element line[100];
        fgets(line, sizeof(line), stdin);

        for (i = 0; i < strlen(line); ++i)
        { //= �̶��? ���ڰ� ������
            if (line[i] == '=')
            {
                varname = line[i - 1]; //=���� �����̸�
                exp = &line[i + 1];    //=�ڴ� �������̴�
                break;
            }
        }

        if (i < strlen(line))
        { // for���� �������� i ���� line�� ���̺��� �۴ٸ� = �� �ִ� ���̶��? ���̹Ƿ�
            for (i = 0; i < len; ++i)
            {
                if (arr[i].name == varname)
                { //�����̸��� ã��
                    if (valid_exp(exp) != 0)
                    {
                        element *postfix = infix_to_postfix(exp); //���������� �����Ͽ�
                        arr[i].value = eval(postfix);             // ���� ����Ͽ�? �ش� �������� ������Ʈ�Ѵ�.
                        break;
                    }
                }
            }

            if (i == len)
            { //�� ���� �ش� ���� �̸��� �����迭�� ���� ����̹Ƿ�?
                element *postfix = infix_to_postfix(exp);
                arr[i].name = varname;
                arr[i].value = eval(postfix);
                ++len; //�迭�� ������ �ϳ� �ø��� ������ �Ҵ��Ѵ�.
            }
        }
        else
        {

            char *ptr = strstr(line, "print"); //print���? ���ڿ��� ����Ǹ�?
            char *exprptr = ptr + 6;           //�� ���� expression�� eprptr�� ����Ų��

            for (i = 6; i < strlen(ptr); ++i)
            {
                if ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z'))
                {
                    for (j = 0; j < len; ++j)
                    {
                        if (arr[j].name == ptr[i])
                        {                               //���� ����Ǿ��ִ�? ���� �̸��� ������
                            ptr[i] = arr[j].value + 48; // �� ���� �̸��� �ش� ���� ������ �ٲٰ�
                        }
                    }
                }
            }
            element *postfix = infix_to_postfix(exprptr); //���������� �ٲپ� ����ϰ�? ����Ѵ�?
            printf("%d\n", eval(postfix));
        }
    }
    return 0;
}

int is_digit(element item)
{
    if (item >= '0' && item <= '9')
        return 1;
    return 0;
}
int valid_exp(element *exp)
{
    int i = 0;
    int j = 0;

    StackType s;
    init_stack(&s);

    //��ȣ�ָ´��� üũ
    while (1)
    {
        if (exp[i] == '\0' && is_empty(&s))
            break;
        else if (exp[i] == '\0' && !is_empty(&s))
        {
            printf("\n<<error �߻�>>\n");
            printf("��ȣ ��Ī �Ұ���\n");
            return 0;
        }
        else if (exp[i] == '(')
        {
            push(&s, '(');
        }
        else if (exp[i] == ')')
        {
            if (s.top == -1)
            {
                printf("\n<<error �߻�>>\n");
                printf("��ȣ ��Ī �Ұ���\n");
                return 0;
            }
            pop(&s);
        }
        i += 1;
    }

    //�������� ���ڰ� �ΰ� ���������� ����
    i = 0;
    while (1)
    {
        if (exp[i] == '\0')
            break;
        else if (is_digit(exp[i]) && is_digit(exp[i + 1]))
        { //���ڸ� ���� �����̸�
            printf("\n<<error �߻�>>\n");
            printf("���ڸ� �� �̻��� �Է� ����\n");

            return 0;
        }
        i += 1;
    }

    //�������� /�� 0�� ���������� ����
    i = 0;
    while (1)
    {
        if (exp[i] == '\0')
            break;
        else if (exp[i] == '/' && exp[i + 1] == '0')
        {
            printf("\n<<error �߻�>>\n");
            printf("divide by 0\n");
            return 0;
        }
        i += 1;
    }

    //���� ���ڸ� �����ϴ��� Ȯ��
    i = 0;
    while (1)
    {
        if (exp[i] == '\0')
            break;
        else if (exp[i] == '^' || exp[i] == '%')
        {
            printf("\n<<error �߻�>>\n");
            printf("���ܹ��� ����\n");
            return 0;
        }
        i += 1;
    }

    //���? ���̽��� �� ����Ǹ�? 1 return
    return 1;
}

element *infix_to_postfix(element infix[])
{
    element *postfix_arr = (element *)(malloc(sizeof(element) * 100));
    StackType s;
    element x, token;
    int i, j; //i-index of infix,j-index of postfix
    init_stack(&s);
    j = 0;

    //infix to postfix algorithm
    for (i = 0; infix[i] != '\0'; i++)
    {
        token = infix[i];
        if (is_digit(token)) //�����̸� postfix_arr�� �ְ�
            postfix_arr[j++] = token;
        else //���� ��ȣ�̸� push
            if (token == '(')
            push(&s, '(');
        else //�ݴ°�ȣ�̸� ���� ��ȣ�� ���ö� ���� pop�ϰ� �װ��� postfix�� ����
            if (token == ')')
            while ((x = pop(&s)) != '(')
                postfix_arr[j++] = x;
        else
        {
            while (prec(token) <= prec(peek(&s)) && !is_empty(&s))
            {
                x = pop(&s);
                postfix_arr[j++] = x;
            }
            push(&s, token);
        }
    }

    while (!is_empty(&s))
    {
        x = pop(&s);
        postfix_arr[j++] = x;
    }

    postfix_arr[j] = '\0';

    return postfix_arr;
}

int prec(element op)
{
    //�켱������ �˻��Ѵ�.
    switch (op)
    {
    case '(':
    case ')':
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    }
    return -1;
}

void init_stack(StackType *s)
{
    s->top = -1;
}

int is_empty(StackType *s)
{
    return (s->top == -1);
}

int is_full(StackType *s)
{
    return (s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType *s, element item)
{
    if (is_full(s))
    {
        fprintf(stderr, "���� ��ȭ ����\n");
        return;
    }
    else
        s->data[++(s->top)] = item;
}

int pop(StackType *s)
{
    if (is_empty(s))
    {
        fprintf(stderr, "���� ���� ����\n");
        exit(1);
    }
    else
        return s->data[(s->top)--];
}

element peek(StackType *p)
{
    return (p->data[p->top]);
}

int eval(element exp[])
{
    int op1, op2, value, i = 0;
    int len = strlen(exp);
    element ch;
    StackType s;
    init_stack(&s);
    for (i = 0; i < len; i++)
    {
        ch = exp[i];
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/')
        {
            value = ch - '0'; // �Է��� �ǿ������̸�
            push(&s, value);
        }
        else
        { //�������̸� �ǿ����ڸ� ���ÿ��� ����
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch)
            { //������ �����ϰ� ���ÿ� ����
            case '+':
                push(&s, op1 + op2);
                break;
            case '-':
                push(&s, op1 - op2);
                break;
            case '*':
                push(&s, op1 * op2);
                break;
            case '/':
                push(&s, op1 / op2);
                break;
            }
        }
    }
    return pop(&s);
}
