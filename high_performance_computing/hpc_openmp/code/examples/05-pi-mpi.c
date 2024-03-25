#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>

#define ROOT_RANK 0
#define PI 3.141592653589793238462643

int main(void)
{
    struct timespec begin;
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    int my_rank;
    int num_ranks;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    const long N = (long)1e10;
    const double h = 1.0 / N;
    double sum = 0.0;

    for (long i = my_rank; i <= N; i = i + num_ranks) {
        const double x = h * (double)i;
        sum += 4.0 / (1.0 + x * x);
    }
    const double rank_pi = h * sum;

    double reduced_pi;
    MPI_Reduce(&rank_pi, &reduced_pi, 1, MPI_DOUBLE, MPI_SUM, ROOT_RANK, MPI_COMM_WORLD);

    if (my_rank == ROOT_RANK) {
        struct timespec end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        printf("Calculated using %d MPI ranks\n", num_ranks);
        printf("Calculated pi %18.6f error %18.6f\n", reduced_pi, reduced_pi - PI);
        printf("Total time = %f seconds\n", (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec));
    }

    MPI_Finalize();

    return 0;
}
