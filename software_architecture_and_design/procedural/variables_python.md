---
name: Variables
dependsOn: [
    technology_and_tooling.bash_shell.bash,
    technology_and_tooling.ide.cpp,
]
tags: [python]
learningObjectives:
  - Describe the fundamental types of variables.
  - Assign values to basic variables and make use of them.
  - Print the content of variables.
---

## Getting started

Create a new folder and open it in VSCode, e.g. on the command-line (bash or other) type the following:

```bash
mkdir procedural
cd procedual
code .
```

### Running a Python script

In the terminal start a new Python virtual environment.

```bash
python3 -m venv env
source env/bin/activate
```

Ensure you can start the Python interpreter with:

```bash
python
```

And then you are presented with something like:

```
Python 3.10.4 (main, Jun 29 2022, 12:14:53) [GCC 11.2.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> 
```

And lo and behold! You are presented with yet another prompt.
So now, we're actually running a Python interpreter from the shell - it's only yet another program we can run from the shell after all.
But note that shell commands won't work again until we exit the interpreter. Whilst we're in the Python interpreter, we can only use Python commands.

You can exit the interpreter and get back to the shell by typing:

```python
>>> exit()
```

...or alternatively pressing the `Control` and `D` keys at the same time.

Now create a new file `procedural.py` and copy in the following contents

```python
print('hello world')
```

Hit F1 to bring up the command palette and choose "Python: Run Python file in
Terminal", or click the play button in the top right hand corner. This should
print out "hello world" in the terminal

## Variables

Let's take a look at how variables are handled within Python. Compared to some
other languages such as C++ or JavaScript, variables in Python do not need to be
declared before assigning something to them. for example:

```python
six = 2 * 3
print(six)
```

~~~
6
~~~

Note that in terms of naming variables, Python's variables must begin with a letter.

Try and write the previous into your `procedural.py` file and run it (you can also select these lines and run
the command "Run Selection/Line in Python Terminal").

If we look for a variable that hasn't ever been defined, we get an error telling us so:

~~~python
print(seven)
~~~

~~~
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'seven' is not defined
~~~

You can also assign an arbitrary number of variables on the same line:

~~~python
one, two = 1, 2
~~~

::::challenge{title="Sorting out references" id=sorting_reference}

What does the following program print out?

~~~python
first, second = 1, 2
third, fourth = second, first
print(third, fourth)
~~~

:::solution

 ~~~
 2 1
 ~~~

:::
::::


Although we commonly refer to `variables` even in Python (because it is the
common terminology), we really mean `names` or `identifiers`. In Python,
`variables` are name tags for values, or labelled boxes that contain a value.

So to better understand our labels and boxes: each box is a piece of space (an
address) in computer memory. Each label (variable) is a reference to such a
place.

When the number of labels on a box ("variables referencing an address") gets
down to zero, then the data in the box cannot be found any more.

After a while, the language's "garbage collector" will wander by, notice a box
with no labels, and throw the data away, making that box available for more
data. This is the same in other garbage collected languages like Java or C#.
However, the use of garbage collection means that these languages can be slower,
and are certainly more unpredictable in terms of performance than non-garbage
collected languages like C, C++, Fortran or Rust. In these langages memory
address with no references to it still takes up memory, and the computer can
more easily run out.

So in Python, when I write:

~~~python
number = 1
number = 2
~~~

The following things happen:

1. A new integer object '1' is created, and an address in memory is found for it.
1. The variable "number" will refer to that address.
1. A new integer object '2' is created, and a different address in memory is found for it.
1. The variable "number" is moved to refer to that different address.
1. The old address, containing '1', now has no labels.
1. The garbage collector frees the memory at the old address.


## Objects and Types

An object, like `number`, has a type. We can use `type()` to tell us the type of the variable. For our variable above:

~~~python
type(number)
~~~

Note we don't need to use `print` - the Python interpreter will just output the result:

~~~
<class 'int'>
~~~

Depending on its type, an object can have different properties: data fields *inside* the object.

Consider a Python complex number for example, which Python supports natively:

~~~python
z = 3+1j
~~~

We can see what properties and methods an object has available using the dir function:

~~~python
dir(z)
~~~

