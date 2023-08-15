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


## Strings 

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

A `String` is a *heap-allocated* string. In a systems programming language like
Rust, it is important to understand the difference between stack-allocated and
heap-allocated data. Data allocated on the stack is stored directly in the
memory where the function is executing. This is very fast, but the size of the
data must be known at compile time. Data allocated on the heap is stored in a
different part of memory, and the size of the data does not need to be known at
compile time. This is slower that stack allocation, since the program must
request memory from the operating system at runtime, but it is more flexible.

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

## Pattern matching with Enums and Option

An *enum* is a type that can have a number of different values. You can declare
an enum using the `enum` keyword, and then list the possible values. For
example:

```rust
enum Direction {
    Positive,
    Negative,
}
```

The enum can hold data, for example we might want to store the magnitude of the change in either direction:

```rust
enum Change { 
    Positive(i32),
    Negative(i32),
}
```

Rust has a userful feature called *pattern matching*, which allows you to match on the
value of an enum. For example:

```rust
let change = Change::Positive(1);
match change {
    Change::Positive(x) => println!("change is positive by {}", x),
    Change::Negative(x) => println!("change is negative by {}", x),
}
```

Match is an expression and can be used on the right hand side of an assignment like so:

```rust
let mut value = 5;
value += match change {
    Change::Positive(x) => x,
    Change::Negative(x) => -x,
};


A common use of enums is to represent the absence of a value, using the `Option`
enum in the standard library. The `Option` enum can have the value `Some` or
`None`. The `Some` variant contains the value, and the `None` variant does not
contain a value. For example:

```rust
let x: Option<i32> = Some(1);
let y: Option<i32> = None;
```

We can use pattern matching to check if a value is present:

```rust
let x: Option<i32> = Some(1);
match x {
    Some(x) => println!("x is {}", x),
    None => println!("x is not present"),
}
```

You can also use the `if let` syntax to check if a value is present:

```rust
let x: Option<i32> = Some(1);
if let Some(x) = x {
    println!("x is {}", x);
} else {
    println!("x is not present");
}
```

::::challenge{id=option title="Option"}

The following code snippet loops through numbers from 0 to `x` and if the number
5 exists, stores it in the variable `five`. It uses `five = 0` to indicate that
the number 5 has not been found, but an `Option` type would be a more suitable
choice. Using the `Option` enum, rewrite the code so that the number 5 is stored
in the variable `five` if it exists, and `None` otherwise.

```rust
let max = 10;
let five = 0;
for i in 0..max {
    if i == 5 {
        five = i;
    }
}
if five == 0 {
    println!("five is not present");
} else {
    println!("five is {}", five);
}
```

:::solution

```rust
let max = 10;
let mut five = None;
for i in 0..max {
    if i == 5 {
        five = Some(i);
    }
}
if let Some(five) = five {
    println!("five is {}", five);
} else {
    println!("five is not present");
}
```
:::
::::


## Ownership

Rust is a *memory safe* language and has an explicit model of *ownership* to
ensure memory safety. The ownership model ensures that there is always exactly
one owner of a piece of data, and that the owner is responsible for cleaning up
the data when it is no longer needed. This put the responsibility of clearly
defining the lifetime of data and ownership on the programmer, rather than the
compiler. The compiler, however, will check that the rules of ownership are
followed and will give an error if they are not, ensuring that the program is
memory safe at compile time.

The ownership model is one of the most important features of Rust, and is
fundamental to understanding how to write Rust code. It is also one of the most
difficult concepts to understand, and you will likely spend a lot of time
fighting with the compiler to get your code to compile. However, once you
understand the ownership model and how to work with it, you will find that it
makes writing Rust code much easier and safer, and you will spend less time
debugging your code at runtime.

The rules of ownership are as follows:
- Each value or data in Rust has a variable that is called its *owner*.
- There can only be one owner at a time.
- When the owner goes out of scope, the value or data will be dropped.

Ownership is mainly relevent in the context of heap-allocated data, such as the `String` type. Let's look at an example:

```rust
{
    let s = String::from("hello"); // The memory for storing "hello" is allocated on the heap, and s is the owner of the String data
    println!("{}", s);             // s is valid within the block so we can access it here
} // The memory for storing the "hello" String data is dropped here, since s goes out of scope
println!("{}", s); // s is no longer valid here, so this will give a compiler error
```

Assigning one variable to another **moves** the data from one variable to the other, transferring ownership. For example:

```rust
let mut s_outer = String::from("hello");
{
    let s = String::from("goodbye"); // Allocate a new string and assign it to s
    s_outer = s;                     // The memory for storing "goodbye" is moved from s to s_outer, and s is no longer valid
}
println!("{}", s_outer); // s_outer is still valid here, since it is the owner of the String data
```

If we try to access `s` after it has been moved, we get a compiler error:

```rust
let mut s_outer = String::from("hello");
{
    let s = String::from("goodbye");
    s_outer = s;
    println!("{}", s);
}
```

```
error[E0382]: borrow of moved value: `s`
 --> src/main.rs:7:22
  |
5 |         let s = String::from("goodbye"); // The memory for storing "hello" is ...
  |             - move occurs because `s` has type `String`, which does not implement the `Copy` trait
6 |         s_outer = s;                     // The memory for storing "goodbye" i...
  |                   - value moved here
7 |         print!("{}", s);                 // s is no longer valid, so this line...
  |                      ^ value borrowed here after move
