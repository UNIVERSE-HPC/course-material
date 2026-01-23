---
name: Practice and personal use
dependsOn: [technology_and_tooling.version_control.09-practice]
tags: [git]
learningOutcomes:
  - Continued use in your own work and projects
  - Good git practice
attribution:
  - citation: >
      This material was originally taken from training materials developed by the
      University of Southampton Research Software Group, which are based on
      the Software Carpentries course "Version Control with Git".
    url: https://github.com/Southampton-RSG-Training/git-novice/
    image: https://southampton-rsg-training.github.io/git-novice/assets/img/home-logo.png
    license: CC-BY-4.0
---

## Day-to-day use

Taking this workshop further into your daily practice, the simplest thing to do is to
immediately start using Git in your projects. For example, if you are working on something
at the moment, run `git init` in the folder and add whichever files are in there (take a
copy first if you are still nervous about losing anything, but you will soon learn to
trust these tools!).

You will likely find on the first commit that many files that don't need to be in stored.
For example, say one of your scripts always generates a file in `out/log.txt`, then you
could add `out/` to a new .gitignore.

:::callout{variant="tip"}
If you are a Windows user and have followed this workshop using Windows Subsystem for
Linux (WSL) as suggested, then bear in mind that the WSL is a sandboxed environment;
today we have set up `git config` and SSH for Ubuntu running in WSL but not for your
personal PC.

To use git directly in Windows, you will have to download the installer from
[https://git-scm.com](https://git-scm.com/downloads/win). The installer provides a tool
called **Git Bash** which is a custom Windows terminal that has unix-like features (for
example in Windows there is no `ls` command, but Git Bash provides this, along with
all basic unix tools like `cat`, `ssh-keygen` and `grep`).

Start a Git Bash terminal and run `git config` as we did during setup and then add a
new SSH key for your personal PC. You will now be able to use Git directly on your
Windows PC.

We have avoided using Git Bash today whilst using the command line interface to get
familiar with remote server usage. You may also find a few subtle differences between
Git Bash and a true Linux/BSD operating system - for example the folder structure
and the line endings on files - and using WSL has helped us bypass these.
:::

## README.md and LICENSE

You find that the majority of Git repositories contain these two files. `README.md`
is a welcome page for new users and `LICENSE` users how they can use the repository.

The `.md` signifies the README is in markdown format. Markdown is regular plain text
that uses certain characters for nicer formatting. For example the header of this
section is written as `## README.md and LICENSE` where the hash symbols indicate
different types of heading.

Version control platforms like GitHub and GitLab will show the formatted version
of `README.md` directly underneath the file list, so it's a good place to state
what the project does and ideally how to use it. Some projects have very detailed
READMEs, but you will find even a brief description will help other users find
their bearings.

Without a `LICENSE` file, other users will not legally be allowed to use your
code; no license implies that your work is closed source. When you create a new
repository, you can choose to automatically add a particular type of license.
If in doubt and you're happy to share your work, use a permissive license like MIT.
The content of the LICENSE file is plain text.
