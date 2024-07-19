---
name: Continuous Integration with GitHub Actions
dependsOn: [technology_and_tooling.unit_testing.unit_testing_python]
tags: [python, unit-testing, ci, github]
---

# Continuous Integration

Continuous Integration (CI) is the process where when code changes are merged
into the `main` branch, automated builds and tests are run by the CI platform,
and developers notified of any errors. This process encourages developers to
have a standardised tooling for running tests and can help prevent code with
errors from getting merged into the codebase.

In this episode we will look at how GitHub works as a CI platform, and how to
specify tests that run on every push and pull request.

## GitHub Actions

GitHub Actions is the name of the CI platform in GitHub. GitHub Actions is
configured by workflow files in YAML format, which are in the
`.github/workflows` folder. Each workflow file within the workflows folder is a
separate GitHub Action and can have different triggers, such as running only
when certain file types are updated, or running on a schedule.

If you look in the `.github/workflows` folder, you will find a YAML file with
the following contents:

```yaml
name: "Unit tests"

on:
  push:
    branches:
      - main
  pull_request:
    branches:
      - "**"

jobs:
  build-and-test:
    name: Unit tests
    runs-on: ubuntu-20.04

    steps:
      - name: checkout repository
        uses: actions/checkout@v3

      - name: Set up Python 3.9
        uses: actions/setup-python@v4
        with:
          python-version: 3.9
          architecture: x64

      - name: install packages
        run: |
          python -m pip install --upgrade pip setuptools wheel
          python -m pip install -e .

      - name: python unit tests
        run: |
          pytest -v
```

The three sections of a workflow file are:

- **Metadata**: This is the first block and specifies global action metadata.
  Only the `name` key is generally specified.
- **Triggers**: This is specified using the `on` block. Usual triggers are
  pushes to the `main` branch and all pull requests. There are a variety of
  triggers available, including [triggering on
  tags](https://docs.github.com/en/actions/using-workflows/triggering-a-workflow#example-including-branches-and-tags),
  including triggering [only when certain files
  change](https://docs.github.com/en/actions/using-workflows/triggering-a-workflow#using-filters-to-target-specific-paths-for-pull-request-or-push-events)
  (useful in a large codebase, when you don't want to run all tests), and
  [scheduled
  triggers](https://docs.github.com/en/actions/using-workflows/events-that-trigger-workflows#schedule).
- **Jobs**: Multiple jobs can be run in parallel within a workflow. Each job has
  at least a `runs-on` key which specifies the [operating system image it runs
  on](https://github.com/actions/runner-images#available-images), a readable
  `name` key and a list of `steps`.

## Job steps

Each `step` can in turn refer to other GitHub Actions by using the `uses` key,
or run commands in the shell by using the `run` key. When a `uses` key has a
value like `actions/checkout@v3` it refers to a GitHub Action at
<https://github.com/actions/checkout> and specifies the version number of the
action, here `v3`. These actions repositories allow one to re-use workflows.
Some workflows, like
[`actions/setup-python`](https://github.com/actions/setup-python) take
parameters, in this case, `python-version` which specifies which Python
interpreter version to use, and `architecture` that specifies we want to use the
64-bit version of the interpreter.

::: callout
YAML treats numbers in values as floating point, so it is best to surround
version numbers in quotes, to avoid errors like 3.10 being treated the same way
as 3.1
:::

We can also run commands in the shell by using the `run` key:

```yaml
run: |
  python -m pip install --upgrade pip setuptools wheel
  python -m pip install -e .
```

We use the `|` character after run which indicates that line breaks are
significant ([YAML 1.2.2, §2.3](https://yaml.org/spec/1.2.2/#scalars)) and
specify the commands to run after that. We can break the script into separate
steps, execution proceeds to the next step only when all the previous steps in
the job succeed. Here we install package dependencies and subsequently run
`pytest` to test our code.

## Notifications

If the steps are running as part of a pull request, GitHub Actions will inform
us in the pull request page whether the steps succeeded. If the steps are
running on push, then a circular indicator will appear next to the commit
message in the GitHub web interface, turning into a tick if the steps succeeded,
and a red cross if the steps failed.

You can set up e-mail notifications for GitHub Actions failures in your profile
settings (Notifications > System section). Note that turning this setting on
only notifies you of workflows that you created.

The Actions tab in your GitHub repository shows all currently running and past
actions, as well as their output logs, which are useful in diagnosing errors.

## Status badges

Status badges are a quick way to see whether tests in your repository are
passing or not. These are usually added to the top of your README.md and
automatically update. You can find the status badge corresponding to a
particular workflow by going to Actions > Your Workflow, clicking the [...] icon
next to _Filter workflow runs_ and clicking _Create status badge_.

::: exercise
Add the unit tests status badge to README
:::

## Next steps

- Write some unit tests!
- Once you have set up the infrastructure it’s very easy, especially for the
  majority of testing.
- There is complexity: how to test random functions, for instance.
- Don’t fall into the trap of thinking “I could be using this time to write more
  features”. Code worthless if you aren’t certain it’s correct!
- Encourage others in your research group to start testing their code.
