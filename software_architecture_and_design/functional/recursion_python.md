---
name: Recursion
dependsOn: [
    software_architecture_and_design.functional.higher_order_functions_python,
]
tags: [python]
---

## Recursion

Recursion is one of the common strategies used in Functional Programming.
Instead of using loops to **iteratively** apply an operation, we can express a
result in terms of previous results.  To do this, the function needs to call
itself to get the previous result, this is called **recursion**.

The following two code examples implement the calculation of a factorial using
iteration and recursion, respectively. Recall that the factorial of a number `n`
(denoted by `n!`) is calculated as the product of integer numbers from 1 to `n`.

~~~python
def factorial(n):
    """Calculate the factorial of a given number.

    :param int n: The factorial to calculate
    :return: The resultant factorial
    """
    if n < 0:
        raise ValueError('Only use non-negative integers.')

    factorial = 1
    for i in range(1, n + 1): # iterate from 1 to n
        # save intermediate value to use in the next iteration
        factorial = factorial * i

    return factorial
~~~

Functions in procedural programming are *procedures* that describe a detailed
list of instructions to tell the computer what to do step by step and how to
change the state of the program and advance towards the result. They often use
*iteration* to repeat a series of steps. Functional programming, on the other
hand, typically uses *recursion* - an ability of a function to call/repeat
itself until a particular condition is reached.

~~~python
def factorial(n):
    """Calculate the factorial of a given number.

    :param int n: The factorial to calculate
    :return: The resultant factorial
    """
    if n < 0:
        raise ValueError('Only use non-negative integers.')

    if n == 0 or n == 1:
        return 1 # exit from recursion, prevents infinite loops
    else:
        return  n * factorial(n-1) # recursive call to the same function
~~~

::::challenge{id="recursion_on_trees" title="Recursion on trees"}

Recursion is a powerful tool for traversing tree data structures. Consider a
tree representing a mathematical expression like `1 + 2 * 3`. The tree could
have the following structure:

```python
class Node(object):
    "Generic tree node."
    def __init__(self, name='root', children=None):
        self.value = name
        self.children = children or []

    def __repr__(self):
        return f"Node({self.value}, {self.children})"

#    +
#   / \
#  1  *
#    / \
#   2   3
t = Node('+', [Node('1'),
               Node('*', [Node('2'),
                          Node('3')])])
```

Write:
1. a function that traverses the tree and returns the total number of nodes
2. a function that traverses the tree and returns the result of the
   expression


:::solution

```python
def count_nodes(tree):
    """Count the number of nodes in a tree.

    :param Node tree: The tree to count the nodes of
    :return: The number of nodes in the tree
    """
    if not tree.children:
        return 1
    else:
        return 1 + sum(count_nodes(child) for child in tree.children)

def evaluate(tree):
    """Evaluate the result of a tree representing a mathematical expression.

    :param Node tree: The tree to evaluate
    :return: The result of the expression
    """
    if not tree.children:
        return int(tree.value)
    else:
        left = evaluate(tree.children[0])
        right = evaluate(tree.children[1])
        if tree.name == '+':
            return left + right
        elif tree.name == '-':
            return left - right
        elif tree.name == '*':
            return left * right
        elif tree.name == '/':
            return left / right
        else:
            raise ValueError(f"Unknown operator: {tree.value}")
```
:::
::::

## Key Points:

- Recursion is a programming technique where a function calls itself, allowing solutions to problems that can be broken down into smaller subproblems
- Recursion is a useful approach for calculation and operations on tree data structures.