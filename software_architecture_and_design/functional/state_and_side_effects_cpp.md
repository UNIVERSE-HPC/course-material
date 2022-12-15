---
name: State and Side Effects
dependsOn: [
    software_architecture_and_design.procedural.functions_cpp,
]
---

## Program state

The current state of a given program is composed of many different parts, some
of which are clear, others which are more opaque. The
most obvious state is the values of all the current variables in the scope. Take
this code snippet:

```cpp
int y = 3;
int x = 2;
x = y;
```

Here we have two variables `x` and `y` with initial values, when the last line
is executed the state is updated so that `x` now has the value 3. We can clearly see this from the code given, but it can be less obvious when this code is hidden in a function:

```cpp
void my_cool_function(int& x, const int y) {
    x = y
}

int main () {
  int y = 3;
  int x = 2;
  my_cool_function(x, y);
}
```

Now imagine we have a global variable defined elsewhere that is updated by
`my_cool_function`, this is not even passed into the function so it is even
more unclear that this is being updated. The global variable and function might
even be declared in a separate file and brought in via a `#include`

```cpp
int z = 3;
void my_cool_function(int& x, const int y) {
    x = y
    z++;
}

int main () {
  int y = 3;
  int x = 2;
  my_cool_function(x, y);
}
```

Finally, any interaction with the Operating System is yet another, even more
opaque, part of the current state. This includes memory allocations, and file
IO.

```cpp
std::vector<BigType> data(1000); // do we have enough RAM available for this?

ofstream myfile;
myfile.open ("example.txt", ios::out); // does this file exist? Do I have write permissions?
std::string line;
std::getline(myfile, line); // did I open this for reading?
std::getline(myfile, line); // Same call to getline, but result is different!
```

The main downside of having a state that is constantly updated is that it makes
it harder for us to *reason* about our code, to work out what it is doing.
However, the main upside is that we can use state to make calculations more
efficient, for example to sum up the values in a vector we use a single variable
`sum` to hold the state of the computation

```cpp
std::vector<int> data = {1, 2, 3, 4};
int sum = 0;
for (const auto& x: data) {
    sum += x;
}
```

Declaritive programming (which includes functional programming) tries to reduce
the impact of state on the readability of our code by simply not including it.
In declaritive programming the programmer typically only states the task to be
done, or the desired outcome, rather than the individual steps required to
achive it. This is in contrast with imperitive programming (which includes
procedural programming) where the programmer instead directly writes the steps
neccessary to achieve an (not explicitly stated) outcome.

Now C++ is a declaritive programming language, but nevertheless we can still
write as much as possible in a *declaritive* or *functional* style. After all,
We want our code to be easy to reason about and written in such a way that it
is clear what the code is doing. As a minor example, lets update
`my_cool_function` to be named appropriately and with
decent documentation. 

```cpp
/**
 * Set x to equal y
 *
 * Update the value of x equal to the given value of y
 *
 * @param x this arg is updated with the value of y
 * @param y value to set x
 */
void set_first_arg(int& x, const int y);

int main() {
  int y = 3;
  int x = 2;
  set_first_arg(x, y);
}
```

The rest of this course builds on from this to describe all the features of C++
that allow you to write your code in a more functional style, increasing the
readability and maintainability of your code.

## Side Effect and Pure Functions

In well designed code, a function should only be responsible for one task (see:
[Single Responsibility Principle](https://en.wikipedia.org/wiki/Single-responsibility_principle)). As
with most guidelines, the reason we do this is to make it easier to reason about
the behaviour of our code. If a function performs only one task, we can be sure
about when it is appropriate to use that function and what will happen when we
call it.

Any behaviour which is not part of the single task of a block of code is called
a **side effect**, while a function which has no side effects is a **pure
function**. In practice these definitions are a little flexible depending on
who is using them and in which context. The strictest definition of **side effect** includes things like printing to the terminal, saving to a file or even reading a global variable, so
in these cases a function without side effects does not rely on anything
external to its own arguments. Using a stricter definition, a **pure function**
must not only have no side effects, but also must return the same value when
given the same arguments. A pure function is therefore the computational version of a mathematical function.

```cpp
// a pure function, no side effects :)
int increment_and_return_x(const int& x) {
  return x + 1;
}

// technically a side effect with the print, but this is probably safe
int increment_and_return_x(const int& x) {
  std::cout << "Incrementing x = " << x << std::endl;
  return x + 1;
}

// a side effect since we modify x, but clear from the function signature and name
// however, still not recommended since the pure function above is just as easy to implement
int increment_x(int& x) {
  x += 1;
}

int one = 1;
// dangerous side effect! effect of function changes with global `one` variable
int increment_x(int& x) {
  x += one;
}
~~~