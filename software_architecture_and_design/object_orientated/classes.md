## Using Classes

Classes are one of the fundamental concepts of Object Oriented programming - acting as the template for objects which share common attributes and behaviour.

Just like functions, we've already been using some of the classes built into Python.

~~~ python
my_list = [1, 2, 3]
my_dict = {1: '1', 2: '2', 3: '3'}
my_set = {1, 2, 3}

print(type(my_list))
print(type(my_dict))
print(type(my_set))
~~~
{: .language-python}

~~~
<class 'list'>
<class 'dict'>
<class 'set'>
~~~
{: .output}

Lists, dictionaries and sets are a slightly special type of class, but they behave in much the same way as a class we might define ourselves.
They each contain data, as we have seen before.
They also provide a set of functions, or **methods** which describe the **behaviours** of the data.

The behaviours we have seen previously include:

- Lists can be appended to
- Lists can be indexed (we'll make our own class with this later)
- Lists can be sliced
- The union of two sets can be found
- The intersection of two sets can be found

## Creating Classes

So how do we create and use our own class in Python?

~~~ python
class Academic:
    def __init__(self, name):
        self.name = name
        self.papers = []

alice = Academic('Alice')
print(alice.name)
~~~
{: .language-python}

~~~
Alice
~~~
{: .output}

Similar to functions, we begin by using a special keyword, in this case `class`, followed by a name, then a colon to begin a new block.
Inside this block, we define the data and behaviour that we want the class to provide.

Almost every class you define will have a `__init__` (pronounced "init" or "dunder-init" for double-underscore) **method** which is responsible for initialising any data that an instance of the class needs in order to be valid.
Note that this is slightly different from the **constructor** if you've encountered classes in other OO languages (it doesn't allocate memory for the class itself), but it's usually safe to treat it as the same.

> ## Data Classes
> Python 3.7 added a slightly different syntax we can use to automatically generate some of the class structure.
> These **Data Classes** are intended for cases where the data is more important than the behaviour, but are otherwise completely normal classes.
>
> In this example we just define the data **attributes** and their types - the `__init__` method is then generated automatically.
>
> ~~~ python
> import dataclasses
> import typing
>
> @dataclasses.dataclass
> class Academic:
>     name: str
>     papers: typing.List[typing.Dict] = dataclasses.field(default_factory=list)
>
> alice = Academic('Alice')
> print(alice)
> ~~~
> {: .language-python}
>
> ~~~
> Alice
> ~~~
> {: .output}
>
> While dataclasses reduce the overhead in some respects, they also introduce overhead in defining some of the data types inside the class.
>
> For more information see [this page](https://docs.python.org/3/library/dataclasses.html) of the Python documentation.
{: .callout}

### Methods

To define the behaviour of a class we can add functions which operate on the data the class contains.
We call these functions **member functions** or **methods**.

These functions are the same as normal functions (alternatively known as **free functions**), but we have an extra first parameter `self`.
The `self` parameter is a normal variable, but when we use a method of an object, the value of `self` is automatically set to the object.

~~~ python
class Academic:
    def __init__(self, name):
        self.name = name
        self.papers = []

    def write_paper(self, title, date):
        new_paper = {
            'title': title,
            'date': date
        }

        self.papers.append(new_paper)
        return new_paper

alice = Academic('Alice')
print(alice)

# Normal use of a member function
paper = alice.write_paper('A new paper', 2018)
print(paper)
print(alice.papers)
~~~
{: .language-python}

~~~
<__main__.Academic object at 0x7f4826fdff10>
{'title': 'A new paper', 'date': 2018}
[{'title': 'A new paper', 'date': 2018}]
~~~
{: .output}

### Dunder Methods

Why is the `__init__` method not called `init`?

There are a few special method names that we can use which Python will use to provide a few common behaviours, each of which begins and ends with two underscores, hence the name **dunder method**.
The most commonly used dunder method is `__init__`, but there are a few other common ones:

~~~ python
class Academic:
    def __init__(self, name):
        self.name = name
        self.papers = []

    def write_paper(self, title, date):
        new_paper = {
            'title': title,
            'date': date
        }

        self.papers.append(new_paper)
        return new_paper

    def __str__(self):
        return self.name

    def __getitem__(self, index):
        return self.papers[index]

    def __len__(self):
        return len(self.papers)

alice = Academic('Alice')
print(alice)

alice.write_paper('A new paper', 2018)
paper = alice[0]
print(paper)

print(len(alice))
~~~
{: .language-python}

~~~
Alice
{'title': 'A new paper', 'date': 2018}
1
~~~
{: .output}

In the example above we can see:

- `__str__` - converts an object into its string representation, used when you do `str(object)` or `print(object)`
- `__getitem__` - Accesses an object by key, this is how `list[x]` and `dict[x]` are implemented
- `__len__` - gets the length of an object - usually the number of items it contains

> ## Meaningful Behaviours
>
> In the previous example, we defined some of the **data** and **behaviours** of an academic, but do all of these make sense?
> What data and behaviours did we define?
> Do these really represent the real-world object we're modelling?
>
> > ## Solution
> >
> > The data attributes we defined for an academic make sense, these were:
> >
> > - has a name
> > - has papers
> >
> > The behaviours we defined were:
> >
> > - can write a paper
> > - can be represented as text
> > - can get contents
> > - can get number of contents
> >
> > These last two don't really make sense as they suggest that an academic **contains** papers, which probably isn't what we intended to say.
> > To get the number of papers, we probably should have used `len(alice.papers)` from the main code and not had the `__len__` method in the class.
> > This works because the `papers` attribute is a list type, which comes with a built in `__len__` method.
> >
> > These were included in the example to demonstrate their use, but when designing your own classes, it's important to think about whether what you're designing is a meaningful representation of reality.
> > When we encounter code that behaves differently from reality, it can cause us to have incorrect assumptions about the structure and behaviour of the code.
> {: .solution}
{: .challenge}

There are many more described in the Python documentation, but it's also worth experimenting with built in Python objects to see which methods provide which behaviour.

> ## A Basic Class
>
> Implement a class to represent a book.
> Your class should:
>
> - Have a title
> - Have an author
> - When printed, show text in the format "title by author"
>
> ~~~ python
> book = Book('A Book', 'Me')
>
> print(book)
> ~~~
> {: .language-python}
>
> ~~~
> A Book by Me
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
> > ~~~
> > {: .output}
> {: .solution}
{: .challenge}

## Properties

The final special type of method we'll introduce is **properties**.
Properties are methods which behave like data - when we want to access them, we don't need to use brackets to call the method manually.
They're often used a bit like **getters** from other Object Oriented languages (see [this page](https://www.w3schools.com/cpp/cpp_encapsulation.asp) for more information).

~~~ python
class Academic:
    ...

    @property
    def last_paper(self):
        return self.papers[-1]

alice = Academic('Alice')

alice.write_paper('First paper', 1)
alice.write_paper('Second paper', 2)

paper = alice.last_paper
print(paper)
~~~
{: .language-python}

The `@` syntax means that a function called `property` is being used to modify the behavior of the method - this is called a **decorator**.
In this case the `@property` decorator converts `last_paper` from a normal method into a property.
We'll see tomorrow how we decorators work in more detail and how we can make our own.

Function decorators in Python are a modification of the [Decorator](https://en.wikipedia.org/wiki/Decorator_pattern) **Design Pattern**.
Design Patterns are established templates that we can use to design the interactions between components in software using the Object Oriented paradigm.
Many of these come from a book titled '[Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns)' by Erich Gamma *et al*, published in 1994.