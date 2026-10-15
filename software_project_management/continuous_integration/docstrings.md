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

There is a second question worth asking of the same codebase: which public functions has anybody bothered to describe?
Nothing you have run so far will tell you.
The tests pass whether or not a function is documented, and so does the coverage report.

In the next section you will generate API documentation automatically from the docstrings in your code.
That only works for the functions that have one.

## Checking docstrings with ruff

You already have the tool. Ruff's rule set includes `pydocstyle`, under the `D` prefix, which checks docstrings rather than code.

These rules are deliberately not part of the lint step you wrote earlier.
Linting the code and checking the documentation are different questions, asked at different points, and mixing them means a missing docstring blocks you while you are trying to fix a real bug.

Run the check:

```bash
ruff check --select D src/
```

Two details matter.

`--select D` picks the docstring rules instead of the default selection in `pyproject.toml`.
This is the same tool you have already used, pointed at a different question.

`src/` scopes the check to the package.
Run it over the whole repository and it reports your tests as well, which do not need docstrings: a test's name is its description, and `"""Test that it works."""` above `test_that_it_works` helps nobody.

## Configuring the rules

Look in `pyproject.toml`:

```toml
[tool.ruff.lint.pydocstyle]
convention = "numpy"
```

There are several common docstring styles, and the rules for one contradict the rules for another.
`convention` tells ruff which one this project uses, and switches off the rules belonging to the others.

This matters more than it sounds.
Without that line the same command reports 15 problems on this project, nearly all of them complaining that perfectly good docstrings are laid out in the wrong style.
With it, you get the one that is real.

A linter nobody has configured for the house style is a linter people learn to ignore, and a linter people ignore is worse than no linter at all, because it still costs time in CI.

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

## Why this one mattered

`count_missing` was not dead code.
It is exported from the package, it has a test, and that test passes.
By every measure you have applied so far, it was fine.

Keep that in mind during the next section, where Sphinx generates an API reference from these docstrings.
Sphinx documents the things that have a docstring.
A function without one is not flagged, not marked incomplete, and not listed: it is simply absent, and the generated documentation looks complete.

That is the failure mode worth remembering.
A test suite tells you when something is broken.
Documentation tooling, left alone, tells you nothing at all, and quietly produces a smaller result.

## Next steps

1. \[optional\] read the [ruff pydocstyle rules](https://docs.astral.sh/ruff/rules/#pydocstyle-d), particularly the `D1` group, which is the "is there one at all" family.

1. \[optional\] look at [numpydoc](https://numpydoc.readthedocs.io/en/latest/format.html), the docstring style this project uses.
