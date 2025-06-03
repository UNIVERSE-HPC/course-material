---
name: Higher-Order Functions
dependsOn: [software_architecture_and_design.functional.side_effects_python]
tags: [python]
attribution:
  - citation: This material has been adapted from the "Software Engineering" module of the SABS R³ Center for Doctoral Training.
    url: https://www.sabsr3.ox.ac.uk
    image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## First Class Functions

Languages that treat functions as first-class citizens allow functions to be
passed as arguments to other functions, returned from functions, or assigned to
variables. This is a powerful feature of functional programming languages, and is also available in Python.

In Python, functions are first-class citizens, which means that they can be passed to other functions as arguments, for example:

```python
def add_one(x):
    return x + 1

def apply_function(f, x):
    return f(x)

print(apply_function(add_one, 1))
```

```text
2
```

## Lambda Functions

_Lambda functions_ are small, nameless functions which are defined in the
normal flow of the program, typically as they are needed. The structure of these
functions is not dissimilar to a normal python function definition - we have a
keyword `lambda`, a list of parameters, a colon, then the function body. In
Python, the function body is limited to a single expression, which becomes the
return value.

```python
add_one = lambda x: x + 1 # NOQA E731

print(add_one(1))
```

```text
2
```

We have assigned the lambda function to a variable, so we can see it more clearly, but we'd normally use it immediately.
Most style guides (we'll come back to these later in the course) consider it bad style to assign a lambda to a variable.
This is because the main point of lambdas in Python is to avoid having to name the function - it's a bit strange to do that and then give it a name.
Because of this, if you find yourself wanting to name a lambda, just use a normal function instead.

Lambda functions may also make it slightly harder to do debugging (coming in a few days time) as the debugger and error messages doesn't have a name to show for them.

Anonymous functions, exist in many modern languages, though they may not be called 'lambda functions' and may be more or less complex to use.
For example, see [Lambda Expressions](https://en.cppreference.com/w/cpp/language/lambda) in C++ with precise control over the visibility of variables inside the function scope, and the multiple methods available in Javascript such as [Function Expressions](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions#the_function_expression_function_expression), which use syntax much more similar to named function definition.

Finally, there's another common use case of lambda functions that we'll come back to later when we see **closures**.
Due to their simplicity, it can be useful to have a lamdba function as the inner function in a closure.

## Higher-Order Functions

Higher-order functions are functions that take another function as an argument
or that return a function. One of the main uses of lambda functions is to create
temporary functions to pass into higher-order functions.

To illustrate the benefits of higher-order functions, let us define two
functions, one that calculates the sum of a list of values, the other
which calculates the maximum value of the list.

```python
def sum(data):
    result = 0
    for x in data:
        result = result + x
    return result

def maximum(data):
    result = 0
    for x in data:
        result = max(result, x)
    return result
```

We notice that these are really exactly the same algorithm, except that we
change the binary operation done on the RHS of the statement in the loop, we
therefore decide to combine these functions into one higher-order function.

```python
def reduce(data, bin_op):
    result = 0
    for x in data:
        result = bin_op(result, x)
    return result


data = [1, 2, 3, 4, -1]
print(reduce(data, lambda a, b: a + b))
print(reduce(data, lambda a, b: a * b))
print(reduce(data, lambda a, b: max(a, b)))
print(reduce(data, lambda a, b: min(a, b)))
```

```text
9
0
4
-1
```

Excellent! We have reduced the amount of code we need to write, reducing the
number of possible bugs and making the code easier to maintain in the future.

Notice, though, that `max` and `min` are already binary functions, so we can
pass them directly to `reduce` without having to wrap them in lambdas:

```python
print(reduce(data, max))
print(reduce(data, min))
```

## Map, Filter, Reduce

Python has a number of higher-order functions built in, including `map`,
`filter` and `reduce`. Note that the `map` and `filter` functions in Python use
**lazy evaluation**. This means that values in an iterable collection are not
actually calculated until you need them. We'll explain some of the implications
of this a little later, but for now, we'll just use `list()` to convert the
results to a normal list. In these examples we also see the more typical usage
of lambda functions.

The `map` function, takes a function and applies it to each value in an
**iterable**. Here, 'iterable' means any object that can be iterated over - for
more details see the [Iterable Abstract Base Class
documentation](https://docs.python.org/3/library/collections.abc.html#collections.abc.Iterable).
The results of each of those applications become the values in the **iterable**
that is returned.

```python
l = [1, 2, 3]

def add_one(x):
    return x + 1

# Returns a <map object> so need to convert to list
print(list(map(add_one, l)))
print(list(map(lambda x: x + 1, l)))
```

```text
[2, 3, 4]
[2, 3, 4]
```

Like `map`, `filter` takes a function and applies it to each value in an iterable, keeping the value if the result of the function application is `True`.

```python
l = [1, 2, 3]

def is_gt_one(x):
    return x > 1

# Returns a <filter object> so need to convert to list
print(list(filter(is_gt_one, l)))
print(list(filter(lambda x: x > 1, l)))
```

```text
[2, 3]
[2, 3]
```

The `reduce` function is different.
This function uses a function which accepts two values to accumulate the values in the iterable.
The simplest uses here are to calculate the sum or product of a sequence.

```python
from functools import reduce

l = [1, 2, 3]

def add(a, b):
    return a + b

print(reduce(add, l))
print(reduce(lambda a, b: a + b, l))
```

```text
6
6
```

These are the fundamental components of the MapReduce style, and can be combined to perform much more complex data processing operations.

::::challenge{id=sum_squares title="Sum of Squares"}

Using `map` and `reduce`, write a function that calculates the sum of the squares of the values in a list.
Your function should behave as below:

```python
def sum_of_squares(l):
    # Your code here
    return

print(sum_of_squares([0]))
print(sum_of_squares([1]))
print(sum_of_squares([1, 2, 3]))
print(sum_of_squares([-1]))
print(sum_of_squares([-1, -2, -3]))
```

```text
0
1
14
1
14
```

:::solution

```python
from functools import reduce

def sum_of_squares(l):
    squares = map(lambda x: x * x, l)
    return reduce(lambda a, b: a + b, squares)
```

:::

Now let's assume we're reading in these numbers from an input file, so they arrive as a list of strings.
Modify your function so that it passes the following tests:

```python
print(sum_of_squares(['1', '2', '3']))
print(sum_of_squares(['-1', '-2', '-3']))
```

```text
14
14
```

:::solution

```python
from functools import reduce

def sum_of_squares(l):
    integers = map(int, l)
    squares = map(lambda x: x * x, integers)
    return reduce(lambda a, b: a + b, squares)
```

:::

Finally, like comments in Python, we'd like it to be possible for users to comment out numbers in the input file they give to our program.
Extend your function so that the following tests pass (don't worry about passing the first set of tests with lists of integers):

```python
print(sum_of_squares(['1', '2', '3']))
print(sum_of_squares(['-1', '-2', '-3']))
print(sum_of_squares(['1', '2', '#100', '3']))
```

```text
14
14
14
```

:::solution

```python
from functools import reduce

def sum_of_squares(l):
    not_comments = filter(lambda x: x[0] != '#', l)
    integers = map(int, not_comments)
    squares = map(lambda x: x * x, integers)
    return reduce(lambda a, b: a + b, squares)
```

:::
::::

::::challenge{id=multiprocessing title="Multiprocessing (Optional)"}

**Advanced challenge for if you're finished early.**

One of the benefits of functional programming is that, if we have pure functions, when applying / mapping a function to many values in a collection, each application is completely independent of the others.
This means that we can take advantage of multiprocessing, without many of the normal problems in synchronisation that this brings.

Read through the Python documentation for the [multiprocessing module](https://docs.python.org/3/library/multiprocessing.html), particularly the `Pool.map` method.

Update one of our examples to make use of multiprocessing.
How much of a performance improvement do you get?
Is this as much as you would expect for the number of cores your CPU has?

**Hint:** To time the execution of a Python script we can use the Linux program `time`:

```bash
time python3 my_script.py
```

Would we get the same benefits from parallel equivalents of the `filter` and `reduce` functions?
Why, or why not?

::::

## Comprehensions

Comprehensions are a more Pythonic way to structure map and filter operations.
They serve exactly the same purpose, but are more concise and can be easier to
structure in more complex cases, such as mapping over a 2d data structure.
Using comprehensions also gives us control over which data structures we end up
with, rather than always getting back a `map` or `filter` iterable.

### List Comprehensions

The **list comprehension** is probably the most commonly used comprehension
type. As you might expect from the name, list comprehensions produce a list
from some other iterable type. In effect they are the same as using `map`
and/or `filter` and using `list()` to cast the result to a list, as we did
previously.

All comprehension types are structured in a similar way, using the syntax for a
literal of that type (in the case below, a list literal) containing what looks
like the top of a for loop. To the left of the `for` we put the equivalent of
the map operation we want to use:

```python
print([i for i in range(5)])
print([2 * i for i in range(5)])
```

```text
[0, 1, 2, 3, 4]
[0, 2, 4, 6, 8]
```

We can also use list comprehensions to perform the equivalent of a filter
operation, by putting the filter condition at the end:

```python
print([2 * i for i in range(5) if i % 2 == 0])
```

```text
[0, 4, 8]
```

### Dictionary and Set Comprehensions

Dictionary and set comprehensions are fundamentally the same as list
comprehensions but use the dictionary or set literal syntax.

So set comprehensions are:

```python
print({2 * i for i in range(5)})
```

```text
{0, 2, 4, 6, 8}
```

While dictionary comprehensions are:

```python
print({i: 2 * i for i in range(5)})
```

```text
{0: 0, 1: 2, 2: 4, 3: 6, 4: 8}
```

:::callout

## Why No Tuple Comprehension

Raymond Hettinger, one of the Python core developers, said in 2013:

> Generally, lists are for looping; tuples for structs. Lists are homogeneous; tuples heterogeneous. Lists for variable length.

Since tuples aren't intended to represent sequences, there's no need for them to have a comprehension structure.
:::

## Generators

**Generator expressions** look exactly like you might expect a tuple
comprehension (which don't exist) to look, and behaves a little differently
from the other comprehensions.

What happens if we try to use them in the same was as we did list
comprehensions?

```python
print((2 * i for i in range(5)))
```

```text
<generator object <genexpr> at 0x7efc21efcdd0>
```

Like the `map` and `filter` functions, generator expressions are not evaluated
until you iterate over them.

```python
for i in (2 * i for i in range(5)):
    print(i)
```

## Decorators

Decorators are higher-order functions that take a function as an argument, modify it, and return it.

Let's look at the following code for ways on how to "decorate" functions.

```python
def with_logging(func):

    """A decorator which adds logging to a function."""
    def inner(*args, **kwargs):
        print("Before function call")
        result = func(*args, **kwargs)
        print("After function call")
        return result

    return inner


def add_one(n):
    print("Adding one")
    return n + 1

# Redefine function add_one by wrapping it within with_logging function
add_one = with_logging(add_one)

# Another way to redefine a function - using a decorator
@with_logging
def add_two(n):
    print("Adding two")
    return n + 2

print(add_one(1))
print(add_two(1))
```

```text
Before function call
Adding one
After function call
2
Before function call
Adding two
After function call
3
```

In this example, we see a decorator (`with_logging`) and two different syntaxes
for applying the decorator to a function. The decorator is implemented here as a
function which encloses another function. Because the inner function (`inner()`)
calls the function being decorated (`func()`) and returns its result, it still
behaves like this original function. Part of this is the use of `*args` and
`**kwargs` - these allow our decorated function to accept any arguments or
keyword arguments and pass them directly to the function being decorated. Our
decorator in this case does not need to modify any of the arguments, so we do
not need to know what they are. Any additional behaviour we want to add as part
of our decorated function, we can put before or after the call to the original
function. Here we print some text both before and after the decorated function,
to show the order in which events happen.

We also see in this example the two different ways in which a decorator can be
applied. The first of these is to use a normal function call
(`with_logging(add_one)`), where we then assign the resulting function back to a
variable - often using the original name of the function, so replacing it with
the decorated version. The second syntax is the one we have seen previously
(`@with_logging`). This syntax is equivalent to the previous one - the result is
that we have a decorated version of the function, here with the name `add_two`.
Both of these syntaxes can be useful in different situations: the `@` syntax is
more concise if we never need to use the un-decorated version, while the
function-call syntax gives us more flexibility - we can continue to use the
un-decorated function if we make sure to give the decorated one a different
name, and can even make multiple decorated versions using different decorators.

::::challenge{id="decorators" title="Measuring Performance Using Decorators"}

One small task you might find a useful case for a decorator is measuring the
time taken to execute a particular function. This is an important part of
performance profiling.

Write a decorator which you can use to measure the execution time of the
decorated function using the
[time.process_time_ns()](https://docs.python.org/3/library/time.html#time.process_time_ns)
function. There are several different timing functions each with slightly
different use-cases, but we won’t worry about that here.

For the function to measure, you may wish to use this as an example:

```python
def measure_me(n):
    total = 0
    for i in range(n):
        total += i * i

    return total
```

:::solution

```python
import time

def profile(func):
    def inner(*args, **kwargs):
        start = time.process_time_ns()
        result = func(*args, **kwargs)
        stop = time.process_time_ns()

        print("Took {0} seconds".format((stop - start) / 1e9))
        return result

    return inner

@profile
def measure_me(n):
    total = 0
    for i in range(n):
        total += i * i

    return total

print(measure_me(1000000))
```

```text
Took 0.124199753 seconds
333332833333500000
```

:::
::::

## Key Points

- _First-Class Functions_: functions that can be passed as arguments to other functions, returned from functions, or assigned to variables.
- _Lambda Functions_: small, nameless functions defined in the normal flow of the program with a keyword lambda.
- _Higher-Order Functions_: a function that has other functions as one of its arguments or that returns another function.
- _Map, Filter and Reduce_: built-in higher-order functions in Python that use lazy evaluation.
- _Comprehensions_: a more Pythonic way to structure map and filter operations.
- _Generators_: similar to list comprehensions, but behave differently and not evaluated until you iterate over them.
- _Decorators_: higher-order functions that take a function as an argument, modify it, and return it.
