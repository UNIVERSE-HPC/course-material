---
name: Testing with CTest
dependsOn: [technology_and_tooling.cmake.09_reproducible_dependencies]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## CTest

CMake ships with a test runner, **CTest**. You register tests in your
`CMakeLists.txt`, and CTest discovers and runs them, reports pass or fail, and
integrates with continuous integration.

Turn testing on in your top-level `CMakeLists.txt`:

```cmake
include(CTest)
```

`include(CTest)` calls `enable_testing()` for you and also adds a `BUILD_TESTING`
option (on by default), which lets consumers of your project skip building the
tests.

## A first test

The simplest test just runs a program and checks its exit code: a non-zero exit
is a failure.

```cmake
add_test(NAME runs_ok COMMAND main_executable)
```

Configure and build as usual, then run the tests from the build directory:

```bash
ctest --test-dir build_dir --output-on-failure
```

```output
Test project .../build_dir
    Start 1: runs_ok
1/1 Test #1: runs_ok .......................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1
```

## Unit tests with a framework

Exit codes only get you so far. For real unit tests you want a testing framework.
We can pull one in with `FetchContent`, exactly as in the previous section. Here
we use [Catch2](https://github.com/catchorg/Catch2):

```cmake
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.15.2
)
FetchContent_MakeAvailable(Catch2)
```

Write a test executable that links your library and Catch2, then let CMake
discover the individual test cases:

```cmake
add_executable(unit_tests test_functionality.cpp)
target_link_libraries(unit_tests PRIVATE cmake_course_lib Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(unit_tests)
```

Linking `Catch2::Catch2WithMain` gives you a `main()`, so the test file only has
to contain test cases. `catch_discover_tests` then runs the test binary at build
time to find every `TEST_CASE` and registers each as a separate CTest test, so
`ctest` reports them individually.

:::callout
`catch_discover_tests` lives in `Catch.cmake`, which Catch2 ships in an `extras/`
directory rather than installing as a module. That is why we add
`${Catch2_SOURCE_DIR}/extras` to `CMAKE_MODULE_PATH` before `include(Catch)`. If
you get Catch2 from a package manager instead of `FetchContent`, its config
package puts `Catch.cmake` on the module path for you and the extra line is
unnecessary.
:::

A small test file might look like:

```cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "functionality.hpp"

TEST_CASE("Hypergeometric PMF peaks near the expected value", "[hypergeometric]") {
    REQUIRE(hypergeometricPmf(10'000, 4'270, 300, 128)
            == Catch::Approx(0.047).margin(0.02));
}
```

:::callout
[GoogleTest](https://github.com/google/googletest) is a popular alternative, and
the shape is the same: fetch it, link `GTest::gtest_main`, and use
`gtest_discover_tests` (from the built-in `GoogleTest` module) in place of
`catch_discover_tests`.
:::

::::challenge{id=add-tests title="Add tests to the library"}

Carrying on from the previous challenge, in the project where you have just added
`fmt`, enable testing in the top-level `CMakeLists.txt`, fetch Catch2, and add a
`tests/` directory with a test executable that links `cmake_course_lib` and
checks one of its functions. Build and run the tests with
`ctest --test-dir build_dir --output-on-failure`.

:::solution

In the top-level `CMakeLists.txt`:

```cmake
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
```

In `tests/CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.15.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests test_functionality.cpp)
target_link_libraries(unit_tests PRIVATE cmake_course_lib Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(unit_tests)
```

See Checkpoint 6 for a complete version.

:::

::::
