---
name: Subdirectories and the CMake language
dependsOn: [technology_and_tooling.cmake.03_configuring_the_build]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Checkpoint 1

Now look at the next checkpoint in the `checkpoint_1` subdirectory, which looks like this:

```text
CMakeLists.txt
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

This causes CMake to process the `CMakeLists.txt` file in the directory `src`.

```cmake
# src/CMakeLists.txt
add_executable(main_executable
        functionality.cpp
        functionality.hpp
        main.cpp
)
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

:::callout
Storing a list of sources in a variable like this works, but in modern CMake it
is usually clearer to list the sources directly in `add_executable` or
`add_library`.

Also avoid using `file(GLOB ...)` to collect sources automatically:
CMake cannot tell when you add or remove a file, so the build can silently go
stale.
:::

::::challenge{id=add-function title="Adding new files to the project"}

Look through the files in Checkpoint 1.

Add a new pair of hpp/cpp files that defines a new function.

- Call it from the main executable
- Add the files to `src/CMakeLists.txt`
- Configure, compile and run: check that your new function has been executed

:::solution

`src/CMakeLists.txt` should look like this:

```cmake
add_executable(main_executable
        functionality.cpp
        functionality.hpp
        new_function.cpp
        new_function.hpp
        main.cpp
)
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
    std::cout << "Checkpoint 1" << std::endl;
    std::cout << "Hypergeometric probability: " << hypergeometricPmf(10'000, 4'270, 300, 128) << std::endl;
    new_function();
    return 0;
}
```

:::

::::

### Print information with `message()`

You often need to print information during the configuration step. This can be
done with the `message()` command:

```cmake
set(name "Jane Doe")
message(STATUS "Hello ${name}")
```

```output
-- The CXX compiler identification is GNU 13.3.0
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

```text
CMake Error at CMakeLists.txt:2 (message):
    An error occurred but configure step continues

-- Configuring incomplete, errors occurred!
```
