---
name: Variables
dependsOn: [technology_and_tooling.bash_shell, technology_and_tooling.ide.cpp]
tags: [cpp]
learningOutcomes:
  - Understand static typing, and the core variable types.
  - Understand the concepts of code blocks and variable scope.
  - Know how to use references and convert between types.
attribution:
  - citation: >
      This material was adapted from an "Introduction to C++" course developed by the
      Oxford RSE group.
    url: https://www.rse.ox.ac.uk
    image: https://www.rse.ox.ac.uk/sites/default/files/styles/listing_tile_text_displayed_image/public/rse/images/media/oxrse_banner_2.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Getting started

Create a new folder and open it in VSCode:

```bash
mkdir procedural
cd procedual
code .
```

### Compiling and Running a C++ executable

Ensure that you have the `clang` cpp compiler installed using:

```bash
clang++ --version
```

You should see something like:

```text
Homebrew clang version 15.0.3
Target: x86_64-apple-darwin22.1.0
Thread model: posix
InstalledDir: /usr/local/opt/llvm/bin
```

Check where the compiler executable is located on your machine

```bash
which clang++
```

You should see something like:

```text
/usr/local/opt/llvm/bin/clang++
```

Create a new file `prodedural.cpp` and copy in the following contents:

```cpp
#include <iostream>

int main() {
    std::cout << "hello world" << std::endl;
}
```

Open the command palette and choose "C/C++ Run C/C++ File", or click on the play
button in the top right hand corner of the screen. Choose the clang compiler
located earlier, and it will create the file `.vscode/tasks.json` with the
configuration used to build and run the currently active file

The file should compile successfully and output the text "hello world" in the
debug console.

## Static Typing

Variables in C++ must be declared with their type, before they are used, for example:

```cpp
int six = 2 * 3;
std::cout << "six = " << six << std::endl;
```

```text
six = 6
```

