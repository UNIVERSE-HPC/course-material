---
name: Running a parallel job
dependsOn: [
  high_performance_computing.hpc_intro.06_transferring_files
]
tags: [slurm]
attribution: 
    - citation: >
        "Introduction to High-Performance Computing" course by the HPC-carpentries
      url: https://carpentries-incubator.github.io/hpc-intro/
      image: https://carpentries-incubator.github.io/hpc-intro/assets/img/incubator-logo-blue.svg
      license: CC-BY-4.0
---

We now have the tools we need to run a multi-processor job. This is a very
important aspect of HPC systems, as parallelism is one of the primary tools
we have to improve the performance of computational tasks.

If you disconnected, log back in to the cluster.

```bash
local$ ssh user@cluster.name
```

## Install the Amdahl Program

With the Amdahl source code on the cluster, we can install it, which will
provide access to the `amdahl` executable.
Move into the extracted directory, then use the Package Installer for Python,
or `pip`, to install it in your ("user") home directory:

```bash
remote$ cd amdahl
remote$ python3 -m pip install --user .
```

::::callout
## Amdahl is Python Code

The Amdahl program is written in Python, and installing or using it requires
locating the `python3` executable on the login node.
If it can't be found, try listing available modules using `module avail`,
load the appropriate one, and try the command again.
::::

### MPI for Python

The Amdahl code has one dependency: __`mpi4py`__.
If it hasn't already been installed on the cluster, `pip` will attempt to
collect `mpi4py` from the Internet and install it for you.
If this fails due to a one-way firewall, you must retrieve `mpi4py` on your
local machine and upload it, just as we did for Amdahl.

::::callout
## Retrieve and Upload `mpi4py`

If installing Amdahl failed because `mpi4py` could not be installed,
retrieve the tarball from <https://github.com/mpi4py/mpi4py/tarball/master>
then `rsync` it to the cluster, extract, and install:

```bash
local$ wget -O mpi4py.tar.gz https://github.com/mpi4py/mpi4py/releases/download/3.1.4/mpi4py-3.1.4.tar.gz
local$ scp mpi4py.tar.gz user@cluster.name:
# or
local$ rsync -avP mpi4py.tar.gz user@cluster.name:
```

```bash
local$ ssh user@cluster.name
remote$ tar -xvzf mpi4py.tar.gz  # extract the archive
remote$ mv mpi4py* mpi4py        # rename the directory
remote$ cd mpi4py
remote$ python3 -m pip install --user .
remote$ cd ../amdahl
remote$ python3 -m pip install --user .
```
::::

::::callout
## If `pip` Raises a Warning...

`pip` may warn that your user package binaries are not in your PATH.

```
WARNING: The script amdahl is installed in "${HOME}/.local/bin" which is
not on PATH. Consider adding this directory to PATH or, if you prefer to
suppress this warning, use --no-warn-script-location.
```

To check whether this warning is a problem, use `which` to search for the
`amdahl` program:

```bash
remote$ which amdahl
```

If the command returns no output, displaying a new prompt, it means the file
`amdahl` has not been found. You must update the environment variable named
`PATH` to include the missing folder.
Edit your shell configuration file as follows, then log off the cluster and
back on again so it takes effect.

```bash
remote$ nano ~/.bashrc
remote$ tail ~/.bashrc
```
```
export PATH=${PATH}:${HOME}/.local/bin
```

After logging back in to cluster.name, `which` should be able to
find `amdahl` without difficulties.
If you had to load a Python module, load it again.
::::

## Help!

Many command-line programs include a "help" message. Try it with `amdahl`:

```bash
remote$ amdahl --help
usage: amdahl [-h] [-p [PARALLEL_PROPORTION]] [-w [WORK_SECONDS]] [-t] [-e] [-j [JITTER_PROPORTION]]

optional arguments:
  -h, --help            show this help message and exit
  -p [PARALLEL_PROPORTION], --parallel-proportion [PARALLEL_PROPORTION]
                        Parallel proportion: a float between 0 and 1
  -w [WORK_SECONDS], --work-seconds [WORK_SECONDS]
                        Total seconds of workload: an integer greater than 0
  -t, --terse           Format output as a machine-readable object for easier analysis
  -e, --exact           Exactly match requested timing by disabling random jitter
  -j [JITTER_PROPORTION], --jitter-proportion [JITTER_PROPORTION]
                        Random jitter: a float between -1 and +1
```

This message doesn't tell us much about what the program _does_, but it does
tell us the important flags we might want to use when launching it.

## Running the Job on a Compute Node

Create a submission file, requesting one task on a single node, then launch it.

```bash
remote$ nano serial-job.sh
remote$ cat serial-job.sh
```
```bash
#!/bin/bash
#SBATCH -J solo-job
#SBATCH -p cpubase_bycore_b1
#SBATCH -N 1
#SBATCH -n 1

# Load the computing environment we need
module load Python

# Execute the task
amdahl
```

```bash
remote$ sbatch serial-job.sh
```

As before, use the Slurm status commands to check whether your job
is running and when it ends:

```bash
remote$ squeue -u yourUsername
```

