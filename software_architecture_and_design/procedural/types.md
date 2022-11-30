---
name: Types
---

## Getting started

Create a new folder and open it in VSCode:

```bash
mkdir procedural
cd procedual
code .
```

### Running a Python script


In the terminal start a new Python virtual environment

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


### Compiling and Running a C++ executable (optional)

Ensure that you have the `clang` cpp compiler installed using:

~~~bash
clang++ --version
~~~

You should see something like:

~~~
Homebrew clang version 15.0.3
Target: x86_64-apple-darwin22.1.0
Thread model: posix
InstalledDir: /usr/local/opt/llvm/bin
~~~

Check where the compiler executable is located on your machine

~~~bash
which clang++
~~~

You should see something like:

~~~
/usr/local/opt/llvm/bin/clang++
~~~

Create a new file `prodedural.cpp` and copy in the following contents:

~~~cpp
#include <iostream>

int main() {
    std::cout << "hello world" << std::endl;
}
~~~

Open the command palette and choose "C/C++ Run C/C++ File", or click on the play
button in the top right hand corner of the screen. Choose the clang compiler
located earlier, and it will create the file `.vscode/tasks.json` with the
configuration used to build and run the currently active file

The file should compile successfully and output the text "hello world" in the
debug console.

## Variables

Let's take a look at how variables are handled within Python. Compared to some
other languages such as C++ or JavaScript, variables in Python do not need to be
declared before assigning something to them. for example:

```python
six = 2 * 3
print(six)
```

Note that in terms of naming variables, Python's variables must begin with a letter.

~~~
6
~~~

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

> ## Sorting out references
>
> What does the following program print out?
>
> ~~~
> first, second = 1, 2
> third, fourth = second, first
> print(third, fourth)
> ~~~
> {: .language-python}
>
> > ## Solution
> > ~~~
> > 2 1
> > ~~~
> > {: .output}
> {: .solution}
{: .challenge}

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

> ## What's inside the box?
>
> Draw diagrams showing what variables refer to what values after each statement
> in the following program:
>
> ~~~
> weight = 70.5
> age = 35
> weight = weight * 1.14
> age = age + 20
> ~~~
{: .challenge}

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


## Variables in C++ (optional)

Variables in C++ must be declared before they are used, along with their type, for example:

~~~cpp
int six = 2 * 3;
std::cout << "six = " << six << std::endl;
~~~

~~~
six = 6
~~~

