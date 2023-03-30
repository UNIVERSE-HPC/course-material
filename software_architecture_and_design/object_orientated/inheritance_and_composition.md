---
name: Inheritance and Composition
dependsOn: [
    software_architecture_and_design.object_orientated.classes,
]
tags: [python]
---

## Relationships Between Classes

We now have a language construct for grouping data and behaviour related to a single conceptual object.
The next step we need to take is to describe the relationships between the concepts in our code.

There are two fundamental types of relationship between objects which we need to be able to describe:

1. Ownership - x **has a** y - this is **composition**
2. Identity - x **is a** y - this is **inheritance**

### Composition

You should hopefully have come across the term **composition** already - in the novice Software Carpentry, we use composition of functions to reduce code duplication.
That time, we used a function which converted temperatures in Celsius to Kelvin as a **component** of another function which converted temperatures in Fahrenheit to Kelvin.

In the same way, in object oriented programming, we can make things components of other things.

We often use composition where we can say 'x *has a* y' - for example in our inflammation project, we might want to say that a doctor *has* patients or that a patient *has* observations.

In the case of our example, we're already saying that patients have observations, so we're already using composition here.
We're currently implementing an observation as a dictionary with a known set of keys though, so maybe we should make an `Observation` class as well.

~~~ python
# file: inflammation/models.py

class Observation:
    def __init__(self, day, value):
        self.day = day
        self.value = value

    def __str__(self):
        return str(self.value)

class Patient:
    """A patient in an inflammation study."""
    def __init__(self, name):
        self.name = name
        self.observations = []

    def add_observation(self, value, day=None):
        if day is None:
            try:
                day = self.observations[-1].day + 1

            except IndexError:
                day = 0

        new_observation = Observation(day, value)

        self.observations.append(new_observation)
        return new_observation

    def __str__(self):
        return self.name


alice = Patient('Alice')
obs = alice.add_observation(3)

print(obs)
~~~

~~~
3
~~~

Now we're using a composition of two custom classes to describe the relationship between two types of entity in the system that we're modelling.

### Inheritance

The other type of relationship used in object oriented programming is **inheritance**.
Inheritance is about data and behaviour shared by classes, because they have some shared identity - 'x *is a* y'.
If class `X` inherits from (*is a*) class `Y`, we say that `Y` is the **superclass** or **parent class** of `X`, or `X` is a **subclass** of `Y`.

If we want to extend the previous example to also manage people who aren't patients we can add another class `Person`.
But `Person` will share some data and behaviour with `Patient` - in this case both have a name and show that name when you print them.
Since we expect all patients to be people (hopefully!), it makes sense to implement the behaviour in `Person` and then reuse it in `Patient`.

To write our class in Python, we used the `class` keyword, the name of the class, and then a block of the functions that belong to it.
If the class **inherits** from another class, we include the parent class name in brackets.

~~~ python
# file: inflammation/models.py

class Observation:
    def __init__(self, day, value):
        self.day = day
        self.value = value

    def __str__(self):
        return str(self.value)

class Person:
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return self.name

class Patient(Person):
    """A patient in an inflammation study."""
    def __init__(self, name):
        super().__init__(name)
        self.observations = []

    def add_observation(self, value, day=None):
        if day is None:
            try:
                day = self.observations[-1].day + 1

            except IndexError:
                day = 0

        new_observation = Observation(day, value)

        self.observations.append(new_observation)
        return new_observation

alice = Patient('Alice')
print(alice)

obs = alice.add_observation(3)
print(obs)

bob = Person('Bob')
print(bob)

obs = bob.add_observation(4)
print(obs)
~~~

~~~
Alice
3
Bob
AttributeError: 'Person' object has no attribute 'add_observation'
~~~

As expected, an error is thrown because we cannot add an observation to `bob`, who is a Person but not a Patient.

We see in the example above that to say that a class inherits from another, we put the **parent class** (or **superclass**) in brackets after the name of the **subclass**.

There's something else we need to add as well - Python doesn't automatically call the `__init__` method on the parent class if we provide a new `__init__` for our subclass, so we'll need to call it ourselves.
This makes sure that everything that needs to be initialised on the parent class has been, before we need to use it.
If we don't define a new `__init__` method for our subclass, Python will look for one on the parent class and use it automatically.
This is true of all methods - if we call a method which doesn't exist directly on our class, Python will search for it among the parent classes.
The order in which it does this search is known as the **method resolution order** - a little more on this in the Multiple Inheritance callout below.

The line `super().__init__(name)` gets the parent class, then calls the `__init__` method, providing the `name` variable that `Person.__init__` requires.
This is quite a common pattern, particularly for `__init__` methods, where we need to make sure an object is initialised as a valid `X`, before we can initialise it as a valid `Y` - e.g. a valid `Person` must have a name, before we can properly initialise a `Patient` model with their inflammation data.


## Composition vs Inheritance