:::::challenge{id=read-output, title="Read the Job Output"}
Use `ls` to locate the output file. The `-t` flag sorts in
reverse-chronological order: newest first. What was the output?

::::solution
The cluster output should be written to a file in the folder you launched the
job from. For example,

```bash
remote$ ls -t
```
```
slurm-347087.out  serial-job.sh  amdahl  README.md  LICENSE.txt
```
```bash
remote$ cat slurm-347087.out
```
```
Doing 30.000 seconds of 'work' on 1 processor,
which should take 30.000 seconds with 0.850 parallel proportion of the workload.

  Hello, World! I am process 0 of 1 on <<node1>>. I will do all the serial 'work' for 4.500 seconds.
  Hello, World! I am process 0 of 1 on <<node1>>. I will do parallel 'work' for 25.500 seconds.

Total execution time (according to rank 0): 30.033 seconds
```
::::
:::::

As we saw before, two of the `amdahl` program flags set the amount of work and
the proportion of that work that is parallel in nature. Based on the output, we
can see that the code uses a default of 30 seconds of work that is 85%
parallel. The program ran for just over 30 seconds in total, and if we run the
numbers, it is true that 15% of it was marked 'serial' and 85% was 'parallel'.

Since we only gave the job one CPU, this job wasn't really parallel: the same
processor performed the 'serial' work for 4.5 seconds, then the 'parallel' part
for 25.5 seconds, and no time was saved. The cluster can do better, if we ask.

## Running the Parallel Job

The `amdahl` program uses the Message Passing Interface (MPI) for parallelism
-- this is a common tool on HPC systems.

::::callout
## What is MPI?

The Message Passing Interface is a set of tools which allow multiple tasks
running simultaneously to communicate with each other.
Typically, a single executable is run multiple times, possibly on different
machines, and the MPI tools are used to inform each instance of the
executable about its sibling processes, and which instance it is.
MPI also provides tools to allow communication between instances to
coordinate work, exchange information about elements of the task, or to
transfer data.
An MPI instance typically has its own copy of all the local variables.
::::

While MPI-aware executables can generally be run as stand-alone programs, in
order for them to run in parallel they must use an MPI _run-time environment_,
which is a specific implementation of the MPI _standard_.
To activate the MPI environment, the program should be started via a command
such as `mpiexec` (or `mpirun`, or `srun`, etc. depending on the MPI run-time
you need to use), which will ensure that the appropriate run-time support for
parallelism is included.

::::callout
## MPI Runtime Arguments

On their own, commands such as `mpiexec` can take many arguments specifying
how many machines will participate in the execution,
and you might need these if you would like to run an MPI program on your
own (for example, on your laptop).
In the context of a queuing system, however, it is frequently the case that
MPI run-time will obtain the necessary parameters from the queuing system,
by examining the environment variables set when the job is launched.
::::

Let's modify the job script to request more cores and use the MPI run-time.

```bash
remote$ cp serial-job.sh parallel-job.sh
remote$ nano parallel-job.sh
remote$ cat parallel-job.sh
```
```bash
#!/bin/bash
#SBATCH -J parallel-job
#SBATCH -p cpubase_bycore_b1
#SBATCH -N 1
#SBATCH -n 4

# Load the computing environment we need
# (mpi4py and numpy are in SciPy-bundle)
module load Python
module load SciPy-bundle

# Execute the task
mpiexec amdahl
```

Then submit your job. Note that the submission command has not really changed
from how we submitted the serial job: all the parallel settings are in the
batch file rather than the command line.

```bash
remote$ sbatch parallel-job.sh
```

As before, use the status commands to check when your job runs.

```bash
remote$ ls -t
```
```
slurm-347178.out  parallel-job.sh  slurm-347087.out  serial-job.sh  amdahl  README.md  LICENSE.txt
```
```bash
remote$ cat slurm-347178.out
```
```
Doing 30.000 seconds of 'work' on 4 processors,
which should take 10.875 seconds with 0.850 parallel proportion of the workload.

  Hello, World! I am process 0 of 4 on <<node1>>. I will do all the serial 'work' for 4.500 seconds.
  Hello, World! I am process 2 of 4 on <<node1>>. I will do parallel 'work' for 6.375 seconds.
  Hello, World! I am process 1 of 4 on <<node1>>. I will do parallel 'work' for 6.375 seconds.
  Hello, World! I am process 3 of 4 on <<node1>>. I will do parallel 'work' for 6.375 seconds.
  Hello, World! I am process 0 of 4 on <<node1>>. I will do parallel 'work' for 6.375 seconds.

Total execution time (according to rank 0): 10.888 seconds
```

::::challenge{id=speedup-factor, title="Is it 4× faster?"}

The parallel job received 4× more processors than the serial job:
does that mean it finished in ¼ the time?

:::solution
The parallel job did take _less_ time: 11 seconds is better than 30!
But it is only a 2.7× improvement, not 4×.

Look at the job output:

* While "process 0" did serial work, processes 1 through 3 did their parallel work.
* While process 0 caught up on its parallel work, the rest did nothing at all.

