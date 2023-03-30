---
name: Introduction to Unit Testing
dependsOn: [
  technology_and_tooling.unit_testing.setup
]
tags: [python, unit-testing]
---


## Introduction

Being able to demonstrate that a process generates the right results is
important in any field of research, whether it's software generating those
results or not. So when writing software we need to ask ourselves some key
questions:

- Does the code we develop work the way it should do?
- Can we (and others) verify these assertions for themselves?
- Perhaps most importantly, to what extent are we confident of the accuracy of results that software produces?

If we are unable to demonstrate that our software fulfills these criteria, why
would anyone use it? Having well-defined tests for our software is useful for
this, but manually testing software can prove an expensive process.

Automation can help, and automation where possible is a good thing - it enables
us to define a potentially complex process in a repeatable way that is far less
prone to error than manual approaches. Once defined, automation can also save us
a lot of effort, particularly in the long run. In this episode we'll look into
techniques of automated testing to improve the predictability of a software
change, make development more productive, and help us produce code that works as
expected and produces desired results.

::: callout

## But I don't write code with errors in

If each line we write has a 99% chance of being right, then a 70-line program is
more likely than not to be wrong. We need to do better than that, which means we
need to test our software to catch these mistakes. Even if you write some
perfect code the first time around, you (or someone else) will later modify it:
does it still behave the way you expect?

:::


## What Is Software Testing?

For the sake of argument, if each line we write has a 99% chance of being right,
then a 70-line program will be wrong more than half the time. We need to do
better than that, which means we need to test our software to catch these
mistakes.

We can and should extensively test our software manually, and manual testing is
well-suited to testing aspects such as graphical user interfaces and reconciling
visual outputs against inputs. However, even with a good test plan, manual
testing is very time consuming and prone to error. Another style of testing is
automated testing, where we write code that tests the functions of our software.
Since computers are very good and efficient at automating repetitive tasks, we
should take advantage of this wherever possible.

There are three main types of automated tests:

- **Unit tests** are tests for fairly small and specific units of functionality, e.g. determining that a particular function returns output as expected given specific inputs.
- **Functional or integration tests** work at a higher level, and test functional paths through your code, e.g. given some specific inputs, a set of interconnected functions across a number of modules (or the entire code) produce the expected result. These are particularly useful for exposing faults in how functional units interact.
- **Regression tests** make sure that your program's output hasn't changed, for example after making changes your code to add new functionality or fix a bug.

For the purposes of this course, we'll focus on unit tests. But the principles
and practices we'll talk about can be built on and applied to the other types of
tests too.

