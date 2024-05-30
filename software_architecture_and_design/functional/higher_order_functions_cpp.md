---
name: Higher Order Functions
dependsOn: [
    software_architecture_and_design.functional.side_effects_cpp,
]
tags: [cpp]
attribution: 
    - citation: >
        This material was adapted from an "Introduction to C++" course developed by the
        Oxford RSE group.
      url: https://www.rse.ox.ac.uk
      image: https://www.rse.ox.ac.uk/images/banner_ox_rse.svg
      license: CC-BY-4.0
    - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1 
      url: https://www.universe-hpc.ac.uk
      image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
      license: CC-BY-4.0

---


## First Class Functions

Languages that treat functions as first-class citizens allow functions to be
passed as arguments to other functions, returned from functions, or assigned to
variables. In C++ this is typically done via lamda functions or function objects.

### Lambda Functions

*Lambda functions* are small, nameless functions which are defined in the
normal flow of the program, typically as they are needed. They consist of three part,
delimited by square, round, then curly brackets. The curly brackets form the
body of the function, for example 

~~~cpp
auto hello_world = []() {
  std::cout << "hello world" << std::endl;
};

hello_world();
~~~

The `auto`{.Cpp} keyword allows the compiler to determine the correct type for
the lambda, rather than you declaring it manually (impossible for lambda
functions). You can call or execute a lambda as you would any other function.

The round brackets contain the list of arguments to the function, and the square
brackets **capture** variables from the outside scope, for example

~~~cpp
int i = 1;
auto add_i_to_arg = [i](int arg) { return arg + i; };
std::cout << add_i_to_arg(3) << std::endl; // prints 4
~~~

This captures `i` by value. To capture by reference use `&`:

~~~cpp
int i = 1;
auto add_arg_to_i = [&i](int arg) { i += arg; };
add_arg_to_i(3);
std::cout << i << std::endl; // prints 4
~~~

You can capture all variables used in the lambda function using either `[=]`,
which captures everything by value, or `[&]`, which captures everything by
reference:

~~~cpp
int i = 1;
auto add_i_to_arg = [=](int arg) { return arg + i; };
std::cout << add_i_to_arg(3) << std::endl; // prints 4

auto add_arg_to_i = [&](int arg) { i += arg; };
add_arg_to_i(3);
std::cout << i << std::endl; // prints 4
~~~

### Function objects

A lambda function in C++ is syntactical suger for a function object, which is
simply a class with a round bracket operator defined. For example you could
define the last `add_i_to_arg` lamda manually as a function object

```cpp
class AddIToArg {
    int& m_i;
public:
    AddIToArg(int& i):m_i(i) {
    }
    void operator()(int arg) {
        m_i += arg;
    }
}

int main() {
    int i = 1;
    AddOToArg add_arg_to_i(i);
    add_arg_to_i(3);
    std::cout << i << std::endl; // prints 4
}
```

Under the hood, when you write you lambda the compiler simply creates and
compiles the equivilant function object for you. If you want more control of the
process, you can write the function object manually.

### Polymorphic function

Recall that polymorphism allows us to provide a single interface for a variety
of types. One issue with lambdas is that each lambda is a unique type, so the
following will raise an error.

```cpp
auto add = [](int i) { return i + 1; };
add = [](int i) { return i + 2; };
```

The two lambdas have different types, even though they are both functions that take a single `int` as an argument and return another `int`.

```
/home/mrobins/git/cpp_tmp/prodecural.cpp:14:35: error: no match for 'operator=' (operand types are 'main()::<lambda(int)>' and 'main()::<lambda(int)>')
   14 |   add = [](int i) { return i + 2; };
      |                                   ^
/home/mrobins/git/cpp_tmp/prodecural.cpp:13:15: note: candidate: 'main()::<lambda(int)>& main()::<lambda(int)>::operator=(const main()::<lambda(int)>&)' (deleted)
   13 |   auto add = [](int i) { return i + 1; };
      |               ^
/home/mrobins/git/cpp_tmp/prodecural.cpp:13:15: note:   no known conversion for argument 1 from 'main()::<lambda(int)>' to 'const main()::<lambda(int)>&'
```

This causes problems if for example, you want to store a collection of function
objects with the same interface. To support this and other use-cases where a
polymorphic function is needed, C++ provides `std::function`, which can be used
like so

