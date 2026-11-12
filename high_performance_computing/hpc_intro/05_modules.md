---
name: Using Software Modules
dependsOn: [high_performance_computing.hpc_intro.04_scheduler]
tags: []
learningOutcomes:
  - Find centrally installed software and identify the versions available on an HPC cluster.
  - Explain how loading a module changes the shell environment and may introduce dependencies or conflicts.
  - Select an explicit software version and recreate that module selection in a Slurm batch job.
attribution:
  - citation: >
      This course was developed by Alasdair Wilson as part of the Oxford Research Software Engineering group (OxRSE).
    url: https://www.rse.ox.ac.uk
    image: ./fig/2024_oxrse_square.svg
    license: CC-BY-4.0
  - citation: >
      A previous version of this material was adapted from "Introduction to High-Performance Computing" by The Carpentries.
    url: https://carpentries-incubator.github.io/hpc-intro/
    image: /carpentries-incubator.svg
    license: CC-BY-4.0
---

HPC users rely on software ranging from compilers and numerical libraries to specialised simulation, analysis and machine-learning applications.
Different applications, and even different versions of the same application, may require incompatible compilers, libraries, packages or other dependencies.
A shared cluster must make these combinations of software available to its users without forcing everyone into one system-wide environment.

Most clusters solve this problem by providing centrally installed software through an *environment module* system.
Modules allow each user to select the software they need for a particular job without affecting anyone else's environment.

## Modules

A module is a description of the environment required by a particular software package or tool.
Loading it modifies variables in the current shell, commonly adding executable directories to `PATH`, library directories to search paths and package-specific configuration such as licence-server locations.
It may also load compatible dependencies, commonly reporting conflicts with software already present.

Loading a module is not the same as installing the application or copying it into the working directory.
It makes an existing installation accessible from that shell, and unloading the module removes the corresponding changes wherever possible.

Clusters commonly use either Lmod or Environment Modules.
Both provide similar core operations through the `module` command, but the available software, default environment and exact behaviour are configured on a cluster-by-cluster basis.

## Inspect the Current Environment

A new shell may already contain default modules.
Do not assume that the environment begins empty.

You can list modules currently loaded with:

```bash
remote$ module list
```

The names and versions in the output describe part of the current software environment.
They may include compiler, network or architecture modules as well as applications that you recognise.

Software discovery can often be performed on a login node, but the authoritative environment is the one available on the type of compute node where the application will run.
Use an interactive allocation when the service documentation directs you to search or test software there.

:::callout{variant="note"}

## Finding Software on ARC

ARC's login and compute nodes do not expose the same software environment.
Enter the [interactive allocation introduced in the previous section](high_performance_computing/hpc_intro/04_scheduler) before searching for available modules with `module spider` or testing them.

