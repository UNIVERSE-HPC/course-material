---
name: Code Coverage
dependsOn: [
  software_project_management.continuous_integration.github_actions
]
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
    - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1 
      url: https://www.universe-hpc.ac.uk
      image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
      license: CC-BY-4.0


---

## Testing code coverage

In this session we will be using GitHub actions along with a service called [Codecov](https://about.codecov.io/).

Code coverage is a measure used in software testing.
It describes the degree to which the source code of a program is tested by a particular test suite.
It's a way to check that all paths through the code – all functions, methods, lines, etc. – are exercised by the tests.

There are various types of code coverage, including:

1. **Function coverage:** Has each function (or subroutine) in the program been called?
2. **Statement coverage:** Has each line of the source code been executed?
3. **Branch coverage:** Has each branch (also called DD-path) of each control structure (such as in if and case statements) been executed?
4. **Condition coverage:** Has each boolean sub-expression evaluated both to true and false?

Statement coverage is probably the most common of these, and that will be our focus right now.

There are many benefits of testing code coverage:

1. **Identify untested parts of code:** Code coverage allows developers to understand which parts of the code are not covered by tests. It brings attention to areas where bugs may be lurking unnoticed.

1. **Improve code quality:** A high percentage of code coverage can indicate that the software has a lower chance of containing undetected software bugs.

1. **Prevent regressions:** Coverage can help you prevent regressions – instances where a change breaks existing functionality – by making sure that all parts of the code are tested.

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

Next to your repository, you should see text like:

> Not yet enabled [setup repo]

You will see a token of the form `CODECOV_TOKEN=XXXXXXXXXX`.
This token will allow your GitHub Actions workflow to communicate with Codecov.
It is not strictly needed if your repository is public, but let's run through the process of setting it up anway.

1. To to your repository on GitHub
1. Go to *Settings*
1. On the left, go to *Secrets and variables* then to *Actions*
1. Click *New repository secret*
1. Set the name to `CODECOV_TOKEN` and the value to the token string, copied from Codecov
1. Click *Add secret*

This will allow you to use the token in a GitHub Actions workflow, without anyone who looks at your workflow seeing what the token is.
We will see shortly how this is used.

Your repository is now set up and ready to use with Codecov.

## Creating coverage data

We will use the [pytest-cov](https://pytest-cov.readthedocs.io/en/latest/) tool to generate coverage information when running our unit tests with pytest.
To install this locally, using pip, run:

~~~ bash
pip install pytest-cov
~~~

We can now run pytest in the following way to generate an xml file containing coverage information for the whole project:

~~~ bash
pytest --cov-config=.coveragerc --cov=./ci_course --cov-report=xml
~~~

Let's break that down:

- `--cov-config=.coveragerc`: This option specifies the configuration file for coverage reporting. In this case, `.coveragerc` is a configuration file which you can look at in the repository. It is not strictly necessary, as tests are usually filtered out by default. But if you have a more complex project, it can be useful to specify a more complicated configuration.

- `--cov=./ci_course`: The `--cov` option is used to specify the directory to measure coverage for. In this case, `./ci_course` means that coverage should be measured for the `ci_course` directory in the current directory.

- `--cov-report=xml`: This option specifies the type of the report to generate and in this case, it generates an XML report.

So, in simple terms, this command is saying: "Run pytest, use the settings in the `.coveragerc` file to measure code coverage for the `ci_course` directory in the current directory, and then generate an XML report of the code coverage."

::::challenge{id="run-coverage-locally" title="Run coverage locally"}

With your project checked out, ensure it is installed in a virtual environment.
Then, run the pytest command above and verify that it generates a `coverage.xml` file.

:::solution

The file should contain something along these lines:

~~~ xml
<?xml version="1.0" ?>
<coverage version="7.2.5" timestamp="1684770290392" lines-valid="14" lines-covered="13" line-rate="0.9286" branches-covered="0" branches-valid="0" branch-rate="0" complexity="0">
 <!-- Generated by coverage.py: https://coverage.readthedocs.io/en/7.2.5 -->
 <!-- Based on https://raw.githubusercontent.com/cobertura/web/master/htdocs/xml/coverage-04.dtd -->
 <sources>
  <source>/home/runner/work/test_ci/test_ci/ci_course</source>
 </sources>
 <packages>
  <package name="." line-rate="0.9286" branch-rate="0" complexity="0">
   <classes>
    <class name="__init__.py" filename="__init__.py" complexity="0" line-rate="1" branch-rate="0">
     <methods/>
     <lines>
      <line number="1" hits="1"/>
      <line number="2" hits="1"/>
      <line number="4" hits="1"/>
     </lines>
    </class>
    <class name="functionality.py" filename="functionality.py" complexity="0" line-rate="0.9091" branch-rate="0">
     <methods/>
     <lines>
      <line number="1" hits="1"/>
      <line number="4" hits="1"/>
      <line number="18" hits="1"/>
      <line number="21" hits="1"/>
      <line number="35" hits="1"/>
      <line number="36" hits="0"/>
      <line number="38" hits="1"/>
      <line number="39" hits="1"/>
      <line number="40" hits="1"/>
      <line number="41" hits="1"/>
      <line number="43" hits="1"/>
     </lines>
    </class>
   </classes>
  </package>
 </packages>
</coverage>
~~~

:::
::::

## Use GitHub Actions to automate this process

We want to generate coverage, and upload the results to Codecov, every time someone commits or opens a pull request.

::::challenge{id="run-coverage-on-github" title="Run coverage on GitHub"}

Write a GitHub Actions workflow that generates a coverage report, and uploads it to codecov.
You will find the following step helpful:

~~~ yml
- name: Upload coverage reports to Codecov
  uses: codecov/codecov-action@v3
  with:
    token: ${{ secrets.CODECOV_TOKEN }}
    fail_ci_if_error: true
    files: coverage.xml
~~~

You can read more about the `codecov-action` step here:
[https://github.com/codecov/codecov-action](https://github.com/codecov/codecov-action)

:::solution

The file should contain something along these lines:

~~~ yml
name: Coverage

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]
  workflow_dispatch:

jobs:
  build:

    runs-on: ubuntu-latest

    steps:

    - uses: actions/checkout@v3

    - name: Set up Python 3.10
      uses: actions/setup-python@v3
      with:
        python-version: "3.10"

    - name: Install dependencies
      run: |
        python -m pip install --upgrade pip setuptools wheel
        python -m pip install .[dev]

    - name: Run coverage
      run: |
        pytest --cov-config=.coveragerc --cov=./ci_course --cov-report=xml
        cat coverage.xml

    - name: Upload coverage reports to Codecov
      uses: codecov/codecov-action@v3
      with:
        token: ${{ secrets.CODECOV_TOKEN }}
        fail_ci_if_error: true
        files: coverage.xml
~~~

:::
::::

Once you have committed this workflow file, go to GitHub and check that it has run successfully.

If it has, go to Codecov and explore the coverage report.
It may take several minutes to finish processing.

## Next steps

::::challenge{id="improve-code-coverage" title="Improve code coverage"}

Identify which parts of the project are not covered, and write a new test to ensure every line is covered.

:::solution

Update `test_functionality.py`, for instace by adding the following line to `test_minimum()`:

~~~ python
assert ci_course.minimum("hi", "there") is None
~~~

:::

::::

::::challenge{id="add-branch-coverage" title="Add branch coverage"}

Turn on branch coverage, and see how that affects your results on Codecov.

You can read about the pytest coverage configuration here:
[https://pytest-cov.readthedocs.io/en/latest/config.html](https://pytest-cov.readthedocs.io/en/latest/config.html)

:::solution

Add `--cov-branch` to your `pytest` command in your coverage workflow file.

:::

::::

- \[optional\] Read more about [encrypted secrets](https://docs.github.com/en/free-pro-team@latest/actions/reference/encrypted-secrets) in GitHub Actions.
- \[optional\] Read more about [environment variables](https://docs.github.com/en/free-pro-team@latest/actions/reference/environment-variables) in GitHub Actions.
