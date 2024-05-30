---
name: Packaging Python Projects
dependsOn: [
  software_architecture_and_design.procedural.arrays_python
]
tags: [python, setuptools]
attribution: 
    - citation: >
        "Python Packaging" course developed by Thibault Lestang and the Oxford Research 
        Software Engineering group
      url: https://github.com/OxfordRSE/python-packaging-course
      image: https://www.rse.ox.ac.uk/images/banner_ox_rse.svg
      license: CC-BY-4.0
    - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1 
      url: https://www.universe-hpc.ac.uk
      image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
      license: CC-BY-4.0


---

In this workshop, you are going to learn how to organise your Python software into
_packages_. Doing so, you will be able to

-   Have your software clearly organised in a way that is standard among Python developpers, making
    your software easier to understand, test and debug.
-   Reuse your code across your research projects and analyses. No more copying and pasting
    blocks of code around: implement and test things once.
-   Easily share your software, making everybody (including yourself) able to `pip install`
    your package!

The **plan** is the following: we are going to start from a couple of rather messy python scripts and gradually
transform them into a full-blown python package. At the end of this workshop, you'll know:

-   What is a Python package and how to create one (and why!).
-   How to share your packages across several of your projects.
-   Maintain packages independantly from your research projects and analyses.
-   What are virtual environments and how to use them to install different versions of a package
    for different analyses.
-   How to share your package on the [Python Package Index](https://pypi.org/) (PyPI), effectively making it straightforward
    for anyone to install your package with the `pip` package manager (and much more!).
-   Where to go next.

Sounds interesting? Good! Get a cup of your favorite beverage and let's get started.


## Materials for this course

This course assumes that you have a local copy of the materials repository.
To make it, you can simply clone the repository using git:

```shell
git clone https://github.com/OxfordRSE/python-packaging-course
```

For non-git users, you can visit <https://github.com/OxfordRSE/python-packaging-course>
and download the materials as a ZIP archive ("code" green button on the top right corner).


## Two scripts to analyse a timeseries

Our starting point for this workshop is the script `analysis.py`. You'll find it in the `course/initial_scripts/` directory at the root of the repository.

This script perform operations on a _timeseries_ dataset, located in `course/initial_scripts/data/brownian.csv` and describes the (simulated)
one-dimensional movement of a particle undergoing [brownian motion](https://en.wikipedia.org/wiki/Brownian%5Fmotion).

```nil
0.0,-0.2709970143466439
0.1,-0.5901672546059646
0.2,-0.3330040851951451
0.3,-0.6087488066987489
0.4,-0.40381970872171624
0.5,-1.0618436436553174
...
```

The first column contains the various times when the particle's position was recorded, and
the second column the corresponding position.

Let's have a quick overview of these scripts, but **don't try to understand the
details**, it is irrelevant to the present workshop.  Instead, let's briefly
describe their structure.


### Overview of `analysis.py`

After reading the timeseries from the file `brownian.csv`, this script `base.py` does
three things:

-   It computes the average value of the particle's position over time and the standard
    deviation, which gives a measure of the spread around the average value.
-   It plots the particle's position as a function of time from the initial time until
    50 time units.
-   Lastly, it computes and plots the histogram of the particle's position over the entirety
    of the timeseries. In addition, the theoritical histogram is computed and drawn as a
    continuous line on top of the measured histogram. For this, a function `get_theoritical_histogram`
    is defined, resembling the `numpy` function `histogram`.

You're probably familiar with this kind of script, in which several independant
operations are performed on a single dataset.  It is the typical output of some
"back of the envelope", exploratory work that is common in research.  Taking a step
back, these scripts are the reason why high-level languages like Python are so
popular among scientists and researchers: got some data and want to quickly get
some insight into it? Let's just jot down a few lines of code and get some
numbers, figures and... ideas!

Whilst great for short early research phases, this "back of the envelope scripting" way of working can quickly
backfire if maintained over longer periods of time, perhaps even over your whole research project.
Going back to `analysis.py`, consider the following questions:

-   What would you do if you wanted to plot the timeseries over the last 50 time units instead of the first 50?
-   What would you do if you wanted to visualise the _Probablity Density Function_ (PDF) instead of the histogram (effectively passing the optional argument `density=true`
    to `numpy.histogram`).
-   What would you do if you were given a similar dataset to `brownian.csv` and asked to compute the mean, compute the histogram along with other things not implemented in `analysis.py` ?

In the interest of time, you are likely to end up modifying some specific lines
(to compute the PDF instead of the histogram for example), or/and copy and paste
of lot of code.  Whilst convenience on a short term basis, is it going to be
increasingly difficult to understand your script, track its purpose, and test
that its results are correct.  Three months later, facing a smilar dataset,
would you not be tempted to rewrite things from scratch? It doesn't have to be
this way! As you're going to learn in this ourse, organising your Python
software into _packages_ alleviates most of these issues.


## Separating methods from parameters and data

Roughly speaking, a numerical experiment is made of three components:

-   The data (dataset, or parameters of simulation).
-   The operations performed on this data.
-   The output (numbers, plots).

As we saw, `analysis.py` mixes the three above components into a single `.py`
file, making the analysis difficult (sometimes even risky!) to modify and test. 
Re-using part of the code means copying and pasting blocks of code out of their
original context, which is a dangerous practice. You might be thinking (and you
would be right) that this statement is an exaggeration for a script of this
size, but it is not uncommon to see scripts of several hundreds of lines of
code, mixing data, operations and output.

The operations performed on the timeseries `brownian.csv` are
independant from it, and could very well be applied to another timeseries. In
this workshop, we're going to extract these operations (computing the mean, the
histogram, visualising the extremes...), and formulate them as Python
_functions_, grouped by theme inside _modules_, in a way that can be reused
across similar analyses. We'll then bundle these modules into a Python _package_
that will make it straightfoward to share them across different analysis, but
also with other people.

A script using our package could look like this:

```python
import numpy as np
import matplotlib.pyplot as plt
import my_pkg

timeseries = np.genfromtxt("./data/my_timeseries.csv", delimiter=",")

mean, var = my_pkg.get_mean_and_var(timeseries)

fig, ax = my_pkg.get_pdf(timeseries)

threshold = 3*np.sqrt(var)
fig, ax = my_pkg.show_extremes(timeseries, threshold)
```

Compare the above to `analysis.py`: it is much shorter and easier to read.
The actual implementation of the various operations (computing the mean and variance, computing the histogram...) is now
_encapsulated_ inside the package `my_pkg`.
All that remains are the actual steps of the analysis.

If we were to make changes to the way some operations are implemented, we would
simply make changes to the package, leaving the scripts unmodified. This reduces
the risk of messing of introducing errors in your analysis, when all what you
want to do is modyfying some opearation of data.  The changes are then made
available to all the programs that use the package: no more copying and pasting
code around.

:::callout
Taking a step back, the idea of separating different components is pervasive in software 
developemt
and software design. Different names depending on the field (encapsulation, separation of concerns,
bounded contexts...).
:::
