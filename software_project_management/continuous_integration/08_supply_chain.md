---
name: Trusting the code your workflows run
dependsOn: []
tags: [github]
learningOutcomes:
  - Explain what a third-party action can do inside your workflow, and what it is trusted with.
  - Pin an action to a commit, and decide which actions warrant it.
  - Give each workflow only the permissions it needs.
  - Audit your own workflows with a tool, and keep their dependencies updated deliberately.
attribution:
  - citation: This course material was developed as part of UNIVERSE-HPC, which was funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Whose code are you running?

Count the `uses:` lines across your workflows.

Every one of them downloads somebody else's code and runs it on a machine that has your repository checked out on it, and in one case has one of your secrets in its environment.
You have not read any of that code, and nobody really expects you to.
That is a reasonable trade, but it is worth understanding what you are trading away.

Nothing in this course has checked any of this so far, and none of it shows up as a red cross.
A workflow can pass every check and still be doing something you would not be happy about.

## What a version tag actually guarantees

Look at how your actions are identified:

```yml
- uses: actions/checkout@v7
```

`v7` is a git tag in the `actions/checkout` repository.
A tag is just a label attached to a particular commit, and labels can be moved.
Whoever maintains that repository could point `v7` at different code tomorrow, and your workflow would pick it up without you changing a line, without a pull request, and without any notification.

For actions published by GitHub this is usually a good thing rather than a worry, because it means fixes reach you without any effort on your part.
For an action maintained by somebody you have never met, it means you are running whatever they decide to publish, indefinitely, and trusting them to get it right.

A commit hash cannot be moved:

```yml
- uses: codecov/codecov-action@303a32d7a59b442fa8d48b6a1cc6825c09c847a5 # v7.1.1
```

That is the same action, pinned so that the code cannot change under you.
The comment keeps it readable, because a bare hash isn't very useful at all.

### Deciding where to draw the line

Pinning everything is the safest option, and also the most tedious, because every pin then has to be updated by hand.
Most projects settle on a policy instead.

Create a file called `zizmor.yml` at the top level of your repository:

```yaml
rules:
  unpinned-uses:
    config:
      policies:
        actions/*: ref-pin
        "*": hash-pin
```

That says GitHub's own actions may be used by tag, and everything else must be pinned to a hash.

`zizmor` is a tool that picks this file up automatically when you run it from the root of the repository; more on that later.

::::challenge{id="apply-the-pin-policy" title="Which of your actions needs a hash?"}

Go through your workflows and work out which actions that policy applies to.

:::solution

Exactly one: `codecov/codecov-action`.

Everything else you have used is `actions/something`, published by GitHub.

It is worth noticing that the policy picked out the right one on its own.
Codecov's action is third-party, and it is also the only step in any of your workflows that is handed a secret, your `CODECOV_TOKEN`.
If that action were replaced with something malicious, it would be running with your token already in its environment.

It is also worth asking whether you agree with the policy.
A stricter project would pin GitHub's own actions too.
The argument for the looser rule is that GitHub already controls the machine your job runs on, so pinning its actions does not protect you from very much.

:::
::::

## Giving a job only what it needs

By default a workflow's token can read your repository, and depending on your settings it may be able to write to it.
Most of your jobs do not need to write anything.

You have already seen this done properly in the deployment workflow: `contents: read` at the top, and `pages: write` granted only to the job that deploys.

The same idea applies everywhere else:

```yml
permissions:
  contents: read
```

If a linting job has no permission to write to your repository, then nothing running inside that job can write to it either, however it tries.

There is a second, quieter leak.
`actions/checkout` stores the credentials it used in `.git/config` so that later steps can push.
Nearly all of your jobs never push, so those credentials sit there for the rest of the run, available to every subsequent step, including other people's actions.
Turn it off where you do not need it:

```yml
- uses: actions/checkout@v7
  with:
    persist-credentials: false
```

## Auditing the workflows

You do not have to remember any of this, because there is a tool.

