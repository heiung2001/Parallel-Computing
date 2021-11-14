#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <windows.h>

#define GT (1.0 + sqrt(5.0))/2.0

void ProcessInitialization(int &n);

double LiToDouble(LARGE_INTEGER x);
double GetTime();

long double SerialResult(unsigned int n);
long double ParallelResult(unsigned int n);

void Test(unsigned int n);


int main()
{
	int n;
	long double res;
	double start, end;

	ProcessInitialization(n);

	start = GetTime();
	res = SerialResult(n);
	end = GetTime();
	printf("Fib(%d): %.0Lf.\nTime: %lf\n", n, res, end - start);

	start = GetTime();
	res = ParallelResult(n);
	end = GetTime();
	printf("\nFib(%d): %.0Lf.\nTime: %lf", n, res, end - start);

	Test(n);

	return 0;
}

void ProcessInitialization(int &n)
{
	do {
		printf("Input a number: ");
		scanf("%d", &n);
		if (n < 3) {
			printf("\nNumber must be larger than 2\n");
		}
	} while (n < 3);
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

long double SerialResult(unsigned int n)
{
	long double res;
	long double Phi = GT;
	long double oMinusPhi = 1 - GT;
	for (int i = 0; i < n-1; i++) {
		Phi *= GT;
		oMinusPhi *= (1-GT);
	}
	res = (Phi-oMinusPhi)/sqrt(5);
	return res;
}
long double ParallelResult(unsigned int n)
{
	long double res;
	long double Phi = GT;
	long double oMinusPhi = 1 - GT;
	
	#pragma omp parallel for reduction(*: Phi, oMinusPhi)
		for (int i = 0; i < n-1; i++) {
			Phi *= GT;
			oMinusPhi *= (1-GT);
		}
	res = (Phi-oMinusPhi)/sqrt(5);
	return res;
}

void Test(unsigned int n)
{
	long double serialRes = SerialResult(n);
	long double parallelRes = ParallelResult(n);

	if (rint(serialRes) == rint(parallelRes)) {
		printf("\nTrue!");
	}
	else {
		printf("\nFalse!");
	}
}