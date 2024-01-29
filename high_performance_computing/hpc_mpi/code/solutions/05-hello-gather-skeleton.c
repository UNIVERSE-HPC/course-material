#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CHARS 32

int main(int argc, char **argv) {
    int my_rank, num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    char message[NUM_CHARS];
    snprintf(message, NUM_CHARS, "Hello from rank %d", my_rank);

    return MPI_Finalize();
}
