---
name: Functions
dependsOn: [
    software_architecture_and_design.procedural.containers_rust,
]
tags: [rust]
---



# Functions

The *function* (aka *procedure*) is a one of the defining aspects of proceedural programming. It
allows you to package up some code defining a particular operation into a
re-useable *function* can can take zero or more *arguments* and (optionally)
*return* a value.

## Declaring and calling functions

In Rust a function is declared using the `fn` keyword, followed by the name of
the function, a list of parameters in brackets, the return type of the function, and finally the body of the function in curly brackets. Note that unlike C or C++, Rust does not split the declaration and definition of a function into two parts, the declaration is the definition.

```rust
fn first_times_second(x: f64, y: f64) -> f64 {
    return x * y;
}
```

Function names should be written in snake case (i.e. all lower case with words
separated by underscores).

The implementation of the function is the code between the curly brackets. This
is a block of code that is executed when the function is called. The function
can return a value by using the `return` keyword followed by the value to be
returned, or by simply writing the value to be returned as the last expression
in the function.

```rust
fn first_times_second(x: f64, y: f64) -> f64 {
    x * y
}
```

This latter form is idiomatic Rust, and is used in the rest of this course.

The function can be called by writing the name of the function followed by a list of arguments in brackets:

```rust
fn main() {
    let a = 1.0;
    let b = 2.0;
    let z = first_times_second(a, b);
    println!("{} times {} equals {}", a, b, z);
}
```


A function may also return no value by omitting the return type:

```rust
fn print_this(x: i32) {
    println!("{}", x);
}
```

Any variables that are used in the function must be declared as normal.

For example:

```rust
fn multiply_by_5(x: f64) -> f64 {
    let y = 5.0;
    x * y
}
```

Recall the rules about code blocks and *scope*, the scope of `y` lasts until the end of the
function (the last curly bracket) after which `y` is no longer available.


## Passing arguments

As we have previously seen, moving values is the default behaviour in Rust. In general, this is also the case for all function arguments, *except* for primitive types (e.g. `i32`, `f64`, `bool`, etc.) which are copied by default. For example:

```rust 
fn no_effect(x: f64) {
    x += 1.0;
}
```

Here the value of `x` is copied into the function, and the function only sees a copy. Changes in this copied variable have no effect on the original variable, for example the function `no_effect` has no effect on the `x` variable passed into it in `main`:

```rust
fn main() {
    let mut x = 2.0;
    no_effect(x);
    println!("{}", x);
}
```

The output of this program is `2.0`, not `3.0` as you might expect.

For all non-primitive types (e.g. `String`, `Vec`, etc.) the default behaviour is to move the value into the function. For example:

```rust
fn add_world(x: String) -> String {
    x.push_str(" world");
    x
}
```

Here the value of `x` is moved into the function, and changes within the
function are made the the variable that is moved from the outside scope.
However, since we have moved `x` into the function, we have also invalidated the
variable in the outside scope and are thus required to return it in order to see
the changes. We can use this function as follows:

```rust
fn main() {
    let x = String::from("hello");
    let y = add_world(x);
    println!("{}", y);
}
```

The output of this program is `hello world`. Note that is is rather cumbersome
to have to return the value from the function in order to see the changes. We
can avoid this by passing a *mutable* reference to the value into the function instead.
For example:

```rust
fn add_world(x: &mut String) {
    x.push_str(" world");
}
```

Here we have changed the type of `x` to be a mutable reference to a `String`
(i.e. `&mut String`). This means that the function now takes a reference to a
`String` as an argument, and that the function can modify the value that is
referenced. We can use this function as follows:

```rust
fn main() {
    let mut x = String::from("hello");
    add_world(&mut x);
    println!("{}", x);
}
```

Often the function will need to only read the value of the argument, and not
modify it. In this case we can pass an *immutable* reference to the value into
the function. For example:

```rust
fn print_this(x: &String) {
    println!("{}", x);
}

fn main() {
    let x = String::from("hello");
    print_this(&x);
    // x is still valid here
}
```

In summary, the four ways of passing arguments to a function are:

1. Copy (for primitive types) `fn foo(x: i32) { ... }`
1. Move (for non-primitive types) `fn foo(x: String) { ... }`
1. By immutable reference `fn foo(x: &String) { ... }`
1. By mutable reference `fn foo(x: &mut String) { ... }`


