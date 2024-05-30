---
name: Functions
dependsOn: [
    software_architecture_and_design.procedural.containers_python,
]
tags: [python]
learningObjectives:
    - Define a function that takes parameters.
    - Return a value from a function.
    - Test and debug a function.
    - Set default values for function parameters.
    - Explain why we should divide programs into small, single-purpose functions.
---
## Using Functions
In most modern programming languages these procedures are called **functions**.
Python has many pre-defined functions built in and we've already met some of them.

To use, or **call**, a function we use the name of the function, followed by brackets containing any **arguments** that we wish to **pass** to the function.
All functions in Python **return** a single value as their result.

:::callout
## Return Values
Though all functions return a single value in Python, this value may itself be:
- a collection of values
- `None` - a special value that is interpreted as though nothing has been returned
:::

~~~python
char_count = len('Python')
print(char_count)
~~~

~~~
6
~~~

Some functions are a little different in that they belong to an object, so must be accessed through the object using the dot operator.
These are called **methods** or **member functions**.
We've already seen some of these as well, but we'll see more when we get to the Object Oriented Paradigm later.

~~~python
nums = [1, 2, 3]
nums.append(4)

print(nums)
~~~

~~~
[1, 2, 3, 4]
~~~

The append function is actually also one of these functions that return `None`.
We can test this again by printing its output.

~~~ python
nums = [1, 2, 3]
result = nums.append(4)

print(result)
print(nums)
~~~

~~~
None
[1, 2, 3, 4]
~~~

It's relatively common for a function to return `None` if the purpose of the function is to modify one of its input values.
That's the case here - the purpose of the `append` function is to append a value to an existing list.


## Creating Functions

Although Python has many built in functions, it wouldn't be much use if we couldn't also define our own.
Most languages use a keyword to signify a **function definition**, in Python that keyword is `def`.

~~~ python
def add_one(value):
    return value + 1

two = add_one(1)
print(two)
~~~

~~~
2
~~~

~~~ python
def say_hello(name):
    return 'Hello, ' + name + '!'

print(say_hello('World'))
~~~

~~~
Hello, World!
~~~

To define a function, we use `def`, followed by the name of the function and its **parameters** in brackets.
Just like with other code blocks (like `for` and `if`), we use a colon to signify the body of the function and indent the body by four spaces.

Note that we used the word **argument** when we were calling a function, but **parameter** when we were defining one.
The parameters of a function are the names of the variables which are created inside the function to accept its input data.
The arguments of a function are the values that we give to a function when we call it, to put into its parameters.


Sometimes, it's useful for a parameter to have a default value.
When we call a function, parameters with default values can be used in one of three ways:

1. We can use the default value, by not providing our own value
2. We can provide our own value in the normal way
3. We can provide a value in the form of a **named argument** - arguments which are not named are called **positional arguments**

~~~ python
def say_hello(name='World'):
    return 'Hello, ' + name + '!'

print(say_hello())
print(say_hello('Python'))
print(say_hello(name='Named Argument'))
~~~

~~~
Hello, World!
Hello, Python!
Hello, Named Argument!
~~~

:::callout
## Declarations and Definitions

Some languages have a distinction between **declaration** and **definition** (or **implementation**) of a function.
In these languages function declaration provides a name and information about its return type and parameters, but does not provide the actual code inside the function.
Function definition is when the code is provided, and the function's behaviour is defined.

This distinction can be useful as it allows us to call a function from code which appears above the function definition in the source file.
Python does not have this distinction - that is, a function is always declared and defined at the same time - so we must define the function before we can use it.

