---
name: Sharing Packages
dependsOn: [
  technology_and_tooling.packaging_dependency_management.pack_python_03reusing 
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