---
name: Functions
dependsOn: [software_architecture_and_design.procedural.containers_cpp]
tags: [cpp]
learningOutcomes:
  - Know how to define and call a function that takes parameters.
  - Understand how and when to pass a function parameter by value or by reference.
  - Return a value from a function.
  - Understand and make use of function overloading.
  - Handle errors and work with exceptions.
  - Understand and use function templating.
attribution:
  - citation: This material was adapted from an "Introduction to C++" course developed by the Oxford RSE group.
    url: https://www.rse.ox.ac.uk
    image: https://www.rse.ox.ac.uk/sites/default/files/styles/listing_tile_text_displayed_image/public/rse/images/media/oxrse_banner_2.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

# Functions

The _function_ (aka _procedure_) is a one of the defining aspects of proceedural programming. It
allows you to package up some code defining a particular operation into a
re-useable _function_ can can take zero or more _arguments_ and (optionally)
_return_ a value.

## Declaring and calling funcitons

The following _function prototypes_ declare two functions: one called
`my_func` that takes two parameters of type `double`{.Cpp} and returns a
variable of type `double`{.Cpp}, and one called `main` that takes no parameters
and returns an `int`{.Cpp}:

```Cpp
double my_func(double x, double y);
int main();
```

The function prototype tells the compiler about function's name, return type,
and parameters. You must declare the function before you can use it, like so:

```Cpp
#include <iostream>
double multiply(double x, double y); // function prototype

int main()
{
  double a = 1.0, b = 2.0, z;
  z = multiply(a, b);
  std::cout << a << " times " << b << " equals " << z << '\n';
  return 0;
}

double multiply(double x, double y) // function definition
{
  return x * y;
}
```

A function may also return no value, and be declared as `void`{.Cpp}.

```Cpp
#include <iostream>
void output(int score, int passMark);

int main() {
  int score = 29, pass_mark = 30;
  output(score, pass_mark);
  return 0;
}

void output(int score, int passMark) {
  if (score >= passMark)
    std::cout << "Pass - congratulations!\n";
  else
    std::cout << "Fail - better luck next time\n";
}
```

Any variables that are used in the function must be declared as normal.

For example:

```Cpp
double multiply_by_5(double x)
{
  double y = 5.0;
  return x * y;
}
```

Recall the rules about _scope_, the scope of `y` lasts until the end of the
function (the last curly bracket) after which `y` is removed from memory and is
no longer available.

## Pass by value

A function can only change the value of a variable inside the function, and not
in the main program. This is because, by default, variables are _passed by value_, and the function
only sees a **copy**.

Changes in this copied variable have no effect on the original variable, for
example the function `no_effect` has no effect on the `x` variable passed into it in `main`:

```Cpp
#include <iostream>

void no_effect(double x) {
  x += 1.0;
}

int main() {
  double x = 2.0;
  no_effect(x);
  std::cout << x << '\n';
}
```

