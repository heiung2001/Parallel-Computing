#include <math.h>
#include <stdio.h>

#define GT (1.0 + sqrt(5.0))/2.0

void ProcessInitialization(int &n);

long double SerialResult(unsigned int n);

int main()
{
	int n;
	long double res;

	ProcessInitialization(n);

	res = SerialResult(n);
	printf("Fib(%d): %.0Lf.\n", n, res);

	return 0;
}

void ProcessInitialization(int &n)
{
	do {
		printf("Input a number: ");
		scanf("%d", &n);
		if (n < 3) {
			printf("Number must be larger than 2");
		}
	} while (n < 3);
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