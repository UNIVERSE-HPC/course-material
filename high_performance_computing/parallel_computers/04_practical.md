---
name: Shared vs Distributed Hello World
dependsOn: [
    high_performance_computing.parallel_computers.03_comparison
]
tags: [foundation]
attribution: 
    - citation: >
        "Introduction to High-Performance Computing" course by Edinburgh Parallel Computing Centre.
      url: https://epcced.github.io/Intro-to-HPC/
      image: https://epcced.github.io/Intro-to-HPC/_static/epcc_logo.svg
      license: CC-BY-4.0
---

So far the code examples we've looked at and submitted have been limited to serial computation.
Building on what we've learned so far, the next lesson will look at how we achieve parallel computations using both shared and distributed memory approaches.
To get us started, let's look at how we can compile and run parallel code versions of our "Hello world" example using shared and distributed memory frameworks,
MPI (Message Passing Interface) and OpenMP (Open Multi-processing),
which are both heavily used in HPC applications and we'll cover in more detail later on.

## Part 1: Shared Memory Parallelism Using OpenMP

OpenMP uses a shared memory approach to parallelism, allowing simultaneous computations to be spread over multiple threads within a process.

This threaded example runs on as many threads on a node as you allow it to.

You'll notice the code below is a little more complex than our original Hello world example, with
OpenMP making use of compiler directives (`#pragma`) to inform the compiler how to parallelise certain parts of the code when building an executable.

Add this code to a new file `helloWorldThreaded.c`:

```c
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // Check input argument
    if(argc != 2)
    {
        printf("Required one argument `name`.\n");
        return 1;
    }

    // Receive argument
    char* iname = (char *)malloc(strlen(argv[1]));
    strcpy(iname,argv[1]);

    // Get the name of the node we are running on
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);

    // Message from each thread on the node to the user
    #pragma omp parallel
    {
        printf("Hello %s, this is node %s responding from thread %d\n", iname, hostname,
               omp_get_thread_num());
    }
  
    // Release memory holding command line argument
    free(iname);
}
```

In this case, the code block indicated by the `#pragma omp parallel` statement will be run (by default) over as many threads as are available (typically one per available core).

Let's compile this code now.
On ARCHER2, this looks like the following:

```bash
cc helloWorldThreaded.c -fopenmp -o hello-THRD
```

Again, on a local machine, depending on your compiler setup, you may need to use `gcc` instead of `cc`.

Here, we inform the C compiler that this is an OpenMP program using the `-fopenmp` flag.
Without it, the `#pragma` statements won't be interpreted and our program will just run within a single thread.

If you run this now using `./hello-THRD yourname` you should see something like:

```output
Hello yourname, this is node ln01 responding from thread 151
Hello yourname, this is node ln01 responding from thread 157
Hello yourname, this is node ln01 responding from thread 106
Hello yourname, this is node ln01 responding from thread 65
Hello yourname, this is node ln01 responding from thread 144
Hello yourname, this is node ln01 responding from thread 116
Hello yourname, this is node ln01 responding from thread 199
Hello yourname, this is node ln01 responding from thread 239
Hello yourname, this is node ln01 responding from thread 47
Hello yourname, this is node ln01 responding from thread 63
Hello yourname, this is node ln01 responding from thread 254
Hello yourname, this is node ln01 responding from thread 173
Hello yourname, this is node ln01 responding from thread 169
Hello yourname, this is node ln01 responding from thread 44
Hello yourname, this is node ln01 responding from thread 243
Hello yourname, this is node ln01 responding from thread 244
Hello yourname, this is node ln01 responding from thread 245
Hello yourname, this is node ln01 responding from thread 242
...
```

Which when running on an ARCHER2 login node will likely make use of 256 threads.
If on your own machine, this is probably more like 4, 8 or perhaps 16 threads.

::::challenge{id=parallel_comp_pr.1 title="How many threads?"}
We can change the number of threads used by an OpenMP program by setting the `OMP_NUM_THREADS` environment variable.
Try this now, and check the output.

:::solution

```bash
export OMP_NUM_THREADS=4
```

```output
Hello yourname, this is node ln01 responding from thread 0
Hello yourname, this is node ln01 responding from thread 1
Hello yourname, this is node ln01 responding from thread 3
Hello yourname, this is node ln01 responding from thread 2
```

:::

::::

::::challenge{id=parallel_comp_pr.2 title="Why the random order?"}
You likely noticed that the order of the output from each thread is not (necessarily) output in order.
Why do you think this is?

:::solution
Since the threads are running in parallel, they are not guaranteed to run their code statements in any particular order.
:::

::::

::::challenge{id=parallel_comp_pr.3 title="Submitting an OpenMP job"}

**To be able to run the job submission examples in this segment, you'll need to have access to a Slurm job scheduler for example on an HPC infrastructure such as ARCHER2 or DiRAC.**

Write a job submission script that runs this OpenMP code.

You'll need to specify the number of CPU cores to use using the `--cpus-per-task` `#SBATCH` parameter.

:::solution

```bash
#!/bin/bash

#SBATCH --job-name=Hello-OMP
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:01:00

# Replace [project code] below with your project code (e.g. t01)
#SBATCH --account=[project code]
#SBATCH --partition=standard
#SBATCH --qos=standard

# Set the number of threads to the CPUs per task
export OMP_NUM_THREADS=4

./hello-THRD yourname
```

:::
::::

## Part 2: Distributed Memory Parallelism Using MPI

