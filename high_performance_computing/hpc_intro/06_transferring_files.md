---
name: Transferring Files with Remote Computers
dependsOn: [
  high_performance_computing.hpc_intro.05_modules
]
tags: []
learningOutcomes:
  - Transfer files to and from a computing cluster.
attribution: 
    - citation: >
        "Introduction to High-Performance Computing" course by the HPC-carpentries
      url: https://carpentries-incubator.github.io/hpc-intro/
      image: https://carpentries-incubator.github.io/hpc-intro/assets/img/incubator-logo-blue.svg
      license: CC-BY-4.0
---

Performing work on a remote computer is not very useful if we cannot get files
to or from the cluster. There are several options for transferring data between
computing resources using CLI and GUI utilities, a few of which we will cover.

## Download Lesson Files From the Internet

One of the most straightforward ways to download files is to use either `curl`
or `wget`. One of these is usually installed in most Linux shells, on Mac OS
terminal and in GitBash. Any file that can be downloaded in your web browser
through a direct link can be downloaded using `curl` or `wget`. This is a
quick way to download datasets or source code. The syntax for these commands is

* `wget [-O new_name] https://some/link/to/a/file`
* `curl [-o new_name] https://some/link/to/a/file`

Try it out by downloading some material we'll use later on, from a terminal on
your local machine, using the URL of the current codebase:

<https://github.com/hpc-carpentry/amdahl/tarball/main>

::::challenge{id=download title="Download the Tarball"}

The word "tarball" in the above URL refers to a compressed archive format
commonly used on Linux, which is the operating system the majority of HPC
cluster machines run.
A tarball is a lot like a `.zip` file.
The actual file extension is `.tar.gz`, which reflects the two-stage process
used to create the file:
the files or folders are merged into a single file using `tar`, which is then
compressed using `gzip`, so the file extension is "tar-dot-g-z."
That's a mouthful, so people often say "the _xyz_ tarball" instead.

You may also see the extension `.tgz`, which is just an abbreviation of
`.tar.gz`.

By default, `curl` and `wget` download files to the same name as the URL:
in this case, `main`.
Use one of the above commands to save the tarball as `amdahl.tar.gz`.

:::solution

## `wget` and `curl` Commands

```bash
local$ wget -O amdahl.tar.gz https://github.com/hpc-carpentry/amdahl/tarball/main
# or
local$ curl -o amdahl.tar.gz https://github.com/hpc-carpentry/amdahl/tarball/main
```

:::
::::

After downloading the file, use `ls` to see it in your working directory:

```bash
local$ ls
```

## Archiving Files

One of the biggest challenges we often face when transferring data between
remote HPC systems is that of large numbers of files. There is an overhead to
transferring each individual file and when we are transferring large numbers of
files these overheads combine to slow down our transfers to a large degree.

The solution to this problem is to _archive_ multiple files into smaller
numbers of larger files before we transfer the data to improve our transfer
efficiency.
Sometimes we will combine archiving with _compression_ to reduce the amount of
data we have to transfer and so speed up the transfer.
The most common archiving command you will use on a (Linux) HPC cluster is
`tar`.

`tar` can be used to combine files and folders into a single archive file and,
optionally, compress the result.
Let's look at the file we downloaded from the lesson site, `amdahl.tar.gz`.

The `.gz` part stands for _gzip_, which is a compression library.
It's common (but not necessary!) that this kind of file can be interpreted by
reading its name: it appears somebody took files and folders relating to
something called "amdahl," wrapped them all up into a single file with `tar`,
then compressed that archive with `gzip` to save space.

Let's see if that is the case, _without_ unpacking the file.
`tar` prints the "**t**able of contents" with the `-t` flag, for the file
specified with the `-f` flag followed by the filename.
Note that you can concatenate the two flags: writing `-t -f` is interchangeable
with writing `-tf` together.
However, the argument following `-f` must be a filename, so writing `-ft` will
_not_ work.

