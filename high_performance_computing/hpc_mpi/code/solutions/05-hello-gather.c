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

    char *recv_buffer = malloc(NUM_CHARS * num_ranks * sizeof(char));
    MPI_Gather(message, NUM_CHARS, MPI_CHAR, recv_buffer, NUM_CHARS, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        for (int i = 0; i < num_ranks; ++i) { /* snprintf null terminates strings */
            printf("%s\n", &recv_buffer[i * NUM_CHARS]);
        }
    }

    free(recv_buffer);

    return MPI_Finalize();
}