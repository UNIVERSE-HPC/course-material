---
name: Transferring Data
dependsOn: [high_performance_computing.hpc_intro.05_modules]
tags: []
learningOutcomes:
  - Identify the connection details, remote storage path and local policy required for a transfer.
  - Transfer files and directories to and from a cluster using `scp`.
  - Use `rsync` to preview and apply incremental directory updates.
  - Select an appropriate approach for repeated transfers, large datasets and collections containing many small files.
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

Computational work normally begins with input files and ends with results that must be retained, analysed or shared.
Those files may need to move between your local computer, active storage on a cluster, a data archive, another computing service or a collaborator.

The appropriate transfer method depends on the volume and number of files, whether an earlier transfer must be updated, the available network route, the sensitivity of the data and the services provided by the cluster.

We will first demonstrate how to use `scp` (secure copy) for straightforward copies between a local computer and a cluster.
Then we will provide a detailed introduction to `rsync`, a more advanced tool for synchronising remote and local directories and how to use either it or `tar` to package many small files into a single archive for transfer.
Finally, we will provide a very brief overview of some of the other data transfer methods you may encounter.

## Preparing the Transfer

A file-transfer command needs an unambiguous source, destination as well as a route between them.
Before you transfer any data, obtain the following information from your cluster's documentation:

1. The hostname or SSH alias intended for file transfer.
1. Any required institutional network, VPN, gateway or other route.
1. The authentication procedure used by transfer connections.
1. The complete path to the destination storage space.
1. Any dedicated transfer nodes, managed transfer services or restrictions on sustained transfers.
1. Any security or data-governance rules that apply to the material.

Some services simply use the interactive login address for transfers, while others provide a separate endpoint.
Similarly, a transfer endpoint may expose project storage without exposing the remote home directory.

:::callout{variant="note"}

## Use the Documented Transfer Route

