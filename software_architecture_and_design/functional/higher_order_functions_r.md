---
name: Higher-Order Functions
dependsOn: [software_architecture_and_design.functional.side_effects_python]
tags: [r]
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

Languages that treat functions as first-class citizens allow functions to be passed as arguments to other functions, returned from functions, or assigned to variables. This is a powerful feature of functional programming languages, and is also available in R.

In R, functions are first-class citizens, which means that they can be passed to other functions as arguments, for example:

```r
add_one <- function(x) {
  return(x + 1)
}

apply_function <- function(x, f) {
  return(f(x))
}

apply_function(1:3, add_one)
#> [1] 2 3 4
```

We have used `return()` in the example above, but it is not necessary to do so: R automatically returns the value of the last evaluated expression in the function, so it is common practice to omit `return()` unless we want the function to return early (inside an `if` statement, for example). Furthermore, since R 4.1.0, there is a new shorthand syntax for defining functions, which uses the `\` character instead of spelling out `function`. Also since R 4.1.0, there is a base-R "pipe" operator (`|>`) that allows you to pass the result of the expression that is on the left side of the pipe as the first argument of the function that is on the right side of the pipe. It is a slightly simpler equivalent of the `%>%` pipe operator from the `magrittr` package, which is commonly used with the `tidyverse` family of packages. Finally, the curly braces are optional when the function consists of a single expression, like the ones above. Taking advantage of these features, we can re-write our example as:

```r
add_one <- \(x) x + 1
apply_function <- \(x, f) f(x)
1:3 |> apply_function(add_one)
#> [1] 2 3 4
```

This is the style we will use from now on.

## Lambda Functions

_Lambda functions_ are small, nameless functions which are defined in the normal flow of the program, typically as they are needed. 

Other languages, like Python or C++, often have a special syntax for defining lambda functions, but R already uses a lambda-like syntax for all function definitions. The `\` character used in the shorthand function definition syntax was chosen for its vague similarity with the greek letter lambda: $\lambda$.

In the previous example, we have given names to the functions we were defining by assigning them to the `add_one` and `apply_function` variables, but functions are often used anonymously, especially when dealing with "higher-order" functions that accept other functions as arguments, as we will see in the next section. We could, for example, pass an anonymous function directly to `apply_function`:

```r
apply_function <- \(x, f) f(x)
1:3 |> apply_function(\(x) x + 1)
#> [1] 2 3 4
```

Anonymous functions exist in many modern languages, though they may not be called 'lambda functions' and may be more or less complex to use. For example, see [Lambda Expressions](https://en.cppreference.com/w/cpp/language/lambda) in C++ with precise control over the visibility of variables inside the function scope, and the multiple methods available in Javascript such as [Function Expressions](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions#the_function_expression_function_expression), which use syntax much more similar to named function definition.

## Higher-Order Functions

Higher-order functions are functions that take another function as an argument or that return a function. One of the main uses of lambda functions is to create temporary functions to pass into higher-order functions.

To show the benefits of higher-order functions, let us define two functions, one that calculates the sum of a list of values, the other which calculates the maximum value of the list:

```r
sum <- \(data) {
  result <- 0
  for (x in data) {
    result <- result + x
  }
  result
}

maximum <- \(data) {
  result <- 0
  for (x in data) {
    result <- max(result, x)
  }
  result
}
```

This is purely for illustrative purposes, or course, since R already has built-in `sum` and `max` functions that perform much better than what we have written here. But still, the thing to notice is that these are really exactly the same algorithm, except that we change the binary operation done on the RHS of the statement in the loop. We can therefore decide to combine these functions into one higher-order function:

```r
reduce <- \(data, bin_op) {
  result <- 0
  for (x in data) {
    result <- bin_op(result, x)
  }
  result
}

