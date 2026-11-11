---
name: Tuples and Dictionaries
dependsOn: [introductory_courses.python.11_handling_exceptions]
tags: [python]
learningOutcomes:
  - Creating and indexing tuples, and explaining how they differ from lists.
  - Storing and looking up values by key in a dictionary.
  - Explaining why dictionary keys must be immutable.
  - Copying lists safely, including nested lists.
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

## A tuple is a sequence that can't be changed

- [Lists](05_lists) are not Python's only container type.
- A _tuple_ is an ordered sequence, like a list, but it is _immutable_: once created, it can't be changed.
- Write a tuple with round brackets instead of square brackets.
- Indexing and slicing work just as they do for lists and strings.

```python
fruits = ('Apple', 'Banana', 'Cherry')
print(fruits[1])
print(type(fruits))
```

```text
Banana
<class 'tuple'>
```

- It's the comma that makes a tuple, not the brackets, so a tuple with one element needs a trailing comma.

```python
single = ('Apple',)
not_a_tuple = ('Apple')
print(type(single), type(not_a_tuple))
```

```text
<class 'tuple'> <class 'str'>
```

- Trying to change an element raises a `TypeError`:

```python
greeting = ('Hello', 'World')
greeting[0] = 'Goodbye'
```

```text
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: 'tuple' object does not support item assignment
```

- Use a tuple for a fixed group of values that belong together, such as a coordinate `(x, y)` or a date `(year, month, day)`.
- Functions often use a tuple to return more than one value, which the caller can _unpack_ into separate variables:

```python
def min_max(values):
    return min(values), max(values)

smallest, largest = min_max([3, 1, 4, 1, 5])
print(smallest, largest)
```

```text
1 5
```

## A dictionary looks values up by key

- In a list, we look up an element by its position, a number.
- In a _dictionary_ (`dict`), we look up a _value_ by a _key_ of our choice, often a string.
- Write a dictionary with curly brackets and `key: value` pairs, separated by commas.

```python
person = {'name': 'Ada', 'age': 36, 'jobs': ['Mathematician', 'Writer']}
print(person['name'])
print(person['jobs'])
print(type(person))
```

```text
Ada
['Mathematician', 'Writer']
<class 'dict'>
```

- Dictionaries are mutable: assign to a key to add a new entry or replace an existing one.

```python
person['age'] = 37
person['country'] = 'UK'
print(person)
```

```text
{'name': 'Ada', 'age': 37, 'jobs': ['Mathematician', 'Writer'], 'country': 'UK'}
```

- Looking up a key that isn't there raises a `KeyError`. You saw one in the "Reading Error Messages" challenge in [Errors and Exceptions](08_errors_and_exceptions).
- Use `.get` to supply a default instead:

```python
print(person.get('email', 'no email given'))
```

```text
no email given
```

- Dictionaries remember the order in which keys were added (since Python 3.7).

## Keys and values can be listed and tested separately

- `.keys()` gives the keys and `.values()` gives the values.
- `.items()` gives `(key, value)` tuples, which is the usual way to loop over a dictionary.

```python
scores = {'Ada': 92, 'Grace': 88, 'Alan': 75}
print(list(scores.keys()))
print(list(scores.values()))

for name, score in scores.items():
    print(name, 'scored', score)
```

```text
['Ada', 'Grace', 'Alan']
[92, 88, 75]
Ada scored 92
Grace scored 88
Alan scored 75
```

- `in` tests the _keys_ of a dictionary, not its values:

```python
print('Ada' in scores)
print(92 in scores)
print(92 in scores.values())
```

```text
True
False
True
```

## Dictionary keys must be immutable

- Dictionaries use a technique called _hashing_ to find keys quickly.
- This only works if a key can't change after it has been stored, so keys must be immutable: numbers, strings and tuples can be keys, but lists can't.

```python
good_match = {
    ('Lamb', 'Mint'): True,
    ('Bacon', 'Chocolate'): False,
}
print(good_match[('Lamb', 'Mint')])
```

```text
True
```

```python nolint
illegal = {
    ['Lamb', 'Mint']: True,
    ['Bacon', 'Chocolate']: False,
}
```

```text
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: unhashable type: 'list'
```

- Remember: square brackets make lists, round brackets make tuples.

## Copying a container needs care

- In [Lists](05_lists) we saw that `primes = odds` doesn't copy a list. It gives the _same_ list a second name.
  Changing the list through either name changes it for both.
- This is true of every mutable container, including dictionaries.
- To copy a list, use `list(odds)` or the slice `odds[:]`. For a dictionary, use `dict(person)` or `person.copy()`.

```python
x = [1, 2, 3]
y = x[:]
y[1] = 20
print(x, y)
```

```text
[1, 2, 3] [1, 20, 3]
```

- These are _shallow_ copies: they make a new outer container, but the elements inside are shared.
- That matters when the container holds other mutable containers, like a list of lists:

```python
x = [['a', 'b'], 'c']
y = x
z = x[:]

x[0][1] = 'd'
z[1] = 'e'

print(x)
print(y)
print(z)
```

```text
[['a', 'd'], 'c']
[['a', 'd'], 'c']
[['a', 'd'], 'e']
```

- `y` is just another name for `x`, so it sees every change.
- `z` is a new list, so changing `z[1]` doesn't affect `x`. But `z[0]` is the same inner list as `x[0]`, so it sees the change to `x[0][1]`.
- To copy everything, however deeply nested, use `deepcopy` from the standard library's `copy` module:

```python
from copy import deepcopy

x = [['a', 'b'], 'c']
z = deepcopy(x)
x[0][1] = 'd'
print(x)
print(z)
```

```text
[['a', 'd'], 'c']
[['a', 'b'], 'c']
```

- You'll learn more about `import` in [Libraries](13_libraries).

::::challenge{id="counting_words" title="Counting Words"}

Fill in the blanks so that `counts` records how many times each word appears in `words`.

```python nolint
words = ['red', 'blue', 'red', 'green', 'blue', 'red']
counts = {}
for word in words:
    if word ____ counts:
        counts[word] = ____
    else:
        counts[word] = ____
print(counts)
```

```text
{'red': 3, 'blue': 2, 'green': 1}
```

:::solution

```python
words = ['red', 'blue', 'red', 'green', 'blue', 'red']
counts = {}
for word in words:
    if word in counts:
        counts[word] = counts[word] + 1
    else:
        counts[word] = 1
print(counts)
```

Using `.get`, the loop body can be a single line: `counts[word] = counts.get(word, 0) + 1`.

:::
::::

::::challenge{id="copying_a_grid" title="Copying a Grid"}

What does this program print, and why?
How would you change it so that `grid` is not affected?

```python
grid = [[0, 0], [0, 0]]
backup = grid[:]
backup[0][0] = 1
backup[1] = [9, 9]
print(grid)
```

:::solution

```text
[[1, 0], [0, 0]]
```

`grid[:]` makes a shallow copy. `backup` is a new outer list, so replacing `backup[1]` doesn't affect `grid`,
but `backup[0]` is the same inner list as `grid[0]`, so `backup[0][0] = 1` changes `grid` too.
Use `backup = deepcopy(grid)` (after `from copy import deepcopy`) to copy the inner lists as well.

:::
::::
