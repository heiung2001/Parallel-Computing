#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <windows.h>

double LiToDouble(LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(int &n, double& dx);

double SerialResult(int n, double& integral, double dx);
double ParallelResult(int n, double& integral, double dx);

int main()
{
    int N = 0;
    double pi, dx, integral = 0.0;
    double start, end, duration;

    ProcessInitialization(N, dx);

    // start = GetTime();
    start = omp_get_wtime();
    pi = SerialResult(N, integral, dx);
    // end = GetTime();
    end = omp_get_wtime();
    duration = end - start;
    printf("PI = %f. Time: %f\n", pi, duration);

    integral = 0.0;

    // start = GetTime();
    start = omp_get_wtime();
    pi = ParallelResult(N, integral, dx);
    // end = GetTime();
    end = omp_get_wtime();
    duration = end - start;
    printf("PI = %f. Time: %f", pi, duration);


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

void ProcessInitialization(int &n, double& dx)
{
    do {
        printf("Input a number: ");
        scanf("%d", &n);
        if (n <= 0) {
            printf("Number must be positive!\n\n");
        }
        fflush(stdin);
    } while (n <= 0);
    dx = 1.0 / n;
}

double SerialResult(int n, double& integral, double dx)
{
    double fx, x, pi;
    for (int i = 0; i < n; i++) {
        x = i * dx;
        fx = sqrt(1.0 - x*x);
        integral += fx * dx;
    }
    pi = 4 * integral;
    return pi;
}
double ParallelResult(int n, double& integral, double dx)
{
    double fx, x, pi;
    #pragma omp parallel for private(fx, x) reduction(+:integral)
        for (int i = 0; i < n; i++) {
            x = i * dx;
            fx = sqrt(1.0 - x * x);
            integral += fx * dx;
        }
    pi = 4 * integral;
    return pi;
}
