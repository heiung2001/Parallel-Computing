#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 2

static long num_steps = 1000000000;
double step;

int main()
{
    int i, nthreads;
    double pi, sum[NUM_THREADS], total;
    double start, end;

    step = 1.0/(double) num_steps;

    omp_set_num_threads(NUM_THREADS);
    start = omp_get_wtime();
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        int nthrds = omp_get_num_threads();
        double x;
        
        // check how many threads did the environment actually give.
        if (ID == 0) nthreads = nthrds;

        for (i = ID, sum[ID] = 0.0; i < num_steps; i = i + nthrds)
        {
            x = (i+0.5)*step;
            sum[ID] += 4.0/(1.0+x*x);
        }
    }
    end = omp_get_wtime() - start;

    for (i = 0; i < nthreads; i++)
    {
        total += sum[i];
    }
    pi = total*step;
    printf("pi is: %lf with %d threads in %lf sec", pi, nthreads, end);
    return 0;
}