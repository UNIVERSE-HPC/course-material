---
name: "Unit tests"
teaching: 15
exercises: 15
dependsOn: [
	technology_and_tooling.r_packaging.function_documentation
]
tags: [rpackaging]
attribution:
    - citation: >
        Wickham, Hadley, and Jennifer Bryan. R packages. " O'Reilly Media, Inc.", 2023.
      url: https://r-pkgs.org/
      license:  CC BY-NC-ND 4.0
---

## Unit tests

We've tested our function `strsplit1` informally, but we would like to do so systematically to ensure that: the function works as intended across more general examples; and the function continues to work when we develop the package. To do so, we are going to create unit tests.

A first step in this process is to call `use_testthat()`. This sets up the whole unit testing infrastructure used by the popular `testthat` package. You'll notice that by doing so, this creates a new folder in the package directory called `tests`. Within it are two things: a file called `testthat.R` -- generally, this file does not need to be manually changed; and a folder called `testthat` which will contain R files housing your unit tests.

Call `use_test("strsplit1")` to create a file which contains a dummy example test. Try highlighting this code and running it interactively to check it runs ok.

In developing a package, you will more typically do `test()` to run all of your unit tests. Try this
   - Note that tests are also run when `check()` is run

Write a `test_that` function which checks that `strsplit1` can split a string with comma separation. The `testthat` library has a whole raft of testing functions but the most commonly used of these are `expect_equal` and `expect_true`. `expect_equal` checks that the output of running a function matches that of a known value; `expect_true` checks that a given statement is true. For example, `expect_true(2 == 2)` will execute without error whereas `expect_true(2 == 1)` will not.

Write an additional separate test that checks if `strsplit1` can split a string with hyphen separation

Try `test()` again to check that all unit tests pass.

## Checking for appropriate function arguments

It is good practice to check that a function's inputs are what are expected and to build your function to raise an informative error message if not. We are now going to modify the function to check that the input `x` is a character string and, if not, raise an error message. Our modified function is shown below:

```R
strsplit1 <- function(x, split) {

	if(!is.character(x))
		stop("Input must be a character string")

	unlist(strsplit(x, split = split))
}
```

We now want to check that, if an argument is supplied that is not a character string, then the function raises the appropriate error. To do so, write an additional test function in the same file that uses the following error check:

```R
expect_error(strsplit1(123, ","), "Input must be a character string")
```

If calling `strsplit1(123, ",")` raises the error message "Input must be a character string" then the function has behaved as intended.

Try `test()` again to check that all unit tests pass.
