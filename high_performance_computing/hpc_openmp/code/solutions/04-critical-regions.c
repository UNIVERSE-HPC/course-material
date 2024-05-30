#include <omp.h>
#include <stdio.h>

int main() {
  int value = 0;

#pragma omp parallel num_threads(4)
  {
#pragma omp master
    {
      // Only the master thread executes this block of code
      value += 10;
      printf("Master thread, value: %d\n", value);
    }

#pragma omp single
    {
      // Only one thread (not necessarily the master) executes this block of
      // code
      value -= 5;
      printf("Single thread, value: %d\n", value);
    }

#pragma omp critical
    {
      // Each thread takes turns executing this block of code, one at a time
      value *= 2;
      printf("Critical thread, value: %d\n", value);
    }
  }

  return 0;
}