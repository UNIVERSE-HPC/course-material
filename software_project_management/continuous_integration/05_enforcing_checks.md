---
name: Running checks before you commit and before you merge
dependsOn: []
tags: [github]
learningOutcomes:
  - Install pre-commit hooks so that checks run on your own machine before a commit is created.
  - Protect a branch so that a pull request cannot be merged until its checks pass.
  - Explain why a check that nobody is required to pass does not have much effect.
attribution:
  - citation: This course material was developed as part of UNIVERSE-HPC, which was funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Two gaps

You now have several workflows, and between them they check a good deal.
They all have the same two weaknesses.

The first is that they only tell you about things _after_ you push.
If you leave an unused import in a file, you commit it, push it, wait a minute or two for a runner to start, and then read a message that a tool on your own machine could have given you instantly.

The second is more serious.
Nothing we have built so far actually stops a broken change from going in.
A red cross appears next to the commit, but the merge button still works.
Your checks tell you about problems, but nobody has to act on what they say.

This section closes both gaps: first by moving the checks earlier, then by giving them real weight.

## Running checks before you commit

Git can run a command every time you make a commit, and refuse to make the commit if that command fails.
These are called _hooks_, and they live in a hidden folder inside your clone of the repository.
Writing and sharing them by hand is fiddly, so most people use a tool called [pre-commit](https://pre-commit.com/) to manage them.

It is already available: `pre-commit` is in the `dev` extra you installed at the start.

### Describing the hooks you want

Create a file called `.pre-commit-config.yaml` at the top level of your repository:

```yaml
repos:
  - repo: https://github.com/pre-commit/pre-commit-hooks
    rev: v6.0.0
    hooks:
      - id: trailing-whitespace
      - id: end-of-file-fixer
      - id: check-yaml
      - id: check-merge-conflict

  - repo: https://github.com/astral-sh/ruff-pre-commit
    rev: v0.17.1
    hooks:
      - id: ruff-check
```

The structure is worth reading carefully, because it is unlike the workflow files.

`repos` is a list of places to get checks from, and each entry has three parts.
`repo` is a git repository that publishes some hooks.
`rev` is the version to use, which pins it in exactly the same way and for exactly the same reasons as `uses: actions/checkout@v7` pins an action.
`hooks` lists which of the checks in that repository you actually want, chosen by `id`.

A repository can offer dozens of hooks, and you choose the ones you want.
The available ids are listed in a file called `.pre-commit-hooks.yaml` in the source repository, which is worth a look if you are curious about what else is available.

The two sources here are the standard collection of small, general checks, and Astral's wrapper around ruff.
Note that `ruff-check` reports problems rather than fixing them, which keeps it consistent with what the lint job in CI will tell you.

### Turning them on

```bash
pre-commit install
```

This writes a script into `.git/hooks/` in your clone.
From now on `git commit` runs the hooks first.

::::challenge{id="see-pre-commit-work" title="Watch a commit be refused"}

Your code currently passes the linter, so there is nothing for the hooks to catch.
Give them something.

Add an unused import to the top of any file in `src/ci_course/`, stage it, and try to commit.
Then fix it and commit again.

:::solution

The commit is refused and ruff tells you why:

```text
ruff check...............................................................Failed
```

No commit is created, so there is nothing to undo.
Remove the import, stage the change, and the commit goes through.

Notice how much quicker that was than pushing and waiting for CI to tell you the same thing!

:::
::::

Two details that will matter later.

A commit hook only sees the files in that commit.
If a check starts failing because you changed its configuration, rather than because you changed the code, the hook will not notice until you happen to touch the affected file.
Running the hooks over everything is a separate command:

```bash
pre-commit run --all-files
```

Second, hooks are optional.
`pre-commit install` is something each person does in each clone, so a collaborator who has never run it is unprotected, and `git commit --no-verify` skips them for anyone who finds them inconvenient.

### Checking the hooks in CI as well

::::challenge{id="pre-commit-workflow" title="Add a pre-commit workflow"}

Write a workflow, `pre_commit.yml`, that runs `pre-commit run --all-files`.

:::solution

```yml
name: Pre-commit

on:
  push:
    branches: ["main"]
  pull_request:
    branches: ["main"]
  workflow_dispatch:

jobs:
  pre-commit:
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

      - name: Run every hook against every file
        run: |
          pre-commit run --all-files
```

:::
::::

It's worth thinking about what this workflow is actually doing for you.
It runs ruff, which your Python versions and Operating systems workflows already run, so most of it is duplicated work.

It is still worth having, for the reasons just listed: the hooks are installed separately in each clone, they can be skipped, and a commit hook never looks at the whole repository.
This job is what notices when any of that happens.
Think of it as a safety net rather than as anything new, and ten seconds of repeated linting is a reasonable price to pay for one.

## Requiring checks before you merge

Now the second gap.

Everything so far has been advisory.
A failing workflow puts a red cross on a commit and changes nothing else.
You can still merge, and so can anybody else with write access, whether they noticed the cross or not.

Fortunately, GitHub can be told to refuse.

### Protecting the branch

In your repository, go to _Settings_, then _Rules_, then _Rulesets_, and create a new branch ruleset targeting `main`.

The two settings we care about for now are:

- **Require a pull request before merging.** Changes reach `main` through a PR rather than a direct push.
- **Require status checks to pass.** Then search for the checks you want, and select your test jobs.

A check only appears in that list once it has run at least once, so if the box looks empty, make sure your checks have run at least once.

::::challenge{id="watch-a-merge-be-blocked" title="Watch a merge be blocked"}

Protect `main` and require your Python versions checks to pass.

Then create a branch, break something deliberately, such as changing a function so a test fails, and open a pull request.

Watch what the pull request page says.
Then fix the branch, push again, and watch it change.

:::solution

While the check is failing, the pull request shows the failure and the merge button is disabled, with a line saying that required checks have not passed.

Push the fix and the same page re-runs the checks and enables the button.

You have not written any new YAML.
The workflow was already running and already reporting; the only thing that changed is that you're now forced to care about the answer.

:::
::::

### What this is really for

On a repository you own, you can usually still force the merge through, because repository administrators are often allowed to bypass their own rules.
That is not a flaw in the exercise.
Branch protection is not really there to stop somebody who is determined to get round it.
It is there to stop an ordinary mistake on an ordinary afternoon, by somebody who simply had not noticed the red cross.

It also changes what a pull request means.
The [collaboration course](/software_project_management/collaboration) covers pull requests and code review, where a human reads the change and decides.
Required checks put the mechanical part of that review, the part a computer does better than a person, in front of the human part, so that a reviewer is never spending their attention on a branch whose tests do not pass.

### How you work from here on

You have just changed the way you have to use this repository, so it is worth being really clear about that here.

You can no longer push directly to `main`.
Every change from this point in the course onwards goes on a branch and reaches `main` through a pull request.

Now, look at the triggers on the workflows you have written:

```yml
on:
  push:
    branches: ["main"]
  pull_request:
    branches: ["main"]
```

They run on pushes to `main`, and on pull requests aimed at `main`.
Pushing to a branch on its own matches neither, so nothing runs and no checks appear.

So wherever the rest of this course says to make a change and see what CI does, it means:

```bash
git switch -c some-branch-name
# make the change
git commit -am "Describe the change"
git push -u origin some-branch-name
gh pr create --fill     # or open the pull request on the website
```

Pushing further commits to that branch re-runs the checks on the open pull request, so you only need to open it once.

## Three places, one set of checks

You have now run the same checks in three places, and each one is doing a different job.

| Where | When | What it is good at | How it fails |
| :- | :- | :- | :- |
| Pre-commit hook | Before a commit exists | Immediate feedback, no waiting | Optional, skippable, only sees the commit |
| Workflow run | After every push | Clean machine, whole repository, every platform | Only reports |
| Required status check | Before a merge | Actually binding | Needs the workflow to exist and to be selected |

The last row is the one that is easy to skip.
Writing the check is the easy part, and it is where a lot of projects stop.
A check that nobody has to pass will eventually be ignored by somebody in a hurry!
