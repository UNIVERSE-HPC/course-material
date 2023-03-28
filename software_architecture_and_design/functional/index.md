---
id: functional
name: Functional Programming
dependsOn: [
    software_architecture_and_design.procedural,
]
files: [
    state_and_side_effects_cpp.md,
    state_and_side_effects_python.md,
    recursion_cpp.md,
    recursion_python.md,
    higher_order_functions_cpp.md,
    higher_order_functions_python.md,
]
---

Functional programming is a programming paradigm where programs are constructed
by applying and composing/chaining **functions**. Functional programming is
based on the [mathematical definition of a
function](https://en.wikipedia.org/wiki/Function_(mathematics)) `f()`, which
applies a transformation to some input data giving us some other data as a
result (i.e. a mapping from input `x` to output `f(x)`). Thus, a program written
in a functional style becomes a series of transformations on data which are
performed to produce a desired output. Each function (transformation) taken by
itself is simple and straightforward to understand; complexity is handled by
composing functions in various ways.

Often when we use the term function we are referring to a construct containing a
block of code which performs a particular task and can be reused. We have
already seen this in procedural programming - so how are functions in functional
programming different? The key difference is that functional programming is
focussed on **what** transformations are done to the data, rather than **how**
these transformations are performed (i.e. a detailed sequence of steps which
update the state of the code to reach a desired state).


:::callout

In his introduction to functional programming in Advanced R, Hadley Wickham gives a good summary of the style:

> It’s hard to describe exactly what a functional style is, but generally I think it means decomposing a big problem into smaller pieces, then solving each piece with a function or combination of functions.
> When using a functional style, you strive to decompose components of the problem into isolated functions that operate independently.
> Each function taken by itself is simple and straightforward to understand; complexity is handled by composing functions in various ways.
>
> -- Hadley Wickham - [Functional Style](https://adv-r.hadley.nz/fp.html)
:::