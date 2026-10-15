---
name: Build artifacts and approving a deployment
dependsOn: []
tags: [github]
learningOutcomes:
  - Save the output of a workflow as an artifact and download it for review.
  - Explain how an artifact belongs to a run, outlives the runner, and eventually expires.
  - Connect jobs with needs, so that one job uses what another one built.
  - Require a person to approve a deployment, and explain what separates Continuous Delivery from Continuous Deployment.
attribution:
  - citation: This course material was developed as part of UNIVERSE-HPC, which was funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Everything a run makes is thrown away

When a job finishes, its runner is destroyed, and everything on it goes too.

That has been fine so far, because every workflow you have written answers a yes or no question.
Did the linter pass?
Did the tests pass?
You only needed the answer, not anything the machine produced along the way.

There are two things that might change that.

The first is review.
Your documentation workflow builds HTML and publishes it when a change reaches `main`.
A reviewer looking at a pull request has no way to see what that HTML will look like until after they have approved it.
A green build says the documentation compiled, which is not the same as saying it reads well, the links go anywhere, or the new section appears at all.

The second is deployment.
Once you want a person to approve something before it goes live, the thing they approve has to still exist afterwards.

Both need the same mechanism.

## Artifacts

An **artifact** is a set of files saved out of a workflow run.

It belongs to the run rather than to the repository, so it is not committed and does not appear in your git history.
You will find it listed at the bottom of that run's own page in the _Actions_ tab.
It stays there after the runner is gone, so you can download it later, but not forever: artifacts are deleted after a retention period, which is 90 days by default and can be set for each upload.

### Previewing the documentation from a pull request

::::challenge{id="docs-preview-workflow" title="Build a documentation preview"}

Write a workflow, `docs_preview.yml`, that runs on pull requests, builds the Sphinx HTML, and uploads it as an artifact.

Use `actions/upload-artifact`, and give the artifact a name you will recognise.

This workflow does not publish anything, so it only needs read access to the repository.

:::solution

```yml
name: Documentation preview

on:
  pull_request:
    branches: ["main"]
  workflow_dispatch:

permissions:
  contents: read

jobs:
  docs-preview:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v7

      - name: Set up Python 3.13
        uses: actions/setup-python@v7
        with:
          python-version: "3.13"

      - name: Install dependencies
        run: |
          python -m pip install ".[docs]"

      - name: Build documentation
        run: |
          sphinx-build -W -b html docs docs/_build/html

      - name: Upload documentation preview
        uses: actions/upload-artifact@v7
        with:
          name: documentation-preview
          path: docs/_build/html
          if-no-files-found: error
          retention-days: 7
```

Two of those settings are worth explaining.

`if-no-files-found: error` fails the job if the path is empty.
Without it, a build that quietly produced nothing would upload an empty artifact and the job would still pass.
You would end up with no preview to look at, and nothing to tell you that it was missing.

`retention-days: 7` overrides the default of 90.
A preview is useful while the pull request is open and worthless afterwards, and artifacts count against your account's storage.

:::
::::

::::challenge{id="review-a-preview" title="Review a change before merging it"}

Make a change you can see: edit the text of a docstring on one of the public functions, or add a paragraph to the quickstart page.

Open a pull request. When the workflow finishes, scroll to the bottom of the run's summary page, download `documentation-preview`, unzip it, and open `index.html` in a browser.

Check that your change appears, reads correctly, and has not broken the formatting around it.

:::solution

You are looking at the real rendered output, built from the branch, before anything is merged or published.

This is the point of the exercise.
The build passing told you that Sphinx did not hit an error.
It could not tell you that you had written the wrong word, pointed a link at the wrong place, or indented a code block so that it swallowed the paragraph after it.
Somebody needs to look at the pages!

:::
::::

Artifacts are useful for failures too.
Sometimes a test produces a file that would help you understand why it failed: a log, a generated report, or an image that was supposed to match a reference.
Uploading that file from the workflow is often much quicker than trying to reproduce the whole failure on your own machine.

## Deploying what you reviewed

Now look again at the documentation workflow you wrote in [the documentation section](/software_project_management/continuous_integration/04_documentation).

It has one job, and that job checks out the code, builds the HTML, and publishes it.
Three problems follow from that.

It does not run the tests, so documentation can be published from a commit that is broken.

It builds and publishes in the same job, so the thing that goes live was never a separate, inspectable object.

And, finally, nobody is asked for their approval.
A push to `main` goes to real users, immediately.

### Jobs that depend on other jobs

A workflow can have several jobs, and `needs:` makes one wait for another:

```yml
jobs:
  tests:
    runs-on: ubuntu-latest
    steps: ...

  build-docs:
    needs: tests
    runs-on: ubuntu-latest
    steps: ...

  deploy:
    needs: build-docs
    runs-on: ubuntu-latest
    steps: ...
```

Jobs without `needs` run in parallel, which is why your matrix jobs all start at once.
A job with `needs` waits, and is skipped entirely if what it depends on failed.

