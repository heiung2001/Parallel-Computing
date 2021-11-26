#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <windows.h>

double LiToDouble (LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(double* &pArray, int &Size);
void RandomDataInitialization(double* pArray, int Size);

void PrintArray(double* pArray, int Size);

int main()
{
    double* pArray;
    int Size;

    ProcessInitialization(pArray, Size);

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

void PrintArray(double* pArray, int Size)
{
    int i;
    for (i = 0; i < Size; i++)
    {
        printf("7.17%lf ", pArray[i]);
    }
}