#include <omp.h>
#include <stdio.h>

#define TABLE_SIZE 8


void initialise_lookup_table(int thread_id, double lookup_table[TABLE_SIZE]) {
    int num_threads = omp_get_num_threads();
    for (int i = thread_id; i < TABLE_SIZE; i += num_threads) {
        lookup_table[i] = thread_id * 2;
        printf("Thread %d initializing lookup_table[%d] = %f\n", thread_id, i, lookup_table[i]);
    }
}


void write_table_to_disk(double lookup_table[TABLE_SIZE]) {
    printf("Writing lookup table to disk:\n");
    for (int i = 0; i < TABLE_SIZE; ++i) {
        printf("lookup_table[%d] = %f\n", i, lookup_table[i]);
    }
}


void do_main_calculation(int thread_id) {
    printf("Thread %d performing its main calculation.\n", thread_id);
}

int main() {
    double lookup_table[TABLE_SIZE] = {0};

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();


        initialise_lookup_table(thread_id, lookup_table);

        #pragma omp barrier

        #pragma omp single
        {
            write_table_to_disk(lookup_table);
        }

        do_main_calculation(thread_id);
    }

    return 0;
}