```bash
local$ tar -tf amdahl.tar.gz
hpc-carpentry-amdahl-46c9b4b/
hpc-carpentry-amdahl-46c9b4b/.github/
hpc-carpentry-amdahl-46c9b4b/.github/workflows/
hpc-carpentry-amdahl-46c9b4b/.github/workflows/python-publish.yml
hpc-carpentry-amdahl-46c9b4b/.gitignore
hpc-carpentry-amdahl-46c9b4b/LICENSE
hpc-carpentry-amdahl-46c9b4b/README.md
hpc-carpentry-amdahl-46c9b4b/amdahl/
hpc-carpentry-amdahl-46c9b4b/amdahl/__init__.py
hpc-carpentry-amdahl-46c9b4b/amdahl/__main__.py
hpc-carpentry-amdahl-46c9b4b/amdahl/amdahl.py
hpc-carpentry-amdahl-46c9b4b/requirements.txt
hpc-carpentry-amdahl-46c9b4b/setup.py
```

This example output shows a folder which contains a few files, where `46c9b4b`
is an 8-character [git][git-swc] commit hash that will change when the source
material is updated.

Now let's unpack the archive. We'll run `tar` with a few common flags:

* `-x` to e**x**tract the archive
* `-v` for **v**erbose output
* `-z` for g**z**ip compression
* `-f «tarball»` for the file to be unpacked

::::challenge{id=extract title="Extract the Archive"}

Using the flags above, unpack the source code tarball into a new
directory named "amdahl" using `tar`.

:::solution

```bash
local$ tar -xvzf amdahl.tar.gz
```

```text
hpc-carpentry-amdahl-46c9b4b/
hpc-carpentry-amdahl-46c9b4b/.github/
hpc-carpentry-amdahl-46c9b4b/.github/workflows/
hpc-carpentry-amdahl-46c9b4b/.github/workflows/python-publish.yml
hpc-carpentry-amdahl-46c9b4b/.gitignore
hpc-carpentry-amdahl-46c9b4b/LICENSE
hpc-carpentry-amdahl-46c9b4b/README.md
hpc-carpentry-amdahl-46c9b4b/amdahl/
hpc-carpentry-amdahl-46c9b4b/amdahl/__init__.py
hpc-carpentry-amdahl-46c9b4b/amdahl/__main__.py
hpc-carpentry-amdahl-46c9b4b/amdahl/amdahl.py
hpc-carpentry-amdahl-46c9b4b/requirements.txt
hpc-carpentry-amdahl-46c9b4b/setup.py
```

Note that we did not need to type out `-x -v -z -f`, thanks to flag
concatenation, though the command works identically either way -
so long as the concatenated list ends with `f`, because the next string
must specify the name of the file to extract.
:::
::::

The folder has an unfortunate name, so let's change that to something more
convenient.

```bash
local$ mv hpc-carpentry-amdahl-46c9b4b amdahl
```

Check the size of the extracted directory and compare to the compressed
file size, using `du` for "**d**isk **u**sage".

```bash
local$ du -sh amdahl.tar.gz
8.0K     amdahl.tar.gz
local$ du -sh amdahl
48K    amdahl
```

Text files (including Python source code) compress nicely:
the "tarball" is one-sixth the total size of the raw data!

If you want to reverse the process - compressing raw data instead of
extracting it - set a `c` flag instead of `x`, set the archive filename,
then provide a directory to compress:

```bash
local$ tar -cvzf compressed_code.tar.gz amdahl
```

```text
amdahl/
amdahl/.github/
amdahl/.github/workflows/
amdahl/.github/workflows/python-publish.yml
amdahl/.gitignore
amdahl/LICENSE
amdahl/README.md
amdahl/amdahl/
amdahl/amdahl/__init__.py
amdahl/amdahl/__main__.py
amdahl/amdahl/amdahl.py
amdahl/requirements.txt
amdahl/setup.py
```

