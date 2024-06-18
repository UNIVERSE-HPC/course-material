---
name: Built-in Functions and Help
dependsOn: [
    introductory_courses.python.03_writing_and_running_ide
]
tags: [python]
attribution: 
    - citation: >
        "Programming with Python" course by the Carpentries
      url: https://swcarpentry.github.io/python-novice-inflammation/
      image: https://carpentries.org/assets/img/TheCarpentries.svg
      license: CC-BY-4.0
---

## Use comments to add documentation to programs

Any text preceded by the hash mark (`#`) is ignored by Python, this is called a "comment".

``` python
# This sentence isn't executed by Python.
adjustment = 0.5   # Neither is this - anything after '#' is ignored.
```

Comments make programs easier for humans to understand, good code is written for humans to read, not just for computers to execute.

## Function inputs

We have used functions already --- now let's take a closer look.

An *argument* is a value passed into a function.
Functions can take any number of arguments, including zero.

* `len` takes exactly one.
* `int`, `str`, and `float` take one (and return one).
* `print` takes zero or more.
  * `print` with no arguments prints a blank line.

Function calls must always include the parentheses, even if they're empty, so that Python knows a function is being called.

``` python
print('before')
print()
print('after')
```

produces this output:

``` text
before

after
```

## Function returns

Every function call produces some result but not every function returns something.

* If the function doesn't have a useful result to return, it usually returns the special value `None`.
  `None` is a Python object that stands in anytime there is no value.

``` python
result = print('example')
print('result of print is', result)
```

``` text
example
result of print is None
```

## Commonly-used built-in functions

* Use `max` to find the largest value of one or more values.
* Use `min` to find the smallest.
* Both work on character strings as well as numbers.
  * "Larger" and "smaller" use (0-9, A-Z, a-z) to compare letters.

``` python
print(max(1, 2, 3))
print(min('a', 'A', '0'))
```

``` text
3
0
```

## Functions may only work for certain (combinations of) arguments

`max` and `min` must be given at least one argument; "Largest of the empty set" is a meaningless question.

They must be given things that can meaningfully be compared,

``` python
print(max(1, 'a'))
```

``` text
TypeError                                 Traceback (most recent call last)
<ipython-input-52-3f049acf3762> in <module>
----> 1 print(max(1, 'a'))

TypeError: '>' not supported between instances of 'str' and 'int'
```

## Functions may have default values

Some functions have optional values which are used when the user does not provide a value for the argument.

`round` will round off a floating-point number, by default, it rounds to zero decimal places.

``` python
round(3.712)
```

``` text
4
```

We can specify the number of decimal places we want,

``` python
round(3.712, 1)
```

``` text
3.7
```

## Methods

Functions can be tied to a particular object, these are called *methods*, we will make extensive use of these when learning about [pandas](07_pandas_dataframes).
Methods behave in the same way as functions, and are called with parentheses like functions.
They can be accessed from their base object with a dot (`obj.method()`).

* Some methods are used for internal Python operations, these are marked with double underlines.

``` python
my_string = 'Hello world!'  # creation of a string object 

print(len(my_string))       # the len function takes a string as an argument and returns the length of the string

print(my_string.swapcase()) # calling the swapcase method on the my_string object

print(my_string.__len__())  # calling the internal __len__ method on the my_string object, used by len(my_string)

```

``` text
12
hELLO WORLD!
12
```

* You might even see them chained together, in which case they operate left to right.

``` python
print(my_string.isupper())          # Not all the letters are uppercase
print(my_string.upper())            # This capitalizes all the letters

print(my_string.upper().isupper())  # Now all the letters are uppercase
```

``` text
False
HELLO WORLD
True
```

## Use the built-in function `help`

Every built-in function has online documentation, you can access this with the `help` function.

``` python
help(round)
```

``` text
Help on built-in function round in module builtins:

round(number, ndigits=None)
    Round a number to a given precision in decimal digits.
    
    The return value is an integer if ndigits is omitted or None.  Otherwise
    the return value has the same type as the number.  ndigits may be negative.
```

## Syntax errors

When you execute Python code, mistakes in your syntax will be reported immediately.
This is called a *syntax error*.
Here, syntax refers to the structure of a program and the rules about that structure, essentially the grammar of programming.

* Python won't even try to run the program if it can't be parsed.

``` python
# Forgot to close the quote marks around the string.
name = 'Feng
```

```text
  File "<ipython-input-56-f42768451d55>", line 2
    name = 'Feng
                ^
SyntaxError: EOL while scanning string literal
```

``` python
# An extra '=' in the assignment.
age = = 52
```

```text
  File "<ipython-input-57-ccc3df3cf902>", line 2
    age = = 52
          ^
SyntaxError: invalid syntax
```

* Look more closely at the error message:

``` python
print("hello world"
```

```text
  File "<ipython-input-6-d1cc229bf815>", line 1
    print ("hello world"
                        ^
SyntaxError: unexpected EOF while parsing
```

* The message indicates a problem on first line of the input ("line 1").
  * In this case the "ipython-input" section of the file name tells us that we are working with input into IPython, the Python interpreter used by the Jupyter Notebook.
* The `-6-` part of the filename indicates that the error occurred in cell 6 of our Notebook.
* Next is the problematic line of code, the problem is indicated with a `^` pointer.

Syntax errors can be caught before executing the program, and can be easier to fix.
We can make use of "linting" tools (such as those built in to our IDEs) which are effectively spellcheckers for programming to help us find, and correct, syntax errors.

## Runtime Errors

Python reports a `runtime error` when something goes wrong while a program is executing.
A runtime error is also called an `exception` because it usually indicates that something exceptional (and bad), outside the bounds of the programs normal operations has happened.

``` python
age = 53
remaining = 100 - aege # mis-spelled 'age'
```

``` text
NameError                                 Traceback (most recent call last)
<ipython-input-59-1214fb6c55fc> in <module>
      1 age = 53
----> 2 remaining = 100 - aege # mis-spelled 'age'

NameError: name 'aege' is not defined
```

As before, the error message indicates where the problem occurred.
It is up to you to diagnose and fix the problem, though there are tools which make this easier.

We will see more about [errors and exceptions](12_errors_and_exceptions) later.

* Fix syntax errors by reading the source and runtime errors by tracing execution.

:::callout

## Explore the Python docs

The [official Python documentation](https://docs.python.org/3/) is arguably the most complete source of information about the language.
It is available in different languages and contains a lot of useful resources.
The [Built-in Functions page](https://docs.python.org/3/library/functions.html) contains a catalogue of all of these functions, including the ones that we've covered in this lesson.
In addition, the vast majority of [libraries](05_libraries) which you will use in Python will have their own documentation, found online.
Some of these are more advanced and unnecessary at the moment, but others are very simple and useful.
:::
