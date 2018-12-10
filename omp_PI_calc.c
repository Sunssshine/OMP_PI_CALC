// COMPILE: GCC omp_PI_calc.c -o omp_PI_calc -fopenmp -lm

#include <stdio.h>
#include <math.h>
#include <omp.h>

#define MAX_THREADS_NUM 20

double omp_reduction_PI_calc(int calcAccuracy)
{
    int j = 0;
    double result = 0;

    #pragma omp parallel for reduction(+:result)
    for (j = 0; j < calcAccuracy; j++) {
        result += 4 * ((powl(-1, j)) / (2 * j + 1));
    }

    return result;
}

double omp_critical_PI_calc(int calcAccuracy)
{
    int j = 0;
    double result = 0;
    #pragma omp parallel
    {
        double localResult = 0;

        #pragma omp for
        for (j = 0; j < calcAccuracy; j++) {
            localResult += 4 * ((powl(-1, j)) / (2 * j + 1));
        }

        #pragma omp critical
        result += localResult;
    }
    return result;
}

int main()
{
    int calcAccuracy = 10000000;
    int i = 0;
    double result = 0;

    for(i = 1; i<MAX_THREADS_NUM; i++)
    {
        omp_set_num_threads(i);
        double start = omp_get_wtime();
        result = omp_critical_PI_calc(calcAccuracy);
        double delta = omp_get_wtime() - start;
        printf("|THREADS %d| |CRITICAL| PI = %.13g computed in %.4g seconds\n", i, result, delta);
    }

    printf("\n------------------------------------------------------------------------------\n\n");

    for(i = 1; i<MAX_THREADS_NUM; i++)
    {
        omp_set_num_threads(i);
        double start = omp_get_wtime();
        result = omp_reduction_PI_calc(calcAccuracy);
        double delta = omp_get_wtime() - start;
        printf("|THREADS %d| |REDUCTION| PI = %.13g computed in %.4g seconds\n", i, result, delta);
    }
    return 0;
}
