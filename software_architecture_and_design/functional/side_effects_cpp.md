---
name: State and Side Effects
dependsOn: []
tags: [cpp]
attribution:
  - citation: This material was adapted from an "Introduction to C++" course developed by the Oxford RSE group.
    url: https://www.rse.ox.ac.uk
    image: https://www.rse.ox.ac.uk/images/banner_ox_rse.svg
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Program state

The current state of a given program is composed of many different parts, some
of which are clear, others which are more opaque. The
most obvious state is the values of all the current variables in the scope. Take
this code snippet:

```cpp
int y = 3;
int x = 2;
x = y;
```

Here we have two variables `x` and `y` with initial values, when the last line
is executed the state is updated so that `x` now has the value 3. We can clearly see this from the code given, but it can be less obvious when this code is hidden in a function:

```cpp
void my_cool_function(int& x, const int y) {
    x = y;
}

int main () {
  int y = 3;
  int x = 2;
  my_cool_function(x, y);
}
```

Now imagine we have a global variable defined elsewhere that is updated by
`my_cool_function`, this is not even passed into the function so it is even
more unclear that this is being updated. The global variable and function might
even be declared in a separate file and brought in via a `#include`

```cpp
int z = 3;
void my_cool_function(int& x, const int y) {
    x = y;
    z++;
}

int main () {
  int y = 3;
  int x = 2;
  my_cool_function(x, y);
}
```

Finally, any interaction with the Operating System is yet another, even more
opaque, part of the current state. This includes memory allocations, and file
IO.

```cpp
std::vector<BigType> data(1000); // do we have enough RAM available for this?

ofstream myfile;
myfile.open ("example.txt", ios::out); // does this file exist? Do I have write permissions?
std::string line;
std::getline(myfile, line); // did I open this for reading?
std::getline(myfile, line); // Same call to getline, but result is different!
```

The main downside of having a state that is constantly updated is that it makes
it harder for us to _reason_ about our code, to work out what it is doing.
However, the main upside is that we can use state to make calculations more
efficient, for example to sum up the values in a vector we use a single variable
`sum` to hold the state of the computation

```cpp
std::vector<int> data = {1, 2, 3, 4};
int sum = 0;
for (const auto& x: data) {
    sum += x;
}
```

## Side Effect and Pure Functions

Functional computations only rely on the values that are provided as inputs to a
function and not on the state of the program that precedes the function call.
They do not modify data that exists outside the current function, including the
input data - this property is referred to as the _immutability of data_. This
means that such functions do not create any _side effects_, i.e. do not perform
any action that affects anything other than the value they return. A pure function is therefore the
computational version of a mathematical function.

For example: printing text, writing to a file, modifying the value of an input argument, or
changing the value of a global variable. Functions without side affects that
return the same data each time the same input arguments are provided are called
_pure functions_.

::::challenge{id="pure-functions" title="Pure Functions"}

Which of the following are pure functions? Can you explain why or why not?

```cpp
int increment_and_return_x(const int& x) {
  return x + 1;
}

int increment_and_return_x(const int& x) {
  std::cout << "Incrementing x = " << x << std::endl;
  return x + 1;
}

void increment_x(int& x) {
  x += 1;
}

int one = 1;
void increment_x(int& x) {
  x += one;
}
```

:::solution

```cpp
// a pure function, no side effects :)
int increment_and_return_x(const int& x) {
  return x + 1;
}

// technically a side effect with the print, but this is probably safe
int increment_and_return_x(const int& x) {
  std::cout << "Incrementing x = " << x << std::endl;
  return x + 1;
}

// a side effect since we modify x, which is outside the scope of this function.
// We make this clear from the function signature and name, however, still not
// recommended for something as simple as an `int` since the pure function above
// is just as easy to implement and just as efficient. This might change for a
// more complex type that is expensive to create
void increment_x(int& x) {
  x += 1;
}

int one = 1;
// dangerous side effect! effect of function changes with global `one` variable
void increment_x(int& x) {
  x += one;
}
```

:::
::::

::::challenge{id="conways-game-of-life" title="Conway's Game of Life"}

Conway's Game of Life is a popular cellular automaton that simulates the
evolution of a two-dimensional grid of cells. In this exercise, you will
refactor a Python program that implements Conway's Game of Life. The basic rules of the game of life are:

1. Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.

The code has a bug related to the improper management of the program
state, which you will fix. Refactor the code so that the `step`
function is a pure function.

