---
name: Introduction
dependsOn: [technology_and_tooling.ide.cpp]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Prerequisites

This course assumes a Linux-like environment and a command line. If you are on
Windows, the simplest route is the
[Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/install)
(WSL2) with an Ubuntu image, which gives you an Ubuntu shell alongside Windows.
All instructions below assume a recent Ubuntu (24.04 LTS or newer) and its `apt`
package manager. macOS works too, though you will need to translate the install
commands to Homebrew.

You will also want to be comfortable enough with C++ to read and edit a small
program, though the C++ in the exercises is deliberately kept simple: the
interesting part is always the `CMakeLists.txt`.

### What you need

- **CMake 3.24 or newer.** The exercises use features that need a reasonably
  recent CMake, including header file sets, `CMakePresets.json`, and
  `FetchContent`'s `FIND_PACKAGE_ARGS`.
- **A C++20 compiler**, such as GCC 11 or newer, which you get from
  `build-essential`.
- **Ninja**, used as the build tool in the presets we write later on.
- **Git**, both to clone the exercises and because `FetchContent` uses it to
  download dependencies.
- **Eigen** and **Boost.Program_options**, the two dependencies we ask CMake to
  find on the system. Boost 1.70 or newer is needed, so that it provides its own
  CMake config files.

Everything comes from `apt`:

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build git \
                 libeigen3-dev libboost-program-options-dev
```

Two other libraries appear later in the course, `fmt` and `Catch2`, but you do
not need to install those: the build downloads them for you. That does mean you
need a working internet connection when you reach those sections.

### Checking your setup

```bash
cmake --version
g++ --version
ninja --version
```

:::callout{variant="warning"}
Older Ubuntu releases ship a CMake that is too old for this course. Ubuntu 22.04,
for instance, packages CMake 3.22, which predates the header file sets we rely on
when installing a library.

If `cmake --version` reports anything below 3.24, upgrade rather than working
around it. Kitware, who develop CMake, publish an
[APT repository](https://apt.kitware.com/) with current releases, and
`pip install cmake` or `snap install cmake --classic` are quick alternatives.
:::

## Course materials

Clone the material repository and change your current working directory to the project
root:

```bash
git clone https://github.com/OxfordRSE/ModernCMakeCourse
cd ModernCMakeCourse
```

## What is CMake and why should I use it?

To compile a simple C++ project, you might compile the source files on the command line by directly invoking the compiler:

```bash
g++ -std=c++20 main.cpp functionality.cpp -o main_executable -I/usr/local/include/eigen3 -lboost_program_options
```

This is fine for small projects, but it quickly becomes unwieldy as the number
of source files and dependencies increases. Moreover, it is not portable: the
above command will only work on systems with the Boost and Eigen libraries
installed in the specified locations. We might also have to change the compiler,
and would have to change the command line invocation.

Another option is to use a build tool like [Make](https://www.gnu.org/software/make/), which allows us to specify
the build process by defining rules. This can scale to larger projects, but
still is not completely portable as the rules are specific to Make, and we still
have to manually specify the compiler and libraries.

CMake is a build system *generator* that allows us to specify the build process for large project, in
a portable way, and to generate build files for a variety of build tools like
Unix Makefiles, Visual Studio projects, or the [Ninja build tool](https://ninja-build.org/). It has a lot
of features for finding dependencies and for making the build process more portable across
operating systems, compilers, and architectures. It has many features, but we
will only cover the basics here.

## Enter CMake

In [CMake](https://cmake.org/) you describe *targets* (what to build), *inputs* (the sources files),
and *configuration* (what libraries to use, what compiler settings, etc.).

From that description, CMake *generates* the rules for a build tool of your
choosing: a Makefile, a Ninja build file, an IDE project, or something else.
CMake doesn't build your project itself, and you don't write the rules by hand.

CMake works on Linux, Windows, macOS and more.
