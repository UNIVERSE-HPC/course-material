#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#define PI 3.141592653589793238462643

int main(void)
{
    struct timespec begin;
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    /* Initialise parameters. N is the number of rectangles we will sum over,
       and h is the width of each rectangle (1 / N) */
    double sum = 0.0;
    const long N = (long)1e10;
    const double h = 1.0 / N;

/* Parallelise the loop using a parallel for directive. We will set the sum
   variable to be a reduction variable. As it is marked explicitly as a reduction
   variable, we don't need to worry about any race conditions corrupting the
   final value of sum */
#pragma omp parallel for shared(N, h), reduction(+:sum)
    for (long i = 0; i <= N; ++i) {
        const double x = h * (double)i;
        sum += 4.0 / (1.0 + x * x);
    }

    /* To attain our final value of pi, we multiply by h as we did not include
       this in the loop */
    const double pi = h * sum;

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("Calculated using %d OMP threads\n", omp_get_max_threads());
    printf("Calculated pi %18.6f error %18.6f\n", pi, pi - PI);
    printf("Total time = %f seconds\n", (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec));

    return 0;
}
