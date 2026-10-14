---
name: Options and compiler warnings
dependsOn: [technology_and_tooling.cmake.06_finding_dependencies]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Adding CMake functionality using `include`

Any file containing valid CMake syntax can be "included" in the
current `CMakeLists.txt`. This is similar to the `#include`{.cpp} directive in C++, and is
useful for splitting up large `CMakeLists.txt` files into smaller, more
manageable chunks, for using additional CMake functionality, or for reusing
functionality across projects.

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.24...3.31)
project(ModernCMakeCourse LANGUAGES CXX)
include(cmake/file_to_include.cmake)

set(name "Foo Bar")
message(STATUS "Hello ${name}")

# cmake/file_to_include.cmake
set(name "Jane Doe")
message(STATUS "Hello ${name}")
```

```text
-- Hello Jane Doe
-- Hello Foo Bar
-- Configuring done
...
```

## More programming CMake

CMake has a number of programming constructs that you can use to make your
`CMakeLists.txt` files more powerful.

### conditionals

Conditionals are used to control the flow of the CMake script. They are similar
to conditionals in other languages, but are evaluated at configure time, not
build time. Note the round brackets around the if-condition, else and endif:

```cmake
if(expression)
    # Do something
else()
    # Do something else
endif()
```

### loops

CMake has a `foreach` loop construct:

```cmake
set(mylist A B C D)
foreach(var IN LISTS mylist)
    message(${var})
endforeach()
```

### functions

CMake allows the declaration of reusable functions:

```cmake
function(add a b)
    math(EXPR result "${a}+${b}")
    message("The sum is ${result}")
endfunction()
```

Functions cannot return a value.

Functions introduce a new scope.

A similar notion is CMake _macros_, which does **not** introduce a new scope.

## Setting options with `option()`

Boolean variables can be declared using `option()`:

```cmake
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" TRUE)
```

This is useful for allowing users to configure the build process. The value of
options can be specified at the command line using the `-D` syntax:

```bash
cmake -S . -B build_dir -DWARNINGS_AS_ERRORS=FALSE
```

Options are a special case of "cache" variable, whose value persist
between CMake runs. You can see the value of all cache variables by looking at
the file `CMakeCache.txt` in the build directory.

## Built-in CMake variables

CMake provides _a lot_ of pre-defined variables whose values describe your system.

For instance, the value of `CMAKE_CXX_COMPILER_ID` can be queried
to determine which C++ compiler is used.

```cmake
if(MSVC)
    set(PROJECT_WARNINGS ${MSVC_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
else()
    # ...
```

This is useful to add compiler-specific flags to make your project more
portable, and there are similar variables for operating systems, architectures,
and more. You can find a list of all the built-in variables in the [CMake documentation](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html).

## Using an interface "library" to apply options across targets

A useful technique for adding options to targets, for instance adding compiler
flags to use with a library, is to create an empty "library" and set the
compiler flags you wish on this target, and then link that against your other
targets so that they inherit the flags.

Let's see how that works, in Checkpoint 5...

:::callout
This interface-"library" pattern is a flexible way to share any compile options
across targets. If all you need is to promote warnings to errors, recent CMake
(3.24 and later) has a built-in switch: set the target property
`COMPILE_WARNING_AS_ERROR`, or configure with
`-DCMAKE_COMPILE_WARNING_AS_ERROR=ON`. You still choose _which_ warnings to
enable separately, which is what `CompilerWarnings.cmake` does here.
:::

::::challenge{id=compiler-options title="Setting compiler options"}

Look at Checkpoint 5. We have a new directory `cmake/` containing a file
`CompilerWarnings.cmake` that sets compiler flags to enable many different
warnings for MSVC, Clang and GCC. We've also added an option
`WARNINGS_AS_ERRORS` that can be used to treat warnings as errors.

The compiler should now warn us about bad C++. This is encouraged!

Add some bad C++ to `main.cpp`, for instance:

```cpp
int unused_variable = 0;
```

Do you get a compiler warning? An error? Try configuring `WARNINGS_AS_ERRORS` and see the difference:

```bash
cmake -S . -B build_dir -DWARNINGS_AS_ERRORS=ON
```

```bash
cmake -S . -B build_dir -DWARNINGS_AS_ERRORS=OFF
```

::::
