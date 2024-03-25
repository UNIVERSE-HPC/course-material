#include <math.h>
#include <omp.h>
#include <stdio.h>

#define ARRAY_SIZE 524288

int main(int argc, char **argv) {
  float sum = 0;
  float array[ARRAY_SIZE];

  omp_set_num_threads(4);

#pragma omp parallel for schedule(static)
  for (int i = 0; i < ARRAY_SIZE; ++i) {
    array[i] = cos(M_PI * i);
  }

#pragma omp parallel for schedule(static)
  for (int i = 0; i < ARRAY_SIZE; i++) {
#pragma omp atomic
    sum += array[i];
  }

  printf("Sum: %f\n", sum);

  return 0;
}