If you give `amdahl.tar.gz` as the filename in the above command, `tar` will
update the existing tarball with any changes you made to the files.
That would mean adding the new `amdahl` folder to the _existing_ folder
(`hpc-carpentry-amdahl-46c9b4b`) inside the tarball, doubling the size of the
archive!

:::callout

## Working with Windows

When you transfer text files from a Windows system to a Unix system (Mac,
Linux, BSD, Solaris, etc.) this can cause problems. Windows encodes its files
slightly different than Unix, and adds an extra character to every line.

On a Unix system, every line in a file ends with a `\n` (newline). On
Windows, every line in a file ends with a `\r\n` (carriage return + newline).
This causes problems sometimes.

Though most modern programming languages and software handles this correctly,
in some rare instances, you may run into an issue. The solution is to convert
a file from Windows to Unix encoding with the `dos2unix` command.

You can identify if a file has Windows line endings with `cat -A filename`. A
file with Windows line endings will have `^M$` at the end of every line. A
file with Unix line endings will have `$` at the end of a line.

To convert the file, just run `dos2unix filename`. (Conversely, to convert
back to Windows format, you can run `unix2dos filename`.)
:::

## Transferring Single Files and Folders With `scp`

To copy a single file to or from the cluster, we can use `scp` ("secure copy").
The syntax can be a little complex for new users, but we'll break it down.
The `scp` command is a relative of the `ssh` command we used to
access the system, and can use the same public-key authentication
mechanism.

To _upload to_ another computer, the template command is

```bash
local$ scp local_file user@cluster.name:remote_destination
```

in which `@` and `:` are field separators and `remote_destination` is a path
relative to your remote home directory, or a new filename if you wish to change
it, or both a relative path _and_ a new filename.
If you don't have a specific folder in mind you can omit the
`remote_destination` and the file will be copied to your home directory on the
remote computer (with its original name).
If you include a `remote_destination`, note that `scp` interprets this the same
way `cp` does when making local copies:
if it exists and is a folder, the file is copied inside the folder; if it
exists and is a file, the file is overwritten with the contents of
`local_file`; if it does not exist, it is assumed to be a destination filename
for `local_file`.

Upload the lesson material to your remote home directory like so:

```bash
local$ scp amdahl.tar.gz user@cluster.name:
```

:::::challenge{id=download-directly, title="Why Not Download on the Cluster Directly?"}

Most computer clusters are protected from the open internet by a _firewall_.
For enhanced security, some are configured to allow traffic _inbound_, but
not _outbound_.
This means that an authenticated user can send a file to a cluster machine,
but a cluster machine cannot retrieve files from a user's machine or the
open Internet.

Try downloading the file directly. Note that it may well fail, and that's
OK!

:::solution

## Commands

```bash
local$ ssh user@cluster.name
remote$ wget -O amdahl.tar.gz https://github.com/hpc-carpentry/amdahl/tarball/main
# or
remote$ curl -o amdahl.tar.gz https://github.com/hpc-carpentry/amdahl/tarball/main
```

:::

Did it work? If not, what does the terminal output tell you about what
happened?
:::::

## Transferring a Directory

To transfer an entire directory, we add the `-r` flag for "**r**ecursive":
copy the item specified, and every item below it, and every item below those...
until it reaches the bottom of the directory tree rooted at the folder name you
provided.

```bash
local$ scp -r amdahl user@cluster.name:
```

:::callout

## Caution

For a large directory  either in size or number of files -
copying with `-r` can take a long time to complete.
:::

When using `scp`, you may have noticed that a `:` _always_ follows the remote
computer name.
A string _after_ the `:` specifies the remote directory you wish to transfer
the file or folder to, including a new name if you wish to rename the remote
material.
If you leave this field blank, `scp` defaults to your home directory and the
name of the local material to be transferred.

On Linux computers, `/` is the separator in file or directory paths.
A path starting with a `/` is called _absolute_, since there can be nothing
above the root `/`.
A path that does not start with `/` is called _relative_, since it is not
anchored to the root.

