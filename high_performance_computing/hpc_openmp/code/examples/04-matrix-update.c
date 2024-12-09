#include <omp.h>
#include <stdio.h>

#define NY 4
#define NUM_ITERATIONS 10

void initialise_matrix(double matrix[][NY], int nx) {
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < NY; ++j) {
            matrix[i][j] = i + j * 0.5;
        }
    }
}
void iterate_matrix_solution(double current_matrix[][NY], double next_matrix[][NY], int thread_id, int nx) {
    for (int j = 0; j < NY; ++j) {
        if (thread_id > 0) {
            /* Each row depends on the current and previous rows */
            next_matrix[thread_id][j] = current_matrix[thread_id][j] + current_matrix[thread_id - 1][j];
        } else {
            /* First row is independent */
            next_matrix[thread_id][j] = current_matrix[thread_id][j] + 1;
        }
    }
}
void copy_matrix(double src[][NY], double dest[][NY], int nx) {
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < NY; ++j) {
            dest[i][j] = src[i][j];
        }
    }
}
void print_matrix(const char *label, double matrix[][NY], int nx) {
    printf("%s:\n", label);
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < NY; ++j) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
int main() {
    int nx = omp_get_max_threads();
    double current_matrix[nx][NY];
    double next_matrix[nx][NY];

    initialise_matrix(current_matrix, nx);
    print_matrix("Initial Matrix", current_matrix, nx);

    for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();

            iterate_matrix_solution(current_matrix, next_matrix, thread_id, nx);

            #pragma omp barrier /* Synchronise all threads before copying */
        }
        copy_matrix(next_matrix, current_matrix, nx);
    }
    print_matrix("Final Matrix", current_matrix, nx);
    return 0;
}