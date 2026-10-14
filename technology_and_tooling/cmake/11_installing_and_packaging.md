---
name: Installing and packaging
dependsOn: [technology_and_tooling.cmake.10_testing]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Making a library consumable

We have built a library, `cmake_course_lib`, and used it within our own project.
The final step in modern CMake is to _install_ it so that a completely separate
project can find and link it with `find_package`, exactly as we did for Eigen and
Boost.

### Namespaced targets

First, give the library a namespaced alias:

```cmake
add_library(CMakeCourse::cmake_course_lib ALIAS cmake_course_lib)
```

Consumers link against `CMakeCourse::cmake_course_lib`. The `::` form is more than
cosmetic: CMake treats a name containing `::` as a target, so if you mistype it
you get an error at configure time rather than a confusing linker error later. It
also means consumers use the same name whether they get the library from
`add_subdirectory` or from an installed package.

### Include directories that survive installation

Until now our library's include directory pointed at its source tree:

```cmake
target_include_directories(cmake_course_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
```

That path is correct while building in place, but it is meaningless once the
library is installed somewhere else. Generator expressions let us give the right
answer for each case:

```cmake
target_include_directories(cmake_course_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
```

`$<BUILD_INTERFACE:...>` applies when building the project directly;
`$<INSTALL_INTERFACE:...>` applies to consumers of the installed package.

### Declaring the public headers

To install the headers, tell CMake which ones are public using a _file set_
(CMake 3.23 and later):

```cmake
target_sources(cmake_course_lib PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
  FILES functionality.hpp functionality_eigen.hpp)
```

## Installing

`install()` describes what ends up in the install tree. `GNUInstallDirs` gives us
the conventional destinations (`lib`, `include`, `bin`, ...) for the platform:

```cmake
include(GNUInstallDirs)

install(TARGETS cmake_course_lib
        EXPORT CMakeCourseTargets
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
```

The `EXPORT` keyword collects the target into an export set we can turn into a
CMake package.

:::callout{variant="warning"}
The first time you do this, `install(EXPORT ...)` is likely to fail with an error
about a target that "is not in any export set", naming `project_warnings`.

Even though we linked it `PRIVATE`, a _static_ library cannot carry its
dependencies inside itself, so CMake records them in the exported target's link
interface for consumers to link too. `project_warnings` is a local, un-exported
target, so the export cannot be written.

The warnings are only needed while building this project, so scope them to the
build:

```cmake
target_link_libraries(cmake_course_lib PRIVATE $<BUILD_INTERFACE:project_warnings>)
```

Real dependencies such as Eigen are different: consumers genuinely do need them,
which is why the package config re-finds Eigen below.
:::

## Generating a package so `find_package` works

Consumers use `find_package(CMakeCourse)`, which looks for a
`CMakeCourseConfig.cmake` file. We generate one, plus a matching targets file and
a version file.

```cmake
include(CMakePackageConfigHelpers)

install(EXPORT CMakeCourseTargets
        FILE CMakeCourseTargets.cmake
        NAMESPACE CMakeCourse::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CMakeCourse)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCourseConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeCourseConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CMakeCourse)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeCourseConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeCourseConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeCourseConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CMakeCourse)
```

The `CMakeCourseConfig.cmake.in` template is short: it pulls in the exported
targets, and re-finds the dependencies consumers need so they do not have to.

```cmake
@PACKAGE_INIT@

include(CMakeFindDependencyMacro)
find_dependency(Eigen3 CONFIG)

include("${CMAKE_CURRENT_LIST_DIR}/CMakeCourseTargets.cmake")
```

`find_dependency` is a thin wrapper around `find_package` that forwards the
`REQUIRED` and `QUIET` arguments from the outer `find_package(CMakeCourse ...)`
call, and reports a missing dependency as a failure to find _our_ package.

:::callout

## Why re-find Eigen when we linked it `PRIVATE`?

Because `cmake_course_lib` is **static**. A shared library resolves its private
dependencies into itself at link time, and consumers never hear about them. A
static library is only an archive of object files, so whoever links it must
supply everything it needs.