```cpp
#include <iostream>
#include <vector>

void step(std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::vector<int> neighbors = get_neighbors(grid, i, j);
            int count = 0;
            for (int neighbor : neighbors) {
                count += neighbor;
            }
            if (grid[i][j] == 1) {
                if (count == 2 || count == 3) {
                    grid[i][j] = 1;
                }
            } else if (count == 3) {
                grid[i][j] = 1;
            }
        }
    }
}

std::vector<int> get_neighbors(const std::vector<std::vector<int>>& grid, int i, int j) {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::pair<int, int>> indices = {{i - 1, j - 1}, {i - 1, j}, {i - 1, j + 1},
                                                {i, j - 1},                 {i, j + 1},
                                                {i + 1, j - 1}, {i + 1, j}, {i + 1, j + 1}};
    std::vector<int> neighbors;

    for (const auto& idx : indices) {
        int row = idx.first;
        int col = idx.second;
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            neighbors.push_back(grid[row][col]);
        }
    }

    return neighbors;
}

int main() {
    std::vector<std::vector<int>> grid = {{0, 0, 0, 0, 0},
                                          {0, 0, 1, 0, 0},
                                          {0, 1, 0, 1, 0},
                                          {0, 0, 1, 0, 0},
                                          {0, 0, 0, 0, 0}};
    step(grid);

    // Print the grid
    for (const auto& row : grid) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

:::solution

We can fix the bug by creating a new grid to store the results of the step, and
we can make the function pure by returning the new grid instead of modifying the
input grid.

Note that we have sacrificed some efficiency by creating a new grid each time.
If we wanted to improve the efficiency, we could pass the new grid as an
argument to the function, but this would make the function impure again. Program
design often involves trade-offs like this, if efficiency is important we can
sacrifice purity, but if we want to be able to reason about our code and test it
easily, we should strive for purity.

```cpp
#include <iostream>
#include <vector>

std::vector<std::vector<int>> step(const std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<int>> new_grid(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::vector<int> neighbors = get_neighbors(grid, i, j);
            int count = 0;
            for (int neighbor : neighbors) {
                count += neighbor;
            }
            if (grid[i][j] == 1 && (count == 2 || count == 3)) {
                new_grid[i][j] = 1;
            } else if (grid[i][j] == 0 && count == 3) {
                new_grid[i][j] = 1;
            }
        }
    }

    return new_grid;
}

std::vector<int> get_neighbors(const std::vector<std::vector<int>>& grid, int i, int j) {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::pair<int, int>> indices = {{i - 1, j - 1}, {i - 1, j}, {i - 1, j + 1},
                                                {i, j - 1},                {i, j + 1},
                                                {i + 1, j - 1}, {i + 1, j}, {i + 1, j + 1}};
    std::vector<int> neighbors;

    for (const auto& idx : indices) {
        int row = idx.first;
        int col = idx.second;
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            neighbors.push_back(grid[row][col]);
        }
    }

    return neighbors;
}

int main() {
    std::vector<std::vector<int>> grid = {{0, 0, 0, 0, 0},
                                          {0, 0, 1, 0, 0},
                                          {0, 1, 0, 1, 0},
                                          {0, 0, 1, 0, 0},
                                          {0, 0, 0, 0, 0}};
    std::vector<std::vector<int>> new_grid = step(grid);

    // Check if the grid is unchanged
    bool unchanged = true;
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[0].size(); ++j) {
            if (grid[i][j] != new_grid[i][j]) {
                unchanged = false;
                break;
            }
        }
        if (!unchanged) {
            break;
        }
    }
    if (unchanged) {
        std::cout << "Grid is unchanged" << std::endl;
    } else {
        std::cout << "Grid has changed" << std::endl;
    }

    return 0;
}
```

:::
::::

## Benefits of Functional Code

There are a few benefits we get when working with pure functions:

- Testability
- Composability
- Parallelisability

**Testability** indicates how easy it is to test the function - usually meaning
unit tests. It is much easier to test a function if we can be certain that a
particular input will always produce the same output. If a function we are
testing might have different results each time it runs (e.g. a function that
generates random numbers drawn from a normal distribution), we need to come up
with a new way to test it. Similarly, it can be more difficult to test a
function with side effects as it is not always obvious what the side effects
will be, or how to measure them.

**Composability** refers to the ability to make a new function from a chain of
other functions by piping the output of one as the input to the next. If a
function does not have side effects or non-deterministic behaviour, then all
of its behaviour is reflected in the value it returns. As a consequence of
this, any chain of combined pure functions is itself pure, so we keep all these
benefits when we are combining functions into a larger program.

**Parallelisability** is the ability for operations to be performed at the same
*time (independently). If we know that a function is fully pure and we have got
*a lot of data, we can often improve performance by splitting data and
*distributing the computation across multiple processors. The output of a pure
*function depends only on its input, so we will get the right result regardless
\*of when or where the code runs.

There are other advantageous properties that can be derived from the functional
approach to coding. In languages which support functional programming, a
function is a _first-class object_ like any other object - not only can you
compose/chain functions together, but functions can be used as inputs to, passed
around or returned as results from other functions (remember, in functional
programming _code is data_). This is why functional programming is suitable for
processing data efficiently - in particular in the world of Big Data, where code
is much smaller than the data, sending the code to where data is located is
cheaper and faster than the other way round. Let's see how we can do data
processing using functional programming.

## Key Points

- Program state is composed of variables' values, including those modified by functions and interactions with the Operating System.
- Functional computations rely only on input values, are immutable, and do not create side effects. Pure functions are testable, composable, and parallelizable.
