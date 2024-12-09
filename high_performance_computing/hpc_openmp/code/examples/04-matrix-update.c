#include <omp.h>
#include <stdio.h>

#define NY 4 // Number of columns
#define NUM_ITERATIONS 10 // Number of iterations

void initialise_matrix(double matrix[][NY], int NX) {
    for (int i = 0; i < NX; ++i) {
        for (int j = 0; j < NY; ++j) {
            matrix[i][j] = i + j * 0.5;
        }
    }
}

void iterate_matrix_solution(double current_matrix[][NY], double next_matrix[][NY], int thread_id, int NX) {
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

void copy_matrix(double src[][NY], double dest[][NY], int NX) {
    for (int i = 0; i < NX; ++i) {
        for (int j = 0; j < NY; ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

void print_matrix(const char *label, double matrix[][NY], int NX) {
    printf("%s:\n", label);
    for (int i = 0; i < NX; ++i) {
        for (int j = 0; j < NY; ++j) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int NX;

    /* Dynamically determine NX based on the number of threads */
    #pragma omp parallel
    {
        NX = omp_get_num_threads();
    }

    double current_matrix[NX][NY];
    double next_matrix[NX][NY];

    /* Initialise the matrix */
    initialise_matrix(current_matrix, NX);

    /* Print the initial matrix */
    print_matrix("Initial Matrix", current_matrix, NX);


    for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();

            /* Update the next matrix based on the current matrix */
            iterate_matrix_solution(current_matrix, next_matrix, thread_id, NX);

            #pragma omp barrier /* Synchronise all threads before copying */
        }

        /* Copy the next matrix back into the current matrix */
        copy_matrix(next_matrix, current_matrix, NX);
    }

    print_matrix("Final Matrix", current_matrix, NX);

    return 0;
}
