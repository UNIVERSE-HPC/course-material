---
name: "Setup"
dependsOn: []
tags: [snakemake]
attribution:
  - citation: >
      Mölder, F., Jablonski, K.P., Letcher, B., Hall, M.B., Tomkins-Tinch,
      C.H., Sochat, V., Forster, J., Lee, S., Twardziok, S.O., Kanitz, A.,
      Wilm, A., Holtgrewe, M., Rahmann, S., Nahnsen, S., Köster, J., 2021.
      Sustainable data analysis with Snakemake. F1000Res 10, 33.
      Revision c7ae161c.
    url: https://snakemake.readthedocs.io/en/stable/tutorial/setup.html
    image: https://raw.githubusercontent.com/snakemake/snakemake/main/snakemake/report/template/logo.svg
    license: MIT license
---

# Setup

## Requirements

To go through this tutorial, you need a number of software packages including
[Python](https://www.python.org),
[Snakemake](https://snakemake.readthedocs.io),
[BWA](http://bio-bwa.sourceforge.net),
[SAMtools](https://www.htslib.org),
[Pysam](https://pysam.readthedocs.io),
[BCFtools](https://www.htslib.org),
[Graphviz](https://www.graphviz.org),
[Jinja2](https://jinja.palletsprojects.com),
[NetworkX](https://networkx.github.io),
[Matplotlib](https://matplotlib.org).
However, don\'t install any of these manually now as we guide you
through better ways below.

## Run tutorial for free in the cloud via Gitpod

:::callout

A common thing to happen while using the development environment in
GitPod is to hit `Ctrl-s` while in the terminal window, because you
wanted to save a file in the editor window. This will freeze up you
terminal. To get it back, make sure you selected the terminal window by
clicking on it and then hit `Ctrl-q`.

:::

The easiest way to run this tutorial is to use Gitpod, which enables
performing the excercises via your browser\-\--including all required
software, for free and in the cloud. In order to do this, simply open
the predefined [snakemake-tutorial GitPod
workspace](https://gitpod.io/#https://github.com/snakemake/snakemake-tutorial-data)
in your browser. GitPod provides you with a [Theia development
environment](https://theia-ide.org/docs), which you can learn about in
the linked documentation. Once you have a basic understanding of this
environment, you can go on directy with
`tutorial-basics`.

## Running the tutorial on your local machine

If you prefer to run the tutorial on your local machine, please follow
the steps below.

The easiest way to set these prerequisites up, is to use the
[Mambaforge](https://github.com/conda-forge/miniforge#mambaforge) Python
3 distribution
([Mambaforge](https://github.com/conda-forge/miniforge#mambaforge) is a
Conda based distribution like
[Miniconda](https://conda.pydata.org/miniconda.html), which however uses
[Mamba](https://github.com/mamba-org/mamba) a fast and more robust
replacement for the [Conda](https://conda.pydata.org) package manager).
The tutorial assumes that you are using either Linux or MacOS X. Both
Snakemake and
[Mambaforge](https://github.com/conda-forge/miniforge#mambaforge) work
also under Windows, but the Windows shell is too different to be able to
provide generic examples.

## Setup on Windows

If you already use Linux or MacOS X, go on with **Step 1**.

### Windows Subsystem for Linux

If you use Windows 10, you can set up the Windows Subsystem for Linux
([WSL](https://docs.microsoft.com/en-us/windows/wsl/about)) to natively
run linux applications. Install the WSL following the instructions in
the [WSL
Documentation](https://docs.microsoft.com/en-us/windows/wsl/install-win10).
You can chose any Linux distribution available for the WSL, but the most
popular and accessible one is Ubuntu. Start the WSL and set up your
account; now, you can follow the steps of our tutorial from within your
Linux environment in the WSL.

### Vagrant virtual machine

If you are using a version of Windows older than 10 or if you do not
wish to install the WSL, you can instead setup a Linux virtual machine
(VM) with [Vagrant](https://www.vagrantup.com). First, install Vagrant
following the installation instructions in the [Vagrant
Documentation](https://docs.vagrantup.com). Then, create a new directory
you want to share with your Linux VM, for example, create a folder named
`vagrant-linux` somewhere. Open a command line prompt, and change into
that directory. Here, you create a 64-bit Ubuntu Linux environment with

```shell
> vagrant init hashicorp/precise64
> vagrant up
```

If you decide to use a 32-bit image, you will need to download the
32-bit version of Miniconda in the next step. The contents of the
`vagrant-linux` folder will be shared with the virtual machine that is
set up by vagrant. You can log into the virtual machine via

```shell
> vagrant ssh
```

If this command tells you to install an SSH client, you can follow the
instructions in this
[Blogpost](https://blog.osteel.me/posts/2015/01/25/how-to-use-vagrant-on-windows.html).
Now, you can follow the steps of our tutorial from within your Linux VM.

## Step 1: Installing Mambaforge

First, please **open a terminal** or make sure you are logged into your
Vagrant Linux VM. Assuming that you have a 64-bit system, on Linux,
download and install Miniconda 3 with

```shell
curl -L https://github.com/conda-forge/miniforge/releases/latest/download/Mambaforge-Linux-x86_64.sh -o Mambaforge-Linux-x86_64.sh
bash Mambaforge-Linux-x86_64.sh
```

On MacOS with x86_64 architecture, download and install with

```shell
curl -L https://github.com/conda-forge/miniforge/releases/latest/download/Mambaforge-MacOSX-x86_64.sh -o Mambaforge-MacOSX-x86_64.sh
bash Mambaforge-MacOSX-x86_64.sh
```

On MacOS with ARM/M1 architecture, download and install with

```shell
curl -L https://github.com/conda-forge/miniforge/releases/latest/download/Mambaforge-MacOSX-arm64.sh -o Mambaforge-MacOSX-arm64.sh
bash Mambaforge-MacOSX-arm64.sh
```

When you are asked the question

```text
Do you wish the installer to prepend the install location to PATH ...? [yes|no]
```

answer with **yes**. Along with a minimal Python 3 environment, Mambaforge contains the package manager [Mamba](https://github.com/mamba-org/mamba).
After closing your current terminal and opening a **new terminal**, you can use the new `conda` command to install software packages and create isolated environments to, for example, use different versions of the same package.
We will later use [Conda](https://conda.pydata.org) to create an isolated environment with all the required software for this tutorial.

## Step 2: Preparing a working directory

First, **create a new directory** `snakemake-tutorial` at a **place you can easily remember** and change into that directory in your terminal:

```shell
mkdir snakemake-tutorial
cd snakemake-tutorial
```

If you use a Vagrant Linux VM from Windows as described above, create
that directory under `/vagrant/`, so that the contents are shared with
your host system (you can then edit all files from within Windows with
an editor that supports Unix line breaks). Then, **change to the newly
created directory**. In this directory, we will later create an example
workflow that illustrates the Snakemake syntax and execution
environment. First, we download some example data on which the workflow
shall be executed:

```shell
curl -L https://api.github.com/repos/snakemake/snakemake-tutorial-data/tarball -o snakemake-tutorial-data.tar.gz
```

Next we extract the data. On Linux, run

```shell
tar --wildcards -xf snakemake-tutorial-data.tar.gz --strip 1 "*/data" "*/environment.yaml"
```

On MacOS, run

```shell
tar -xf snakemake-tutorial-data.tar.gz --strip 1 "*/data" "*/environment.yaml"
```

This will create a folder `data` and a file `environment.yaml` in the
working directory.

## Step 3: Creating an environment with the required software

First, make sure to activate the conda base environment with

```shell
conda activate base
```

The `environment.yaml` file that you have obtained with the previous
step (Step 2) can be used to install all required software into an
isolated Conda environment with the name `snakemake-tutorial` via

```shell
mamba env create --name snakemake-tutorial --file environment.yaml
```

:::callout

Not all of the required software used in this tutorial are supplied
with native support for newer chipsets, such as the Apple M-Series hardware.
In this case we can force conda/mamba to create a virtual environment that
corresponds to another chipset by prepending `CONDA_SUBDIR=osx-64` (for x64)
or `CONDA_SUBDIR=osx-arm64` (for arm64) to the `mamba create` command, like so:

```shell
CONDA_SUBDIR=osx-64 mamba env create --name snakemake-tutorial --file environment.yaml
```

:::

If you don\'t have the [Mamba](https://github.com/mamba-org/mamba)
command because you used a different conda distribution than
[Mambaforge](https://github.com/conda-forge/miniforge#mambaforge), you
can also first install [Mamba](https://github.com/mamba-org/mamba)
(which is a faster and more robust replacement for
[Conda](https://conda.pydata.org)) in your base environment with

```shell
conda install -n base -c conda-forge mamba
```

and then run the `mamba env create` command shown above.

## Step 4: Activating the environment

To activate the `snakemake-tutorial` environment, execute

```shell
conda activate snakemake-tutorial
```

Now you can use the installed tools. Execute

```shell
snakemake --help
```

to test this and get information about the command-line interface of
Snakemake.

To exit the environment, you can execute the following command (but **don\'t do
that now**, since we finally want to start working with Snakemake :-)).

```shell
conda deactivate
```
