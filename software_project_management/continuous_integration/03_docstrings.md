---
name: Documenting your code
dependsOn: []
tags: [github]
learningOutcomes:
  - Check that every public function in a project has a docstring.
  - Explain why a missing docstring is not reported by a passing test suite or by code coverage.
  - Run one tool with two different rule selections for two different purposes.
attribution:
  - citation: This course material was developed as part of UNIVERSE-HPC, which was funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Another kind of coverage

Code coverage asked: which lines do the tests run?

There is a second question worth asking of the same codebase: which functions has anybody bothered to document?
Nothing we have run so far tells us.
The tests pass whether or not a function is documented, and so does the coverage report.

In the next section you will generate API documentation automatically from the docstrings in your code.
That only works for the functions that have one.

## Checking docstrings with ruff

Fortunately, we can use a tool we're already familiar with.
Ruff's rule set includes `pydocstyle`, under the `D` prefix, which checks docstrings rather than code.

These rules are deliberately not part of the lint step you wrote earlier.
Linting the code and checking the documentation are deliberately done at different points.
Mixing them means a missing docstring could block you while you are trying to fix a real bug.

Run the check:

```bash
ruff check --select D src/
```

Two points to note.

`--select D` picks the docstring rules instead of the default selection in `pyproject.toml`.

`src/` scopes the check to the package.
Run it over the whole repository and it reports your tests as well, which do not necessarily need docstrings: a test's name is usually a good-enough description, and `"""Test that it works."""` above `test_that_it_works` doesn't add much.

## Configuring the rules

Look in `pyproject.toml`:

```toml
[tool.ruff.lint.pydocstyle]
convention = "numpy"
```

There are several common docstring styles, and the rules for one contradict the rules for another.
`convention` tells ruff which one this project uses, and switches off the rules belonging to the others.

This matters quite a bit.
Without that line the same command reports 15 problems on this project, nearly all of them complaining that perfectly good docstrings are laid out in the wrong style.
With it, you get the one that is real.
It's a good idea to pick a style and stick with it: don't spend ages agonising over which style you like the most.
The most important thing is that you're running a tool that will tell you useful information about jobs you've forgotten to do.

## Automating it

::::challenge{id="docstring-workflow" title="Add a docstrings workflow"}

Write a workflow, `docstrings.yml`, that runs this check on every push and pull request.

One job, one Python version, no matrix: you are checking the source, and the source does not change between Python versions.

:::solution

```yml
name: Docstrings

on:
  push:
    branches: ["main"]
  pull_request:
    branches: ["main"]
  workflow_dispatch:

jobs:
  docstrings:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v7

      - name: Set up Python 3.13
        uses: actions/setup-python@v7
        with:
          python-version: "3.13"

      - name: Install dependencies
        run: |
          python -m pip install .[dev]

      - name: Check every public thing has a docstring
        run: |
          ruff check --select D src/
```

:::
::::

## Fixing the finding

The check fails with one finding:

```text
D103 Missing docstring in public function
  --> src/ci_course/stats.py:41:5
```

::::challenge{id="document-count-missing" title="Document the undocumented function"}

Write a docstring for `count_missing`, in the same style as the rest of the package, and get the check passing.

:::solution

Match the numpydoc style used by its neighbours:

```python
def count_missing(values):
    """
    Count the missing values in a column of measurements.

    Parameters
    ----------
    values : array_like
        The measurements to inspect. Missing values should be ``nan``.

    Returns
    -------
    int
        The number of missing values.
    """
```

:::
::::

## Why this finding mattered

`count_missing` was not dead code.
It is exported from the package, it has a test, and that test passes.
By every measure you have applied so far, it was fine.

Keep that in mind during the next section, where we will use a tool called Sphinx to generate an API reference from these docstrings.
Sphinx documents the things that have a docstring.
A function without one is not flagged, not marked incomplete, and not listed: it is simply absent, and the generated documentation looks complete.

## Next steps

1. \[optional\] read the [ruff pydocstyle rules](https://docs.astral.sh/ruff/rules/#pydocstyle-d), particularly the `D1` group, which is the "is there one at all" family.

1. \[optional\] look at [numpydoc](https://numpydoc.readthedocs.io/en/latest/format.html), the docstring style this project uses.
