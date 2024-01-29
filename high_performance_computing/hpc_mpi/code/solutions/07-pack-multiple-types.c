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
        int int_data_count = 5, float_data_count = 10;
        int *int_data = malloc(int_data_count * sizeof(int));
        float *float_data = malloc(float_data_count * sizeof(float));
        for (int i = 0; i < int_data_count; ++i) {
            int_data[i] = i + 1;
        }
        for (int i = 0; i < float_data_count; ++i) {
            float_data[i] = 3.14159 * (i + 1);
        }

        /* use MPI_Pack_size to determine how big the packed buffer needs to be */
        int buffer_size_count, buffer_size_int, buffer_size_float;
        MPI_Pack_size(2, MPI_INT, MPI_COMM_WORLD, &buffer_size_count); /* 2 * INT because we will have 2 counts*/
        MPI_Pack_size(int_data_count, MPI_INT, MPI_COMM_WORLD, &buffer_size_int);
        MPI_Pack_size(float_data_count, MPI_FLOAT, MPI_COMM_WORLD, &buffer_size_float);
        int total_buffer_size = buffer_size_int + buffer_size_float + buffer_size_count;

        int position = 0;
        char *buffer = malloc(total_buffer_size);

        /* Pack the data size, followed by the actually data */
        MPI_Pack(&int_data_count, 1, MPI_INT, buffer, total_buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(int_data, int_data_count, MPI_INT, buffer, total_buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&float_data_count, 1, MPI_INT, buffer, total_buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(float_data, float_data_count, MPI_FLOAT, buffer, total_buffer_size, &position, MPI_COMM_WORLD);

        /* buffer is sent in one communication using MPI_PACKED */
        MPI_Send(buffer, total_buffer_size, MPI_PACKED, 1, 0, MPI_COMM_WORLD);

        free(buffer);
        free(int_data);
        free(float_data);
    } else {
        int buffer_size;
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_PACKED, &buffer_size);

        char *buffer = malloc(buffer_size);
        MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, &status);

        int position = 0;
        int int_data_count, float_data_count;

        MPI_Unpack(buffer, buffer_size, &position, &int_data_count, 1, MPI_INT, MPI_COMM_WORLD);
        int *int_data = malloc(int_data_count * sizeof(int));
        MPI_Unpack(buffer, buffer_size, &position, int_data, int_data_count, MPI_INT, MPI_COMM_WORLD);

        MPI_Unpack(buffer, buffer_size, &position, &float_data_count, 1, MPI_INT, MPI_COMM_WORLD);
        float *float_data = malloc(float_data_count * sizeof(float));
        MPI_Unpack(buffer, buffer_size, &position, float_data, float_data_count, MPI_FLOAT, MPI_COMM_WORLD);

        printf("int data: [");
        for (int i = 0; i < int_data_count; ++i) {
            printf(" %d", int_data[i]);
        }
        printf(" ]\n");

        printf("float data: [");
        for (int i = 0; i < float_data_count; ++i) {
            printf(" %f", float_data[i]);
        }
        printf(" ]\n");

        free(int_data);
        free(float_data);
        free(buffer);
    }

    return MPI_Finalize();
}