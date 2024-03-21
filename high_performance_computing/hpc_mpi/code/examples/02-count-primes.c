#include <stdbool.h>
#include <stdio.h>
#include <mpi.h>

#define NUM_ITERATIONS 100000

int main (int argc, char *argv[]) {
    int my_rank;
    int num_ranks;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    // calculate how many iterations each rank will deal with
    int iterations_per_rank = NUM_ITERATIONS / num_ranks;
    int rank_start = (my_rank * iterations_per_rank) + 1;
    int rank_end = (my_rank + 1) * iterations_per_rank;

    // catch cases where the work can't be split evenly
    if (rank_end > NUM_ITERATIONS || (my_rank == (num_ranks-1) && rank_end < NUM_ITERATIONS))
        rank_end = NUM_ITERATIONS;

    // each rank is dealing with a subset of the problem
    int prime_count = 0;
    for (int n = rank_start; n <= rank_end; ++n) {
        bool is_prime = true;

        // 0 and 1 are not prime numbers
        if (n == 0 || n == 1)
            is_prime = false;

        // if we can only divide n by i, then n is not prime
        for (int i = 2; i <= n / 2; ++i) {
            if (n % i == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
            prime_count++;
    }
    printf("Rank %d - count of primes between %d-%d: %d\n", my_rank, rank_start, rank_end, prime_count);

    MPI_Finalize();
}
