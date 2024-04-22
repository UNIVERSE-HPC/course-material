/* A serial code for Poisson equation
 * This will apply the diffusion equation to an initial state
 * untill an equilibrium state is reached. */

/* contact seyong.kim81@gmail.com for comments and questions */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_ITERATIONS 25000
#define GRIDSIZE 12


/* Apply a single time step */
double poisson_step(
  float *u, float *unew, float *rho,
  float hsq, int points
) {
  double unorm;

  // Calculate one timestep
  for (int i = 1; i <= points; i++) {
     float difference = u[i-1] + u[i+1];
     unew[i] = 0.5 * (difference - hsq*rho[i]);
  }

  // Find the difference compared to the previous time step
  unorm = 0.0;
  for (int i = 1;i <= points; i++) {
     float diff = unew[i]-u[i];
     unorm += diff*diff;
  }

  // Overwrite u with the new field
  for (int i = 1;i <= points; i++) {
     u[i] = unew[i];
  }

  return unorm;
}


int main(int argc, char** argv) {

  // The heat energy in each block
  float *u, *unew, *rho;
  float h, hsq;
  double unorm, residual;
  int i;

  u = malloc(sizeof(*u) * (GRIDSIZE+2));
  unew = malloc(sizeof(*unew) * (GRIDSIZE+2));
  rho = malloc(sizeof(*rho) * (GRIDSIZE+2));

  // Set up parameters
  h = 0.1;
  hsq = h*h;
  residual = 1e-5;

  // Initialise the u and rho field to 0
  for (i = 0; i <= GRIDSIZE+1; i++) {
    u[i] = 0.0;
    rho[i] = 0.0;
  }

  // Create a start configuration with the heat energy
  // u=10 at the x=0 boundary for rank 1
  u[0] = 10.0;

  // Run iterations until the field reaches an equilibrium
  // and no longer changes
  for (i = 0; i < MAX_ITERATIONS; i++) {
    unorm = poisson_step(u, unew, rho, hsq, GRIDSIZE);
    if (sqrt(unorm) < sqrt(residual))
      break;
  }

  printf("Final result:\n");
  for (int j = 1; j <= GRIDSIZE; j++) {
    printf("%d-", (int) u[j]);
  }
  printf("\nRun completed in %d iterations with residue %g\n", i, unorm);
}
