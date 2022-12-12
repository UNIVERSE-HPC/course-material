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

### Deleting Values, big-O notation and std::list

Deleting elements from the end of a vector is simple and fast and can be done using the 
`pop_back` function, which takes constant, or $\mathcal{O}(1)$ time using big-O notation. This 
means that the time taken is a constant or fixed amount of time independent of the size 
of the vector. Deleting elements from the *start* or *middle* of the vector is more 
difficult. An vector in C++ is an implementation of an *array* data structure, and 
therefore the values contained occupy a *contiguous* section of memory, the start of 
which is also the start of the vector. When deleting an element from the start or 
middle, the remainder of the vector must be shifted down to maintain the contiguous 
nature of the vector and the alignment of the first element to the start of the 
allocated memory. Therefore deleting elmements from the start or middle of a vector 
takes an amount of time that scales linearly with the size of the vector $n$, or 
$\mathcal{O}(n)$ time.

For example, if we want to delete an element from the middle of a vector while
preserving the order of the elements, we can do the 
 following:

~~~cpp
std::vector<int> x = {1, 2, 3, 4};
auto delete_this = x.begin() + 1; // an iterator to "2"
for (auto i = x.begin(); i != x.end(); i++) {
  if (i >= delete_this) {
    *i = *(i + 1);
  }
}
x.resize(x.size() - 1);
std::cout << "[";
for (auto i = x.begin(); i != x.end(); i++) {
  std::cout << *i << ", ";
}
std::cout << "]" << std::endl;
~~~

Notice that this requires a loop through all the elements of the vector, hence the time 
taken is $\mathcal{O}(n)$. The output of this program will show us the vector with a '2' 
removed:

~~~
[1, 3, 4, ]
~~~

A linked list is a data structure that provides constant-time insertion or deletion of 
elements in the middle/start of the container. The C++ implmentation of a linked list is `std::list`, which you can use like this:

~~~cpp
std::list<int> x = {1, 2, 3, 4};
auto delete_this = x.begin() + 1; // an iterator to "2"
x.erase(delete_this);

std::cout << "[";
for (auto i = x.begin(); i != x.end(); i++) {
  std::cout << *i << ", ";
}
std::cout << "]" << std::endl;
~~~

## Memory allocation and iterator invalidation

Each container manages its own memory via an allocator class (one of the
template arguements to each container). When using the `std::vector` class, it
is useful to have an awareness of how this allocation works. Generally
the memory allocation is handled automatically by the allocator, which reserves a
certain amount of memory (its capacity) which might be greater than the size of
the vector. Whenever the size of the vector exceeds this capacity the allocator
reallocates the memory for that vector, reserving a greater amount. 

~~~cpp
std::vector<int> x;
int old_capacity = x.capacity();
for (int i = 0; i < 3000; i++) {
  x.push_back(1);
  if (old_capacity != x.capacity()) {
    old_capacity = x.capacity();
    std::cout << "Size = " << x.size() << " Capacity = " << x.capacity() << std::endl;
  }
}
~~~

~~~
Size = 1 Capacity = 1
Size = 2 Capacity = 2
Size = 3 Capacity = 4
Size = 5 Capacity = 8
Size = 9 Capacity = 16
Size = 17 Capacity = 32
Size = 33 Capacity = 64
Size = 65 Capacity = 128
Size = 129 Capacity = 256
Size = 257 Capacity = 512
Size = 513 Capacity = 1024
Size = 1025 Capacity = 2048
Size = 2049 Capacity = 4096
~~~

Memory allocations are in general slow, so if the user has knowledge of the
neccessary size of the vector, then this process can be optimised by reserving
the correct amount of memory using `std::vector::reserve()`{.cpp}

~~~cpp
std::vector<int> x;
x.reserve(3000);
~~~

Another implication of memory reallocation for any container is that memory
reallocation neccessarily invalidates any iterators currently pointing at
specific elements (since they are now at a new memory address). This can be a
source of bugs, so be aware that growing or resizing a vector can invalidate
your iterators!

~~~cpp
std::vector<int> data = {1, 2, 3, 4};
// keep track of how much data we've already processed
auto processed = data.begin();
for (; processed != data.end(); processed++) {
  process_data(*processed);
}

// add some new data
for (int i = 0; i < 10; i++) {
  data.push_back(i);
}

// processed iterator might be invalid, so we might not be processing the new data
for (; processed != data.end(); processed++) {
  process_data(*processed);
}
~~~

If the function `process_data` prints out the value given, then the output might
look like this (it will vary depending on the particular allocations performed):