::::challenge{id=swap_cpp title="Swap Two Numbers"}

Write a function that accepts two floating point numbers (using references), and
swaps the values of these numbers.

:::solution
```rust
fn swap_these(x: &mut f64, y: &mut f64) {
    let tmp = *x;
    *x = *y;
    *y = tmp;
}

fn main() {
    let mut a = 1.0;
    let mut b = 2.0;
    swap_these(&mut a, &mut b);
    println!("a = {}, b = {}", a, b);
}
```
:::

::::

## Passing `String`/`Vec`` as `str`/`slice` references

When passing a `String` or `Vec` to a function, it is often the case that the
function only needs to read the value of the argument, and not modify it. In
this case, it is better to pass the argument as an immutable reference to a `str` or a slice, rather
than by a reference to the owning container. This allows the function to be used in more
situations.

```rust
fn print_this(x: &str) {
    println!("{}", x);
}

fn sum_this(x: &[f64]) -> f64 {
    let mut sum = 0.0;
    for i in x {
        sum += i;
    }
    sum
}

fn main() {
    let x = String::from("hello");
    print_this(x.as_str());
    let x = "world";
    print_this(&x);

    let y = vec![1.0, 2.0, 3.0];
    println!("{}", sum_this(y.as_slice()));
    let y = [1.0, 2.0, 3.0];
    println!("{}", sum_this(&y));
}
```

::::challenge{id=dot_product title="Scalar (dot) product"}

Write a function that returns the scalar (dot) product of two
`f64` slices.

:::solution
```rust
fn dot_product(x: &[f64], y: &[f64]) -> f64 {
    let mut dot = 0.0;
    for i in 0..x.len() {
        dot += x[i] * y[i];
    }
    dot
}
```

fn main() {
    let x = [1.0, 2.0, 3.0];
    let y = [4.0, 5.0, 6.0];
    println!("{}", dot_product(&x, &y));
}
:::
::::

## Generic argument types

Often a function will need to accept arguments of different types. For example,
a function that returns the minimum of two numbers should work for both `i32`
and `f64` types. In this case, we can use *generic* types. For example:

```rust
fn get_min<T: PartialOrd>(x: T, y: T) -> T {
    if x < y {
        x
    } else {
        y
    }
}

fn main() {
    let a = 1;
    let b = 2;
    let z = get_min(a, b);
    println!("{} is the minimum of {} and {}", z, a, b);

    let a = 1.0;
    let b = 2.0;
    let z = get_min(a, b);
    println!("{} is the minimum of {} and {}", z, a, b);
}
```

Here we have defined a function `get_min` that takes two arguments of the same
type `T` and returns a value of the same type `T`. The type `T` is a *generic*
type, and can be any type that supports the `<` operator. This restriction is
enforced by the `PartialOrd` trait bound on the generic type `T`. The compiler
will generate a new version of the `get_min` function for each type that is used
with it. For example, the compiler will generate a version of `get_min` for
`i32` and a version for `f64`.

Traits are an important concept in Rust, and we will cover them in more detail
in the next course so don't worry if you don't understand them fully yet. For
now, just think of them as a way of specifying a set of operations that a type
must support. For example, the `PartialOrd` trait specifies that a type must
support the `<` operator. Some other traits for numeric types are:

- `std::op::Add` - `+` operator
- `std::op::Sub` - `-` operator
- `std::op::Mul` - `*` operator
- `std::op::Div` - `/` operator
- `std::Copy` - type can be copied

The `num_traits` crate provides a number of useful traits for numeric types, including traits for specifing literal zero and one values:

- `num_traits::Zero` - type has a zero value, which can be obtained with `T::zero()`
- `num_traits::One` - type has a one value, which can be obtained with `T::one()`

If you want to use these traits, you will need to add the `num_traits` crate to
your `Cargo.toml` file:

```toml
[dependencies]
num-traits = "0.2"
```

To specify multiple trait bounds, you can use the `+` operator like so:

```rust
fn get_min<T: PartialOrd + Zero>(x: T, y: T) -> T {
    if x < y {
        x
    } else {
        y
    }
}
```

::::challenge{id=dot_product_generic title="Scalar (dot) product continued"}

Rewrite your dot product function to take any two slices `a` and `b` that follow
the standard slice interface in Rust, and where the type of the elements can be
multiplied, added, copied and have a zero value defined.

:::solution

