#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4
#define M 4
#define NUM_ELEMENTS (N * M)
#define ROOT_RANK 0

int multiply_matrix(double *local_a, double *matrix_b, double *local_result, int a_rows, int a_cols, int b_cols,
                    int rows_per_rank, int my_rank)
{
    for (int i = 0; i < rows_per_rank; ++i) {
        for (int j = 0; j < b_cols; ++j) {
            for (int k = 0; k < a_cols; ++k) {
                local_result[i * b_cols + j] += local_a[i * a_cols + k] * matrix_b[k * b_cols + j];
            }
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    int my_rank;
    int num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
    srand(time(NULL));

    double *matrix_a;
    double *matrix_b;
    double *matrix_result;

    const int a_rows = 10;
    const int a_cols = 7;
    const int b_rows = 7;
    const int b_cols = 12;

    if (a_cols != b_rows) {
        printf("Invalid dimensions for matrix a and b\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    matrix_b = malloc(b_rows * b_cols * sizeof(double));

    if (my_rank == ROOT_RANK) {
        matrix_a = malloc(a_rows * a_cols * sizeof(double));
        matrix_result = malloc(a_rows * b_cols * sizeof(double));
        for (int i = 0; i < a_rows * a_cols; ++i) {
            matrix_a[i] = (double)rand() / RAND_MAX;
        }
        for (int i = 0; i < b_rows * b_cols; ++i) {
            matrix_b[i] = (double)rand() / RAND_MAX;
        }
    }

    const int rows_per_rank = a_rows / num_ranks;
    double *local_a = malloc(rows_per_rank * a_cols * sizeof(double));
    double *local_result = malloc(rows_per_rank * b_cols * sizeof(double));

    MPI_Bcast(matrix_b, b_rows * b_cols, MPI_DOUBLE, ROOT_RANK, MPI_COMM_WORLD);
    MPI_Scatter(matrix_a, rows_per_rank * a_cols, MPI_DOUBLE, local_a, rows_per_rank * a_cols, MPI_DOUBLE, ROOT_RANK,
                MPI_COMM_WORLD);

    multiply_matrix(local_a, matrix_b, local_result, a_rows, a_cols, b_cols, rows_per_rank, my_rank);

    MPI_Gather(local_result, rows_per_rank * b_cols, MPI_DOUBLE, matrix_result, rows_per_rank * b_cols, MPI_DOUBLE,
               ROOT_RANK, MPI_COMM_WORLD);

    if (my_rank == ROOT_RANK) {
        for (int i = 0; i < a_rows; ++i) {
            for (int j = 0; j < b_cols; ++j) {
                printf("%g ", matrix_result[i * b_cols + j]);
            }
            printf("\n");
        }
    }

    return MPI_Finalize();
}