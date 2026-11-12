---
name: Lists
dependsOn: [introductory_courses.python.07_pandas_dataframes]
tags: [python]
learningOutcomes:
  - Storing and accessing values in lists
attribution:
  - citation: >
      "Programming with Python" course by the Carpentries
    url: https://swcarpentry.github.io/python-novice-inflammation/
    image: https://carpentries.org/carpentries-logo.svg
    license: CC-BY-4.0
---

In Python, a list is a way to store multiple values together.
In this episode, we will learn how to store multiple values in a list as well as how to work with lists.

## Python lists

Unlike NumPy arrays, lists are built into the language so we do not have to load a library to use them.
We create a list by putting values inside square brackets and separating the values with commas:

```python
odds = [1, 3, 5, 7]
print('odds are:', odds)
```

```text
odds are: [1, 3, 5, 7]
```

We can access elements of a list using indices -- numbered positions of elements in the list.
These positions are numbered starting at 0, so the first element has an index of 0.

```python
print('first element:', odds[0])
print('last element:', odds[3])
print('"-1" element:', odds[-1])
```

```text
first element: 1
last element: 7
"-1" element: 7
```

Yes, we can use negative numbers as indices in Python. When we do so, the index `-1` gives us the
last element in the list, `-2` the second to last, and so on.
Because of this, `odds[3]` and `odds[-1]` point to the same element here.

There is one important difference between lists and strings:
we can change the values in a list,
but we cannot change individual characters in a string.
For example:

```python
names = ['Curie', 'Darwing', 'Turing']  # typo in Darwin's name
print('names is originally:', names)
names[1] = 'Darwin'  # correct the name
print('final value of names:', names)
```

```text
names is originally: ['Curie', 'Darwing', 'Turing']
final value of names: ['Curie', 'Darwin', 'Turing']
```

works, but:

```python
name = 'Darwin'
name[0] = 'd'
```

```text
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-8-220df48aeb2ein <module>()
      1 name = 'Darwin'
----2 name[0] = 'd'

TypeError: 'str' object does not support item assignment
```

does not.

:::callout

## Ch-Ch-Ch-Ch-Changes

Data which can be modified in place is called mutable (can be altered after creation), while data which cannot be modified is called immutable (cannot be altered after creation).
Strings and numbers are immutable. This does not mean that variables with string or number values
are constants, but when we want to change the value of a string or number variable, we can only
replace the old value with a completely new value.

Lists and arrays, on the other hand, are mutable: we can modify them after they have been
created. We can change individual elements, append new elements, or reorder the whole list. For
some operations, like sorting, we can choose whether to use a function that modifies the data
in-place or a function that returns a modified copy and leaves the original unchanged.

Be careful when modifying data in-place. If two variables refer to the same list, and you modify
the list value, it will change for both variables!

```python
salsa = ['peppers', 'onions', 'cilantro', 'tomatoes']
my_salsa = salsa        # <-- my_salsa and salsa point to the *same* list data in memory
salsa[0] = 'hot peppers'
print('Ingredients in my salsa:', my_salsa)
```

```text
Ingredients in my salsa: ['hot peppers', 'onions', 'cilantro', 'tomatoes']
```

If you want variables with mutable values to be independent, you
must make a copy of the value when you assign it.

```python
salsa = ['peppers', 'onions', 'cilantro', 'tomatoes']
my_salsa = list(salsa)        # <-- makes a *copy* of the list
salsa[0] = 'hot peppers'
print('Ingredients in my salsa:', my_salsa)
```

```text
Ingredients in my salsa: ['peppers', 'onions', 'cilantro', 'tomatoes']
```

Because of pitfalls like this, code which modifies data in place can be more difficult to
understand. However, it is often far more efficient to modify a large data structure in place
than to create a modified copy for every small change. You should consider both of these aspects
when writing your code.
:::

:::callout

## Nested Lists

Since a list can contain any Python variables, it can even contain other lists.

For example, we could represent the products in the shelves of a small grocery shop:

```python
x = [['pepper', 'zucchini', 'onion'],
     ['cabbage', 'lettuce', 'garlic'],
     ['apple', 'pear', 'banana']]
```

Here is a visual example of how indexing a list of lists `x` works:

x is represented as a pepper shaker containing several packets of pepper. `[x[0]]` is represented as a pepper shaker containing a single packet of pepper. `x[0]` is represented as a single packet of pepper. `x[0][0]` is represented as single grain of pepper. [Adapted from @hadleywickham: indexing lists in python](./fig/indexing_lists_python.png)

Using the previously declared list `x`, these would be the results of the
index operations shown in the image:

```python
print([x[0]])
```

```text
[['pepper', 'zucchini', 'onion']]
```

```python
print(x[0])
```

```text
['pepper', 'zucchini', 'onion']
```

```python
print(x[0][0])
```

```text
'pepper'
```

:::

:::callout

## Heterogeneous Lists

Lists in Python can contain elements of different types. Example:

```python
sample_ages = [10, 12.5, 'Unknown']
```

:::

There are many ways to change the contents of lists besides assigning new values to
individual elements:

```python
odds.append(11)
print('odds after adding a value:', odds)
```

```text
odds after adding a value: [1, 3, 5, 7, 11]
```

```python
removed_element = odds.pop(0)
print('odds after removing the first element:', odds)
print('removed_element:', removed_element)
```

