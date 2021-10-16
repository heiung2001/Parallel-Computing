#include <stdio.h>
#include <omp.h>

#define M 3
#define N 3

void readMtx(int (*a)[N], const char *filename)
{
    FILE *pf;
    pf = fopen(filename, "r");
    if (pf == NULL)
        return;

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fscanf(pf, "%d", a[i] + j);
        }
    }
    fclose(pf);
}

void printMtx(int (*a)[N])
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

void collapse_Mul(int (*a)[N], int (*b)[N], int (*c)[N])
{
#pragma omp parallel for collapse(2)
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

void normal_Mul(int (*a)[N], int (*b)[N], int (*c)[N])
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

int main()
{
    int a[M][N];
    int b[M][N];
    int res[M][N];
    readMtx(a, "a.txt");
    printMtx(a);
    readMtx(b, "b.txt");

    collapse_Mul(a, b, res);
    printMtx(res);
    return 0;
}