That is the gate: `deploy` cannot run unless `build-docs` succeeded, which cannot run unless `tests` succeeded.

Each job gets a fresh runner, so a later job does not inherit the files an earlier one made.
That is exactly what artifacts are for, and the Pages actions use them: `upload-pages-artifact` in one job, `deploy-pages` in another.

::::challenge{id="gated-deployment" title="Split the deployment into three jobs"}

Rewrite `pages.yml` so that it has a `tests` job, a `build-docs` job and a `deploy` job, connected with `needs:`.

The documentation must be built exactly once. The `deploy` job should not check out the code or build anything.

Narrow the permissions too. At the moment the whole workflow can write to Pages; only the deployment needs to.

:::solution

```yml
name: Documentation

on:
  push:
    branches: ["main"]
  workflow_dispatch:

permissions:
  contents: read

concurrency:
  group: pages
  cancel-in-progress: false

jobs:
  tests:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v7

      - name: Set up Python 3.13
        uses: actions/setup-python@v7
        with:
          python-version: "3.13"
          cache: pip
          cache-dependency-path: pyproject.toml

      - name: Install dependencies
        run: |
          python -m pip install .[dev]

      - name: Test with pytest
        run: |
          pytest

  build-docs:
    needs: tests
    runs-on: ubuntu-latest
    permissions:
      contents: read
      pages: read

    steps:
      - uses: actions/checkout@v7

      - name: Set up Python 3.13
        uses: actions/setup-python@v7
        with:
          python-version: "3.13"
          cache: pip
          cache-dependency-path: pyproject.toml

      - name: Install dependencies
        run: |
          python -m pip install .[docs]

      - name: Build documentation
        run: |
          sphinx-build -W -b html docs docs/_build/html

      - uses: actions/configure-pages@v6

      - name: Upload the built documentation as a Pages artifact
        uses: actions/upload-pages-artifact@v5
        with:
          path: docs/_build/html

  deploy:
    needs: build-docs
    runs-on: ubuntu-latest
    permissions:
      pages: write
      id-token: write
    environment:
      name: github-pages
      url: ${{ steps.deployment.outputs.page_url }}

    steps:
      - name: Deploy the artifact built above
        id: deployment
        uses: actions/deploy-pages@v5
```

Look at how short the `deploy` job is.
There is no checkout, no Python and no Sphinx.
It publishes the artifact that `build-docs` produced, and does nothing else.

That is deliberate.
If the deploy job rebuilt the documentation, it would be publishing something that had never been tested or reviewed, built from a second checkout at a slightly later moment.

The `concurrency` block differs from the one in the previous section: `cancel-in-progress: false`.
Cancelling a deployment halfway through could potentially leave things in a broken state.
It's not overly likely, but it is not worth taking the risk!

:::
::::

### Asking a person

The workflow now refuses to deploy broken code. It still does not ask anyone.

`environment: github-pages` in that last job is doing more than labelling it.
An **environment** is a named thing in your repository settings that can carry rules, and the most useful rule is that a human has to approve before any job using it will start.

::::challenge{id="require-approval" title="Require an approval"}

In _Settings_, then _Environments_, open `github-pages` and add yourself as a required reviewer.

Make a documentation change and merge it, then watch what the workflow does.

:::solution

`tests` runs, `build-docs` runs, and then the run stops and waits.
The `deploy` job is pending, the run page shows that review is required, and you get a button to approve or reject.

Approve it, and the deployment proceeds.
The run records who approved it and when.

The thing most people miss here is that the approval is a repository setting, not something written in the workflow file.
All your workflow does is name the environment, and naming it is what makes the setting apply.
Somebody reading the YAML on its own cannot tell whether an approval is required, which is worth remembering when you are looking at someone else's project.

:::
::::

## Delivery, or deployment

You have now built both halves of a distinction the lecture made at the start.

Before you added the reviewer, a change that passed its checks went live on its own.
That is **Continuous Deployment**.

After you added the reviewer, every change that passes is built, tested, packaged up and left ready to go, and a person decides when it goes.
That is **Continuous Delivery**.

The pipeline is identical until the final step.
The whole difference is whether a human has to say yes.

Neither one is the right answer in general.
Deploying documentation automatically is sensible: the cost of a bad page is low, and fixing it just means pushing another commit.
Deploying a service that people depend on, or anything that is awkward to undo, usually deserves the approval step, and in some regulated settings you will not have a choice about it.

The important thing is to make the decision on purpose.
It is quite easy to end up deploying automatically simply because nobody ever stopped to think about whether you should!

## Next steps

1. \[optional\] read about [storing workflow data as artifacts](https://docs.github.com/en/actions/how-tos/write-workflows/choose-what-workflows-do/store-and-share-data).

1. \[optional\] read about [deployment environments](https://docs.github.com/en/actions/how-tos/deploy/configure-and-manage-deployments/manage-environments) and the other protection rules they can carry, such as wait timers and branch restrictions.
