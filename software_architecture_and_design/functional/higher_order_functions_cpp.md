---
name: First Class and Higher Order Functions
dependsOn: [
    software_architecture_and_design.functional.state_and_side_effects_cpp,
]
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
process, you can naturally just write the function object yourself.

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
"map", "filter" and "reduce" (which also have recently inspired the MapReduce
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
               is_prime);
}
```

Finally, the reduce, or `std::reduce`

~~~ python
l = [1, 2, 3]

def add_one(x):
    return x + 1

# Returns a <map object> so need to cast to list
print(list(map(add_one, l)))
print(list(map(lambda x: x + 1, l)))
~~~
{: .language-python}

~~~
[2, 3, 4]
[2, 3, 4]
~~~
{: .output}

Like `map`, `filter` takes a function and applies it to each value in an iterable, keeping the value if the result of the function application is `True`.

~~~ python
l = [1, 2, 3]

def is_gt_one(x):
    return x > 1

# Returns a <filter object> so need to cast to list
print(list(filter(is_gt_one, l)))
print(list(filter(lambda x: x > 1, l)))
~~~
{: .language-python}

~~~
[2, 3]
[2, 3]
~~~
{: .output}

The `reduce` function is different.
This function uses a function which accepts two values to accumulate the values in the iterable.
The simplest uses here are to calculate the sum or product of a sequence.

~~~ python
from functools import reduce

l = [1, 2, 3]

def add(a, b):
    return a + b

print(reduce(add, l))
print(reduce(lambda a, b: a + b, l))
~~~
{: .language-python}

~~~
6
6
~~~
{: .output}

These are the fundamental components of the MapReduce style, and can be combined to perform much more complex data processing operations.

> ## Sum of Squares
>
> Using the MapReduce model, write a function that calculates the sum of the squares of the values in a list.
> Your function should behave as below:
>
> ~~~ python
> def sum_of_squares(l):
>     # Your code here
>
> print(sum_of_squares([0]))
> print(sum_of_squares([1]))
> print(sum_of_squares([1, 2, 3]))
> print(sum_of_squares([-1]))
> print(sum_of_squares([-1, -2, -3]))
> ~~~
> {: .language-python}
>
> ~~~
> 0
> 1
> 14
> 1
> 14
> ~~~
> {: .output}
>
> > ## Solution
> >
> > ~~~ python
> > from functools import reduce
> >
> > def sum_of_squares(l):
> >     squares = map(lambda x: x * x, l)
> >     return reduce(lambda a, b: a + b, squares)
> > ~~~
> > {: .language-python}
> >
>{: .solution}
>
> Now let's assume we're reading in these numbers from an input file, so they arrive as a list of strings.
> Modify your function so that it passes the following tests:
>
> ~~~ python
> print(sum_of_squares(['1', '2', '3']))
> print(sum_of_squares(['-1', '-2', '-3']))
> ~~~
> {: .language-python}
>
> ~~~
> 14
> 14
> ~~~
> {: .output}
>
> > ## Solution
> >
> > ~~~ python
> > from functools import reduce
> >
> > def sum_of_squares(l):
> >     integers = map(int, l)
> >     squares = map(lambda x: x * x, integers)
> >     return reduce(lambda a, b: a + b, squares)
> > ~~~
> > {: .language-python}
> >
>{: .solution}
>
> Finally, like comments in Python, we'd like it to be possible for users to comment out numbers in the input file they give to our program.
> Extend your function so that the following tests pass (don't worry about passing the first set of tests with lists of integers):
>
> ~~~ python
> print(sum_of_squares(['1', '2', '3']))
> print(sum_of_squares(['-1', '-2', '-3']))
> print(sum_of_squares(['1', '2', '#100', '3']))
> ~~~
> {: .language-python}
>
> ~~~
> 14
> 14
> 14
> ~~~
> {: .output}
>
> > ## Solution
> >
> > ~~~ python
> > from functools import reduce
> >
> > def sum_of_squares(l):
> >     not_comments = filter(lambda x: x[0] != '#', l)
> >     integers = map(int, not_comments)
> >     squares = map(lambda x: x * x, integers)
> >     return reduce(lambda a, b: a + b, squares)
> > ~~~
> > {: .language-python}
>{: .solution}
{: .challenge}

> ## Multiprocessing (Optional Advanced Challenge)
>
> **Advanced challenge for if you're finished early.**
>
> One of the benefits of functional programming is that, if we have pure functions, when applying / mapping a function to many values in a collection, each application is completely independent of the others.
> This means that we can take advantage of multiprocessing, without many of the normal problems in synchronisation that this brings.
>
> Read through the Python documentation for the [multiprocessing module](https://docs.python.org/3/library/multiprocessing.html), particularly the `Pool.map` method.
>
> Update one of our examples to make use of multiprocessing.
> How much of a performance improvement do you get?
> Is this as much as you would expect for the number of cores your CPU has?
>
> **Hint:** To time the execution of a Python script we can use the Linux program `time`:
>
> ~~~
> time python3 my_script.py
> ~~~
> {: .language-bash}
>
> Would we get the same benefits from parallel equivalents of the `filter` and `reduce` functions?
> Why, or why not?
>
> {: .language-bash}
{: .challenge}


## Part 2 - Moving beyond the `for` loop: STL algorithms

Having just told you about all the great new ways you can write a `for`{.cpp} loop, we're going to spend the rest of the workshop trying to convince you to use them as little as possible!

~~~cpp
std::vector v = {1, 2, 3, 4, 5};

// Option 1
int sum1 = 0;
for (const auto x : v) {
   sum1 += x;
}

// Option 2 (<numeric> header)
const int sum2 = std::accumulate(v.begin(), v.end(), 0);
~~~


## Using the [algorithms (link)](https://en.cppreference.com/w/cpp/algorithm) library

Things about option 2:

- `sum2` is `const`{.cpp}
- the operation has an explicit name (accumulate)
- you are conveying your intent to the compiler
- it's more concise
- it requires another header...


## Using the [algorithms (link)](https://en.cppreference.com/w/cpp/algorithm) library

There are algorithms for:

- Adding things up (`std::accumulate`{.cpp}, `std::reduce`{.cpp})
- Doing "something" to a range (`std::transform`{.cpp})
- Doing "something" to a range and then adding up (`std::inner_product`{.cpp}, `std::transform_reduce`{.cpp})
- Sorting (`std::sort`{.cpp})
- Rotating (`std::rotate`{.cpp})
- Permuting (`std::next_permutation`{.cpp})
- Many, many other things


## Task 2

We're currently using a `for`{.cpp} loop to calculate the mean and the variance. Yuck!

Replace those for loops with:

- Algorithms!
- First, try `std::accumulate`{.cpp} and `std::inner_product`{.cpp}
- Then, try `std::reduce`{.cpp} and `std::transform_reduce`{.cpp}


## Part 3 - customising algorithms

Many algorithms allow customisation.

~~~ cpp
template< class RandomIt, class Compare >
void sort( RandomIt first, RandomIt last, Compare comp );
~~~

Here, `std::sort`{.cpp} is templated over `class Compare`{.cpp} (as well as the iterator type). How can we make use of this customisation point?

There are several ways, but usually the most convenient in modern C++ is the **lambda function**.

## Using lambdas with algorithms

Let's sort a range largest to smallest rather than smallest to largest...

~~~cpp

std::vector v = {4.53, 2.38, 3.45, 9.68};

auto sort_greater = [](double x, double y) { return x > y; };
std::sort(v.begin(), v.end(), sort_greater);

std::cout << v.at(0) << '\n';  // prints 9.68

~~~