The compiler must be able to determine the type of every variable when it
compiles your code. This is a characteristic of _statically typed_ languages. It
uses this type when working out how much memory to allocate for the value (in
this case the number 6), and to determine the format for _how_ this value is
stored in memory. For example, consulting a reference webpage like
[this](https://en.cppreference.com/w/cpp/language/types) tells us that the `int`
type is at least 16 bits in size, each bit can be either 0 or 1, and so the
value 6 will be stored in these 16 bits as the binary number 0000000000000110.
In this case `int` is a _signed_ type, and the first bit is 0 for positive, and
1 for negative values, so -6 will be represented as 1000000000000110.

So the _type_ of a variable tells the compiler how to interpret the data stored
in memory to arrive at a value for our variable `six`. Simliarly, it tells the
compiler what format to _write_ to memory when the value of the variable is
changed.

If we try to use a variable that hasn't been defined, we get a compiler error:

```cpp
int seven = sixe + 1;
```

````text
/Users/martinjrobins/git/thing/procedural.cpp:7:17: error: use of undeclared identifier 'sixe'; did you mean 'six'?
    int seven = sixe + 1;
                ^```
                six
/Users/martinjrobins/git/thing/procedural.cpp:5:9: note: 'six' declared here
    int six = 2 * 3;
        ^
1 error generated.
````

Note here we accidentally wrote `sixe` instead of `six`, so the compiler
recognised this as an _undeclared identifier_ and gave an error. It even
helpfully identified a variable with a similar name that you might have meant.
This highlights one of the advantages of a compiler, it can catch a range of
errors before you even run your program, which can be very useful if you have a
large project that takes a significant time to run.

If we know that a variable will be constant, we can indicate this using the `const` keyword, like so:

```cpp
const int six = 2 * 3;
```

This has the advantage that if we try and modify `six` later on, the compiler will inform us of our error:

```cpp
const int six = 2 * 3;
six = 7;
```

````text
/Users/martinjrobins/git/thing/procedural.cpp:8:9: error: cannot assign to variable 'six' with const-qualified type 'const int'
    six = 7;
    ``` ^
````

The compiler has saved us again! You can assist the compiler (and perhaps more
importantly, other readers of your code!) by always marking variables that you
expect to the constant with `const`.

### Blocks and scope

A C++ program is made up of many _blocks_ which are delimited by curly brackets.
As an example, lets define a `main` function with a `for` loop inside. The curly
brackets after the `main` function delimite an outer block, whereas the curly
brackets after the `for` loop delimite an inner block.

```cpp
int main() {
  const int two = 2;
  for (int i = 0; i < 10; i++) {
    const int x = two * i;
    std::cout << i << x << std::endl;
  }
  // i and x no longer valid here
}
```

Each variable has a particular block _scope_ where it is valid, which starts
where the variable is declared and covers to the end of the current block,
including any inner blocks. In the example above, the `two` variable is in
scope until the end of the main function. However, the `i` and `x` variables
are only in scope until the end of the `for` loop.

::::challenge{id=scope title="Largest Circumference"}

Below is a code snippet calculating the circumference for a series of
ever-larger circles. At the end of the loop we wish to print out the largest
circumference, but unfortunately our program won't compile.

Fix the code so that it compiles and the largest circumference is printed.
Ensure that all constant variables are explicitly marked `const`.

```cpp
int sum = 0;
int pi = 3.14;
for (int i = 0; i < 10; i++) {
    double radius = static_cast<double>(i);
    double circumference  = 2 * pi * radius;
}
std::cout << "largest circumference is " << circumference << std::endl;
```

:::callout
In this snippet we _cast_ the integer `i` to the floating point number `radius`,
this is a way of converting between different types in C++. You will learn more
about type conversions later in this section
:::

:::solution

The simplest solution is to simple move the `circumference` variable to the outer scope, and correcting the type of `pi`.

```cpp
int sum = 0;
const double pi = 3.14;
double circumference  = 0;
for (int i = 0; i < 10; i++) {
    const double radius = static_cast<double>(i);
    circumference  = 2 * pi * radius;
}
std::cout << "largest circumference is " << circumference << std::endl;
```

:::

However, this code assumes that the largest circumference is computed last. You
might later on decide to alter the loop so this is no longer the case, so it
would be safer to not to make this assumption.

```cpp
int sum = 0;
const double pi = 3.14;
double largest_circumference = 0;
for (int i = 0; i < 10; i++) {
    const double radius = static_cast<double>(i);
    const double circumference  = 2 * pi * radius;
    if (circumference > largest_circumference) {
        largest_circumference = circumference;
    }
}
std::cout << "largest circumference is " << largest_circumference << std::endl;
```

::::

### Floating point numbers

Lets declare a floating point number in C++:

```cpp
const float weight_kg = 55.0;
const float weight_lb = 2.2 * weight_kg;
std::cout << "Weight in lb " << weight_lb << std::endl;
```

The useful resource
[cppreference](https://en.cppreference.com/w/cpp/language/types) tells us that
the type `float` in C++ is stored in 32 bits, and matches the [IEEE-754 binary32
format](https://en.wikipedia.org/wiki/Single-precision_floating-point_format).
If you follow this link to the wikipedia article, you can see that this format
is very different to the format used to store an `int`.

Note that literals like `55.0` also have a type in C++, and it is a good rule of
thumb to be consistent with your types. Writing something like `float weight_kg
= 55` is actually assigning an integer to a float and involves an _implicit_
type conversion. In this case there is no harm done, but implicit type
conversions are a source of bugs and should be avoided.

Note that you can, and should, be even more explicit in type of literal you are using and
specify it using a
[suffix](https://en.cppreference.com/w/cpp/language/floating_literal):

```cpp
const float weight_kg = 55.0f;
const float weight_lb = 2.2f * weight_kg;
std::cout << "Weight in lb " << weight_lb << std::endl;
```

Now we have specified that all the literals are `float` (as opposed to
`double`, which is a 64 bit floating point type in C++). Writing the same code
using `double` looks like this:

```cpp
const double weight_kg = 55.0;
const double weight_lb = 2.2 * weight_kg;
std::cout << "Weight in lb " << weight_lb << std::endl;
```

## Strings

Thus far we have been using only the _fundamental types_ in C++, and have only touched on a couple of these, [you can see a more thorough list here](https://en.cppreference.com/w/cpp/language/types).

Like in many languages, C++ allows you to define _classes_.
These are user-defined types that contain data in the form of _member variables_ (inPython we call these "properties" or "attributes"), and functions that operate on that data in the form of _member functions_ (in Python these are called "methods").
This follows a different programming paradigm known as _object-orientated_ programming, or OO.
We won't cover OO much in this course, but since many useful standard types in C++ are defined as classes in the C++ standard library you need to be aware how to use classes in C++.
If you wish to learn more about this paradigm; [we have a full course on using object-orientated programming in C++ here](../object_orientated/)

A string in C++ typically uses the `std::string` class. Here `std::string`
indicates the `string` class that is defined in the `std` namespace. Namespaces
are a way to group together a number of related functions, types and classes in
such a way that their names do not conflict with functions/types/classes in
other namespaces. E.g. `std::string` refers to a different class than
`my_namespace::string`. The `std::string` class is defined in the `string`
header file, so before we use it we need to _include_ this header in our source
file (at the top of the file above our `main` function):

```cpp
#include <iostream>
#include <string>

int main() {
    std::string given = "Joe";
    std::string middle = "Frederick";
    std::string family = "\'Bloggs\'";
    std::string full = given + " " + middle + " " + family;
    std::cout << full << std::endl;
}
```

```text
Joe Frederick 'Bloggs'
```

As with strings in Python, we can use the `+` operator to concatenate two C++
strings together. However, we can only use double quotes for strings in C++, as
single quotes are reserved for characters. To include the single quotes in our
string, we use the backslash to _escape_ the normal meaning of the single quote
character.

## References

Previously we explained that variables in Python are only labels for a "box", or
section of memory, that holds a value. You can therefore have multiple labels
for the same box. In C++ a variables is the box itself, each variables is
assigned a given section of memory where the value is stored according to its
type. You can obtain the address, or _pointer_, to the start of this section of
memory by using the _address-of_ operator `&`. We can obtain the original
variable by using the _dereferencing_ operator `*`.

```cpp
int *p_six = &six;
std::cout << "six = *p_six" << std::endl;
```

Note that the type of `p_six` is a pointer to an `int`, denoted as `int *`. On a
64-bit operating system, all pointer types are stored in 64 bits of memory. You
might think that this pointer is similar to a label, or Python variable, but a
raw pointer like this is much more low-level and potentially dangerous than
this, and you should generally avoid creating them. Instead, C++ features a wide
variety of [_smart
pointers_](https://en.cppreference.com/book/intro/smart_pointers) that you
should use instead (a variable in Python is most closely related to a
`std::shared_pointer`). However, all of these pointers share simliar semantics
in that a pointer can point to nothing, otherwise known as a _null pointer_ and
represented in C++ by the literal `nullptr`.

Generally, you should not use pointers unless you need to worry about allocating
memory (e.g. you are writing a custom data structure). A more useful
label-to-a-box in C++ that must point to a valid memory location is provided by
a _reference_. This is similar to a pointer in that it is a label to a box
(rather than _being_ that box), and therefore you can have multiple references
to the same box. But you can use it in the same way you would the original
variable. The type of a reference to an `int` is given by `int &`, so:

```cpp
int &r_number = six;
int &r_number2 = six;
std::cout << "six = " << r_number << std::endl;
r_number += 1;
std::cout << "seven = " << r_number << std::endl;
std::cout << "seven = " << r_number2 << std::endl;
std::cout << "seven = " << six << std::endl;
```

```text
six = 6
seven = 7
seven = 7
seven = 7
```

::::challenge{id=pointers title="Getting used references"}

Declare a `double` called `d` with the value `5.0`. Create a reference to this
double `r_d` and assign `6.0` to this reference. Verify that `d` has changed
value by printing it out.

Try and mark `d` as `const` and see what the compiler tells you.

:::solution

```cpp
double d = 5.0;
double& r_d = d;
r_d = 6.0
std::cout << d << std::endl;
```

:::

::::

There are two types of references in C++, _lvalue_ and _rvalue_ references.
Above we have used lvalue references, which are so-called because they can only
be _bound_ to an lvalue, such as a variable or element of a container (i.e.
anything
that you could put on the left hand side of an assignment `=`
statement). An lvalue reference is declared using a single `&`.

```cpp
int six = 6;
int& r_six = six;
```

An rvalue reference is more general in that it can also be bound to temporaries,
or rvalues. An rvalue could be a literal like `6` or the result of an expression
like `a + b` (i.e. something that you might see on the right hand side of an
assignment `=` statement). An rvalue reference is declared using two ampersands `&&`.

```cpp
int&& rr_six = 6;
```

Rvalue references are commonly used to enable _move semantics_ in C++. You
often want to write code that minimises the number of copies, for example say
you wished to swap the values of two (large) strings `war_and_peace` and `moby_dick`.

```cpp
T tmp(war_and_peace);
war_and_peace = moby_dick;
moby_dick = tmp;
```

Here we have done three copies of what could be very large strings. We can use
the `std::move` function to do this more efficiently by changing the lvalue
references to rvalue references.

```cpp
T tmp(std::move(war_and_peace));
war_and_peace = std::move(moby_dick);
moby_dick = std::move(tmp);
```

The `std::move` function allows us to transfer the value of variable `a` to
variable `b`, without the requirement of maintaining the value of `a`. Note that
after we have moved `a` its value is now unspecified, so after the last
statement in the snippet above, the value of `tmp` will be unspecified.

### Optional types

What if you wish to represent a variable that could be a value or could be
_nothing_? For example perhaps you have a function that takes as argument a
string and finds the first word beginning with the letter `a`. What should it
return if the string does not contain `a`?

Other languages have an inbuilt concept of nothing, for example the `None` value
in Python. In C++ you can instead define an _optional_ type that could have a value, or not. If we have a function `find_first_word_starting_with_a` that returns an optional string type we can use it like so:

```cpp
const std::optional<std::string> word = find_first_word_starting_with_a(my_string);
if (word) {
    std::cout << "found the word " << *word std::endl;
} else {
    std::cout << "did not find a word" << std::endl;
}
```

Th angle brackets indicate that `std::optional` is a templated class. The first
and only _template argument_ is `std::string`, meaning that
`std::optional<std::string>` is an optional `std::string` type. Templates in C++
enable yet another programming paradigm called
_generic programming_. As you can see, there are a number of different
programming paradigms, all with their own uses! It is best to be aware of and
use all of them interchangably for the problem at hand or the idea you wish to
express.

### Converting Between Types with C++

Conversion between types in C++ can occur implicitly, or explicitly. You should
always strive to be as explicit as possible in any code you write. Code is meant
to be read, and the most important reader is another human (a computer probably
would prefer it if you just wrote assembly!), so be as clear and as explicit as
you can when you write your code.

Unfortunately, implicit conversions can occur quite easily in C++, and this is a
source of many bugs. For example:

```cpp
const double x = std::atan(1.0d) * 4.0d;
const float y = x;
if (y == x) {
    std::cout << "x = y" << std::endl;
} else {
    std::cout << "x != y" << std::endl;
}
```

In this case the value of x, represented by a `double` type, is close to the mathematical constant pi.
The variable `y` is of type `float`, and so the assignment statement rounds the value held in `x` to the nearest available value that can be represented by a `float`, according to the [rules dictated on the cpp reference page for implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion).
Since the value now in `y` is different to the value in `x`, the result is:

```text
x != y
```

Let's instead write the conversion between `double` and `float` explicitly using `static_cast`.

```cpp
const double x = std::atan(1.0d) * 4.0d;
const float y = static_cast<float>(x);
if (y == x) {
    std::cout << "x = y" << std::endl;
} else {
    std::cout << "x != y" << std::endl;
}
```

Not only have we highlighted that a conversion occurs, the very act of writing
the `static_cast` has forced us to think about the conversion and its
implications.

Here is another example of a potential bug (and use-case for `static_cast`):

```cpp
const int n = 100;
std::vector<double> vec(n, 1.0);
const double rn = 1 / n;
double mean = 0.0;
for (size_t i = 0; i < n; i++) {
    mean += rn * vec[i];
}
std::cout << "mean is " << mean << std::endl;
```

Here we are creating a vector of `double` with all the elements initialised to
1.0. This program outputs:

```text
mean is 0
```

What has happened? In this case, we have heard somewhere that multiplications
are cheaper to compute than division, so we have attempted to optimise the loop
by replacing the `1/n` term with a precomputed value. However, in doing so we
have introduced two mistakes on the line `const double rn = 1 / n;`. Both the
'1' and `n` here should be of type `double`, not `int`, so that the division is
a floating-point division rather than integer division. Instead this should be:

```cpp
const double rn = 1.0 / static_cast<double>(n);
```

to get the behaviour we are expecting.

Recall, these are fairly tame example using fundamental types. However, once
you start creating your own types via classes the opportunities for implicit
conversions to introduce subtle bugs increases exponentially, so a good rule of
thumb is to discourage implicit casts and to _always_ be explicit.

::::challenge{id=cpp_calculate_pi title="Calculating PI"}

Create two `double` variables $x$ and $y$. Set $x=0.3$ and $y=0.4$ and
calculate $r = \sqrt{x^2 + y^2}$. Write the result $r$ to the console using
`std::cout`. Note that C++ has library functions `std::sqrt` and `std::pow` for
square root and power. You can see the [cpp-reference page for `std::sqrt`](https://en.cppreference.com/w/cpp/numeric/math/sqrt).

:::solution

```cpp
const double x = 0.3;
const double y = 0.4;

const double r = std::sqrt(std::pow(x,2) + std::pow(y,2));

std::cout << "r = "<< r << std::endl;
```

:::

Generate $N$ uniform random numbers $x_i$ and $y_i$ between -1 and 1 using
`std::uniform_real_distribution` ([see example of use here](https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution)).
Count the number of points where $\sqrt{x_i^2 + y_i^2} < 1$, and use this to estimate the value of $\pi$.

:::solution

```cpp
std::default_random_engine generator;
std::uniform_real_distribution<double> uniform(-1.0,1.0);
const int N = 1e6;
int count = 0;
for (int i = 0; i < N; ++i) {
  const double x = uniform(generator);
  const double y = uniform(generator);
  const double r2 = std::pow(x,2) + std::pow(y,2);

  if (r2 < 1.0) {
    ++count;
  }
}

std::cout << "pi is about "<< 4.0*static_cast<double>(count)/static_cast<double>(N) << std::endl;
```

:::

Code up another estimator for $\pi$ by calculating the sum of the reciprocals
of square numbers (The Basel problem) for $N$ terms, which converges to
$\pi^2/6$ for large enough $N$.

$$
S = \sum_{n=1}^{n=N} \frac{1}{n^2} \rightarrow \frac{\pi^2}{6}
$$

:::solution

```cpp
const int N = 1000;
double sum = 0.0;
for (int i = 1; i < N; ++i) {
  sum += 1.0/static_cast<double>(std::pow(i,2));
}
std::cout << "pi is about "<< std::sqrt(6.0*sum) << std::endl;
```

:::

::::
