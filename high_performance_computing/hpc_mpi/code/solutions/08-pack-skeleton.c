#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int my_rank;
    int num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    if (num_ranks != 2) {
        if (my_rank == 0) {
            printf("This example only works with 2 ranks\n");
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (my_rank == 0) {
        /* Allocate and initialize an int and float array */
        int int_data_count = 5, float_data_count = 10;
        int *int_data = malloc(int_data_count * sizeof(int));
        float *float_data = malloc(float_data_count * sizeof(float));
        for (int i = 0; i < int_data_count; ++i) {
            int_data[i] = i + 1;
        }
        for (int i = 0; i < float_data_count; ++i) {
            float_data[i] = 3.14159 * (i + 1);
        }

        /*
         * Your code here to pack the data
         */

    } else {

        /*
         * Your code here to unpack the data
         */
    }

    return MPI_Finalize();
}
