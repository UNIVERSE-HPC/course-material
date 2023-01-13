---
id: functional
name: Functional Programming
dependsOn: [
    software_architecture_and_design.procedural,
]
files: [
    state_and_side_effects_cpp.md,
    state_and_side_effects_python.md,
    recursion_cpp.md,
    recursion_python.md,
    higher_order_functions_cpp.md,
    higher_order_functions_python.md,
    lazy_evaluation_python.md,
]
---


## The Functional Paradigm

We don't have any new slides or intro video today, since we're continuing from yesterday's Programming Paradigms.

If you want to refresh your memory though, here's the [video](https://youtu.be/YC4ohR5Pf5Q) and [slides](../slides/1.2-Programming-Paradigms.pptx) from yesterday.

## Set up training materials

So let's download the training materials for this material from the GitHub code repository online.
Go to [https://github.com/SABS-R3/software-engineering-day2/tree/gh-pages](https://github.com/SABS-R3/software-engineering-day2/tree/gh-pages) in a browser (any will do, although Firefox is already installed on the provided laptops).
Select the green `Code` button, and then select `Download ZIP`, and then in Firefox selecting `Save File` at the dialogue prompt.
This will download all the files within a single archive file.
After it's finished downloading, we need to extract all files from the archive.
Find where the file has been downloaded to (on the provided laptops this is `/home/dtcse/Downloads`, then start a terminal.
You can start a terminal by right-clicking on the desktop and selecting `Open in Terminal`.
Assuming the file has downloaded to e.g. `/home/dtcse/Downloads`, type the following within the Terminal shell:

~~~ bash
cd ~
unzip /home/dtcse/Downloads/software-engineering-day2-gh-pages.zip
~~~
{: .language-bash}

The first `cd ~` command *changes our working directory* to our home directory (on the provisioned laptops, this is `/home/dtcse`).

The second command uses the unzip program to unpack the archive in your home directory, within a subdirectory called `software-engineering-day2-gh-pages`.
This subdirectory name is a little long to easily work with, so we'll rename it to something shorter:

~~~
mv software-engineering-day2-gh-pages se-day2
~~~
{: .language-bash}

Change to the `code` directory within that new directory:

~~~ bash
cd se-day2/code
~~~
{: .language-bash}

> ## Functions in Maths
> > In mathematics, a function is a relation between sets that associates to every element of a first set exactly one element of the second set.
> >
> > -- Wikipedia - [Function (mathematics)](https://en.wikipedia.org/wiki/Function_(mathematics))
{: .callout}

The Functional paradigm is based on mathematical functions.
A program written in a functional style describes a series of operations which are performed on data to produce a desired output, the focus being on **what** rather than **how**.

You will likely encounter functional programming in the future in data analysis code, or if you use frameworks such as Hadoop, or languages like R.
In fact, there's a good [section on functional programming](https://adv-r.hadley.nz/fp.html) in Hadley Wickham's Advanced R book.

Though we are teaching Functional Programming after Object Oriented Programming, it does not build on Object Oriented as Object Oriented built on Procedural Programming.
It belongs to a different branch in the history of paradigms, the Declarative family.

> ## Functional Style
>
> In his introduction to functional programming in Advanced R, Hadley Wickham gives a good summary of the style:
>
> > It’s hard to describe exactly what a functional style is, but generally I think it means decomposing a big problem into smaller pieces, then solving each piece with a function or combination of functions.
> > When using a functional style, you strive to decompose components of the problem into isolated functions that operate independently.
> > Each function taken by itself is simple and straightforward to understand; complexity is handled by composing functions in various ways.
> >
> > -- Hadley Wickham - [Functional Style](https://adv-r.hadley.nz/fp.html)
{: .callout}