[Oxford ARC](https://arc-user-guide.readthedocs.io/en/latest/arc-copying-data.html), [ARCHER2](https://docs.archer2.ac.uk/user-guide/data/) and [Isambard](https://docs.isambard.ac.uk/user-documentation/guides/file_transfer/local/) each document the endpoints and tools appropriate to their current systems.
Use the instructions for the cluster rather than substituting a familiar login hostname or starting a long transfer on an arbitrary node.

An SSH alias configured in [Connecting to an HPC Cluster](high_performance_computing/hpc_intro/02_connecting) can include a username, identity file and gateway settings.
Commands such as `scp` and `rsync` can then use that alias instead of repeating those connection details.

:::

The [storage section](high_performance_computing/hpc_intro/03_storage) asked you to prepare a suitable working directory for this course.
Return to that directory on the cluster and use `pwd` to obtain its complete path:

```bash
remote$ cd /path/selected/earlier/hpc-intro
remote$ pwd
/remote/path/to/hpc-intro
```

Record the absolute path reported by `pwd`.
Using an absolute destination path makes it clear which remote storage space will receive the data.

## Copy Files and Directories with `scp`

The [`scp` command](https://man.openbsd.org/scp) copies files over an SSH connection and uses the same authentication and SSH configuration as a remote shell.
It takes two positional arguments: the source path followed by the destination path.

Each argument can identify either a local or remote path.
A remote path uses the form `username@hostname:path`.
The colon separates the remote host from the path on that host.

To upload `input.txt` from the current local directory to the course working directory, run:

```bash
local$ scp input.txt username@login.example.ac.uk:/remote/path/to/hpc-intro/
```

Replace the username, hostname and path with appropriate values for your cluster and working directory.
If you have configured an SSH alias, e.g. `my-cluster`, the equivalent destination begins `my-cluster:`.

To retrieve `result.txt` from the working directory, reverse the remote and local arguments:

```bash
local$ mkdir -p retrieved-results
local$ scp username@login.example.ac.uk:/remote/path/to/hpc-intro/result.txt retrieved-results/
```

These commands are run from the local computer.
`scp` copies from the path in its first argument to the path in its second.
Using a local source and remote destination uploads a file, while reversing them downloads it.

:::callout{variant="warning"}

## Check Both Paths Before Copying

Without the colon, `username@login.example.ac.uk` is not a remote destination and may be interpreted as an ordinary local filename.
An incorrect remote path can place data in the wrong storage space, while an existing destination file may be replaced without confirmation.

Check the local working directory, the remote hostname and the complete remote path before starting the command.

:::

::::challenge{id=transfer-round-trip title="Transfer an Input and Retrieve a Result"}

Create a small text file on your local computer and upload it to the course working directory.
Connect to the cluster, confirm that the uploaded content is present, transform it into a second file and then retrieve that result to your local computer.

Use the transfer endpoint and authentication route documented by the service rather than assuming that the example hostname is valid.

:::solution

First create an input on the local computer and upload it:

```bash
local$ printf 'transfer test\n' > transfer-input.txt
local$ scp transfer-input.txt username@login.example.ac.uk:/remote/path/to/hpc-intro/
```

In a remote shell, enter the same directory, inspect the input and create a result:

```bash
remote$ cd /remote/path/to/hpc-intro
remote$ cat transfer-input.txt
transfer test
remote$ tr '[:lower:]' '[:upper:]' < transfer-input.txt > transfer-result.txt
```

Retrieve the result from the local computer:

```bash
local$ scp username@login.example.ac.uk:/remote/path/to/hpc-intro/transfer-result.txt ./
local$ cat transfer-result.txt
TRANSFER TEST
```

A successful exercise demonstrates both directions of transfer and leaves both files available on the local computer and the cluster for inspection.

:::
::::

`scp` can also copy a complete directory tree using its `-r` option:

```bash
local$ scp -r local-project username@login.example.ac.uk:/remote/path/to/hpc-intro/
```

The option copies the named directory recursively, including its files and subdirectories.
This is convenient for a one-off copy, but repeating the command copies every file again and cannot resume an interrupted transfer.
For directories that will be updated or transferred repeatedly, `rsync` provides more control.

## Update a Directory with `rsync`

When only a few files have changed, copying an entire directory again transfers data that is already present at the destination.
[`rsync`](https://rsync.samba.org/) compares the source and destination so that it can transfer the required updates.

This part of the section requires `rsync` on both the local computer and the cluster.
Check that it is available at each end before beginning the exercise:

```bash
local$ rsync --version
remote$ rsync --version
```

If either command is unavailable, consult the cluster documentation for its supported directory-transfer method and use the remaining examples to understand the `rsync` workflow.

The following command updates a remote directory from a local one:

```bash
local$ rsync -aP local-project/ username@login.example.ac.uk:/remote/path/to/hpc-intro/local-project/
```

This command uses two options:
The `-a` option transfers the directory in archive mode, preserving common metadata such as timestamps, permissions and symbolic links where the destination permits this.
The `-P` option displays progress and retains partially transferred files so that a later run can continue the transfer more efficiently.

The trailing slash on the source `local-project/` is significant.
It means “copy the contents of this directory”; omitting it means “copy the directory itself”, which introduces an additional directory level at the destination.

Before updating valuable data, ask `rsync` to perform a dry run and produce an itemised list of proposed changes:

```bash
local$ rsync -ai --dry-run local-project/ username@login.example.ac.uk:/remote/path/to/hpc-intro/local-project/
```

The dry run transfers no file data.
Review its source, destination and proposed changes before running the intended transfer command without `--dry-run`.

:::callout{variant="warning"}

## Synchronisation Can Replace Data

`rsync` may replace destination files whose source versions have changed.
It does not normally remove unrelated destination files, but options such as `--delete` enable that behaviour.

Do not add deletion options copied from another command until you understand which path is authoritative and have reviewed a dry run.

:::

::::challenge{id=update-transfer title="Update a Transferred Directory"}

Create a local directory containing at least two files.
Use an `rsync` dry run to inspect the initial transfer, perform it and confirm that the files reached the course working directory.

Modify one local file, repeat the dry run and identify which item `rsync` proposes to update.
Run the transfer again and confirm that the remote copy contains the new content.

:::solution

The following commands create a small example directory:

```bash
local$ mkdir -p transfer-project
local$ printf 'configuration=initial\n' > transfer-project/config.txt
local$ printf '1\n2\n3\n' > transfer-project/data.txt
```

Inspect and then perform the first transfer:

```bash
local$ rsync -ai --dry-run transfer-project/ username@login.example.ac.uk:/remote/path/to/hpc-intro/transfer-project/
local$ rsync -aP transfer-project/ username@login.example.ac.uk:/remote/path/to/hpc-intro/transfer-project/
```

Change one file and inspect the proposed update:

```bash
local$ printf 'configuration=updated-value\n' > transfer-project/config.txt
local$ rsync -ai --dry-run transfer-project/ username@login.example.ac.uk:/remote/path/to/hpc-intro/transfer-project/
local$ rsync -aP transfer-project/ username@login.example.ac.uk:/remote/path/to/hpc-intro/transfer-project/
```

The second dry run should identify `config.txt` without proposing another transfer of the unchanged `data.txt`.
Confirm the result from a remote shell:

```bash
remote$ cat /remote/path/to/hpc-intro/transfer-project/config.txt
configuration=updated-value
```

:::
::::

## Package Collections of Small Files

Transferring many small files can be much slower than transferring the same number of bytes in a few large files.
Each file requires separate filesystem metadata operations and transfer setup at both ends.

`tar` is not a transfer tool, but it can package a directory that no longer needs file-by-file updates into a single archive file:

```bash
remote$ tar -cf results.tar results/
remote$ tar -tf results.tar
```

The first command creates the archive and the second lists its contents without extracting them.
After transferring the archive with the selected transfer tool, inspect it and extract it into a new directory:

```bash
local$ mkdir -p extracted-results
local$ tar -tf results.tar
local$ tar -xf results.tar -C extracted-results
```

Inspecting an unfamiliar archive before extraction reveals its paths, while a separate destination reduces the risk of overwriting existing files.

### Compression

Archiving and compression are separate operations but both can be done simultaneously with `tar`.
The `-z` option creates a gzip-compressed archive, conventionally named with a `.tar.gz` suffix:

```bash
remote$ tar -czf results.tar.gz results/
```

Compression can reduce transfer time for suitable data, but it consumes processor time and may provide little benefit for formats that are already compressed, including many image, video and scientific-data formats.
An archive also prevents `rsync` from updating its constituent files individually, so packaging is a trade-off rather than a universal optimisation.

## Other Transfer Methods

This course practises `scp` for straightforward copies and `rsync` for repeatedly updating directory trees because together they cover many ordinary cluster workflows.
Other mechanisms solve different problems, and it is useful to recognise their roles without learning every tool at once, it is possible you may encounter some or more of these options at some point:

| Category | Examples | Role |
| --- | --- | --- |
| Straightforward SSH copies | `scp` | Copies files between named paths using the established SSH route. |
| Interactive or graphical SSH transfer | `sftp` and service-supported SFTP clients | Allows users to browse remote directories and select files while retaining SSH authentication. |
| Incremental directory transfer | `rsync` | Updates directory trees and retains partial transfers, but must be available at both endpoints. |
| Web or source-code retrieval | `curl`, `wget` and Git | Retrieves material from a public service when the cluster permits outbound access. |
| Managed service-to-service transfer | Globus and institutional transfer services | Coordinates long-running transfers between supported endpoints and may provide automatic retry and integrity checking. |
| Cloud or object-storage transfer | `rclone` and provider-specific command-line tools | Moves data to or from services that do not appear as ordinary SSH-accessible filesystems. |
| Mounted or locally integrated storage | `cp` and service-specific tools | Copies data when another storage system is exposed directly within the cluster's filesystem namespace. |

`tar` prepares a collection for another transfer tool, while a dedicated transfer node provides an appropriate place to run a tool; neither is itself a transfer protocol.
There is no universal data-size threshold at which one category becomes correct, so use the characteristics of the data and the services available at both endpoints.

Large transfers may consume substantial network, processor and filesystem capacity.
It might be necessary to use a dedicated transfer node or schedule the transfer during off-peak hours rather thanrunning sustained transfers or creating large archives on a shared login node (unless the service permits it).
Some clusters provide data-transfer nodes, analysis nodes, managed services or scheduled transfer jobs for this purpose.

:::callout{variant="note"}

## A Public URL Does Not Guarantee Cluster Access

A dataset or source repository that is reachable from your local computer may not be reachable from a cluster.
Services can restrict outbound network access, and compute nodes commonly have different connectivity from login or data-transfer nodes.

When direct download is not supported, retrieve the material on an approved system and transfer it using the documented route.

:::

## Verify Before Removing the Source

A successful command does not prove that every intended file reached the correct destination.
Before removing the source, inspect the destination and compare file counts, sizes or checksums as appropriate.
Keep a durable copy of irreplaceable inputs and results in storage that meets the backup, retention and data-governance requirements described in the [storage section](high_performance_computing/hpc_intro/03_storage).

The preceding sections have introduced each part of a cluster workflow separately.
[Running an Image-Sharpening Application](high_performance_computing/hpc_intro/07_image_sharpening) now brings them together by obtaining and compiling an application, running it through Slurm and examining the result on your own computer.
