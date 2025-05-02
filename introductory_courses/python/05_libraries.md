---
name: Libraries
dependsOn: [introductory_courses.python.04_built_in_functions_and_help]
tags: [python]
learningOutcomes:
  - Understanding and importing libraries.
attribution:
  - citation: >
      "Programming with Python" course by the Carpentries
    url: https://swcarpentry.github.io/python-novice-inflammation/
    image: https://carpentries.org/assets/img/TheCarpentries.svg
    license: CC-BY-4.0
---

## The power of libraries

Most of the power of a programming language lies in its libraries.

A _library_, or a _package_ is a collection of files (each called a _module_) that contains functions, types etc. for use in other programs.

- May also contain data values (e.g., numerical constants) and other things.
- A good libraries content will be related, but there's no way to enforce that.

The [Python standard library](https://docs.python.org/3/library/) is an extensive suite of modules that comes with Python itself.

- Many additional libraries are available from [PyPI](https://pypi.python.org/pypi/) (the Python Package Index) or elsewhere.
- We will later see how to write our own libraries.

Using libraries brings several benefits:

- We don't have to write everything ourselves, often the task you are trying to accomplish has been done before.
- Good libraries may also be documented, tested, optimised and continually maintained such that they are more reliable than our own code.
- Our code is easier to understand to others, since they may be familiar with the library we are using.
- We can learn good programming practices from libraries.
  - How did they accomplish something?

:::callout

## Libraries and modules

A library is a collection of modules, but the terms are often used
interchangeably, especially since many libraries only consist of a single
module, so don't worry if you mix the terms.
:::

## Importing modules

A module must be imported before it can be used.

- Use an `import` statement to load a library module into a program's memory.
- Once imported, we refer to "things" from the module as `module_name.thing_name`.
  - Python uses `.` to mean "part of".
  - Modules can be nested within one another e.g. `module_name.submodule.function`.

Using `math`, one of the modules in the standard library:

```python
import math

print('pi is', math.pi)
print('cos(pi) is', math.cos(math.pi))
```

```text
pi is 3.141592653589793
cos(pi) is -1.0
```

We always have to refer to each item with the module's name.

- `math.cos(pi)` won't work: the reference to `pi` doesn't somehow "inherit" the function's reference to `math`.

## Use `help` on modules

We can find more out about a module with `help`, this works just like with a function.

```python
help(math)
```

```text
Help on module math:

NAME
    math

MODULE REFERENCE
    http://docs.python.org/3/library/math

    The following documentation is automatically generated from the Python
    source files.  It may be incomplete, incorrect or include features that
    are considered implementation detail and may vary between Python
    implementations.  When in doubt, consult the module reference at the
    location listed above.

DESCRIPTION
    This module is always available.  It provides access to the
    mathematical functions defined by the C standard.

FUNCTIONS
    acos(x, /)
        Return the arc cosine (measured in radians) of x.
⋮ ⋮ ⋮
```

## Importing specific items

We can simplify and speed up our programs by importing only what we need.

- Use the form `from ... import ...` to load only specific items from a library module.
- Then refer to them directly without library name as prefix.

```python
from math import cos, pi

print('cos(pi) is', cos(pi))
```

```text
cos(pi) is -1.0
```

## Creating aliases

We can create an alias for a library module when importing it to make our programs clearer and shorter.

- Use the form `import ... as ...` to give a library a short _alias_ while importing it.
- Then refer to items in the library using that shortened name.

```python
import math as m

print('cos(pi) is', m.cos(m.pi))
```

```text
cos(pi) is -1.0
```

- Commonly used for libraries that are frequently used or have long names.
  - E.g., the `matplotlib` plotting library is often aliased as `mpl`.
    Overusing aliasing can make programs harder to understand, since readers must learn your program's aliases.

We can, of course, also combine both selective importing and aliasing using `from ... import ... as ...` to do both things at once.

```python
from matplotlib import pyplot as plt
```

Will alias just the matplotlib.pyplot module into `plt`.

::::challenge{id="locating_the_right_module" title="Locating the Right Module"}

You want to select a random character from a string:

```python
bases = 'ACTTGCTTGAC'
```

1. Which standard library [stdlib] module could help you?
2. Which function would you select from that module? Are there alternatives?
3. Try to write a program that uses the function.

:::solution

The [random module](https://docs.python.org/3/library/random.html) seems like it could help you.

The string has 11 characters, each having a positional index from 0 to 10.
You could use either `random.randrange` or `random.randint` functions
to get a random integer between 0 and
10, and then pick out the character at that position:

```python
from random import randrange

random_index = randrange(len(bases))
print(bases[random_index])
```

or more compactly:

```python
from random import randrange

print(bases[randrange(len(bases))])
```

Perhaps you found the `random.sample` function? It allows for slightly less typing:

```python
from random import sample

print(sample(bases, 1)[0])
```

Note that this function returns a list of values.
We will learn about [lists](08_lists) later.

There's also other functions you could use, but with more convoluted code as a result.
:::
::::

::::challenge{id="when_is_help_available" title="When is Help Available?"}

When a colleague of yours types `help(math)`, Python reports an error:

```text
NameError: name 'math' is not defined
```

What has your colleague forgotten to do?

:::solution

Importing the math module (`import math`)
:::
::::

::::challenge{id="importing_with_aliases" title="Importing With Aliases"}

1. Fill in the blanks so that the program below prints `90.0`.
2. Rewrite the program so that it uses `import` _without_ `as`.
3. Which form do you find easier to read?

```python nolint
import math as m
angle = ____.degrees(____.pi / 2)
print(____)
```

:::solution

```python
import math as m
angle = m.degrees(m.pi / 2)
print(angle)
```

can be written as

```python
import math
angle = math.degrees(math.pi / 2)
print(angle)
```

Since you just wrote the code and are familiar with it, you might actually find the first version easier to read.
But when trying to read a huge piece of code written by someone else, or when getting back to your own huge piece of code after several months, non-abbreviated names are often easier, except where there are clear abbreviation conventions.
:::
::::

::::challenge{id="many_ways_to_import_libraries" title="Many Ways To Import Libraries"}

Match the following print statements with the appropriate library calls.

Print commands:

1. `print("sin(pi/2) =", sin(pi/2))`
2. `print("sin(pi/2) =", m.sin(m.pi/2))`
3. `print("sin(pi/2) =", math.sin(math.pi/2))`

Library calls:

1. `from math import sin, pi`
2. `import math`
3. `import math as m`
4. `from math import *`

:::solution

1. Library calls 1 and 4. In order to directly refer to `sin` and `pi` without
   the library name as prefix, you need to use the `from ... import ...`
   statement. Whereas library call 1 specifically imports the two functions
   `sin` and `pi`, library call 4 imports all functions in the `math` module.
2. Library call 3. Here `sin` and `pi` are referred to with a shortened library
   name `m` instead of `math`. Library call 3 does exactly that using the
   `import ... as ...` syntax - it creates an alias for `math` in the form of
   the shortened name `m`.
3. Library call 2. Here `sin` and `pi` are referred to with the regular library
   name `math`, so the regular `import ...` call suffices.

**Note:** although library call 4 works, importing all names from a module using a wildcard
import is [not recommended](https://pep8.org/#imports) as it makes it unclear which names from the module
are used in the code. In general it is best to make your imports as specific as possible and to
only import what your code uses. In library call 1, the `import` statement explicitly tells us
that the `sin` function is imported from the `math` module, but library call 4 does not
convey this information.
:::
::::

::::challenge{id="importing_specific_items" title="Importing specific items"}

1. Fill in the blanks so that the program below prints `90.0`.
2. Do you find this version easier to read than preceding ones?
3. Why _wouldn't_ programmers always use this form of `import`?

```python nolint
____ math import ____, ____
angle = degrees(pi / 2)
print(angle)
```

:::solution

```python
from math import degrees, pi
angle = degrees(pi / 2)
print(angle)
```

Most likely you find this version easier to read since it's less dense.
The main reason not to use this form of import is to avoid name clashes.
For instance, you wouldn't import `degrees` this way if you also wanted to use the name `degrees` for a variable or function of your own.
Or if you were to also import a function named `degrees` from another library.
:::
::::

::::challenge{id="reading_error_messages" title="Reading Error Messages"}

1. Read the code below and try to identify what the errors are without running it.
2. Run the code, and read the error message. What type of error is it?

```python
from math import log
log(0)
```

:::solution

```text
---------------------------------------------------------------------------
ValueError                                Traceback (most recent call last)
<ipython-input-1-d72e1d780babin <module>
      1 from math import log
----2 log(0)

ValueError: math domain error
```

1. The logarithm of `x` is only defined for `x > 0`, so 0 is outside the
   domain of the function.
2. You get an error of type `ValueError`, indicating that the function
   received an inappropriate argument value. The additional message
   "math domain error" makes it clearer what the problem is.

:::
::::

## Useful links

- [pypi](https://pypi.python.org/pypi/)
- [stdlib](https://docs.python.org/3/library/)
- [randommod](https://docs.python.org/3/library/random.html)
- [pep8-imports](https://pep8.org/#imports)
