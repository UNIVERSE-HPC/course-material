/* A serial code for Poisson equation
 * This will apply the diffusion equation to an initial state
 * until an equilibrium state is reached. */

/* contact seyong.kim81@gmail.com for comments and questions */
 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_ITERATIONS 25000
#define GRIDSIZE 12


/* Apply a single time step */
double poisson_step(
  float *u, float *unew, float *rho,
  float hsq, int points,
  int rank, int n_ranks
) {
  double unorm, global_unorm;
  float sendbuf, recvbuf;
  MPI_Status mpi_status;

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

  // Use Allreduce to calculate the sum over ranks
  MPI_Allreduce(&unorm, &global_unorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  // Overwrite u with the new field
  for (int i = 1;i <= points; i++) {
     u[i] = unew[i];
  }

  // The u field has been changed, communicate it to neighbours
  // With blocking communication, half the ranks should send first
  // and the other half should receive first
  if ((rank%2) == 1) {
    // Ranks with odd number send first

    // Send data down from rank to rank-1
    sendbuf = unew[1];
    MPI_Send(&sendbuf, 1, MPI_FLOAT, rank-1, 1, MPI_COMM_WORLD);
    // Receive dat from rank-1
    MPI_Recv(&recvbuf, 1, MPI_FLOAT, rank-1, 2, MPI_COMM_WORLD, &mpi_status);
    u[0] = recvbuf;

    if (rank != (n_ranks-1)) {
      // Send data up to rank+1 (if I'm not the last rank)
      MPI_Send(&u[points], 1, MPI_FLOAT, rank+1, 1, MPI_COMM_WORLD);
      // Receive data from rank+1
      MPI_Recv(&u[points+1], 1, MPI_FLOAT, rank+1, 2, MPI_COMM_WORLD, &mpi_status);
    }

  } else {
    // Ranks with even number receive first

    if (rank != 0) {
      // Receive data from rank-1 (if I'm not the first rank)
      MPI_Recv(&u[0], 1, MPI_FLOAT, rank-1, 1, MPI_COMM_WORLD, &mpi_status);
      // Send data down to rank-1
      MPI_Send(&u[1], 1, MPI_FLOAT, rank-1, 2, MPI_COMM_WORLD);
    }

    if (rank != (n_ranks-1)) {
      // Receive data from rank+1 (if I'm not the last rank)
      MPI_Recv(&u[points+1], 1, MPI_FLOAT, rank+1, 1, MPI_COMM_WORLD, &mpi_status);
      // Send data up to rank+1
      MPI_Send(&u[points], 1, MPI_FLOAT, rank+1, 2, MPI_COMM_WORLD);
    }
  }

  return global_unorm;
}


int main(int argc, char** argv) {

  // The heat energy in each block
  float *u, *unew, *rho;
  float h, hsq;
  double unorm, residual;
  int rank, n_ranks, rank_gridsize;
  float *resultbuf;
  int i;

  MPI_Init(&argc, &argv);

  // Find the number of x-slices calculated by each rank
  // The simple calculation here assumes that GRIDSIZE is divisible by n_ranks
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
  rank_gridsize = GRIDSIZE / n_ranks;

  u = malloc(sizeof(*u) * (rank_gridsize + 2));
  unew = malloc(sizeof(*unew) * (rank_gridsize + 2));
  rho = malloc(sizeof(*rho) * (rank_gridsize + 2));

  // Set up parameters
  h = 0.1;
  hsq = h*h;
  residual = 1e-5;

  // Initialise the u and rho field to 0
  for (i = 0; i <= rank_gridsize+1; i++) {
    u[i] = 0.0;
    rho[i] = 0.0;
  }

  // Create a start configuration with the heat energy
  // u=10 at the x=0 boundary for rank 0
  if (rank == 0)
    u[0] = 10.0;

  // Run iterations until the field reaches an equilibrium
  // and no longer changes
  for (i = 0; i < MAX_ITERATIONS; i++) {
    unorm = poisson_step( u, unew, rho, hsq, rank_gridsize, rank, n_ranks );
    if (sqrt(unorm) < sqrt(residual))
      break;
  }

  // Gather results from all ranks
  // We need to send data starting from the second element of u, since u[0] is a boundary  resultbuf = malloc(sizeof(*resultbuf) * GRIDSIZE);
  MPI_Gather(&u[1], rank_gridsize, MPI_FLOAT, resultbuf, rank_gridsize, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("Final result:\n");
    for (int j = 0; j < GRIDSIZE; j++) {
      printf("%d-", (int) resultbuf[j]);
    }
    printf("\nRun completed in %d iterations with residue %g\n", i, unorm, n_ranks);
  }

  MPI_Finalize();
}
