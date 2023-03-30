---
name: Polymorphism
dependsOn: [
    software_architecture_and_design.object_orientated.inheritance_and_composition,
]
tags: [python]
---

## Class-based polymorphism

Class-based polymorphism is a fundamental concept in object-oriented programming
that allows objects of different classes to be treated as if they were the same
type, while still maintaining their individual differences. In other words,
polymorphism enables objects to be manipulated in a generic way, irrespective of
their specific class or implementation

As an example, lets return to our `Patient` and `Doctor` classes from the
previous section. We wish to have some common functionality for both types of
people, but have this functionality customized for each type. In particular we
want to be able to:

1. Print a more useful string representation of the object, for example `Doctor: Alice` or `Patient: Bob`
2. Set a unique identifier to each person, so that each doctor has an id of the form `D0001`, and each patient has an id of the form `P0001`

We can implement this in our classes like so:

~~~ python
...
class Person:
    """A person."""
    def __init__(self, name):
        self.name = name
        self.id = None

    def __str__(self):
        return self.name

    def set_id(self, id):
      raise NotImplementedError('set_id not implemented')

    def get_id(self):
        return self.id

class Patient(Person):
    """A patient in an inflammation study."""
    def __init__(self, name):
        super().__init__(name)

    def __str__(self):
        return 'Patient: ' + super().__str__()

    def set_id(self, id):
        self.id = 'P' + str(id).zfill(4)


class Doctor(Person):
    """A doctor in an inflammation study."""
    def __init__(self, name):
        super().__init__(name)

    def __str__(self):
        return 'Doctor: ' + super().__str__()

    def set_id(self, id):
        self.id = 'D' + str(id).zfill(4)
...
~~~

Here we have defined the **interface** for our `Person` class, which is that
there should be a `set_id` method. We have also defined the `__str__` method,
which is a special method that is called when you do `str(object)` or
`print(object)`. We have also defined the `__str__` method for the `Patient` and
`Doctor` classes, which override the `__str__` method of the `Person` class.
This is called **method overriding**.

Notice the two different methods we have implemented the `set_id` and `__str__`
methods in the base class. For the `set_id` method we have raised a
`NotImplementedError`, which is a special type of error that is used to indicate
that a method has not been implemented. This is a good way to ensure that the
interface is implemented correctly. For the `__str__` method we have used a
default implementation that a derived class can choose to override or not. In
this case the derived classes both override the `__str__` method and provide
their own implementation, calling the `__str__` method of the base class to get
the name of the person. This is called **method chaining**.

Now we can use our classes like so:

```python
alice = Doctor('Alice')
bob = Patient('Bob')
alice.set_id(1)
bob.set_id(1)
print(alice)
print(bob)
```

```
Doctor: Alice
Patient: Bob
```


We can also store collections of different types of people in a single list:

```python
people = [alice, bob]
for person in people:
    print(person)
```

```
Doctor: Alice
Patient: Bob
```

## Duck typing

The dynamic typing system in Python is very flexible compared with many other
langauges. Duck typing is a concept that is often used in Python, which is that
if an object has the right methods, it can be treated as if it is of a
particular type. Using our example above, if an object has a `set_id` and
`__str__` method, it can be treated as if it is a `Person` object. For example,


```python
class Administrator:
    """An administrator in an inflammation study."""
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return 'Administrator: ' + self.name

    def set_id(self, id):
        self.id = 'A' + str(id).zfill(4)

    def get_id(self):
        return self.id
```

The quote below is from the [Python documentation](https://docs.python.org/3/glossary.html#term-duck-typing):

> A programming style which does not look at an object’s type to determine if it
> has the right interface; instead, the method or attribute is simply called or
> used (“If it looks like a duck and quacks like a duck, it must be a duck.”) By
> emphasizing interfaces rather than specific types, well-designed code improves
> its flexibility by allowing polymorphic substitution.

::::challenge{id="create-a-trial" title="A Trial full of People"}

Create a class called `Trial` that has a list of people assigned to it. The
`Trial` class should have a method called `add_person` that adds a person to the
room. The `Trial` class should also have a method called `print_people` that
prints the names of all the people in the room, and a `set_ids` method that
assigns unique ids for each person in the trial.

We also want our interface to be flexible in terms of the input to the
`add_person` method. We will accept any type of person, as long as it has a
`get_id()` method. If the method cannot recognise the input as a `Person`
object, it should fall-back on using the id of the person obtained by the
`get_id()` method, using the person id as their name. For example, if the id of
the person is `D0001`, it should add a `Doctor` object to the trial with name
`D0001`.

Note: you can use the `isinstance` function to check if an object is of a
particular type. You can also use the `hasattr` function to check if an object
has a particular attribute (or method).

:::solution

```python
class Trial:
    """A trial full of People."""
    def __init__(self):
        self.people = []

    def add_person(self, person):
        if isinstance(person, Person):
            self.people.append(person)
        elif hasattr(person, 'get_id'):
            person_id = person.get_id()
            if person_id[0] == 'D':
                person = Doctor(person_id)
            elif person_id[0] == 'P':
                person = Patient(person_id)
            elif person_id[0] == 'A':
                person = Administrator(person_id)
            else:
                raise TypeError('could not recognise person id, should start with D, P or A')
            self.people.append(person)
        else:
            raise TypeError('person must be a Person object or have a get_id method')

    def print_people(self):
        for person in self.people:
            print(person)

    def set_ids(self):
        for i, person in enumerate(self.people):
            person.set_id(i + 1)
```

:::
::::