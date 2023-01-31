---
name: Packaging Python Projects
dependsOn: [
]
tags: [python]
---

:::callout
This course is adapted from the "Python Packaging" course developed by Thibault Lestang 
and the OxRSE group (https://github.com/OxfordRSE/python-packaging-course)
:::

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


## Materials for this course {#materials-for-this-course}

This course assumes that you have a local copy of the materials repository.
To make it, you can simply clone the repository using git:

```shell
git clone https://github.com/OxfordRSE/python-packaging-course
```

For non-git users, you can visit <https://github.com/OxfordRSE/python-packaging-course>
and download the materials as a ZIP archive ("code" green button on the top right corner).


## Two scripts to analyse a timeseries {#two-scripts-to-analyse-a-timeseries}

Our starting point for this workshop is the scripts `analysis.py` and `show_extremes.py`.
You'll find them in the `scripts/` directory at the root of the repository.

Both scripts perform operations on a _timeseries_, a sequence of numbers indexed by time.
This timeseries is located in `analysis1/data/brownian.csv` and describes the (simulated)
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

Let's have a quick overview of these scripts, but **don't try to understand the details**, it is irrelevant to the present workshop.
Instead, let's briefly describe their structure.


### Overview of `base.py` {#overview-of-base-dot-py}

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

You're probably familiar with this kind of script, in which several independant operations are performed
on a single dataset.
It is the typical output of some "back of the enveloppe", exploratory work so common in research.
Taking a step back, these scripts are the reason why high-level languages like Python are so popular
among scientists and researchers: got some data and want to quickly get some insight into it? Let's
just jot down a few lines of code and get some numbers, figures and... ideas!

Whilst great for short early research phases, this "back of the enveloppe scripting" way of working can quickly
backfire if maintained over longer periods of time, perhaps even over your whole research project.
Going back to `analysis.py`, consider the following questions:

-   What would you do if you wanted to plot the timeseries over the last 50 time units instead of the first 50?
-   What would you do if you wanted to visualise the _Probablity Density Function_ (PDF) instead of the histogram (effectively passing the optional argument `density=true`
    to `numpy.histogram`).
-   What would you do if you were given a similar dataset to `brownian.csv` and asked to compute the mean, compute the histogram along with other things not implemented in `analysis.py` ?

In the interest of time, you are likely to end up modifying some specific lines (to compute the PDF instead of the histogram for example), or/and copy and paste of lot of code.
Whilst convenience on a short term basis, is it going to be increasingly difficult to understand your script, track its purpose, and test that its results are correct.
Three months later, facing a smilar dataset, would you not be tempted to rewrite things from scratch? It doesn't have to be this way! As you're going to learn in this ourse,
organising your Python software into _packages_ alleviates most of these issues.


### Overview of `show_extremes.py` {#overview-of-show-extremes-dot-py}

Contrarily to `base.py`, the script `show_extreme.py` has one purpose: to
produce a figure displaying the full timeseries (the particle's position as a function
of time from the initial recorded time to the final recorded time) and to hightlight
_extreme fluctuations_: the rare events when the particle's position is above a given
value `threshold`.

The script starts by reading the data and setting the value of the threshold:

```python
timeseries = np.genfromtxt("./data/brownian.csv", delimiter=",")
threshold = 2.5
```

The rest of the script is rather complex and its discussion is irrelevant to this course.
Let's just stress that it exhibits the same pitfalls than `analysis.py`.


## Separating methods from parameters and data {#separating-methods-from-parameters-and-data}

Roughly speaking, a numerical experiment is made of three components:

-   The data (dataset, or parameters of simulation).
-   The operations performed on this data.
-   The output (numbers, plots).

As we saw, scripts `analysis.py`, and `show_extremes.py` mix the three above components into a single
`.py` file, making the analysis difficult (sometimes even risky!) to modify and test.
Re-using part of the code means copying and pasting blocks of code out of their original context, which is
a dangerous practice.

In both scripts, the operations performed on the timeseries `brownian.csv` are independant from it, and could very well
be applied to another timeseries. In this workshop, we're going to extract these operations (computing the mean, the histogram, visualising the extremes...),
and formulate them as Python _functions_, grouped by theme inside _modules_, in a way that can be reused across similar analyses. We'll then bundle these modules into a Python
_package_ that will make it straightfoward to share them across different analysis, but also with other people.

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

If we were to make changes to the way some operations are implemented, we would simply make
changes to the package, leaving the scripts unmodified. This reduces the risk of messing of introducing errors in your analysis, when all what you want to do is modyfying
some opearation of data.
The changes are then made available to all the programs that use the package: no more copying and pasting code around.

:::callout
Taking a step back, the idea of separating different components is pervasive in software 
developemt
and software design. Different names depending on the field (encapsulation, separation of concerns,
bounded contexts...).
:::

# Making a package

## init dot pie

Whenever you import a directory, Python will look for a file <span class="underline"><span class="underline">init</span></span>.py at the root of this
directory, and, if found, will import it.
It is the presence of this initialization file that truly makes the `tstools` directory a Python
package[^fn:1].

As a first example, let's add the following code to `__init__.py`:

```python
# tstools/__init__.py
from os.path import basename
filename = basename(__file__)
print(f"Hello from {filename}")
```

If we now import the `tstools` package:

```python
import tstools
print(tstools.filename)
```

```text
Hello from __init__.py
__init__.py
```

The lesson here is that any object (variable, function, class) defined in the `__init__.py` file is available under the package's namespace.


::::challenge{id=single-namespace, title="Bringing all functions under a single 
namespace"}

Our package isn't very big, and the internal strucure with 2 different modules isn't
very relevant for a user.

Write the `__init__.py` so that all functions defined in
modules `vis.py` and `moments.py` are accessible directly
under the `tstools` namespace, that is

```python
import tstools

# instead of mean, var = tstools.moments.get_mean_and_var(...)
mean, var = tstools.get_mean_and_var(timeseries) 

# instead of fig, ax = tstools.vis.plot_histogram(...)
fig, ax = tstools.plot_histogram(timeseries, 4*np.sqrt(var)) 
```
	
:::callout
By default python looks for modules in the current directory
and some other locations (more about that later). When using `import`,
you can refer to modules in the current package using the _dot notation_:

```python
# import something from module that resides
# in the current package (next to the __init__.py)
from .module import something
```
:::
::::


### Using the package {#using-the-package}

Our package is now ready to be used in our analysis, and an analysis scripts could look like this:

```python
# analysis1/analysis1.py
import numpy as np
import matplotlib.pyplot as plt
import tstools

timeseries = np.genfromtxt("./data/brownian.csv", delimiter=",")

mean, var = tstools.get_mean_and_var(timeseries)

fig, ax = tstools.plot_histogram(timeseries, nbins=100)
```

Note that the above does the exact same amount of work job as 
`initial_scripts/analysis.py`... but is much shorter and easier to read!

[^fn:1]: Since Python 3.3, this isn't technically true. Directories without a `__init__.py` file are called namespace packages, see [Packaging namespace packages](https://packaging.python.org/guides/packaging-namespace-packages/) on the Python Packaging User Guide). However, their discussion is beyond the scope of this course.

## The tstools package

We now have a `tstools` directory with 2 modules:

```text
analysis1/
      tstools/
  	      __init__.py
  	      moments.py
  	      vis.py
      data/
```

The directory `tstools` is a Python package! It is possible to import functions from the modules it contains:

```python
import numpy as np

import tstools.moments
from tstools.vis import plot_histogram

timeseries = np.genfromtxt("./data/brownian.csv", delimiter=",")

mean, var = tstools.moments.get_mean_and_var(timeseries)
fig, ax = tstools.vis.plot_histogram(timeseries)
```

Let's try to import the package as a whole:

```python
# compute-mean.py
import numpy as np

import tstools

timeseries = np.genfromtxt("./data/brownian.csv", delimiter=",")

mean = tstools.moments.get_mean_and_var(timeseries)
```

```text
$ python compute_mean.py
Traceback (most recent call last):
  File "<stdin>", line 4, in <module>
AttributeError: module 'tstools' has no attribute 'moments'
```

What went wrong?
:::solution
When importing the directory `tstools`, the python interpreter
looks for a file named `__init__.py` inside this directory and imports this python file.
If this python file is empty, or simply doesnt exists... nothing is imported.
:::

In the following section we add a couple of `import` statements in
the `__init__.py` so that all our functions (in both modules) are
available under the single namespae `tstools`.

# Reusing a package

## Another analysis

Let's say that we have another directory `analysis2`, that contains another
but similar dataset to `analysis1/data/brownian.csv`.
Now that we've structured our software into a python package, we would like
to reuse that package for our second analysis.

In the directory `analysis2/`, let's write a script `analysis2.py`, that imports the `tstools` package created in the previous section.

```text
analysis2/
      analysis2.py
      data/
  	      hotwire.csv
```

```python
# analysis2/analysis2.py
import numpy as np

import tstools

timeseries = np.genfromtxt("./data/hotwire.csv", delimiter=",")
fig, ax = tstools.plot_trajectory_subset(timeseries, 0, 50)
```

```text
$ python analysis2.py
Traceback (most recent call last):
  File "<stdin>", line 10, in <module>
  File "<stdin>", line 5, in main
ModuleNotFoundError: No module named 'tstools'
```

At the moment lives in the directory `analysis1/`, and, unfortunately, Python cannot find it!
How can we tell Python where our package is?

## Where does Python look for packages?

When using the `import` statement, the python interpreter looks for the package (or module) in a list of directories known as the _python path_.

Let's find out about what directories make the python path:

```text
$ python
>>> import sys
>>> sys.path
['', '/usr/lib/python38.zip', '/usr/lib/python3.8', '/usr/lib/python3.8/lin-dynload', '/home/thibault/python-workshop-venv/lib/python3.8/site-packages/']
```

The order of this list matters: it is the order in which python looks into the directories that constitute the python path.
To begin with, Python first looks in the current directory.
If the package/module isn't found there, the python intepreter looks in the following directories
(in this order):

-   `/usr/lib/python38.zip`
-   `/usr/lib/python3.8`
-   `/usr/lib/python3.8/lib-dynload`

The above contain the modules and packages in the _standard library_, _i.e_ the packages and modules that come "pre-installed" with Python.
Finally, the python interpreter looks inside the directory `/home/thibault/python-workshop-venv/lib/python3.8/site-packages/`.

:::callout
The output of `sys.path` is probaby different on your machine. It depends on many 
factors,
like your operating system, your version of Python, the location of your current active Python
environment.
:::

For Python to find out package `tstools` it must be located in one of the directories listed in
the `sys.path` list. If it is the case, the package is said to be _installed_.

Looking back at the example in the [previous section]({{< relref "another-analysis" >}}), let's list some potential ways we can make the `tstools` package importable from the `analysis2/` directory:

1.  **Copy (`analysis1/tstools/`) in `analysis2/`**.
    You end up with two independant packages. If you make changes to one, you have to remember to make the same
    changes to the other. It's the usual copy and paste problems: inefficient and error-prone.
2.  **Add `analysis1/` to `sys.path`**.
    At the beginning of `analysis2.py`, you could just add

    ```python
    import sys
    sys.path.append("../analysis1/")
    ```

    This approach can be sufficient in some situations, but generally not recommended. What if the package directory is relocated?
3.  **Copy `analysis1/tstools` directory to the `site-packages/` directory.**
    You have to know where the `site-packages` is. This depends on your current system and python environment (see below).
    The location on your macine may very well be differnt from the location on your colleague's machine.

More generally, the three above approaches overlook a very important
point: **dependencies**.  Our package has two: numpy and matplotlib.
If you were to give your package to a colleague, nothing guarantees
that they have both packages installed.  This is a pedagogical
example, as it is likely that they would have both installed, given
the popularity of these packages.  However, if your package relies on
less widespread packages, specific versions of them or maybe a long
list of packages, it is important to make sure that they are
available.

Note that all three above approaches work.  However, unless you have a
good reason to use one of them, these are not recommended for the
reasons above. In the next section, we look at the recommended way to
install a package, using `setuptools` and `pip`.

## setuptools, setup dot pie and pip


The recommended way to install a package is to use the `setuptools` library in conjunction with `pip`, the official python _package manager_.
Effectively, this approach is roughly equivalent to copying the package to the `site-packages` directory, but the process is **automated**.


### pip {#pip}

Pip is the de facto package manager for Python packages.  It's main
job is to install, remove, upgrade, configure and manage Python
packages, both available locally on your machine but also hosted on on
the [Python Package Index (PyPI)](https://pypi.org/).  Pip is
maintained by the [Python Packaging
Authority](https://www.pypa.io/en/latest/).

Installing a package with `pip` looks like this

```shell
pip install <package directory>
```

let's give it a try

```shell
# In directory analysis1/
pip install ./tstools
```

```text
ERROR: Directory './tstools' is not installable. Neither 'setup.py' nor 'pyproject.toml' found.
```

The above doesn't really look like our package got installed properly.
For `pip` to be able to install our package, we must first give it
some information about it. In fact, `pip` expects to find a python
file named `setup.py` in the directory that it is given as an
argument. This file will contain some metadata about the package and
tell `pip` the location of the actual source of the package.


### `setup.py` (setup dot pie) and distribution packages 

The `setup.py` file is a regular Python file that makes a call to the `setup` function
available in the `setuptools` package.

Let's have a look at a minimal `setup.py` file for our `tstools` package:

```python
from setuptools import setup

setup(name='tstools',
      version='0.1',
      description='A package to analyse timeseries',
      url='myfancywebsite.com',
      author='Spam Eggs',
      packages=['tstools'],
      install_requires = ["numpy", "matplotlib", "scipy"],
      license='GPLv3')
```

The above gives `pip` some metadata about our package: its version, a
short description, its authors, ad its license. It also provides
information regarding the dependencies of our package, _i.e_ `numpy`
and `matplotlib`.  In addition, it gives `setup` the location of the
package to be installed, in this case the directory `tstools`.

:::callout
The above `setup.py` states `(...,package=["tstools"],...)`.
In English, this means "setuptools, please install the package `tstools/` located in the same directory as the file `setup.py`".
This therefore assumes that the file `setup.py` resides in the directory that _contains_ the package, in this case `analysis1/`.
:::

```text
python-workshop/
      analysis1/
  	      data/
  	      analysis1.py
  	      setup.py
  	      tstools/
```

Actually, there are no reasons for our `tstools` package to be located
in the `analysis1/` directory.  Indeed, the package is independant
from this specific analysis, and we want to share it among multiple
analyses.

To reflect this, let's move the `tstools` package into a new directory
`tstools-dist` located next to the `anaylis1` and `analysis2`
directories:

```text
python-workshop/
      analysis1/
  	      data/
  	      analysis1.py
      analysis2/
  	      data/
  	      analysis2.py
      tsools-dist/
  	      setup.py
  	      tstools/
```

The directory `tstools-dist` is a _distribution package_, containing the `setup.py` file and the package itself - the `tstools` directory.
These are the two minimal ingredients required to _distribute_ a package, see section [Building python distributions]({{< relref "building_distributions" >}}).


::::challenge{id=installing-tstools, title="Installing `tsools` with pip"} 

1.  Write a new `setup.py` file in directory `tstools-dist` including the following metadata:

    -   The name of the package (could be `tstools` but also could be anything else)
    -   The version of the package (for example 0.1)
    -   A one-line description
    -   Your name as the author
    -   Your email
    -   The GPLv3 license

    Hint: A list of optional keywords for `setuptools.setup` can be found [here](https://setuptools.readthedocs.io/en/latest/setuptools.html#new-and-changed-setup-keywords).
2.  \*Un\*install numpy and matplotlib

    ```shell
    pip uninstall numpy matplotlib
    ```

:::callout
Make sure `pip` points to your current virtual environment (you can check this by typing 
`pip --version`. Particularly, if admin rights are necessary to uninstall and install 
packages, you're probably using `pip` in your global Python environment. To make sure 
that you run the correct `pip` for your correct Python environment, run `python -m pip 
<pip command>` instead of `pip <pip command>`.)
:::


3.  Install the `tstools` package with `pip`.
    Remember: `pip install <location of setup file>`
    Notice how `numpy` and `matplotlib` are automatically downloaded (can you find from where?) even though your just uninstalled them.
4.  Move to the directory `analysis2/` and check that you can import
    your package from there.  Where is this package located?  Hint:
    You can check the location a package using the `__file__`
    attribute.
5.  The directory `analysis2` contains a timeseries under
    `data/`. What is the average value of the timeseries?

::::


Congratulations! Your `tstools` package is now installed can be reused
across your analyses...  no more dangerous copying and pasting!

## Maintaining your package


In the previous section you made your package "pip installable" by
creating a `setup.py` file.  You then installed the package,
effectively making accessible between different analysis directories.

However, a package is never set in stone: as you work on your
analyses, you will almost certainly likely make changes to it, for
instance to add functionalities or to fix bugs.

You could just reinstall the package each time you make a modification
to it, but this obviously becomes tedious if you are constantly making
changes (maybe to hunt down a bug) and/or testing your package.  In
addition, you may simply forget to reinstall your package, leading to
potentially very frustrating and time-consuming errors.


### Editable installs {#editable-installs}

`pip` has the ability to install the package in a so-called "editable" mode.
Instead of copying your package to the package installation location, pip will just
write a link to your package directory.
In this way, when importing your package, the python interpreter is redirected to
your package project directory.

To install your package in editable mode, use the `-e` option for the `install` command:

```shell
# In directory tstools-dist/
pip install -e .
```


::::challenge{id=editable-install, title="Editable install"}

1.  Uninstall the package with `pip uninstall tstools`
2.  List all the installed packages and check that `tstools` is not among them
    Hint: Use `pip --help` to get alist of available `pip` commands.
3.  re-install `tstools` in editable mode.
4.  Modify the `tstools.vis.plot_trajectory_subset` so that it returns the maximum value
    over the trajectory subset, in addition to `figure` and `axis`.
    Hint: You can use the numpy function `amax` to find the maximum of an array.
5.  Edit and run the script `analysis2/analysis2.py` to print the
    maximum value of the timeseries `analysis2/data/hotwire.csv` between t=0
    and t = 0.25. 

In editable mode, `pip install` just write a file
`<package-name>.egg-link` at the package installation location in
place of the actual package. This file contains the location of the
package in your package project directory:

```shell
cat ~/python-workshop-venv/lib/python3.8/site-packages/tstools.egg-link
/home/thibault/python-packaging-workshop/tstools
```
::::

## Summary

-   In order to reuse our package across different analyses, we must _install_ it.
    In effect, this means copying the package into a directory that is in the python path.
    This shouldn't be done manually, but instead using the `setuptools` package to write a
    `setup.py` file that is then processed by the `pip install` command.
-   It would be both cumbersome and error-prone to have to reinstall the package each time
    we make a change to it (to fix a bug for instance). Instead, the package can be installed
    in "editable" mode using the `pip install -e` command. This just redirects the python
    interpreter to your project directory.
-   The main value of packaging software is to facilitate its reuse across different projects.
    One you have extracted the right operations into a package that is independent of your
    analysis, you can easily "share" it between projects. In this way you avoid inefficient
    and dangerous duplication of code.

Beyond greatly facilitating code reuse, writing a python package (as opposed to a loosely
organised collection of modules) enables a clear organisation of your software into modules
and possibly sub-packages. It makes it much easier for others, as well as yourself, to
understand the structure of your software, _i.e_ what-does-what.

Moreover, organising your python software into a package gives you access to a myriad
of fantastic tools used by thousands of python developers everyday. Examples include
pytest for automated testing, sphinx for building you documentation, tox for automation
of project-level tasks.

# Sharing a package

## Building Python distributions

Before you can distribute a package, you must first create a _distribution_.
A distribution is a single file that bundles all the files and data necessary to install and use
the package - but also sometimes compile and test it.

A distribution usually takes the from of an archive (`.tar`, `.zip` or similar).
There are several possible distribution formats, but in 2020, only two are really important: the _source distribution_ (sdist) and the _wheel_ (bdist\_wheel).


### Source distributions {#source-distributions}

Python distributions are commonly built using the `setuptools` library, _via_ the `setup.py` file.
Building a source distribution looks like this:

```shell
python setup.py sdist
```

```text
running sdist
running egg_info
writing tstools.egg-info/PKG-INFO
writing dependency_links to tstools.egg-info/dependency_links.txt
writing requirements to tstools.egg-info/requires.txt
writing top-level names to tstools.egg-info/top_level.txt
reading manifest file 'tstools.egg-info/SOURCES.txt'
writing manifest file 'tstools.egg-info/SOURCES.txt'
running check
creating tstools-0.1
creating tstools-0.1/tstools
creating tstools-0.1/tstools.egg-info
copying files to tstools-0.1...
copying setup.py -> tstools-0.1
copying tstools/__init__.py -> tstools-0.1/tstools
copying tstools/show_extremes.py -> tstools-0.1/tstools
copying tstools/tstools.py -> tstools-0.1/tstools
copying tstools.egg-info/PKG-INFO -> tstools-0.1/tstools.egg-info
copying tstools.egg-info/SOURCES.txt -> tstools-0.1/tstools.egg-info
copying tstools.egg-info/dependency_links.txt -> tstools-0.1/tstools.egg-info
copying tstools.egg-info/requires.txt -> tstools-0.1/tstools.egg-info
copying tstools.egg-info/top_level.txt -> tstools-0.1/tstools.egg-info
Writing tstools-0.1/setup.cfg
creating dist
Creating tar archive
removing 'tstools-0.1' (and everything under it)
```

This mainly does three things:

-   It gathers the python source files that consitute the package (incuding the `setup.py`).
-   It writes some metadata about the package in a directory `<package name>.egg-info`.
-   It bundles everyting into a tar archive.

The newly created sdist is written in a directory `dist` next to the `setup.py` file:

```shell
tar --list -f dist/tstools-0.1.tar.gz
```

```text
tstools-0.1/
tstools-0.1/PKG-INFO
tstools-0.1/setup.cfg
tstools-0.1/setup.py
tstools-0.1/tstools/
tstools-0.1/tstools/__init__.py
tstools-0.1/tstools/vis.py
tstools-0.1/tstools/moments.py
tstools-0.1/tstools.egg-info/
tstools-0.1/tstools.egg-info/PKG-INFO
tstools-0.1/tstools.egg-info/SOURCES.txt
tstools-0.1/tstools.egg-info/dependency_links.txt
tstools-0.1/tstools.egg-info/requires.txt
tstools-0.1/tstools.egg-info/top_level.txt
```

{{% notice tip %}}
Take a moment to explore the content of the archive.
{{% /notice %}}

As the name suggest a source distribution is nothing more than the source code of your package,
along with the `setup.py` necessary to install it.
Anyone with the source distribution therefore has everything they need to install your package.
Actually it's even possible to give the sdist directly to `pip`:

```shell
pip install tstools-0.1.tar.gz
```

And you're done!


### Wheel distributions {#wheel-distributions}

Source distributions are very basic, and installing them basically amount
to running the package's `setup.py` script.
These poses two issues:

-   In addition to the call to `setup`, the `setup.py` can contain any valid Python.
    Thinking about security for moment, this means that installing a package could
    result in the execution of malicious code.
-   To install from a source distribution, `pip` must first unpack the distribution, then
    execute the `setup.py` script. Directly unpacking to the correct location in the python
    path would be much faster.
-   Package can contain code written in a compiled language like C or Fortran. Source
    distributions assume that the recipient has all the tools necesseray to compile
    this code. Compiling code can also takes time (hours!).

This issues can be overcome by using _wheel distributions_.
For pure-Python packages, a wheel is very similar to a source distribution: it's an
archive that contains both the python source of the package and some metadata.
The main difference with sdists is that **wheels doesn't require pip to execute
the `setup.py`** file, instead the content of wheels is directly unpacked in the correct
location - most likely your current environment's `site-packages/` directory.
This makes the installation of wheels both safer and faster.

Another very important feature of python wheels is that they can embed compiled code,
effectively alleviating the need for the recipient to compile (_build_) anything.
As a result, the wheel is platform-dependant, but makes the installation considerably easier
and faster. For this reason, wheels are part of the family of _built distrubtions_.
Another type of built distribution is the python _egg_. However, the wheel format was
created in response to the shortcomings of Python eggs and this format is now obsolete.
See [Wheel vs Egg](https://packaging.python.org/discussions/wheel-vs-egg/) on the Python Packaging User Guide.


::::challenge{id=python-wheel, title="Building a Python wheel"

1.  If you don't have one, create a new developement virtual environment in the
    `tstools-dist` directory:

    ```shell
    python -m venv tstools-venv
    source tstools-venv/bin/activate # (GNU/Linux and MacOS)
    tstools-venv\Scripts\activate.bat # (Windows command prompt)
    tstools-venv\Scripts\Activate.ps1 # (Windows PowerShell)
    ```
2.  Update `pip`

    ```shell
    pip install --upgrade pip
    ```
3.  Install `setuptools` and the `wheel` extension:

    ```shell
    pip install setuptools wheel
    ```
4.  Build a wheel

    ```shell
    python setup.py bdist_wheel
    ```
5.  Install the wheel using `pip`.
    Hint: wheels are written in the `dist/` directory, next to the `setup.py` file, just
    like source distributions.
6.  `.whl` files are basically zip files. Unzip the wheel and explore its contents.

:::callout
The `bdist_wheel` command is only available after the package 
[wheel](https://pypi.org/project/wheel/) is installed in your current environment.
It is an extension to the `setuptools` package.
:::

::::

## Uploading distributions to PyPI

In the previous section you learned how to create distributions for your packages.
In this section, we look at how to share them with others, so that other people can easily install and use your packages.


### Package repositories {#package-repositories}

Let's think about distributing packages for a minute.
If you wanted to share one of your distributions (whether it's a source distribution or a wheel distribution) with a colleague, how would
you proceed?
If you both work next to each other, you could simply exchange a USB stick. If not, you can probably email the distribution, or share it _via_ a cloud host.

Although effective on a short term basis, these solutions present serious shortcomings:

-   You would have to share the distribution again each time you make a change to the package.
-   If your colleague wants a specific version (that's not the latest), you would have to check out the old version of your package and build the distribution again - unless your manually
    keep track of all your distributions.
-   Users of your package must contact you to get the distribution, and wait for you to get back to them.

These issues can be overcome by using _package repositories_. A package repository is just an index of packages hosted on distant servers, available to download from installation.
If you're using GNU/Linux, you use a package repository each time you install new software: `apt install libreoffice` is nothing but a request for the package `libreoffice` to one of
the Ubuntu package repositories.

The main reposotiry for Python is the [Python Package Index](https://pypi.org/) (PyPI).
Whenever you install a package with `pip install package`, `pip` first check than `package` isnt a directory on your machine (in which case `pip` tries to install it as a package).
If not, `pip` makes a request to PyPI and, if it exists, downloads and install package `package`.


### Publishing distributions to the test PyPI index 
{#publishing-distributions-to-the-test-pypi-index}

Once a package is uploaded to PyPI, it cannot easily be removed.
This is to prevent packages from disappearing without warning while other software depends on it.
To test publishing our distributions, we can use [test.pypi.org](https://test.pypi.org) instead of the regular [pypi.org](https://pypi.org/).
This is a separate database dedicated to tests and experiments.

Uploading distributions to PyPI and (TestPyPI) is a very simple process, thanks to [twine](https://twine.readthedocs.io/en/latest/), a utility for publishing Python packages on PyPI.
Installing twine is as simple as

```shell
pip install twine
```

You can now upload a distribution to to the regular PyPI (not the test one) as follows:

```shell
twine upload dist/tstools-0.1-py3-none-any.whl
```

You will be asked for your usernanme and password for PyPI. To create an account, visit <https://pypi.org/account/register/>.
If you find yourself uploading packages often, or if you are concerned about security, it is possible to authenticate to PyPI using
a token that's specific for your account, or a particular project. This token is usually configured in a `~/.pypirc` file, and allows you to authenticate
without entering your username and password every time. Note that you might want to encrypt `~/.pypirc` if concerned about security.

::::challenge{id=publishing-distributions, title="Publishing distributions to TestPyPI"}

1.  On PyPI (or TestPyPI), there cannot be two package with the same name. Therefore, before you upload your `tstools` package,
    you must give the project a unique name. To do so, open the `tstools-dist/setup.py` file and change the `name` entry
    in the call to the `setup` function to something unique to you, for instance:

    ```python
    name='tstools-<yourname>'
    ```
2.  Install `twine` in your `python-packaging-venv` environment

    ```shell
    pip install twine
    ```
3.  If you created some distributions in the previous sections, remove everything inside your `dist/` directory

    ```shell
    rm dist/*
    ```
4.  Create a source distribution and a wheel for your `tstools` package

    ```shell
    python setup.py sdist bdist_wheel
    ```
5.  If you don't have one, create an account on the Test PyPI index by visiting <https://test.pypi.org/account/register/>.
6.  Lastly, publish your distributions to the test PyPI index:

    ```shell
    twine upload --repository testpypi dist/*
    ```

    Can you find your package on [test.pypi.org](https://test.pypi.org) ?
7.  Create a new virtual environment and install your `tstools` package from the test PyPI index

    ```shell
    pip install --index-url https://test.pypi.org/simple/ --extra-index-url https://pypi.org/simple your-package
    ```

    The above command is a bit lengthy, but it's just because we are installing from the test
    index instead of the regular one. `--index-url https://test.pypi.org/simple/` tells `pip`
    to look for the package at `test.pypi.org` instead of `pypi.org` (which is the default).
    In addition, `--extra-index-url https://pypi.org/simple` tells `pip` to looks for dependencies
    in the regular index, instead of the test one. In our case dependencies are `numpy` and `matplotlib`.
::::

Congratulations! You just published your first Python package.

Remarks:

-   It's always a good idea to first publish your package on the test index, before
    you publish it to the real index.
-   `twine` and `pip` defaut to the real index <https://pypi.org>, so commands are really simple:

    ```shell
    twine upload <distributions> # Publish package
    pip install <package name> # Install package from pypi.org
    ```
-   You can, and _should_ publish your package each time you make a new version of it.
    All versions are stored on PyPI, and are accessible from pip.
    See the [release history for numpy](https://pypi.org/project/numpy/#history) for example.
    You could just install a specific version of numpy with:

    ```shell
    pip install numpy==1.17.5
    ```
-   Note that _you cannot_ erase a published version of your package.
    If you discover a bug in a version of your package that already has been published and want to fix it without changing the version number,
    what is known as a _post-release_, _i.e_ adding `.postX` add the end of the faulty version number.
    For instance:

    ```python
    setup(name='tstools',
          version='0.1.post1',
          ...)
    ```

    and upload your fixed package.
    This will still be considered version `0.1`, but `pip install tstools==0.1` will download
    the `0.1.post1` version.
    Note that you could publish subsequent post-releases, _i.e_ `.post2`, `.post3`...