## Generators

### Generator Expressions

**Generator expressions** look exactly like you might expect a tuple comprehension (which don't exist) to look, and behaves a little differently from the other comprehensions.
What happens if we try to use them in the same was as we did list comprehensions?

~~~
print((2 * i for i in range(5)))
~~~
{: .language-python}

~~~
<generator object <genexpr> at 0x7efc21efcdd0>
~~~
{: .output}

Like the `map` and `filter` functions, generator expressions are not evaluated until you iterate over them.

~~~
for i in (2 * i for i in range(5)):
    print(i)
~~~
{: .language-python}