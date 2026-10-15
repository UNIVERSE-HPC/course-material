---
name: Diagnosing failures and keeping the build fast
dependsOn: []
tags: [github]
learningOutcomes:
  - Read a workflow log to find the cause of a failure rather than only the step that failed.
  - Re-run a workflow, including with debug logging, and know when re-running is not the answer.
  - Shorten the time between making a change and finding out whether it worked.
  - Cache dependencies and cancel superseded runs, and judge when either is worth doing.
attribution:
  - citation: This course material was developed as part of UNIVERSE-HPC, which was funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## The loop

Working with CI is a loop: you change something, push it, wait, read the result, and go around again.

Every failure you have met so far came with the answer attached, because this course put it there deliberately and then told you what it was.
Real failures are not usually so considerate.

This section is about the two things that make the loop bearable: finding out what actually went wrong, and spending less time waiting.

## Reading a failed run

Open the _Actions_ tab and click into a failed run.

You get a list of jobs down the left, with the failed one marked.
Click it and you see the steps, with the failed step already expanded.
Every other step is collapsed, and this is the first trap: the step that failed is not always the step that is wrong.

A few things worth knowing:

- Each step can be expanded, including the ones that succeeded.
- There is a search box for the log, which is more useful than scrolling once a log is a few thousand lines long.
- The gear icon at the top right of the log view will download the raw text of the whole run, which you can then search properly.
- Timestamps can be turned on from the same menu, which tells you where the time went.

### The failure is often not where the error is

::::challenge{id="debug-a-confusing-failure" title="Find the real cause"}

Break your workflow in a specific way.

In `python_versions.yml`, change the install step so that it asks for an _extra_ that does not exist:

```yml
      - name: Install dependencies
        run: |
          python -m pip install .[developer]
```

Commit it on a branch, open a pull request, and look at what CI tells you.

The failing step will be _Lint with ruff_, and the message will be some variation of `ruff: command not found`.
That is not the problem.
Find the problem.

:::solution

Expand the _Install dependencies_ step, which passed, and read its output.

Buried in its output is:

```text
WARNING: ci-course 0.1.0 does not provide the extra 'developer'
```

`pip` does not treat an unknown extra as an error.
It warns, installs the project without that extra, and exits successfully.
So the development dependencies, including ruff and pytest, were never installed, and the first step that tries to use one of them is the first step that fails.

The fix is to correct the extra back to `dev`.

The habit to take away is to be a little suspicious of the step that is marked red.
It tells you where things stopped, which is not always where the mistake was made.
When a step fails with "command not found", or a missing file, or an empty result, the step that was supposed to produce that command or file is often one that reported success.

:::
::::

### Re-running

At the top right of a run there is a _Re-run jobs_ menu, with two useful options.

_Re-run failed jobs_ repeats only what failed, which matters when you have a matrix of nine jobs and one of them failed.

_Re-run all jobs_ starts the lot again.

Both have a checkbox for **Enable debug logging**.
Ticking it sets two variables inside the run that make the runner and the actions much more talkative, which is how you investigate an action that is misbehaving rather than your own code.
It is noisy, so it is a second resort.

Re-running is the right move when you have reason to think the failure was not about your code: a network timeout while downloading a package, a runner that died, a service that was briefly unavailable...

It is not much use the rest of the time.
If the cause is something in your repository, re-running will produce exactly the same failure a few minutes later.
It is tempting, because it feels like doing something, but you will not learn anything you did not already know.

### Not waiting for CI at all

The quickest way to shorten the loop is to leave it.

Every step in the workflows you have written is an ordinary command.
There is nothing special about `ruff check .` or `pytest` just because a runner is the one typing them, and you can run exactly the same commands on your own machine in a second or two.
The hooks from the previous section are simply this idea, done automatically.

What CI genuinely tells you that your machine cannot is whether the thing works somewhere that is not your machine: a clean install, a different Python, a different operating system.
Use it for those, but run everything else locally.

When you do need a run, two things help:

- The `workflow_dispatch` trigger you have had on your workflows since the beginning lets you start one from the _Actions_ tab without pushing anything.
- Work on a branch with a pull request open. You get the same checks without putting broken commits into your project's history, and pushing again to the same branch re-runs them. Remember that a branch with no pull request runs nothing at all, because your workflows only trigger on `main` and on pull requests targeting it.

