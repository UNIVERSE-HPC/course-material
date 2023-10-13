---
name: Introduction to CMake
dependsOn: [
  technology_and_tooling.ide.cpp
]
tags: [cpp]
attribution: 
    - citation: >
        "Introduction to CMake" course developed by Fergus Cooper and the Oxford Research 
        Software Engineering group
      url: https://github.com/OxfordRSE/IntroCMakeCourse
      image: https://www.rse.ox.ac.uk/images/banner_ox_rse.svg
      license: CC-BY-4.0
---

### Course materials

Clone the material repository and change your current working directory to the project 
root:

~~~bash
git clone https://github.com/OxfordRSE/IntroCMakeCourse
cd IntroCMakeCourse
~~~

## What is CMake and why should I use it?

To compile a simple C++ project, you might compile the source files on the command line by directly invoking the compiler:

~~~bash
g++ -std=c++17 main.cpp functionality.cpp -o main_executable -I/usr/local/include/eigen3 -lboost_program_options
~~~

This is fine for small projects, but it quickly becomes unwieldy as the number
of source files and dependencies increases. Moreover, it is not portable: the
above command will only work on systems with the Boost and Eigen libraries
installed in the specified locations. We might also have to change the compiler,
and would have to change the command line invocation.

Another option is to use a build system like Make, which allows us to specify
the build process by defining rules. This can scale to larger projects, but
still is not completely portable as the rules are specific to Make, and we still
have to manually specify the compiler and libraries.

CMake is a build system generator that allows us to specify the build process for large project, in
a portable way, and to generate build files for a variety of build systems like
Unix Makefiles, Visual Studio projects, or the Ninja build system. It has a lot
of features for finding dependencies and for making the build process more portable across
operating systems, compilers, and architectures. It has many features, but we
will only cover the basics here.


## Enter CMake