You will need to add the `num_traits` crate to your `Cargo.toml` file. Then you
can make your function generic over any type that implements the `Zero`, `Add`,
`Mul` and `Copy` traits. Note that the compiler will inform you that you also
need to specify the output type of `Mul` (i.e. `Output = T`).

```rust
use std::ops::{Add, Mul};
use num_traits::Zero;

fn dot_product<T: Mul<Output = T> + Add + Copy + Zero>(x: &[T], y: &[T]) -> T {
    let mut dot = T::zero();
    for i in 0..x.len() {
        dot = dot + x[i] * y[i];
    }
    dot
}

fn main() {
    let x = [1.0, 2.0, 3.0];
    let y = [4.0, 5.0, 6.0];
    println!("{}", dot_product(&x, &y));
}
```
:::
::::


## Lifetimes of reference arguments

When passing a reference to a function, the compiler sometimes needs to know how
long the reference will be valid for. For example, consider the example we used
in the previous section where we stored a reference to an element of a `Vec`
before modifying it:

```rust
let mut v = vec![1];
let first = &v[0];
v.push(2);
println!("{}", first);
```

In this case, the borrow checker detects that we have both an immutable and a
mutable reference to the same `Vec` at the same time, and so the code is
rejected.

Now imagine that the act of taking the first reference is hidden in a function:

```rust
fn get_first(x: &[f64]) -> &f64 {
    &x[0]
}

fn main() {
    let mut v = vec![1.0];
    let first = get_first(&v);
    v.push(2.0);
    println!("{}", first);
}
```

