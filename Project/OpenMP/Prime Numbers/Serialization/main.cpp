#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <cstring>
#include <math.h>

void ProcessInitialization(char* &pEratos, int &n, int &Size);
void ProcessTermination(char* &pEratos);

int SerialResult(char* pEratos, int n, int Size);

int main()
{
    int N = 0, Size = 0;
    char* pEratosthenes;
    
    ProcessInitialization(pEratosthenes, N, Size);

    int res = SerialResult(pEratosthenes, N, Size);

    printf("\n%d\n", res);
    
    ProcessTermination(pEratosthenes);
    return 0;
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

    for (i = 1; i <= Size; i++)
    {
        number_of_primes += pEratos[i];
    }

    return number_of_primes;
}