[zizmor](https://docs.zizmor.sh/) is a static analyser for GitHub Actions workflows.
It reads your YAML and reports patterns that are known to be risky, in the same way that ruff reads your Python.

::::challenge{id="run-zizmor" title="Audit your own workflows"}

Run it over your workflows:

```bash
pip install zizmor
zizmor .github/workflows/
```

If you would rather not add it to your environment, `pipx run zizmor .github/workflows/` and `uvx zizmor .github/workflows/` both work without installing anything permanently.

Read what it finds, then fix everything it reports.

:::solution

On workflows written as this course has written them so far, it reports three kinds of problem, and all three are the ones described above:

- `unpinned-uses`, for actions identified by tag where your policy wants a hash.
- `excessive-permissions`, for workflows with no `permissions` block, which therefore take the default.
- `artipacked`, for checkouts that leave their credentials behind.

Fixing them means adding `permissions: contents: read` to each workflow, adding `persist-credentials: false` to each checkout, and pinning the Codecov action to a commit.

None of these changes alter what your workflows actually do.
That is typical of this kind of work.
When it is done properly you never notice it, which is a large part of why it often does not get done until after something has gone wrong.

:::
::::

::::challenge{id="zizmor-workflow" title="Run the audit in CI"}

Add a workflow that runs the audit, so that a future change cannot quietly reintroduce any of the problems.

Run it on changes to the workflow files, and on a schedule, because new audit rules appear over time even when your files do not change.

:::solution

```yml
name: Audit workflows

on:
  push:
    branches: ["main"]
    paths:
      - ".github/workflows/**"
  pull_request:
    branches: ["main"]
    paths:
      - ".github/workflows/**"
  schedule:
    - cron: "15 3 * * 1"
  workflow_dispatch:

permissions:
  contents: read

jobs:
  zizmor:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v7
        with:
          persist-credentials: false

      - name: Audit the workflows
        uses: zizmorcore/zizmor-action@cc914d7f3750a2d13d75c7f184a1060aa0e9d482 # v0.6.4
        with:
          inputs: .github/workflows
          version: 1.30.1
          advanced-security: false
          annotations: true
```

Notice that this workflow follows its own advice.
It is a third-party action, so it is pinned to a commit.

The `version` input matters just as much, and it is an easy one to miss.
Without it, the action installs whatever the latest release of zizmor happens to be at the moment it runs.
You would have pinned the wrapper and left the tool it actually runs completely unpinned, which gives you most of the inconvenience of pinning and not much of the benefit.

:::
::::

## Keeping the pins current

A pinned dependency is a dependency that stops receiving fixes.
That is the cost of pinning, and the answer is to update deliberately rather than never.

[Dependabot](https://docs.github.com/en/code-security/dependabot) watches your dependencies and automatically opens pull requests when new versions appear.
Those pull requests run your checks like any other, so you see whether the update breaks anything before you take it.

Create `.github/dependabot.yml`:

```yaml
version: 2

updates:
  - package-ecosystem: github-actions
    directory: "/"
    schedule:
      interval: weekly
    cooldown:
      default-days: 14
    groups:
      actions:
        patterns:
          - "*"
```

Three details:

`cooldown` waits a fortnight after a release before offering it to you.
Taking an update on the day it ships carries its own risk.
A release that has been out for two weeks has had two weeks of other people discovering what is wrong with it, and a compromised release is usually spotted and pulled within days.
None of that helps you if you upgraded an hour after it appeared.

`groups` puts all the action updates into one pull request rather than six.
Six separate pull requests, each needing its own review, is how people end up ignoring Dependabot altogether.
That is worse than not having it at all, because the unread pull requests make it look as though somebody is keeping the project up to date.

This configuration watches GitHub Actions only, not pip.
Your Python dependencies are declared without version bounds and there is no lockfile, so there is nothing for Dependabot to raise: it can only offer to change a version that is written down somewhere.
If you add bounds or a lockfile to a project, you should add the `pip` ecosystem too.

## Where your jobs run

One last question, which is really the same question asked about the hardware rather than the code: whose machine is your job running on?

Everything so far has run on **GitHub-hosted runners**, which are destroyed after every job.
That clean slate is most of why CI is trustworthy, and it is why the missing data file in the first section showed up there and not on your laptop.

There are alternatives, and they come up quickly in research.

**Self-hosted runners** are machines you provide and register with GitHub, which is how people run CI that needs a GPU, a large dataset, a licensed compiler, or a machine inside an institutional network.
They are also a genuine hazard on a public repository.
A runner that is not destroyed between jobs keeps whatever the last job left behind, and anybody who can open a pull request can propose a workflow change that runs on your hardware.
If you ever set one up, read GitHub's security guidance first, and do not attach one to a public repository casually.

**Containers.** A job can run inside a container image you specify, which pins the whole environment rather than just the Python version. The [containerisation course](/technology_and_tooling/docker) covers building those images.

**Scheduled runs.** The `schedule` trigger you used for the audit is also how you catch problems that arrive without you doing anything. A weekly run of your test suite against unpinned dependencies tells you when something upstream has broken you, rather than finding out in the middle of unrelated work.

**Cost.** Runners are free on public repositories and metered on private ones. If you make a repository private, a matrix of three Python versions across three operating systems is nine jobs consuming your quota on every push, and the Windows and macOS runners are billed at several times the Linux rate.

## Next steps

1. \[optional\] read GitHub's [security hardening guide for Actions](https://docs.github.com/en/actions/reference/security/secure-use).

1. \[optional\] read the [zizmor audit documentation](https://docs.zizmor.sh/audits/), which explains each finding and why it matters.

1. \[optional\] read about [self-hosted runner security](https://docs.github.com/en/actions/how-tos/manage-runners/self-hosted-runners/manage-access) before you ever set one up.
