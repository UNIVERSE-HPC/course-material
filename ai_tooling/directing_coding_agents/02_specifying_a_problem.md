---
name: Specifying a problem
dependsOn: [
  ai_tooling.directing_coding_agents.01_prompt_design,
  software_project_management.collaboration.issues
]
tags: [genai, debugging]
learningOutcomes:
  - Recognise what a coding agent needs from an issue in order to resolve it
  - Decide whether an issue is written good enough for a coding agent
attribution:
  - citation: '"Introducing SWE-bench Verified" by OpenAI on 2024-08-13.'
    url: https://openai.com/index/introducing-swe-bench-verified/
    image: https://upload.wikimedia.org/wikipedia/commons/6/66/OpenAI_logo_2025_%28symbol%29.svg
    license: MIT
  - citation: The SymPy issue 19608 reported by ninely on 2020-06-21.
    url: https://github.com/sympy/sympy/issues/19608
    image: https://raw.githubusercontent.com/sympy/sympy/master/doc/src/logo/sympy.svg
    license: BSD-3-Clause
  - citation: The SymPy issue 14461 reported by rushyam on 2018-03-10.
    url: https://github.com/sympy/sympy/issues/14461
    image: https://raw.githubusercontent.com/sympy/sympy/master/doc/src/logo/sympy.svg
    license: BSD-3-Clause
  - citation: The SciPy issue 20797 reported by macdems on 2024-05-26.
    url: https://github.com/scipy/scipy/issues/20797
    image: https://raw.githubusercontent.com/scipy/scipy/main/doc/source/_static/logo.svg
    license: BSD-3-Clause
---

## Introduction

The previous module is about the design of a prompt *around* an issue and in
this module we are going to focus on the issue itself. No matter how good your
prompt is, if the issue is vague or misleading, no magic prompt will resolve
your actual problem. So in this module, you are going to learn, before giving
an issue to a coding agent, how to write a good enough issue and make sure the
coding agent can act on it.

Resolving an issue with coding agents is slightly different from working with
a human maintainer. Maintainers will reject or ask for more information on any
unclear bug report. Coding agents are increasingly likely to pause and ask for
clarification as well, but they can only ask about the confusion they have.
They will just act on whatever information you provided. So you still need to
know what is worth clarifying and how to define your problem.

Even before the advent of coding agents, writing a good issue or bug report
has always been an essential skill when you collaborate with others or report
a problem in other codebases. Now with coding agents, not only you need to
describe a problem to human maintainers (or just yourself), but also to a
coding agent. Having a good bug report is particularly important in research
code as whether it is correct or not sometimes requires domain knowledge (e.g.
can this output be negative? does the bug originate from invalid input
data/configuration?).

You can imagine just telling the coding agent 'my_func doesn't work' will not
work even if you have the state-of-the-art model. In fact, it may do more harm
than good: telling a coding agent to identify one bug, it will give you one
bug; ask for 5 bugs, it will confidently give you 5 bugs; ask for 20, it will
cheerfully give you 20... Coding agents will almost always claim there is an
issue and go on to 'fix' it.

## Evaluating an issue