data <- c(1, 2, 3, 4, -1)
data |> reduce(\(a, b) a + b)
#> [1] 9
data |> reduce(\(a, b) a * b)
#> [1] 0
data |> reduce(\(a, b) max(a, b))
#> [1] 4
data |> reduce(\(a, b) min(a, b))
#> [1] -1
```

Excellent! We have reduced the amount of code we need to write, reducing the number of possible bugs and making the code easier to maintain in the future.

Notice, though, that `max` and `min` are already binary functions, so we can pass them directly to `reduce` without having to wrap them in lambdas:

```r
data |> reduce(max)
data |> reduce(min)
```

## Mapping, filtering and reducing

Mapping, filtering and reducing are three of the most commonly used higher-order functions. Base R has `Map`, `Filter` and `Reduce` (plus the `mapply`/`sapply`/`lapply` family for mapping) but, as is often the case in the R world, the tidyverse offers an alternative way of doing things: here, it's the `purrr` package, which we will be using in this lesson. 

The `purrr::map` function, takes a function and applies it to each value in a list or a vector:

```r
library(purrr)
data <- c(1, 2, 3, 4, -1)
data |> map(\(x) x + 1)
#> [[1]]
#> [1] 2
#> 
#> [[2]]
#> [1] 3
#> 
#> [[3]]
#> [1] 4
#> 
#> [[4]]
#> [1] 5
#> 
#> [[5]]
#> [1] 0
```

As you can see, every number in our `data` vector was incremented by one, but you might notice that `map` returned a list instead of a numeric vector. Lists in R can hold objects of any type, so the basic `map` function will work no matter what the lambda returns. Still, if your lambda always returns the same atomic type, storing the result in a vector instead of a list is faster and more memory efficient. The `purrr` package offers a series of `map` variants that can do just that: `map_lgl`, `map_int`, `map_dbl` and `map_chr` will respectively return vectors of logical, integer, double and character values.

In our "plus one" example we are dealing with integers so `map_int` is the appropriate `map` variant to use:

```r
library(purrr)
data <- c(1, 2, 3, 4, -1)
data |> map_int(\(x) x + 1)
#> [1] 2 3 4 5 0
```

There is also `map_vec`, which will attempt to automatically pick an appropriate vector type. This is especially useful if you are dealing with S3 classes like `Date` or `factor`:

```r
library(purrr)
data <- c(1, 2, 3, 4, -1)
data |> map_vec(\(x) as.Date("2000-01-01") + x)
#> [1] "2000-01-02" "2000-01-03" "2000-01-04" "2000-01-05" "1999-12-31"
```

Now one thing that is nice about R is that a lot of common operations are already "vectorised", meaning that they automatically apply to whole vectors without you having to use a mapping operation or (worse) writing a for loop. We used "add one" as a silly example, but in the real world, there is no reason you would ever write `data |> map_int(\(x) x + 1)` instead of `data + 1`. And it's not just for mathematical operations. A lot of base-R and package functions are vectorised over their inputs, e.g.:

```r
weekdays(as.Date("2000-01-01") + 1:5)
#> [1] "Sunday"    "Monday"    "Tuesday"   "Wednesday" "Thursday"
```

Still, there are plenty of cases where `map` (or one of its variants) is the tool to reach for. Take this example:

```r
library(purrr)
data <- list(c("a", "b"), c("c", "d", "e"), c("f"))
data |> map_int(length)
#> [1] 2 3 1
```

The `length` function, like most summary-type functions, is not vectorised over its input. If we tried to apply it directly to our list of vectors, it would give the length of the list itself, not the length of its constituent vectors:

```r
length(list(c("a", "b"), c("c", "d", "e"), c("f")))
#> [1] 3
```

There would be a lot more to say about mapping functions in R. The `purrr` package has pretty extensive functionalities and there is only so much ground we can cover in this lesson. Still a few more things are worth mentioning.

Higher-order functions in `purrr` will accept an R formula instead of a function definition, so instead of writing:

```r
1:5 |> map_int(\(x) x + 1)
```

you can write:

```r
1:5 |> map_int(~ . + 1)
```

where the dot (`.`) is a placeholder for a formula's single argument. This used to save a lot of typing back when had to spell out `function (x)` but is less relevant now that we have the concise `\(x)` syntax. Still, you will commonly see that syntax in legacy code.

You should also know that there are functions that allow you to map over two inputs at once (the `map2` family of variants), over more than two inputs at once (the `pmap` family of variant) and over an input and its names or indices (the `imap` family). I would particularly recommand getting acquainted with the `pmap` variants, which are especially useful when trying to map over multiple columns of a data frame.

Let us now move on to the filtering operation, which in `purrr` is called `keep` (as `filter` is already used by the `dplyr` package, where it operates on the rows of a data frame). Like `map`, `keep` is a higher-order function that can operate on a list or a vector. The function that you have to pass to it acts as a predicate: it is applied to each element of the input and must return a logical value. If it returns `TRUE`, the element is kept. If it returns `FALSE`, the element is discarded.

```r
library(purrr)
1:10 |> keep(\(x) x > 5)
#> [1]  6  7  8  9 10
c("a", "B", "c", "1", "&", "z") |> keep(\(x) x %in% letters)
#> [1] "a" "c" "z"
c(TRUE, FALSE, TRUE) |> keep(\(x) x)
#> [1] TRUE TRUE
list(1, "x", pi, TRUE, date()) |> keep(is.numeric)
#> [[1]]
#> [1] 1
#> 
#> [[2]]
#> [1] 3.141593
```

Notice how there are no specialized variants of `keep` for producing atomic vectors like there is for `map`. That is because `keep` always preserves the type of its input, which is usually what's needed. If you are filtering a vector of integers, you want the output to also be a vector of integers. Sometimes, though, when filtering a list, you might want to convert the result to an atomic vector. The `purrr` package provides the handy `list_simplify` function for those cases:

```r
library(purrr)
list(1, "x", pi, TRUE, date()) |> 
  keep(is.numeric) |> 
  list_simplify()
