---
name: Reproducible dependencies
dependsOn: [technology_and_tooling.cmake.08_build_presets]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Beyond `find_package`

In the earlier _Finding dependencies_ section, we used `find_package` to locate
libraries that are _already installed_ on the system. That is a common situation on a well-managed machine, but it puts the
burden on whoever builds the project to install the right libraries first, at
the right versions.

For builds that are reproducible anywhere, it helps to have the build itself
acquire its dependencies. There are two broad approaches: fetching sources with
CMake, or using a dedicated package manager.

## FetchContent

CMake's built-in `FetchContent` module downloads a dependency's source and adds
it to your build, so its targets are available just as if they were part of your
project:

```cmake
include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 12.2.0
)
FetchContent_MakeAvailable(fmt)

target_link_libraries(main_executable PRIVATE fmt::fmt)
```

`FetchContent_MakeAvailable` downloads the sources (the first time) and calls
`add_subdirectory` on them, defining the `fmt::fmt` target we then link against.

Here we are fetching [{fmt}](https://github.com/fmtlib/fmt), a formatting
library, and once it is linked we can use it in place of stream output:

```cpp
#include <fmt/format.h>

fmt::print("Hypergeometric probability: {:.4f}\n",
           hypergeometricPmf(10'000, 4'270, 300, 128));
```

:::callout
Always pin `GIT_TAG` to a specific tag or commit, not a branch like `main`.
Pinning is what makes the build reproducible: everyone gets the exact same
version of the dependency, and it will not change under you.
:::

### Fetch, or find if already installed

Downloading and building a dependency every time is wasteful if it is already
installed. Since CMake 3.24, `FetchContent_Declare` accepts `FIND_PACKAGE_ARGS`,
which tells `FetchContent_MakeAvailable` to try `find_package` first and only
fall back to downloading:

```cmake
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 12.2.0
  FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(fmt)
```

This gives you the best of both: fast builds where the library is installed, and
a self-contained build where it is not.

## Package managers (for information only)

:::callout{variant="info"}
The rest of this section is **background reading, and out of scope for this
course**. Package managers are worth knowing about, and you will meet them on
real projects, so we describe what they look like and where they fit. But there
is nothing to install, no exercise depends on them, and every checkpoint in this
course uses `FetchContent` instead. Follow the links below if you want to go
further on your own.
:::

For larger projects, a dedicated C++ package manager can cache and reuse
pre-built binaries and manage many dependencies at once. Two are widely used
with CMake.

### vcpkg

[vcpkg](https://vcpkg.io/) is maintained by Microsoft. Its
[getting started guide](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started)
and [CMake integration docs](https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration)
are the place to start if you want to try it.

Declare your dependencies in a `vcpkg.json` manifest at the project root:

```json
{
  "name": "intro-cmake-course",
  "version": "1.0.0",
  "dependencies": ["fmt", "eigen3", "boost-program-options"]
}
```

Then point CMake at vcpkg's toolchain file at configure time, and it installs
and exposes the dependencies for `find_package`:

```bash
cmake -S . -B build_dir \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Setting the toolchain file is a good thing to record in a preset so nobody has to
remember the path.

### Conan

[Conan](https://conan.io/) (version 2) is maintained by JFrog. Its
[tutorial](https://docs.conan.io/2/tutorial.html) and
[CMake integration docs](https://docs.conan.io/2/integrations/cmake.html) cover
the same ground for Conan.

Conan uses a `conanfile.txt` (or `conanfile.py`) to list dependencies:

```ini
[requires]
fmt/12.2.0

[generators]
CMakeDeps
CMakeToolchain
```

You install the dependencies and generate CMake integration in one step, then
configure using the toolchain and presets Conan wrote:

```bash
conan install . --build=missing
cmake --preset conan-release
```

## Which should I use?

- **`FetchContent`** needs no extra tools and keeps everything in-tree, which is
  ideal for small projects and for pulling in a few small header-only or
  source-only libraries.
- A **package manager** (vcpkg or Conan) is usually a better fit once you have
  many dependencies, want cached binaries, or need to share a locked set of
  versions across a team. Which of the two to pick tends to come down to what
  your dependencies, your collaborators, and your CI already use.

For this course, the answer is `FetchContent`: it is what the checkpoints use,
and it needs nothing beyond CMake itself.

::::challenge{id=fetch-fmt title="Fetch and use {fmt}"}

Starting from Checkpoint 5, fetch [{fmt}](https://github.com/fmtlib/fmt) version
`12.2.0` with `FetchContent`, link it to `main_executable`, and replace some of
the `std::cout` output in `main.cpp` with `fmt::print`.

Unlike Eigen and Boost, you should not need to install anything first: the build
acquires `fmt` for you.

:::solution

In `exe/CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 12.2.0
)
FetchContent_MakeAvailable(fmt)

target_link_libraries(main_executable PRIVATE fmt::fmt)
```

And in `main.cpp`:

```cpp
#include <fmt/format.h>

fmt::print("Hypergeometric probability: {:.4f}\n",
           hypergeometricPmf(10'000, 4'270, 300, 128));
```

The first configure will be slower, since CMake clones and builds `fmt`. After
that it is cached in the build directory.

Checkpoint 6 has this, alongside the tests we add in the next section.

:::

::::
