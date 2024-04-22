#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define NUM_CHARS 32

int main(int argc, char **argv) {
    int my_rank, num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    char message[NUM_CHARS];

    if (my_rank == 0) {
        strcpy(message, "Hello from rank 0");
    }

    MPI_Bcast(message, NUM_CHARS, MPI_CHAR, 0, MPI_COMM_WORLD);

    printf("I'm rank %d and I got the message '%s'\n", my_rank, message);

    return MPI_Finalize();
}