Process 0 always has to finish its serial task before it can start on the
parallel work. This sets a lower limit on the amount of time this job will
take, no matter how many cores you throw at it.

This is the basic principle behind [Amdahl's Law][amdahl], which is one way
of predicting improvements in execution time for a __fixed__ workload that
can be subdivided and run in parallel to some extent.
:::
::::

## How Much Does Parallel Execution Improve Performance?

In theory, dividing up a perfectly parallel calculation among _n_ MPI processes
should produce a decrease in total run time by a factor of _n_.
As we have just seen, real programs need some time for the MPI processes to
communicate and coordinate, and some types of calculations can't be subdivided:
they only run effectively on a single CPU.

Additionally, if the MPI processes operate on different physical CPUs in the
computer, or across multiple compute nodes, even more time is required for
communication than it takes when all processes operate on a single CPU.

In practice, it's common to evaluate the parallelism of an MPI program by

* running the program across a range of CPU counts,
* recording the execution time on each run,
* comparing each execution time to the time when using a single CPU.

Since "more is better" - improvement is easier to interpret from increases in
some quantity than decreases - comparisons are made using the speedup factor
_S_, which is calculated as the single-CPU execution time divided by the multi-CPU
execution time. For a perfectly parallel program, a plot of the speedup _S_
versus the number of CPUs _n_ would give a straight line, _S_ = _n_.

Let's run one more job, so we can see how close to a straight line our `amdahl`
code gets.

```bash
remote$ nano parallel-job.sh
remote$ cat parallel-job.sh
```
```bash
#!/bin/bash
#SBATCH -J parallel-job
#SBATCH -p cpubase_bycore_b1
#SBATCH -N 1
#SBATCH -n 8

# Load the computing environment we need
# (mpi4py and numpy are in SciPy-bundle)
module load Python
module load SciPy-bundle

# Execute the task
mpiexec amdahl
```

Then submit your job. Note that the submission command has not really changed
from how we submitted the serial job: all the parallel settings are in the
batch file rather than the command line.

```bash
remote$ sbatch parallel-job.sh
```

As before, use the status commands to check when your job runs.

```bash
remote$ ls -t
```
```
slurm-347271.out  parallel-job.sh  slurm-347178.out  slurm-347087.out  serial-job.sh  amdahl  README.md  LICENSE.txt
```
```bash
remote$ cat slurm-347178.out
```
```
which should take 7.688 seconds with 0.850 parallel proportion of the workload.

  Hello, World! I am process 4 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.
  Hello, World! I am process 0 of 8 on <<node1>>. I will do all the serial 'work' for 4.500 seconds.
  Hello, World! I am process 2 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.
  Hello, World! I am process 1 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.
  Hello, World! I am process 3 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.
  Hello, World! I am process 5 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.
  Hello, World! I am process 6 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.
  Hello, World! I am process 7 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.
  Hello, World! I am process 0 of 8 on <<node1>>. I will do parallel 'work' for 3.188 seconds.

Total execution time (according to rank 0): 7.697 seconds
```

:::callout
## Non-Linear Output
When we ran the job with 4 parallel workers, the serial job wrote its output
first, then the parallel processes wrote their output, with process 0 coming
in first and last.

With 8 workers, this is not the case: since the parallel workers take less
time than the serial work, it is hard to say which process will write its
output first, except that it will _not_ be process 0!
:::

Now, let's summarize the amount of time it took each job to run:

| Number of CPUs | Runtime (sec) |
| ---            | ---           |
| 1              | 30.033        |
| 4              | 10.888        |
| 8              |  7.697        |

Then, use the first row to compute speedups _S_, using Python as a command-line calculator:

```bash
remote$ for n in 30.033 10.888 7.697; do python3 -c "print(30.033 / $n)"; done
```

| Number of CPUs | Speedup | Ideal |
| ---            | ---     | ---   |
| 1              | 1.0     | 1     |
| 4              | 2.75    | 4     |
| 8              | 3.90    | 8     |

The job output files have been telling us that this program is performing 85%
of its work in parallel, leaving 15% to run in serial. This seems reasonably
high, but our quick study of speedup shows that in order to get a 4× speedup,
we have to use 8 or 9 processors in parallel. In real programs, the speedup
factor is influenced by

* CPU design
* communication network between compute nodes
* MPI library implementations
* details of the MPI program itself

Using Amdahl's Law, you can prove that with this program, it is _impossible_
to reach 8× speedup, no matter how many processors you have on hand. Details of
that analysis, with results to back it up, are left for the next section of the material,  _Scalability Profiling_.

[amdahl]: https://en.wikipedia.org/wiki/Amdahl's_law
[cmd-line]: https://swcarpentry.github.io/python-novice-inflammation/12-cmdline/index.html
[inflammation]: https://swcarpentry.github.io/python-novice-inflammation/
[np-dtype]: https://numpy.org/doc/stable/reference/generated/numpy.dtype.html
[parallel-novice]: http://www.hpc-carpentry.org/hpc-parallel-novice/
[python-func]: https://swcarpentry.github.io/python-novice-inflammation/08-func/index.html
[units]: https://en.wikipedia.org/wiki/Byte#Multiple-byte_units
