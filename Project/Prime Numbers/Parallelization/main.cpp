#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <cstring>
#include <math.h>
#include <omp.h>
#include <windows.h>

double LiToDouble(LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(char* &pEratos, int &n, int &Size);
void ProcessTermination(char* &pEratos);

int SerialResult(char* pEratos, int n, int Size);
int ParallelResult(char* pEratos, int n, int Size, bool useOpenMP);

int main()
{
    int N = 0, Size = 0;
    int res;
    char* pEratosthenes;
    double start, end, duration;
    
    ProcessInitialization(pEratosthenes, N, Size);

    start = GetTime();
    res = SerialResult(pEratosthenes, N, Size);
    end = GetTime();
    duration = end - start;
    printf("\nIn serialism: %d primes was found.\tTime of execution: %f(s)\n", res, duration);

    start = GetTime();
    res = ParallelResult(pEratosthenes, N, Size, 1);
    end = GetTime();
    duration = end - start;

    printf("\nIn parallelism: %d primes was found.\tTime of execution: %f(s)\n", res, duration);
    
    ProcessTermination(pEratosthenes);
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

void ProcessInitialization(char* &pEratos, int &n, int &Size)
{
    do {
        printf("\nInput N: ");
        scanf("%d", &n);

        if (n < 2) {
            printf("N must be larger than 2!\n");
        }
    } while (n < 2);

    // Only representing odd numbers
    Size = (n-1)/2;

    // Size+1 means the array contains the number n
    pEratos = new char[Size+1];
    if (!pEratos) {
        printf("\nError Allocating Memory!\n");
        exit(0);
    }
    memset(pEratos, 1, Size+1);
}
void ProcessTermination(char* &pEratos)
{
    delete[] pEratos;
}

int SerialResult(char* pEratos, int n, int Size)
{
    int i, loop = 1, prime;
    int number_of_primes = 1;

    for (i = 3; i*i <= n; i += 2)
    {
        if (pEratos[i/2]) {
            for (int j = i*i; j <= n; j += 2*i) {
                pEratos[j/2] = 0;
            }
        }
    }

    for (i = 1; i < Size; i++)
    {
        number_of_primes += pEratos[i];
    }

    return number_of_primes;
}
int ParallelResult(char* pEratos, int n, int Size, bool useOpenMP)
{
    omp_set_num_threads(useOpenMP ? omp_get_num_procs() : 1);

    const int Isqrt = (int)sqrt((double)n);
    int number_of_primes = 1;

    #pragma omp parallel for schedule(dynamic)
        for (int i = 3; i <= Isqrt; i += 2) {
            if (pEratos[i/2]) {
                for (int j = i*i; j <= n; j += 2*i) {
                    pEratos[j/2] = 0;
                }
            }
        }
    #pragma omp parallel for reduction(+:number_of_primes)
        for (int i = 1; i <= Size; i++) {
            number_of_primes += pEratos[i];
        }
    
    return number_of_primes;
}