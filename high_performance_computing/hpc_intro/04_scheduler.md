---
name: Running Jobs with Slurm
dependsOn: [high_performance_computing.hpc_intro.03_storage]
tags: [slurm]
learningOutcomes:
  - Prepare and submit a Slurm batch job using appropriate resource requests and site-specific settings.
  - Relate Slurm's nodes, tasks and CPUs per task to serial, multithreaded and multiprocess applications.
  - Monitor pending and running jobs, inspect completed jobs and locate their output.
  - Cancel a job and start an interactive allocation on a compute node.
attribution:
  - citation: >
      This course was developed by Alasdair Wilson as part of the Oxford Research Software Engineering group (OxRSE).
    url: https://www.rse.ox.ac.uk
    image: ./fig/2024_oxrse_square.svg
  - citation: >
      A previous version of this material was adapted from "Introduction to High-Performance Computing" by The Carpentries.
    url: https://carpentries-incubator.github.io/hpc-intro/
    image: /carpentries-incubator.svg
    license: CC-BY-4.0
---

You have connected to a cluster and prepared a working directory on storage visible to its compute nodes.
The next step is to run a command on those nodes without using the shared login node for the computation itself.

[Working on an HPC Cluster](high_performance_computing/hpc_intro/01_working_on_a_cluster) introduced how work moves from preparation and submission to execution and results.
This section puts that outline into practice with Slurm: describe the required resources, submit the work, monitor it and retrieve its output.

## From a Command to a Scheduled Job

A command entered at your remote shell runs on the login node unless it explicitly requests compute resources.
For example, `hostname` reports the login node on which your shell is running:

```bash
remote$ hostname
login01
```

A *batch job* packages commands so that they can run unattended within resources assigned by the scheduler.
The `sbatch` command transfers a batch script to Slurm and returns a numeric job ID.
The job may remain pending until Slurm can grant a suitable *allocation*, after which one copy of the script begins on an allocated node.

Slurm distinguishes the allocation from the work launched inside it.
An allocation is a reservation of resources, while a *job step* is a command or set of processes launched within those resources.
The batch script itself is one step, and parallel applications commonly use `srun` to launch additional steps.

## Resolve the Local Requirements

Slurm commands are portable, but the policy values supplied to them are not.
A service may require an account or project code, a partition, a quality of service, a reservation, or a particular accelerator request.
The names and permitted combinations of those settings are defined by the service rather than by Slurm.

