/*
    Problem 4: Find the largest element among minimum values by row of a matrix
    Note: The matrix does not need to be squared, i just do it for simplicity.
*/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <omp.h>
#include <conio.h>
#include <random>

double LiToDouble (LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(double* &pMat, int &Size);
void RandomDataInitialization(double* pMat, int Size);
void ProcessTermination(double* pMat);
void PrintMatrix(double* pMat, int Size);

double Sequence_Res(double* pMat, int Size);
double Parallel_Res(double* pMat, int Size);

int main()
{
    double *pMatrix;
    int Size;
    double start, end;
    double res;

    ProcessInitialization(pMatrix, Size);
    // PrintMatrix(pMatrix, Size);

    start = GetTime();
    res = Sequence_Res(pMatrix, Size);
    end = GetTime();
    printf("Sequence: %lf in %lf sec.\n", res, end - start);

    start = GetTime();
    res = Parallel_Res(pMatrix, Size);
    end = GetTime();
    printf("Parallel: %lf in %lf sec.\n", res, end - start);

    ProcessTermination(pMatrix);
    return 0;
}

double LiToDouble(LARGE_INTEGER x)
{
    double result = ((double)x.HighPart) * 4.294967296E9 + (double)((x).LowPart);
    return result;
}
double GetTime()
{
    LARGE_INTEGER lpFrequency, lpPerformanceCount;
    QueryPerformanceFrequency (&lpFrequency);
    QueryPerformanceCounter (&lpPerformanceCount);
    return LiToDouble(lpPerformanceCount)/LiToDouble(lpFrequency);
}

void ProcessInitialization(double* &pMat, int &Size)
{
    do
    {
        printf("Size of the Matrix: ");
        scanf("%d", &Size);
        if (Size <= 0) {
            printf("Size of the Matrix must be largher than 0!");
        }
    } while (Size <= 0);

    pMat = new double[Size * Size];
    RandomDataInitialization(pMat, Size);
}
void RandomDataInitialization(double* pMat, int Size)
{
    int i, j;
    srand(unsigned(clock()));
    for (i = 0; i < Size; i++) {
        for (j = 0; j < Size; j++) {
            pMat[i*Size+j] = rand()/double(1000);
        }
    }
}
void ProcessTermination(double* pMat)
{
    delete[] pMat;
}
void PrintMatrix(double* pMat, int Size)
{
    int i, j;
    for (i = 0; i < Size; i++)
    {
        for (j = 0; j < Size; j++)
        {
            printf("%.3lf ", pMat[i*Size+j]);
        }
        printf("\n");
    }
    printf("\n");
}

double Sequence_Res(double* pMat, int Size)
{
    int i, j;
    double max, min;
    for (i = 0; i < Size; i++)
    {
        min = pMat[i*Size];
        for (j = 0; j < Size; j++)
        {
            if (pMat[i*Size+j] < min) {
                min = pMat[i*Size+j];
            }
        }
        if (i == 0 || min > max) {
            max = min;
        }
    }
    return max;
}
double Parallel_Res(double* pMat, int Size)
{
    int i, j;
    double max, min;
    #pragma omp parallel
    {
        omp_set_num_threads(omp_get_num_procs());
        #pragma omp for private(i, j) firstprivate(min)
            for (i = 0; i < Size; i++)
            {
                min = pMat[i*Size];
                for (j = 0; j < Size; j++)
                {
                    if (pMat[i*Size+j] < min) {
                        min = pMat[i*Size+j];
                    }
                }
                if (i == 0 || min > max){
                    max = min;
                }
            }
    }
    return max;
}