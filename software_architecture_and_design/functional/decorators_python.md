
Finally, we will look at one last aspect of Python where functional programming is coming handy. 
As we have seen in the [episode on parametrising our unit tests](../22-scaling-up-unit-testing/index.html#parameterising-our-unit-tests), a decorator can take a function, modify/decorate it, then return the resulting function. This is possible because Python treats functions as first-class objects that can be passed around 
as normal data. Here, we discuss decorators in more detail and learn how to write our own. Let's look at the following code for ways on how to "decorate" functions.

~~~
def with_logging(func):

    """A decorator which adds logging to a function."""
    def inner(*args, **kwargs):
        print("Before function call")
        result = func(*args, **kwargs)
        print("After function call")
        return result

    return inner


def add_one(n):
    print("Adding one")
    return n + 1

# Redefine function add_one by wrapping it within with_logging function
add_one = with_logging(add_one)

# Another way to redefine a function - using a decorator
@with_logging
def add_two(n):
    print("Adding two")
    return n + 2

print(add_one(1))
print(add_two(1))
~~~
{: .language-python}
~~~
Before function call
Adding one
After function call
2
Before function call
Adding two
After function call
3
~~~
{: .output}

In this example, we see a decorator (`with_logging`) and two different syntaxes for applying the decorator to a function. The decorator is implemented here as a function which encloses another function. Because the inner function (`inner()`) calls the function being decorated (`func()`) and returns its result, it still behaves like this original function. Part of this is the use of `*args` and `**kwargs` - these allow our decorated function to accept any arguments or keyword arguments and pass them directly to the function being decorated. Our decorator in this case does not need to modify any of the arguments, so we do not need to know what they are. Any additional behaviour we want to add as part of our decorated function, we can put before or after the call to the original function. Here we print some text both before and after the decorated function, to show the order in which events happen.

We also see in this example the two different ways in which a decorator can be applied. The first of these is to use a normal function call (`with_logging(add_one)`), where we then assign the resulting function back to a variable - often using the original name of the function, so replacing it with the decorated version. The second syntax is the one we have seen previously (`@with_logging`). This syntax is equivalent to the previous one - the result is that we have a decorated version of the function, here with the name `add_two`. Both of these syntaxes can be useful in different situations: the `@` syntax is more concise if we never need to use the un-decorated version, while the function-call syntax gives us more flexibility - we can continue to use the un-decorated function if we make sure to give the decorated one a different name, and can even make multiple decorated versions using different decorators.

> ## Exercise: Measuring Performance Using Decorators
>One small task you might find a useful case for a decorator is measuring the time taken to execute a particular function. This is an important part of performance profiling.
>
>Write a decorator which you can use to measure the execution time of the decorated function using the [time.process_time_ns()](https://docs.python.org/3/library/time.html#time.process_time_ns) function. There are several different timing functions each with slightly different use-cases, but we won’t worry about that here.
>
>For the function to measure, you may wish to use this as an example:
>~~~
>def measure_me(n):
>    total = 0
>    for i in range(n):
>        total += i * i
>
>    return total
>~~~
>{: .language-python}
> >## Solution
> >
> >~~~
> >import time
> >
> >def profile(func):
> >    def inner(*args, **kwargs):
> >        start = time.process_time_ns()
> >        result = func(*args, **kwargs)
> >        stop = time.process_time_ns()
> >
> >        print("Took {0} seconds".format((stop - start) / 1e9))
> >        return result
> >
> >    return inner
> >
> >@profile
> >def measure_me(n):
> >    total = 0
> >    for i in range(n):
> >        total += i * i
> >
> >    return total
> >
> >print(measure_me(1000000))
> >~~~
> >{: .language-python}  
> >~~~
> >Took 0.124199753 seconds
> >333332833333500000
> >~~~
> >{: .output}
>{: .solution}
{: .challenge}