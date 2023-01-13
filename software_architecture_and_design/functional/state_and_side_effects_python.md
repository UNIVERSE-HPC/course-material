---
name: State and Side Effects
dependsOn: [
    software_architecture_and_design.procedural.arrays_python,
]
---



## Side Effect and Pure Functions

There's nothing particularly special about the behaviour of functions in Python,
almost any valid code can be put inside a function to be called from elsewhere
(even defining classes or other functions).  This means that each function can
do anything the Python language can do.

In well designed code, a function should only be responsible for one task (see:
[Single Responsibility Principle](https://en.wikipedia.org/wiki/Single-responsibility_principle)).  As
with most guidelines, the reason we do this is to make it easier to reason about
the behaviour of our code.  If a function performs only one task, we can be sure
about when it is appropriate to use that function and what will happen when we
call it.

Any behaviour which is not part of the single task of a block of code is called
a **side effect**, while a function which has no side effects is a **pure
function**.  In practice these definitions are a little flexible depending on
who is using them and in which context.  The strictest definition of **side
effect** includes things like printing to the terminal or saving to a file, so
in these cases a function without side effects have no other effect than to
return a value.  Using a stricter definition, a **pure function** must not only
have no side effects, but also must return the same value when given the same
arguments.

~~~ python
def increment_x(x):
    return x + 1

print(increment_x(3))
~~~
{: .language-python}

~~~
4
~~~
{: .output}

~~~ python
def increment_x(x):
    print('Incrementing', x)
    return x + 1

print(increment_x(3))
~~~
{: .language-python}

~~~
Incrementing 3
4
~~~
{: .output}
