#include <omp.h>
#include <stdio.h>

#define NX 4
#define NY 4
#define NUM_ITERATIONS 10

void iterate_matrix_solution(double old_matrix[NX][NY], double new_matrix[NX][NY], int thread_id) {
    for (int j = 0; j < NY; ++j) {
        new_matrix[thread_id][j] = old_matrix[thread_id][j] + 1;
    }
}

void copy_matrix(double src[NX][NY], double dest[NX][NY]) {
    for (int i = 0; i < NX; ++i) {
        for (int j = 0; j < NY; ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

int main() {
    double old_matrix[NX][NY] = {{0}};
    double new_matrix[NX][NY] = {{0}};

    #pragma omp parallel
    {
        for (int i = 0; i < NUM_ITERATIONS; ++i) {
            int thread_id = omp_get_thread_num();

            iterate_matrix_solution(old_matrix, new_matrix, thread_id); /* Each thread updates a portion of the matrix */

            #pragma omp barrier /* Wait until all threads complete updates to new_matrix */

            copy_matrix(new_matrix, old_matrix);
        }
    }

    printf("Matrix update complete.\n");
    return 0;
}