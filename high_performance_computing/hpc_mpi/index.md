---
name: Introduction to the Message Passing Interface
id: hpc_mpi
dependsOn: [
    high_performance_computing.hpc_parallel_intro
]
files: [
    02_mpi_api.md,
    03_communicating_data.md,
    04_point_to_point_communication.md,
    05_collective_communication.md,
    06_non_blocking_communication.md,
    07-derived-data-types.md,
    08_porting_serial_to_mpi.md,
    09_optimising_mpi.md,
    10_communication_patterns.md,
    11_advanced_communication.md,
]
summary: |
    This session introduces the Message Passing Interface (MPI), teaching trainees how to use the MPI API for compiling and running applications across multiple processes. It covers point-to-point and collective communication, non-blocking functions, and handling non-contiguous memory with derived data types. Attendees will practice converting serial code to parallel, understanding scaling performance, and using profilers to optimise MPI applications.
---
