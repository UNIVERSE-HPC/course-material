#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define PI 3.141592653589793238462643

int main(void)
{
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    double sum = 0.0;
    const long N = (long)1e10;
    const double h = 1.0 / N;

    for (long i = 0; i <= N; ++i) {
        const double x = h * (double)i;
        sum += 4.0 / (1.0 + x * x);
    }

    const double pi = h * sum;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    printf("Calculated pi %18.6f error %18.6f\n", pi, pi - PI);
    printf("Total time = %f seconds\n", (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec));

    return 0;
}