In [CMake](https://cmake.org/) you describe _targets_ (what to build), _inputs_ (the sources files),
and _configuration_ (what libraries to use, what compiler settings, etc.).

CMake uses that with its own _rules_ (how to turn sources into programs) to
generate makefiles, IDE projects, or other outputs. CMake doesn't build your
project itself!

CMake works on Linux, Windows, macOS and more.

## Getting Started

The course repository contains a number of checkpoints, each of which is a
step through this course. Each checkpoint contains a small C++ project along with a `CMakeLists.txt` file.

Checkpoint 0 is a simple "hello, world" program written in C++. Let's use CMake to build it.

```bash
$ cd checkpoint_0
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
cmake_minimum_required(VERSION 3.13)
```

tells CMake which version we used, affecting the features available and the
interpretation of `CMakeLists.txt`

### Define a project

Then we define a project by giving it a name and specifying the languages used.

```cmake
project(IntroCMakeCourse LANGUAGES CXX)
```

We have a project called `IntroCMakeCourse`, in the C++ language.

### Configure the compiler

Often we want to target a particular version of the C++ standard, and we can do that like so:

```cmake
set(CMAKE_CXX_STANDARD 17)
```

We're using the [C++17](https://en.cppreference.com/w/cpp/17) language dialect.

### Tell it what to build

Now we can tell CMake what to build. We do this with the `add_executable` command:

```cmake
add_executable(main_executable main.cpp)
```

There is a program, called `main_executable`, which depends on the source code in `main.cpp`

### Using CMake

That is our `CMakeLists.txt`, now we can use CMake to configure our project.
This is the first step in the compilation, during which CMake uses our
`CMakeLists.txt` file to generate the build files. If we are using a Linux
distribution like Ubuntu, it is likely that we will be using Make, so in this
case CMake will generate a Makefile that can build our project.

It's typical to build "out of tree", by running CMake in a separate place. This keeps
generated files out of your source folder and allows you to have multiple
builds, for instance for different compilers or build configurations.

```bash
checkpoint0$ mkdir build
checkpoint0$ cd build
build$ cmake ..
[...]
-- Build files have been written to: <...>/checkpoint_0/build
```

### Build your project

CMake only generated the build script, it didn't actually compile anything. To compile the project, we use the build system that CMake generated, in this case Make:

```bash
build$ make
[...]
[100%] Built target main_executable
build$ ./main_executable
Checkpoint 0
Hello, World!
```

We can also use CMake to build the project. This does the same thing as the
`make` command above, but is more portable as it works with other build systems
too.

```bash
build$ cmake --build .
[...]
[100%] Built target main_executable
```

::::challenge{id=cmake-workflow title="CMake workflow"} 
Verify that you can configure, compile and run the executable in Checkpoint 0.
::::

## CMake configuration

### Choosing a generator

CMake can create more than Makefiles. It can generate IDE projects, or build descriptions for the fast Ninja tool.

```bash
build$ cmake -G Ninja ..
[...]

build$ ninja
[2/2] Linking CXX executable main_executable
```

You can build uniformly, regardless of the generator:

```bash
build$ cmake -G Ninja ..
build$ cmake --build . --target main_executable
```

This can be particularly useful in automated scripts that may be run on
different systems using different generators.

### Setting configuration

You (and users) can override choices made by CMake using the `-D` argument.

```bash
build$ cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-10 ..
-- Configuring done
You have changed variables that require your cache to be deleted.
Configure will be re-run and you may have to reset some variables.
The following variables have changed:
CMAKE_CXX_COMPILER= /usr/local/bin/g++-10

-- The CXX compiler identification is GNU 10.2.0
[...]
```

You can switch between Debug, Release, RelWithDebInfo and MinSizeRel, by default:

```bash
build$ cmake -DCMAKE_BUILD_TYPE=Release ..
[...]
```

The default flags with `g++` are:

```cmake
CMAKE_CXX_FLAGS_DEBUG            -g
CMAKE_CXX_FLAGS_MINSIZEREL       -Os -DNDEBUG
CMAKE_CXX_FLAGS_RELEASE          -O3 -DNDEBUG
CMAKE_CXX_FLAGS_RELWITHDEBINFO   -O2 -g -DNDEBUG
```

::::challenge{id=cmake-config title="CMake configuration"} 

Try using the Ninja generator, compiling in Release mode, and using another compiler if you have one installed.

Remember that you might have to clean your build directory when, e.g., changing generator.

::::


## Checkpoint 1

Now look at the next checkpoint in the `checkpoint_1` subdirectory, which looks like this:

```bash
CMakeLists.txt/
src/
    CMakeLists.txt
    functionality.cpp
    functionality.hpp
    main.cpp
```

Our project has grown! In addition to the code in `main.cpp`, some new
functionality was added to new source files `functionality.cpp` and
`functionality.hpp`.

This code is now contained in a specific directory `src/`, inside the project
directory.

### Compartmentalising build logic

In the top-level `CMakeLists.txt`, we use the `add_subdirectory` cmake function
to add the `src` directory to the project:

```cmake
add_subdirectory(src)
```

This causes CMake to processes the `CMakeLists.txt` file in the directory `src`.


```bash
# src/CMakeLists.txt
set(src_source_files file1 file2 file3)
add_executable(executable ${src_source_files})
```

Variables defined in the upper scope are available in the lower scope, but not
the other way around.

Using subdirectories enables clear structure and modularity, and keeps the
top-level `CMakeLists.txt` clean and tidy.

### Programming CMake

Lets have a look at some of the features of the CMake language.

Like many languages you can create variables, which you can do by simply
assigning a value via the `set` function:

```cmake
set( my_var "Hello, World!" )
```

This creates a variable called `my_var` and assigns it the string "Hello, World!".

Variables can hold lists:

```cmake
set( src_files main.cpp functionality.cpp functionality.hpp )
```

Variables can be dereferenced using `${}`:

```cmake
set(another_list ${src_files})
```

Here the value of `another_list` is set to the value of `src_files`.

You can even store a variable name or part of a variable name in another variable. For example:

```cmake
set(var files) # var = "files"
set(yet_another_list ${src_${var}})
```

::::challenge{id=add-function title="Adding new files to the project"} 

Look through the files in Checkpoint 1.

Add a new pair of hpp/cpp files that defines a new function.

- Call it from the main executable
- Add the files to `src/CMakeLists.txt`
- Configure, compile and run: check that your new function has been executed

:::solution

`src/CMakeLists.txt` should look like this:

```cmake
set(
        src_source_files
        functionality.cpp
        new_function.cpp
        main.cpp
)

set(
        src_header_files
        functionality.hpp
        new_function.hpp
)

add_executable(main_executable ${src_source_files} ${src_header_files})
```

`src/new_function.cpp` should look like this:

```cpp
#include "new_function.hpp"
#include <iostream>

void new_function()
{
    std::cout << "New function called" << std::endl;
}
```

`src/new_function.hpp` should look like this:

```cpp
#ifndef NEW_FUNCTION_HPP
#define NEW_FUNCTION_HPP

void new_function();

#endif
```

`src/main.cpp` should look like this:

```cpp
#include <iostream>
#include "new_function.hpp"
#include "functionality.hpp"

int main() {
    std::cout << "Checkpoint 2" << std::endl;
    std::cout << "Hypergeometric probability: " << hypergeometricPmf(10'000, 4'270, 300, 128) << std::endl;
    new_function();
    return 0;
}
```
:::

::::

## Targets

Targets are the basic building blocks of CMake. In checkpoint 1, we have a
single target, the executable `main_executable`. You could also have a
library target, or a test target, or a custom target that runs a script.

### Target properties

CMake allows for a very fine-grained control of target builds, through
*properties*.

For example, the property `INCLUDE_DIRECTORIES` specifies the list of
directories to be specified with the compiler switch `-I` (or `/I`).

Properties can be set manually like variables, but in general CMake provides
commands for it:

```cmake
target_include_directories(main_executable
                            PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}
)
```

*Properties are different from variables!*


### Creating a library target

Creating a library target is similar to an executable target, but uses the
`add_library` command:

```cmake
add_library(my_lib STATIC ${source_files})
```

Use `SHARED` instead of `STATIC` to build a shared library: or,
if omitted, CMake will pick a default based on the value of the variable
`BUILD_SHARED_LIBS`.

### Linking libraries (`PRIVATE`)

Library dependencies can be declared using the `target_link_libraries()` command:

```cmake
target_link_libraries(another_target PRIVATE my_lib)
```

The `PRIVATE` keyword states that `another_target` uses `my_lib` only in its internal
implementation. Programs using `another_target` don't need to know about `my_lib`.

### Linking libraries (`PUBLIC`)

Picture another dependency scenario:

-   `another_target` uses `my_lib` in its internal implementation.
-   **and** `another_target` defines some function that take parameters of a type defined
    in `my_lib`.

Programs using `another_target` also must link against `my_lib`:

```cmake
target_link_libraries(another_target PUBLIC my_lib)
```

### Link libraries (`INTERFACE`)

Picture another dependency scenario:

- `another_target` only uses `my_lib` in its interface.
- **but not** in its internal implementation.

```cmake
target_link_libraries(another_target INTERFACE my_lib)
```

### Behaviour of target properties across dependencies

Target properties are paired with another property
`INTERFACE_<PROPERTY>`. For instance

    INTERFACE_INCLUDE_DIRECTORIES

These properties are inherited by depending targets (such as
executables and other libraries).

Example:

    target_include_directories(my_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

-   `PRIVATE`: sets `INCLUDE_DIRECTORIES`.
-   `INTERFACE`: sets `INTERFACE_INCLUDE_DIRECTORIES`.
-   `PUBLIC`: sets both.


::::challenge{id=make-a-library title="Make a library"} 

Let's separate the functionality from the executable itself:

```bash
CMakeLists.txt
src/
    <library>
exe/
    <executable>
```

Tasks:

1. Modify `src/CMakeLists.txt` so that a static library is created out
   of `functionality.cpp` and `functionality.hpp`.
2. Move `main.cpp` into a new directory `exe`, and add a `CMakeLists.txt` defining a new
   target that links against the library.
3. Modify the top-level `CMakeLists.txt` so that it processes both directories.

:::solution

Have a look at Checkpoint 2 for a solution.

:::

::::

### Print information with `message()`

You often need to print information during the configuration step. This can be
done with the `message()` command:

```cmake
set(name "Jane Doe")
message(STATUS "Hello ${name}")
```
```
-- The C compiler identification is GNU 8.3.0
...
-- Hello Jane Doe
-- Configuring done
-- Generating done
```

### Options for `message()`

```cmake
message(STATUS "A simple message")
```

`STATUS` can be replaced by *e.g.* `WARNING`, `SEND_ERROR`, `FATAL_ERROR`
depending on the situation.

```cmake
message(SEND_ERROR "An error occurred but configure step continues")
```
```
CMake Error at CMakeLists.txt:2 (message):
    An error occurred but configure step continues

-- Configuring incomplete, errors occurred!
```


## Finding dependencies

Libraries can be installed in various locations on your system. CMake makes it
easy to link against libraries **without having to know where they are
installed**:

```cmake
find_package(library_name CONFIG REQUIRED)
```

The above defines a new target (usually named `library_name`) that can now be linked
against other targets using `target_link_libraries`.

### "config" mode for `find_package`

```cmake
find_package(library_name CONFIG REQUIRED)
```

In "config mode", `find_package` will search for a
`<PackageName>Config.cmake` file.

This file specifies all the information CMake needs (particularly where
the library is installed).

This is usually given by the library vendor.

::::challenge{id=adding-eigen-dep title="Adding the Eigen dependency"} 

Look at Checkpoint 3. A new file `src/functionality_eigen.cpp` depends on the
[Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) library for linear
algebra.

Task: Using `find_package`, modify the `CMakeLists.txt` in directory `src/` to
link target `cmake_course_lib` against Eigen.

*Hint: Useful instructions can be found at [Using Eigen in CMake Projects](http://eigen.tuxfamily.org/dox/TopicCMakeGuide.html).*

Note that keyword `NO_MODULE` is equivalent to `CONFIG`.

:::solution

Your `src/CMakeLists.txt` should look like this:

```cmake
find_package(Eigen3 CONFIG REQUIRED)
message(STATUS "Found Eigen3 ${Eigen3_VERSION}")

set(
        cmake_course_source_files
        functionality.cpp
        functionality_eigen.cpp
)

set(
        cmake_course_header_files
        functionality.hpp
        functionality_eigen.hpp
)

add_library(cmake_course_lib STATIC ${cmake_course_source_files} ${cmake_course_header_files})
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

Such *module files* are provided by CMake itself for common libraries,
they can also be written for a particular use case if required.

### Package components

Often libraries are split into different components. E.g. Boost: filesystem,
thread, date-time, program-options, numpy...

Most programs only rely on a subset of components, and so it is useful to be
able to find and link against only the components that are needed using `find_package`.

```cmake
set(boost_components filesystem chrono)
find_package(Boost MODULE REQUIRED COMPONENTS ${boost_components})
```

The CMake target for a component is `<PackageName>::<ComponentName>`
(*e.g.* `Boost::filesystem`).


::::challenge{id=adding-boost-dep title="Adding the Boost dependency"} 

Look at Checkpoint 4. The executable `exe/main.cpp` depends on the [Boost Program Options](https://www.boost.org/doc/libs/1_74_0/doc/html/program_options.html)
library for handling command line arguments.

Task: Using `find_package` in `MODULE` mode, modify the `CMakeLists.txt` in directory `exe/` to
find and link target `main_executable` against `Boost::program_options`.

:::solution
Your `exe/CMakeLists.txt` should look like this:

```cmake
find_package(Boost MODULE REQUIRED COMPONENTS program_options)
message(STATUS "Found Boost ${Boost_VERSION}")

add_executable(main_executable main.cpp)
target_link_libraries(main_executable PRIVATE cmake_course_lib)
target_link_libraries(main_executable PRIVATE project_warnings)
target_link_libraries(main_executable PRIVATE Boost::program_options)
```
:::
::::


## Adding CMake functionality using `include`

Any file containing valid CMake syntax can be "included" in the
current `CMakeLists.txt`. This is similar to the `#include`{.cpp} directive in C++, and is
useful for splitting up large `CMakeLists.txt` files into smaller, more
manageable chunks, for using additional CMake functionality, or for reusing 
functionality across projects.

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
project(IntroCMakeCourse LANGUAGES CXX)
include(file_to_include.cmake)

set(name "Foo Bar")
message(STATUS "Hello ${name}")

# cmake/file_to_include.cmake
set(name "Jane Doe")
message(STATUS "Hello ${name}")
```

```
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
    math(EXPR result "{a}+{b}")
    message("The sum is ${result}")
endfunction()
```

Functions cannot return a value.

Functions introduce a new scope.

A similar notion is CMake *macros*, which does **not** introduce a new scope.


## Setting options with `option()`

Boolean variables can be declared using `option()`:

```cmake
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" TRUE)
```

This is useful for allowing users to configure the build process. The value of
options can be specified at the command line using the `-D` syntax:

```cmake
cmake -DWARNINGS_AS_ERRORS=FALSE ..
```

Options are a special case of "cache" variable, whose value persist
between CMake runs. You can see the value of all cache variables by looking at
the file `CMakeCache.txt` in the build directory.

## Built-in CMake variables

CMake provides *a lot* of pre-defined variables which values describe the system.

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
cmake -DWARNINGS_AS_ERRORS=ON ..
```
```bash
cmake -DWARNINGS_AS_ERRORS=OFF ..
```

::::


## That's all, folks

This was only the tiniest tip of the modern CMake iceberg. There are so many great resources available, and here are just a few of them:

- [The CMake documentation (link)](https://cmake.org/cmake/help/latest/)
- [Professional CMake: A Practical Guide (link)](https://crascit.com/professional-cmake/)