```cpp
std::vector<std::function<int(int)>> ops = {
    [] (int i) {return 2 * i;},
    [] (int i) {return std::pow(i, 2);},
    [] (int i) {return 2 * (i - 1);}
};

int result = 1;
for (const auto& op: ops) {
    result = op(result);
}
std::cout << result << std::end; // prints 6
```

`std::function` is an example of *type erasure*.


## Higher Order Functions

One of the main uses of lambda functions is to create temporary functions to
pass into higher order functions. A higher order function is simply a function
that has other functions as one of its arguments. 

To illustrate the benifits of higher order functions, let us define two
functions, one that calculates the sum of a `std::vector<int>`, the other
which calculates the maximum value the same vector type.

```cpp
int sum(const std::vector<int>& data) {
    int result = 0;
    for (const auto& x: data) {
        result = result + x;
    }
    return result;
}
int maximum(const std::vector<int>& data) {
    int result = 0;
    for (const auto& x: data) {
        result = std::max(result, x);
    }
    return result;
}
```

We notice that these are really exactly the same algorithm, except that we
change the binary operation done on the rhs of the statement in the loop, we
therefore decide to combine these functions into one higher order function.

```cpp
int reduce(const std::vector<int>& data, std::function<int(int, int)> bin_op) {
    int result = 0;
    for (const auto& x: data) {
        result = bin_op(result, x);
    }
    return result;
}

int main() {
  std::vector<int> data = {1, 2, 3, 4, -1};
  std::cout << reduce(data, std::plus<int>()) << std::endl; 
  std::cout << reduce(data, std::multiplies<int>()) << std::endl; 
  std::cout << reduce(data, [](int a, int b) { return std::max(a, b); }) << std::endl; 
  std::cout << reduce(data, [](int a, int b) { return std::min(a, b); }) << std::endl; 
}
```

Excellent! We have reduced the amount of code we need to write, reducing the
number of possible bugs and making the code easier to maintain in the future.

C++ actually has a `std::reduce`, which is part of the *algorithms* standard library.

### The Algorithms Library

The [algorithms library](https://en.cppreference.com/w/cpp/algorithm) is a
collection of higher order functions implementing many common algorithms. These
are typically algorithms that you write over and over again, often without
recognising their conceptual similarities. Using the algorithms library means:

(a) you reduce the amount of (algorithmic) code you need to write, reducing bugs and increasing maintainability
(b) you make clear to the reader what your code is doing, since these are commonly used algorithms
(b) you benifit from bullet proof, efficient implementations written by the same teams that write the compiler you are using
(c) you can benifit from *executors* to instantly parallise or vectorise your code for high performance.

Lets go through a few examples inspired by the common functional algorithms
"map", "filter" and "reduce" (also the inspiration for the MapReduce
programming model implemented in Spark and Hadoop).

First the map, or `std::transform`:

```cpp
std::vector<double> data = {1.0, 2.0, -1.1, 5.0};

// transform in-place
std::transform(std::begin(data), std::end(data), std::begin(data), 
               [](const double& x) { return 2.0 * x; } );

std::vector<double> new_data(data.size());

// transform to a new collection
std::transform(std::begin(data), std::end(data), std::begin(new_data), 
               [](const double& x) { return 3.14 * std::pow(x, 2); } );

```

Then the filter, or `std::copy_if`, which we will use to print out all the prime numbers to 1000. 

Here we also introduce two more useful tools:

1. `std::iota` which fills a vector with increasing values.
2. `std::ostream_iterator`. This iterator-like object allows you to output a sequence of values to the screen or to a file

```cpp
bool is_prime(int n) {
  bool is_prime = true;
  if (n == 0 || n == 1) {
    is_prime = false;
  }
  for (int i = 2; i <= n/2; ++i) {
    if (n % i == 0) {
      is_prime = false;
      break;
    }
  }
  return is_prime;
}

int main() {
  std::vector<int> data(1000); 
  std::iota(data.begin(), data.end(), 1); // fill with numbers 1 -> 1000
  std::copy_if(data.begin(), data.end(),
               std::ostream_iterator<int>(std::cout, " "),
               is_prime); // print primes
}
```

Notice here we are breaking out the inner algorithm of determining if an `int` is prime
or not, from the outer algorithm of looping through a collection of numbers and
filtering them according to a function (as opposed to writing them together in a
standard loop). This division makes each algorithm clearer, and we also have a nice
self-contained `is_prime` function we can potentially reuse.

Finally, the reduce, or `std::reduce`, which we will use to calculate the min and
maximum elements of an vector. At the same time we introduce another algorithm
`std::generate`, which assigns values to a range based on a generator function, and some
of the random number generation options in the standard library. 

~~~ cpp
#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <tuple>


int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> dist(5, 2);
  auto gen_random = [&]() { return dist(gen);};

  std::vector<double> data(1000); 
  std::generate(data.begin(), data.end(), gen_random);

  auto calc_min_max = [](std::tuple<double, double> acc, double x) {
    auto [min, max] = acc;
    min = std::min(min, x);
    max = std::max(max, x);
    return std::make_tuple(min, max);
  };
  auto [min, max] = std::accumulate(data.begin(), data.end(), std::make_tuple(0., 0.), calc_min_max);
  std::cout << "min is "<< min << " max is "<< max << std::endl;
}
~~~


