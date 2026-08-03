---
name: Introduction to Supercomputing
id: supercomputing
dependsOn: []
files: [
  01_introduction.md,
  02_shared_memory.md,
  03_distributed_memory.md,
  04_accelerators.md,
  05_measuring_supercomputers.md
]
learningOutcomes:
  - Explain why workloads constrained by time to solution, problem size or throughput are often run on shared, centralised systems.
  - Describe how multi-core processors, shared-memory nodes and distributed-memory systems contribute to supercomputer performance.
  - Reason about performance limits arising from caches, memory contention, network communication and host-device data movement.
  - Explain how accelerators such as GPUs provide high computational throughput and assess which workloads can benefit from them.
  - Distinguish between theoretical peak, benchmark and application performance, and interpret machine rankings in that context.
summary: |
  An introduction to why supercomputers are needed and how their architecture
  provides performance, covering shared and distributed memory, accelerators,
  and the limits imposed by computation, memory and communication.
attribution:
  - citation: >
      This course was developed by Alasdair Wilson as part of the Oxford Research
      Software Engineering group (OxRSE).
    url: https://www.rse.ox.ac.uk/
    image: ./supercomputing/images/2024_oxrse_square.svg
    license: CC-BY-4.0
---

Supercomputers are assembled from familiar computing components, but their usefulness arises from how computation, memory and communication are combined at scale.
Concentrating those resources in shared, centralised systems makes demanding workloads practical while allowing expensive hardware to be kept well used.

Supercomputers obtain performance through massive parallelism at several levels:
CPU-cores work concurrently while sharing memory, nodes exchange data over an interconnect, and accelerators trade some flexibility for high throughput.
By tracing computation and data through this hierarchy, you will learn to recognise which workloads can exploit a system effectively and where performance is likely to be lost.
Hardware specifications and benchmarks provide useful evidence, but only when interpreted in that context.

The focus here is the architecture and performance of supercomputers.
Practical use of a cluster, including job schedulers and file transfer, is covered in [Introduction to High Performance Computing](high_performance_computing/hpc_intro).
The [Introduction to Parallelism](high_performance_computing/hpc_parallel_intro) provides a bridge from these architectural ideas to parallel programming, which is then developed in [Introduction to the Message Passing Interface](high_performance_computing/hpc_mpi) and [Parallel Programming with OpenMP](high_performance_computing/hpc_openmp).
