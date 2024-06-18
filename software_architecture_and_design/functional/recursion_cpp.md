---
name: Recursion
dependsOn: [
    software_architecture_and_design.functional.higher_order_functions_cpp,
]
tags: [cpp]
attribution: 
    - citation: >
        This material was adapted from an "Introduction to C++" course developed by the
        Oxford RSE group.
      url: https://www.rse.ox.ac.uk
      image: https://www.rse.ox.ac.uk/images/banner_ox_rse.svg
      license: CC-BY-4.0
    - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1 
      url: https://www.universe-hpc.ac.uk
      image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
      license: CC-BY-4.0

---

## Recursion

Recursion is one of the common strategies used in Functional Programming.
Instead of using loops to **iteratively** apply an operation, we can express a
result in terms of previous results.  To do this, the function needs to call
itself to get the previous result, this is called **recursion**.

The following two code examples implement the calculation of a factorial using
iteration and recursion, respectively. Recall that the factorial of a number `n`
(denoted by `n!`) is calculated as the product of integer numbers from 1 to `n`.

```cpp
// factorial
// @param n: the number to calculate the factorial of
// @return: the factorial of n
int factorial(int n):
  int product = 1;
  for (int i = 2; i <= n; ++i) {
      product *= i;
  }
  return product;
}
```

Functions in procedural programming are *procedures* that describe a detailed
list of instructions to tell the computer what to do step by step and how to
change the state of the program and advance towards the result. They often use
*iteration* to repeat a series of steps. Functional programming, on the other
hand, often uses *recursion* - an ability of a function to call/repeat
itself until a particular condition is reached.

```cpp
// factorial
// @param n: the number to calculate the factorial of
// @return: the factorial of n
int factorial(int n) {
  if (n < 0) {
    throw std::invalid_argument("factorial is not defined for values less than 0");
  }
  if (n == 0) {
    return 1;
  }
  return n * factorial(n - 1);
}
```

Note: this implementation is an example of *tail recursion*, which is typically
optimised by the compiler back to an iterative implementation (since this is
faster).

::::challenge{id="recursion_on_trees" title="Recursion on trees"}

Recursion is a powerful tool for traversing tree data structures. Consider a
tree representing a mathematical expression like `1 + 2 * 3`. The tree could
have the following structure:

```cpp
class Node {
  public:
    int value;
    std::vector<Node> children;
    Node(int value, std::vector<Node> children) : value(value), children(children) {}
};

int main() {
  //    +
  //   / \
  //  1  *
  //    / \
  //   2   3
  Node t = Node('+', { Node(1), 
                       Node('*', { Node(2), 
                                   Node(3)
                                 })
                     }
               );
}
```

Write:

1. a function that traverses the tree and returns the total number of nodes
2. a function that traverses the tree and returns the result of the
   expression

:::solution

```cpp
int count_nodes(const Node& t) {
  int count = 1;
  for (const Node& child : t.children) {
    count += count_nodes(child);
  }
  return count;
}

// or using std::accumulate
int count_nodes2(const Node& t) {
  int count = 1;
  std::accumulate(t.children.begin(), t.children.end(), 0, [](int a, const Node& b) { return a + count_nodes(b); }
  return count;
}

int evaluate(const Node& t) {
  if (t.children.empty()) {
    return t.value;
  }
  int result = evaluate(t.children[0]);
  for (int i = 1; i < t.children.size(); ++i) {
    switch (t.value) {
      case '+':
        result += evaluate(t.children[i]);
        break;
      case '-':
        result -= evaluate(t.children[i]);
        break;
      case '*':
        result *= evaluate(t.children[i]);
        break;
      case '/':
        result /= evaluate(t.children[i]);
        break;
    }
  }
  return result;
}

// or using std::accumulate and std::function
int evaluate2(const Node& t) {
  if (t.children.empty()) {
    return t.value;
  }
  const std::function<int(const int, const int)> op;
  switch (t.value) {
    case '+':
      op = [](int a, int b) { return a + b; };
      break;
    case '-':
      op = [](int a, int b) { return a - b; };
      break;
    case '*':
      op = [](int a, int b) { return a * b; };
      break;
    case '/':
      op = [](int a, int b) { return a / b; };
      break;
  }
  return std::accumulate(t.children.begin() + 1, t.children.end(), evaluate(t.children[0]), op);
}
```

:::
::::

## Key Points

- Recursion is a programming technique where a function calls itself, allowing solutions to problems that can be broken down into smaller subproblems
- Recursion is a useful approach for calculation and operations on tree data structures.
