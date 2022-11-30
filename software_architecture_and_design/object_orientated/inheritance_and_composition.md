---
name: Inheritance and Composition
---

## Relationships

We now have a language construct for grouping data and behaviour related to a single conceptual object.
The next step is talking about the relationships between objects.

There are two fundamental types of relationship between objects which we need to be able to describe:

1. Ownership - x **has a** y - **composition**
2. Identity - x **is a** y - **inheritance**

### Composition

Composition is about ownership of an object or resource - x **has a** y.

In the case of our academics example, we can say that academics have papers.
Note that after the previous time we used this example, we clarified that academics should not *contain* papers.

~~~ python
class Paper:
    def __init__(self, title, pub_date):
        self.title = title
        self.pub_date = pub_date

    def __str__(self):
        return self.title

class Academic:
    def __init__(self, name):
        self.name = name
        self.papers = []

    def write_paper(self, title, date):
        new_paper = Paper(title, date)

        self.papers.append(new_paper)
        return new_paper

alice = Academic('Alice')
paper = alice.write_paper('A new paper', 2019)

print(paper)
~~~
{: .language-python}

~~~
A new paper
~~~
{: .output}

### Inheritance

Inheritance is about behaviour shared by classes, because they have some shared identity.

If we want to extend the previous example to also manage people who aren't academics we can add another class `Person`.
But `Person` will share some behaviour with `Academic` - in this case both have a name and show that name when you print them.

Since we expect all academics to be people (hopefully!), it makes sense to implement the behaviour in `Person` and then reuse it in `Academic`.

~~~ python
class Person:
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return self.name

class Academic(Person):
    def __init__(self, name):
        super().__init__(name)
        self.papers = []

    def write_paper(self, title, date):
        new_paper = Paper(title, date)

        self.papers.append(new_paper)
        return new_paper

alice = Acadmic('Alice')
print(alice)

bob = Person('Bob')
print(bob)

paper = alice.write_paper('A paper', 2016)
print(paper)

paper = bob.write_paper('A different paper', 2019)
print(paper)
~~~
{: .language-python}

~~~
Alice
Bob
A paper
AttributeError: 'Person' object has no attribute 'write_paper'
~~~
{: .output}

We see in the example above that to say that a class inherits from another, we put the **parent class** (or **superclass**) in brackets after the name of the **subclass**.

There's something else we need to add as well - Python doesn't automatically call the `__init__` method on the parent class, so we need to do this manually within the `__init__` method of the subclass.

Python does however provide us with a shortcut to access the parent class.
The line `super().__init__(name)` gets the parent class, then calls the `__init__` method, providing the `name` variable that `Person.__init__` requires.


