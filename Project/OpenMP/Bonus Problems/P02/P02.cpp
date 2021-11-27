/*
    Problem 2: Scalar product of 2 Vectors
*/

#include <stdio.h>
#include <omp.h>
#include <windows.h>
#include <vector>

double LiToDouble (LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(std::vector<double> &a, std::vector<double> &b, int &Size);
double Sequence_Res(std::vector<double> a, std::vector<double> b, int Size);
double Parallel_Res(std::vector<double> a, std::vector<double> b, int Size);

void Printvector(std::vector<double> vec);

int main()
{
    int Size;
    std::vector<double> a;
    std::vector<double> b;
    double res;
    double start, end;

    ProcessInitialization(a, b, Size);

    start = GetTime();
    res = Sequence_Res(a, b, Size);
    end = GetTime();
    printf("Sequence: %lf in %lf sec.\n", res,  end - start);

    start = GetTime();
    res = Parallel_Res(a, b, Size);
    end = GetTime();
    printf("Parallel: %lf in %lf sec.\n", res,  end - start);



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

void ProcessInitialization(std::vector<double> &a, std::vector<double> &b, int &Size)
{
    int i, j;
    do
    {
        printf("Size of Vectors: ");
        scanf("%d", &Size);
        if (Size <= 0) {
            printf("Size of vector must be larger than 0!");
        }
    } while (Size <= 0);

    a.resize(Size);
    b.resize(Size);

    for (i = 0; i < Size; i++)
    {
        a[i] = rand() / double(1000);
        b[i] = rand() / double(1000);
    }
}
double Sequence_Res(std::vector<double> a, std::vector<double> b, int Size)
{
    int i;
    double res = 0.0;
    for (i = 0; i < Size; i++)
    {
        res += a.at(i) * b.at(i); 
    }
    return res;
}
double Parallel_Res(std::vector<double> a, std::vector<double> b, int Size)
{
    int i;
    double res = 0.0;
    #pragma omp parallel for private(i) reduction(+:res)
        for (i = 0; i < Size; i++)
        {
            res += a.at(i) * b.at(i);
        }
    return res;
}

void Printvector(std::vector<double> vec)
{
    int i;
    for (i = 0; i < vec.size(); i++)
    {
        printf("%.3lf ", vec[i]);
    }
    printf("\n");
}
