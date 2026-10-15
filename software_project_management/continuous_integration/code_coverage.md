---
name: Code Coverage
dependsOn: [software_project_management.continuous_integration.github_actions]
tags: [codecov, github]
learningOutcomes:
  - Describe the types of code coverage.
  - List the benefits of code coverage.
  - Add and configure use of code coverage tool to our software project.
  - Run a code coverage tool to understand how much of our code is being tested using unit tests.
  - Configure GitHub Actions to automate the process of code coverage analysis over a code repository.
attribution:
  - citation: This material has been adapted from the "Software Engineering" module of the SABS R³ Center for Doctoral Training.
    url: https://www.sabsr3.ox.ac.uk
    image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which was funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Testing code coverage

In this session we will be using GitHub actions along with a service called [Codecov](https://about.codecov.io/).

Code coverage is a measure used in software testing.
It describes the degree to which the source code of a program is tested by a particular test suite.
It's a way to check that all paths through the code, meaning every function, method and line, are exercised by the tests.

There are various types of code coverage, including:

1. **Function coverage:** Has each function (or subroutine) in the program been called?
2. **Statement coverage:** Has each line of the source code been executed?
3. **Branch coverage:** Has each branch (also called DD-path) of each control structure (such as in if and case statements) been executed?
4. **Condition coverage:** Has each boolean sub-expression evaluated both to true and false?

Statement coverage is probably the most common of these, and that will be our focus right now.

There are many benefits of testing code coverage:

1. **Identify untested parts of code:** Code coverage allows developers to understand which parts of the code are not covered by tests. It brings attention to areas where bugs may be lurking unnoticed.

1. **Improve code quality:** A high percentage of code coverage can indicate that the software has a lower chance of containing undetected software bugs.

1. **Prevent regressions:** Coverage can help you prevent regressions, which are changes that break existing functionality, by making sure that all parts of the code are tested.

1. **Confidence during Refactoring:** It allows developers to refactor code or add new features with the confidence that they haven't broken existing functionality.

While code coverage is a valuable tool in software testing, it's not a silver bullet.
100% code coverage doesn't guarantee that your software is 100% bug-free or that it handles all possible use cases.
It's also possible for code to have high coverage but low test quality: it's still essential to write good quality tests.

We will be using a service called Codecov to automate the process of collecting and analyzing code coverage metrics.
Codecov is free for public repositories.

## Set up codecov

Go to [https://codecov.io/](https://codecov.io/), and log in with your GitHub account.

You may need to follow the steps required to grant Codecov access to see your repositories.
Once you have done that, you should see your repository on the Codecov webpage.

Next to your repository, you should see a 'Configure' button. Click this to set up code coverage:

1. Make sure 'Using GitHub Actions' is selected.
1. Select 'Pytest' from the menu of language options.
1. Select 'Repository token' to generate a secret just for this repository.

You will see a token of the form `CODECOV_TOKEN=XXXXXXXXXX`.
This token will allow your GitHub Actions workflow to communicate with Codecov.
It is not strictly needed if your repository is public, but let's run through the process of setting it up anyway.

1. Go to your repository on GitHub
1. Go to _Settings_
1. On the left, go to _Secrets and variables_ then to _Actions_
1. Click _New repository secret_
1. Set the name to `CODECOV_TOKEN` and the value to the token string, copied from Codecov
1. Click _Add secret_

This will allow you to use the token in a GitHub Actions workflow, without anyone who looks at your workflow seeing what the token is.
We will see shortly how this is used.

Your repository is now set up and ready to use with Codecov.

## Creating coverage data

We will use the [pytest-cov](https://pytest-cov.readthedocs.io/en/latest/) tool to generate coverage information when running our unit tests with pytest.
To install this locally, using pip, run:

```bash
pip install pytest-cov
```

We can now run pytest in the following way to generate an xml file containing coverage information for the whole project:

```bash
pytest --cov=ci_course --cov-report=xml
```

Let's break that down:

- `--cov=ci_course`: measure coverage of the `ci_course` package.

- `--cov-report=xml`: write the report as XML, which is the format Codecov expects. Leave this off and you get a summary in the terminal instead, which is more useful while you are working.

The rest of the configuration lives under `[tool.coverage.run]` in `pyproject.toml`, alongside the settings for ruff and pytest.
Keeping tool configuration in one file is worth doing: a project that scatters it across `.coveragerc`, `.flake8`, `setup.cfg` and `tox.ini` is a project where nobody is sure which file is in charge.

Try it without `--cov-report=xml` as well, and read the table it prints.
The `Missing` column is the interesting one.

::::challenge{id="run-coverage-locally" title="Run coverage locally"}

With your project installed in a virtual environment, run the command above and check that it produces a `coverage.xml` file.

Then run it again without `--cov-report=xml` and read the table.

:::solution

The table looks like this:

```text
Name                        Stmts   Miss  Cover   Missing
---------------------------------------------------------
src/ci_course/__init__.py       4      0   100%
src/ci_course/cli.py           14      0   100%
src/ci_course/io.py            25      3    88%   61-63
src/ci_course/report.py        14      0   100%
src/ci_course/stats.py         15      2    87%   25, 28
---------------------------------------------------------
TOTAL                          72      5    93%
```

Your line numbers may be slightly different depending on exactly how you fixed the earlier problems.

`coverage.xml` contains the same information in a machine-readable form.
It is not meant to be read by people, and you will not need to open it, but it is what you hand to Codecov in a moment.

Five statements are never executed by the tests.
Make a note of where they are: we come back to them at the end of this section.

:::
::::

## Use GitHub Actions to automate this process

We want to generate coverage, and upload the results to Codecov, every time someone commits or opens a pull request.

::::challenge{id="run-coverage-on-github" title="Run coverage on GitHub"}

Write a GitHub Actions workflow, `coverage.yml`, that generates a coverage report and uploads it to Codecov.
You will find the following step helpful:

```yml
- name: Upload coverage reports to Codecov
  uses: codecov/codecov-action@v7
  with:
    token: ${{ secrets.CODECOV_TOKEN }}
    fail_ci_if_error: true
    files: coverage.xml
```

You can read more about the `codecov-action` step here:
[https://github.com/codecov/codecov-action](https://github.com/codecov/codecov-action)

:::solution

The file should contain something along these lines:

```yml
name: Coverage

on:
  push:
    branches: ["main"]
  pull_request:
    branches: ["main"]
  workflow_dispatch:

jobs:
  coverage:
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

      - name: Run coverage
        run: |
          pytest --cov=ci_course --cov-report=xml

      - name: Upload coverage reports to Codecov
        uses: codecov/codecov-action@v7
        with:
          token: ${{ secrets.CODECOV_TOKEN }}
          fail_ci_if_error: true
          files: coverage.xml
```

:::
::::

Once you have committed this workflow file, go to GitHub and check that it has run successfully.

If it has, go to Codecov and explore the coverage report.
It may take several minutes to finish processing.

## Next steps

::::challenge{id="improve-code-coverage" title="Improve code coverage"}

Look at the lines Codecov reports as uncovered in `src/ci_course/stats.py`.

Write a test that covers the first of them, then look hard at what the test tells you.

:::solution

The uncovered branch is the one that handles a column where every value is missing:

```python
if present.size == 0:
    return {"n": 0, "mean": 0.0, "sd": 0.0, "min": 0.0, "max": 0.0}
```

A test that reaches it looks like this:

```python
def test_a_column_of_only_missing_values():
    summary = ci_course.summarise(np.array([np.nan, np.nan]))

    assert summary["n"] == 0
    assert summary["mean"] == 0.0
```

That test passes, and it should worry you.

The function reports that a column with no measurements in it has a mean of zero.
Zero is a measurement. If that summary reached a report, or a plot, or a paper, nothing anywhere would indicate that the number was invented.

So the right fix is not to write a test that accepts the current behaviour, but to decide what the function should do and make it do that.
Returning `nan` is reasonable, because that is what the rest of the code already uses for "no value here":

```python
if present.size == 0:
    return {"n": 0, "mean": float("nan"), "sd": float("nan"),
            "min": float("nan"), "max": float("nan")}
```

Raising an exception is also defensible. What is not defensible is quietly returning zero.

This is the real argument for measuring coverage.
Nobody had written that branch down as a thing to think about, and nothing failed.
Coverage did not find a bug; it pointed at the one piece of code that nobody had looked at, and a person found the bug.

:::

::::

::::challenge{id="cover-the-rest" title="Cover the rest"}

There are still uncovered lines in `src/ci_course/io.py`.
Work out what they are and whether they are worth a test.

:::solution

They are the error path: the branch that runs when a cell in the CSV cannot be read as a number.
The sample data is clean, so nothing exercises it.

A test needs a file with a bad value in it:

```python
def test_unparseable_values_are_recorded(tmp_path):
    csv = tmp_path / "bad.csv"
    csv.write_text("sample_id,height_m\nA,1.5\nB,not-a-number\n", encoding="utf-8")

    errors = []
    data = ci_course.load_measurements(csv, errors=errors)

    assert np.isnan(data["height_m"][1])
    assert len(errors) == 1
```

Worth noticing: this is the code path that the mutable default argument bug was hiding in.
The linter spotted it in the first section without any test reaching it, and only now does the test suite go anywhere near it.
Static analysis and coverage are looking for different things, which is why the course runs both.

:::

::::

::::challenge{id="add-branch-coverage" title="Add branch coverage"}

Statement coverage asks whether each line ran.
Branch coverage asks whether each decision went both ways: an `if` with no `else` can have every line executed while the false case has never once been taken.

Turn branch coverage on and see how your numbers change.

You can read about the configuration here:
[https://pytest-cov.readthedocs.io/en/latest/config.html](https://pytest-cov.readthedocs.io/en/latest/config.html)

:::solution

Add `--cov-branch` to the `pytest` command in your coverage workflow, or set it in `pyproject.toml`:

```toml
[tool.coverage.run]
branch = true
```

The percentage drops, because the denominator has got bigger: every branch now counts as something that can be missed.

A falling number after a change like this is not a regression. It means you are now measuring something you previously were not, and the gap was always there.

:::

::::

- \[optional\] Read more about [encrypted secrets](https://docs.github.com/en/free-pro-team@latest/actions/reference/encrypted-secrets) in GitHub Actions.
- \[optional\] Read more about [environment variables](https://docs.github.com/en/free-pro-team@latest/actions/reference/environment-variables) in GitHub Actions.
