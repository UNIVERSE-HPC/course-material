---
name: Variables
dependsOn: [
    technology_and_tooling.bash_shell.bash,
    technology_and_tooling.ide.rust,
]
tags: [rust]
---

## Cargo


Cargo is the official build system and package manager for the Rust programming
language. It serves as a central tool in the Rust development ecosystem,
managing tasks like building and testing code, downloading and incorporating libraries or
"crates", and handling dependencies. This integration of
building, dependency management, and testing through Cargo simplifies the
development process, making it more efficient and less error-prone. 

To create a new Rust project, we use the `cargo new` command. This creates a new
directory with the name of the project, and a `Cargo.toml` file that contains
the project metadata and dependencies. It also creates a `src` directory
containing a `main.rs` file, which is where we will write our code.

```bash
cargo new procedural
cd procedural
code .
```

### Compiling and Running a Rust Program

Open the `main.rs` file and copy in the following contents:

```rust
fn main() {
    println!("hello world");
}
```

To compile and run a Rust program, we use the `cargo run` command. In VSCode,
you can also click on the "Run" button above the `main` function. This will
compile the program, and then run it. If the program is already compiled, it
will skip the compilation step and just run the program.

```bash
cargo run
```

This will print out `hello world` to the terminal.

## Static Typing

Rust is a *statically typed* language, which means that the compiler must be
able to determine the type of every variable when it compiles your code. This is
in contrast to *dynamically typed* languages like Python, where the type of a
variable is determined at runtime.

Variables in Rust must be declared with their type, before they are used, for
example:

```rust
let six: i32 = 2 * 3;
println!("six = {}", six);
```

Here we have declared a variable `six` of type `i32`, which is a 32-bit signed
integer. The `let` keyword is used to declare a variable. The `print!` macro
prints out the value of the variable `six` to the terminal. Note that we have
used the `{}` syntax to indicate where the value of the variable should be
substituted into the string.

Note that we don't need to always specify the type of a variable, as the rust compiler can
infer the type from the value assigned to the variable:

```rust
let six = 2 * 3;
println!("six = {}", six);
```

If we try to use a variable that hasn't been defined, we get a compiler error:

~~~rust
let seven = sixe + 1;
~~~

~~~
error[E0425]: cannot find value `sixe` in this scope
 --> src/main.rs:5:17
  |
5 |     let seven = sixe + 1;
  |                 ^^^^ help: a local variable with a similar name exists: `six`
~~~

Note here we accidentally wrote `sixe` instead of `six`, so the compiler
recognised this as an *undeclared identifier* and gave an error. It even
helpfully identified a variable with a similar name that you might have meant.
This highlights one of the advantages of a compiler, it can catch a range of
errors before you even run your program, which can be very useful if you have a
large project that takes a significant time to run. In fact, if you are using
VSCode (or another IDE) you will typically see this error highlighted in the
editor before you even try to compile your code.

In rust, all variables are *immutable* by default. This means that once you have
assigned a value to a variable, you cannot change it. If you try to do so, you
will get a compiler error:

~~~rust
let six_or_seven = 2 * 3;
six_or_seven = 7;
~~~

```
error[E0384]: cannot assign twice to immutable variable `six`
 --> src/main.rs:4:5
  |
3 |     let six = 2 * 3;
  |         ---
  |         |
  |         first assignment to `six`
  |         help: consider making this binding mutable: `mut six`
4 |     six = 7;
  |     ^^^^^^^ cannot assign twice to immutable variable
```

The compiler has identified that we are trying to assign a new value to an immutable variable, and
has suggested that we make the variable mutable by adding the `mut` keyword:

```rust
let mut six_or_seven = 2 * 3;
six_or_seven = 7;
```

Now the code compiles and runs as expected.

### Expression, Statements & Blocks

In Rust, there are two types of code: *statements* and *expressions*. A *statement* is a piece of
code that performs some action, but does not return a value. For example, the
`let` keyword is a statement that declares a variable.

```rust
let a = 1;
```

An *expression* is a piece of code that returns a value. For example, the
`+` operator is an expression that returns the sum of two numbers.

```rust
let b = 2 + 3;
```

A *block* is a collection of statements and expressions, delimited by curly
brackets `{}`, and which returns the value of the last expression in the block. Therefore, a block
can be used anywhere an expression can be used. For example, we can use a block
as the value assigned to a variable:

```rust
let c = {
    let d = 2;
    d + 3
};
```

Here we have declared a variable `c` and assigned it the value of the block. The
second line of the block is an expression that returns the value `d + 3`. Since we have not put a
semicolon `;` at the end of this line, it is not a statement, but an expression. If we had put a
semicolon at the end of this line, it would have been a statement, and the block would have returned
the value `()` (pronounced "unit"), which is the value of an empty statement. Try this out for yourself
and see what type the variable `c` is.

### Shadowing

Rust allows you to *shadow* variables, which means that you can declare a new
variable with the same name as an existing variable. The new variable will
*shadow* the existing variable, meaning that the existing variable is no longer
accessible. For example:

```rust
let x = 1;
let x = x + 1;
```

