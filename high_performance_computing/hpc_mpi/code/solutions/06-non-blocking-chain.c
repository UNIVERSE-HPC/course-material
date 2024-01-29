#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>

#define MESSAGE_SIZE 32

int main(int argc, char **argv)
{
    int my_rank;
    int num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    if (num_ranks < 2) {
        printf("This example requires at least two ranks\n");
        return MPI_Finalize();
    }

    char send_message[MESSAGE_SIZE];
    char recv_message[MESSAGE_SIZE];

    int right_rank = (my_rank + 1) % num_ranks;
    int left_rank = my_rank < 1 ? num_ranks - 1 : my_rank - 1;

    MPI_Status send_status, recv_status;
    MPI_Request send_request, recv_request;

    MPI_Irecv(recv_message, MESSAGE_SIZE, MPI_CHAR, left_rank, 0, MPI_COMM_WORLD, &recv_request);

    sprintf(send_message, "Hello from rank %d!", my_rank);
    MPI_Isend(send_message, MESSAGE_SIZE, MPI_CHAR, right_rank, 0, MPI_COMM_WORLD, &send_request);
    MPI_Wait(&send_request, &send_status);

    MPI_Wait(&recv_request, &recv_status);

    /* But maybe you prefer MPI_Test()

    int recv_flag = false;
    while (recv_flag == false) {
        MPI_Test(&recv_request, &recv_flag, &recv_status);
    }

    */

    printf("Rank %d: message received -- %s\n", my_rank, recv_message);

    return MPI_Finalize();
}
