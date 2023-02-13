---
name: Introduction to Unit Testing
dependsOn: [
  technology_and_tooling.unit_testing.setup
]
tags: [python, unit-testing]
---

# Introduction to Unit Testing

:::callout
This material was edited from the original in [Introduction to Unit Testing
with Python and GitHub](https://github.com/OxfordRSE/IntroUnitTestingCourse)
hosted by the Oxford RSE group.
:::

**Why should we test software?** Being able to demonstrate that a process
generates the right results is important in every field of research.

* Does the code we develop work the way it should do?
* Can we (and others) verify this correctness?
* To what extent are we confident in the results that appear in publications?

If we are unable to demonstrate that our software fulfils these criteria, then
it presents a barrier to use and adoption of the software not only by others,
but also by ourselves in the future.

::: callout

## But I don't write code with errors in

If each line we write has a 99% chance of being right, then a 70-line program is
more likely than not to be wrong. We need to do better than that, which means we
need to test our software to catch these mistakes. Even if you write some
perfect code the first time around, you (or someone else) will later modify it:
does it still behave the way you expect?

:::

## Manual versus automated testing

When we start out writing code, all the testing is done manually. We can often
look at the output of code, for example:

* a plot showing convergence with timestep
* running an analysis pipeline on an existing dataset with known output
* visually inspecting that a simulation “does the right thing”

These are important ways of testing during development, but have the drawback
that they only test a subset of expected behaviour. Also you have to rememember
to run them manually.

In contrast to manual testing, **automated testing** by a computer automates the
repetitive task of running tests and reporting errors.

## Kinds of tests

Not all tests are created equal. Tests can vary in size and also in the kind of
behaviour that they are verifying.

1. **Unit tests** are tests for fairly small and specific units of
   functionality. Typically these test at the level of functions or class
   methods.
2. **Functional or integration tests** work at a higher level, and test
   functional paths through your code. Typically these tests are at the class or
   module level.
3. **Regression tests** make sure that your program’s output hasn’t changed.

For the purposes of this course, we will focus on unit tests. In the next episode,
we will work with a sample dataset and application in Python and write tests for
it.
