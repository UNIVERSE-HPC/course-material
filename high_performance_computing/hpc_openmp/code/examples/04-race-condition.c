#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_TIMES 10000

int main(void) {
    int value = 0;

#pragma omp parallel for
    for (int i = 0; i < NUM_TIMES; ++i) {
        value += 1;
    }

    printf("The final value is: %d\n", value);

    return EXIT_SUCCESS;
}
