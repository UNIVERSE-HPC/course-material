---
name: Ignoring Things
dependsOn: [technology_and_tooling.version_control.07-branches]
tags: [git]
learningOutcomes:
  - Use a .gitignore file to exclude specific files from version control.
  - Explain the importance and benefits of using .gitignore files.
attribution:
  - citation: >
      This material was originally taken from training materials developed by the
      University of Southampton Research Software Group, which are based on
      the Software Carpentries course "Version Control with Git".
    url: https://github.com/Southampton-RSG-Training/git-novice/
    image: https://southampton-rsg-training.github.io/git-novice/assets/img/home-logo.png
    license: CC-BY-4.0
---

What if we have files that we **do not** want Git to track for us,
like **backup files** created by our editor
or **intermediate** files created during data analysis.
Let's switch to our dev branch, and create a few dummy files:

```bash
git switch dev
mkdir results
touch a.dat b.dat c.dat results/a.out results/b.out
```

and see what Git says:

```bash
git status
```

```text
# On branch dev
# Untracked files:
#   (use "git add <file>..." to include in what will be committed)
#
# a.dat
# b.dat
# c.dat
# results/
nothing added to commit but untracked files present (use "git add" to track)
```

Putting these files under version control would be a **waste of disk space**.
What's worse,
having them all listed could **distract** us from changes that actually matter,
so let's tell Git to **ignore** them.

We do this by creating a file in the root directory of our project called `.gitignore`.

```bash
nano .gitignore
cat .gitignore
```

```text
*.dat
results/
```

These patterns tell Git to **ignore** any file whose name ends in **`.dat`**
and everything in the **`results`** directory.
(If any of these files were **already** being tracked,
Git would **continue** to track them.)

Once we have created this file,
the output of `git status` is much cleaner:

```bash
git status
```

```text
# On branch dev
# Untracked files:
#   (use "git add <file>..." to include in what will be committed)
#
# .gitignore
nothing added to commit but untracked files present (use "git add" to track)
```

The only thing Git notices now is the newly-created `.gitignore` file.
You might think we wouldn't want to track it,
but everyone we're **sharing** our repository with will probably **want to ignore
the same** things that we're ignoring.
Let's add and commit `.gitignore`:

```bash
git add .gitignore
git commit -m "Add the ignore file"
git status
```

```text
# On branch dev
nothing to commit, working directory clean
```

As a bonus,
using `.gitignore` helps us **avoid accidentally adding files** to the repository that we don't want.

```bash
git add a.dat
```

```text
The following paths are ignored by one of your .gitignore files:
a.dat
Use -f if you really want to add them.
fatal: no files added
```

If we really want to override our ignore settings,
we can use `git add -f` to force Git to add something.
We can also always see the status of ignored files if we want:

```bash
git status --ignored
```

```text
# On branch dev
# Ignored files:
#  (use "git add -f <file>..." to include in what will be committed)
#
#        a.dat
#        b.dat
#        c.dat
#        results/

nothing to commit, working directory clean
```

Force adding can be useful for adding a `.gitkeep` file. You can't add empty directories to a repository- they have to have some files within them. But if your code expects there to be a `results/` directory to output to, for example, this can be a problem. Users will run your code, and have it error out at a missing directory and have to create it themselves.

Instead, we can create an empty `.gitkeep` file using `touch` in the `results/` directory, and force-add it. As it starts with a `.`, it's a special file and won't appear in `ls` (only `ls -a`), but it will ensure that the directory structure is kept as part of your repository.
