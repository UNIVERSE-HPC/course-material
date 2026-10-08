---
name: The Bash shell
id: bash
dependsOn: [
]
files: [
    01-intro.md,
    02-filedir.md,
    03-create.md,
    04-pipefilter.md,
    05-script.md,
    06-loop.md
]
learningOutcomes:
    - Navigate the file system using absolute and relative paths.
    - Modify shell command behavior with options and arguments.
    - Create and manage directory hierarchies.
    - Combine commands to generate new outputs.
    - Redirect command outputs to files.
    - Write and execute shell scripts.
summary: |
    The Bash shell is a command-line interface used in Unix-based operating systems
    such as Linux and macOS. This course will introduce you to the basics of using
    the Bash shell.
attribution:
    - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
      url: https://www.universe-hpc.ac.uk
      image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
      license: CC-BY-4.0


---


The Bash shell is a command-line interface typically used in Unix-based
operating systems such as Linux and macOS, but is also available for Windows.
It provides a way for users to interact with the operating system using
text-based commands instead of a graphical user interface.

:::callout{variant="note"}

Since macOS 10.15 (released in Oct 2019), [the default shell becomes
`zsh`](https://support.apple.com/en-ca/102360) which is not the same as Bash.
They are highly compatibile and you will be able to follow the materials of
this course by using `zsh`. Simply type `bash` followed by 'Enter' if you want
to use the Bash shell.

:::

Some basic commands you might use in Bash include:

- `ls`: Lists the files and directories in the current directory.
- `cd`: Changes the current directory to a specified directory.
- `mkdir`: Creates a new directory.
- `rm`: Deletes a file or directory.
- `echo`: Prints a message to the terminal.

In addition to these basic commands, you can use Bash to write scripts, which
are collections of commands that can be executed together. Bash scripts can be
used for tasks such as automating repetitive tasks, managing files, and running
programs.

Learning Bash can take some time, but it's a powerful tool for working with
Unix-based operating systems. There are many online resources available to help
you learn, including tutorials, books, and online communities.
