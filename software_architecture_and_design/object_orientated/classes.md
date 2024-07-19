---
name: Classes
dependsOn: []
tags: [python]
learningOutcomes:
  - Explain the object orientated programming paradigm.
  - Define a class to encapsulate data.
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

## Structuring Data

One of the main difficulties we encounter when building more complex software is how to structure our data.
So far, we've been processing data from a single source and with a simple tabular structure, but it would be useful to be able to combine data from a range of different sources and with more data than just an array of numbers.

```python
import numpy as np
data = np.array([[1., 2., 3.],
                 [4., 5., 6.]])
```

Using this data structure has the advantage of being able to use NumPy operations to process the data and Matplotlib to plot it, but often we need to have more structure than this.
For example, we may need to attach more information about the patients and store this alongside our measurements of inflammation.

We can do this using the Python data structures we're already familiar with, dictionaries and lists.
For instance, say we wish to store a list of patients on a clinical inflammation trial. We could attach a name to each of our patients:

```python
patients = [
    {
        'name': 'Alice',
        'data': [1., 2., 3.],
    },
    {
        'name': 'Bob',
        'data': [4., 5., 6.],
    },
]
```

::::challenge{id=structuring-data title="Structuring Data"}

Write a function, called `attach_names`, which can be used to attach names to our patient dataset.
When used as below, it should produce the expected output.

If you're not sure where to begin, think about ways you might be able to effectively loop over two collections at once.
Also, don't worry too much about the data type of the `data` value, it can be a Python list, or a NumPy array - either is fine.

```python nolint
data = np.array([[1., 2., 3.],
                 [4., 5., 6.]])

output = attach_names(data, ['Alice', 'Bob'])
print(output)
```

```text
[
    {
        'name': 'Alice',
        'data': [1., 2., 3.],
    },
    {
        'name': 'Bob',
        'data': [4., 5., 6.],
    },
]
```

:::solution

One possible solution, perhaps the most obvious, is to use the `range` function to index into both lists at the same location:

```python
def attach_names(data, names):
    """Create datastructure containing patient records."""
    output = []

    for i in range(len(data)):
        output.append({'name': names[i],
                       'data': data[i]})

    return output
```

However, this solution has a potential problem that can occur sometimes, depending on the input.
What might go wrong with this solution? How could we fix it?

:::

:::solution

What would happen if the `data` and `names` inputs were different lengths?

If `names` is longer, we'll loop through, until we run out of rows in the `data` input, at which point we'll stop processing the last few names.
If `data` is longer, we'll loop through, but at some point we'll run out of names - but this time we try to access part of the list that doesn't exist, so we'll get an exception.

A better solution would be to use the `zip` function, which allows us to iterate over multiple iterables without needing an index variable.
The `zip` function also limits the iteration to whichever of the iterables is smaller, so we won't raise an exception here, but this might not quite be the behaviour we want, so we'll also explicitly `assert` that the inputs should be the same length.
Checking that our inputs are valid in this way is an example of a precondition, which we introduced conceptually in an earlier episode.

If you've not previously come across the `zip` function, read [this section](https://docs.python.org/3/library/functions.html#zip) of the Python documentation.

```python
def attach_names(data, names):
    """Create datastructure containing patient records."""
    assert len(data) == len(names)
    output = []

    for data_row, name in zip(data, names):
        output.append({'name': name,
                       'data': data_row})

    return output
```

:::

::::

## Classes in Python

Using nested dictionaries and lists should work for some of the simpler cases
where we need to handle structured data, but they get quite difficult to manage
once the structure becomes a bit more complex. For this reason, in the object
oriented paradigm, we use **classes** to help with managing this data and the
operations we would want to perform on it. A class is a **template**
(blueprint) for a structured piece of data, so when we create some data using a
class, we can be certain that it has the same structure each time.

With our list of dictionaries we had in the example above, we have no real
guarantee that each dictionary has the same structure, e.g. the same keys
(`name` and `data`) unless we check it manually. With a class, if an object is
an **instance** of that class (i.e. it was made using that template), we know it
will have the structure defined by that class. Different programming languages
make slightly different guarantees about how strictly the structure will match,
but in object oriented programming this is one of the core ideas - all objects
derived from the same class must follow the same behaviour.

