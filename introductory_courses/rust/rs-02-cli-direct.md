---
name: Command line arguments direct
dependsOn: [introductory_courses.rust.rs-01-empty-app]
tags: [rust]
learningOutcomes:
  - Using modules with `use`
  - Using `std::env` for command line arguments
  - Iterator function examples
  - Type inference
  - Basic traits
  - Printing complex types with `dbg!`
---

# Command line arguments using `std::env::args`

In real-world application we often need user input either from arguments passed at the terminal or from input files. Here we explore the latter and update our application from a hello world app to printing any arguments.

Firstly, replace our `"Hello, world!"` `println!` with the following code:

```rs
    println!("Command line arguments:");
```

Then add this beneath

```rs
    for arg in std::env::args() {
        println!("{}", arg);
    }
```

We can see this is much like a Python `for` loop, where we are printing each `arg` in turn using a `{}`. Each `arg` is coming from an iterator function called `args` in the `std::env` namespace. This is a standard module in Rust for querying the process's environment.

Now try running this code with some arguments

```sh
cargo run -- xyz 123
```

Which will display

```
target/debug/heatsolve
xyz
123
```

There are three arguments: the application path, followed by the two command line arguments.

The `--` is optional in this case, but is good practice because it distinguishes from args passed to `cargo run`, and args passed to our program. For example, let's say we wanted to run a release version, it allows us to disambiguate

```sh
cargo run --release -- xyz 123
```

Now change change the title to print the number of incoming arguments

```rs
    println!("Got {} command line arguments:", std::env::args().len());
```

Here we see that `args()` is not just a for loop, the iterator provides other methods we can query.

## Use `use`

As our projects grow, so will the modules we need to use and long names like `std::env::args()` can be hard to manage. Let's simplify that now by adding the following line at the top of the file

```rs
use std::env;
```

Much like a Python import, we can now access our args without the leading `std::`. For example, let's change our iterator loop to

```rs
    for arg in env::args() {
```

## Collecting values

Another method on our iterator is `collect()` which, rather than stepping though values in turn for a `for` loop, allows us to pull them all into one container. Let's first try this:

```rs
    let args = env::args().collect();
```

When you try to `cargo run` this, rather than executing our program, the compiler prints some error messages. This is our first example of a compilation failure, we have a problem with out code that needs to be fixed. There is a lot of text which at first may seem overwhelming, but the key thing is to look for <span style="color:red">red</span> and <span style="colour:green">green</span> text. And here you will see in red:

```
error[E0283]: type annotations needed
```

And further down in green, the compiler suggests

```
  40 |     let args: Vec<_> = env::args().collect();
     |             ++++++++
```

This is our first encounter with typing in Rust and we are going to avoid going deep into details, the key thing to remember here is that `collect()` can return different types, and because we're working with a statically-typed language, the compiler needs to know what types it's working with.

`Vec` is a 1D vector or list of values, and because again Rust is statically typed, we can't have a `Vec` of arbitrary objects - as one could have in a `list` in Python - but we must instead declare the type of objects in the `Vec`. Let's park `_` for a moment and instead try the following:

```rs
    let args: Vec<String> = env::args().collect();
```

The code compiles and runs, but you will see a warning about `args` not being used, but first let's understand what the compiler originally meant by `Vec<_>`

## Inferred types

The Rust compiler needs to know about types, but that does not mean the programmer always has to declare types explicitly. It is possible in many cases for the compiler to fill the gaps by inferring what type goes where.

So even though we had to tell `collect()` that we wanted our results in a `Vec`, the compiler knows that all `env::args()` items will be strings. This makes our explicit declaration of `String` type here redundant

```rs
    let args: Vec<String> = env::args().collect();
```

We can instead say

```rs
    let args: Vec<_> = env::args().collect();
```

This asks the compiler make a `Vec` containing whatever type whatever type fits given the contents of `env::args()`. Here `_` is called an **inferred type placeholder** `_`.

There is another use of `_`  in Rust. As in Python, if we have a variable that we we don't care about or want to ignore, we can declare it as `_` or add a `_` prefix.

## Compiler warnings

This brings us back to our compiler warning earlier. We declared a variable `args` but never used it. The Rust compiler is risk adverse, an unused variable isn't doing anything and at best it's an oversight, but at worst it could be causing bugs.

Try changing `args` to `_` or `_args`, and `cargo run` for each, and you will find the warning goes away.

## Traits and debug printing

Let's now do an operation on the collected args. Rename `_args` back to the orignal `args` and add the following

```rs
    println!("{}", args);
```

When you `cargo run` it will again fail with an error

```
error[E0277]: `Vec<String>` doesn't implement `std::fmt::Display`
```

Unlike Python, not all types are printable at runtime. A type can only be rendered in a `{}` marker if if implements a **trait** called `std::fmt::Display`. We will touch on traits more throughout this material, but for now regard them as a set of methods that need to be implemented for something to work. Passing a `String` type to `println!` works because `String` implements the `Display` trait, but `Vec` not.

Traits are a powerful concept in Rust because we can write multiple implementations for any data type. We will not go deeply into this concept but we will be using them implicitly throughout these examples. In fact, we had already used traits earlier on; the result of `args()` is a structure called [`Args`](https://doc.rust-lang.org/beta/std/env/struct.Args.html) that implements many traits, including the [`Iterator`](https://doc.rust-lang.org/beta/std/iter/trait.Iterator.html) trait. This is what made the `collect()` and `len()` available to us, but there are many others like `skip()` we we could use here to strip off the first argument, the executable name.

For the time being though, to display our `Vec` we can turn to an alternative formatter called `std::fmt::Debug`. This is more a programmer tool than to be used in production code, but it is very useful whilst building applications.

Try changing the code to

```rs
    println!("{:?}", args);
```

Where `"{:?}"` tells the compiler to use the `Debug` [formatting trait](https://doc.rust-lang.org/std/fmt/index.html#formatting-traits). The `:` separates an optional identifier and formatting options.

Now run `cargo run -- xyz 123` and it will display

```
["target/debug/heatsolve", "xyz", "123"]
```

Alternatively, you can use `dbg!` macro instead of `println!` formatting for a quick-and-dirty way to show a value with more information

```rs
    dbg!(args);
```

Will print

```
[src/main.rs:54:5] args = [
    "target/debug/heatsolve",
    "xyz",
    "123",
]
```

## References

- [Module `env`](https://doc.rust-lang.org/beta/std/env/index.html)
- [`Args` iterator](https://doc.rust-lang.org/std/env/struct.Args.html#impl-Iterator-for-Args)
- [CLI example in Rust book](https://doc.rust-lang.org/book/ch12-01-accepting-command-line-arguments.html)