We could have also written using a mutable variable:

```rust
let mut x = 1;
x = x + 1;
```

But shadowing allows us to update the value of a variable, and still keep the
variable immutable.

Like other languages, Rust also allows shadowing of variables within a block. However, the difference here is that the
shadowed variable is still accessible outside the block. For example:

```rust
let x = 1;
{
    let x = 2;
    println!("inner x = {}", x);
}
println!("outer x = {}", x);
```

```
inner x = 2
outer x = 1
```


::::challenge{id=scope title="Largest Circumference"}

Below is a code snippet calculating the circumference for a series of
ever-larger circles. At the end of the loop we wish to print out the largest
circumference, but unfortunately our program won't compile.

Fix the code so that it compiles and the largest circumference is printed.
Ensure that all constant variables are explicitly marked `const`.

```rust
fn main() {
    let pi = 3.14;
    for i in 0..10 {
        let radius = i / 10.0;
        let circumference = 2.0 * pi * radius;
    }
    println!("largest circumference is {}", largest_circumference);
}
```

:::callout
We have used the `..` operator to create a range of numbers from 0 to 9, and we 
use a `for` loop to iterate over this range. Note that the range is exclusive of
the last number, so the loop will iterate over the numbers 0 to 9.
:::


:::solution

The simplest solution is to simple move the `circumference` variable to the outer block.

```rust
fn main() {
    let pi = 3.14;
    let mut circumference = 0.0;
    for i in 0..10 {
        let radius = i / 10.0;
        circumference = 2.0 * pi * radius;
    }
    println!("largest circumference is {}", circumference);
}
```

However, this code assumes that the largest circumference is computed last. You
might later on decide to alter the loop so this is no longer the case, so it
would be safer to not to make this assumption.

```rust
fn main() {
    let pi = 3.14;
    let mut largest_circumference = 0.0;
    for i in 0..10 {
        let radius = i / 10.0;
        let circumference = 2.0 * pi * radius;
        if circumference > largest_circumference {
            largest_circumference = circumference;
        }
    }
    println!("largest circumference is {}", largest_circumference);
}
```
:::

::::


### Basic Types and Type conversions

Rust has a number of built-in *primitive* types, including integers, floating
point numbers, booleans, characters, and tuples. We have already seen the `i32`
type, which is a 32-bit signed integer. Rust also has `i8`, `i16`, `i64`, and
`i128` types, which are 8, 16, 64, and 128-bit signed integers respectively.
There are also unsigned integer types `u8`, `u16`, `u32`, `u64`, and `u128`.

Rust also has floating point types `f32` and `f64`, which are 32-bit and 64-bit
floating point numbers respectively. The `f64` type is the default floating
point type, and is the same as the `double` type in C++.

A *literal* is a value that is written directly into the source code. When we declare a variable
with a literal, the compiler infers the type of the variable from the literal. For example:

```rust
let x = 1; // x has type i32
let y = 2.0; // y has type f64
```

In the above example, `x` has type `i32` because `i32` is the default type for integer literals, and `y` has type `f64` because `f64` is the default type for floating point literals.

We can also explicitly specify the type of a variable by using a *type suffix* on the literal. For example:

```rust
let x = 1i64; // x has type i64
let y = 2.0f32; // y has type f32
```

Many other languages allow you to convert between types implicitly, which is a
common source of bugs. With its focus on safety, Rust does not allow implicit
type conversions. Instead, you must explicitly convert between types using the
`as` keyword. For example:

```rust
let x = 1i64; // x has type i64
let y = x as f32; // y has type f32
```

Another common source of bugs in other languages is overflow, which is when a
number is too large to be represented by its type. Rust prevents this by
checking for overflow at compile time. For example:

```rust
let x = 127i8;
let y = x + 1;
```

```
error: this arithmetic operation will overflow
 --> src/main.rs:4:13
  |
4 |     let y = x + 1;
  |             ^^^^^ attempt to compute `i8::MAX + 1_i8`, which would overflow
```

If the compiler cannot determine whether overflow will occur at compile time, it
will panic at runtime. For example:

```rust
let mut x = 0i8;
for _ in 0..128 {
    x += 1;
}
```

```
error: this arithmetic operation will overflow
 --> src/main.rs:4:13
  |
4 |     let y = x + 1; // error: literal out of range for i8
  |             ^^^^^ attempt to compute `i8::MAX + 1_i8`, which would overflow
  |
  = note: `#[deny(arithmetic_overflow)]` on by default
```


## char, str and String

A `char` is a single Unicode character, and is denoted by single quotes `'`. For
example:

```rust
let c = 'a';
```

A `str` is a *string slice*, which is a reference to a sequence of UTF-8 bytes
in memory. A string slice is denoted by double quotes `"`. For example:

```rust
let s = "hello";
```

A `String` is a *heap-allocated* string. In a systems programming language like Rust, it is important to
understand the difference between stack-allocated and heap-allocated data. Data
allocated on the stack is stored directly in the memory where the function is
executing. This is very fast, but the size of the data must be known at compile
time. Data allocated on the heap is stored in a different part of memory, and
the size of the data does not need to be known at compile time. This is slower
that stack allocation, since the program must request memory from the operating
system at runtime, but it is more flexible.