The compiler must be able to determine the type of every variable when it
compiles your code. This is a characteristic of *statically typed* languages. It
uses this type when working out how much memory to allocate for the value (in
this case the number 6), and to determine the format for *how* this value is
stored in memory. For example, consulting a reference webpage like
[this](https://en.cppreference.com/w/cpp/language/types) tells us that the `int`
type is at least 16 bits in size, each bit can be either 0 or 1, and so the
value 6 will be stored in these 16 bits as the binary number 0000000000000110.
In this case `int` is a *signed* type, and the first bit is 0 for positive, and
1 for negative values, so -6 will be represented as 1000000000000110.

So the *type* of a variable tells the compiler how to interpret the data stored
in memory to arrive at a value for our variable `six`. Simliarly, it tells the
compiler what format to *write* to memory when the value of the variable is
changed.

If we try to use a variable that hasn't been defined, we get a compiler error:

~~~cpp
int seven = sixe + 1;
~~~

~~~
/Users/martinjrobins/git/thing/procedural.cpp:7:17: error: use of undeclared identifier 'sixe'; did you mean 'six'?
    int seven = sixe + 1;
                ^~~~
                six
/Users/martinjrobins/git/thing/procedural.cpp:5:9: note: 'six' declared here
    int six = 2 * 3;
        ^
1 error generated.
~~~

Note here we accidentally wrote `sixe` instead of `six`, so the compiler
recognised this as an *undeclared identifier* and gave an error. It even
helpfully identified a variable with a similar name that you might have meant.
This highlights one of the advantages of a compiler, it can catch a range of
errors before you even run your program, which can be very useful if you have a
large project that takes a significant time to run.

If we know that a variable will be constant, we can indicate this using the `const` keyword, like so:

~~~cpp
const int six = 2 * 3;
~~~

This has the advantage that if we try and modify `six` later on, the compiler will inform us of our error:

~~~cpp
const int six = 2 * 3;
six = 7;
~~~

```
/Users/martinjrobins/git/thing/procedural.cpp:8:9: error: cannot assign to variable 'six' with const-qualified type 'const int'
    six = 7;
    ~~~ ^
```

The compiler has saved us again! You can assist the compiler (and perhaps more
importantly, other readers of your code!) by always marking variables that you
expect to the constant with `const`. Other languages like Rust encourage this by
making variables constant by default and having special syntax for non-constant,
aka *mutable*, variables.

## References and Pointers in C++ (optional)

Previously we explained that variables in Python are only labels for a "box", or
section of memory, that holds a value. You can therefore have multiple labels
for the same box. In C++ a variables is the box itself, each variables is
assigned a given section of memory where the value is stored according to its
type. You can obtain the address, or *pointer*, to the start of this section of
memory by using the *address-of* operator `&`. We can obtain the original
variable by using the *dereferencing* operator `*`.

~~~cpp
int *p_six = &six;
std::cout << "six = *p_six" << std::endl;
~~~

Note that the type of `p_six` is a pointer to an `int`, denoted as `int *`. On a
64-bit operating system, all pointer types are stored in 64 bits of memory. You
might think that this pointer is similar to a label, or Python variable, but a
raw pointer like this is much more low-level and potentially dangerous than
this, and you should generally avoid creating them. Instead, C++ features a wide
variety of [*smart
pointers*](https://en.cppreference.com/book/intro/smart_pointers) that you
should use instead (a variable in Python is most closely related to a
`std::shared_pointer`). However, all of these pointers share simliar semantics
in that a pointer can point to nothing, otherwise known as a *null pointer* and
represented in C++ by the literal `nullptr`. 

A more useful label-to-a-box in C++ that must point to a valid memory location
is provided by a *reference*. This is similar to a pointer in that it is a label
to a box (rather than *being* that box), and therefore you can have multiple
references to the same box. But you can use it in the same way you would the
original variable. The type of a reference to an `int` is given by `int &`, so:

~~~cpp
int &r_number = six;
int &r_number2 = six;
std::cout << "six = " << r_number << std::endl;
r_number += 1;
std::cout << "seven = " << r_number2 << std::endl;
~~~

~~~
six = 6
seven = 7
~~~

### References

> ## What's inside the box?
>
> Draw diagrams showing what variables refer to what values after each statement
> in the following program:
>
> ~~~
> weight = 70.5
> age = 35
> weight = weight * 1.14
> age = age + 20
> ~~~
{: .challenge}

### Floats in C++ (optional)

Lets declare a floating point number in C++:

~~~cpp
float weight_kg = 55.0;
float weight_lb = 2.2 * weight_kg;
std::cout << "Weight in lb " << weight_lb << std::endl;
~~~

The useful resource
[cppreference](https://en.cppreference.com/w/cpp/language/types) tells us that
the type `float` in C++ is stored in 32 bits, and matches the [IEEE-754 binary32
format](https://en.wikipedia.org/wiki/Single-precision_floating-point_format).
If you follow this link to the wikipedia article, you can see that this format
is very different to the format used to store an `int`.

Note that literals like `55.0` also have a type in C++, and it is a good rule of
thumb to be consistent with your types. Writing something like `float weight_kg
= 55` is actually assigning an integer to a float and involves an *implicit*
type conversion. In this case there is no harm done, but implicit type
conversions are a source of bugs and should be avoided.

Note that you can, and should, be even more explicit in type of literal you are using and
specify it using a
[suffix](https://en.cppreference.com/w/cpp/language/floating_literal):

~~~cpp
float weight_kg = 55.0f;
float weight_lb = 2.2f * weight_kg;
std::cout << "Weight in lb " << weight_lb << std::endl;
~~~

Now we have specified that all the literals are `float` (as opposed to `double`,
which is a 64 bit floating point type in C++).

### Strings in C++

Thus far we have been using only the *fundamental types* in C++. And we've only
touched on a couple of these, you can see a more thoughor list
[here](https://en.cppreference.com/w/cpp/language/types).

Like in many languages, C++ allows you to define *classes*. These are
user-defined types that contain data in the form of *member variables* (in
Python we call these "properties" or "attributes"), and functions that operate
on that data in the form of *member functions* (in Python these are called
"methods"). This follows a different programming paradigm known as
*object-orientated* programming, or OO. We won't cover OO much in this course,
but since many useful standard types in C++ are defined as classes in the C++
standard library you need to be aware how to use classes in C++.

A string in C++ typically uses the `std::string` class. Here `std::string`
indicates the `string` class that is defined in the `std` namespace. Namespaces
are a way to group together a number of related functions, types and classes in
such a way that their names do not conflict with functions/types/classes in
other namespaces. E.g. `std::string` refers to a different class than
`my_namespace::string`. The `std::string` class is defined in the `string`
header file, so before we use it we need to *include* this header in our source
file (at the top of the file above our `main` function):

~~~cpp
#include <string>

int main() {
    std::string given = "Joe";
    std::string middle = "Frederick";
    std::string family = "\'Bloggs\'";
    full = given + " " + middle + " " + family;
    std::cout << full << std::endl;
}
~~~

~~~
Joe Frederick 'Bloggs'
~~~


As with strings in Python, we can use the `+` operator to concatenate two C++
strings together. However, we can only use double quotes for strings in C++, as
single quotes are reserved for characters. To include the single quotes in our
string, we use the backslash to *escape* the normal meaning of the single quote
character.

### No Value?

Nothing is a complicated concept, and each language deals with no value, or a
null value, in its own way. In C++ you can define a variable without
initialising it, like so:

~~~cpp
int something;
std::cout << something << std::endl;
~~~

Here we have used `something` without initialising to some value. Something will
be printed out, but we don't know what. One option is that the compiler will
allocate `something` a certain section of memory, and then print out whatever is
at that location. Another option is that the compiler will optimise out
`something` altogether and just print out 0, or something of its own choosing.

This is an example of [*undefined
behaviour*](https://en.cppreference.com/w/cpp/language/ub). This covers all
situations where the programmer violated certain rules of the C++ language, and
in these cases it is up to each individual compiler what actually occurs during
compilation and execution of the program. For another, more interesting example,
we can use one of those given in the previous cppreference page:

~~~cpp
bool p; // uninitialized local variable
if(p) // UB access to uninitialized scalar
    std::puts("p is true");
if(!p) // UB access to uninitialized scalar
    std::puts("p is false");
~~~

Here `p` is a boolean variable (`true` or `false`), and for a certain older version of gcc the output is:

~~~
p is true
p is false
~~~

What if we try to create an uninitialised reference?

~~~cpp
int& something;
std::cout << something << std::endl;
~~~

In this case we get the following error:

~~~
/Users/martinjrobins/git/thing/procedural.cpp:27:10: error: declaration of reference variable 'something' requires an initializer
    int& something;
~~~

So no luck here creating nothing! In C++ you could represent no value, or nothing, by a null pointer like so:

~~~cpp
std::unique_ptr<int> nothing = nullptr;
if (nothing) {
    std::cout << *nothing << std::endl;
} else {
    std::cout << "None" << std::endl;
}
~~~

Here we use one of the smart pointers we mentioned earlier, a unique pointer, to
emphasise our earlier message to not use a raw pointer if you can avoid it. The
angle brackets indicate a templated class, in this case a unique pointer to an
`int`. Templates in C++ enable yet another programming paradigm called
*generic programming*. As you can see, there are a number of different
programming paradigms, all with their own uses! It is best to be aware of and
use all of them interchangably for the problem at hand or the idea you wish to
express. 

Going back to the example, note that we have initialised our variable, so there
is no undefined behaviour and we can be assured of the same behaviour over all
compilers. It should be noted, however, that in this case the `std::unique_ptr`
will be default initialised even if we don't initialise it, but it is still
better to write the explicit initialisation to be clear. Note that we need to
use two *different* print statements to print either the `int` or the string
literal "None", since these are two different types. Recall, in C++ all types
must be known at compile time, for each variable and also for each expression or
statement in your program.

The output is:

~~~
None
~~~

Another way to create nothing in C++ (since C++17) is to use `std::optional`:

~~~cpp
std::optional<int> nothing = std::nullopt; 
if (nothing) {
    std::cout << *nothing << std::endl;
} else {
    std::cout << "None" << std::endl;
}
~~~

~~~
None
~~~


### Converting Between Types with C++


Conversion between types in C++ can occur implicitly, or explicitly. You should
always strive to be as explicit as possible in any code you write. Code is meant
to be read, and the most important reader is another human (a computer probably
would prefer it if you just wrote assembly!), so be as clear and as explicit as
you can when you write your code. 

Unfortunately, implicit conversions can occur quite easily in C++, and this is a
source of many bugs. For example:

~~~cpp
const double x = std::atan(1.0d) * 4.0d;
const float y = x;
if (y == x) {
    std::cout << "x = y" << std::endl;
} else {
    std::cout << "x != y" << std::endl;
}
~~~

In this case the value of x, represented by a `double` type, is close to the
mathematical constant pi. The variable `y` is of type `float`, and so the assignment
statement rounds the value held in `x` to the nearest available value that can
be represented by a `float`, according to the rules dictated
[here](https://en.cppreference.com/w/cpp/language/implicit_conversion). Since
the value now in `y` is different to the value in `x`, the result is:

~~~
x != y
~~~

Let's instead write the conversion between `double` and `float` explicitly using `static_cast`.

~~~cpp
const double x = std::atan(1.0d) * 4.0d;
const float y = static_cast<float>(x);
if (y == x) {
    std::cout << "x = y" << std::endl;
} else {
    std::cout << "x != y" << std::endl;
}
~~~

Not only have we highlighted that a conversion occurs, the very act of writing
the `static_cast` has forced us to think about the conversion and its
implications. 

Here is another example of a potential bug (and use-case for `static_cast`):

~~~cpp
const int n = 100;
std::vector<double> vec(n, 1.0);
const double rn = 1 / n;
double mean = 0.0;
for (size_t i = 0; i < n; i++) {
    mean += rn * vec[i];
}
std::cout << "mean is " << mean << std::endl;
~~~

Here we are creating a vector of `double` with all the elements initialised to
1.0. This program outputs:

~~~
mean is 0
~~~

What has happened? In this case, we have heard somewhere that multiplications
are cheaper to compute than division, so we have attempted to optimise the loop
by replacing the `1/n` term with a precomputed value. However, in doing so we
have introduced two mistakes on the line `const double rn = 1 / n;`. Both the
'1' and `n` here should be of type `double`, not `int`, so that the division is
a floating-point division rather than integer division. Instead this should be:

~~~cpp
const double rn = 1.0 / static_cast<double>(n);
~~~

to get the behaviour we are expecting.

Recall, these are fairly tame example using fundamental types.  However, once
you start creating your own types via classes the opportunities for implicit
conversions to introduce subtle bugs increases exponentially, so a good rule of
thumb is to discourage implicit casts and to *always* be explicit. 