CMake therefore keeps a static library's `PRIVATE` dependencies in the exported
link interface, wrapped in `$<LINK_ONLY:...>`, which means "link this, but take
none of its usage requirements". You can see it in the generated
`CMakeCourseTargets.cmake`:

```cmake
set_target_properties(CMakeCourse::cmake_course_lib PROPERTIES
  INTERFACE_LINK_LIBRARIES "\$<LINK_ONLY:Eigen3::Eigen>")
```

That line names a _target_, and CMake requires every target in an imported
target's link interface to exist. Without `find_dependency(Eigen3 CONFIG)`,
nothing defines `Eigen3::Eigen` in the consumer's project, and configuring it
fails with an error about linking to a target that was not found.

This is the same rule that made `install(EXPORT ...)` reject `project_warnings`
above. The difference is that Eigen comes from a package the consumer can find
again, whereas `project_warnings` is a local target belonging to no package,
which is why one is re-found and the other is confined to the build interface.
:::

:::callout

## What if the library were shared?

The export machinery would barely change. `install(TARGETS ...)` already lists
`ARCHIVE`, `LIBRARY` and `RUNTIME` destinations precisely so it handles either
kind of library, and the `install(EXPORT ...)` and package-config steps are the
same.

A couple of things above would simply stop mattering: a shared library keeps its
private dependencies to itself, so `Eigen3::Eigen` would not reach the exported
interface and `project_warnings` would not upset `install(EXPORT ...)`. Both
lines are still worth keeping, in case the library type ever changes.

The extra work with a shared library is in _building_ it rather than exporting
it: controlling which symbols are visible (usually with `generate_export_header`,
whose generated header then needs installing too), setting `VERSION` and
`SOVERSION`, and making sure the consumer can find the library at run time as
well as at link time.
:::

Now anyone can install the project and consume it from an unrelated project. To
install, build as usual and then run `cmake --install`, choosing a prefix:

```bash
cmake --build build_dir
cmake --install build_dir --prefix ./stage
```

A prefix such as `./stage` inside the project keeps the experiment self-contained,
and avoids needing root to write to `/usr/local`. Have a look at what appears
there: the static library under `lib/`, the headers under `include/`, and the
package files under `lib/cmake/CMakeCourse/`.

## Consuming the installed package

A separate project now needs no knowledge of where our source tree lives. It
finds the package and links the namespaced target:

```cmake
find_package(CMakeCourse CONFIG REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE CMakeCourse::cmake_course_lib)
```

The one thing to tell CMake is where to look, which is what `CMAKE_PREFIX_PATH`
is for:

```bash
cmake -S . -B build_dir -DCMAKE_PREFIX_PATH=/absolute/path/to/checkpoint_7/stage
cmake --build build_dir
```

Everything else comes from the package: the include directory, the library to
link, and (through `find_dependency`) Eigen. This is the same mechanism we used
back in _Finding dependencies_ to consume Eigen and Boost, seen from the other
side.

## Packaging with CPack

CMake can also produce distributable archives and installers through **CPack**.
Set a few variables and include the module:

```cmake
set(CPACK_PACKAGE_NAME "ModernCMakeCourse")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
```

After building, run `cpack` from the build directory to produce a package:

```bash
cpack --config build_dir/CPackConfig.cmake -G TGZ
```

CPack can generate `.tar.gz`, `.zip`, `.deb`, `.rpm`, NSIS installers and more,
from the same install rules you already wrote.

::::challenge{id=install-the-library title="Install the library"}

Work in **Checkpoint 6**, the project you finished in the previous section. You
are going to turn `cmake_course_lib` into something another project can install
and use. All of the changes are in `src/CMakeLists.txt` unless stated otherwise.

1. **Give the library a namespaced alias**, so consumers can use the same name
   whether they build this project or find an installed copy:

   ```cmake
   add_library(CMakeCourse::cmake_course_lib ALIAS cmake_course_lib)
   ```

