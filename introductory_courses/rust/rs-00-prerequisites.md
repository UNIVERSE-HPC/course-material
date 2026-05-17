---
name: Rust Foundation
dependsOn: []
tags: [rust]
learningOutcomes:
  - Rust language overview
  - Setting up your IDE and installing Rust
  - Structure of course
---

# Prerequisites

This first section is for getting everyone to a working Rust installation and a
comfortable editor setup before the workshop code begins.

## Rust overview

This course is provides a broad overview of the Rust programming language. Rather than taking a deep dive into language features, we will instead work by example in developing a heat equation solver command line application, and on the way we will touch on common operations like reading command line arguments, reading and writing files, and use of libraries. We assume that students are already familiar with another language like Python and concepts like program flow.

Rust's syntax can appear imposing to newcomers, so the focus of this course is for us to begin develop our 'Rust eyes' and have enough exposure to the language to get started in our own projects.

Before starting this course, you should install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/).

Rust is a language that allows developers to make the most of a system's performance. Unlike dynamically-typed interpreted languages like Python - which by default runs code on-the-fly and where the types of values are not known until they are evaluated - Rust is a statically-typed compiled language. This means that rather than processing the files through an interpreter like Python, the code first must be compiled into machine code that can be run directly on that hardware. Rust has become very popular as a language because it offers clearer memory safety and packaging that C++.

## Course structure

You can work through this material by following just the material on this side, but if you get stuck each page of this course has it's own folder in [rust-foundation](github.com/OxfordRSE/rust-foundation). As you work through this material you are effectively implementing the TODO blocks in that code.

Each page follows from the next as we gradually add more features, so unlike the reference material you do not need create a new folder for each page; every follows naturally from the first `cargo init` command covered on the next page.

The reference source files contain commented-out code, but for reference you can see checkpoints where each TODO item has been implemented along the way in the `.checkpoints/` folder in each.

Note in the reference source material, the `rs-` prefix in the folder names is deliberate: Rust package names cannot start with a digit, so `rs-01-empty-app` is a valid Cargo package name.

## Install Rust

Rust is usually installed with `rustup`, which also installs the main tools we
will use today:

- `rustc` - the Rust compiler.
- `cargo` - the package manager, build tool, and test runner.
- `rustup` - the toolchain installer and updater.

On macOS and Linux, install from <https://rustup.rs/> and follow the terminal
instructions. On Windows, the same installer will guide you through any extra
requirements.

Check the install with:

```sh
rustc --version
cargo --version
rustup --version
```

## Editor Setup

The examples work from any terminal, but if you want to explore debugging in VSCode then install the `rust-analyzer` extension.

## Dev workspace

The material is all designed to be worked through incrementally, unlike the reference material, you can just work in one folder and add code as you go. You may find it useful to `git init` and `git commit` changes as you go so you can track changes and have a log of your work as you go.

To get started, navigate to a folder you want to work in in your terminal, e.g.

```sh
cd ~/Documents
```
