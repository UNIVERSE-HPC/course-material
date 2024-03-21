#include <mpi.h>
#include <stdio.h>

struct Node {
    int id;
    char name[16];
    double temperature;
};

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

    struct Node node = {.id = 0, .name = "Dale Cooper", .temperature = 42};

    int block_lengths[3] = {1, 16, 1};
    MPI_Datatype block_types[3] = {MPI_INT, MPI_CHAR, MPI_DOUBLE};

    MPI_Aint base_address;
    MPI_Aint block_offsets[3];
    MPI_Get_address(&node, &base_address);
    MPI_Get_address(&node.id, &block_offsets[0]);
    MPI_Get_address(&node.name, &block_offsets[1]);
    MPI_Get_address(&node.temperature, &block_offsets[2]);
    for (int i = 0; i < 3; ++i) {
        block_offsets[i] = MPI_Aint_diff(block_offsets[i], base_address);
    }

    MPI_Datatype node_struct;
    MPI_Type_create_struct(3, block_lengths, block_offsets, block_types, &node_struct);
    MPI_Type_commit(&node_struct);

    if (my_rank == 0) {
        MPI_Send(&node, 1, node_struct, 1, 0, MPI_COMM_WORLD);
    } else {
        struct Node recv_node;
        MPI_Recv(&recv_node, 1, node_struct, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Received node: id = %d name = %s temperature %f\n", recv_node.id, recv_node.name,
               recv_node.temperature);
    }

    MPI_Type_free(&node_struct);

    return MPI_Finalize();
}