You may not have realised, but you should already be familiar with some of the
classes that come bundled as part of Python, for example:

```python
my_list = [1, 2, 3]
my_dict = {1: '1', 2: '2', 3: '3'}
my_set = {1, 2, 3}

print(type(my_list))
print(type(my_dict))
print(type(my_set))
```

```text
<class 'list'>
<class 'dict'>
<class 'set'>
```

Lists, dictionaries and sets are a slightly special type of class, but they behave in much the same way as a class we might define ourselves:

- They each hold some data (**attributes** or **state**).
- They also provide some methods describing the behaviours of the data - what can the data do and what can we do to the data?

The behaviours we may have seen previously include:

- Lists can be appended to
- Lists can be indexed
- Lists can be sliced
- Key-value pairs can be added to dictionaries
- The value at a key can be looked up in a dictionary
- The union of two sets can be found (the set of values present in any of the sets)
- The intersection of two sets can be found (the set of values present in all of the sets)

## Encapsulating Data

Let's start with a minimal example of a class representing our patients.

```python
# file: inflammation/models.py

class Patient:
    def __init__(self, name):
        self.name = name
        self.observations = []

alice = Patient('Alice')
print(alice.name)
```

```text
Alice
```

Here we've defined a class with one method: `__init__`. This method is the
**initialiser** method, which is responsible for setting up the initial values
and structure of the data inside a new instance of the class - this is very
similar to **constructors** in other languages, so the term is often used in
Python too. The `__init__` method is called every time we create a new instance
of the class, as in `Patient('Alice')`. The argument `self` refers to the
instance on which we are calling the method and gets filled in automatically by
Python - we do not need to provide a value for this when we call the method.

Data encapsulated within our Patient class includes the patient's name and a
list of inflammation observations. In the initialiser method, we set a patient's
name to the value provided, and create a list of inflammation observations for
the patient (initially empty). Such data is also referred to as the attributes
of a class and holds the current state of an instance of the class. Attributes
are typically hidden (encapsulated) internal object details ensuring that access
to data is protected from unintended changes. They are manipulated internally by
the class, which, in addition, can expose certain functionality as public
behavior of the class to allow other objects to interact with this class'
instances.

## Encapsulating Behaviour

In addition to representing a piece of structured data (e.g. a patient who has a
name and a list of inflammation observations), a class can also provide a set of
functions, or **methods**, which describe the **behaviours** of the data
encapsulated in the instances of that class. To define the behaviour of a class
we add functions which operate on the data the class contains. These functions
are the member functions or methods.

Methods on classes are the same as normal functions, except that they live
inside a class and have an extra first parameter `self`. Using the name `self`
is not strictly necessary, but is a very strong convention - it is extremely
rare to see any other name chosen. When we call a method on an object, the
value of `self` is automatically set to this object - hence the name. As we saw
with the `__init__` method previously, we do not need to explicitly provide a
value for the `self` argument, this is done for us by Python.

Let's add another method on our Patient class that adds a new observation to a Patient instance.

```python
# file: inflammation/models.py

class Patient:
    """A patient in an inflammation study."""
    def __init__(self, name):
        self.name = name
        self.observations = []

    def add_observation(self, value, day=None):
        if day is None:
            try:
                day = self.observations[-1]['day'] + 1

            except IndexError:
                day = 0

        new_observation = {
            'day': day,
            'value': value,
        }

        self.observations.append(new_observation)
        return new_observation

alice = Patient('Alice')
print(alice)

observation = alice.add_observation(3)
print(observation)
print(alice.observations)
```

```text
<__main__.Patient object at 0x7fd7e61b73d0>
{'day': 0, 'value': 3}
[{'day': 0, 'value': 3}]
```

Note also how we used `day=None` in the parameter list of the `add_observation` method, then initialise it if the value is indeed `None`.
This is one of the common ways to handle an optional argument in Python, so we'll see this pattern quite a lot in real projects.

:::callout

## Class and Static Methods

