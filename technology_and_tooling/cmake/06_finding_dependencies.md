---
name: Finding dependencies
dependsOn: [technology_and_tooling.cmake.05_targets_and_libraries]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Finding dependencies

Libraries can be installed in various locations on your system. CMake makes it
easy to link against libraries **without having to know where they are
installed**:

```cmake
find_package(library_name CONFIG REQUIRED)
```

The above defines a new target that can now be linked against other targets using
`target_link_libraries`. The target is usually _namespaced_, of the form
`Package::Component` or `Package::Package`, for instance `Eigen3::Eigen`.

### "config" mode for `find_package`

In "config mode", `find_package` will search for a
`<PackageName>Config.cmake` file.

This file specifies all the information CMake needs (particularly where
the library is installed).

This is usually given by the library vendor.

These files are installed alongside the libraries themselves, so you can see
what your own system already provides. On a Debian-based system, most of them
live under `/usr/lib/x86_64-linux-gnu/cmake/`:

```bash
ls /usr/lib/x86_64-linux-gnu/cmake/
```

```output
[...]  Boost-1.83.0  boost_headers-1.83.0  boost_program_options-1.83.0  [...]
```

Architecture-independent packages, such as header-only libraries, tend to live
under `/usr/share/` instead. Eigen is one of these:

```bash
ls /usr/share/eigen3/cmake/
```

```output
Eigen3Config.cmake  Eigen3ConfigVersion.cmake  Eigen3Targets.cmake  UseEigen3.cmake
```

Both are on CMake's default search path, so `find_package` locates them without
being told where to look. A package installed somewhere else, for instance into
`/opt` or a directory in your home, is found by adding that prefix to
`CMAKE_PREFIX_PATH`, which we return to when we install a library of our own.

::::challenge{id=adding-eigen-dep title="Adding the Eigen dependency"}

Look at Checkpoint 3. A new file `src/functionality_eigen.cpp` depends on the
[Eigen](https://libeigen.gitlab.io/) library for linear algebra.

Task: Using `find_package`, modify the `CMakeLists.txt` in directory `src/` to
link target `cmake_course_lib` against Eigen.

_Hint: Useful instructions can be found at [Using Eigen in CMake Projects](https://libeigen.gitlab.io/eigen/docs-5.0/TopicCMakeGuide.html)._

Note that keyword `NO_MODULE` is equivalent to `CONFIG`.

:::solution

Your `src/CMakeLists.txt` should look like this:

```cmake
find_package(Eigen3 CONFIG REQUIRED)
message(STATUS "Found Eigen3 ${Eigen3_VERSION}")

add_library(cmake_course_lib STATIC
        functionality.cpp
        functionality.hpp
        functionality_eigen.cpp
        functionality_eigen.hpp
)
target_include_directories(cmake_course_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cmake_course_lib PRIVATE Eigen3::Eigen)
```

:::
::::

### "module" mode for `find_package`

Libraries don't always come with a CMake config file
`<PackageName>Config.cmake`.

CMake can also find the library based on a file `Find<PackageName>.cmake`.
This behaviour corresponds to using `find_package` with the keyword `MODULE`:

```cmake
find_package(library_name MODULE REQUIRED)
```

`Find<PackageName>.cmake` files are cmake language scripts that try to find the
library on the system, they are often specific to a particular library and look
in specific locations that the library might be found in different operating
systems (e.g. `/usr/lib`, `/usr/local/lib`).

Such _module files_ are provided by CMake itself for common libraries,
they can also be written for a particular use case if required.

### Package components

Often libraries are split into different components. E.g. Boost: filesystem,
thread, date-time, program-options, numpy...

Most programs only rely on a subset of components, and so it is useful to be
able to find and link against only the components that are needed using `find_package`.

```cmake
set(boost_components filesystem chrono)
find_package(Boost CONFIG REQUIRED COMPONENTS ${boost_components})
```

The CMake target for a component is `<PackageName>::<ComponentName>`
(_e.g._ `Boost::filesystem`).

:::callout
Boost used to be the classic example of a library found in _module_ mode, but
modern Boost (1.70 and later) ships its own `BoostConfig.cmake`, so we use
_config_ mode (`CONFIG`) here. In fact, CMake's bundled `FindBoost` module was
deprecated in CMake 3.30 (policy `CMP0167`), so `CONFIG` is now the recommended
way to find Boost. _Module_ mode is still important for the many libraries that
only ship a `Find<PackageName>.cmake` module.
:::

::::challenge{id=adding-boost-dep title="Adding the Boost dependency"}

Look at Checkpoint 4. The executable `exe/main.cpp` depends on the [Boost Program Options](https://www.boost.org/doc/libs/latest/doc/html/program_options.html)
library for handling command line arguments.

Task: Using `find_package` in `CONFIG` mode, modify the `CMakeLists.txt` in directory `exe/` to
find and link target `main_executable` against `Boost::program_options`.

:::solution
Your `exe/CMakeLists.txt` should look like this:

```cmake
find_package(Boost CONFIG REQUIRED COMPONENTS program_options)
message(STATUS "Found Boost ${Boost_VERSION}")

add_executable(main_executable main.cpp)
target_link_libraries(main_executable PRIVATE cmake_course_lib)
target_link_libraries(main_executable PRIVATE Boost::program_options)
```

:::
::::
