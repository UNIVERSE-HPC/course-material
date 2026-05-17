---
name: Cargo package manager
dependsOn: [introductory_courses.rust.rs-00-prerequisites]
tags: [rust]
learningOutcomes:
  - Introduction to cargo
  - Creating, building and running an application
  - Debug and release builds
  - Cargo.toml
  - Cargo lock files
  - Printing with `println!`
---

# Cargo

Cargo is the Rust package manager. This includes managing dependencies and building and running your own packages. For most of the course we will mostly consider building and running our code and only touch on packages, so for the scope of this material we can regard packages in Rust as one would `pip install` a module in Python. For more information please refer to [doc.rust-lang.org/cargo/](https://doc.rust-lang.org/cargo/)

`cargo init` creates a Rust project. By default it creates a binary command line
application, which is what these workshop folders use. Library projects are
created with `cargo init --lib`, but we will keep the main focus on binaries.

# Base Rust Project

In your terminal run the command

```sh
cargo init heatsolve
```

This creates a new `heatsolve` subfolder and will initialise it as a git repo. Note you can also run this in an existing folder with no args, e.g. if you had run `mkdir heatsolve` and `cd heatsolve` then running `cargo init` will set up the current folder as a prokect.

By default `cargo init` sets up a binary executable command line app (equivalent to `cargo init --bin`). The main entrypoint is `src/main.rs` which by default is set up with a minimal hello world app.

*One can also build libraries that can be shared and used in other apps running `cargo init --lib`. We will not cover libs in this material but if you do generate a lib, instead of `src/main.rs` you will see `src/lib.rs`. Many lib projects will not commit their `Cargo.lock` file for flexible versioning, whereas most bin projects do commit `Cargo.lock` to make their build requirements explicit.
*

## Folder contents

Navigate to the folder to view the contents

```sh
cd heatsolve
ls -al
```

The folder contains

- `src/main.rs` - source code
- `Cargo.toml` - project specification
- `.git` - git repo
- `.gitignore` git ignores

Before we start modifying code, let's git commit our starting point for reference

```sh
git commit -m 'Create heatsolve project'
```

## Running the code

In the folder now simply run

```sh
cargo run
```

And you will see something like this

```
❯ cargo run
   Compiling heatsolve v0.1.0 (/Users/<username>/heatsolve)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.32s
     Running `target/debug/heatsolve`
Hello, world!
```

This `Hello, world!` app is the default skeleton project that `cargo init` had created. If you open `src/main.rs` you will see

```rs
fn main() {
    println!("Hello, world!");
}
```

When we ran `cargo run` this source code was read and compiled into machine code that runs directly on your hardware, i.e. there is no interpreter like Python reading the code in real-time, but instead it is running in the native binary language of your computer, say x64 for Windows or ARM on macos.

This `Hello, world!` app is the default skeleton project that `cargo init` creates. If you open `src/main.rs` you will see

```rs
fn main() {
    println!("Hello, world!");
}
```

Now run `cargo run` again

```
❯ cargo run
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.03s
     Running `target/debug/heatsolve`
Hello, world!
```

Notice the output here is a little different, we are missing the 'Compiling...' information. This is because `cargo run` has detected there have been no changes to `src/main.rs` and it can rerun the previously-built binary executable.

We can see how `cargo` determined this is if we `ls -al` again, and see we have two new files

- `target/` - built binary files
- `Cargo.lock` - version information about build and dependencies

The `target/` folder is where the binary was built to. `cargo` can guarantee that these binary artefacts be reliably rebuilt, so they do not need to be checked in, and if you open `.gitignore` will can see they are excluded from git.

The last line of our `cargo run` output said ``Running `target/debug/heatsolve/` ``. Try that directly by running directly in your terminal

```sh
target/debug/heatsolve
```

`cargo run` simply offers a friendly layer around this build-and-run process. If you want you can rebuild manually with `cargo build` and then run the executable directly.

## Debug and release builds

The folder `target/debug` is so named because this code is compiled primarily for debugging. You will frequently encounter the names 'debug' and 'release' when working in compiled languages and it is a rich subject to explore, but in short the debug code contains extra information that makes it much easier to interrogate during development, and if we were to open it in a debugger we could put breakpoints on lines of code and look at the values of variables. This is great for early development and diagnosing problems as we can 'step' through lines of code and see what's going on.

However, if you were shipping a production app to end users you would instead build for release. This strips away debugging information so the executable is often smaller, but will also usually run much faster because it enables the compiler to aggressively optimise the code. This makes it much harder to debug, so we tend to only build release as the final stage of a build. Try this now with by running

```sh
cargo build --release
```

You will see this instead creates binary artefacts in `target/release`, and you can also run these directly.

## Versioning and Cargo.toml

When we ran `cargo run` it sait 'Compiling heatsolve v0.1.0...' and this version information is coming from `Cargo.toml`. This is the [Cargo manifest](https://doc.rust-lang.org/cargo/reference/manifest.html) file which contains version and build information about this project, and much like a `pyproject.toml` file in python can also contain metadata about the authors, license and various features. Importantly, it also details and project dependencies, which we will come to later.

## Cargo.lock

The other file that was created from `cargo run` was `Cargo.lock` this lockfile retains all packaging information about this build and it is what allows Cargo to reliably rebuild binary artefacts on any machine. At this stage the file is very minimal because we have no dependencies in our `.toml` but as we work through the material we will see this file become more complex

```
# This file is automatically @generated by Cargo.
# It is not intended for manual editing.
version = 4

[[package]]
name = "heatsolve"
version = "0.1.0"
```

Do not worry about changes in this file, it is autogenerated by and for `cargo`. However, one thing to note is whether we should commit this file or not. The rule of thumb is when generating a standalone binary application (`cargo init` or `cargo init --bin`) then we *do* want to commit this `.lock` file so other users can rebuild exactly the same binary. However if we were building a library (`cargo init --lib`) then we may *not* want to commit this file, so users of our library have freedom to build against different dependency versions. We do not explore this further in this course as we focus entirely on binary applications.

## Printing with `println!`

A natural question to ask looking at our code is what is `println!` and why does it have an exclamation mark. Much like Python's `print` method, this will print some text to stdout with a newline at the end.

However, the `!` at the end is significant and you will see this a lot in Rust. This means that this is not a *method* but a *macro*. If you are not writing your own macros then most of the time, from a developer's perspective, macros are used much like functions: they take a number of parameters and do something. Under the hood though they differ in how they are evaluated by the compiler, which looks at this expression and can make compile-time decisions about how to process it.

Importantly, function-like macros like this allow for different numbers of arguments of different types, because - unlike Python - in Rust a function has a fixed number of arguments, and whoever calls that function must call with exactly number of those arguments and types. This may feel limiting at first but is beneficial to the compiled code. For now, let's not worry about these intricacies and just familiar with the syntax.

Say you wanted to print a number of values, then you can do do using `{}` formatting arguments (which much like Python, may contain formatting style information). Try this now with the following changes:

```rs
println!("Hello, world! {} {}", 123, "test");
```

Each `{}` corresponds to a value after the string.

## Cargo quick reference

```sh
cargo init my-project
cd my-project
cargo run
cargo test
```

## References

- [Formatting styles](https://doc.rust-lang.org/stable/std/fmt/index.html)
- [Formatted print](https://doc.rust-lang.org/rust-by-example/hello/print.html)