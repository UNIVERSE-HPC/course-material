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
