---
name: Higher Order Functions
dependsOn: [
    software_architecture_and_design.functional.recursion_python,
]
tags: [python]
---



## Lambda Functions

If we build our programs in a functional way, we tend to end up with a lot of small, one line functions which perform very simple operations.
For example, we might have a function which adds one to a number:

~~~ python
def add_one(x):
    return x + 1

print(add_one(1))
~~~
{: .language-python}

~~~
2
~~~
{: .output}

If we have a lot of these smaller functions which only get used once, it makes more sense to define them where they're used.

**Lambda functions** are small, nameless functions which fulfil this need.
The structure of these functions is not dissimilar to a normal python function definition - we have a keyword `lambda`, a list of parameters, a colon, then the function body.
In Python, the function body is limited to a single expression, which becomes the return value.

~~~ python
add_one = lambda x: x + 1

print(add_one(1))
~~~
{: .language-python}

~~~
2
~~~
{: .output}

We have assigned the lambda function to a variable, so we can see it more clearly, but we'd normally use it immediately.
Most style guides (we'll come back to these later in the course) consider it bad style to assign a lambda to a variable.
This is because the main point of lambdas in Python is to avoid having to name the function - it's a bit strange to do that and then give it a name.
Because of this, if you find yourself wanting to name a lambda, just use a normal function instead.

Lambda functions may also make it slightly harder to do debugging (coming in a few days time) as the debugger and error messages doesn't have a name to show for them.

Anonymous functions, exist in many modern languages, though they may not be called 'lambda functions' and may be more or less complex to use.
For example, see [Lambda Expressions](https://en.cppreference.com/w/cpp/language/lambda) in C++ with precise control over the visibility of variables inside the function scope, and the multiple methods available in Javascript such as [Function Expressions](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions#the_function_expression_function_expression), which use syntax much more similar to named function definition.

Finally, there's another common use case of lambda functions that we'll come back to later when we see **closures**.
Due to their simplicity, it can be useful to have a lamdba function as the inner function in a closure.

## Map, Filter, Reduce

One of the most important applications of functional programming in recent years is the Map, Filter, Reduce model of data processing, usually refered to as **MapReduce**.
This model is particularly useful for the processing and analysis of **Big Data** using tools such as Spark or Hadoop.

Note that the `map` and `filter` functions in Python use **lazy evaluation**.
This means that values in an iterable collection are not actually calculated until you need them.
We'll explain some of the implications of this a little later, but for now, we'll just use `list()` to convert the results to a normal list.
In these examples we also see the more typical usage of lambda functions.

The `map` function, takes a function and applies it to each value in an **iterable**.
Here, 'iterable' means any object that can be iterated over - for more details see the [Iterable Abstract Base Class documentation](https://docs.python.org/3/library/collections.abc.html#collections.abc.Iterable).
The results of each of those applications become the values in the **iterable** that is returned.

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

## Comprehensions

Comprehensions are a more Pythonic way to structure map and filter operations.
They serve exactly the same purpose, but are more concise and can be easier to structure in more complex cases, such as mapping over a 2d data structure.
Using comprehensions also gives us control over which data structures we end up with, rather than always getting back a `map` or `filter` iterable.

### List Comprehensions

The **list comprehension** is probably the most commonly used comprehension type.
As you might expect from the name, list comprehensions produce a list from some other iterable type.
In effect they are the same as using `map` and/or `filter` and using `list()` to cast the result to a list, as we did previously.

All comprehension types are structured in a similar way, using the syntax for a literal of that type (in the case below, a list literal) containing what looks like the top of a for loop.
To the left of the `for` we put the equivalent of the map operation we want to use:

~~~
print([i for i in range(5)])
print([2 * i for i in range(5)])
~~~
{: .language-python}

~~~
[0, 1, 2, 3, 4]
[0, 2, 4, 6, 8]
~~~
{: .output}

We can also use list comprehensions to perform the equivalent of a filter operation, by putting the filter condition at the end:

~~~
print([2 * i for i in range(5) if i % 2 == 0])
~~~
{: .language-python}

~~~
[0, 4, 8]
~~~
{: .output}

### Dictionary and Set Comprehensions

Dictionary and set comprehensions are fundamentally the same as list comprehensions but use the dictionary or set literal syntax.

So set comprehensions are:

~~~
print({2 * i for i in range(5)})
~~~
{: .language-python}

~~~
{0, 2, 4, 6, 8}
~~~
{: .output}

While dictionary comprehensions are:

~~~
print({i: 2 * i for i in range(5)})
~~~
{: .language-python}

~~~
{0: 0, 1: 2, 2: 4, 3: 6, 4: 8}
~~~
{: .output}

> ## Why No Tuple Comprehension
>
> Raymond Hettinger, one of the Python core developers, said in 2013:
>
> ~~~
> Generally, lists are for looping; tuples for structs. Lists are homogeneous; tuples heterogeneous. Lists for variable length.
> ~~~
>
> Since tuples aren't intended to represent sequences, there's no need for them to have a comprehension structure.
>
{: .callout}

## Generators

### Generator Expressions

**Generator expressions** look exactly like you might expect a tuple comprehension (which don't exist) to look, and behaves a little differently from the other comprehensions.
What happens if we try to use them in the same was as we did list comprehensions?

~~~
print((2 * i for i in range(5)))
~~~
{: .language-python}

~~~
<generator object <genexpr> at 0x7efc21efcdd0>
~~~
{: .output}

Like the `map` and `filter` functions, generator expressions are not evaluated until you iterate over them.

~~~
for i in (2 * i for i in range(5)):
    print(i)
~~~
{: .language-python}