[`[< compiler explorer >]`](https://gcc.godbolt.org/z/F1MBsS)

## Pass by reference

A common way of allowing a function to change the value of a variable outside the
function is to use _references_. You can do this by adding
the `&` symbol before the variable name in the declaration of the
function and the prototype.

```Cpp
#include <iostream>
void add(double x, double y, double& rz);

int main() {
  double x = 1.0, y = 2.0, z;
  add(x, y, z);
  std::cout << x <<" plus "<< y <<" equals "<< z <<'\n';
  return 0;
}

void add(double x, double y, double& rz) {
  rz = x + y;
}
```

::::challenge{id=swap_cpp title="Swap Two Numbers"}

Write a function that accepts two floating point numbers (using references), and
swaps the values of these numbers.

:::solution

```cpp
void swap_these(float &x, float &y) {
    float tmp = x;
    x = y;
    y = tmp;
}
```

:::

::::

## Function overloading

When a function is declared, the return type and parameter type
must be specified.

If a function `mult` is to be written that multiplies two numbers, we
would like it to work for floating point numbers and for integers.

This can be achieved by **function overloading**.

More than one function `mult` can be written - one that takes two
integers and returns an integer, one that takes two floating point
numbers and returns a floating point number, etc.

```Cpp
float mult(float x, float y) {
  return x * y;
}

int mult(int x, int y) {
  return x * y;
}

int main() {
  int i = mult(7, 10);
  float f = mult(21.5f, 14.5f);
}
```

::::challenge{id=dot_product title="Scalar (dot) product"}

Write a function that returns the scalar (dot) product of two
`std::array<double, 3>` vectors. Overload this function to multiply two scalar
`double` values.

:::solution

```cpp
double dot_product(const std::array<double, 3>& x, const std::array<double, 3>& y) {
    double dot = 0.0;
    for (unsigned int i = 0; i < x.size(); ++i) {
        dot += x[i]*y[i];
    }
    return dot;
}

double dot_product(const double x, const double y) {
    return x * y;
}
```

:::

::::

## Return values

Functions can have no return value

```cpp
void print_this(int x);
```

a single return value

```cpp
int get_constant();
```

multiple return values via a `std::tuple`

```cpp
std::tuple<std::string, float> get_student_and_grade();
```

or can _optionally_ return value (i.e. either a value or nothing) via `std::optional`

```cpp
std::optional<std::string> read_file_if_exists(const std::string& filename);
```

The use of `std::optional` here tells the caller that the return
`std::string` might not exist (e.g. if the file does not exist or cannot be
opened for reading) and that this possibility must be dealt with after calling
the function. For example:

```cpp
const std::string filename = "data.txt";
if (const auto contents_opt = read_file_if_exists(filename)) {
    std::cout << *contents_opt << std::end;
} else {
    std::cerr << "Cannot read file " << filename << std::end;
}
```

## Errors and Exceptions

It is normally neccessary to deal with errors that occur within a function in
such a way that the caller of that function is aware of the error and can deal
with it (if possible), or fail gracefully (perhaps clean up resources like an
open file for example). In the previous section we saw one approach to dealling
with an error, which is to return an optional value from the function. Another
approach is to use C++ _exceptions_.

Let us define a function for solving a particular problem (e.g. a root-finding
problem). This function has an input argument `x` of type `double`, but the
solver we are writing can only solve the given problem for $x > 2.0$. Furthermore,
even if $x > 2.0$ it is possible that the function fails to find a solution to
the problem.

Since we have two possible points of failure, we decide to use exceptions to
make the caller aware of any failures, and what in particular has gone wrong.

```cpp
double solve_problem(const double x) {
    if (x <= 2) {
        throw std::invalid_argument("x must be greater than two");
    }
    /// ... solve problem here
    if (!success) {
        throw std::runtime_error("solver failed");
    }
    return result;
}
```

Both `std::invalid_argument` and `std::runtime_error` are exception classes in
the [standard library](https://en.cppreference.com/w/cpp/error/exception). When
the program gets to the `throw`{.cpp} expression, excecution is halted and
control flow immediately works backwards up the current call stack until a
`catch` expression is encountered with an argument compatible with the
exceptions thrown (here either `std::invalid_argument` or `std::runtime_error`).
If none is found then the program halts with an error.

Below is an example of how you might call `solve_problem` and handle the
possible errors with a _try-catch_ expression:

```cpp
int main() {
    double solve_for_x = 1.456;
    try {
        solve_problem(solve_for_x);
    }
    catch (std::invalid_argument err) {
        // oh no, double it and try again
        solve_problem(2 * solve_for_x);
    }
    catch (std::runtime_error err) {
        // Fall back and try 10.0, I know this one works!
        solve_problem(10.0);
    }
    catch (std::exception err) {
        // unknown error, just print it out and exit
        std::cerr << err << std::endl;
        return -1;
    }
}
```

Here we have three `catch` blocks, corresponding with different exceptions we
want to handle. The first two are the ones we saw in the definition of
`solve_problem`. The third is the base exception class in the standard library,
so any exception in the standard library (or any exception derived from one of
these) will be caught.

## Templated functions

Templates in C++ introduce compile-time polymorphism (Polymorphism is a
programming concept meaning to provide a single interface for entities of
differing types). Templates can be used to where the same code may need to
repeated for different values or for different types. For example, say we had a
function `get_min` that could accept either `double` or `int` via overloading:

```Cpp
double get_min(double a, double b)
{
   if (a < b) {return a;} return b;
}

int get_min(int a, int b)
{
   if (a < b) {return a;} return b;
}
```

This is rather cumborsome as we have to repeat the implementation of the two
overloaded functions. Instead, we can use the `template`{.Cpp} keyword to
produce as many functions as may be required:

```Cpp
template <typename Number>
Number get_min (Number a, Number b) {
    if (a < b) {
        return a;
    }
    return b;
}

int main(void) {
   int i = get_min<int>(10,-2);
   double d1 = get_min<double>(22.0/7.0, 3.14159265359);
   double d2 = get_min(22.0/7.0, 3.14159265359);
}
```

[`[< compiler explorer >]`](https://gcc.godbolt.org/z/O5DSn8)

Each use of the templated function (i.e. `get_min<int>()`, `get_min<double>()`)
causes the compiler to generate a new version of the `get_min` function with the
template argument `Number` replaced by the type given by the template argument.

### Function template type deduction

Note: it is not always necessary to provide the typename when calling a
templated function, as long as the compiler can infer it:

```Cpp
int main(void) {
   int arg1 = 10;
   int arg2 = -1;
   std::cout << get_min(arg1,arg2) << std::endl;
}
```

### Multiple template arguments

You can list multiple template arguments one after the other. These can be types
(e.g. `typename T`{.Cpp}) or non-types (e.g. `int N`{.Cpp})

```Cpp
template <int N, typename T>
T multiply_by_n (T a) {
    return N*a;
}

int main(void) {
    int i = 1;
    std::cout << multiply_by_n<2>(i) << std::endl;
}
```

::::challenge{id=dot_product_cont title="Scalar (dot) product continued"}

Rewrite your dot product function to take any two containers $a$ and $b$ that follow the standard container interface in C++. Your function should take three arguments:

1. A start iterator for vector $a$
1. An end iterator for vector $a$
1. A start iterator for vector $b$ (vector $b$ is assumed to be the same size as vector $a$)

Template your function on the iterator type for $a$ `Ta`, and the iterator type
for $b$ `Tb`. If you like, you can perform the calculation of the dot product
using a fixed type `double`. For an extra challenge, make sure you use the same
type contained in $a$ (hint: each iterator and container in the standard library
has a subtype `value_type` that is the value type held by the container).

:::solution

```cpp
template <typename Ta, typename Tb>
Ta::value_type dot_product(Ta start_a, Ta end_a, Tb start_b) {
    Ta::value_type dot = 0.0;
    Ta a = start_a;
    Tb b = start_b;
    for (; a != end_a; ++a, ++b) {
        dot += (*a) * (*b);
    }
    return dot;
}

```

:::
::::
