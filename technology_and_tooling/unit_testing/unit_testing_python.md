---
name: Unit Testing with Python
dependsOn: [technology_and_tooling.unit_testing.introduction]
tags: [python, unit-testing]
---

# Unit testing in Python

In this episode, we will look at how to write unit tests in Python using the
example dataset and application at
<https://github.com/OxfordRSE/IntroUnitTestingCourse>

## Using the `assert` statement

The `assert` statement in Python evaluates an expression. If the expression is
true, program execution continues on to the next statement. If the expression is
false, program execution halts and raises an `AssertionError`. Adding `assert`
statements to code is an easy way to start testing.

As an example, we will test the `daily_mean` function defined in
[models.py](https://github.com/OxfordRSE/IntroUnitTestingCourse/blob/main/inflammation/models.py).
Create a file `sandbox.py` and paste the following code:

```python
import pytest
from inflammation.models import daily_mean
import numpy as np

assert np.array_equal(np.array([0, 0]), daily_mean(np.array([[0, 0], [0, 0]])))
assert np.array_equal(np.array([3, 4]), daily_mean(np.array([[1, 2], [3, 4], [5, 6]])))
```

Once we run this code (using `python sandbox.py`), we get no output as no
`AssertionError` was raised. So far so good. But now let us add a failing line at
the front:

```python
from inflammation.models import daily_mean
import numpy as np

assert np.array_equal(np.array([2, 0]), daily_mean(np.array([[2, 0], [4, 0]])))
assert np.array_equal(np.array([0, 0]), daily_mean(np.array([[0, 0], [0, 0]])))
assert np.array_equal(np.array([3, 4]), daily_mean(np.array([[1, 2], [3, 4], [5, 6]])))
```

Once we re-run, we get the following error:

```shell
Traceback (most recent call last):
  File "/home/user/IntroUnitTestingCourse/sandbox.py", line 4, in <module>
    assert np.array_equal(np.array([2, 0]), daily_mean(np.array([[2, 0], [4, 0]])))
AssertionError
```

### What are the problems with this?

Using `assert` statements pauses the execution of the program at the first error
that occurs. We do not know if the other tests are passing or failing as program
execution never reaches that point.

## Using a testing framework

Most people don’t enjoy writing tests, so if we want them to actually do it, it must be easy to:

- Add or change tests.
- Understand the tests that have already been written.
- Run those tests.
- Understand those tests’ results.

There are many unit testing frameworks in different languages

- Python: pytest, unittest, nose2
- C++: Catch2, GoogleTest, ...
- Java: JUnit
- Fortran: FRUIT

Let’s add some tests for our library function, `daily_mean`:

```python
import numpy as np
def test_daily_mean_zeros():
    """Test that mean function works for an array of zeros."""
    from inflammation.models import daily_mean

    test_array = np.array([[0, 0],
                            [0, 0],
                            [0, 0]])

    # Need to use Numpy testing functions to compare arrays
    np.assert_array_equal(np.array([0, 0]), daily_mean(test_array))
```

Run `pytest tests/test_models.py`

::: exercise
Add tests for the library functions `daily_min` and `daily_max`.
:::

## Testing for exceptions and errors

Some functions do not support all inputs. In compiled languages you often get
help from the type system (but not so much in Python). Even then, a function
might expect, say, a strictly positive argument and throw an error of some kind
if it does not. How can we test this? All unit testing frameworks have this
functionality built in.

```python
def test_daily_min_string():
    """Test for TypeError when passing strings"""
    from inflammation.models import daily_min
    from pytest import raises

    with raises(TypeError):
        daily_min([['Cannot', 'min'], ['string', 'arguments']])
```

This code uses the `raises` function defined in pytest to create a block that
**expects** a `TypeError` to be raised. In fact, the test will fail if
`daily_min` does not raise any exception.

## When should we test invalid input data?

Testing the behaviour of inputs, both valid and invalid, is a good idea and is
known as **data validation**. Even if you are developing command-line software
that cannot be exploited by malicious data entry, testing behaviour against
invalid inputs prevents generation of erroneous results that could lead to
serious misinterpretation. It’s generally best not to assume your user’s inputs
will always be rational.

::: exercise
Add some tests for errors with invalid input.
:::

## Parameterise tests to run over many test cases

We are starting to build up a number of tests that test the same function, but
just have different parameters. Instead of writing a separate function for each
different test, we can **parameterise** the tests with multiple test inputs. For
example, we could rewrite the `test_daily_mean_zeros()` and
`test_daily_mean_integers()` into a single test function:

```python
@pytest.mark.parametrize(
    "test, expected",
    [
        ([[0, 0], [0, 0], [0, 0]], [0, 0]),
        ([[1, 2], [3, 4], [5, 6]], [3, 4]),
    ])
def test_daily_mean(test, expected):
    """Test mean function works for array of zeroes and positive integers."""
    from inflammation.models import daily_mean
    np.assert_array_equal(np.array(expected), daily_mean(np.array(test)))
```

::: exercise
Re-write your tests for mean, min and max to be parameterised.
:::

::: callout

## Commiting changes

At this point, commit the changes to your testing code:

```shell
git add tests/test_models.py
git commit -m “Added more tests”
git push
```

For more information on version control, refer to the Version Control with Git
module in the curriculum.
:::

## Recap

- We are making use of a unit testing framework (pytest).
- We have written tests that verify normal functional behaviour of three specific units.
- We have tested that some common cases of function misuse fail in the way we expect them to fail.
- We have parameterised tests to cut down on code duplication.

In the next episode, we will walk through how to run unit tests automatically
when we push code changes to GitHub.
