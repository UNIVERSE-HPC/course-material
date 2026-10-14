---
name: Configuring the build
dependsOn: [technology_and_tooling.cmake.02_a_first_cmake_project]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## CMake configuration

### Choosing a generator

CMake can create more than Makefiles. It can generate IDE projects, or build descriptions for the fast [Ninja](https://ninja-build.org/) tool.

To try that out, you'll need a clean build directory. In CMake ≥ 3.24, you can
achieve that with the `--fresh` flag. With older CMake, you can carefully delete
the current contents of your build directory.

```bash
cmake --fresh -S . -B build_dir -G Ninja
[...]

cmake --build build_dir
[4/4] Linking CXX executable main_executable
```

You can build the same way regardless of the generator, because `cmake --build`
drives whichever tool CMake generated for:

```bash
cmake --build build_dir --target main_executable
```

This can be particularly useful in automated scripts that may be run on
different systems using different generators.

### Setting configuration

You (and users) can override choices made by CMake using the `-D` argument.
For instance, you might want to use a different compiler:

```bash
cmake -S . -B build_dir -DCMAKE_CXX_COMPILER=clang++
```

```output
-- The CXX compiler identification is Clang 21.1.8
[...]
```

Another common task is setting the optimisation level. CMake provides several
default configurations:

- **Debug**: no optimisation, with debug symbols, for stepping through code in a
  debugger.
- **Release**: fully optimised, no debug symbols, and assertions disabled. What
  you would ship to users.
- **RelWithDebInfo**: optimised, but keeping debug symbols, which is useful for
  profiling or for debugging a problem that only appears in an optimised build.
- **MinSizeRel**: optimised for small binary size rather than speed, which
  matters mostly on embedded targets.

```bash
cmake -S . -B build_dir -DCMAKE_BUILD_TYPE=Release
```

The default flags with the `g++` compiler are:

```cmake
CMAKE_CXX_FLAGS_DEBUG            -g
CMAKE_CXX_FLAGS_MINSIZEREL       -Os -DNDEBUG
CMAKE_CXX_FLAGS_RELEASE          -O3 -DNDEBUG
CMAKE_CXX_FLAGS_RELWITHDEBINFO   -O2 -g -DNDEBUG
```

::::challenge{id=cmake-config title="CMake configuration"}

Try using the Ninja generator, compiling in Release mode, and using another compiler if you have one installed.

Remember that you might have to use `--fresh` to clean your build directory when, e.g., changing generator.

::::
