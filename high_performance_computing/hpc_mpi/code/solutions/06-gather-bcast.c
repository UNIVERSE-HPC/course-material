#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int my_rank;
    int num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    if (num_ranks < 2) {
        printf("This example needs at least 2 ranks\n");
        MPI_Finalize();
        return 1;
    }

    int sum = 0;
    int my_num = my_rank + 1;

    MPI_Status status;
    MPI_Request request;

    printf("Start : Rank %d: my_num = %d sum = %d\n", my_rank, my_num, sum);

    MPI_Ireduce(&my_num, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    MPI_Ibcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    printf("End : Rank %d: my_num = %d sum = %d\n", my_rank, my_num, sum);

    return MPI_Finalize();
}
