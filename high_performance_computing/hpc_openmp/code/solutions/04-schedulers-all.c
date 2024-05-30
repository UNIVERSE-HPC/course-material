#include <math.h>
#include <omp.h>
#include <stdio.h>

#define N 729
#define NUM_THREADS 2
#define NUM_ITERATIONS 100

double a[N][N], b[N][N];

void init_loop(void) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      a[i][j] = 0.0;
      b[i][j] = 3.142 * (i + j);
    }
  }
}

void unbalanced_loop(void) {
  for (int i = 0; i < N; i++) {
    for (int j = N - 1; j > i; j--) {
      a[i][j] += cos(b[i][j]);
    }
  }
}

/* -----------------------------------------------------------------------------
 * Modify the code below this
 */

int main(int argc, char **argv) {
  int i;
  int sum = 0;

  init_loop();
  omp_set_num_threads(NUM_THREADS);

  double start;
  double end;

  start = omp_get_wtime();

#pragma omp parallel for schedule(static)
  for (i = 0; i < NUM_ELEMENTS; i++) {
    unbalanced_loop();
  }

  end = omp_get_wtime();

  printf("Static: Total time per rep = %f\n", (end - start) / NUM_ELEMENTS);

  start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < NUM_ELEMENTS; i++) {
    unbalanced_loop();
  }

  end = omp_get_wtime();

  printf("Dynamic: Total time per rep = %f\n", (end - start) / NUM_ELEMENTS);

  start = omp_get_wtime();

#pragma omp parallel for schedule(guided)
  for (i = 0; i < NUM_ELEMENTS; i++) {
    unbalanced_loop();
  }

  end = omp_get_wtime();

  printf("Guided: Total time per rep = %f\n", (end - start) / NUM_ELEMENTS);

  return 0;
}
