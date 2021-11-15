#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <conio.h>
#include <windows.h>

double LiToDouble(LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(double* &pMat, double* &pVec, double* &pRes, int &Size);
void ProcessTermination(double* pMat, double* pVec, double* pRes);
void DummyDataInitialization(double* pMat, double* pVec, double* pRes, int Size);
void RandomDataInitialization(double* pMath, double* pVec, int Size);

void PrintMatrix(double* pMat, int RowCount, int ColCount);
void PrintVector(double* pVec, int Size);

void SerialResultCalculation(double* pMat, double* pVec, double* pRes, int Size);

int main()
{
    double* pMatrix;
    double* pVector;
    double* pResult;
    double start, finish, duration;
    int Size;

    printf ("Serial matrix-vector multiplication program\n");
    ProcessInitialization(pMatrix, pVector, pResult, Size);

    printf("\nInitial Matrix:\n");
    // PrintMatrix(pMatrix, Size, Size);
    printf("\nInitial Vector:\n");
    // PrintVector(pVector, Size);

    start = GetTime();
    SerialResultCalculation(pMatrix, pVector, pResult, Size);
    finish = GetTime();
    duration = finish - start;

    printf("\nResult Vector:\n");
    // PrintVector(pResult, Size);
    printf("\nTime of execution: %f\n", duration);

    ProcessTermination(pMatrix, pVector, pResult);
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

void ProcessTermination(double* pMat, double* pVec, double* pRes)
{
    delete [] pMat;
    delete [] pVec;
    delete [] pRes;
}
void ProcessInitialization(double* &pMat, double* &pVec, double* &pRes, int &Size)
{
    do {
    printf("\nEnter size of the initial objects: ");
    scanf("%d", &Size);
    printf("\nChosen objects size = %d", Size);
    if (Size <= 0) {
        printf("\nSize of objects must be greater than 0!\n");
    }        
    } while (Size <= 0);

    pMat = new double[Size*Size];
    pVec = new double[Size];
    pRes = new double[Size];

    // DummyDataInitialization(pMat, pVec, pRes, Size);
    RandomDataInitialization(pMat, pVec, Size);
}
void DummyDataInitialization(double* pMat, double* pVec, double* pRes, int Size)
{
    int i, j;

    for (i = 0; i < Size; i++) {
        pVec[i] = 1;
        for (j = 0; j < Size; j++) {
            pMat[i*Size+j] = i;
        }
    }
}
void RandomDataInitialization(double* pMath, double* pVec, int Size)
{
    int i, j;
    srand(unsigned(clock()));
    for (i = 0; i < Size; i++) {
        pVec[i] = rand()/double(1000);
        for (j = 0; j < Size; j++) {
            pMath[i*Size+j] = rand()/double(1000);
        }
    }
}

void PrintMatrix(double* pMat, int RowCount, int ColCount) {
    int i, j; 

    for (i = 0; i < RowCount; i++) {
        for (j = 0; j < ColCount; j++) {
            printf("%7.4f ", pMat[i*ColCount+j]);
        }
        printf("\n");
    }
}
void PrintVector(double* pVec, int Size) {
    int i;
    for (i=0; i<Size; i++) {
        printf("%7.4f ", pVec[i]);
    }
    printf("\n");
}

void SerialResultCalculation(double* pMat, double* pVec, double* pRes, int Size)
{
    int i, j;

    for (i = 0; i < Size; i++) {
        pRes[i] = 0;
        for (j = 0; j < Size; j++) {
            pRes[i] += pMat[j*Size+i]*pVec[j];
        }
    }
}