You can also run workflows locally with tools such as [act](https://github.com/nektos/act), which executes jobs in containers on your own machine.
It is a reasonable way to iterate on a complicated workflow, but it approximates the hosted runners rather than reproducing them, so a green result locally isn't necessarily the end of the story.

## Keeping the build fast

The second half of the loop is the waiting.

"Keep the build fast" was one of the principles at the start of this course, and the reason is behavioural rather than technical.
A slow pipeline does not just waste time, it changes what people do.
They save up changes to avoid paying the cost of waiting, so each change gets bigger, which makes it harder to review and more likely to break something.

Before changing anything, find out where the time actually goes.
Open a recent run, turn on timestamps, and look at how long each step took.

### Caching dependencies

Most of the time in these jobs is spent downloading and installing the same packages that were downloaded and installed last time.

`setup-python` can keep them between runs:

```yml
      - name: Set up Python ${{ matrix.python-version }}
        uses: actions/setup-python@v7
        with:
          python-version: ${{ matrix.python-version }}
          cache: pip
          cache-dependency-path: pyproject.toml
```

`cache: pip` turns on caching of pip's download cache.
`cache-dependency-path` tells it which file to watch: when `pyproject.toml` changes, the dependencies might have changed too, so the cache key changes and a fresh one is built.

::::challenge{id="watch-the-cache" title="Watch the cache being saved and restored"}

Add those two lines to your Python versions workflow, on a branch, and open a pull request.

Look at the _Set up Python_ step in that run, then push a second commit to the same branch and look at the same step in the next run.

:::solution

In the first run the step reports that no cache was found for the key, and at the end of the job it saves one.

In the second run it reports restoring the cache it saved, and pip then installs from local files rather than downloading.

What you will probably not see is a dramatic difference in the clock.
This project has one real dependency, and the whole install takes a few seconds either way.

The reason to learn it now is that the same two lines can be the difference between a two-minute job and a twenty-second one on a project that builds a compiled extension, installs a large scientific stack, or downloads a model file.
You are learning how it works on an example small enough to understand, so that you can use it on a project where it actually saves you an appreciable amount of time.

:::
::::

### Not running what you do not need to

Caching makes a run quicker. Not running at all is quicker still.

**Cancelling superseded runs.** If you push three commits to a branch in quick succession, by default you get three full sets of jobs, and the results of the first two are worthless the moment the third arrives.

```yml
concurrency:
  group: ${{ github.workflow }}-${{ github.ref }}
  cancel-in-progress: true
```

`group` is just a name: runs that compute the same name are considered to be the same queue.
Here it combines the workflow and the branch, so a new push to a branch cancels the older run of that same workflow on that same branch, and leaves other workflows and other branches alone.

Be careful where you put this.
Cancelling a half-finished test run is absolutely fine.
ButL cancelling a half-finished deployment (such as uploading your documentation to GitHub Pages) can leave something broken, so a deployment workflow usually sets `cancel-in-progress: false`, which queues instead.

**Skipping irrelevant changes.** A commit that only edits a README does not need the test matrix:

```yml
on:
  push:
    paths-ignore:
      - "**.md"
```

This can cause a real problem now that you have required status checks.
A workflow that is skipped does not report a result, and a required check with no result is not a passed check, so a documentation-only pull request can sit unmergeable waiting for a job that is never going to run.
If you use `paths-ignore` on a required workflow, check that a documentation-only PR can still merge.

**Failing fast, or not.** You have had `fail-fast: false` in your matrix since the start.

The default is `true`, which cancels every other job in the matrix as soon as one fails.
That is the faster setting, and the wrong one here.
When a test fails on Windows, the useful question is whether it also failed on macOS, and `fail-fast: true` throws that information away.

Leave it at the default when you just want to know as quickly as possible that something is wrong, and set it to `false` when you want the full picture of what is broken.

## Next steps

1. \[optional\] read about [caching dependencies](https://docs.github.com/en/actions/how-tos/write-workflows/choose-what-workflows-do/cache-dependencies), including `actions/cache` for things `setup-python` does not handle.

1. \[optional\] read about [controlling concurrency](https://docs.github.com/en/actions/reference/workflows-and-actions/control-flow#using-concurrency).
