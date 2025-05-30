---
name: How to setup a minimal Slurm cluster
dependsOn: []
tags: []
attribution: 
    - citation: >
        "SLURM Multinode on AWS" tutorial by OpenFlightHPC
      url: https://www.openflighthpc.org/latest/docs/flight-solo/cluster-build-methods/slurm-multinode-aws/
      image: https://www.openflighthpc.org/latest/assets/images/openflighthpc_grey.svg
      license: CC-BY-4.0
---

:::callout{variant="note"}
This is not a requirement for the High Performance Computing theme. It is a
tutorial for teachers & course runners to setup a training environment - if that
is not relevant to you then please ignore!
:::

The following is a tutorial for setting up a minimal Slurm cluster, using AWS
and the Flight Solo image from OpenFightHPC, for trainees to use in the rest of
the High Performance Computing (HPC) theme. In particular this is aimed at
getting trainees an environment for running the [Intro to HPC][hpcintro] course
and should be **followed by the trainers** before the course is taught if the
trainees have no other access to a HPC environment on which to run simple
commands.

For this task we will be using [Flight Solo][flightsolopage], an open source
image for setting up a HPC environment for research and scientific computing,
including SLURM and HPC package management using `spack` (among other things).
We will be setting up a minimal training cluster consisting of:

- 1 login node
- 2 compute nodes

with `SLURM` and `OpenMPI` set up to run jobs on the 2 compute nodes. As we will
not be using this for serious computation, these can all run on pretty small
machines within Amazon's EC2 System – in our case we opted for `T3.medium` as it
was the smallest available in our region.

## Spinning up the Nodes

Fortunately, Flight Solo comes with good tutorials for setting up the image/images
on AWS (and other cloud platforms) with detailed, step-by-step instructions for
getting the machines spun up. Therefore, the first advice is to just follow the [tutorial instructions](flightsolotutorial).