2. **Fix the include directories.** Replace the existing
   `target_include_directories(... INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})` with a
   `PUBLIC` version that gives one answer while building and another once
   installed, using `$<BUILD_INTERFACE:...>` and `$<INSTALL_INTERFACE:...>`. You
   will need `include(GNUInstallDirs)` for `${CMAKE_INSTALL_INCLUDEDIR}`.

3. **Declare the public headers** with a header file set, so CMake knows which
   headers to install and where they sit relative to the include root
   (`target_sources(... PUBLIC FILE_SET HEADERS BASE_DIRS ... FILES ...)`).

4. **Keep the warnings out of the exported interface.** Change the
   `project_warnings` link to
   `target_link_libraries(cmake_course_lib PRIVATE $<BUILD_INTERFACE:project_warnings>)`.
   If you skip this step, step 6 will fail: try it and read the error, it is one
   worth recognising.

5. **Add the install rules**: `install(TARGETS cmake_course_lib EXPORT
   CMakeCourseTargets ...)` with `ARCHIVE`, `LIBRARY`, `RUNTIME` and `FILE_SET
   HEADERS` destinations taken from `GNUInstallDirs`.

6. **Generate the package files** so that `find_package(CMakeCourse)` will work:

   - `install(EXPORT CMakeCourseTargets ... NAMESPACE CMakeCourse:: DESTINATION
     ${CMAKE_INSTALL_LIBDIR}/cmake/CMakeCourse)`
   - a `CMakeCourseConfig.cmake.in` template next to `src/CMakeLists.txt`, holding
     `@PACKAGE_INIT@`, a `find_dependency(Eigen3 CONFIG)`, and an `include()` of
     the targets file
   - `configure_package_config_file` and `write_basic_package_version_file` from
     `CMakePackageConfigHelpers`, plus an `install(FILES ...)` to put both
     generated files alongside the targets file

7. **Configure, build, and install** into a staging prefix inside the project, so
   you do not need root and can throw it away afterwards:

   ```bash
   cmake -S . -B build_dir
   cmake --build build_dir
   cmake --install build_dir --prefix ./stage
   ```

8. **Look at what you produced.** Explore `stage/`, and confirm you have:

   - the static library in `stage/lib/`
   - `functionality.hpp` and `functionality_eigen.hpp` in `stage/include/`
   - `CMakeCourseConfig.cmake`, `CMakeCourseConfigVersion.cmake` and
     `CMakeCourseTargets.cmake` in `stage/lib/cmake/CMakeCourse/`

   Open `CMakeCourseTargets.cmake` and find the `add_library(CMakeCourse::cmake_course_lib
   STATIC IMPORTED)` line, and the properties recording the include directory and
   the Eigen dependency. This file is the whole trick: it is generated from the
   target you described, so an installed copy behaves just like the one in your
   build tree.

Keep the `stage/` directory: the next challenge consumes it.

:::solution

The install rules go in `src/CMakeLists.txt`, next to the library they describe,
with `CMakeCourseConfig.cmake.in` alongside them.

Step 4 is the one most people trip over. Without the `$<BUILD_INTERFACE:...>`
wrapper, `install(EXPORT ...)` reports that `project_warnings` "is not in any
export set", because a static library's `PRIVATE` link dependencies still appear
in the exported target's link interface.

See Checkpoint 7 for the complete version.

:::

::::

::::challenge{id=consume-the-library title="Consume the installed library"}

Now play the part of somebody else: a developer who has our library installed,
has never seen its source tree, and just wants to use it. Start from the
`stage/` prefix you installed in the previous challenge (or install Checkpoint 7
to create one).

1. **Make a new directory** somewhere outside the checkpoint you have been
   working in, containing just two files: `CMakeLists.txt` and `main.cpp`. It is
   a project of its own, with its own `cmake_minimum_required` and `project()`
   calls, and it must not `add_subdirectory` anything from Checkpoint 7.

2. **Write `main.cpp`** so that it includes `functionality.hpp` by name, calls
   `hypergeometricPmf(10'000, 4'270, 300, 128)`, and prints the result.