ARC also maintains a [catalogue of available modules and versions](https://arc-module-list.readthedocs.io/en/latest/).

:::

## Find and Inspect Software

The module catalogue on a large cluster may contain hundreds upon hundreds of packages and usually several versions or builds of each one.
On systems using Lmod, `module spider` searches the wider module hierarchy and can reveal versions that require another compiler or toolchain module to be loaded first.
Search for Python using the familiar software name:

```bash
remote-compute$ module spider python
```

ARC performs this search without regard to case and reports a module family named `Python`.
Other services may use a different name, so copy the module name from the local result rather than assuming that ARC's spelling is universal.

Once you know the module name, `module avail` reports matching versions that can be loaded in the current environment:

```bash
remote-compute$ module avail Python
```

If `module spider` is unavailable, use the search mechanism documented by the service, commonly a filtered `module avail` command.
Once you have selected a version, inspect its documentation and the changes it would make:

```bash
remote-compute$ module help Python/version
remote-compute$ module show Python/version
```

Searches may ignore capitalisation, but when loading a module, copy its name exactly as it appears in the results.
Use the spelling and version reported by the cluster rather than copying a name from another service.
The module name also need not match the executable name: ARC's `Python` module provides commands including `python3`.

## Load and Verify Python

Choose one of the versions reported by the catalogue and copy its complete module name.
For example, if the catalogue reports a module named `Python/version`, load it with:

```bash
remote-compute$ module load Python/version
```

Some catalogues allow the version to be omitted and load a default instead.
Defaults are convenient for exploration but may change as the service updates its software, so jobs should normally request an explicit version.

After loading a module, inspect the resulting environment and verify the program that the shell will execute:

```bash
remote-compute$ module list
remote-compute$ command -v python3
remote-compute$ python3 --version
```

A single `module load` command may introduce several modules to the environment because applications are often built for a particular compiler, message-passing library or accelerator toolchain.
Conversely, the module system may refuse an incompatible combination or replace one member of a mutually exclusive family.
Read these messages and use `module list` to record the environment that was actually created.

::::challenge{id=observe-module-environment title="Observe How a Module Changes the Environment"}

Search the local module catalogue for Python and identify an exact module version.
Use an interactive compute allocation if required by the service.
If you loaded the selected Python module while following the preceding example, unload it before recording the initial environment.

Record the environment before loading the module, inspect the selected Python module, load it and then record the environment again.
Compare the two records and verify both the location and version of Python.

:::solution

Substitute the exact Python module name that you found in the local catalogue:

```bash
remote-compute$ python_module='Python/version'

remote-compute$ module unload "$python_module"
remote-compute$ env | sort > environment-before.txt
remote-compute$ module show "$python_module"
remote-compute$ module load "$python_module"
remote-compute$ env | sort > environment-after.txt

remote-compute$ module list
remote-compute$ command -v python3
remote-compute$ python3 --version
remote-compute$ diff -u environment-before.txt environment-after.txt
```

The comparison will commonly show changes to `PATH`, library search paths, `MANPATH`, `MODULEPATH` or variables specific to the application.
It may also show compiler or library modules loaded as dependencies.

:::
::::

## Change or Reset the Environment

You may remove a module that is no longer required with:

```bash
remote-compute$ module unload name/version
```

Where supported, `module swap old-name/version new-name/version` replaces one module with another and allows the module system to check the transition for conflicts.
Commands such as `module restore`, `module reset` and `module purge` do not behave identically on every service.
Removing the service's default modules can leave an unusable compiler or runtime environment.
Use the reset operation recommended in your cluster's documentation rather than assuming that `module purge` is safe.

:::callout{variant="warning"}

## Preserve the Service Defaults

[ARCHER2 explicitly warns against `module purge`](https://docs.archer2.ac.uk/user-guide/sw-environment/) because it disrupts the HPE Cray programming environment and recommends `module restore` instead.
[Isambard documents `module reset`](https://docs.isambard.ac.uk/user-documentation/tutorials/intro-tour/) for returning to its default set.
This difference is a useful reminder that even familiar module commands operate within a locally configured environment.

:::

## Reproduce the Environment in a Batch Job

An application tested successfully in an interactive shell should be run with the same module environment in the batch job.
Slurm and the shell may inherit some of the submission environment, but that behaviour is not a clear or durable record of the software the job requires.
Put the necessary module commands in the job script so that the environment is established whenever the job runs.

:::callout{variant="note"}

## Treat the Environment like Dependencies

It can be helpful to think of defining these explicit module versions as analogous to declaring package dependencies in software ecosystems such as Python and Node.js.
A job script should be able to produce a clear and durable record of the software environment it requires rather than depend on ad hoc changes or configurations.

:::

Use explicit module versions and print `module list` into the job output alongside the application version.
These records make it much easier to reproduce a result or diagnose a later difference.

The following script extends the job pattern from [Running Jobs with Slurm](high_performance_computing/hpc_intro/04_scheduler):

```bash
#!/bin/bash
#SBATCH --job-name=software-environment
#SBATCH --time=00:02:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --output=software-%j.out

# Add any other #SBATCH directives required by your service above this line.

set -euo pipefail

python_module='Python/version'

# Add the reset command recommended by your service here if one is required.
module load "$python_module"

printf 'Loaded modules:\n'
module list
printf 'Python path: '
command -v python3
printf 'Python version:\n'
python3 --version
```

Set `python_module` to the exact module name reported by your local catalogue before submitting the script.

::::challenge{id=reproduce-module-job title="Run the Same Software in a Batch Job"}

Adapt the script above for the module investigated in the previous exercise and add any directives required by the cluster.
Submit it, monitor it and inspect its output using the workflow from the previous section.

Confirm that the batch job loaded the intended module and found the expected Python version.
Compare the reported module list and executable path with the values from the interactive allocation.

:::solution

The submission and inspection follow the same pattern as the first scheduled job:

```bash
remote$ sbatch software-environment.sh
Submitted batch job 12345

remote$ squeue --job=12345
remote$ sacct --jobs=12345 --format=JobID,JobName,State,Elapsed,ExitCode
remote$ cat software-12345.out
```

Replace `12345` with the job ID returned by `sbatch`.
A successful output file contains the exact module environment, Python executable path and Python version selected by the script.

If the module is unavailable in the job, repeat the search from the same type of compute node and check whether another compiler or toolchain module must be loaded first.
If the module command itself is unavailable, use the shell initialisation or job template documented by the service.

:::
::::

The software environment is now part of the description of the job rather than an unrecorded property of an earlier shell session.

[Transferring Files with Remote Computers](high_performance_computing/hpc_intro/06_transferring_files) next considers how application inputs and outputs can be moved between the cluster and other systems.