When deciding how to implement a model of a particular system, you often have a choice of either composition or inheritance, where there is no obviously correct choice.
For example, it's not obvious whether a photocopier *is a* printer and *is a* scanner, or *has a* printer and *has a* scanner.

~~~ python
class Machine:
    pass

class Printer(Machine):
    pass

class Scanner(Machine):
    pass

class Copier(Printer, Scanner):
    # Copier `is a` Printer and `is a` Scanner
    pass
~~~

~~~ python
class Machine:
    pass

class Printer(Machine):
    pass

class Scanner(Machine):
    pass

class Copier(Machine):
    def __init__(self):
        # Copier `has a` Printer and `has a` Scanner
        self.printer = Printer()
        self.scanner = Scanner()
~~~

Both of these would be perfectly valid models and would work for most purposes.
However, unless there's something about how you need to use the model which would benefit from using a model based on inheritance, it's usually recommended to opt for **composition over inheritance**.
This is a common design principle in the object oriented paradigm and is worth remembering, as it's very common for people to overuse inheritance once they've been introduced to it.

For much more detail on this see the [Python Design Patterns guide](https://python-patterns.guide/gang-of-four/composition-over-inheritance/).

## Multiple Inheritance

**Multiple Inheritance** is when a class inherits from more than one direct parent class.
It exists in Python, but is often not present in other Object Oriented languages.
Although this might seem useful, like in our inheritance-based model of the photocopier above, it's best to avoid it unless you're sure it's the right thing to do, due to the complexity of the inheritance heirarchy.
Often using multiple inheritance is a sign you should instead be using composition - again like the photocopier model above.


::::challenge{id="a-model-patient" title="A Model Patient"}

Above we gave an example of a `Patient` class which inherits from `Person`. Let's can start with extending the system such that there must be a `Doctor` class to hold the data representing a single doctor, which:
  - must have a `name` attribute
  - must have a list of patients that this doctor is responsible for.

In addition to these, try to think of an extra feature you could add to the
models which would be useful for managing a dataset like this - imagine we're
running a clinical trial, what else might we want to know?  Try using Test
Driven Development for any features you add: write the tests first, then add the
feature.

Once you've finished the initial implementation, do you have much duplicated
code?  Is there anywhere you could make better use of composition or inheritance
to improve your implementation?

For any extra features you've added, explain them and how you implemented them
to your neighbour.  Would they have implemented that feature in the same way?

:::solution
One example solution is shown below. You may start by writing some tests (that will initially fail), and then 
develop the code to satisfy the new requirements and pass the tests.

~~~ python
# file: tests/test_patient.py   
"""Tests for the Patient model."""    

def test_create_patient():
    """Check a patient is created correctly given a name."""
    from inflammation.models import Patient
    name = 'Alice'
    p = Patient(name=name)
    assert p.name == name

def test_create_doctor():
    """Check a doctor is created correctly given a name."""
    from inflammation.models import Doctor
    name = 'Sheila Wheels'
    doc = Doctor(name=name)
    assert doc.name == name

def test_doctor_is_person():
    """Check if a doctor is a person."""
    from inflammation.models import Doctor, Person
    doc = Doctor("Sheila Wheels")
    assert isinstance(doc, Person)

def test_patient_is_person():
    """Check if a patient is a person. """
    from inflammation.models import Patient, Person
    alice = Patient("Alice")
    assert isinstance(alice, Person)

def test_patients_added_correctly():
    """Check patients are being added correctly by a doctor. """
    from inflammation.models import Doctor, Patient
    doc = Doctor("Sheila Wheels")
    alice = Patient("Alice")
    doc.add_patient(alice)
    assert doc.patients is not None
    assert len(doc.patients) == 1

def test_no_duplicate_patients():
    """Check adding the same patient to the same doctor twice does not result in duplicates. """
    from inflammation.models import Doctor, Patient
    doc = Doctor("Sheila Wheels")
    alice = Patient("Alice")
    doc.add_patient(alice)
    doc.add_patient(alice)
    assert len(doc.patients) == 1   
...
~~~    

~~~ python
# file: inflammation/models.py
...
class Person:
    """A person."""
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return self.name

class Patient(Person):
    """A patient in an inflammation study."""
    def __init__(self, name):
        super().__init__(name)
        self.observations = []

    def add_observation(self, value, day=None):
        if day is None:
            try:
                day = self.observations[-1].day + 1
            except IndexError:
                day = 0
        new_observation = Observation(day, value)
        self.observations.append(new_observation)
      return new_observation

class Doctor(Person):
    """A doctor in an inflammation study."""
    def __init__(self, name):
        super().__init__(name)
        self.patients = []

    def add_patient(self, new_patient):
        # A crude check by name if this patient is already looked after
        # by this doctor before adding them
        for patient in self.patients:
            if patient.name == new_patient.name:
                return
        self.patients.append(new_patient)
...
~~~    
:::
::::