::::challenge{id=sum_squares title="Sum of Squares"}

Use `std::accumulate` to write a function that calculates the sum of the squares of the values in a vector.
Your function should behave as below:

~~~ cpp 
std::cout << sum_of_squares({0}) << std::endl;
std::cout << sum_of_squares({1, 3, -2}) << std::endl;
~~~

~~~
0
14
~~~


:::solution

~~~cpp
#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>

int sum_of_squares(const std::vector<int>& data) {
  auto sum_squares = [](int sum, int x) { return sum + std::pow(x, 2); };
  return std::accumulate(data.begin(), data.end(), 0, sum_squares);
}
~~~
:::


Now let's assume we're reading in these numbers from an input file, so they arrive as a list of strings.
Write a new function `map_str_to_int` using `std::transform` that passes the following tests:

~~~ cpp
std::cout << sum_of_squares(map_str_to_int({"1", "2", "3"})) << std::endl;
std::cout << sum_of_squares(map_str_to_int({"-1", "-2", "-3"})) << std::endl;
~~~

~~~
14
14
~~~

:::solution
~~~cpp
const std::vector<int> map_str_to_int(const std::vector<std::string>& data) {
  std::vector<int> new_data(data.size());
  auto str_to_int = [](std::string x) { return std::atoi(x.c_str()); };
  std::transform(data.begin(), data.end(), new_data.begin(), str_to_int);
  return new_data;
}
~~~
:::

Finally, we'd like it to be possible for users to comment out numbers in the input file they give to our program.
Extend your `map_str_to_int` function so that the following tests pass:

~~~ cpp
std::cout << sum_of_squares(map_str_to_int({"1", "2", "3"})) << std::endl;
std::cout << sum_of_squares(map_str_to_int({"1", "2", "#100", "3"})) << std::endl;
~~~

~~~
14
14
14
~~~

:::solution

~~~cpp
std::vector<int> map_str_to_int(const std::vector<std::string>& data) {
  std::vector<int> new_data;
  std::vector<std::string> filtered_data;
  filtered_data.reserve(data.size());
  std::copy_if(data.begin(), data.end(),
               std::back_inserter(filtered_data), [](std::string x) { return x[0] != '#'; });
  new_data.resize(filtered_data.size());
  std::transform(filtered_data.begin(), filtered_data.end(), new_data.begin(),
                 [](std::string x) { return std::atoi(x.c_str()); });
  return new_data;
}
~~~

or

~~~cpp
std::vector<int> map_str_to_int(const std::vector<std::string>& data) {
  std::vector<int> new_data;
  new_data.reserve(data.size());
  for (const std::string& x: data) {
    if (x[0] == '#') {
      continue;
    }
    new_data.push_back(std::atoi(x.c_str()));
  }
  return new_data; 
}
~~~

Here you can start to see a limitation of the algorithms in the standard
library, in that it is difficult to efficiently compose together multiple
elemental algorithms into more complex algorithm. The *ranges* library is an
C++20 addition to the standard library aims to solve this problem, you can read
more about the ranges library [here](https://en.cppreference.com/w/cpp/ranges).

:::
::::

## Key Points:
- Higher-order functions in C++: Functions that accept other functions as arguments or return them as results, enabling more modular, reusable, and expressive code.
- Lambda expressions: Anonymous functions defined using lambda syntax, often utilized as arguments for higher-order functions, offering flexibility and conciseness.
- Polymorphic functions: `std::function` allows functions to be passed around as objects with a common interface, enabling polymorphism and more flexible higher-order function usage.
- Standard library algorithms: C++ includes a variety of higher-order functions (e.g. `std::transform`, `std::for_each`, and `std::accumulate`) to perform common operations on data structures efficiently and with less boilerplate code.