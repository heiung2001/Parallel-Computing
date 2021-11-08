#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define SEED 1053608

double LiToDouble(LARGE_INTEGER x);
double GetTime();

void ProcessInitialization(int &n, double& dx);

double SerialResult(int n, double& integral, double dx);

int main()
{
    int N = 0;
    double pi, dx, integral = 0.0;

    ProcessInitialization(N, dx);

    pi = SerialResult(N, integral, dx);

    printf("%f\n", pi);

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
        printf("Input number of sample: ");
        scanf("%d", &n);
        if (n <= 0) {
            printf("Number of Sample must be positive");
        }
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