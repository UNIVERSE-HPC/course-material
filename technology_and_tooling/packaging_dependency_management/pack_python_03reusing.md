---
name: Reusing Packages
dependsOn: [
  technology_and_tooling.packaging_dependency_management.pack_python_02making
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

At the moment `tstools` lives in the directory `analysis1/`, and, unfortunately, Python cannot find it!
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
If the package/module isn't found there, the python interpreter looks in the following directories
(in this order):

-   `/usr/lib/python38.zip`
-   `/usr/lib/python3.8`
-   `/usr/lib/python3.8/lib-dynload`

The above contain the modules and packages in the _standard library_, _i.e_ the
packages and modules that come "pre-installed" with Python.  Finally, the python
interpreter looks inside the directory
`/home/thibault/python-workshop-venv/lib/python3.8/site-packages/`, which is our
currently active virtual environment.

:::callout
The output of `sys.path` is probably different on your machine. It depends on many 
factors,
like your operating system, your version of Python, the location of your current active Python
environment.
:::

For Python to find out package `tstools` it must be located in one of the directories listed in
the `sys.path` list. If it is the case, the package is said to be _installed_.

Looking back at the example in the previous section, let's list some potential
ways we can make the `tstools` package importable from the `analysis2/`
directory:

1.  **Copy (`analysis1/tstools/`) in `analysis2/`**.
    You end up with two independent packages. If you make changes to one, you have to remember to make the same
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
    The location on your machine may very well be different from the location on your colleague's machine.

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

## setuptools, pyproject dot toml, setup dot pie and pip


The recommended way to install a package is to use the `setuptools` library in
conjunction with `pip`, the official python _package manager_.  Effectively,
this approach is roughly equivalent to copying the package to the
`site-packages` directory, but the process is **automated**.


### pip

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

The above doesn't really look like our package got installed properly.  For
`pip` to be able to install our package, we must first give it some information
about it. In fact, `pip` expects to find either a `pyproject.toml` configuration
file or a python file named `setup.py` in the directory that it is given as an
argument. These file will contain some metadata about the package and tell `pip`
the location of the actual source of the package.


### `setup.py` (setup dot pie)

The `setup.py` file is a regular Python file that makes a call to the `setup`
function available in the `setuptools` package. This is a legacy approach to
package installation, and since Pip v10, the recommended way to install a
package is to use a `pyproject.toml` file (see below).

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

## `pyproject.toml` (pyproject dot toml)

The `pyproject.toml` file is a configuration file for Python packages,
introduced in [PEP 518](https://www.python.org/dev/peps/pep-0518/). It is
intended to replace `setup.py` for package management tasks and is designed to be
used by build tools like `pip`.

The `pyproject.toml` configuration file, offers a more standardized, reliable,
and flexible approach to specifying Python project metadata compared to
`setup.py`. It facilitates the specification of the build system a project
requires, thereby breaking the implicit dependency on setuptools. It also
improves dependency management by specifying build dependencies that are
isolated from system-wide packages, reducing the risk of interference. Unlike
`setup.py`, `pyproject.toml` isn't a Python script, reducing the risk of arbitrary
code execution and making packaging more predictable. The format is
tool-agnostic and easily extensible, allowing for the coexistence and
cooperation of different tools in the same project, which makes the packaging
process more uniform across different tools.

Here is an equivalent `pyproject.toml` file for our `tstools` package:

```toml
[build-system]
requires = ["setuptools", "wheel"] # setuptools and wheel are necessary for the build

[project]
name = "tstools"
version = "0.1"
description = "A package to analyse timeseries"
authors = [
    {name = "Spam Eggs", email = "spam.eggs@email.com"}
]
readme = "README.md"
license = {text = "MIT"}
dependencies = ["numpy", "matplotlib", "scipy"]

[project.urls]
Source = "example.com"

[project.scripts] # Define scripts here if you have any

[project.optional-dependencies] # Define optional dependencies here if you have any
```

Note that both `setup.py` and `pyproject.toml` can be used in conjunction, and in
the transition period it is common for Python packages to include both these files, as we
will do in this workshop. In this case, we can create a minimal `pyproject.toml` file that just
specifies the use of `setuptools` and links to the `setup.py` file:

```toml
[build-system]
requires = ["setuptools", "wheel"]
build-backend = "setuptools.build_meta"
```

:::callout

In this workshop we focus on the use of the `setuptools` package to install our package. There are other
tools that can be used to install Python packages, such as [`flit`](https://flit.pypa.io/), [`hatchling`](https://hatch.pypa.io/) and [`PDM`](https://pdm.fming.dev/).

:::

## Creating a package for distribution

After writing a `setup.py` and `pyproject.toml` file, our directory structure looks like this:


```text
python-workshop/
      analysis1/
  	      data/
  	      analysis1.py
  	      setup.py
          pyproject.toml
  	      tstools/
```

Actually, there are no reasons for our `tstools` package to be located
in the `analysis1/` directory.  Indeed, the package is independent
from this specific analysis, and we want to share it among multiple
analyses.

To reflect this, let's move the `tstools` package into a new directory
`tstools-dist` located next to the `analysis1` and `analysis2`
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
          pyproject.toml
  	      tstools/
```

The directory `tstools-dist` is a _distribution package_, containing the `setup.py` file and the package itself - the `tstools` directory.
These are the two minimal ingredients required to _distribute_ a package.

::::challenge{id=installing-tstools title="Installing `tsools` with pip"} 

1.  Write a stand-alone `pyproject.toml` file, or use a combination of
    `setup.py` and `pyproject.toml` files in directory `tstools-dist`. Include the
    following metadata:
    -   The name of the package (could be `tstools` but also could be anything else)
    -   The version of the package (for example 0.1)
    -   A one-line description
    -   Your name as the author
    -   Your email
    -   The GPLv3 license

2.  *Un*install numpy and matplotlib

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


### Editable installs

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


::::challenge{id=editable-install title="Editable install"}

1.  Uninstall the package with `pip uninstall tstools`
2.  List all the installed packages and check that `tstools` is not among them
    Hint: Use `pip --help` to get a list of available `pip` commands.
3.  re-install `tstools` in editable mode.
4.  Modify the `tstools.vis.plot_trajectory_subset` so that it returns the maximum value
    over the trajectory subset, in addition to `figure` and `axis`.
    Hint: You can use the numpy function `amax` to find the maximum of an array.
5.  Edit and run the script `analysis2/analysis2.py` to print the
    maximum value of the timeseries `analysis2/data/hotwire.csv` between t=0
    and t = 0.25. 

In editable mode, `pip install` creates a file,
`<package-name>.egg-link`, at the package installation location in
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
    This shouldn't be done manually, but instead using a `pyproject.toml` (or `setup.py`) 
    configuration file that a tool like `pip` can process using the  `pip install` command.
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
