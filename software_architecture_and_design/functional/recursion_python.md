---
name: Recursion
dependsOn: [
    software_architecture_and_design.functional.state_and_side_effects_python,
]
tags: [python]
---

## Recursion

See: Recursion

Recursion is one of the common strategies used in Functional Programming.
Instead of using loops to **iteratively** apply an operation, we can express a result in terms of previous results.
To do this, the function needs to call itself to get the previous result, this is called **recursion**.

<a href="{{ page.root }}/fig/droste-effect.jpg">
  <img src="{{ page.root }}/fig/droste-effect-small.jpg" alt="Droste Effect" />
</a>

To illustrate recursive functions, we'll use factorials as an example.
The factorial of a positive integer `N` (written `N!`) is the product of all of the positive integers equal to or less than `N`.
For example, `5! = 5 x 4 x 3 x 2 x 1 = 120`.

We can calculate a factorial iteratively:

~~~ python
def factorial(n):
    product = 1
    for i in range(1, n + 1):
        product = product * i

    return product

for i in range(5):
    print(factorial(i))
~~~

~~~
1
1
2
6
24
~~~

But the factorial function has a property which makes it particularly suitable to be defined recursively.

To define a recursive function we need two things: a **recurrence relation** and a **base case**.
A recurrence relation is a process which can be used to derive the value of a sequence, given the previous value in the sequence.
With just a recurrence relation, the function would run forever, continually trying to get the previous value, so we also need a base case.
The base case is a value in the sequence which is known without having to derive it from previous values.

In the case of the factorial function the recurrence relation is: `N! = N * (N-1)!` or equivalently `f(N) = N * f(N - 1)` - the Nth value in the sequence is N times the previous value.
The base case is `0! = 1` - the factorial of zero is one.

So, if we express the factorial function recursively, we get:

~~~python
def factorial(n):
    if n == 0:
        return 1

    return n * factorial(n - 1)
~~~

There's something a bit dangerous about this implementation though: if we attempt to get the factorial of a negative number, the code will get stuck in an infinite loop.
In practice, Python has a limit to the number of times a function is allowed to recurse, so we'll actually get an error.

To make this safer, we should handle the case where `n < 0` and raise an error.

~~~python
def factorial(n):
    if n < 0:
        raise ValueError('Factorial is not defined for values less than 0')
    if n == 0:
        return 1

    return n * factorial(n - 1)
~~~

::::challenge{id=fibonacci, title="Recursive Fibonacci"}

Another well known sequence is the Fibonacci sequence: `0, 1, 1, 2, 3, 5, 8, 13, ...` where each value is the sum of the previous two values.

One possible iterative implementation of a function to calculate the Nth Fibonacci number is shown below.
Also note how tuple packing and unpacking are used to effectively swap two values without using a temporary variable.

~~~python
def fibonacci(n):
    # Iterative fibonacci
    a, b = 0, 1

    for _ in range(n):
        a, b = b, a + b

    return a

for i in range(8):
    print(fibonacci(i))
~~~

~~~
0
1
1
2
3
5
8
13
~~~

Write an equivalent function which uses recursion to calculate the Nth Fibonacci number.

Hint: first think about what the recurrence relation and base case are.

:::solution
First, we need to decide what the recurrence relation is - in this case it's `f(N) = f(N - 1) + f(N - 2)`.
And the base cases `f(0) = 0` and `f(1) = 1`.

For the function itself, we can use the same approach as for the factorial function: first handle the base cases, then the recurrence relation:

~~~python
def fibonacci(n):
    if n < 0:
        raise ValueError('Fibonacci is not defined for N < 0')
    if n == 0:
        return 0
    if n == 1:
        return 1

    return fibonacci(n - 1) + fibonacci(n - 2)

for i in range(8):
    print(fibonacci(i))
~~~

~~~
0
1
1
2
3
5
8
13
~~~
:::
::::