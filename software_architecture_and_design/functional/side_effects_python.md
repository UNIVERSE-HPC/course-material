---
name: Side Effects
dependsOn: [
]
tags: [python]
---

## Program state

The current state of a given program is composed of many different parts, some
of which are clear, others which are more opaque. The
most obvious state is the values of all the current variables in the scope. Take
this code snippet:

```python
y = [3, 2]
x = [2]
x = y
```

Here we have two lists `x` and `y` with initial values, when the last line
is executed the state is updated so that `x` now has the value 3. We can clearly
see this from the code given, but it can be less obvious when this code is
hidden in a function:

```python
def my_cool_function(x, y):
    x[:] = y

y = [3, 2]
x = [2]
my_cool_function(x, y)
```

Now imagine we have a global variable defined elsewhere that is updated by
`my_cool_function`, this is not even passed into the function so it is even
more unclear that this is being updated. The global variable and function might
even be declared in a separate file and brought in via an `import`

```python
z = 3 
def my_cool_function(x, y):
    x = y
    z++;


y = [3, 2]
x = [2]
my_cool_function(x, y)
```

Finally, any interaction with the Operating System is yet another, even more
opaque, part of the current state. This includes memory allocations, and file
IO.

```python
x = np.zeros(1000) # do we have enough RAM available for this?

myfile = open("example.txt", "w") # does this file exist? Do I have write permissions?

line = myfile.readline() # did I open this for reading?
line = myfile.readline() # Same call to readline, but result is different!
```

The main downside of having a state that is constantly updated is that it makes
it harder for us to *reason* about our code, to work out what it is doing.
However, the upside is that we can use state to store temporary data to make
calculations more efficient and store temporary data. For example an iteration
loop that keeps track of a running total is a common pattern in procedural
programming:

```python
result = 0
for x in data:
    result += expensive_computation(x)
```

## Side Effects and Pure Functions

Functional computations only rely on the values that are provided as inputs to a
function and not on the state of the program that precedes the function call.
They do not modify data that exists outside the current function, including the
input data - this property is referred to as the *immutability of data*. This
means that such functions do not create any *side effects*, i.e. do not perform
any action that affects anything other than the value they return. For example:
printing text, writing to a file, modifying the value of an input argument, or
changing the value of a global variable. Functions without side affects that
return the same data each time the same input arguments are provided are called
*pure functions*.

::::challenge{id="pure-functions", title="Pure Functions"}

Which of these functions are pure?
If you're not sure, explain your reasoning to someone else, do they agree?

~~~python
def add_one(x):
    return x + 1

def say_hello(name):
    print('Hello', name)

def append_item_1(a_list, item):
    a_list += [item]
    return a_list

def append_item_2(a_list, item):
    result = a_list + [item]
    return result
~~~

:::solution
## Solution

1. `add_one` is pure - it has no effects other than to return a value and this value will always be the same when given the same inputs
2. `say_hello` is not pure - printing text counts as a side effect, even though it is the clear purpose of the function
3. `append_item_1` is not pure - the argument `a_list` gets modified as a side effect - try this yourself to prove it
4. `append_item_2` is pure - the result is a new variable, so this time `a_list` does not get modified - again, try this yourself
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
function does not  have side effects or non-deterministic behaviour, then all
of its behaviour is reflected in the value it returns. As a consequence of
this, any chain of combined pure functions is itself pure, so we keep all these
benefits when we are combining functions into a larger program. As an example
of this, we could make a function called `add_two`, using the `add_one`
function we already have.

~~~python
def add_two(x):
    return add_one(add_one(x))
~~~

**Parallelisability** is the ability for operations to be performed at the same
*time (independently). If we know that a function is fully pure and we have got
*a lot of data, we can often improve performance by splitting data and
*distributing the computation across multiple processors. The output of a pure
*function depends only on its input, so we will get the right result regardless
*of when or where the code runs.

:::callout
## Everything in Moderation
Despite the benefits that pure functions can bring, we should not be trying to
use them everywhere. Any software we write needs to interact with the rest of
the world somehow, which requires side effects. With pure functions you cannot
read any input, write any output, or interact with the rest of the world in any
way, so we cannot usually write useful software using just pure functions.
Python programs or libraries written in functional style will usually not be as
extreme as to completely avoid reading input, writing output, updating the state
of internal local variables, etc.; instead, they will provide a
functional-appearing interface but may use non-functional features internally.
An example of this is the [Python Pandas library](https://pandas.pydata.org/)
for data manipulation built on top of NumPy - most of its functions appear pure
as they return new data objects instead of changing existing ones.
:::

There are other advantageous properties that can be derived from the functional
approach to coding. In languages which support functional programming, a
function is a *first-class object* like any other object - not only can you
compose/chain functions together, but functions can be used as inputs to, passed
around or returned as results from other functions (remember, in functional
programming *code is data*). This is why functional programming is suitable for
processing data efficiently - in particular in the world of Big Data, where code
is much smaller than the data, sending the code to where data is located is
cheaper and faster than the other way round. Let's see how we can do data
processing using functional programming.