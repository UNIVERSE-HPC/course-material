---
name: Making Packages
dependsOn: [
  technology_and_tooling.packaging_dependency_management.pack_python_01intro
]
tags: [python]
attribution: 
    - citation: >
        "Python Packaging" course developed by Thibault Lestang and the Oxford Research 
        Software Engineering group
      url: https://github.com/OxfordRSE/python-packaging-course
      image: https://www.rse.ox.ac.uk/images/banner_ox_rse.svg
      license: CC-BY-4.0
---


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