We still get the same error. This is because the compiler has inferred the
lifetime for the returned `&f64` to be the same as the lifetime of the argument
`&[f64]` (you can see the rules for lifetime elison
[here](https://doc.rust-lang.org/reference/lifetime-elision.html)), and thus we
get the same situation of having both an immutable and a mutable reference to
the same `Vec` at the same time.

If we make the `get_first` function a bit more complicated by adding a second
reference argument, the compiler can no longer infer the lifetime of the returned
reference, and we must specify it explicitly:

```rust
fn get_first<'a>(x: &'a [f64], y: &[f64]) -> &'a f64 {
    &x[0]
}

fn main() {
    let mut x = vec![1.0];
    let mut y = vec![1.0];
    let first = get_first(&x, &y);
    y.push(2.0); // ok :)
    println!("{}", first);

    let mut x = vec![1.0];
    let mut y = vec![1.0];
    let first = get_first(&x, &y);
    x.push(2.0); // error!
    println!("{}", first);
}
```

Here we have specified that the lifetime of the returned reference is the same
as the lifetime of the first argument `x`. This means that the returned
reference is only valid as long as `x` is valid. In the first call to
`get_first`, the `push` to `y` is ok, because the compiler knows the returned
reference is only valid as long as `x` is valid, and `x` is not modified. In the
second call to `get_first`, the `push` to `x` is invalid because we already have
an immutable reference to `x` (the returned reference).



## Return values

Functions can have no return value

```rust
fn print_this(x: i32) {
    println!("{}", x);
}
```

a single return value

```rust
fn get_constant() -> i32 {
    42
}
```

multiple return values via a tuple:

```rust
fn get_student_and_grade() -> (String, f64) {
    ("Alice".to_string(), 42.0)
}
```

or can *optionally* return a value (i.e. either a value or nothing) via `Option`:

```rust
fn read_file_if_exists(filename: &str) -> Option<String> {
    // ... read file here
    Some(contents)
}
```

The use of `Option` here tells the caller that the return
`String` might not exist (e.g. if the file does not exist or cannot be
opened for reading) and that this possibility must be dealt with after calling
the function. For example:

```rust
fn main() {
    let filename = "data.txt";
    match read_file_if_exists(filename) {
        Some(contents) => println!("{}", contents),
        None => println!("Cannot read file {}", filename),
    }
}
```

## Errors and Result

It is normally neccessary to deal with errors that occur within a function in
such a way that the caller of that function is aware of the error and can deal
with it (if possible), or fail gracefully (perhaps clean up resources like an
open file for example). In the previous section we saw one approach to dealling
with an error, which is to return an optional value from the function. Another
approach is to use a Rust `Result` type. This is a type that can either be a
value or a specified error type. As an example, let us define a function for
solving a particular numerical problem (e.g. a root-finding problem). This function has an
input argument `x` of type `f64`, but the solver we are writing can only solve
the given problem for $x > 2.0$. Furthermore, even if $x > 2.0$ it is possible
that the function fails to find a solution to the problem. 

Since we have two possible points of failure, we decide to use a `Result` to
make the caller aware of any failures, and what in particular has gone wrong.

```rust
fn solve_problem(x: f64) -> Result<f64, String> {
    if x <= 2.0 {
        return Err("x must be greater than two".to_string());
    }
    // ... solve problem here
    if !success {
        return Err("solver failed".to_string());
    }
    Ok(result)
}
```

The `Result` type takes two type arguments, the first is the type of the value
that is returned if the function succeeds, and the second is the type of the
error that is returned if the function fails. Here we use `String` as the error
type, or you can use any type that implements `std::error::Error`.
Alternatively, you can use other third-party error types such as the popular `anyhow`
crate.

Below is an example of how you might call `solve_problem` and handle the
possible errors with a `match` statement:

```rust
fn main() {
    let solve_for_x = 1.456;
    match solve_problem(solve_for_x) {
        Ok(result) => println!("The solution is {}", result),
        Err(err) => {
            println!("Error: {}", err);
            // ... handle error here
        }
    }
}
```

## Propagating Result or Option with ?

When calling a function that returns a `Result` or `Option`, you can use the `?`
operator to propagate the error or `None` value to the caller. For example:

```rust
fn solve_with_value() -> Result<(), String> {
    let solve_for_x = 1.456;
    let result = solve_problem(solve_for_x)?;
    println!("The solution is {}", result);
    Ok(())
}
```

This is equivalent to:

```rust
fn solve_with_value() -> Result<(), String> {
    let solve_for_x = 1.456;
    let result = match solve_problem(solve_for_x) {
        Ok(result) => result,
        Err(err) => return Err(err),
    };
    println!("The solution is {}", result);
    Ok(())
}
```

An `Option` can be propagated in the same way:

```rust
fn read_data() -> Option<String>
    let filename = "data.txt";
    let contents = read_file_if_exists(filename)?;
    println!("{}", contents);
    Ok(contents)
}
```

The `?` operator can only be used in functions that return `Result` or `Option`,
and is a convenient way of propagating errors or `None` values through deeply
nested function calls.

::::challenge{id=substring title="Finding a substring"}

Write a function that takes two `&str` arguments `a` and `b`, and which tries to
find if `b` is a substring of `a`. If `b` is a substring of `a`, then the
function should return a `&str` reference of the first occurence of `b` in `a`.
If `b` is not a substring of `a`, then the function should return `None`.

Write a second function that instead of a `Option<&str>` return type, returns a
`Option<usize>` return type, where the `usize` is the index of the first
occurence of `b` in `a`. A common issue when using Rust is proving to the
compiler that the returned reference is valid, and a common way of avoiding this is
to use an index instead of a reference. Note that Rust also has automatic bounds
checking on array and vector indexing, so you can be sure (at run-time at least)
that the index is valid. 

:::solution
```rust
fn find_substring<'a>(a: &'a str, b: &str) -> Option<&'a str> {
    if a.len() < b.len() {
        return None;
    }
    for i in 0..a.len() - b.len() + 1 {
        if &a[i..i + b.len()] == b {
            return Some(&a[i..i + b.len()]);
        }
    }
    None
}

fn find_substring_index(a: &str, b: &str) -> Option<usize> {
    if a.len() < b.len() {
        return None;
    }
    for i in 0..a.len() - b.len() + 1 {
        if &a[i..i + b.len()] == b {
            return Some(i);
        }
    }
    None
}

fn main() {
    let a = "hello world";
    let b = "world";
    let c = "worlds";
    println!("{}", find_substring(a, b).unwrap());
    println!("{}", find_substring_index(a, b).unwrap());
    println!("{}", find_substring(a, c).unwrap_or("not found"));
    println!("{}", find_substring_index(a, c).unwrap_or(a.len()));
}
```
:::
::::

## Key Points

- A function is a block of code that can be called from other parts of the program, allowing code to be re-used.
- A function can take zero or more arguments, and can optionally return a value.
- Arguments can be copied (primitive types), moved (other types), or passed by mutable reference, or by immutable reference.
- Functions can return an `Option` for optional return values, or a `Result` for error handling.
- The `?` operator can be used to propagate `Option` or `Result` values to the caller.