If that works, **great!** You can carry on to the next step: [setting up pacakges with `spack`](#spack-and-modules).
However if, like for us, this did not work first time, you can try the following
modified steps. During the first stage, i.e. "Launch Login Node", we instead did
this via the EC2 panes (see step `f` in "Launch Compute Nodes") and added in the
following yaml config under `advanced-details.user-data`:

``` yaml
#cloud-config
write_files:
  - content: |
      SHAREPUBKEY=true
    path: /opt/flight/cloudinit.in
    permissions: '0600'
    owner: root:root
```

This is basically just configuring Flight Solo by setting `SHAREPUBKEY` to true
in a config , which shares out the public key from the login key-pair to the
compute nodes and allows them to be found and configured by Flight. Note that we
also created the login node with additional storage space (30GB, rather than
10GB) as we were struggling with getting big packages installed otherwise.

At this point we should be able to ssh into the login node with the key pair
generated on the AWS interface and the public ip address of the login node EC2
instance, i.e.

```shell
ssh -i path/to/keyfile.pem flight@$PUBLIC_IP_ADDRESS
```

which should bring you to a login page where you can start configuring the
cluster with Flight.

You can then follow the instructions the rest of the way, including leaving most
of the config set up during `flight profile configure` as default. What we did
specifically:

- `Cluster type`: Openflight Slurm Multinode
- `Cluster name`: slurm-multinode (you can do what you want here)
- `Setup Multi User Environment with IPA?`: none
- `Local user login`: flight
- `Set local user password to`: (we left this as default)
- `IP or FQDN for Web Access`: ec2-13-43-90-160.eu-west-2.compute.amazonaws.com (left as default)
- `IP Range of Compute Nodes`: 172.31.32.0/20

Note that the `IP or FQDN for Web Access` was left as default as we didn't try
configuring Flight's web-interface, the IP range of the compute nodes was
calculated automatically so there was no need to change it, and the password was
left as default but changed after successfully applying profiles.

Profiles were then applied as specified in the [Flight Tutorial][flightsolotutorialslurm].

## Spack and Modules

Flight has [some environments available][flightenv] for installing system-level
packages, we opted for `spack` for no particular reason other than it is
well-regarded, and one of our requirements is for the cluster to have modules,
which is easily achievable with `spack`. First though you have to create a
global `spack` flight-environment with

``` bash
flight env create -g spack
```

We recommend `-g` (global) so every other user can access the installed modules
but not install their own. This will need to be done as the root user though,
which you can escalate to while logged in as the user `flight` with `sudo -s`.
Once finished, you can then activate the `spack` environment with:

``` bash
flight env activate spack
```

after which your regular `spack` commands should work. More info on the `spack`
flight-environment can be found in the [flight docs][flightenvspack].

To get the module files working you can [follow the instructions][spackmodules]
on the `spack` docs, but to summarise what we did:

1. Enable tcl module files

    ``` bash
    spack config add "modules:default:enable:[tcl]" 
    ```

2. Install `lmod` with `spack`

    ``` bash
    spack install lmod   
    ```

3. Make the module tool available to the current shell

    ``` bash
    . $(spack location -i kmod)/lmod/lmod/init/bash
    ```

4. Install and add a new compiler

    ``` bash
    spack install gcc@12.3.0
    spack compiler add
    ```

5. Install any new modules with this new compiler

    ``` bash
    spack install {module_name} %gcc@12.3.0
    ```

The compiler part isn't strictly necessary, so you can ignore if you like, but
it does make formatting the module list a bit more straightforward so we still
recommend it. We also found that the gcc@11 that came pre-installed on
flight didn't have fortran compilers installed so a fresh compiler install was
necessary for MPI, though your mileage may vary.

### Formatting the module list

This then gives us a large list of all of the dependencies `spack` downloaded
and installed for each of these newly installed modules, which we can leave be
if you like, or we can configure down to a nice, minimalist list. To get around
this we added a config file, following the advice of the aforementioned
[tutorial][spackmodules], to `$SPACK_ROOT/etc/spack/modules.yaml` containing the
following:

``` yaml
modules:
  default:
    tcl:
      hash_length: 0
      include:
      - gcc
      {PACKAGE_LIST}
      exclude:
      - '%gcc@11'
      - '%gcc@12'
      all:
        conflict:        
        - '{name}'        
      projections:        
        all: '{name}/{version}'        
```

Where you'll need to replace `{PACKAGE_LIST}` with the yaml-formatted list of
packages you specifically want to include (to see the full list of packages
`spack` has installed, simply run `spack find`). After creating/editing this
file you'll have to run

``` bash
spack module tcl refresh --delete-tree -y
```

for the changes to be reflected in the list of available modules. This will only
show specific packages (and dependencies) you installed with `spack` and
specified in the include section. This can be a little limiting if you're
installing new packages frequently, so the `{list of packages}` and the
`"%gcc@12"` can be removed for all of the `spack` packages and dependencies to
be included on the module avail command.

#### Some sysadmin

The above approach doesn't persist after leaving the shell instance, so we put
the following into `/etc/profile`:

``` bash
flight env activate spack 
. $(spack location -i lmod)/lmod/lmod/init/bash
flight env deactivate
```

which leave the paths in place for any user to be able to call module commands
(e.g. `module avail`) but not install new `spack` packages. You might be able to
copy the output of `$(spack location -i lmod)` and hardcode it to avoid having
to activate the flight environment.

One final bit of sys-admin involved activating password authentication for sshd
so that users could login with a password and then add their own ssh file, as
per the [course][hpcintro]. This just involves uncommenting the line
`PasswordAuthentication yes` in `/etc/ssh/sshd_config`, removing any overriding
references to this option in `/etc/ssh/sshd_config.d`, and then restarting the
`sshd` service with

``` bash
sudo systemctl restart sshd
```

after which you should be able to log in to the login node with just a password.

## OpenMPI Installation

The training material also requires the use of `srun`, `mpirun`, and `mpiexec`,
for which some installation of MPI is required. We went for OpenMPI, and was
required to install it with

``` bash
spack install openmpi ~legacylaunchers schedulers=slurm
```

Where `schedulers=slurm` is telling it to compile with slurm compatibility and
`~legacylaunchers` is telling it not to delete the `mpirun` and `mpiexec`
binaries. There are [good reasons to delete][openmpiissue] them for a proper,
production install, but for our training purposes having them is preferable.

## Summary

At this point we should now have a working SLURM cluster on AWS which we can ssh
into, submit jobs on with `sbatch`, and generally treat like a proper HPC
environment. Feel free at this point to take it for a spin – we ran through the
[HPC introduction course][hpcintro] but you may wish to try something a bit more
involved.

[hpcintro]: ../hpc_intro
[flightsolopage]: https://www.openflighthpc.org/latest/solo/
[flightsolotutorialslurm]: https://www.openflighthpc.org/latest/docs/flight-solo/cluster-build-methods/slurm-multinode-aws#slurm-multinode-configuration
[flightenv]: https://www.openflighthpc.org/latest/docs/flight-environment/use-flight/flight-user-suite/flight-env/usage/
[flightenvspack]: https://www.openflighthpc.org/latest/docs/flight-environment/use-flight/flight-user-suite/flight-env/ecosystems/spack/
[spackmodules]: https://spack-tutorial.readthedocs.io/en/latest/tutorial_modules.html
[openmpiissue]: https://github.com/spack/spack/pull/10340#issuecomment-454355612
