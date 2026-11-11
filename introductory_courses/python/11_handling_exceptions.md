---
name: Raising and Handling Exceptions
dependsOn: [introductory_courses.python.10_variable_scope]
tags: [python]
learningOutcomes:
  - Raising an exception to signal that something has gone wrong.
  - Handling specific exceptions with try and except.
  - Explaining why a bare except can hide real errors.
attribution:
  - citation: This material has been adapted from the "Software Engineering" module of the SABS R³ Center for Doctoral Training.
    url: https://www.sabsr3.ox.ac.uk
    image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Exceptions change the flow of a program

- In [Errors and Exceptions](08_errors_and_exceptions) we read the tracebacks Python prints when something goes wrong.
- Each of those errors is an _exception_: an object that says something unusual has happened.
- When an exception is _raised_ (other languages say _thrown_), Python stops running the current code
  and passes the exception back up to whatever called it.
- If nothing _catches_ (or _handles_) the exception, the program stops and prints a traceback.
- In this episode we'll raise exceptions from our own functions, and catch the ones we expect.

## Use `raise` to signal that something has gone wrong

- A function can't always do something sensible with the values it's given.
- Rather than return a wrong answer quietly, it can `raise` an exception.
- Pick the built-in exception type that best describes the problem, and give it a helpful message.
  - `ValueError`: the value has the right type but makes no sense, e.g. a negative temperature in Kelvin.
  - `TypeError`: the value is the wrong type altogether.

```python
def kelv_to_cels(kelv):
    if kelv < 0:
        raise ValueError('Temperature in Kelvin cannot be negative: ' + str(kelv))
    return kelv - 273.15

print(kelv_to_cels(300))
print(kelv_to_cels(-5))
```

```text
26.850000000000023
Traceback (most recent call last):
  File "temperatures.py", line 7, in <module>
    print(kelv_to_cels(-5))
  File "temperatures.py", line 3, in kelv_to_cels
    raise ValueError('Temperature in Kelvin cannot be negative: ' + str(kelv))
ValueError: Temperature in Kelvin cannot be negative: -5
```

- The traceback points at the line that called the function with a bad value, and the message says what was wrong.
- That is much easier to debug than a negative Celsius value that turns up later in an analysis.

:::callout

## Searching with `for` ... `else`

A `for` loop can have an `else` block. It runs only if the loop finishes without reaching `break`.
When you use a loop to search for something, that means nothing was found, which is a good place to raise an exception:

```python
def find_first_negative(values):
    for value in values:
        if value < 0:
            break
    else:
        raise ValueError('No negative values found')
    return value

print(find_first_negative([3, 1, -4, 1, -5]))
```

```text
-4
```

See the [Python documentation](https://docs.python.org/3/tutorial/controlflow.html#break-and-continue-statements-and-else-clauses-on-loops)
for more on `break` and `else` in loops.
:::

## Use `try` and `except` to handle errors you expect

- Sometimes an error is expected and we know how to recover, e.g. a file might be missing, or a user might type text where we wanted a number.
- Put the code that might fail in a `try` block.
- Follow it with an `except` block naming the exception type you expect, and what to do instead.
- If the `try` block raises that exception, Python jumps straight to the `except` block rather than stopping.

```python
filename = 'settings.txt'
try:
    file_handle = open(filename, 'r')
    print(file_handle.read())
    file_handle.close()
except FileNotFoundError:
    print('Could not find', filename, '- using default settings')
```

```text
Could not find settings.txt - using default settings
```

- Code after the `try`/`except` carries on as normal.
- You can name the exception with `as` to use its message:

```python
text = 'twelve'
try:
    number = float(text)
except ValueError as error:
    print('Not a number:', error)
    number = 0.0
print(number)
```

```text
Not a number: could not convert string to float: 'twelve'
0.0
```

## Catch specific exceptions, not everything

- Python lets you write `except:` with no exception type. This is called a _bare except_, and it catches **every** exception.
- That includes the bugs in your own code that you'd want to know about.

```python nolint
values = ['1.5', '2.0', 'oops']
total = 0.0
for value in values:
    try:
        total = total + flaot(value)
    except:
        print('Skipping', value)
print(total)
```

```text
Skipping 1.5
Skipping 2.0
Skipping oops
0.0
```

- `flaot` is a typo, so every value raises a `NameError`, and the bare `except:` hides it. The program runs to the end and prints a wrong answer without complaint.
- With `except ValueError:` instead, the `NameError` would stop the program with a traceback pointing straight at the typo.
- So catch only the exceptions you expect and know how to handle. Let everything else stop the program.

::::challenge{id="raising_for_bad_input" title="Raising for Bad Input"}

Fill in the blanks so that `mean` raises a `ValueError` with a helpful message when it is given an empty list,
instead of failing with a `ZeroDivisionError`.

```python nolint
def mean(values):
    if ____:
        ____ ValueError(____)
    return sum(values) / len(values)

print(mean([1, 2, 3]))
print(mean([]))
```

:::solution

```python
def mean(values):
    if len(values) == 0:
        raise ValueError('Cannot take the mean of an empty list')
    return sum(values) / len(values)
```

A `ZeroDivisionError` tells the caller what went wrong inside `mean`, not what _they_ did wrong.
The `ValueError` message describes the problem in terms of the input.

:::
::::

::::challenge{id="skipping_bad_values" title="Skipping Bad Values"}

A list of measurements has been read in as strings, and some entries are not numbers.
Write a loop that converts each entry to a `float` and adds it to `numbers`.
It should print a message for each entry it can't convert and skip that entry.

```python
readings = ['3.2', '4.8', 'n/a', '5.1', '', '2.9']
numbers = []
```

Which exception should you catch? What would go wrong if you used a bare `except:`?

:::solution

```python
readings = ['3.2', '4.8', 'n/a', '5.1', '', '2.9']
numbers = []
for reading in readings:
    try:
        numbers.append(float(reading))
    except ValueError:
        print('Skipping', repr(reading))
print(numbers)
```

```text
Skipping 'n/a'
Skipping ''
[3.2, 4.8, 5.1, 2.9]
```

`float` raises a `ValueError` for text it can't convert, so catch that.
A bare `except:` would also hide mistakes such as a misspelled variable name, and give you a silently wrong `numbers` list.

:::
::::
