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

## Use comments to add documentation to programs.

~~~
# This sentence isn't executed by Python.
adjustment = 0.5   # Neither is this - anything after '#' is ignored.
~~~
{: .language-python}

## A function may take zero or more arguments.

*   We have seen some functions already --- now let's take a closer look.
*   An *argument* is a value passed into a function.
*   `len` takes exactly one.
*   `int`, `str`, and `float` create a new value from an existing one.
*   `print` takes zero or more.
*   `print` with no arguments prints a blank line.
    *   Must always use parentheses, even if they're empty,
        so that Python knows a function is being called.

~~~
print('before')
print()
print('after')
~~~
{: .language-python}
~~~
before

after
~~~
{: .output}

## Every function returns something.

*   Every function call produces some result.
*   If the function doesn't have a useful result to return,
    it usually returns the special value `None`. `None` is a Python
    object that stands in anytime there is no value.

~~~
result = print('example')
print('result of print is', result)
~~~
{: .language-python}
~~~
example
result of print is None
~~~
{: .output}

## Commonly-used built-in functions include `max`, `min`, and `round`.

*   Use `max` to find the largest value of one or more values.
*   Use `min` to find the smallest.
*   Both work on character strings as well as numbers.
    *   "Larger" and "smaller" use (0-9, A-Z, a-z) to compare letters.

~~~
print(max(1, 2, 3))
print(min('a', 'A', '0'))
~~~
{: .language-python}
~~~
3
0
~~~
{: .output}

## Functions may only work for certain (combinations of) arguments.

*   `max` and `min` must be given at least one argument.
    *   "Largest of the empty set" is a meaningless question.
*   And they must be given things that can meaningfully be compared.

~~~
print(max(1, 'a'))
~~~
{: .language-python}
~~~
TypeError                                 Traceback (most recent call last)
<ipython-input-52-3f049acf3762> in <module>
----> 1 print(max(1, 'a'))

TypeError: '>' not supported between instances of 'str' and 'int'
~~~
{: .error}

## Functions may have default values for some arguments.

*   `round` will round off a floating-point number.
*   By default, rounds to zero decimal places.

~~~
round(3.712)
~~~
{: .language-python}
~~~
4
~~~
{: .output}

*   We can specify the number of decimal places we want.

~~~
round(3.712, 1)
~~~
{: .language-python}
~~~
3.7
~~~
{: .output}

## Functions attached to objects are called methods

* Functions take another form that will be common in the pandas episodes.
* Methods have parentheses like functions, but come after the variable.
* Some methods are used for internal Python operations, and are marked with double underlines.

~~~
my_string = 'Hello world!'  # creation of a string object 

print(len(my_string))       # the len function takes a string as an argument and returns the length of the string

print(my_string.swapcase()) # calling the swapcase method on the my_string object

print(my_string.__len__())  # calling the internal __len__ method on the my_string object, used by len(my_string)

~~~
{: .language-python}

~~~
12
hELLO WORLD!
12
~~~
{: .output}

* You might even see them chained together.  They operate left to right.

~~~
print(my_string.isupper())          # Not all the letters are uppercase
print(my_string.upper())            # This capitalizes all the letters

print(my_string.upper().isupper())  # Now all the letters are uppercase
~~~
{: .language-python}

~~~
False
HELLO WORLD
True
~~~
{: .output}

## Use the built-in function `help` to get help for a function.

*   Every built-in function has online documentation.

~~~
help(round)
~~~
{: .language-python}
~~~
Help on built-in function round in module builtins:

round(number, ndigits=None)
    Round a number to a given precision in decimal digits.
    
    The return value is an integer if ndigits is omitted or None.  Otherwise
    the return value has the same type as the number.  ndigits may be negative.
~~~
{: .output}

## Python reports a syntax error when it can't understand the source of a program.

*   Won't even try to run the program if it can't be parsed.

~~~
# Forgot to close the quote marks around the string.
name = 'Feng
~~~
{: .language-python}
~~~
  File "<ipython-input-56-f42768451d55>", line 2
    name = 'Feng
                ^
SyntaxError: EOL while scanning string literal
~~~
{: .error}

~~~
# An extra '=' in the assignment.
age = = 52
~~~
{: .language-python}
~~~
  File "<ipython-input-57-ccc3df3cf902>", line 2
    age = = 52
          ^
SyntaxError: invalid syntax
~~~
{: .error}

*   Look more closely at the error message:

~~~
print("hello world"
~~~
{: .language-python}
~~~
  File "<ipython-input-6-d1cc229bf815>", line 1
    print ("hello world"
                        ^
SyntaxError: unexpected EOF while parsing
~~~
{: .error}

*   The message indicates a problem on first line of the input ("line 1").
    *   In this case the "ipython-input" section of the file name tells us that
        we are working with input into IPython,
        the Python interpreter used by the Jupyter Notebook.
*   The `-6-` part of the filename indicates that
    the error occurred in cell 6 of our Notebook.
*   Next is the problematic line of code,
    indicating the problem with a `^` pointer.

## <a name='runtime-error'></a> Python reports a runtime error when something goes wrong while a program is executing.

~~~
age = 53
remaining = 100 - aege # mis-spelled 'age'
~~~
{: .language-python}
~~~
NameError                                 Traceback (most recent call last)
<ipython-input-59-1214fb6c55fc> in <module>
      1 age = 53
----> 2 remaining = 100 - aege # mis-spelled 'age'

NameError: name 'aege' is not defined
~~~
{: .error}

*   Fix syntax errors by reading the source and runtime errors by tracing execution.

> ## Explore the Python docs!
>
> The [official Python documentation](https://docs.python.org/3/) is arguably the most complete
> source of information about the language. It is available in different languages and contains a lot of useful
> resources. The [Built-in Functions page](https://docs.python.org/3/library/functions.html) contains a catalogue of
> all of these functions, including the ones that we've covered in this lesson. Some of these are more advanced and 
> unnecessary at the moment, but others are very simple and useful.
> 
{: .callout}