> ## Multiple Inheritance
>
> Python, and a few other languages, allow you to have a class inherit from multiple parent classes, which is known as **multiple inheritance**.
> In a class with multiple parent classes, it may not be obvious which parent class we should go to to look for a method (see [Deadly Diamond of Death](https://en.wikipedia.org/wiki/Multiple_inheritance#The_diamond_problem)).
> In order to resolve this, Python uses a method known as [C3 Linearisation](https://en.wikipedia.org/wiki/C3_linearization), but this is in the realm of advanced Python and we should not expect people who use our code to know this.
>
> Because of this complexity, it is recommended not to use multiple inheritance, unless you are certain that this is the simplest way to describe the problem you wish to solve.
>
{: .callout}


### Composition vs Inheritance

When deciding how to implement a model of a particular system, you often have a choice of either composition or inheritance, where there is no obviously correct choice.
For example, if we need to produce a model of a photocopier:

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
{: .language-python}

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
{: .language-python}

Which of these is better?
Well, both of them seem like reasonable approximations of the thing we're trying to model.
It seems fair to say that a photocopier *is a* printer and *is a* scanner, but it also seems fair to say that a photocopier *has a* printer and *has a* scanner.

It depends on the context, but generally, we should favour **composition over inheritance** (*has a* over *is a*), as it leads to relationships between objects being slightly simpler to manage.
It's quite common for software developers, when introduced to classes, to produce code which has long, complicated inheritance heirarchies in places where composition would work equally well.
Try to avoid this temptation - the simpler you can keep your code, the better.

> ## Building a Library
>
> Using what we've seen so far, implement two classes: `Book` (you can use the one from the earlier exercise) and `Library` which have the following behaviour:
>
> ~~~ python
> library = Library()
>
> library.add_book('My First Book', 'Alice')
> library.add_book('My Second Book', 'Alice')
> library.add_book('A Different Book', 'Bob')
>
> print(len(library))
>
> book = library[2]
> print(book)
>
> books = library.by_author('Alice')
> for book in books:
>     print(book)
>
> books = library.by_author('Carol')
> ~~~
> {: .language-python}
>
> ~~~
> 3
> A Different Book by Bob
> My First Book by Alice
> My Second Book by Alice
> KeyError: 'Author does not exist'
> ~~~
> {: .output}
>
> > ## Solution
> > ~~~ python
> > class Book:
> >     def __init__(self, title, author):
> >         self.title = title
> >         self.author = author
> >
> >     def __str__(self):
> >         return self.title + ' by ' + self.author
> >
> >
> > class Library:
> >     def __init__(self):
> >         self.books = []
> >
> >     def add_book(self, title, author):
> >         self.books.append(Book(title, author))
> >
> >     def __len__(self):
> >         return len(self.books)
> >
> >     def __getitem__(self, key):
> >         return self.books[key]
> >
> >     def by_author(self, author):
> >         matches = []
> >         for book in self.books:
> >             if book.author == author:
> >                 matches.append(book)
> >
> >         if not matches:
> >             raise KeyError('Author does not exist')
> >
> >         return matches
> > ~~~
> > {: .output}
> {: .solution}
>
> Extend the class so that we can get the list of all authors and titles.
> If an author appears multiple times, they should only appear once in the list of authors:
>
> ~~~ python
> print(library.titles)
> print(library.authors)
> ~~~
> {: .language-python}
>
> ~~~
> ['My First Book', 'My Second Book', 'A Different Book']
> ['Alice', 'Bob']
> ~~~
> {: .output}
>
> > ## Solution
> > ~~~ python
> > class Book:
> >     def __init__(self, title, author):
> >         self.title = title
> >         self.author = author
> >
> >     def __str__(self):
> >         return self.title + ' by ' + self.author
> >
> >
> > class Library:
> >     def __init__(self):
> >         self.books = []
> >
> >     def add_book(self, title, author):
> >         self.books.append(Book(title, author))
> >
> >     def __len__(self):
> >         return len(self.books)
> >
> >     def __getitem__(self, key):
> >         return self.books[key]
> >
> >     def by_author(self, author):
> >         matches = []
> >         for book in self.books:
> >             if book.author == author:
> >                 matches.append(book)
> >
> >         if not matches:
> >             raise KeyError('Author does not exist')
> >
> >         return matches
> >
> >     @property
> >     def titles(self):
> >         titles = []
> >         for book in self.books:
> >             titles.append(book.title)
> >
> >         return titles
> >
> >     @property
> >     def authors(self):
> >         authors = []
> >         for book in self.books:
> >             if book.author not in authors:
> >                 authors.append(book.author)
> >
> >         return authors
> > ~~~
> > {: .output}
> {: .solution}
>
> The built in `set` class has a `set.union` method which takes two sets (one of which is `self`) and returns a new set containing all of the members of both sets, with no duplicates.
>
> Extend your library model with a `union` method which behaves the same way - it should return a new `Library` containing all the books of the two provided libraries.
>
> To do this you might need to create a `Book.__eq__` method.
> The `__eq__` dunder method should take two objects (one of which is `self`) and return `True` if the two objects should be considered equal - otherwise return `False`.
>
> > ## Solution
> > ~~~ python
> > class Book:
> >     def __init__(self, title, author):
> >         self.title = title
> >         self.author = author
> >
> >     def __str__(self):
> >         return self.title + ' by ' + self.author
> >
> >     def __eq__(self, other):
> >         return self.title == other.title and self.author == other.author
> >
> >
> > class Library:
> >     def __init__(self, books=None):
> >         # This is a common pattern if we need to allow an empty collection
> >         # We could use 'books=[]' but this doesn't behave how you might expect - why is this?
> >         if books is None:
> >             self.books = []
> >
> >         else:
> >             self.books = books
> >
> >     def add_book(self, title, author):
> >         self.books.append(Book(title, author))
> >
> >     def __len__(self):
> >         return len(self.books)
> >
> >     def __getitem__(self, key):
> >         return self.books[key]
> >
> >     def by_author(self, author):
> >         matches = []
> >         for book in self.books:
> >             if book.author == author:
> >                 matches.append(book)
> >
> >         if not matches:
> >             raise KeyError('Author does not exist')
> >
> >         return matches
> >
> >     @property
> >     def titles(self):
> >         titles = []
> >         for book in self.books:
> >             titles.append(book.title)
> >
> >         return titles
> >
> >     @property
> >     def authors(self):
> >         authors = []
> >         for book in self.books:
> >             if book.author not in authors:
> >                 authors.append(book.author)
> >
> >         return authors
> >
> >     def union(self, other):
> >         books = []
> >         for book in self.books:
> >             if book not in books:
> >                 books.append(book)
> >
> >         for book in other.books:
> >             if book not in books:
> >                 books.append(book)
> >
> >         return Library(books)
> > ~~~
> > {: .language-python}
> >
> > Since we're copying a lot of the behaviour of the built in `set` class, it might actually make sense to have `Library` inherit from `set`.
> > If you've got this far, try writing this alternative implementation.
> > Here's [the documentation](https://docs.python.org/3/library/stdtypes.html#set) for the `set` type.
> >
> > Which do you prefer?
> >
> > {: .output}
> {: .solution}
{: .challenge}
