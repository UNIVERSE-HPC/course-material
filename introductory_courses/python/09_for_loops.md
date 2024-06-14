---
name: For Loops
dependsOn: [
    introductory_courses.python.08_lists
]
tags: [python]
attribution: 
    - citation: >
        "Programming with Python" course by the Carpentries
      url: https://swcarpentry.github.io/python-novice-inflammation/
      image: https://carpentries.org/assets/img/TheCarpentries.svg
      license: CC-BY-4.0
---

## A *for loop* executes commands once for each value in a collection

* Doing calculations on the values in a list one by one
    is as painful as working with `pressure_001`, `pressure_002`, etc.
* A *for loop* tells Python to execute some statements once for each value in a list,
    a character string,
    or some other collection.
* "for each thing in this group, do these operations"

``` python
for number in [2, 3, 5]:
    print(number)
```

* This `for` loop is equivalent to:

``` python
print(2)
print(3)
print(5)
```

* And the `for` loop's output is:

```text
2
3
5
```

## A `for` loop is made up of a collection, a loop variable, and a body

``` python
for number in [2, 3, 5]:
    print(number)
```

* The collection, `[2, 3, 5]`, is what the loop is being run on.
* The body, `print(number)`, specifies what to do for each value in the collection.
* The loop variable, `number`, is what changes for each *iteration* of the loop.
  * The "current thing".

## The first line of the `for` loop must end with a colon, and the body must be indented

* The colon at the end of the first line signals the start of a *block* of statements.
* Python uses indentation rather than `{}` or `begin`/`end` to show *nesting*.
  * Any consistent indentation is legal, but almost everyone uses four spaces.

```python
for number in [2, 3, 5]:
print(number)
```

```text
IndentationError: expected an indented block
```

* Indentation is always meaningful in Python.

```python
firstName = "Jon"
  lastName = "Smith"
```

```text
  File "<ipython-input-7-f65f2962bf9c>", line 2
    lastName = "Smith"
    ^
IndentationError: unexpected indent
```

* This error can be fixed by removing the extra spaces
    at the beginning of the second line.

## Loop variables can be called anything

* As with all variables, loop variables are:
  * Created on demand.
  * Meaningless: their names can be anything at all.

``` python
for kitten in [2, 3, 5]:
    print(kitten)
```

## The body of a loop can contain many statements

* But no loop should be more than a few lines long.
* Hard for human beings to keep larger chunks of code in mind.

``` python
primes = [2, 3, 5]
for p in primes:
    squared = p ** 2
    cubed = p ** 3
    print(p, squared, cubed)
```

```text
2 4 8
3 9 27
5 25 125
```

## Use `range` to iterate over a sequence of numbers

