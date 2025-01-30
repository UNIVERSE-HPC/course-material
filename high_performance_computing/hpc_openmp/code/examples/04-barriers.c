#include <omp.h>
#include <stdio.h>

#define TABLE_SIZE 8

/* Function to initialise the lookup table */
void initialise_lookup_table(int thread_id, double lookup_table[TABLE_SIZE]) {
    int num_threads = omp_get_num_threads();
    for (int i = thread_id; i < TABLE_SIZE; i += num_threads) {
        lookup_table[i] = thread_id * 2; /* Each thread initialises its own portion */
        printf("Thread %d initializing lookup_table[%d] = %f\n", thread_id, i, lookup_table[i]);
    }
}

/* Function to perform the main calculation */
void do_main_calculation(int thread_id, double lookup_table[TABLE_SIZE]) {
    int num_threads = omp_get_num_threads();
    for (int i = thread_id; i < TABLE_SIZE; i += num_threads) {
        lookup_table[i] = lookup_table[i] * 5.0;
        printf("Thread %d processing lookup_table[%d] = %f\n", thread_id, i, lookup_table[i]);
    }
}

int main() {
    double lookup_table[TABLE_SIZE] = {0}; /* Initialise the lookup table to zeros */

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        /* The initialisation of the lookup table is done in parallel */
        initialise_lookup_table(thread_id, lookup_table);

        #pragma omp barrier  /* As all threads depend on the table, we have to wait until all threads
                            are done and have reached the barrier */


        /* Each thread then proceeds to its main calculation */
        do_main_calculation(thread_id, lookup_table);
    }

    return 0;
}