3. **Find the package** in `CMakeLists.txt`:

   ```cmake
   find_package(CMakeCourse CONFIG REQUIRED)
   ```

   `CONFIG` insists on the config file we generated, rather than a
   `FindCMakeCourse.cmake` module, which does not exist.

4. **Add the executable and link the namespaced target**, and nothing else:

   ```cmake
   add_executable(consumer main.cpp)
   target_link_libraries(consumer PRIVATE CMakeCourse::cmake_course_lib)
   ```

   Resist the urge to add an include directory or to mention Eigen. If the
   install rules were right, they are unnecessary.

5. **Configure, telling CMake where to look.** `find_package` searches a list of
   prefixes, and yours is not one CMake would guess, so pass it explicitly as an
   absolute path:

   ```bash
   cmake -S . -B build_dir -DCMAKE_PREFIX_PATH=/absolute/path/to/checkpoint_7/stage
   ```

   `CMAKE_PREFIX_PATH` takes the prefix, not the directory containing the config
   file: CMake knows to look under `lib/cmake/CMakeCourse/` itself.

6. **Build and run it:**

   ```bash
   cmake --build build_dir
   ./build_dir/consumer
   ```

7. **Convince yourself it is really using the installed copy.** Try deleting (or
   renaming) the `stage/` prefix and configuring again from scratch: the
   `find_package` call should now fail. That failure is the point of the whole
   section, in that your project depends on the _package_, not on where the
   source happened to live.

:::solution

The whole of `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.24...3.31)
project(CMakeCourseConsumer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CMakeCourse CONFIG REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE CMakeCourse::cmake_course_lib)
```

And `main.cpp`:

```cpp
#include <iostream>

#include "functionality.hpp"

int main() {
    std::cout << "Hypergeometric probability: "
              << hypergeometricPmf(10'000, 4'270, 300, 128) << std::endl;
    return 0;
}
```

If `find_package` fails with "could not find a package configuration file", the
prefix is nearly always the culprit. It must be exactly the directory you passed
to `cmake --install --prefix`, given as an absolute path, and it must still
contain `lib/cmake/CMakeCourse/CMakeCourseConfig.cmake`.

If it instead fails while re-finding **Eigen3**, then the `find_dependency` call
in the config file is doing its job, and the machine simply needs Eigen where
CMake can see it.

See Checkpoint 8 for the complete version.

:::

::::

## That's all, folks

This was only a brief look at modern CMake. There are great resources available
for a deeper dive, and here are just a few of them.

**The book to buy, if you buy one:**

- [Professional CMake: A Practical Guide](https://crascit.com/professional-cmake/),
  by Craig Scott, published by Crascit. Written by a CMake co-maintainer, and
  revised for each CMake release, so it stays current in a way that most CMake
  writing does not. It covers everything in this course and a great deal more.

**Official documentation:**

- [The CMake reference documentation](https://cmake.org/cmake/help/latest/), which
  is where you will end up whenever you need the exact behaviour of a command.
- [The official CMake tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/),
  a step-by-step guide maintained by Kitware alongside CMake itself.
- [CMake Discourse](https://discourse.cmake.org/), the community forum, where
  CMake's own developers answer questions.

**Going further:**

- [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/), a
  free and regularly updated online book covering much the same target-based
  approach we have taken here.
- [Deep CMake for Library Authors](https://www.youtube.com/watch?v=m0DwB4OvDXk)
  (CppCon 2019), a talk by Craig Scott which goes considerably deeper into the
  install and export machinery of the previous section. Some of the version
  numbers have moved on, but the reasoning has not.
- [cmake-init](https://github.com/friendlyanon/cmake-init), a project generator
  that writes out a complete, well-structured CMake project. Worth reading even if
  you never run it, as a worked example of how the pieces fit together.

:::callout{variant="warning"}
CMake has been around a long time, and much of what you will find online is
written in the pre-target style: global variables, `include_directories`,
`link_libraries`, and manual compiler flags. A great deal of it still works, which
is exactly what makes it hard to spot.

If an example sets variables where it could set target properties, it is probably
out of date. Check the date of anything you are copying from, and prefer sources
that keep up to date with CMake releases.
:::
