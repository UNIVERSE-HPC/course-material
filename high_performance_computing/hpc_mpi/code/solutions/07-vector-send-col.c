#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int my_rank;
    int num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    int matrix[2][3] = {
        {1, 2, 3},
        {4, 5, 6},
    };

    if (num_ranks != 2) {
        if (my_rank == 0) {
            printf("This example only works with 2 ranks\n");
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Datatype col_t;
    MPI_Type_vector(2, 1, 3, MPI_INT, &col_t);
    MPI_Type_commit(&col_t);

    if (my_rank == 0) {
        MPI_Send(&matrix[0][1], 1, col_t, 1, 0, MPI_COMM_WORLD);
    } else {
        int buffer[2];
        MPI_Status status;

        MPI_Recv(buffer, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        printf("Rank %d received the following:", my_rank);
        for (int i = 0; i < 2; ++i) {
            printf(" %d", buffer[i]);
        }
        printf("\n");
    }

    MPI_Type_free(&col_t);

    return MPI_Finalize();
}
