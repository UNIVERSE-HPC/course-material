#!/bin/bash

#SBATCH --time=00:01:00
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --partition=

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun ./xthi