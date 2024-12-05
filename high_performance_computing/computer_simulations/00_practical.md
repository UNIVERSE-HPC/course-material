---
name: Traffic Simulation Performance
dependsOn: [
    high_performance_computing.parallel_computing.03_parallel_performance
]
tags: [foundation]
attribution: 
    - citation: >
        "Introduction to High-Performance Computing" course by Edinburgh Parallel Computing Centre.
      url: https://epcced.github.io/Intro-to-HPC/
      image: https://epcced.github.io/Intro-to-HPC/_static/epcc_logo.svg
      license: CC-BY-4.0
---

## Part 1: Traffic Simulation - Serial

Let's look at serial and OpenMP implementations of the traffic simulation model we looked at in the previous material,
let's investigate the basic performance characteristics of these implementations.

If on ARCHER2, to find the serial version of the traffic simulation code, firstly make sure you're on the `/work` partition (i.e. `cd /work/[project code]/[project code]/yourusername`).

Change directory to where the code is located, and use `make` as before to compile it:

```bash
cd foundation-exercises/traffic/C-SER
make
```

:::callout

## A Reminder

You may wish to reacquaint yourself with *The traffic model* section in the *Parallel Computing* material that briefly describes
the simulation model.
:::

A number of variables are currently fixed in the source code, which you can see by looking at the following lines
in `traffic.c`:

```c
  int ncell = 100000;
  maxiter = 200000000/ncell; 
  ...
  density = 0.52;
```

- The number of simulation cells is set to `100000`, so our simulated road is 100,000 * 5 = 500,000 metres long
- The number of iterations of the simulation is calculated based on the number of cells, such that - as coded - fewer cells means more iterations, but in this instance 200,000,000 / 100,000 = 2,000 total iterations
- The target traffic density is set to `0.52`, so the simulation aims to occupy just over half of the road cells

You can run the serial program direct on the login nodes:

```bash
./traffic
```

You should see:

```output
Length of road is 100000
Number of iterations is 2000 
Target density of cars is 0.520000 
Initialising road ...
...done
Actual density of cars is 0.517560

At iteration 200 average velocity is 0.919951 
At iteration 400 average velocity is 0.926559 
At iteration 600 average velocity is 0.928743 
At iteration 800 average velocity is 0.930308 
At iteration 1000 average velocity is 0.930849 
At iteration 1200 average velocity is 0.931196 
At iteration 1400 average velocity is 0.931312 
At iteration 1600 average velocity is 0.931506 
At iteration 1800 average velocity is 0.931737 
At iteration 2000 average velocity is 0.931989 

Finished

Time taken was  1.293764 seconds
Update rate was 154.587714 MCOPs
```

The result we are interested in this the final average velocity that is reported at iteration 2000 (i.e. the end of the simulation). In this case, the final average velocity of the traffic was 0.93.

## Part 2: Traffic Simulation - OpenMP

