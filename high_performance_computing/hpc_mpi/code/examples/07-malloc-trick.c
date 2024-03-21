#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    const int num_rows = 3;
    const int num_cols = 3;

    /* To allocate a contiguous memory block, we can allocate a large enough
       chunk of memory and align the memory manually.
       - First, create an array of pointers. Each pointer will be a row.
       - Second, allocate rows*cols for the very first row.
       - Third, iterate over the remaining rows and use pointer arithmetic to
         point each row some offset into the allocation in the last step
    */
    int **matrix = malloc(num_rows * sizeof(int *));
    matrix[0] = malloc(num_rows * num_cols * sizeof(int));
    for (int i = 1; i < num_rows; ++i) {
        matrix[i] = matrix[i - 1] + num_cols;
    }

    /* Add some values to matrix and print those values */
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            matrix[i][j] = num_cols * i + j + 1;
            printf(" %d", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