```text
odds after removing the first element: [3, 5, 7, 11]
removed_element: 1
```

```python
odds.reverse()
print('odds after reversing:', odds)
```

```text
odds after reversing: [11, 7, 5, 3]
```

While modifying in place, it is useful to remember that Python treats lists in a slightly
counter-intuitive way.

As we saw earlier, when we modified the `salsa` list item in-place, if we make a list, (attempt to)
copy it and then modify this list, we can cause all sorts of trouble. This also applies to modifying
the list using the above functions:

```python
odds = [1, 3, 5, 7]
primes = odds
primes.append(2)
print('primes:', primes)
print('odds:', odds)
```

```text
primes: [1, 3, 5, 7, 2]
odds: [1, 3, 5, 7, 2]
```

This is because Python stores a list in memory, and then can use multiple names to refer to the
same list. If all we want to do is copy a (simple) list, we can again use the `list` function, so we
do not modify a list we did not mean to:

```python
odds = [1, 3, 5, 7]
primes = list(odds)
primes.append(2)
print('primes:', primes)
print('odds:', odds)
```

```text
primes: [1, 3, 5, 7, 2]
odds: [1, 3, 5, 7]
```

Subsets of lists and strings can be accessed by specifying ranges of values in brackets,
similar to how we accessed ranges of positions in a NumPy array.
This is commonly referred to as "slicing" the list/string.

```python
binomial_name = 'Drosophila melanogaster'
group = binomial_name[0:10]
print('group:', group)

species = binomial_name[11:23]
print('species:', species)

chromosomes = ['X', 'Y', '2', '3', '4']
autosomes = chromosomes[2:5]
print('autosomes:', autosomes)

last = chromosomes[-1]
print('last:', last)
```

```text
group: Drosophila
species: melanogaster
autosomes: ['2', '3', '4']
last: 4
```

You can also select non-consecutive elements from a list by slicing with a step size, for example

```python
numbers = [1, 2, 3, 4, 5, 6, 7, 8]
odd_numbers = numbers[0:6:2]
print('odd numbers:', odd_numbers)
```

```text
odd numbers: [1, 3, 5]
```

In the above example, `numbers[0:2:7]` tells python to slice the list
`numbers` from element `0` (`1`) to element `6` (`7`), exluded with a
step of elements.

::::challenge{id="slicing_from_the_end" title="Slicing From the End"}

Use slicing to access only the last four characters of a string or entries of a list.

```python
string_for_slicing = 'Observation date: 02-Feb-2013'
list_for_slicing = [['fluorine', 'F'],
                    ['chlorine', 'Cl'],
                    ['bromine', 'Br'],
                    ['iodine', 'I'],
                    ['astatine', 'At']]
```

```text
'2013'
[['chlorine', 'Cl'], ['bromine', 'Br'], ['iodine', 'I'], ['astatine', 'At']]
```

Would your solution work regardless of whether you knew beforehand
the length of the string or list
(e.g. if you wanted to apply the solution to a set of lists of different lengths)?
If not, try to change your approach to make it more robust.

Hint: Remember that indices can be negative as well as positive

:::solution
Use negative indices to count elements from the end of a container (such as list or string):

```python
string_for_slicing[-4:]
list_for_slicing[-4:]
```

:::
::::

If you want to take a slice from the beginning of a sequence, you can omit the first index in the
range:

```python
date = 'Monday 4 January 2016'
day = date[0:6]
print('Using 0 to begin range:', day)
day = date[:6]
print('Omitting beginning index:', day)
```

```text
Using 0 to begin range: Monday
Omitting beginning index: Monday
```

And similarly, you can omit the ending index in the range to take a slice to the very end of the
sequence:

```python
months = ['jan', 'feb', 'mar', 'apr', 'may', 'jun', 'jul', 'aug', 'sep', 'oct', 'nov', 'dec']
sond = months[8:12]
print('With known last position:', sond)
sond = months[8:len(months)]
print('Using len() to get last entry:', sond)
sond = months[8:]
print('Omitting ending index:', sond)
```

```text
With known last position: ['sep', 'oct', 'nov', 'dec']
Using len() to get last entry: ['sep', 'oct', 'nov', 'dec']
Omitting ending index: ['sep', 'oct', 'nov', 'dec']
```

::::challenge{id="overloading" title="Overloading"}

`+` usually means addition, but when used on strings or lists, it means "concatenate".
Given that, what do you think the multiplication operator `*` does on lists?
In particular, what will be the output of the following code?

```python
counts = [2, 4, 6, 8, 10]
repeats = counts * 2
print(repeats)
```

1. `[2, 4, 6, 8, 10, 2, 4, 6, 8, 10]`
2. `[4, 8, 12, 16, 20]`
3. `[[2, 4, 6, 8, 10],[2, 4, 6, 8, 10]]`
4. `[2, 4, 6, 8, 10, 4, 8, 12, 16, 20]`

The technical term for this is _operator overloading_:
a single operator, like `+` or `*`,
can do different things depending on what it's applied to.

:::solution

The multiplication operator `*` used on a list replicates elements of the list and concatenates
them together:

```text
[2, 4, 6, 8, 10, 2, 4, 6, 8, 10]
```

It's equivalent to:

```python
counts + counts
```

:::
::::