Similarly to a prompt, what constitutes a 'good' issue/bug report is highly
dependent on the problem you are addressing. We will start by rating some
issues in real-world scientific codebases with the same scale of 0 to 3 used in
[SWE-bench Verified](https://openai.com/index/introducing-swe-bench-verified/):

> - **0**: The issue is well-specified and it is clear what is required for a
>   successful solution.
> - **1**: There are some blanks to fill in about the issue, but there is a
>   sensible interpretation of what is required for a successful solution.
> - **2**: The issue is vague and there is room for ambiguity. It is unclear
>   what a successful solution would look like.
> - **3**: It is almost impossible to understand what you are being asked to do
>   without further information.

:::callout{variant="note"}

To make [SWE-bench](https://www.swebench.com/) more reliable, OpenAI asked
professional software developers to rate every issue there on the scale above
and kept only the well-specified ones as a subset, which is [SWE-bench
Verified](https://openai.com/index/introducing-swe-bench-verified/). These
benchmarks measure the capability of the model to resolve real-world GitHub
issue and if the issues are under-specified, it will just measure how well the
model *guesses* what the issues mean instead of how well it *resolves* the
underlying problem.

:::

:::callout{variant="note"}

For the sake of the training, please refrain from reading the original issue
page, its fix, and all associated discussions until the end of the
corresponding challenge.

:::

### SymPy issue 19608

The [original issue](https://github.com/sympy/sympy/issues/19608) is included
here (as of 2026-09-07).

:::callout

```text
from sympy.core.sympify import kernS

text = "(2*x)/(x-1)"
expr = kernS(text)
// hit = kern in s
// UnboundLocalError: local variable 'kern' referenced before assignment
```

:::

::::challenge{id="sympy_19608" title="Rate the above issue"}

`kernS` parses a string into an expression while preventing SymPy from
autosimplication. Rate the issue with the above scale. If your rating is not
**0** (well-specified), try to think about what can be improved.

:::solution

This issue was annotated by 3 professional software developers in [SWE-bench
Verified](https://openai.com/index/introducing-swe-bench-verified/) with a
rating **0**. Despite its brevity, it gives the code snippet to reproduce the
exception, which has a clear error message. One thing that could be clearer
is that the comment `hit = kern in s` is the line in the codebase that gives
`UnboundLocalError`, but it is sufficient here as the coding agent will know it
when it tries to reproduce it.

A naive 'fix' would be to wrap `hit = kern in s` in a `try...except` block, as
it is one of the plausible ways to silent `UnboundLocalError`, so the issue
could be clearer on the expected behaviour: `kernS("(2*x)/(x-1)")` should
return the SymPy expression `2*x/(x - 1)`, which is exactly what the test added
by the [merged PR](https://github.com/sympy/sympy/pull/19637). In this case, it
is obvious enough what the expected output should be as modern coding agents
can infer it, and the docstring of `kernS` provides enough information as well.

:::

::::

### SymPy issue 14461

The [original issue](https://github.com/sympy/sympy/issues/14461) is included
here (as of 2026-09-07).

:::callout

```text
>>> from sympy import *
>>> a = symbols('a')
>>> l = Line((-a, 0), slope=oo)
>>> p = Parabola((a, 0), l)
>>> p.vertex
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "sympy/geometry/parabola.py", line 412, in vertex
    vertex = Point(focus.args[0] - self.p_parameter, focus.args[1])
  File "sympy/geometry/parabola.py", line 374, in p_parameter
    if (x < self.focus.args[0]):
  File "sympy/core/relational.py", line 229, in __nonzero__
    raise TypeError("cannot determine truth value of Relational")
TypeError: cannot determine truth value of Relational
```

:::

:::::challenge{id="sympy_14461" title="Rate the above issue"}

The snippet tries to build a `Parabola` object with a point `(a, 0)` (the
focus) and a vertical line (the directrix) which are both defined by a symbolic
coordinate `a`. It gives a `TypeError` when accessing the `vertex` attribute.

Rate the issue with the above scale. If your rating is not **0**, try to think
about what can be improved.

::::solution

The issue contains steps to reproduce the bug and the full traceback with the
specific location of the exception, so this issue provides even more details
than the previous one and it looks obvious to rate this as **0**. However, this
issue was also annotated by 3 professional software developers in [SWE-bench
Verified](https://openai.com/index/introducing-swe-bench-verified/) and 2 of
the 3 developers rated it as **2** and one rated **0**. For the purpose of
SWE-bench Verified, this issue was thus excluded after considering the rating
from the 3 developers.

Although the issue is very precise about what has failed, it does not specify
the expected behaviour. In this case, given a symbolic `Parabola`, should the
vertex also be symbolic? Capture this exception with an explanation? Or
vertex in symbolic `Parabola` should not be supported at all? Judging from the
traceback alone does not provide enough information about what it should do,
unlike the previous SymPy issue which is much more straightforward. Besides, the
message from `TypeError` is not very helpful: what does `"cannot determine
truth value of Relational"` even mean to the coding agent? The coding agent may
fix something in `core/relational.py` to make this comparison work instead and
silently alter a 'core' feature in the codebase. The [merged PR for this
issue](https://github.com/sympy/sympy/pull/16601) fixed the comparison by using
`sign()` instead of operator comparison.

The annotator who rated it **0** wrote:

> the expectation is that SymPy should be able to calculate the vertex of the
> parabola even when symbolic variables are used

while one of the annotators who rated it **2** commented:

> should the method return a symbolic expression for the vertex, or should it
> raise a more descriptive error or warning advising against symbolic
> parameters in this context?

The expected behaviour can be obvious to one person but not another, and it is
worth to be explicit when working with coding agents.

:::callout{variant="note"}

This issue is a reasonably good bug report **between humans**, where the user
reports an issue to the maintainers with steps to reproduce it and then lets
the maintainers decide how to deal with it. The above 'criticism' is aimed at
evaluating the as-is issue based on its suitability for (semi-)autonomous
resolution by coding agents (this issue was written in 2018!).

Nonetheless, it highlights an important difference between writing issue/bug
report for humans and for coding agents: you also become the 'maintainer' and
**should make the expected behaviour explicit**. Anything undecided leaves
room for ambiguity.

:::

::::

:::::

### SciPy issue 20797

The [original issue](https://github.com/scipy/scipy/issues/20797) is included
here with system information omitted for brevity (as of 2026-09-07).

:::callout

**Describe your issue.**

Since 1.10, the `scipy.special.hyp1f1` is computed by Boost. The change was
introduced by #17302. However, the computations by Boost are wrong. Take:

```python nolint
scipy.special.hyp1f1(-0.25, 0.5, 1+2j)
```

I have tested it with SciPy 1.8.0 and 1.13.1. The results are:

- 1.8.0: `(1.1814553180903438-1.2792130661292982j)`
- 1.13.1: `(1+0j)`

The latter one is obviously incorrect answer! Wolfram Alpha shows 1.18146 - 1.2792 *i*.

Please revert #17302!

**Reproducing Code Example:**

```python nolint
import scipy.special

print(scipy.special.hyp1f1(-0.25, 0.5, 1+2j))
```

**Error message:**

```shell
(1+0j)
```

:::

:::::challenge{id="scipy_20797" title="Rate the above issue"}

The issue reports a clear mismatch of output between different versions of
`scipy` from the same function `hyp1f1`. You do not need to know what `hyp1f1`
does (but of course you are welcome to look it up).

Rate the issue with the above scale. If your rating is not **0**, try to think
about what can be improved.

::::solution

The issue not only specifies the steps to reproduce the mismatch, but also
includes the expected output, which was checked independently. No ambiguity is
possible as it is very clear the old result should be the correct one. This is
a straight **0** on a scale that measures how well-specified an issue is.

This issue is not in the annotated set [SWE-bench
Verified](https://openai.com/index/introducing-swe-bench-verified/) so we have
nothing to compare with. If we look at the [merged PR that fixed this
issue](https://github.com/scipy/scipy/pull/20805), the root cause is a missing
line during previous translation from Fortran to C++. The user traced the
change to the PR that introduced the Boost library, which is a sensible thing
to do. However, in this case the mismatch has nothing to do with the Boost
library: that PR only replaced the calculation for real but not complex
arguments. Handing this issue as-is to a coding agent may waste effort
reverting a change that is unrelated to the underlying cause. The maintainer
also noted that even after fixing the missing line, `hyp1f1` still gives
inaccurate results with large parameters and this could be a limitation of the
underlying algorithm (which should be addressed later).

:::callout{variant="note"}

The issue goes further to suggest the root cause and a fix, and this is exactly
why it is misleading. Rating the issue on a scale that only measures how
under-specified it is gives a solid **0**, but coding agents may still not fix
the actual problem correctly as it is being directed to a wrong fix. It is best
to keep all the relevant observations but omit the exact fix, as this prevents
imposing constraints on the coding agents. Even if you are quite sure what goes
wrong, it is better to frame it as a guess or hypothesis, unless it is
absolutely trivial.

:::

::::

:::::

## Some more tips

There are many established good practices to systematically fix a bug and below
are some more tips when you are resolving a bug with a coding agent:

:::callout{variant="tip"}

- Coding agents could make a lot of changes when exploring the solution space
  so make sure the codebase is properly version-controlled so you can always
  revert if something goes wrong.
- Watch out for the coding agents changing your minimal examples/tests to
  pretend fixing the problem!
- For bugs that involve randomness, make sure to provide a seed so coding
  agents have something deterministic to work with.
- If the issue involves sensitive data, prepare a synthetic one with dummy
  data for the coding agent. Coming up with a synthetic dataset yourself
  sometimes actually helps the bug fix as it is easier to include edge cases.
  **Follow any data governance policy of the institution or research project
  before sending anything to a third-party inference provider**.
- For a bug that needs a large dataset, come up independently with a smaller
  representative dataset that still allows the coding agent to reproduce, so
  the coding agent is able to iterate quicker.
- If the bug can only be triggered in an HPC system, **consult the system
  administrators BEFORE using a coding agent in compute nodes** as coding
  agents could affect the shared file systems with performance and security
  implications, and you should **NEVER start a coding agent on the login
  node**.
- If the issue involves communication to any external systems, it is
  recommended to work in a sandbox environment such as a container in case
  anything sensitive (e.g. SSH keys, auth credentials etc.) is sent out or
  anything harmful is received, to limit the damage. It may be possible to mock
  the external systems but coding agents may not be resolving the bug in a
  realistic environment.
- There are nasty bugs that involve race conditions, async operations, and
  segmentation fault etc. which are difficult to reproduce reliably.
  These are always a pain to work with, but you may have some luck by giving
  the traceback (if any) to a coding agent for guidance, or ask coding agents
  to design a debugging framework to narrow down where the bug is.

:::

## Conclusion

:::callout{variant="keypoints"}

- The issue/bug report forms an important part of the context that a coding
  agent will work with so it is worth spending some effort to write a good and
  clear one.
- A good bug report that will be given to a coding agent should include a
  minimal example to reproduce the issue and state clearly the expected
  behaviour, especially if it involves decisions that only humans with domain
  knowledge can make.
- If you are suspicious about why things have gone wrong, it is better to frame
  it as a hypothesis instead of a definitive fix so the coding agents have
  fewer constraints to work with.

:::
