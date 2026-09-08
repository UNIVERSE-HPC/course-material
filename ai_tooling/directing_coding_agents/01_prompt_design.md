---
name: Prompt design
dependsOn: []
tags: []
learningOutcomes:
  - Be able to direct a coding agent by prompting appropriately when handling issues
  - Understand the context that should be provided to a coding agent based on examples
attribution:
  - citation: The Astropy issue 19895 reported by maxnoe on 2026-06-10.
    url: https://github.com/astropy/astropy/issues/19895
  - citation: The Matplotlib issue 32178 reported by ArchangeGabriel on 2026-08-07.
    url: https://github.com/matplotlib/matplotlib/issues/32178
  - citation: The scikit-learn issue 34622 reported by PranavAchar01 on 2026-08-01.
    url: https://github.com/scikit-learn/scikit-learn/issues/34622
  - citation: The pytest issue 14720 reported by dprada on 2026-07-18.
    url: https://github.com/pytest-dev/pytest/issues/14720
  - citation: The SymPy issue 29731 reported by peteroupc on 2026-05-02.
    url: https://github.com/sympy/sympy/issues/29731
---

## Introduction

Good prompt design is one of the most fundamental skills one should have in
order to work with any coding agent, regardless of the capability of the
underlying model. When you discover an issue in your codebase, you would write
a prompt describing your issue and hand it over to the coding agent to address
it. Instead of prompting with 'Fix this issue and make no mistakes', it is much
more effective to design a prompt that sets the right direction of the whole
session and provides context that can shape what the coding agent believes
the problem is, what it is allowed or not allowed to touch, and what counts as
done etc.

In this module, instead of providing you with a set of rules or a checklist
about what a good prompt should be, you will learn how to design a good prompt
by evaluating prompts based on user-reported issues in real-world scientific
codebases. What constitutes a 'good' prompt is highly dependent on the
problem you are addressing, so the aim of this module is to let you evaluate
prompts, which were written with some strengths and weaknesses as what real
prompts would look like, to know how to approach a particular issue in a coding
project appropriately. Note that real-world problems are rarely solved one-shot
and you can/should always iterate to direct the coding agent, but digesting the
output from a misdirected prompt not only costs unnecessary tokens, but also
exhausts your mental power.

## Prompts evaluation

5 issues reported in some popular open-source Python scientific packages have
been collected and presented here, and a respective prompt has been written to
address the issue. Your task is to:

- evaluate the prompt based on the issue
- include what instructions are helpful or not so for this specific issue
- provide your reasoning
- propose a minimal rewrite, if time allows

You are encouraged to write down your evaluation instead of just looking at it
and assign a single good or bad score mentally. If you are attempting this
module in a group, you can even discuss with each other **after your
own analysis**. After the evaluation, you can compare your evaluation with the
comments below.

:::callout{variant="discussion"}

The comments are not meant to be exhaustive and you may find yourself
disagreeing with it, which is not particularly problematic given there are
multiple ways to approach a problem. The important thing is that you think
about the design of a prompt based on a specific issue.

:::

The issues were selected such that no prior knowledge about their codebases is
required, although you may find it helpful to understand some of the related
concepts so you are free to search for anything (or you can use, you guess
what...). However, for the sake of the exercises, you are discouraged to
navigate to the actual issue pages before the evaluation as they may contain
some comments or linked PRs which will impact your evaluation/rewrite.

:::callout{variant="danger"}

