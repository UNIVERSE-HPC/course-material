#include <math.h>
#include <omp.h>
#include <stdio.h>

#define NUM_ELEMENTS 1000

int main(int argc, char **argv) {
  int progress = 0;
  int array[NUM_ELEMENTS] = {0};
  int output_frequency = NUM_ELEMENTS / 10; // output every 10%

#pragma omp parallel for schedule(static)
  for (int i = 0; i < NUM_ELEMENTS; ++i) {
    int thread_id = omp_get_thread_num();

    array[i] = log(i) * cos(3.142 * i);

#pragma omp critical
    {
      progress++;
      if (progress % output_frequency == 0) {
        printf("Thread %d: overall progress %3.0f%%\n", thread_id,
               (double)progress / NUM_ELEMENTS * 100.0);
      }
    }
  }

  return 0;
}