Sometimes, the function we're writing doesn't need access to any data belonging to a particular object.
For these situations, we can instead use a **class method** or a **static method**.
Class methods have access to the class that they're a part of, and can access data on that class - but do not belong to a specific instance of that class, whereas static methods have access to neither the class nor its instances.

By convention, class methods use `cls` as their first argument instead of `self` - this is how we access the class and its data, just like `self` allows us to access the instance and its data.
Static methods have neither `self` nor `cls` so the arguments look like a typical free function.
These are the only common exceptions to using `self` for a method's first argument.

Both of these method types are created using **decorators** - for more information see the [classmethod](https://docs.python.org/3/library/functions.html#classmethod) and [staticmethod](https://docs.python.org/3/library/functions.html#staticmethod) decorator sections of the Python documentation.
:::

### Dunder Methods

Why is the `__init__` method not called `init`?
There are a few special method names that we can use which Python will use to provide a few common behaviours, each of which begins and ends with a **d**ouble-**under**score, hence the name **dunder method**.

When writing your own Python classes, you'll almost always want to write an `__init__` method, but there are a few other common ones you might need sometimes. You may have noticed in the code above that the method `print(alice)` returned `<__main__.Patient object at 0x7fd7e61b73d0>`, which is the string represenation of the `alice` object. We
may want the print statement to display the object's name instead. We can achieve this by overriding the `__str__` method of our class.

```python
# file: inflammation/models.py

class Patient:
    """A patient in an inflammation study."""
    def __init__(self, name):
        self.name = name
        self.observations = []

    def add_observation(self, value, day=None):
        if day is None:
            try:
                day = self.observations[-1]['day'] + 1

            except IndexError:
                day = 0


        new_observation = {
            'day': day,
            'value': value,
        }

        self.observations.append(new_observation)
        return new_observation

    def __str__(self):
        return self.name


alice = Patient('Alice')
print(alice)
```

```text
Alice
```

These dunder methods are not usually called directly, but rather provide the implementation of some functionality we can use - we didn't call `alice.__str__()`, but it was called for us when we did `print(alice)`.
Some we see quite commonly are:

- `__str__` - converts an object into its string representation, used when you call `str(object)` or `print(object)`
- `__getitem__` - Accesses an object by key, this is how `list[x]` and `dict[x]` are implemented
- `__len__` - gets the length of an object when we use `len(object)` - usually the number of items it contains

There are many more described in the Python documentation, but it’s also worth experimenting with built in Python objects to see which methods provide which behaviour.
For a more complete list of these special methods, see the [Special Method Names](https://docs.python.org/3/reference/datamodel.html#special-method-names) section of the Python documentation.

::::challenge{id="a-basic-class" title="A Basic Class"}

Implement a class to represent a book.
Your class should:

- Have a title
- Have an author
- When printed using `print(book)`, show text in the format "title by author"

```python nolint
book = Book('A Book', 'Me')

print(book)
```

```text
A Book by Me
```

:::solution

```python
class Book:
    def __init__(self, title, author):
        self.title = title
        self.author = author

    def __str__(self):
        return self.title + ' by ' + self.author
```

:::
::::

### Properties

The final special type of method we will introduce is a **property**.
Properties are methods which behave like data - when we want to access them, we do not need to use brackets to call the method manually.

```python
# file: inflammation/models.py

class Patient:
    ...

    @property
    def last_observation(self):
        return self.observations[-1]

alice = Patient('Alice')

alice.add_observation(3)
alice.add_observation(4)

obs = alice.last_observation
print(obs)
```

```text
{'day': 1, 'value': 4}
```

You may recognise the `@` syntax from episodes on functional programming -
`property` is another example of a **decorator**. In this case the `property`
decorator is taking the `last_observation` function and modifying its behaviour,
so it can be accessed as if it were a normal attribute. It is also possible to
make your own decorators, but we won't cover it here.

## Key Points

- Object oriented programming is a programming paradigm based on the concept of classes, which encapsulate data and code.
- Classes allow us to organise data into distinct concepts.
- By breaking down our data into classes, we can reason about the behaviour of parts of our data.