The purpose of these exercises is NOT to teach you how to contribute to an
open-source project using AI assistance. You may be tempted to submit a PR (if
there isn't any) after giving the prompt to your favourite LLM model. In any
case, you should engage with the maintainers first and really know what you are
doing as this is always good etiquette. A lot of open-source projects nowadays
contain their AI policy in the contribution guidelines, which either bans any
AI usage or requires you to declare any form of AI usage.

:::

### Astropy issue 19895

The new `format="ecsv"` reader in Astropy fails to read an empty table that the
legacy `format="ascii.ecsv"` reader handles.
[ECSV](https://docs.astropy.org/en/stable/io/ascii/ecsv.html) is Astropy's
human-readable text format for storing a table together with its column
specifications and metadata.

#### The issue

The [original issue](https://github.com/astropy/astropy/issues/19895) is
included here (as of 2026-08-20).

````markdown
### Description

The new `format="ecsv"` raises an error when reading empty tables.

The same code works fine with `format="ascii.ecsv`".

### Expected behavior

Works

### How to Reproduce

```python
from astropy.table import Table

t = Table()
t.meta["foo"] = 1
t.meta["bar"] = "example"
t.write("test.ecsv", format="ecsv", overwrite=True)

Table.read("test.ecsv", format="ecsv")
```

```
Raises:
Traceback (most recent call last):
  File "/home/maxnoe/CTAO/astropy/test_empty_table.py", line 8, in <module>
    Table.read("test.ecsv", format="ecsv")
  File "/home/maxnoe/CTAO/astropy/astropy/table/connect.py", line 72, in __call__
    out = self.registry.read(cls, *args, **kwargs)
          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/home/maxnoe/CTAO/astropy/astropy/io/registry/core.py", line 232, in read
    data = reader(*args, **kwargs)
           ^^^^^^^^^^^^^^^^^^^^^^^
  File "/home/maxnoe/CTAO/astropy/astropy/io/misc/ecsv.py", line 1219, in read_ecsv
    data_raw = read_data(
               ^^^^^^^^^^
  File "/home/maxnoe/CTAO/astropy/astropy/io/misc/ecsv.py", line 762, in read_data
    raise InconsistentTableError(
astropy.io.ascii.core.InconsistentTableError: column names from ECSV header [] do not match names from header line of CSV data ['#', 'schema:', 'astropy-2.0']
```

### Versions

```python
import astropy
astropy.system_info()
```
```
platform
--------
platform.platform() = 'Linux-7.0.10-201.fc44.x86_64-x86_64-with-glibc2.43'
platform.version() = '#1 SMP PREEMPT_DYNAMIC Wed May 27 13:57:41 UTC 2026'
platform.python_version() = '3.12.9'

packages
--------
astropy              8.1.0.dev191+g3be6a3733
numpy                2.2.4
scipy                1.15.2
matplotlib           3.10.8
pandas               2.2.3
pyerfa               2.0.1.5
```

Current main, but also e.g. 7.2.
````

#### The prompt to be evaluated

You can assume the above issue is saved in a file called `iss_astropy_19895.md`
when evaluating the prompt.

::::challenge

Read the Astropy issue #19895, which is saved in the file
`@iss_astropy_19895.md`.

Reproduce the bug, then write a minimal test that writes an empty table with
some dummy metadata using the existing interface, reads it back and checks that
the metadata is preserved, just as the issue illustrated. This test should fail
with the reported error.

Inspect all the relevant functions and methods related to reading and writing
the ECSV format. The fix should reuse the `ascii.ecsv` route that is working so
it can preserve consistency between `format="ecsv"` and `format="ascii.ecsv"`.
If an invalid ECSV file is passed to the reader, return an empty table instead
of raising an error, as users would not want to see such a traceback.

Keep the change within ECSV I/O operations and do not add any dependency or
change how non-empty tables are read. After the fix, the added minimal test
should pass.

::::

#### Comments of the above prompt

:::solution

##### Strengths

The prompt and the reported issue specify a clear failure and an observable
success condition: an empty table with metadata should be able to be created
and read back via the `ecsv` route, demonstrated by a test that must fail
before the fix and pass after it. This is stronger than asking to make parsing
"work". It also asks for inspection of the relevant reading and writing code
before editing, and the ECSV-I/O scope together with the non-empty-table
constraint limits unnecessary changes.

##### Weaknesses

Stating that the fix should reuse the working `ascii.ecsv` route prematurely
chooses an implementation. The working route is something that should be
investigated, but this does not necessarily mean the new `ecsv` route should
use it. It is more important to direct the coding agent to investigate **why**
this is the case, as this might be deliberate and forcing the old route risks
unnecessary coupling or breaking something unintentionally etc.

Turning every invalid ECSV file into an empty table expands the scope beyond
what was stated in the issue, although this might not be a bad idea. Without
engaging with the maintainers of the codebase, it is often wiser to limit the
scope. Besides, converting an invalid ECSV file into an empty table may destroy
an important error signal which some downstream functions/packages may depend
on.

Finally, running only the newly added test is an incomplete verification: it
proves the existing minimal examples but skips other ECSV behaviour the
prompt claims to preserve, so a better prompt would also run more tests, at
least every test that is related to ECSV-I/O behaviour.

:::

### Matplotlib issue 32178

[`legend_handles`](https://matplotlib.org/stable/api/legend_api.html) does not
let a user change the face colour of a
[scatter-plot](https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.scatter.html)
legend. The reporter stated that the older `legendHandles` route allowed this
'some years ago'.

#### The issue

The [original issue](https://github.com/matplotlib/matplotlib/issues/32178) is
included here (as of 2026-08-20).

````markdown
### Bug summary

I had this code some years ago to changes the dot color in the legend of a scatter plot:
```python
legend = plt.gca().get_legend()
legend.legendHandles[0].set_color(plt.cm.viridis(.5))
```
I have to redo these old figs, and after changing `legendHandles` to the new `legend_handles`, I find myself unable to change the `facecolor`. Neither `set_facecolor`, `set_color`, `set(facecolor=<>)` or `set(color=<>)` work.

### Code for reproduction

```Python
import matplotlib.pyplot as plt

plt.scatter([0,1],[0,1],c=[0,1],label='test')
plt.colorbar()
plt.legend()
legend = plt.gca().get_legend()
legend.legend_handles[0].set(color=plt.cm.viridis(.5))
plt.show()
```

### Actual outcome

Only the edge color is changed (or nothing if using `facecolor`).
<img width="1920" height="1440" alt="Image" src="https://github.com/user-attachments/assets/5bbb03ca-2c45-4bb6-9be4-df5100dd8355" />

### Expected outcome

Face color should be changed.

### Additional information

It worked in 2022 (not sure of the matplotlib version back then).

### Operating system

Arch

### Matplotlib Version

3.11.0

### Matplotlib Backend

_No response_

### Python version

_No response_

### Jupyter version

_No response_

### Installation

Linux package manager
````

#### The prompt to be evaluated

You can assume the above issue is saved in a file called
`iss_matplotlib_32178.md` when evaluating the prompt.

::::challenge

Read the matplotlib issue #32178 which is saved in the file
`@iss_matplotlib_32178.md`.

There is a backwards-compatibility issue as the face colour of a scatter plot
can be changed by using the old `legend.legendHandles` but not the new
`legend.legend_handles`. After the fix, the face colour of a scatter plot
should be able to be changed by `legend.legend_handles`.

Reproduce the case in the reported issue and examine whether the face colour of
the legend has been changed or not. Then trace how the returned object from
`legend.legend_handles` handles the change of face colour. Since the old
way worked, this is most likely a small bug introduced during some kind of
refactoring, so identify the source of the problem and provide a minimal fix
without digging too much into the behaviour of older matplotlib releases.

Add a test that checks the face colour of the legend handle after calling
`set_facecolor`. Limit the fix to scatter plot and do not change the behaviour
of other plots.

::::

#### Comments of the above prompt

:::solution

##### Strengths

The prompt first directs the coding agent to reproduce the issue, then trace
from the reported interface, which is more efficient than searching the entire
codebases. The specified scope also limits the change to face colour of scatter
plot itself, which prevents accidental change to other features of scatter
plot.

##### Weaknesses

The prompt asserts the bug as a regression, but the issue only says an old API
was used years ago, which does not guarantee this is about backwards
compatibility. Could the usage of the new API have changed? Could there be a
new way to change the face colour of the scatter plot now? It may be already
documented and rather than prompting the coding agent to implement some kind of
fix, it would be more appropriate to do some detailed investigation first to
find out any reason why the new API is not working now. Fixing it directly may
potentially turn a documented workaround or documentation fix into an
incompatible change which will impact other places of the codebase.

Even if this is about backwards compatibility, comparing older releases is a
direct way to locate the exact change that broke it, and the prompt forbids
that route while guessing 'a small bug introduced during some kind of
refactoring'. Sometimes it is understandable to limit the investigation
intentionally to save some tokens and not to bloat the context window, but the
point is to leave the coding agent free to consult whichever evidence settles
whether the behaviour is intended, whether that is the current implementation,
the documentation, or the release history.

Perhaps the most important defect is that the prompt does not specify how the
coding agent can verify if the legend face colour has been changed. Colour
change is a visual effect and although modern coding agents become increasingly
capable of inspecting colour of an image, it is not totally reliable. The
minimal example ends in `plt.show()`, and without a non-interactive backend
such as Agg and a programmatic colour check, the coding agent cannot work
autonomously in a reliable way. Any visual output should always be verified
through a non-interactive route when working with a coding agent.

:::

### scikit-learn issue 34622

[`explained_variance_score`](https://scikit-learn.org/stable/modules/model_evaluation.html#explained-variance-score)
in scikit-learn reports a perfect score for a one-sample prediction no matter
how wrong it is. As a result
[Leave-One-Out](https://scikit-learn.org/stable/modules/cross_validation.html#leave-one-out)
cross-validation scored with it silently reports a perfect model even on pure
noise.

#### The issue

The [original issue](https://github.com/scikit-learn/scikit-learn/issues/34622)
is included here (as of 2026-08-20).

````markdown
### Describe the bug

`explained_variance_score` returns `1.0` (a perfect score) for a single sample no
matter how wrong the prediction is, and emits no warning. `r2_score`, its
documented sibling, guards the same degenerate case with an
`UndefinedMetricWarning` and returns `nan`.

With one sample there is no variance to explain, so the numerator
(`Var(y_true - y_pred)`) and the denominator (`Var(y_true)`) are both `0`, and
`force_finite` maps `0/0` to `1.0` — documented as "perfect predictions" — even
when the prediction is plainly wrong.

The practical consequence is that **`LeaveOneOut` cross-validation scored with
`explained_variance` silently reports a perfect 1.0 for any model, including one
fit on pure noise.**

### Steps/Code to Reproduce

```python
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.metrics import explained_variance_score, r2_score
from sklearn.model_selection import LeaveOneOut, cross_val_score

# 1) single sample, clearly wrong prediction
print(explained_variance_score([1.0], [2.0]))  # 1.0, no warning
print(r2_score([1.0], [2.0]))                  # nan + UndefinedMetricWarning

# 2) the practical impact: LeaveOneOut on pure noise
rng = np.random.RandomState(0)
X, y = rng.normal(size=(12, 3)), rng.normal(size=12)

ev = cross_val_score(LinearRegression(), X, y, cv=LeaveOneOut(),
                     scoring="explained_variance")
r2 = cross_val_score(LinearRegression(), X, y, cv=LeaveOneOut(), scoring="r2")
print(ev.mean())  # 1.0   <- perfect score on noise, silently
print(r2.mean())  # nan   <- correctly undefined
```

### Expected Results

Something that does not claim a perfect fit on a single sample. Matching
`r2_score` seems most consistent: emit `UndefinedMetricWarning` and return `nan`
when `n_samples < 2`, since the metric is not defined without variance to
explain.

### Actual Results

```
1.0        # explained_variance_score([1.0], [2.0]) -- no warning
nan        # r2_score([1.0], [2.0]) -- UndefinedMetricWarning

1.0        # LeaveOneOut + explained_variance on pure noise
nan        # LeaveOneOut + r2 on pure noise (12 UndefinedMetricWarnings)
```

### Versions

```
System:
    python: 3.14
Python dependencies:
    sklearn: 1.9.0
```

### Notes

I appreciate that ignoring a systematic offset is intentional for this metric
(the docstring notes it), so the `n_samples >= 2` behaviour is not in question
here — only the single-sample case, where there is no variance at all and the
score is silently reported as perfect.

I'm happy to open a PR adding the same `n_samples < 2` guard that `r2_score`
already uses, if that's the direction you'd prefer.

````

#### The prompt to be evaluated

You can assume the above issue is saved in a file called `iss_sklearn_34622.md`
when evaluating the prompt.

::::challenge

Read the scikit-learn issue #34622 which is saved in the file
`@iss_sklearn_34622.md`.

As illustrated in the issue, the behaviour of `explained_variance_score` and
`r2_score` diverges and the fix should align them for consistency. Write a
minimal test to reproduce the current behaviour of `explained_variance_score`
with a single sample. After that, inspect how `r2_score` handles the same input
and use it as a reference implementation to come up with a patch for
`explained_variance_score`. Keep the change focused and do not perform
unnecessary refactoring. After the fix, the added minimal test should be passed
and the behaviour of `explained_variance_score` and `r2_score` should be the
same when the input is one-sample.

::::

#### Comments of the above prompt

:::solution

##### Strengths

The prompt instructs the coding agent to write a failing test of the current
one-sample behaviour, which is helpful as a signal of whether a patch actually
fixes the issue. It also points the coding agent at `r2_score`, a relevant
function for comparison, rather than letting the coding agent explore the
whole codebase, which may use more tokens than necessary. By looking only at
the implementation of `explained_variance_score` and `r2_score`, it limits the
scope of the work to avoid unnecessary refactoring.

##### Weaknesses

One key decision the prompter made is that it asserts the two metrics must be
aligned 'for consistency', which settles a public behaviour that is not
clear from the issue itself. In other words, an incorrect one-sample
prediction from `explained_variance_score` does not by itself require it to
have identical behaviour with `r2_score`. As changing a public behaviour
impacts users, it is usually wiser to engage with the maintainers of the
codebase first.

Hence, treating `r2_score` as a 'reference implementation' goes further than a
behavioural comparison and encourages the coding agent to copy all behaviour
before checking existing tests, documentation, and compatibility expectations
etc.

The verification could be more comprehensive, as it should include both
a wrong and perfect one-sample prediction, together with a normal `n >= 2`
sample case.

:::

### pytest issue 14720

pytest's [`--tb`](https://docs.pytest.org/en/stable/how-to/output.html) option
changes not only how a traceback is displayed but also which location data is
retained in `report.longrepr`. Plugins that suppress the standard output
therefore lose the data they need to build their own tracebacks.

#### The issue

The [original issue](https://github.com/pytest-dev/pytest/issues/14720) is
included here (as of 2026-08-20).

````markdown
A reporting plugin that wants to render its own compact traceback has to stop pytest printing the standard one. The obvious way is `--tb=no`. But `--tb` is consulted when the representation is *built*, not only when it is displayed, so this also removes the data the plugin wanted to read.

Reproduced on 9.1.1 and 8.4.2.

```python
# test_repro.py
def inner(): raise ValueError("boom")
def middle(): inner()
def test_a(): middle()
```

```python
# conftest.py
def pytest_runtest_logreport(report):
    if report.failed:
        rt = getattr(report.longrepr, "reprtraceback", None)
        entries = getattr(rt, "reprentries", []) if rt else []
        located = [e for e in entries if getattr(e, "reprfileloc", None) is not None]
        print(f"entries={len(entries)} located={len(located)} "
              f"len(str(longrepr))={len(str(report.longrepr))}")
```

```
--tb=long    entries=3  located=3  len(str(longrepr))=441
--tb=short   entries=3  located=3  len(str(longrepr))=290
--tb=line    entries=3  located=0  len(str(longrepr))=20
--tb=no      entries=3  located=0  len(str(longrepr))=20
```

The entry count survives, but `reprfileloc` is `None` under `line` and `no`, so the file and line of every frame are gone. A plugin reading `reprtraceback` to build its own summary sees three anonymous entries.

The result is that a plugin must choose between pytest printing a traceback it does not want, and having no traceback data to work with.

We hit this in a reporting plugin and shipped a version that silently lost all frame information: setting `--tb=no` in `pytest_configure` looked like a clean way to suppress output, and the summaries kept rendering, just without any of the frames they were supposed to summarise. The workaround is to leave `--tb` alone and suppress the *rendering* through some other mechanism, which works, but which mechanism is available is not obvious.

This may well be intentional — not building what will not be shown is a reasonable optimisation. If so, it would be worth documenting, since the flag reads as purely presentational. If not, separating "what representation to build" from "what to display" would let plugins summarise tracebacks without either fighting the terminal reporter or losing the data.

Happy to work on a patch if there is a direction the team would prefer.

````

#### The prompt to be evaluated

You can assume the above issue is saved in a file called `iss_pytest_14720.md`
when evaluating the prompt.

::::challenge

Read the pytest issue #14720 which is saved in the file
`@iss_pytest_14720.md`.

As I am not an expert in `pytest`, my understanding of the reported issue could
be incomplete so review critically and deeply my current understanding:

<my_understanding>
Downstream pytest plugins may require information such as the files and the
exact line numbers for every frame in a traceback even when a user passes the
flag `--tb=no`. It could be a good idea to include all the necessary
information despite `--tb=no` is passed so downstream plugins can parse
accordingly. The current output from the terminal when `--tb=no` is passed
should be retained.
</my_understanding>

After reviewing my understanding, provide a detailed explanation of the output
from the minimal example in the reported issue, and how it connects and
illustrates the problem.

Then, reproduce what was illustrated in the minimal example with `--tb=long`
and `--tb=no`. Before formulating a plan for the fix, explore the following:

- how and where `pytest` constructs the traceback
- how different options to `--tb` change the construction of the traceback
- where the output to the terminal is rendered

With the above knowledge, provide a detailed plan with a to-do list which
regardless of the options of `--tb`, constructs the full traceback for every
exception raised so downstream plugins can utilise all the information such as
the file and line number of the error. The options to `--tb` should only affect
the printing of the output.

Preserve all the existing relevant behaviour and ask me for any clarification
if needed.

::::

#### Comments of the above prompt

:::solution

##### Strengths

Letting an LLM evaluate your own understanding about the issue not only
allows it to correct any misconception or invalid assumption you might have,
but also gives it the chance to explore both the issue and the codebase, which
are both very helpful before attempting to change anything. Using an XML tag
also provides a structure to the prompt.

The prompt provides a clear roadmap to approach the issue: reproducing and
understanding the minimal example with both `--tb=long` and `--tb=no`, then
inspecting how the traceback is constructed and rendered. Asking a coding agent
for a detailed plan before any editing is a very common way of working with
agents as it allows you to review what it would do.

The prompt also sets a clear constraint: the printing of the output should only
be affected by the option passed to `--tb`, which is the current behaviour.
Whether a downstream plugin needs all the data from the traceback is
independent of the printed output.

##### Weaknesses

While the prompt makes it clear that the printed output and the information
provided to plugins should be separated, the chosen implementation of always
constructing the full traceback is a design decision whose impact is not
fully analysed. For example, it could change memory usage or affect
performance. The issue itself also admits not constructing the full traceback
with `--tb=no` could be the intended behaviour. One helpful thing a coding
agent can do is to explore documentation, commit history and other GitHub
issues to see if there were any related discussion. Of course, it is always
good to engage with the maintainers as well.

Although the prompt says that all existing relevant behaviour should be
preserved, it does not request any verification or specify how it should
verify. Could changing the construction of the traceback affect other plugins
that depend on a not fully constructed traceback? Could it change the way other
functions or methods within `pytest` parse the traceback? The prompt can direct the coding
agent to investigate more about the impact and suggest appropriate verification
tests if the prompter does not have a clear picture of this.

:::

### SymPy issue 29731

SymPy's symbolic
[summation](https://docs.sympy.org/latest/modules/concrete.html#sympy.concrete.summations.Sum)
can incorrectly simplify an expression to zero. Substituting a value for the
symbol produces a non-zero result for the same expression.

#### The issue

The [original issue](https://github.com/sympy/sympy/issues/29731) is included
here (as of 2026-08-20).

````markdown
In the following example, a summation evaluates as 0 even though it can be nonzero depending on the variable:

```
In [1]: def stirling(k, j):
   ...:     # Stirling numbers of the second kind.
   ...:     i=symbols('stirling_i',integer=True)
   ...:     return Sum(binomial(j,i)*(-1)**(j-i)*i**k,(i,0,j))/factorial(j)
   ...:

# Always zero even though n can be 1
In [2]: stirling(1,n).doit()
Out[2]: 0

# Correct result if 1 is substituted for n
In [3]: stirling(1,n).subs(n,1).doit()
Out[3]: 1
```
````

#### The prompt to be evaluated

You can assume the above issue is saved in a file called `iss_sympy_29731.md`
when evaluating the prompt.

::::challenge

Read the sympy issue #29731 which is saved in the file `@iss_sympy_29731.md`.

Reproduce the example included in the issue. Some imports are apparently
missing, so consult the sympy official documentation or the source code for the
necessary imports. The issue also does not specify the version of which sympy
was ran, so we can assume this happened on the latest commit.

The issue also does not say what the expected output of `stirling(1,n).doit()`
should be and I know nothing about what a Stirling numbers of the second kind
is, so explain in depth to me what it is and the expected output of
`stirling(1,n).doit()`. I only know that the result should be 1 when `n` is
substituted with 1.

I suspect somehow `Sum` got incorrectly simplified to 0, as most probably
`factorial(j)` won't be the source of the problem. Reduce the `Sum` expression
in the function `stirling`, trace its evaluation and simplification path,
identify the step at which a condition on `n` is incorrectly discarded. After
identifying such step, write a minimal test that illustrates the issue.

Then proceed to fix the incorrect simplification. Keep the change minimal and
do not change unrelated simplification rules. Do not investigate other
operations that use this simplification as I suppose `Sum` is used in a lot of
different places and I want to limit the scope of the change. Run the failed
test and after the fix, the test should pass.

::::

#### Comments of the above prompt

:::solution

##### Strengths

The prompt compares the general and the substituted result, stating that there
is a lost condition and it asks for reduction and tracing before editing, which
is very reasonable in this case. Reducing a complex expression often helps
isolating a problem and writing focused tests. Asking for an explanation of
Stirling numbers of the second kind is also helpful to understand the context
of the problem.

##### Weaknesses

'Do not investigate other operations that use this simplification'
unnecessarily limits the analysis required. Limiting the scope of the change is
sensible, but limiting the scope of the investigation when the root cause is
not yet clear means the coding agent cannot freely explore all the viable
possibilities.

The prompt also asserts the cause prematurely, that `Sum` was incorrectly
simplified and `factorial(j)` 'won't be the source' before any tracing has
happened. While this may well be justified in this case, directing the coding
agent to confirm any hypothesis will provide concrete evidence.

If '`Sum` is used in a lot of different places' is true, then running only the
failed test is certainly not enough as it is unclear whether the fix will
impact other usages.

:::

## Conclusion

:::callout{variant="keypoints"}

- Good prompt design is not simply about writing a longer one or dumping every
  detail to it.
- Provide relevant context for the problem you are addressing.
- Specify success conditions which are observable by coding agents rather than
  just asking for a fix.
- State your own understanding and knowledge gaps openly so the coding agent
  can correct or fill them (coding agents won't judge you!), instead of
  asserting a suspected cause as fact.
- Direct the coding agent to confirm any hypothesis with concrete evidence.
- Set clear boundaries for the change, and try not to limit the investigation
  needed to find and validate the root cause, especially if it is unclear.
- Leave implementation choices open unless it is trivial or you are absolutely
  certain something should be done in a particular way.
- Communicating with code maintainers is always encouraged whenever possible,
  especially on unresolved decisions that are public facing or substantial
  changes.

:::
