#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT_RANK 0

int main(int argc, char **argv)
{
    int my_rank, num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    srand(time(NULL) + my_rank);

    int total_num_points = (int)1e8;
    int points_per_rank = total_num_points / num_ranks;
    int rank_points_in_circle = 0;

    for (int i = 0; i < points_per_rank; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            rank_points_in_circle++;
        }
    }

    int total_points_in_circle;
    MPI_Reduce(&rank_points_in_circle, &total_points_in_circle, 1, MPI_INT, MPI_SUM, ROOT_RANK, MPI_COMM_WORLD);

    if (my_rank == ROOT_RANK) {
        double pi = 4.0 * total_points_in_circle / total_num_points;
        printf("Estimated value of π = %f\n", pi);
    }

    return MPI_Finalize();
}
