#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <windows.h>

double LiToDouble(LARGE_INTEGER x);
double GetTime();

double rnd(unsigned int *seed);

void get_num_shots(int &n);
int sequence_shooting(int n);
int parallel_shooting(int n);

int main()
{
    int num_shots;
    int num_hits = 0;
    double pi = 0.0;
    double start, end;

    get_num_shots(num_shots);

    start = GetTime();
    num_hits = sequence_shooting(num_shots);
    end = GetTime();
    printf("\nSequence take %lf sec.\n", end - start);

    start = GetTime();
    num_hits = parallel_shooting(num_shots);
    end = GetTime();
    printf("\nParallel takes %lf sec.\n", end - start);

    pi = 4.0 * (double)num_hits/num_shots;
    printf("\npi = %lf\n", pi);

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

double rnd(unsigned int *seed)
{
    *seed = (1140671485 * (*seed) + 12820163) % (1 << 24);
    return ((double)(*seed)) / (1 << 24);
}

void get_num_shots(int &n)
{
    do
    {
        printf("Number of shots: ");
        scanf("%d", &n);
        if (n <= 0) {
            printf("Number of shots must be larger than 0!");
        }
    } while (n <= 0);
}
int sequence_shooting(int n)
{
    // TODO: I'm depressed!
    return 0;
}
int parallel_shooting(int n)
{
    int hit = 0;
    omp_set_num_threads(omp_get_num_procs());
    #pragma omp parallel
    {
        unsigned int seed = omp_get_thread_num();
        #pragma omp for reduction(+:hit)
            for (int shot = 0; shot < n; shot++)
            {
                double x = rnd(&seed);
                double y = rnd(&seed);
                if (x*x + y*y <= 1) { hit += 1; }
            }
    }
    return hit;
}