~~~
['__abs__'
 '__add__'
 '__bool__'
 '__class__'
 '__delattr__'
 '__dir__'
 '__divmod__'
 '__doc__'
 '__eq__'
 '__float__'
 '__floordiv__'
 '__format__'
 '__ge__'
 '__getattribute__'
 '__getnewargs__'
 '__gt__'
 '__hash__'
 '__init__'
 '__init_subclass__'
 '__int__'
 '__le__'
 '__lt__'
 '__mod__'
 '__mul__'
 '__ne__'
 '__neg__'
 '__new__'
 '__pos__'
 '__pow__'
 '__radd__'
 '__rdivmod__'
 '__reduce__'
 '__reduce_ex__'
 '__repr__'
 '__rfloordiv__'
 '__rmod__'
 '__rmul__'
 '__rpow__'
 '__rsub__'
 '__rtruediv__'
 '__setattr__'
 '__sizeof__'
 '__str__'
 '__sub__'
 '__subclasshook__'
 '__truediv__'
 'conjugate'
 'imag'
 'real']
 ~~~

You can see that there are several methods whose name starts and ends with `__`
(e.g. `__init__`): these are special methods that Python uses internally, and
most of the time, we don't need to concern ourselves with them. We will discuss
some of them later on in this course as they become useful. The others (in this
case, `conjugate`, `img` and `real`) are the methods and fields through which we
can interact with this object.

~~~python
type(z)
~~~

~~~
<class 'complex'>
~~~

~~~python
z.real
~~~

~~~
3.0
~~~

~~~python
z.imag
~~~

~~~
1.0
~~~

A property of an object is accessed with a dot. The jargon is that the "dot operator" is used to obtain a property of an object.

## Other Basic Python Data Types

Since we're not declaring the type of a variable, how does it work it out?

Python is an interpreted language that is *dynamically typed*, which means the
type of a variable is determined and *bound* to the variable at runtime from its
given value. So when we assign a floating point number, for example, it's type
is inferred:

### Floats

~~~python
weight_kg = 55
weight_lb = 2.2 * weight_kg
print('Weight in lb', weight_lb)
~~~

Note we can add as many things that we want to `print` by separating them with a comma.

For a float, a number after a point is optional. But the *dot* makes it a float.

~~~
Weight in lb 121.00000000000001
~~~

So the thing with floats is that they are *representation* of a real number.
Representing a third or the root of 2 would be impossible for a computer, so
these are really approximations of real numbers using an ubiquitous standard
([IEEE-754](https://docs.python.org/3/tutorial/floatingpoint.html#representation-error)).

As the example above shows, we can print several things at once by separating
them with commas.

An important thing to remember, particularly in numerical analyses, is that a `float` in Python is double precision.

:::challenge{title="What's inside the box?" id=inside_box}

Draw diagrams showing what variables refer to what values after each statement
in the following program:

~~~
weight = 70.5
age = 35
weight = weight * 1.14
age = age + 20
~~~

:::

### Strings

Note that before, we also used a `string` in our use of `print`. In Python, we can use either single quotes or double quotes, or even both if we need to include quotes within a string, e.g.:

~~~python
given = 'Joe'
middle = "Frederick"
family = "'Bloggs'"
full = given + " " + middle + " " + family
print(full)
~~~

Here we use the `+` operator to concatenate strings together.

~~~
Joe Frederick 'Bloggs'
~~~

With quotes, the main thing is to be consistent in how you use them (i.e. not like we've used them above!).

We've looked at properties on objects. But many objects can also have *methods* (types of functions) associated with them, which we can use to perform operations on the object.

For strings, we also can do things like:

~~~python
given.upper()
~~~

Which returns the upper case version of the string.

~~~
'JOE'
~~~

Note it isn't changing `given`'s string itself, it's returning a new string in uppercase.

There are other methods we can use on strings, such as:

~~~python
'    Hello'.strip()
~~~

~~~
'Hello'
~~~

We'll be looking at classes and objects in more detail later today.

### Booleans

We can use boolean variables to capture `True` or `False`, useful in conditionals and loops, e.g.:

~~~python
is_joe = (given == 'Joe')
flag = False
print(is_joe, flag)
~~~

~~~
True False
~~~

### No Value?

We can also assign variable with no value:

~~~python
nothing = None
print(nothing)
~~~

~~~
None
~~~

`None` is the special Python value for a no-value variable.

If that's the output, what's the type of `nothing`?

~~~python
type(nothing)
~~~

~~~
<class 'NoneType'>
~~~


### Converting Between Types

With floats, ints and strings, we can use in-built functions to convert between types:

~~~python
age, house_number = 30, '76'
print(str(age), float(age), int(house_number), float(house_number))
~~~

~~~
30 30.0 76 76.0
~~~


## Key Points:
- Python is an interpreted, dynamically typed language.
- Run the interpreter from the command line by typing `python`.
- Use `variable = value` to assign a value to a variable in order to record it in memory.
- Variables are created on demand whenever a value is assigned to them.
- Use `print(something)` to display the value of `something`.
- `None` as an empty variable value has its own type.
- Convert a variable to another type by using `new_type_name(variable)`.