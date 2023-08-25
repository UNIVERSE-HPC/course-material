---
title: Introduction to Version Control
---

## What is Version Control?

<style type="text/css">p { text-align: left; }</style>

- A tool that tracks changes to files
- Records the changes you made, and the order in which you made them
- Like the history of a page on Wikipedia 
- Or - like turning on ‘Track Changes’ in Word, but for code.

![](images/track_changes.svg)

:::{.notes}
- Version control is a tool that tracks changes to files.
- Version control records the changes you made, and the order in which you made them.
- It’s like the history of a page on wikipedia or like turning on “Track Changes” in Word or Google Docs, but for code.
- So why would you want to do that?
:::

## Why use Version Control?

:::{.columns}
::::{.column}
**A more efficient backup**

- Avoid multiple versions of same file
::::

::::{.column width="40%"}
![](images/revisions.gif)
::::
:::

:::{.notes}
- You may have experienced the common problem of having multiple nearly-identical versions of the same file with no meaningful explanation of what the differences are, just incremental changes in filename (thesis.doc, thesis_final.doc, thesis_final2.doc…).
- If we’re just dealing with text documents, some word processors let us deal with this a little better, like Microsoft Word’s “Track Changes” or Google Docs’ version history.
- However, research isn’t just Word docs, it’s code and data and diagrams too, and a single paper or project can involve a whole constellation of files, all of which need backing up!
- Using version control means we don’t keep dozens of different versions of our files hanging about taking up space, and when we store a revision, we store an explanation of what changed.
:::

## Why use Version Control?

**Reproducibility**

- Access to a copy of every version of the code
- Easy to replicate results from any paper!
- Easy to share full copy of any version

:::{.notes}
- When you use version control, at any point in the future, you can retrieve the correct versions of your documents, scripts or code. So, for example, a year after publication, you can get hold of the precise combination of scripts and data that you used to assemble a paper.
- Version control makes reproducibility simpler. Without using version control it’s very hard to say that your research is truly reproducible…
:::

## Why use Version Control?

:::{.columns}
::::{.column width="50%"}
**To aid collaboration**

- Easy to build on others’ changes
- Share one version with collaborators whilst you work on another
::::

::::{.column width="50%"}
![](images/versions.svg)
::::
:::

:::{.notes}
- As well as maintaining a revison history, VC tools also help multiple authors collaborate on the same file or set of files.
- Professional software developers use VC to work in large teams and to keep track of what they’ve done.
- If you know what changes have been made to each file, you can easily combine multiple people’s changes to a single file.
- You can also track down where and when (and by who!) bugs in the code were introduced.
:::

## How do Version Control Tools Work?

- Start by storing the base version of the file
- After that, only changes are stored
- Like instructions for building lego

![](images/track_changes.svg)

:::{.notes}
- Version control systems start by storing the base version of the file that you save and then store just the changes you made at each step on the way.
- You can think of it like storing Lego bricks and the instructions for putting them together - if you start with the first piece, then add each other in turn, you end up with your final document.
:::

## How do Version Control Tools Work?

:::{.columns}
::::{.column width="50%"}
- Changes are separate from the document itself 
- Two users can make independent sets of changes
- Creates two different versions of the document
::::

::::{.column width="50%"}
![](images/versions.svg)
::::
:::

:::{.notes}
- Once you think of changes as separate from the document itself, you can then think about taking the same document and adding different changes to it, getting different versions of the document.
- For example, two users can make independent sets of changes based on the same document.
:::

## How do Version Control Tools Work?

:::{.columns}
::::{.column width="50%"}
![](images/merge.svg)
::::

::::{.column width="50%"}
- If no conflicts - merge changes
- Two different sets of changes can be merged together onto the same base document
::::
:::

:::{.notes}
- If there aren’t conflicts, you can even try to combine two different sets of changes together onto the same base document, a process called merging.
:::

## Version Control Systems

- Git - overwhelmingly the most popular
  - GitHub, GitLab
- Mercurial
- Subversion

:::{.notes}
- Git is overwhelmingly the most popular version control system in academia, and beyond.
- It’s a distributed version control system, where every developer in a team has their own full copy of a repository, and can synchronise between them.
- It’s partly become such a success thanks to sites like GitHub and GitLab, which make it easy to collaborate on a Git repository, and provide all kinds of extra tools to manage software projects.
:::

## How can you use Version Control?

:::{.columns}
::::{.column width="50%"}
- Command-line `git`
- Standalone graphical user interface
  - GitHub Desktop
  - Git Kraken
  - Sourcetree
- Built into most IDEs
  - Visual Studio Code
  - RStudio
  - PyCharm
::::

::::{.column width="50%"}
![](images/integration-vscode.png)
::::
:::

:::{.notes}
- We’re going to teach you how to use Git on the command line, as it’s the same on every single platform (Mac, Linux & Windows) - and it’s the only way to use it on high-performance clusters like Iridis.
- However, if you’re not working on a high-performance system you could use a graphical user interface rather than the command line.
- There are many different graphical user interfaces for Git, like GitHub desktop, git kraken and sourcetree.
- Git is also built into most integrated development environments including VSCode, RStudio, and Pycharm.
:::

## Lesson Outline

We’ll cover how to use git to:

1. Create a ‘repository’ and use it to track changes to files
2. Review how your code has changed over time
3. Recover old versions of files
4. Back up your files to GitHub (or another remote repository!)
5. Collaborate remotely and resolve conflicting edits to files