You'll find the OpenMP version of this code in `foundation-exercises/traffic/C-OMP`.
Change to this directory, and compile the code as before.
The simulation is set at the same initial parameters as the serial version of the code
(if you're interested, take a look at the source code).

What we'd like to do now is measure how long it takes to run the simulation given an increasing number of threads,
so we can determine an ideal number of threads for running simulations in the future.

::::challenge{id=compsim_pr.1 title="Traffic Simulation: Scripting the Process"}
We could submit a number of separate jobs running the code with an increasing number of threads,
or if running this on our own machine, create a Bash script that does this locally,
but with the simulation's current configuration, each of these jobs would only take a second or so to run
(although if it took much longer than this, then separate jobs would likely make sense!).

So instead of creating a number of separate scripts and submitting/running those,
we'll put all the runs into a single script.
Create a single script that does the following for 1, 2, 4, 6, 8, 10, 12, 14, 16, 18, and 20 threads:

- Sets the number of threads (i.e. setting the `OMP_NUM_THREADS` variable)
- Runs the `traffic` code

If you're writing ARCHER2 job submission scripts you'll need to set `--cpus-per-task` to the maximum number of threads you'll use in the script (i.e. 20),
and set `--time` to a suitable value so encompass all the separate runs.

Then, either submit the job script using `sbatch` to submit it to ARCHER2 or run it directly using e.g. `bash script.sh`.

:::solution

(If you're running this on your own machine in a normal Bash script, you can ignore the lines starting `#SBATCH`)

```bash
#!/bin/bash

#SBATCH --job-name=Traffic-OMP
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=20
#SBATCH --time=00:05:00

# Replace [project code] below with your project code (e.g. t01)
#SBATCH --account=[project code]
#SBATCH --partition=standard
#SBATCH --qos=standard

export OMP_NUM_THREADS=1
./traffic

export OMP_NUM_THREADS=2
./traffic

export OMP_NUM_THREADS=4
./traffic

export OMP_NUM_THREADS=6
./traffic

export OMP_NUM_THREADS=8
./traffic

export OMP_NUM_THREADS=10
./traffic

export OMP_NUM_THREADS=12
./traffic

export OMP_NUM_THREADS=14
./traffic

export OMP_NUM_THREADS=16
./traffic

export OMP_NUM_THREADS=18
./traffic

export OMP_NUM_THREADS=20
./traffic
```

Or, if you're familiar with Bash loops:

```bash
#!/bin/bash

#SBATCH --job-name=Traffic-OMP
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=20
#SBATCH --time=00:05:00

# Replace [project code] below with your project code (e.g. t01)
#SBATCH --account=[project code]
#SBATCH --partition=standard
#SBATCH --qos=standard

for THREADS in 1 2 4 6 8 10 12 14 16 18 20
do
    export OMP_NUM_THREADS=${THREADS}
    ./traffic
done
```

:::
::::

::::challenge{id=compsim_pr.2 title="Traffic Simulation: Measuring Multiple Threads Runtimes"}

Next, let's look at the timings together by first entering them into a table,
by examining the output (or via Slurm output files) and enter each time into a table, e.g. using the following columns:

| #Threads | Time(s)
|----------|--------
| 1        | ...
| 2        | ...
| ...      | ...

:::solution

Of course, your timings may differ!

| #Threads | Time(s)
|----------|--------
| 1        | 1.744
| 2        | 0.899
| 4        | 0.468
| 6        | 0.316
| 8        | 0.248
| 10       | 0.211
| 12       | 0.185
| 14       | 0.167
| 16       | 0.157
| 18       | 0.146
| 20       | 0.140

:::
::::

::::challenge{id=compsim_pr.3 title="Traffic Simulation: Analysing Timings"}

Compare the timing results against the serial version of the code.
At what number of threads does the OpenMP version yield faster results?
What does this mean in terms of the overhead of using OpenMP for this simulation code as it stands?

:::solution
Looking at your results, you may find that using just two threads is significantly faster.
In terms of overhead, this means that the overhead of using OpenMP has a significant impact on a single thread,
as one may expect, but by 2 threads we see a significant speedup.
:::

At what point does there appear to be diminishing returns when increasing the number of threads?

:::solution
It depends on what you consider a diminishing return,
but (at least for my runs) beyond about 14 threads the yields are significantly smaller (6% speed increase and below).

Of course, for expediency in this exercise we're using small problem spaces to reduce the job's execution time, but for much larger problem spaces and runtimes the time savings we see here would be significant.
:::
::::

:::callout

## How to Time Code that doesn't Time Itself?

With the traffic simulation code we're fortunate that it has an in-built ability to time itself.
What about code that doesn't do this?
Fortunately, there's a bash command `time` that can be used.
For example, change directory to where your serial version of hello world is located, and then:

```bash
time ./hello-SER yourname
```

```output
Hello World!
Hello yourname, this is ln01.

real    0m0.059s
user    0m0.004s
sys     0m0.000s
```

Which gives us, essentially, the completed run time of 0.059s.
:::