```

This compiler message mentions the `Copy` trait, which is a trait that indicates
that a type can be copied rather than moved. We will look at traits in more detail later.

If we want to copy the data rather than move it, we can use the `clone` method:

```rust
let mut s_outer = String::from("hello");
{
    let s = String::from("goodbye");
    s_outer = s.clone(); // The memory for storing "goodbye" is copied from s to s_outer, and s is still valid
    println!("{}", s);
}
```

Note that this discussion is only relevant for heap-allocated data, like `String`,
and for containers such as `Vec`. For stack-allocated data, like integers, the
data is copied rather than moved, so the original variable is still valid:

```rust
let mut x = 1;
let y = x; // The value of x is copied to y, and x is still valid
println!("x = {}, y = {}", x, y);
```

::::challenge{id=ownership title="Ownership"}

The following code snippet will not compile. Fix the code so that it compiles.
There are at least three ways to fix the code, can you list them all?

```rust
let hot_msg = "Its too hot!".to_string();
let ok_msg = "Its ok!".to_string();
for temp in 0..50 {
    let msg = if temp > 30 {
        hot_msg
    } else {
        ok_msg 
    };
    println!("{}: {}", temp, msg)
}
```

:::solution

You could either:
- Move the `hot_msg` and `ok_msg` variables into the loop

  ```rust
  for temp in 0..50 {
    let hot_msg = "Its too hot!".to_string();
    let ok_msg = "Its ok!".to_string();
    // ...
  ```

- Clone the `hot_msg` and `ok_msg` variables into the loop

  ```rust
    // ...
    let msg = if temp > 30 {
        hot_msg.clone()
    } else {
        ok_msg.clone()
    };
    // ...
  ```

- Use a literal `str` instead of a `String` for the `hot_msg` and `ok_msg` variables

  ```rust
  let hot_msg = "Its too hot!";
  let ok_msg = "Its ok!";
  for temp in 0..50 { //....
  ```

:::
::::



## References and the borrow checker

A *reference* to a variable acts like a pointer to that variable. It is a
lightweight way to allow multiple variables to access the same data, or to
enable passing in a variable to a function without moving or copying the
variable into the function scope. Unlike a pointer, a reference is guarenteed to
always point to valid data, but the reference does not own the data. For
example, we can create a reference to a variable using the `&` operator:

```rust
let x = 1;
let y = &x;
println!("x = {}, y = {}", x, y);
```

```
x = 1, y = 1
```

Here we have created a reference `y` to the mutable variable `x`. But we cannot change
the value of `x` through `y`:

```rust
let mut x = 1;
let y = &x;
*y = 2;
```

```
error[E0594]: cannot assign to `*y`, which is behind a `&` reference
 --> src/main.rs:5:5
  |
5 |     *y = 2;
  |     ^^^^^^ `y` is a `&` reference, so the data it refers to cannot be written
  |
help: consider changing this to be a mutable reference
  |
4 |     let y = &mut x;
  |             ~~~~~~
```

This is because `y` is an *immutable* reference to `x`. Helpfully, the compiler
suggests a fix, if we want to be able to change the value of `x` through `y`, we
need to make `y` a *mutable* reference:

```rust
let mut x = 1;
let y = &mut x;
*y = 2;
println!("y = {}", y);
```

```
y = 2
```

Here we have created a mutable reference `y` to the mutable variable `x`. We
can now change the value of `x` through `y` by dereferencing `y` using the `*`
operator.

In Rust, taking a reference to a variable is called *borrowing*. The compiler
enforces a set of rules to ensure that all borrows, or references, point to
valid data. This is called the *borrow checker*, and while you will find it is a
source of many compiler errors, it is a very powerful tool for ensuring the
safety of your code and eliminates many bugs that would otherwise be difficult
to find at runtime.

The borrow checker ensures that at any given time you can have **one mutable
reference** to a variable, or **any number of immutable references**.

For example, the following code is fine:

```rust
let x = 1;
let y = &x;
let z = &x;
```

```rust
let mut x = 1;
let y = &mut x;
```

But the following code is not:

```rust
let mut x = 1;
let y = &x;
let z = &mut x;
```

```rust
let mut x = 1;
let y = &mut x;
let z = &mut x;
```


::::challenge{id=pointers title="Getting used references"}

This is an example of mixing mutable and immutable references. The code will
not compile. Fix the code so that it compiles.

```rust
let mut full_name = "John Doe".to_string();
// Get the first name, split the string on whitespace and take the first element
let first_name = full_name.split(" ").next().unwrap(); 
println!("Hello, {}", first_name);
full_name.push_str(" Jr.");
println!("Hello, {}", first_name);
```

:::solution
One solution is to move the immutable reference into a block, so that it goes
out of scope before the mutable reference is created. Since we need to repeat the code
in each block, we should probably move it into a function, but we'll cover functions
in more detail later.

```rust
let mut full_name = "John Doe".to_string();
{
    // Get the first name, split the string on whitespace and take the first element
    let first_name = full_name.split(" ").next().unwrap(); 
    println!("Hello, {}", first_name);
}
full_name.push_str(" Jr.");
{
    let first_name = full_name.split(" ").next().unwrap();
    println!("Hello, {}", first_name);
}
```

## Key Points

- Rust is a statically typed language, which means that the compiler must be
  able to determine the type of every variable when it compiles your code.
- Rust has a number of built-in primitive types, including integers, floating
  point numbers, booleans, characters, and tuples.
- All type conversions in Rust must be explicit.
- Rust is a memory safe language and has an explicit model of ownership to
  ensure memory safety.
- The compiler enforces a set of rules via the borrow checker to ensure that all borrows, or
  references, point to valid data.