* The built-in function [`range`](https://docs.python.org/3/library/stdtypes.html#range) produces a sequence of numbers.
  * *Not* a list: the numbers are produced on demand
        to make looping over large ranges more efficient.
* `range(N)` is the numbers 0..N-1
  * Exactly the legal indices of a list or character string of length N

```python
print('a range is not a list: range(0, 3)')
for number in range(0, 3):
    print(number)
```

```text
a range is not a list: range(0, 3)
0
1
2
```

## The Accumulator pattern turns many values into one

* A common pattern in programs is to:
    1. Initialize an *accumulator* variable to zero, the empty string, or the empty list.
    2. Update the variable with values from a collection.

``` python
# Sum the first 10 integers.
total = 0
for number in range(10):
   total = total + (number + 1)
print(total)
```

```text
55
```

* Read `total = total + (number + 1)` as:
  * Add 1 to the current value of the loop variable `number`.
  * Add that to the current value of the accumulator variable `total`.
  * Assign that to `total`, replacing the current value.
* We have to add `number + 1` because `range` produces 0..9, not 1..10.

::::challenge{id="classifying_errors" title="Classifying Errors"}

Is an indentation error a syntax error or a runtime error?
:::solution
An IndentationError is a syntax error. Programs with syntax errors cannot be started.
A program with a runtime error will start but an error will be thrown under certain conditions.
:::
::::

::::challenge{id="tracing_execution" title="Tracing Execution"}

Create a table showing the numbers of the lines that are executed when this program runs,
and the values of the variables after each line is executed.

``` python
total = 0
for char in "tin":
    total = total + 1
```

:::solution

| Line no | Variables            |
|---------|----------------------|
| 1       | total = 0            |
| 2       | total = 0 char = 't' |
| 3       | total = 1 char = 't' |
| 2       | total = 1 char = 'i' |
| 3       | total = 2 char = 'i' |
| 2       | total = 2 char = 'n' |
| 3       | total = 3 char = 'n' |
:::
::::

::::challenge{id="reversing_a_string" title="Reversing a String"}

Fill in the blanks in the program below so that it prints "nit" (the reverse of the original character string "tin").

``` python
original = "tin"
result = ____
for char in original:
    result = ____
print(result)
```

:::solution

``` python
original = "tin"
result = ""
for char in original:
    result = char + result
print(result)
```

:::
::::

::::challenge{id="practice_accumulating" title="Practice Accumulating"}

Fill in the blanks in each of the programs below to produce the indicated result.

``` python
# Total length of the strings in the list: ["red", "green", "blue"] =12
total = 0
for word in ["red", "green", "blue"]:
    ____ = ____ + len(word)
print(total)
```

:::solution

``` python
total = 0
for word in ["red", "green", "blue"]:
    total = total + len(word)
print(total)
```

:::

``` python
# List of word lengths: ["red", "green", "blue"] =[3, 5, 4]
lengths = ____
for word in ["red", "green", "blue"]:
    lengths.____(____)
print(lengths)
```

:::solution

``` python
lengths = []
for word in ["red", "green", "blue"]:
    lengths.append(len(word))
print(lengths)
```

:::

``` python
# Concatenate all words: ["red", "green", "blue"] ="redgreenblue"
words = ["red", "green", "blue"]
result = ____
for ____ in ____:
    ____
print(result)
```

:::solution

``` python
words = ["red", "green", "blue"]
result = ""
for word in words:
    result = result + word
print(result)
```

:::

__Create an acronym:__ Starting from the list `["red", "green", "blue"]`, create the acronym `"RGB"` using a for loop.

__Hint:__ You may need to use a string method to properly format the acronym.
:::solution

``` python
acronym = ""
for word in ["red", "green", "blue"]:
    acronym = acronym + word[0].upper()
print(acronym)
```

:::
::::

::::challenge{id="cumulative_sum" title="Cumulative Sum"}

## Cumulative Sum

Reorder and properly indent the lines of code below
so that they print a list with the cumulative sum of data.
The result should be `[1, 3, 5, 10]`.

``` python
cumulative.append(total)
for number in data:
cumulative = []
total += number
total = 0
print(cumulative)
data = [1,2,2,5]
```

:::solution

``` python
total = 0
data = [1,2,2,5]
cumulative = []
for number in data:
    total += number
    cumulative.append(total)
print(cumulative)
```

:::
::::

::::challenge{id="identifying_variable_name_errors" title="Identifying Variable Name Errors"}

1. Read the code below and try to identify what the errors are
   *without* running it.
2. Run the code and read the error message.
   What type of `NameError` do you think this is?
   Is it a string with no quotes, a misspelled variable, or a
   variable that should have been defined but was not?
3. Fix the error.
4. Repeat steps 2 and 3, until you have fixed all the errors.

``` python
for number in range(10):
    # use a if the number is a multiple of 3, otherwise use b
    if (Number % 3) == 0:
        message = message + a
    else:
        message = message + "b"
print(message)
```

:::solution

* Python variable names are case sensitive: `number` and `Number` refer to different variables.
* The variable `message` needs to be initialized as an empty string.
* We want to add the string `"a"` to `message`, not the undefined variable `a`.

``` python
message = ""
for number in range(10):
    # use a if the number is a multiple of 3, otherwise use b
    if (number % 3) == 0:
        message = message + "a"
    else:
        message = message + "b"
print(message)
```

:::
::::

::::challenge{id="identifying_item_errors" title="Identifying Item Errors"}

1. Read the code below and try to identify what the errors are
   *without* running it.
2. Run the code, and read the error message. What type of error is it?
3. Fix the error.

``` python
seasons = ['Spring', 'Summer', 'Fall', 'Winter']
print('My favorite season is ', seasons[4])
```

:::solution
This list has 4 elements and the index to access the last element in the list is `3`.

``` python
seasons = ['Spring', 'Summer', 'Fall', 'Winter']
print('My favorite season is ', seasons[3])
```

:::
::::