If you want to upload a file to a location inside your home directory -
which is often the case - then you don't need a _leading_ `/`. After the `:`,
you can type the destination path relative to your home directory.
If your home directory _is_ the destination, you can leave the destination
field blank, or type `~` - the shorthand for your home directory - for
completeness.

With `scp`, a trailing slash on the target directory is optional, and has
no effect. It is important for other commands, like `rsync`.

:::callout

## A Note on `rsync`

As you gain experience with transferring files, you may find the `scp`
command limiting. The [rsync] utility provides
advanced features for file transfer and is typically faster compared to both
`scp` and `sftp` (see below). It is especially useful for transferring large
and/or many files and for synchronizing folder contents between computers.

The syntax is similar to `scp`. To transfer _to_ another computer with
commonly used options:

```bash
local$ rsync -avP amdahl.tar.gz user@cluster.name:
```

The options are:

* `-a` (**a**rchive) to preserve file timestamps, permissions, and folders,
   among other things; implies recursion
* `-v` (**v**erbose) to get verbose output to help monitor the transfer
* `-P` (partial/progress) to preserve partially transferred files in case
  of an interruption and also displays the progress of the transfer.

To recursively copy a directory, we can use the same options:

```bash
local$ rsync -avP amdahl user@cluster.name:~/
```

As written, this will place the local directory and its contents under your
home directory on the remote system. If the trailing slash is omitted on
the destination, a new directory corresponding to the transferred directory
will not be created, and the contents of the source
directory will be copied directly into the destination directory.

To download a file, we simply change the source and destination:

```bash
local$ rsync -avP user@cluster.name:amdahl ./
```

:::

File transfers using both `scp` and `rsync` use SSH to encrypt data sent through
the network. So, if you can connect via SSH, you will be able to transfer
files. By default, SSH uses network port 22. If a custom SSH port is in use,
you will have to specify it using the appropriate flag, often `-p`, `-P`, or
`--port`. Check `--help` or the `man` page if you're unsure.

::::challenge{id=rsync-port title="Change the Rsync Port"}

Say we have to connect `rsync` through port 768 instead of 22. How would we
modify this command?

```bash
local$ rsync amdahl.tar.gz user@cluster.name:
```

_Hint:_ check the `man` page or "help" for `rsync`.

:::solution

```bash
local$ man rsync
local$ rsync --help | grep port
     --port=PORT             specify double-colon alternate port number
See http://rsync.samba.org/ for updates, bug reports, and answers
local$ rsync --port=768 amdahl.tar.gz user@cluster.name:
```

(Note that this command will fail, as the correct port in this case is the
default: 22.)
:::
::::

## Transferring Files Interactively with FileZilla

FileZilla is a cross-platform client for downloading and uploading files to and
from a remote computer. It is absolutely fool-proof and always works quite
well. It uses the `sftp` protocol.

Download and install the FileZilla client from <https://filezilla-project.org>.
After installing and opening the program, you should end up with a window with
a file browser of your local system on the left hand side of the screen. When
you connect to the cluster, your cluster files will appear on the right hand
side.

To connect to the cluster, we'll just need to enter our credentials at the top
of the screen:

* Host: `sftp://cluster.name`
* User: Your cluster username
* Password: Your cluster password
* Port: (leave blank to use the default port)

Hit "Quickconnect" to connect. You should see your remote files appear on the
right hand side of the screen. You can drag-and-drop files between the left
(local) and right (remote) sides of the screen to transfer files.

Finally, if you need to move large files (typically larger than a gigabyte)
from one remote computer to another remote computer, SSH in to the computer
hosting the files and use `scp` or `rsync` to transfer over to the other. This
will be more efficient than using FileZilla (or related applications) that
would copy from the source to your local machine, then to the destination
machine.

[git-swc]: https://swcarpentry.github.io/git-novice/
[rsync]: https://rsync.samba.org/
