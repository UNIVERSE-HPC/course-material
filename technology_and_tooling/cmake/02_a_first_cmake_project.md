---
name: A first CMake project
dependsOn: [technology_and_tooling.cmake.01_introduction]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Getting Started

The course repository contains a number of checkpoints, each of which is a
step through this course. Each checkpoint contains a small C++ project along with a `CMakeLists.txt` file.

Checkpoint 0 is a simple "hello, world" program written in C++. Let's use CMake to build it.

```bash
cd checkpoint_0
```

### `CMakeLists.txt`

The `CMakeLists.txt` file is where you write the definitions of your targets and
configuration, and is processed by CMake to generate the build files. It is
written in the CMake language, which is a bit like a scripting language.

Let's look at the sample `CMakeLists.txt` line by line.

### CMake has changed a lot

Often the first line of a `CMakeLists.txt` file describes the minimum version of
CMake required, this is because CMake has changed a lot over the years, and some
features are only available in newer versions.

```cmake
cmake_minimum_required(VERSION 3.24...3.31)
```

This tells CMake which versions we support. The first number (`3.24`) is the
minimum version needed to configure the project. The second (`3.31`) is the most
recent version whose behaviour we have checked against: CMake changes its
defaults over time through _policies_, and this upper bound stops a newer CMake
from silently changing how our project builds.

### Define a project

Then we define a project by giving it a name and specifying the languages used.

```cmake
project(ModernCMakeCourse
        VERSION 1.0.0
        DESCRIPTION "Modern CMake course"
        LANGUAGES CXX)
```

We have a project called `ModernCMakeCourse`, at version `1.0.0`, written in C++.
Giving the project a `VERSION` and `DESCRIPTION` is good practice: CMake exposes
them as variables such as `PROJECT_VERSION`, and reuses them later when we install
or package the project.

### Configure the compiler

Often we want to target a particular version of the C++ standard, and we can do that like so:

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

We're using the [C++20](https://en.cppreference.com/w/cpp/20) language standard.
On its own, `CMAKE_CXX_STANDARD` is only a _request_: if the compiler doesn't
support it, CMake will quietly fall back to an older standard. Setting
`CMAKE_CXX_STANDARD_REQUIRED` to `ON` turns that into a hard error instead.
Setting `CMAKE_CXX_EXTENSIONS` to `OFF` asks for standard C++ (e.g. `-std=c++20`)
rather than compiler-specific extensions (e.g. `-std=gnu++20`), which keeps the
build portable across compilers.

These variables set the standard project-wide. Once you have defined a target,
you can also request a standard on a _per-target_ basis, which is often clearer
and, for a library, propagates the requirement to anything that links it:

```cmake
target_compile_features(main_executable PUBLIC cxx_std_20)
```

We use the project-wide variables here for simplicity.

### Tell it what to build

Now we can tell CMake what to build. We do this with the `add_executable` command:

```cmake
add_executable(main_executable main.cpp)
```

This defines a _target_ called `main_executable`, built from the source code in
`main.cpp`. A target is the unit CMake works in: something to be built, together
with everything known about how to build it. We will have a lot more to say
about targets later in the course.

### Using CMake

That is our `CMakeLists.txt`, and now we can use CMake to configure our project.
This is the first step in the compilation, during which CMake uses our
`CMakeLists.txt` file to generate the build files. If we are using a Linux
distribution like Ubuntu, it is likely that we will be using Make, so in this
case CMake will generate a Makefile that contains instructions to build our project.

It's typical to build "out of tree", keeping the generated files in a separate
directory. This keeps them out of your source folder and lets you keep several
builds side by side, for instance for different compilers or configurations. We
point CMake at the source directory with `-S` and at a build directory with `-B`,
which it creates for us:

```bash
# from checkpoint_0
cmake -S . -B build_dir
```

```output
[...]
-- Build files have been written to: <...>/checkpoint_0/build_dir
```

:::callout
Adding `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` at configure time writes a
`compile_commands.json` file into the build directory. Editors and language
servers such as clangd read this file to understand exactly how each source is
compiled, giving you accurate navigation, completion, and diagnostics.

Try that now, and have a look inside the `compile_commands.json` that it generates:

```bash
# from checkpoint_0
cmake -S . -B build_dir -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

It should look something like:

```json
[
{
  "directory": "<>/ModernCMakeCourse/checkpoint_0/build_dir",
  "command": "/usr/bin/c++   -std=c++20 -o CMakeFiles/main_executable.dir/main.cpp.o -c <>/ModernCMakeCourse/checkpoint_0/main.cpp",
  "file": "<>/ModernCMakeCourse/checkpoint_0/main.cpp",
  "output": "CMakeFiles/main_executable.dir/main.cpp.o"
}
]
```

:::

### Build your project

CMake only generated the build files, it didn't compile anything yet. We ask
CMake to drive the build for us:

```bash
cmake --build build_dir
```

```output
[...]
[100%] Built target main_executable
```

Using `cmake --build` rather than calling `make` (or `ninja`) directly means the
same command works whatever build tool CMake generated for, which is convenient
in scripts and on other people's machines.

We can then run the executable:

```bash
./build_dir/main_executable
```

```output
Checkpoint 0
Hello, World!
```

::::challenge{id=cmake-workflow title="CMake workflow"}
Verify that you can configure, compile and run the executable in Checkpoint 0.
::::