#> [1] 1.000000 3.141593
```

Now that we have seen mapping and filtering, it is time to turn our attention to reducing. We have already written our own version of the `reduce` function when we introduced higher-order functions above, but `purrr` provides its own `reduce` function with a few more bells and whistles. The `reduce` function differs from `map` and `keep` in that it usually returns a single object instead of returning a list or a vector.

The simplest uses here are things like sums and products:

```r
library(purrr)
1:10 |> reduce(\(a, b) a + b)
#> [1] 55
1:10 |> reduce(`+`)
#> [1] 55
1:10 |> reduce(\(a, b) a * b)
#> [1] 3628800
1:10 |> reduce(`*`)
#> [1] 3628800
```

Notice how you can surround operators with backticks to refer to them as functions without having to wrap them in lambdas. In R, operators are just standard functions with a special syntax. You can even use them like this:

```r
`+`(2, 3)
#> [1] 5
```

These are the fundamental components of the MapReduce style, and can be combined to perform much more complex data processing operations.

::::challenge{id=sum_squares title="Sum of Squares"}

Using `map` and `reduce`, write a function that calculates the sum of the squares of the values in a list.

Your function should behave as below:

```r
library(purrr)
sum_of_squares <- \(xs) {
  # your code here
}

sum_of_squares(0)
#> [1] 0
sum_of_squares(1)
#> [1] 1
sum_of_squares(1:3)
#> [1] 14
sum_of_squares(-1)
#> [1] 1
sum_of_squares(c(-1, -2, -3))
#> [1] 14
```

Try to write it as a combination of `map` and `reduce` even if you know that `sum(xs^2)` would work just as well in R.

:::solution

```r
library(purrr)
sum_of_squares <- \(xs) {
  xs |>
    map(\(x) x^2) |>
    reduce(`+`)
}
```

:::

Now let's assume we're reading in these numbers from an input file, so they arrive as a list of strings.

Modify your function so that it passes the following tests:

```r
c("1", "2", "3") |> sum_of_squares()
#> [1] 14
c("-1", "-2", "-3") |> sum_of_squares()
#> [1] 14
```

:::solution

```r
library(purrr)
sum_of_squares <- \(xs) {
  xs |>
    map(as.numeric) |>
    map(\(x) x^2) |>
    reduce(`+`)
}
```

:::

Finally, just like we can have comments in code, we'd like it to be possible for users to comment out numbers in the input file they give to our program. Extend your function so that the following tests pass (don't worry about passing the first set of tests with lists of integers):

```r
c("1", "2", "3") |> sum_of_squares()
#> [1] 14
c("-1", "-2", "-3") |> sum_of_squares()
#> [1] 14
c("1", "2", "#100", "3") |> sum_of_squares()
#> [1] 14
```

:::solution

```r
library(purrr)
sum_of_squares <- \(xs) {
  xs |>
    keep(\(x) !(x |> startsWith("#"))) |>
    map(as.numeric) |>
    map(\(x) x^2) |>
    reduce(`+`)
}
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

Raymond Hettinger, one of the Python core developers, [said in 2013](https://x.com/raymondh/status/324664257004322817):

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
