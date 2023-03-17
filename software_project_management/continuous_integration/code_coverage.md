---
name: Code Coverage
dependsOn: [
  software_project_management.continuous_integration.testing
]
tags: [codecov]
---



## 15:30 Testing code coverage

Code coverage is a metric used to describe how much code in a software project is hit when the unit tests are run.
In its simplest form, this is the proportion of the lines of code that get executed when you run the unit tests.

Code coverage is no guarantee that the unit tests are any good. But, nevertheless, code coverage is an important metric for helping to assess how well tested a codebase is.

In this section, you are going to calculate the code coverage of your Python project, which you will then visualise using a third party tool called [Codecov](https://codecov.io/).
Codecov is free for public repositories.

<iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/yc7hnL04fYs" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

1. Go to [https://codecov.io/](https://codecov.io/), log in with your GitHub account, and configure your project.
1. Copy the token, and set it as a secret in your repository, with the name `CODECOV_TOKEN`.
1. Create a new GitHub Actions workflow for running coverage.
   - You will need to add the following Python dependencies to your YAML file:
```
coverage codecov
```
   - Run coverage with the following block in your YAML file:
{% raw %}
    ```yaml
    - name: Run coverage
      run: |
        coverage run -m unittest
    ```
{% endraw %}
   - Run codecov with the following block in your YAML file:
{% raw %}
    ```yaml
    - name: Run codecov
      if: success()
      env:
        CODECOV_TOKEN: ${{ secrets.CODECOV_TOKEN }}
      run: |
        codecov
    ```
{% endraw %}
1. Find the status badge from [https://codecov.io/](https://codecov.io/) to display in your `README.md`.
1. Add a unit test to cover the remaining line in `functionality.py`, and check that your coverage goes up to 100%.
1. Read the contents of the `.coveragerc` file in your repository. What is it doing?
1. \[optional\] Read about [branch coverage](https://coverage.readthedocs.io/en/v4.5.x/branch.html). Turn branch coverage on in your coverage workflow. Check whether you have 100% branch coverage and, if not, fix it.
1. \[optional\] Read more about [encrypted secrets](https://docs.github.com/en/free-pro-team@latest/actions/reference/encrypted-secrets) in GitHub Actions.
1. \[optional\] Read more about [environment variables](https://docs.github.com/en/free-pro-team@latest/actions/reference/environment-variables) in GitHub Actions.