A `String` is heap-allocated, and can be created from a string slice using the
`to_string` method:

```rust
let s = "hello".to_string();
```




Often a String is created from the `format!` macro, which allows you to create a
string from a template:


```rust
let given = "Joe";
let middle = "Frederick";
let family = "\'Bloggs\'";
let full = format!("{} {} {}", given, middle, family);
println!("{}", full);
```

The syntax for the `format!` macro is similar to the `println!` macro that we have already seen, so we could have also written:

```rust
println!("{} {} {}", given, middle, family);
```

A `str` reference can be created from a `String` using the `as_str` method:

```rust
let full_ref = full.as_str();
```

Now we have a `String` and a `str` reference to the same data.

## References

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

Generally, you should not use pointers unless you need to worry about allocating
memory (e.g. you are writing a custom data structure). A more useful
label-to-a-box in C++ that must point to a valid memory location is provided by
a *reference*. This is similar to a pointer in that it is a label to a box
(rather than *being* that box), and therefore you can have multiple references
to the same box. But you can use it in the same way you would the original
variable. The type of a reference to an `int` is given by `int &`, so:

~~~cpp
int &r_number = six;
int &r_number2 = six;
std::cout << "six = " << r_number << std::endl;
r_number += 1;
std::cout << "seven = " << r_number << std::endl;
std::cout << "seven = " << r_number2 << std::endl;
std::cout << "seven = " << six << std::endl;
~~~

~~~
six = 6
seven = 7
seven = 7
seven = 7
~~~

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

There are two types of references in C++, *lvalue* and *rvalue* references.
Above we have used lvalue references, which are so-called because they can only
be *bound* to an lvalue, such as a variable or element of a container (i.e.
anything
 that you could put on the left hand side of an assignment `=`
statement). An lvalue reference is declared using a single `&`. 

```cpp
int six = 6;
int& r_six = six;
```

An rvalue refernce is more general in that it can also be bound to temporaries,
or rvalues. An rvalue could be a literal like `6` or the result of an expression
like `a + b` (i.e. something that you might see on the right hand side of an
assignment `=` statement). An rvalue reference is declared using two ampersands `&&`.

```cpp
int&& rr_six = 6;
```

Rvalue references are commonly used to enable *move semantics* in C++. You
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
T tmp(std::move(war_and_peace);
war_and_peace = std::move(moby_dick);
moby_dick = std::move(tmp);
```

The `std::move` function allows us to transfer the value of variable `a` to
variable `b`, without the requiriment of maintaining the value of `a`. Note that
after we have moved `a` its value is now unspecified, so after the last
statement in the snippet above, the value of `tmp` will be unspecified.


### Optional types

What if you wish to represent a variable that could be a value or could be
*nothing*? For example perhaps you have a function that takes as argument a
string and finds the first word beginning with the letter `a`. What should it
return if the string does not contain `a`?

Other languages have an inbuilt concept of nothing, for example the `None` value
in Python. In C++ you can instead define an *optional* type that could have a
value, or not. If we have a function `find_first_word_starting_with_a` that
returns an optional string type we can use it like so:

```cpp
const std::optional<std::string> word = find_first_word_starting_with_a(my_string);
if (word) {
    std::cout << "found the word " << *word std::endl;
} else {
    std::cout << "did not find a word" << std::endl;
}
```

Th angle brackets indicate that `std::optional` is a templated class. The first
and only *template argument* is `std::string`, meaning that
`std::optional<std::string>` is an optional `std::string` type. Templates in C++
enable yet another programming paradigm called
*generic programming*. As you can see, there are a number of different
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

::::challenge{id=cpp_calculate_pi title="Calculating PI"}

Create two `double` variables $x$ and $y$. Set $x=0.3$ and $y=0.4$ and
calculate $r = \sqrt{x^2 + y^2}$. Write the result $r$ to the console using
`std::cout`. Note that C++ has library functions `std::sqrt` and `std::pow` for
square root and power. You can see the cpp-reference page for `std::sqrt` [here](https://en.cppreference.com/w/cpp/numeric/math/sqrt). 

:::solution
~~~cpp
const double x = 0.3;
const double y = 0.4;

const double r = std::sqrt(std::pow(x,2) + std::pow(y,2));

std::cout << "r = "<< r << std::endl;
~~~
:::

Generate $N$ uniform random numbers $x_i$ and $y_i$ between -1 and 1 using
`std::uniform_real_distribution` (see example of use
[here](https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution)).
Count 
the number of points where $\sqrt{x_i^2 + y_i^2} < 1$, and use this to 
estimate the value of $\pi$.

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
~~~cpp
const int N = 1000;
double sum = 0.0;
for (int i = 1; i < N; ++i) {
  sum += 1.0/static_cast<double>(std::pow(i,2));
}
std::cout << "pi is about "<< std::sqrt(6.0*sum) << std::endl;
~~~
:::

::::