One common language that does have this distinction is C++.
See [this page](https://docs.microsoft.com/en-us/cpp/cpp/declarations-and-definitions-cpp?view=vs-2019) for more information.
:::

::::challenge{id=combining-strings title="Combining Strings"}

"Adding" two strings produces their concatenation: `'a'` + `'b'` is `'ab'`.
Write a short function called `fence` that takes two parameters called original and wrapper and returns a new string that has the wrapper character at the beginning and end of the original.
A call to your function should look like this:

~~~ python
print(fence('name', '*'))
~~~

~~~
*name*
~~~

:::solution

~~~ python
def fence(original, wrapper):
    return wrapper + original + wrapper
~~~
:::
::::

::::challenge{id=custom-greetings title="Custom Greetings"}

Create a new version of the `say_hello` function which has two parameters, `greeting` and `name`, both with default values.
How many different ways can you call this function using combinations of named and positional arguments?

:::solution

~~~ python
def say_hello(greeting='Hello', name='World'):
    return greeting + ', ' + name + '!'

# No arguments - both default values
print(say_hello())

# One positional argument, one default value
print(say_hello('Hello'))

# One named argument
print(say_hello(greeting='Hello'))
print(say_hello(name='World'))

# Both positional arguments
print(say_hello('Hello', 'World'))

# One positional argument, then one named argument
print(say_hello('Hello', name='World'))

# Both named arguments
print(say_hello(greeting='Hello', name='World'))
~~~

You should have found that Python will not let you provide positional arguments after named ones.

:::
::::

::::challenge{id=function-parameters title="How do function parameters work?"}

It’s important to note that even though variables defined inside a function may use the same name as variables defined outside, they don’t refer to the same thing.
This is because of variable **scoping**.

Within a function, any variables that are created (such as parameters or other variables), only exist within the **scope** of the function.

For example, what would be the output from the following:

~~~ python
f = 0
k = 0

def multiply_by_10(f):
    k = f * 10
    return k

multiply_by_10(2)
multiply_by_10(8)

print(k)
~~~

1. 20
2. 80
3. 0

:::solution

3 - the f and k variables defined and used within the function do not interfere
with those defined outside of the function.

This is really useful, since it means we don’t have to worry about conflicts
with variable names that are defined outside of our function that may cause it
to behave incorrectly.  This is known as variable scoping.

:::
::::

## Function Composition

One of the main reasons for defining a function is to encapsulate our code, so
that it can be used without having to worry about exactly how the computation is
performed.  This means we're free to implement the function however we want,
including deferring some part of the task to another function that already
exists.

For example, if some data processing code we're working on needs to be able to
accept temperatures in Fahrenheit, we might need a way to convert these into
Kelvin.  So we could write these two temperature conversion functions:

~~~ python
def fahr_to_cels(fahr):
    # Convert temperature in Fahrenheit to Celsius
    cels = (fahr + 32) * (5 / 9)
    return cels

def fahr_to_kelv(fahr):
    # Convert temperature in Fahrenheit to Kelvin
    cels = (fahr + 32) * (5 / 9)
    kelv = cels + 273.15
    return kelv

print(fahr_to_kelv(32))
print(fahr_to_kelv(212))
~~~

But if we look at these two functions, we notice that the conversion from
Fahrenheit to Celsius is actually duplicated in both functions.  This makes
sense, since this is a necessary step in both functions, but duplicated code is
wasteful and increases the chance of us making an error - what if we made a typo
in one of the equations?

So, we can remove the duplicated code, by calling one function from inside the other:

~~~ python
def fahr_to_cels(fahr):
    # Convert temperature in Fahrenheit to Celsius
    cels = (fahr + 32) * (5 / 9)
    return cels

def fahr_to_kelv(fahr):
    # Convert temperature in Fahrenheit to Kelvin
    cels = fahr_to_cels(fahr)
    kelv = cels + 273.15
    return kelv

print(fahr_to_kelv(32))
print(fahr_to_kelv(212))
~~~

Now we've removed the duplicated code, but we might actually want to go one step
further and remove some of the other unnecessary bits:

~~~ python
def fahr_to_cels(fahr):
    # Convert temperature in Fahrenheit to Celsius
    return (fahr + 32) * (5 / 9)

def fahr_to_kelv(fahr):
    # Convert temperature in Fahrenheit to Kelvin
    return fahr_to_cels(fahr) + 273.15

print(fahr_to_kelv(32))
print(fahr_to_kelv(212))
~~~

Now we have each function down to one statement, which should be easier to read and hopefully has reduced the chance of us making a mistake.
Whether you actually prefer the second or third version is up to you, but we should at least try to reduce duplication where posssible.

## Managing Academics

As a common example to illustrate each of the paradigms, we'll write some code to help manage a group of academics and their publications.

First, let's create a data structure to keep track of the papers that a group of academics are publishing.
Note that we could use an actual `date` type to store the publication date, but they're much more complicated to work with, so we'll just use the year.

~~~ python
academics = [
    {
        'name': 'Alice',
        'papers': [
            {
                'title': 'My science paper',
                'date': 2015
            },
            {
                'title': 'My other science paper',
                'date': 2017
            }
        ]
    },
    {
        'name': 'Bob',
        'papers': [
            {
                'title': 'Bob writes about science',
                'date': 2018
            }
        ]
    }
]
~~~

We want a convenient way to add new papers to the data structure.

~~~ python
def write_paper(academics, name, title, date):
    paper = {
        'title': title,
        'date': date
    }

    for academic in academics:
        if academic['name'] == name:
            academic['papers'].append(paper)
            break
~~~

We're introducing a new keyword here, `break`, which exits from inside a loop.
When the `break` keyword is encountered, execution jumps to the next line
outside of the loop.  If there isn't a next line, as in our example here, then
it's the end of the current block of code.

This is useful when we have to search for something in a list - once we've found
it we can stop searching and don't need to waste time looping over the remaining
items.

What happens if we call this function for an academic who doesn't exist?

:::callout
## Exceptions
In many programming languages, we use **exceptions** to indicate that exceptional behaviour has occured and the flow of execution should be diverted.

Exceptions are often **raised** (or **thrown** in some other programming languages) as the result of an error condition.
The flow of execution is then returned (the exception is **caught** or **handled**) to a point where the error may be corrected or logged.
For the moment we'll just raise the exception, and assume that it will get handled properly by someone using our code.

In Python, exceptions may also be used to alter the flow of execution even when an error has not occured.
For example, when iterating over a collection, a `StopIteration` exception is the way in which Python tells a loop construct to terminate, though this is hidden from you.

~~~ python
def write_paper(academics, name, title, date):
    paper = {
        'title': title,
        'date': date
    }

    for academic in academics:
        if academic['name'] == name:
            academic['papers'].append(paper)
            break

    else:
        raise KeyError('Named academic does not exist')
~~~
{: .language-python}

The `for-else` structure used here is relatively unusual, but can be useful when you're using a loop to search for a value.
The `else` block is executed if and only if the loop execution completes normally - i.e. when `break` is not used.
When you're using a loop to search for something, this means that it has not been found.

For more information see [this section](https://docs.python.org/3/tutorial/controlflow.html#break-and-continue-statements-and-else-clauses-on-loops) of the Python documentation.
:::


::::challenge{id=passing-lists-to-functions title="Passing Lists to Functions"}

We have seen previously that functions are not able to change the value of a variable which is used as their argument.

~~~ python
def append_to_list(l):
    l.append('appended')
    l = [1, 2, 3]
    l.append('again')
    return l

a_list = ['this', 'is', 'a', 'list']

print(append_to_list(a_list))
print(a_list)
~~~

Before running this code, think about what you expect the output to be.
Now run the code, does it behave as you expected?
Why does the function behave in this way?

:::solution
~~~
[1, 2, 3, 'again']
['this', 'is', 'a', 'list', 'appended']
~~~

The reason for this behaviour is that lists are **mutable** so when we pass one in to a function any modifications are made to the actual list as it exist in memory.
Using `=` to assign a new value creates a new list in memory (it does not modify the existing list) and assigns it to the variable / name `l`.
Any changes made to `l` after this are changes to the new list, so do not affect the previous list.
The original list still exists, but `l` no longer points to it.
Only the variable name `a_list` still points to the original.

For this reason it can be useful to think of the `=` sign as **binding a value to a name**.
:::
::::

::::challenge{id=counting-publications, title="Counting Publications"}

Write a function called `count_papers`, that when called with `count_papers(academics)` returns the total number of publications.

:::solution

One possible solution is:

~~~ python
def count_papers(academics):
    count = 0

    for academic in academics:
        count = count + len(academic['papers'])

    return count

total = count_papers(academics)
print(total)
~~~

~~~
3
~~~
:::
::::

::::challenge{id=listing-publications title="Listing Publications"}

Write a function called `list_papers`, that when called with `list_papers(academics)` returns a list of all publication titles.

:::solution

One possible solution is:

~~~ python
def list_papers(academics):
    papers = []

    for academic in academics:
        papers = papers + academic['papers']

    return papers
~~~
:::
::::

## Key Points:
- Functions allow us to separate out blocks of code which perform a common task
- Functions have their own scope and do not clash with variables defined outside