The details collected in the first section should tell you which settings your cluster requires.
If they do not, consult its job-submission documentation before continuing.
The [Oxford ARC](https://arc-user-guide.readthedocs.io/en/latest/job-scheduling.html), [ARCHER2](https://docs.archer2.ac.uk/user-guide/scheduler/) and [Isambard](https://docs.isambard.ac.uk/user-documentation/guides/slurm/) guides illustrate how three Slurm services expose the same workflow through different local policies.

:::callout{variant="warning"}

## Commands Are Portable; Policy Is Local

Do not copy an account, partition or quality-of-service value from another cluster merely because its submission script looks similar.
An omitted setting may have a sensible default on one service and cause rejection or an unsuitable allocation on another.

:::

## Prepare a First Batch Script

Return to the working directory prepared in [Storage on an HPC Cluster](high_performance_computing/hpc_intro/03_storage).
Create `first-job.sh` there using your preferred text editor:

```bash
remote$ cd /path/to/your/hpc-intro/directory
remote$ nano first-job.sh
```

Enter the following script, then add any account, partition, quality-of-service or other directives required by your service:

```bash
#!/bin/bash
#SBATCH --job-name=hpc-intro
#SBATCH --time=00:02:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --output=slurm-%j.out

# Add any other #SBATCH directives required by your service above this line.

set -euo pipefail

printf 'Job ID: %s\n' "$SLURM_JOB_ID"
printf 'Host: '
hostname
printf 'Working directory: %s\n' "$(pwd)"
printf 'Submitted from: %s\n' "$SLURM_SUBMIT_DIR"
printf 'Tasks: %s\n' "$SLURM_NTASKS"
printf 'CPUs per task: %s\n' "$SLURM_CPUS_PER_TASK"
printf 'Started: '
date
sleep 30
printf 'Finished: '
date
```

The shebang selects Bash as the program that interprets the script.
Lines beginning with `#SBATCH` are comments to Bash but options to Slurm, and they must appear before the first executable line.
The remaining lines are ordinary shell commands; `set -euo pipefail` makes Bash stop on an unhandled command failure, an unset variable or a failed command within a pipeline.

The short `sleep` keeps this demonstration alive long enough to appear in the queue and should not be copied into a real workload.
The `%j` in the output filename is replaced by the job ID, keeping the output from separate submissions distinct.

:::callout{variant="note"}

## Slurm Directives Are Not Shell Commands

Slurm reads `#SBATCH` directives before Bash executes the script.
Shell expressions and variables in a directive are therefore treated as literal text rather than expanded by Bash.
Use command-line options to `sbatch` or construct the required path before submission if an option must vary dynamically.

:::

## Understand the Resource Request

The script asks Slurm for a maximum elapsed time and a small CPU allocation.
These options describe resources made available to the job; they do not change the behaviour of its commands.

| Directive | Meaning |
| --- | --- |
| `--time=00:02:00` | Terminate the job if it has not finished within two minutes of starting. |
| `--nodes=1` | Place the allocation on one compute node. |
| `--ntasks=1` | Provide resources for one task, normally one application process. |
| `--cpus-per-task=1` | Allocate one Slurm CPU to that task. |
| `--output=slurm-%j.out` | Write standard output and standard error to a job-specific file. |

In Slurm, a *CPU* is the smallest processing unit that the service has configured as allocatable.
It commonly corresponds to one physical CPU-core, but a service may instead expose hardware threads, so consult its documentation when the distinction matters.

Memory also belongs in a real resource request.
Slurm provides `--mem` for memory per node and `--mem-per-cpu` for memory per allocated CPU, but service defaults, limits and charging models vary enough that the appropriate form and value must be chosen locally.
Accelerators likewise require service-specific requests, commonly through options such as `--gpus` or `--gres`.

The intended application determines how nodes, tasks and CPUs per task should be combined:

| Application model | Typical relationship to a Slurm request |
| --- | --- |
| Serial program | One task using one CPU on one node. |
| [Multithreaded program](high_performance_computing/hpc_openmp) | Usually one task with several CPUs per task on one node. |
| [Distributed-memory program](high_performance_computing/hpc_mpi) | Several tasks representing processes, potentially distributed across several nodes. |
| [Hybrid program](high_performance_computing/hpc_openmp/05_hybrid_parallelism) | Several tasks with multiple CPUs per task, combining processes and threads. |

:::callout{variant="warning"}

## An Allocation Does Not Parallelise an Application

Requesting four tasks does not automatically start four copies of a command, and requesting four CPUs per task does not make a serial program use four CPU-cores.
The application or its launcher must create the corresponding processes or threads.
Later sections apply this distinction to serial and parallel programs; for now, the first job deliberately uses one task and one CPU.

:::

## Submit and Monitor the Job

Submit the script with [`sbatch`](https://slurm.schedmd.com/sbatch.html):

```bash
remote$ sbatch first-job.sh
Submitted batch job 12345
```

Your job ID will differ from `12345` and uniquely identifies this submission on the cluster.
Successful submission means that Slurm accepted the request, not that the job has started or will complete successfully.

Use [`squeue`](https://slurm.schedmd.com/squeue.html) to view your pending and running jobs:

```bash
remote$ squeue --user="$USER"
```

A job typically moves from `PENDING` (`PD`) to `RUNNING` (`R`) and may briefly enter `COMPLETING` (`CG`) before leaving the queue.
The final column reports the allocated nodes for a running job or the reason why a pending job has not started.
`Resources` and `Priority` are normal pending reasons rather than errors.

If the demonstration finishes before you see it in `squeue`, that simply means it moved through the queue quickly.
Use its accounting record to inspect it after completion:

```bash
remote$ sacct --jobs=12345 --format=JobID,JobName,State,Elapsed,ExitCode
```

Replace `12345` with your job ID.
[`sacct`](https://slurm.schedmd.com/sacct.html) can distinguish outcomes such as `COMPLETED`, `FAILED`, `TIMEOUT`, `CANCELLED` and `OUT_OF_MEMORY` after the job has left the live queue.
Accounting data and available fields depend on the service configuration and may take a short time to appear.
The output may contain separate `.batch` and `.extern` job-step records in addition to the row for the allocation as a whole.

For a pending or running job, `scontrol show job 12345` provides the complete request and its current state.
The information is verbose, but it is useful when a job is waiting for an unexpected reason or appears to have received the wrong resources.

## Inspect the Output

The batch script is not attached to your terminal, so its output is written to the file specified by `--output`.
After the job starts, list and inspect that file:

```bash
remote$ ls -l slurm-12345.out
remote$ cat slurm-12345.out
```

A successful run reports the same job ID, one compute-node hostname, the working directory and the resources requested by the script.
The compute-node hostname should normally differ from the login-node hostname recorded earlier.

By default, a batch job starts in the directory from which `sbatch` was called, and Slurm records that path in `SLURM_SUBMIT_DIR`.
The `--chdir` option can select a different starting directory when required.

:::callout{variant="warning"}

## Slurm Does Not Transfer Application Data

Slurm transfers the batch script to the controller, but it does not copy the script's input files to the compute node or return its results elsewhere.
The paths used by the job must refer to suitable storage visible from the allocated nodes, and important output must ultimately be retained in an appropriate storage space.

:::

## Cancel a Job

A pending or running job can be cancelled with [`scancel`](https://slurm.schedmd.com/scancel.html) and its job ID:

```bash
remote$ scancel 12345
```

Cancellation is appropriate when the request is wrong, the application is not making useful progress or its results are no longer needed.
The request may take a moment to propagate, after which the job disappears from `squeue` and normally appears as `CANCELLED` in `sacct`.

Avoid cancelling by username unless you genuinely intend to terminate every job belonging to your account.

## Interactive Jobs

Batch submission is appropriate when commands can run unattended.
Development, debugging and exploratory analysis sometimes require an interactive shell on a compute node, but that shell must still run inside an allocation.

On many Slurm services, a command of the following form requests a small allocation and starts Bash within it:

```bash
remote$ srun \
  --nodes=1 \
  --ntasks=1 \
  --cpus-per-task=1 \
  --time=00:15:00 \
  --pty bash --login
```

Add the account, partition, quality-of-service or reservation options required by your service.
Some services instead document `salloc` followed by `srun`, provide dedicated interactive nodes, or require a local wrapper, so their documented method takes precedence over this generic form.

Once the allocation begins, inspect the environment:

```bash
remote-compute$ hostname
remote-compute$ printf '%s\n' "$SLURM_JOB_ID"
remote-compute$ exit
```

Exiting the shell ends the interactive job and releases its resources.
Closing the network connection also normally ends a directly attached interactive job, which makes batch submission preferable for work that must survive disconnection.

## Run the Complete Workflow

::::challenge{id=run-first-slurm-job title="Submit, Inspect and Complete a Job"}

Use your cluster's documentation and the information gathered earlier in the course to adapt `first-job.sh` to the service.
Keep the request to one node, one task, one CPU per task and no more than two minutes, adding only the locally required account, partition, quality-of-service, memory or other settings.

Submit the script and retain its job ID.
Inspect it while it is pending or running, then use its accounting record and output file after it finishes.
Confirm which compute node ran the script, which directory it used and whether the allocated task and CPU counts match the request.

Finally, request a short interactive shell using the method documented for the service, compare its hostname with the login-node hostname and exit cleanly.

:::solution

There is no universal set of site-specific directives, but the batch part of a successful workflow has the following shape:

```bash
remote$ sbatch first-job.sh
Submitted batch job 12345

remote$ squeue --job=12345
remote$ sacct --jobs=12345 --format=JobID,JobName,State,Elapsed,ExitCode
remote$ cat slurm-12345.out
```

Replace `12345` with the ID returned by your submission.
It is normal for `squeue` to show no row if the job has already finished; `sacct` and the output file then provide the relevant evidence.

A completed output file should contain the assigned job ID, a compute-node hostname, the selected working directory, one task and one CPU per task.
If `sbatch` rejects the submission, its error usually identifies a missing or invalid local policy setting.
If Slurm accepts the job but it later fails, inspect both the accounting state and the output file before changing the request.

The interactive command is service-specific, but a successful allocation changes the hostname or prompt to identify a compute node and defines `SLURM_JOB_ID` until the shell exits.

:::
::::

The scheduler has now carried one job from submission through allocation, execution and accounting.
[Using Software Modules](high_performance_computing/hpc_intro/05_modules) next examines how to make application software available in both interactive and batch environments.
