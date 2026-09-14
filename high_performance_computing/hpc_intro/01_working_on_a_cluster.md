---
name: Working on an HPC Cluster
dependsOn: []
tags: [foundation, slurm]
learningOutcomes:
  - Describe the roles of login nodes, compute nodes, shared storage and the job scheduler in an HPC service.
  - Trace a computational job from preparation and submission to execution and retrieval of its results.
  - Explain why substantial computation must be run through the scheduler rather than directly on a login node.
  - Identify the local information needed before beginning work on an HPC cluster.
attribution:
  - citation: >
      This course was developed by Alasdair Wilson as part of the Oxford Research Software Engineering group (OxRSE).
    url: https://www.rse.ox.ac.uk
    image: ./hpc_intro/fig/2024_oxrse_square.svg
  - citation: >
      A previous version of this material was adapted from "Introduction to High-Performance Computing" by The Carpentries.
    url: https://carpentries-incubator.github.io/hpc-intro/
    image: /carpentries-incubator.svg
    license: CC-BY-4.0
---

An HPC cluster is not a single remote computer.
It is a collection of systems with different roles, operated as a shared service.
Using that service effectively means directing each part of your work to the appropriate place.

HPC services differ in their details, but most clusters expose the same broad structure: login nodes provide access, a scheduler controls the compute resources, compute nodes run applications, and shared storage makes files available across the system.

:::callout{variant="note"}

## From Architecture to Operation

[Supercomputing: Architecture and Performance](high_performance_computing/supercomputing) explains why supercomputers combine shared-memory nodes, distributed memory and accelerators.
This course starts from the point at which such a system is available as a service and concentrates on using it effectively.

We will show how to request resources for existing serial and parallel applications.
Designing parallel software is covered separately in [Introduction to Parallelism](high_performance_computing/hpc_parallel_intro), [Introduction to the Message Passing Interface](high_performance_computing/hpc_mpi) and [Parallel Programming with OpenMP](high_performance_computing/hpc_openmp).

:::

## Login Nodes

When you connect to a cluster, you normally arrive on a *login node*.
Login nodes provide an interactive environment in which you can organise files, prepare job scripts, inspect results and submit work to the scheduler.
They are also shared by many other users at the same time.

A login node is therefore not a substitute for a more powerful workstation.
Running a substantial calculation there bypasses the scheduler and allows one process to compete directly with other users' interactive sessions.
Work that requires appreciable processing time, memory or accelerator resources should run on a compute node instead.

Exactly what counts as acceptable login-node use is a matter of local policy.
For example, some sites permit short compilations and small test runs, while others provide dedicated development or data-transfer nodes for those tasks.

:::callout{variant="warning"}

## Login-Node Policy Still Applies When the System Looks Idle

Apparent spare capacity on a login node does not override local policy.
Its load can change immediately, and limits enforced by administrators may not be visible to an individual user.
Request an [interactive compute job](high_performance_computing/hpc_intro/04_scheduler#interactive-jobs) when the work exceeds what the service permits on its login nodes.

:::

## Compute Nodes

*Compute nodes* provide the resources used for computational jobs.
A cluster may contain several types of compute node, differing in processor count, memory capacity, accelerators or other hardware.
Users do not normally choose an individual machine or connect to one directly.
Instead, they describe the resources their application needs and allow the scheduler to select suitable nodes.

Requesting resources only makes them available to a job, it does not cause an application to use them.
A serial program will not become parallel because it was assigned additional CPUs, and a program must be designed to use a GPU before requesting one is worthwhile.

[Running Jobs with Slurm](high_performance_computing/hpc_intro/04_scheduler) will show how common types of application map onto scheduler requests.

## The Scheduler

The *scheduler* controls access to the compute nodes.
It records each job's requirements, identifies resources on which the job can run, and starts the job when a suitable allocation becomes available.
This prevents users from manually competing for machines and allows the system operator to apply sharing and accounting policies consistently.

This course uses Slurm, a scheduler found on many HPC systems.
[Slurm's developers report that it powers a majority of the TOP500](https://slurm.schedmd.com/faq.html), making it about as close to a common language as HPC schedulers get.
It is not universal even at the top end: [El Capitan uses Flux](https://hpc.llnl.gov/documentation/user-guides/using-el-capitan-systems/running-jobs-flux-and-mpi), while [Aurora uses PBS](https://docs.alcf.anl.gov/aurora/running-jobs-aurora/).
Other schedulers use different commands, but retain the same central ideas: users request an allocation, work may wait until that allocation is available, and commands are then launched within it.

:::callout{variant="note"}

## Local Requirements

Some systems require a project account, partition or quality-of-service setting when a job is submitted.
These values identify where work should be charged or which resources it may use; they are not portable between clusters.
Use the values given in the local documentation rather than copying them from an example written for another system.

:::

## Shared Storage

Login and compute nodes usually access one or more shared filesystems.
A job prepared on a login node is therefore still able to read its inputs and write results when it runs elsewhere.
The service may also provide storage intended for particular purposes, such as backed-up home directories, project data, high-performance working data or temporary files.

Names such as *home*, *work* and *scratch* are common, but they do not guarantee particular performance, backup or retention policies.
[Storage and Data Transfer](high_performance_computing/hpc_intro/03_cluster) will examine how to choose between the filesystems available on a particular system.

## The Lifecycle of a Job

A typical piece of work passes through the cluster as follows:

1. [Connect to a login node](high_performance_computing/hpc_intro/02_connecting).
1. Place the application's software and input data in [storage accessible to the compute nodes](high_performance_computing/hpc_intro/03_cluster).
1. Select the required [software environment](high_performance_computing/hpc_intro/05_modules).
1. [Submit a job script](high_performance_computing/hpc_intro/04_scheduler) that specifies both the commands to run and the resources they require.
1. [Monitor the job](high_performance_computing/hpc_intro/04_scheduler) while the scheduler holds it pending and after an allocation becomes available.
1. Run the application on the allocated compute resources, writing output and errors to files.
1. [Inspect the completed job](high_performance_computing/hpc_intro/08_resources), [retrieve its results](high_performance_computing/hpc_intro/06_transferring_files) and refine future resource requests using what the run revealed.

Interactive work on a compute node follows the same principle.
Rather than bypassing the scheduler, you request an interactive allocation and work within the resources assigned to it.

::::challenge{id=cluster-information title="Find the Information for Your Cluster"}

Locate the user documentation for the cluster you will use.
Before connecting, determine:

1. The login address and authentication requirements.
1. Whether access also requires a VPN, gateway or multi-factor authentication.
1. Which scheduler the cluster uses.
1. Which settings, if any, identify your project or the type of resource you want to use.
1. Where its login-node and storage policies are documented.
1. How to contact its support team.

:::solution

The answers are deliberately system-specific.
A useful result is a set of links or notes taken from the cluster's own documentation, not values copied from this course or another system.

If any requirement remains unclear, resolve it with the support team before submitting work.
This is particularly important for storage retention and activities permitted on login nodes.

:::
::::

The next section, [Connecting to a Cluster](high_performance_computing/hpc_intro/02_connecting), begins this workflow by establishing a secure remote connection.