MPI is a message passing interface that uses a distributed memory approach to parallelism. This allows for messages to be sent by multiple instances of the program running within different processes to each other.

In this MPI example, which we'll put in a file called `helloWorldMPI.c`, each process prints out a hello message which states which node it is running on and which process in the group it is, and includes a string (the command line argument) passed to it from process (or *rank*) 0.
Rank 0, on the other hand, prints out a slightly different message.

```c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check input argument
    if(argc != 2)
    {
        printf("Required one argument `name`.\n");
        return 1;
    }

    // Receive arguments
    char* iname = (char *)malloc(strlen(argv[1])+1);
    char* iname2 = (char *)malloc(strlen(argv[1])+1);

    strcpy(iname, argv[1]);
    strcpy(iname2, iname);

    // MPI Setup
    int rank, size, len;
    char name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Get_processor_name(name, &len);

    // Create message from rank 0 to broadcast to all processes.
    strcat(iname, "@");
    strcat(iname, name);

    int inameSize = strlen(iname);

    // Create buffer for message
    char* buff = (char *)malloc(inameSize);

    // Sending process fills the buffer
    if (rank == 0)
    {
        strcpy(buff, iname);
    }

    // Send the message
    MPI_Bcast(buff, inameSize, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Send different messages from different ranks
    // Send hello from rank 0
    if (rank == 0)
    {
       printf("Hello world, my name is %s, I am printing this message from process %d of %d total processes executing, which is running on node %s. \n", iname2, rank, size, name);
    }

    // Send responce from the other ranks
    if (rank != 0)
    {
        printf("Hello, %s I am process %d of %d total processes executing and I am running on node %s.\n", buff, rank, size, name);
    }
    
    free(buff);
    free(iname2);
    free(iname);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
```

You’ll notice that the program is a fair bit more complex, since here we need to handle explicitly how we send messages.
We'll cover this in more detail in the next lesson.
But essentially, after initialising MPI and working out how many separate processes we have available to use (known as `ranks`),
rank 0 sends the command line string using `MPI_Bcast` (broadcast) to all other processes.

On ARCHER2, you compile this code using:

```bash
cc helloWorldMPI.c -o hello-MPI
```

::::callout

## On your own machine

If you're compiling and running this on your own machine, you'll very likely need to use a custom MPI compiler called `mpicc` instead which is typically bundled as part of an MPI installation:

```bash
mpicc helloWorldMPI.c -o hello-MPI
```

Then, to run this locally on your own machine, you typically use the `mpiexec` command.
For example, to run our code over 4 processes, or ranks:

```bash
mpiexec -n 4 ./hello-MPI
```

::::

::::challenge{id=parallel_comp_pr.4 title="Submitting an MPI job"}

**To be able to run the job submission examples in this segment, you'll need to either have access to ARCHER2, or an HPC infrastructure running the Slurm job scheduler and knowledge of how to configure job scripts for submission.**

Write a Slurm submission script for our MPI job, so that it runs across 4 processes. Note that you'll need to:

- Specify the number of processes to use as an `#SBATCH` parameter. Which one should you use? (*Hint:* look back at the material that introduced the first job we submitted via Slurm)
- Use the Slurm `srun` command to run our MPI job, e.g. `srun ./hello-MPI yourname`

:::solution
We need to use the `tasks-per-node` parameter to specify the number of processes to run.

```bash
#!/bin/bash

#SBATCH --job-name=Hello-MPI
#SBATCH --nodes=1
#SBATCH --tasks-per-node=4
#SBATCH --cpus-per-task=1
#SBATCH --time=00:01:00

# Replace [project code] below with your project code (e.g. t01)
#SBATCH --account=[project code]
#SBATCH --partition=standard
#SBATCH --qos=standard

srun ./hello-MPI yourname
```

:::
::::

After you've submitted the job (or run it locally) and it's completed, you should see something like:

```output
Hello, yourname@nid001686 I am process 1 of 4 total processes executing and I am running on node nid001686.
Hello, yourname@nid001686 I am process 2 of 4 total processes executing and I am running on node nid001686.
Hello, yourname@nid001686 I am process 3 of 4 total processes executing and I am running on node nid001686.
Hello world, my name is yourname, I am sending this message from process 0 of 4 total processes executing, which is running on node nid001686. 
```

::::challenge{id=parallel_comp_pr.5 title="Increasing the number of nodes"}
What happens if you increase the number of nodes to 2?
Why do you think this happens?

:::solution
You'll see something like:

```output
Hello, yourname@nid003165 I am process 4 of 8 total processes executing and I am running on node nid003174.
Hello, yourname@nid003165 I am process 5 of 8 total processes executing and I am running on node nid003174.
Hello, yourname@nid003165 I am process 6 of 8 total processes executing and I am running on node nid003174.
Hello, yourname@nid003165 I am process 7 of 8 total processes executing and I am running on node nid003174.
Hello world, my name is yourname, I am sending this message from process 0 of 8 total processes executing, which is running on node nid003165. 
Hello, yourname@nid003165 I am process 2 of 8 total processes executing and I am running on node nid003165.
Hello, yourname@nid003165 I am process 3 of 8 total processes executing and I am running on node nid003165.
Hello, yourname@nid003165 I am process 1 of 8 total processes executing and I am running on node nid003165.
```

Increasing the number of nodes to 2, with 4 tasks (or processes) per node means we have a total of 8 processes running our code.
:::
::::
