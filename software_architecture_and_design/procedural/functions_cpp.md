### Exceptions




### No Value?

Nothing is a complicated concept, and each language deals with no value, or a
null value, in its own way. In C++ you can define a variable without
initialising it, like so:

~~~cpp
int something;
std::cout << something << std::endl;
~~~

Here we have used `something` without initialising to some value. Something will
be printed out, but we don't know what. One option is that the compiler will
allocate `something` a certain section of memory, and then print out whatever is
at that location. Another option is that the compiler will optimise out
`something` altogether and just print out 0, or something of its own choosing.

This is an example of [*undefined
behaviour*](https://en.cppreference.com/w/cpp/language/ub). This covers all
situations where the programmer violated certain rules of the C++ language, and
in these cases it is up to each individual compiler what actually occurs during
compilation and execution of the program. For another, more interesting example,
we can use one of those given in the previous cppreference page:

~~~cpp
bool p; // uninitialized local variable
if(p) // UB access to uninitialized scalar
    std::puts("p is true");
if(!p) // UB access to uninitialized scalar
    std::puts("p is false");
~~~

Here `p` is a boolean variable (`true` or `false`), and for a certain older version of gcc the output is:

~~~
p is true
p is false
~~~

What if we try to create an uninitialised reference?

~~~cpp
int& something;
std::cout << something << std::endl;
~~~

In this case we get the following error:

~~~
/Users/martinjrobins/git/thing/procedural.cpp:27:10: error: declaration of reference variable 'something' requires an initializer
    int& something;
~~~

So no luck here creating nothing! In C++ you could represent no value, or nothing, by a null pointer like so:

~~~cpp
std::unique_ptr<int> nothing = nullptr;
std::unique_ptr<int> something = std::make_unique<int>(1);
if (nothing) {
    std::cout << *nothing << std::endl;
} else {
    std::cout << "None" << std::endl;
}
if (something) {
    std::cout << *something << std::endl;
} else {
    std::cout << "None" << std::endl;
}
~~~

Here we use one of the smart pointers we mentioned earlier, a unique pointer, to
emphasise our earlier message to not use a raw pointer if you can avoid it. The
angle brackets indicate a templated class, in this case a unique pointer to an
`int`. Templates in C++ enable yet another programming paradigm called
*generic programming*. As you can see, there are a number of different
programming paradigms, all with their own uses! It is best to be aware of and
use all of them interchangably for the problem at hand or the idea you wish to
express. 

Going back to the example, note that we have initialised our variable, so there
is no undefined behaviour and we can be assured of the same behaviour over all
compilers. It should be noted, however, that in this case the `std::unique_ptr`
will be default initialised even if we don't initialise it, but it is still
better to write the explicit initialisation to be clear. Note that we need to
use two *different* print statements to print either the `int` or the string
literal "None", since these are two different types. Recall, in C++ all types
must be known at compile time, for each variable and also for each expression or
statement in your program.

The output is:

~~~
None
~~~

Note, another way to create nothing in C++ (since C++17) is to use `std::optional`:

~~~cpp
std::optional<int> nothing = std::nullopt; 
if (nothing) {
    std::cout << *nothing << std::endl;
} else {
    std::cout << "None" << std::endl;
}
~~~

~~~
None
~~~

::::challenge

Initialise a `std::string` with an arbitrary string of your choosing. Use the
`std::string::find` function to find the first occurence of the letter "a" in
the string. If "a" is found then create a `std::unique_ptr` or a
`std::optional` pointing to or containing that letter in the string (if it exists).

Print out the value in your `std::optional` or pointed to be `std::unique_ptr`
(if it exists). Change the original string to the other option (exist/not exists)
and test to see if your code still works.

:::solution

:::

::::