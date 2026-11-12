---
name: HPC Scalability Profiling
id: hpc_scalability_profiling
dependsOn: [high_performance_computing.hpc_mpi]
files: [01_understanding_scalability.md, 02_scalability_profiling.md]
summary: |
  This session introduces the concept of scalability and how to profile the scalability of your code with an increasing number of cores.
---

This course looks at how to measure and reason about the performance of parallel code as you scale it to run on more cores.
It builds on the MPI reduction example from [Common Communication Patterns](high_performance_computing/hpc_mpi/10_communication_patterns), using timing data to work through strong scaling, weak scaling, and Amdahl's Law.

:::callout{variant="note"}
You will need access to an HPC cluster to run the exercises yourself.
:::
