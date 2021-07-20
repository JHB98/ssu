#include <stdio.h>
#include <stdlib.h>
void input(double **matrix, int size);
void swap(double **matrix, int size, int num1, int num2);
void print(double **matrix, int size);
void gauss_jordan(double **matrix, int size);
int main(void)
{
    int size;
    double **matrix;
    printf("Matrix size: ");
    scanf("%d", &size);
    matrix = (double **)malloc(sizeof(double *) * size);
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (double *)malloc(sizeof(double) * (size + 1));
    }
    input(matrix, size);
    printf("\n");
    gauss_jordan(matrix, size);

    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}
void input(double **matrix, int size)
{
    printf("input Elements:\n");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size + 1; j++)
        {
            scanf("%lf", &matrix[i][j]);
        }
    }
}
void swap(double **matrix, int size, int num1, int num2)
{
    for (int i = 0; i < size + 1; i++)
    {
        int temp = matrix[num1][i];
        matrix[num1][i] = matrix[num2][i];
        matrix[num2][i] = temp;
    }
}
void print(double **matrix, int size)
{
    printf("output : \n");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size + 1; j++)
        {
            if (matrix[i][j] == -0)
            {
                matrix[i][j] = 0;
            }
            printf("%4.0f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void gauss_jordan(double **matrix, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (!matrix[i][i])
        {
            for (int j = i + 1; j < size; j++)
            {
                if (matrix[j][i])
                {
                    swap(matrix, size, i, j);
                    break;
                }
            }
            i--;
            print(matrix, size);
        }
        else
        {
            double divide = matrix[i][i];
            for (int j = 0; j < size + 1; j++)
            {
                matrix[i][j] *= (1 / divide);
            }
            print(matrix, size);
            for (int j = i + 1; j < size; j++)
            {
                double multiply = matrix[j][i];
                for (int k = 0; k < size + 1; k++)
                {
                    matrix[j][k] -= (matrix[i][k] * multiply);
                }
                print(matrix, size);
            }
        }
    }
    double temp = matrix[size - 1][size - 1];
    for (int i = 0; i < size + 1; i++)
    {
        matrix[size - 1][i] *= (1 / temp);
    }
    print(matrix, size);
    for (int i = size - 1; i > 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            double multiply = matrix[j][i];
            for (int k = i; k < size + 1; k++)
            {
                matrix[j][k] -= (matrix[i][k] * multiply);
            }
            print(matrix, size);
        }
    }
}