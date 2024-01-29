#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int array[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16},
    };

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

    MPI_Datatype sub_array_t;
    MPI_Type_vector(2, 2, 4, MPI_INT, &sub_array_t);
    MPI_Type_commit(&sub_array_t);

    if (my_rank == 0) {
        MPI_Send(&array[1][1], 1, sub_array_t, 1, 0, MPI_COMM_WORLD);
    } else {
        int buffer[4];
        MPI_Status status;

        MPI_Recv(buffer, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        printf("Rank %d received the following:", my_rank);
        for (int i = 0; i < 4; ++i) {
            printf(" %d", buffer[i]);
        }
        printf("\n");
    }

    MPI_Type_free(&sub_array_t);

    return MPI_Finalize();
}
