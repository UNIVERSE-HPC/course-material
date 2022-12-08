---
name: Containers
dependsOn: [
    software_architecture_and_design.procedural.types_cpp,
]
---

*Container* types are those that can hold other objects, and C++ supports a number of 
different [containers](https://en.cppreference.com/w/cpp/container) we can use to hold 
data of differing types in a multitude of ways. 

## Vector and Arrays

One of the most fundamental data structures in any language is the array, used to hold 
many values at once in a contiguous region of memory. There are two array containers in 
C++, depending on if you have a fixed sized array (`std::array`), or variable sized 
(`std::vector`). When using these a user needs to specify the type of value held by that 
container, and for the `std::array` you also need to specify the length. For example, to 
define a `std::vector` of `double` values you could write

```cpp
std::vector<double> x;
```

or a `std::array` of five `int` values is declared as:

```cpp
std::array<int, 5> y;
```

The angle bracket syntax here is an example of using *templates* in C++, and both 
`std::vector` and `std::array` are examples of *templated* classes. Templates in C++ are 
a form of *generic programming*, and allow us to write classes (and functions) that can 
accept many different types. All the container in C++ need to be able to hold any type 
of value, and therefore all of the container types in C++ are templated on the value 
type. The `std::array` class represents an array with a pre-defined size, and so this 
size is another template arguement. Note that unlike arguements to functions, all 
template arguements must be know *at compile time*.

Since `std::array` has more limited use compared with `std::vector`, we will focus the 
remainder of this section on `std::vector`. The interface to `std::array` is very 
similar, and you can read about this particular container 
[here](https://en.cppreference.com/w/cpp/container/array). We will also from here on 
refer to `std::vector` as a vector, rather than the more general term "array", to match 
with the name of the class itself.

### Creating and Extracting Things from Vectors

To define a vector initialised to a list of values, we can simply write a comma 
separated list of items in curly brackets. We can also define a two dimensional vector 
by defining a "vector of vectors".

~~~cpp
std::vector<int> odds = {1, 3, 5, 7, 9, 11, 15};
std::vector<std::vector<int>> more_numbers = { {1, 2}, {3, 4, 5}, { {6, 7}, {8} } }
~~~

We can see that our multi-dimensional vector can contain elements themselves of any size 
and depth. This could be used as way of representing matrices, but later we'll learn a 
better way to represent these.

This curly bracket syntax is for representing *initializer lists* in C++. These 
initializer lists can only be used when initialising, or constructing, an instance of a 
class, and cannot be used once the instance has been already created. For example, the 
following code will give a compile error:

~~~cpp
std::vector<int> odds;
odds = {1, 3, 5, 7, 9, 11, 15};
~~~

Note that every value in a vector must be of the same type, and this must match the type 
that the `std::vector` is templated on.

We can select individual elements from vecotrs by indexing them. Looking at our `odds` 
list:

![index-list](../fig/05-index-list-odd.png)

For example:

~~~cpp
std::cout << odds[0] << ' ' << odds[-1] << std::endl;
~~~

This will print the first and last elements of a list:

~~~
1 15
~~~

We can replace elements within a specific part of the list (note that in C++, indexes 
start at 0):

~~~cpp
odds[6] = 13;
~~~

To add elements to the *end* of the vector use `push_back`, remove elements from
the *end* of the vector using `pop_back`. You can resize the vector using
`resize`. Get the current size of the vector using `size`.

~~~cpp
std::vector<double> x;
x.push_back(1.0);
x.push_back(2.0); // x holds {1.0, 2.0}
x.pop_back();     // x holds {1.0}
x.resize(3);      // x holds {1.0, ?, ?}

std::cout << x.size() << std::endl; // 3
~~~

## Loop or iterate over a Vector

Every container in C++ defines its own *iterators*, which can be used to iterate
over that container.

~~~cpp
for (std::vector<double>::iterator i = x.begin();
     i != x.end(); ++i) {
   std:cout << *i << std::endl;
}
~~~

An iterator acts like a pointer to each element of the vector, and thus it can be 
dereferenced using `*` to obtain a reference to the value pointed to.

We can simplify this rather verbose iterator classname by using the `auto`{.Cpp} 
keyword. This tells the compiler to infer the
correct type (i.e. what is returned from `x.begin()`{.Cpp}:

~~~cpp
for (auto i = x.begin(); i != x.end(); ++i)
{
   std:cout << *i << std::endl;
}
~~~

Another `for` loop in C++ is the *range-based* loop, and these have the most compact 
syntax, and work with any container that has `begin` and `end` methods.

~~~cpp
std::vector<double> x = {1.0, 2.0, 3.0, 4.0};
for (double i: x)
{
   std:cout << i << std::endl;
}
~~~

You can also use `auto`{.Cpp} here to simplify things...

~~~cpp
for (auto i: x)
{
   std:cout << i << std::endl;
}
~~~

The previous code snippet could not alter the contents of the vector
because `i` was a *copy* of each element of x. You can instead make `i` a
reference to either edit values

~~~cpp
for (auto& i: x)
{
    i = 1.0; // set each element to 1.0
}
~~~

or to provide a constant reference to each value (thus avoiding any copies)

~~~cpp
for (const auto& i: x)
{
    std::cout << i << std::endl; // print each element to the console
}
~~~

::::challenge{id=slicing_from_the_end, title="Slicing From the End"}

Use slicing to access only the last four characters of a string or entries of a list.

~~~cpp
string_for_slicing = "Observation date: 02-Feb-2013"
list_for_slicing = [["fluorine", "F"],
                    ["chlorine", "Cl"],
                    ["bromine", "Br"],
                    ["iodine", "I"],
                    ["astatine", "At"]]

print(string_for_slicing)
print(list_for_slicing)
~~~

~~~
'Observation date: 02-Feb-2013'
[['fluorine', 'F'], ['chlorine', 'Cl'], ['bromine', 'Br'], ['iodine', 'I'], ['astatine', 'At']]
~~~

So what would you use to see the following?

~~~
'2013'
[['chlorine', 'Cl'], ['bromine', 'Br'], ['iodine', 'I'], ['astatine', 'At']]
~~~

Would your solution work regardless of whether you knew beforehand
the length of the string or list
(e.g. if you wanted to apply the solution to a set of lists of different lengths)?
If not, try to change your approach to make it more robust.

Hint: Remember that indices can be negative as well as positive

:::solution
Use negative indices to count elements from the end of a container (such as list or string):

~~~
string_for_slicing[-4:]
list_for_slicing[-4:]
~~~
:::
::::

### Deleting Values

Deleting elements from the end of a vector is simple and fast and can be done using the 
`pop_back` function, which takes constant, or `O(1)` time using big-O notation. This 
means that the time taken is a constant or fixed amount of time independent of the size 
of the vector. Deleting elements from the *start* or *middle* of the vector is more 
difficult. An vector in C++ is an implementation of an *array* data structure, and 
therefore the values contained occupy a *contiguous* section of memory, the start of 
which is also the start of the vector. Deleting an element from the middle of the vector 
is more difficultas the contiguous nature of its element must be maintained. Similarly, 
deleting an element from the start of the vector also requires the remainder of the 
elments to be shifted down so that the start of the vector is aligned with the start of 
the memory allocated. Therefore deleting elmements from the start or middle of a vector 
takes an amount of time that scales linearly with the size of the vector $n$, or 
$\mathcal{O}(n)$ time.


For example, one way to delete an element from the middle of a vectorthis is to swap the 
element to be deleted with the last element, and then resize the vector.

~~~cpp
std::vector<int> x = {1, 2, 3, 4};
auto delete_this = x.begin + 1; // an iterator to "2"
for (auto i = x.begin(); i != x.end(); i++) {
  if (i == delete_this) {
    *i = x.back();
  }
}
x.resize(x.size() - 1);
std::cout << "[";
for (auto i = x.begin(); i != x.end(); i++) {
  std::cout << *i << ", ";
}
std::cout << "]" << std::endl;

~~~

This will show us the vector with a '2' removed:

~~~
[1, 3, 4, ]
~~~

Sets or linked lists are conFor containers that can more efficient

### Changing the size of a Vector



## Strings as Containers

Conceptually, a string is a type of container, in this case of letters. We can also index and slice strings in the same way as a list:

~~~
element = 'oxygen'
print(element[1], element[0:3], element[3:6])
~~~
{: .language-python}
~~~
x oxy gen
~~~
{: .output}

## Tuples

A tuple is an immutable sequence. It is like a list, in terms of indexing, repetition and nested objects, except it cannot be changed - it's an immutable type. Instead of using square brackets to define them, we use round brackets.

With a single element tuple, you need to end the assignment with a comma.

~~~
x = 0,
y = ('Apple', 'Banana', 'Cherry')
type(x)
~~~
{: .language-python}

~~~
<class 'tuple'>
~~~
{: .output}

So similarly to lists for indexing elements:

~~~
y[1]
~~~
{: .language-python}

~~~
'Banana'
~~~
{: .output}

But as we mentioned, it's an immutable type:

~~~
my_tuple = ('Hello', 'World')
my_tuple[0] = 'Goodbye'
~~~
{: .language-python}

~~~
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: 'tuple' object does not support item assignment
~~~
{: .output}

## Dictionaries

Python supports a container type called a dictionary.

This is also known as an "associative array", "map" or "hash" in other languages. Dictionaries are ordered: elements used to be unordered in older versions of Python, but since version 3.6 they are guaranteed to be ordered.

In a list, we use a number to look up an element:

~~~
names = 'Martin Luther King'.split(' ')
names[1]
~~~
{: .language-python}

~~~
'Luther'
~~~
{: .output}

In a dictionary, we look up an element using another object of our choice:

~~~
me = { 'name': 'Joe', 'age': 39, 
       'Jobs': ['Programmer', 'Teacher'] }
me
~~~
{: .language-python}

~~~
{'name': 'Joe', 'age': 39, 'Jobs': ['Programmer', 'Teacher']}
~~~
{: .output}

~~~
me['Jobs']
~~~
{: .language-python}

~~~
['Programmer', 'Teacher']
~~~
{: .output}

~~~
me['age']
~~~
{: .language-python}

~~~
39
~~~
{: .output}

~~~
type(me)
~~~
{: .language-python}

~~~
<class 'dict'>
~~~
{: .output}

### Keys and Values

The things we can use to look up with are called keys:

~~~
me.keys()
~~~
{: .language-python}

~~~
dict_keys(['name', age', 'Jobs'])
~~~
{: .output}

The things we can look up are called values:

~~~
me.values()
~~~
{: .language-python}

~~~
dict_values(['Joe', 39, ['Programmer', 'Teacher']])
~~~
{: .output}

When we test for containment on the dict itself we essentially test on its keys:

~~~
'Jobs' in me
~~~
{: .language-python}

~~~
True
~~~
{: .output}

~~~
'Joe' in me
~~~
{: .language-python}

~~~
False
~~~
{: .output}

But we can also test on the values of a dict:

~~~
'Joe' in me.values()
~~~
{: .language-python}

~~~
True
~~~
{: .output}

### Immutable Keys Only

The way in which dictionaries work is one of the coolest things in computer science: the "hash table". The details of this are beyond the scope of this course, but we will consider some aspects in the section on performance programming.

One consequence of this implementation is that you can only use immutable things as keys.

~~~
good_match = {
    ("Lamb", "Mint"): True, 
    ("Bacon", "Chocolate"): False
   }
~~~
{: .language-python}

But:

~~~
illegal = {
    ["Lamb", "Mint"]: True, 
    ["Bacon", "Chocolate"]: False
   }
~~~
{: .language-python}

~~~
Traceback (most recent call last):
  File "<stdin>", line 3, in <module>
TypeError: unhashable type: 'list'
~~~
{: .output}

Remember -- square brackets denote lists, round brackets denote tuples.

## Beware 'Copying' of Containers!

Here, note that `y` is not equal to the contents of `x`, it is a second label on the *same object*. So when we change `y`, we are also changing `x`. This is generally true for mutable types in Python.

~~~
x = [1, 2, 3]
y = x
y[1] = 20
print(x, y)
~~~
{: .language-python}

~~~
[1, 20, 3] [1, 20, 3]
~~~
{: .output}

Instead, if we wanted to ensure our changes occurred separately on an actual copy of the contents, we could do:

~~~
x = [1, 2, 3]
y = x[:]
y[1] = 20
print(x, y)
~~~
{: .language-python}

~~~
[1, 2, 3] [1, 20, 3]
~~~
{: .output}

In this case, we are using `x[:]` to create a new list containing all the elements of `x` which is assigned to `y`. This happens whenever we take any sized slice from a list.

This gets more complicated when we consider nested lists.

~~~
x = [['a', 'b'] , 'c']
y = x
z = x[:]

x[0][1] = 'd'
z[1] = 'e'

print(x, y, z)
~~~
{: .language-python}

~~~
[['a', 'd'], 'c'] [['a', 'd'], 'c'] [['a', 'd'], 'e']
~~~
{: .output}

Note that `x` and `y` are the same as we may expect. But `z`, despite being a copy of `x`'s original contents, now contains `'d'` in its nested list.

The copies that we make through slicing are called shallow copies: we don't copy all the objects they contain, only the references to them. This is why the nested list in `x[0]` is not copied, so `z[0]` still refers to it. It is possible to actually create copies of all the contents, however deeply nested they are - this is called a *deep copy*. Python provides methods for that in its standard library in the `copy` module.

## General Rule

Your programs will be faster and more readable if you use the appropriate container type for your data's meaning. For example, always use a set for lists which can't in principle contain the same data twice, always use a dictionary for anything which feels like a mapping from keys to values.
