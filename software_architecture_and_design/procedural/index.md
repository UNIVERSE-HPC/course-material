---
id: procedural
name: Procedural Programming
dependsOn: [
    tools_and_technology.bash_shell,
    tools_and_technology.ide,
]
files: [
    python.md,
    c-plus-plus.md,
    types.md,
    functions.md,
]
---

## The Procedural Paradigm

So far we've been writing our code as one continuous flow.
If we want to reuse some of our code, we can use loops to repeat some task, but sometimes we need more flexibility than this.

It would also be useful to be able to hide some of the complexity of our code once it's grown to the point where it no longer fits on a single screen.

Procedural Programming is based around the idea that code should be structured into a set of **procedures**.
Each procedure (optionally) takes some input, performs some computation and (optionally) returns some output.
We can then use these procedures to perform computation, without having to be concerned with exactly how the computation is performed.

You may wish to think of the Procedural Paradigm as focussing on the **verbs** of a computation.