~~~
1 2 3 4 0 0 1041 0 540155953 540287027 540024880 825503793 891301920 892416052 859126069 808727840 808925234 891303730 842018868 808990772 892483616 926101557 941634361 808661305 808597809 842610720 808857908 941634101 842086709 959852598 942684192 943141431 941633588 842610736 875770421 825833504 926101555 941633587 825242164 943077432 942684192 925907257 941634103 942944825 909194803 909261088 892416049 958412597 859189556 825635636 942684192 858863158 941634864 959789104 959461431 842283040 925905206 941633586 892876848 942684471 825506080 825504566 941633840 942682676 959461174 959789344 892482872 958412857 943075892 842608948 859060512 875639857 958411059 859189556 943207731 842283040 925905206 941635123 926364983 825373744 892483616 892547896 958411824 808531506 892679473 825506080 892547894 941635384 875705650 875966770 859060512 876033840 958411315 943075892 842608948 892483872 842477625 958412597 859189556 858796340 842283296 942945337 958412082 959527216 858798132 959461664 808531506 941635640 825504313 959721526 943012128 892481844 941635385 942750005 909456697 892483616 909456182 958412339 943075892 842608948 943011872 825439800 958412853 859189556 875968564 959789344 825833527 958411824 909392181 825439281 842283040 808663090 958410804 809055538 909128245 825506080 892547894 941635128 926429753 942946358 842283296 875837494 941633847 808793394 808988726 892483616 892612661 958412342 859189556 808728627 842283296 909260854 958412343 909392181 876032305 959789344 859387959 941634612 942944825 842479666 943012128 942813492 958412597 925905716 842610741 842283040 959983670 941635636 909130037 842085680 892811296 943272758 958412597 825505845 959787057 959789088 891303992 808661305 842610995 942684192 858863158 941634864 825635380 892942640 842283296 825505846 941634105 909654069 943010099 825506080 942945078 941634614 859190578 808989493 842610720 909259833 941633588 942813748 909718067 892483616 943009845 958412340 859189556 892350772 959461664 808727862 958413110 825242420 960049200 892483616 808857653 958410808 876163636 943140917 825506080 909390646 941634609 959527221 943142192 942684192 876165177 941634361 825635380 808597296 959461664 943273266 958411571 859189556 943207731 842283296 926101816 958412852 825702704 926298168 842610720 909326388 958412337 808465204 892614713 943012128 858927412 941633588 942750005 909456697 842610720 925906227 958411319 909392181 875968049 942684192 943141431 958411318 825505845 808530227 892483616 875705394 958410802 875573302 808464953 842610720 909326388 941635121 892876848 859125303 0 0 49 0 1641085147 5 469321016 -564037215 0 1 2 3 0 0 81 0 1 2 3 4 0 1 2 3 4 5 6 7 8 9
~~~

## Strings as Containers

Conceptually, a string is a type of container, in this case of letters. The
`std::string`{.cpp} class is the standard implementation of a string class in
C++. The `std::string`{.cpp} container class has most of the same iterators and
functions as `std::vector`{.cpp} and behaves in a very similar way:

~~~cpp
#include <string>

//...

std::string element = "oxygen";
element.push_back('!');
std::cout << element[1] << ' ';
for (auto i = element.begin(); i != element.end(); i++) {
  std::cout << *i;
}
std::cout << std::endl;
~~~

gives the output

~~~
x oxygen!
~~~

As well as acting like a vector, `std::string`{.cpp} also has some useful
string-specific functionality, like being able to concatenate strings, to find
and extract substrings or to easily print out to screen

~~~cpp
using namespace std::string_literals;

const std::string oxygen = "oxygen"; // initialise with a const char *
const std::string hydrogen = "hydrogen"s; // initialise using a string literal
const std::string water = hydrogen + '-' + oxygen + '-' + hydrogen;
std::string::size_type first_dash = water.find("-");
const auto first_hydrogen = water.substr(0, first_dash); // first_hydrogen is a std::string

std::cout << "water is " << water << std::endl;
std::cout << "first element in water is " << first_hydrogen << std::endl;
~~~

~~~
water is hydrogen-oxygen-hydrogen
first element in water is hydrogen
~~~

## Map

The standard map class in C++ is `std::map`{.cpp}

This is also known as an "associative array", "dictionary" or "hash" in other
languages. The `std::map`{.cpp} class implements an ordered map, if you wish to
use an unordered map use `std::unordered_map`{.cpp}.

A map is templated on two types, one is the key type that we use to index the
map, the other is the value type that is stored in the map. The `std::map`
class implements a mapping between the key type to the value type. For example,
we can store and access the populations of various UK cities like so:

~~~cpp
#include <map>

//...

std::map<std::string, size_t>> populations = { 
  {"Liverpool": 467995},
  {"Edinburgh": 448850}, 
  {"Manchester": 430818}
}
for (const auto& [key, value] : m) {
  std::cout << '[' << key << "] = " << value << "; ";
}
std::cout << std::endl;
const auto key = "Liverpool"s;
std::cout << "The population of " << key << " is " << populations[key] << std::endl; 
~~~

~~~
[Edinburgh] = 448850; [Liverpool] = 467995; [Manchester] = 430818; 
The population of Liverpool is 467995
~~~



## General Rule

Your programs will be faster and more readable if you use the appropriate container type for your data's meaning. For example, always use a set for lists which can't in principle contain the same data twice, always use a dictionary for anything which feels like a mapping from keys to values.
