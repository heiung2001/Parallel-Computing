/*
    Problem 1: Find Min-Max of 1D Array
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <windows.h>

double LiToDouble (LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(double* &pArray, int &Size);
void RandomDataInitialization(double* pArray, int Size);
void ProcessTermination(double* pArray);

double Sequence_Max(double* pArray, int Size);
double Parallel_Max(double* pArray, int Size);

void PrintArray(double* pArray, int Size);

int main()
{
    double* pArray;
    double Max;
    int Size;
    double start, end;

    ProcessInitialization(pArray, Size);
    PrintArray(pArray, Size);

    start = GetTime();
    Max = Sequence_Max(pArray, Size);
    end = GetTime();
    printf("\nMax = %lf in %lf sec.\n", Max, end - start);

    start = GetTime();
    Max = Parallel_Max(pArray, Size);
    end = GetTime();
    printf("\nMax = %lf in %lf sec.\n", Max, end - start);


    ProcessTermination(pArray);
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

void ProcessInitialization(double* &pArray, int &Size)
{
    do
    {
        printf("Size of array: ");
        scanf("%d", &Size);
        if (Size <= 0) {
            printf("Size of array must be larger than 0!");
        }
    } while (Size <= 0);

    pArray = new double[Size];
    RandomDataInitialization(pArray, Size);
}
void RandomDataInitialization(double* pArray, int Size)
{
    int i;
    srand(time(NULL));
    for (i = 0; i < Size; i++)
    {
        pArray[i] = rand() / double(1000);
    }
}
void ProcessTermination(double* pArray)
{
    delete[] pArray;
}

void PrintArray(double* pArray, int Size)
{
    int i;
    for (i = 0; i < Size; i++)
    {
        printf("%7.2lf ", pArray[i]);
    }
}

double Sequence_Max(double* pArray, int Size)
{
    double Max = pArray[0];
    int i;
    for (i = 1; i < Size; i++)
    {
        if (Max < pArray[i]) {
            Max = pArray[i];
        }
    }
    return Max;
}
double Parallel_Max(double* pArray, int Size)
{
    double Max = 0;
    int i;
    #pragma omp parallel shared(Max)
    {
        omp_set_num_threads(omp_get_num_procs());
        #pragma omp for reduction(max : Max)
            for (i = 0; i < Size; i++)
            {
                Max = Max > pArray[i] ? Max : pArray[i];
            }
    }
    return Max;
}