---
name: Handling legacy research codebases
dependsOn: []
tags: []
learningOutcomes:
  - Understand how coding agents can be helpful in handling legacy research codebases
---

## Introduction

In modern scientific research, it becomes increasingly common for researchers
to inherit some research codebases, which often have minimal documentation or
in a semi-working state. This is not because researchers enjoy giving bad
codebases to their successors, often it is just that the focus of researchers
is to produce results under deadline and the accompanying codebases are merely a
tool which they developed solely for themselves.

With the popularity of coding agents, it becomes easier to handle this
situation. However, just prompting the coding agent with 'explain this
codebase' will probably produce a well-written summary which could be wrong in
a subtle way and later when you actually run the code, most likely it won't
work.

## What you will do in this module

This module is an attempt to provide some guidance about how you could use
coding agents as an assistance when you inherit a research codebase. A lot of
principles exist before the advent of LLMs and we will see how coding agents
excel or probably not so helpful in understanding a research codebase.

You will produce an `AGENTS.md`/`CLAUDE.md` for a real-world research codebase
in a step-by-step manner, either with your own inherited codebase or a codebase
written by other researchers. Most importantly, **you are not generating an
`AGENTS.md`/`CLAUDE.md` with a coding agent** despite commands such as `/init`
exist or use some templates. You are going to **write** an
`AGENTS.md`/`CLAUDE.md` by yourself with information you have personally
verified. Creating `AGENTS.md`/`CLAUDE.md` files by a coding agent risks
circular reasoning, which the coding agents put in false claims you are not
aware of and future session reference them as source of truth. Besides, forcing
yourself to write each sentence out will make sure you understand it first, as
it is harder for one to write something that you do not believe. With an
`AGENTS.md`/`CLAUDE.md`, it provides a first step for you to reproduce some of
its results, modify the algorithm, or implement some new features with the
assistance of coding agents.

Five codebases have been curated below if you do not 'bring your own codebase',
and feel free to pick one closest to your domain to work in this module.

| Repository | Field |
| --- | --- |
| [`SamStudio8/gretel`](https://github.com/SamStudio8/gretel) | Bioinformatics, metagenomics |
| [`epicf/ef_python`](https://github.com/epicf/ef_python) | Accelerator physics, plasma physics |
| [`whaley-group-berkeley/qspectra`](https://github.com/whaley-group-berkeley/qspectra) | Molecular spectroscopy, quantum dynamics |
| [`cgq-qgc/HydroSensorReader`](https://github.com/cgq-qgc/HydroSensorReader) | Hydrogeology, environmental data |
| [`modsim/molyso`](https://github.com/modsim/molyso) | Quantitative microscopy, microbiology |

::::callout{variant="discussion"}

These repositories are teaching examples and they were selected because they
produced or support real research which are valuable. The author of this module
has no intention to criticise or imply something have been done improperly.

Their authorship of the above repositories should be respected throughout this
module. Work in your own clone or fork and do not submit AI-generated pull
requests, issues, or comments to the original repositories. If you maintain one
of these repositories and would prefer it not to be included, please contact
rse-training@dtc.ox.ac.uk.

::::

## Dealing with legacy codebases

### The entry point

When you inherited a codebase, the first thing you should find out is how the
software is invoked and this is called the **entry point**. The best way is, of
course, asking someone who knows but often, you are on your own to figure this
out.

::::challenge

For a Python codebase, how do you commonly locate the entry points? Be as
exhaustive as possible.

::::

:::solution

Below are some tips that could help you to locate the entry point (the
principles can be adapted to projects written in other programming languages):

- if there is a `setup.py` or `pyproject.toml` file, look for `console_scripts`
  or `project.scripts`.
- is there any file that could be run directly? It usually contains `if
  __name__ == '__main__'`.
- any Jupyter notebooks?
- any `docs/`, `examples/`, or `scripts/` directory etc.?
- if there are any tests, it should give you a sense of how things are invoked.
- go back in time! The very first few commits of a codebase usually is less
  clustered and contain only the necessary scripts to trigger something.

To conclude, it is often much easier to inspect the metadata of the codebase
first instead of looking at its implementation source as a start.

:::

#### How the coding agent can help

If you are well versed in the terminal, you could probably assemble some `grep`
commands with some regex to get an initial impression. Nowadays, we have coding
agents and they are powerful to look up information. However only a human with
the right domain knowledge and context can decide what the real entry point is.
Asking a coding agent 'Tell me the entry point of this codebase' will probably
give you something like `main.py` or `run_me.sh`, which may or may not be true.
A lot of codebases also have multiple entry points.

::::challenge

You can always ask the coding agent to enumerate all the plausible entry points
and **you** pick those that are relevant to you. Write such prompt, then
evaluate the output and decide what entry points are relevant.

::::

:::solution

Such a prompt should contain:

- a request to enumerate all plausible entry points **but not to pick one**
- the exact location (file and line) for each candidate
- the rationale or evidence for why each is considered an entry point
- a constraint not to run any code at this stage as you have not set up the
  environment yet

One example of such prompt could be:

> Explore the codebase thoroughly and list every plausible entry point. For each
> one, give me the exact location and your rationale that why you think this is
> an entry point. Do not run any code.

:::

:::callout{variant="note"}

**'What this is' section in `AGENTS.md`/`CLAUDE.md`:** in your own words, a
one-sentence overview of what the software does, the location of the entry
point(s), and how to trigger each of them.

:::

### Run something minimal

Once you know the entry point, the next natural step is to run it to see how it
goes. Unless it is a trivial script, chances are there are dependencies you
would need to install or run something like `make install`/`cmake` to build the
software, so we need to figure out how to install those dependencies or compile
it. Afterwards, we can run something minimal or its tests to see if it works.

:::callout{variant="tip"}

When running something unfamiliar to you, it is often good practice to run it
in an isolated environment in case something unpleasant happen. You could
consider install and run everything with a container with your chosen
repository in this module.

:::

::::challenge

What would you normally do to figure out the dependencies of the codebase or
the steps of compilation?

::::

:::solution

The most reliable way is to check if there is any CI pipeline or GitHub Action
present, as this shows you how the software is actually built. Steps outlined
in the documentation or README are useful reference but could be out of sync.

:::

However, legacy research codebases usually lack the above information, and even
if you know the dependencies or how to build it, most likely it will give you
errors even running something minimal. The reason is not because of the
codebase itself, it is because the world has since moved on and interfaces from
the dependencies may just be changed or removed. Even worse, the interfaces
remain the same but the underlying implementation changed or the same
parameters mean different things now etc.

#### How the coding agent can help

If there is completely no information about the dependencies or how to build
it, you could `grep` or visually inspect the `import` statements in the case of
Python. Coding agents are also quite good at figuring out the dependencies for
you (it most probably just `grep` the `import` and come up with a
`pyproject.toml` for you), or suggest the first step how you could compile it.

Coding agents are very good at looking up dependencies history, deprecation
warnings, and their motivation in relevant discussion. Once you have built a
virtual environment or a container with the dependencies and encountered an
error after running something minimal or tests with it, give your coding agent
the traceback and let it diagnose.

::::challenge

Assume you got an error after running something minimal using an entry point
with your virtual environment or container for the first time, write a prompt
for a coding agent so it can assist you to know why and fix it.

::::

:::solution

Such a prompt should contain:

- exactly how the environment was created
- what you have run and its full traceback
- when the codebase was last updated (although modern coding agents should be
  smart enough to inspect commit history by itself, sometimes your inherited
  codebase may not be version-controlled at all!)
- a request to **classify** the failure (dependency deprecation, version
  incompatibility, or a genuine bug) first before fixing it
- a request for evidence from the dependency's history

One example could be:

> I have created a virtual environment with the steps below:
> <venv_creation_steps>
> ...
> </venv_creation_steps>
>
> After running `...`, I got the following error:
> <error_msg>
> ...
> </error_msg>
>
> The software was last updated in June 2018 and I am not sure if this is a
> deprecation in a dependency, a Python version incompatibility, or a bug
> in this software itself. Tell me what you think it is and why.
>
> If this is a deprecation or incompatibility issue, show me the evidence from
> the history of the dependency, when and why this has been made deprecated,
> with all the relevant discussion and comments. With these, propose how this
> can be addressed.

Similar approach could be used when there are loads of compilation errors
during the build process. The idea, at this stage, is to identify whether this
is because of a deprecation/API change or a genuine bug in the codebase. Of
course, you should always verify afterwards.

:::

:::callout{variant="discussion"}

Given you encounter a deprecation, it is your call whether to fix the code or
pin a package to a particular version. Below are something to consider:

To pin:

- It is faster for now.
- It reduces one variable if you encounter any error later.
- It preserves exactly the behaviour from that dependency.

Not to pin and fix the code:

- It is slower but avoids accumulating further technical debt.
- You get the new functionality or optimisation from the package.
- A new version of the dependency may break something in other places subtly.
- If you are going to further develop this codebase, this is usually a better
  option.

:::

:::callout{variant="tip"}

It is useful to record the current status of the inherited codebase by running
the existing tests, if any. Some tests may already fail before you fix or
implement something and with a record, it will be clearer whether this is
because of your new changes or something is just not working when you inherit
the codebase. In some cases, the failed tests are not relevant anymore and you
can decide to remove them all together.

:::

:::callout{variant="note"}

**'Quick start' section in `AGENTS.md`/`CLAUDE.md`:** in your own words,
how to set up the environment, pinned dependencies with a concise reason
for each pin, the exact commands/scripts that work, errors you hit with
their reasons and workarounds, and the current test results.

:::

### The structure of the codebase

If you are going to use the codebase beyond anything like running it for few
times to reproduce something trivial, you are going to need a clear picture of
how different parts of the codebase connect with each other in order to
implement a new feature or fix a bug.

::::challenge

Outline some methods that will be helpful in understanding an unfamiliar
codebase (without any LLM).

::::

:::solution

These are something you could consider:

- use debug mode to step through the codes from one of its entry points.
- identify frequently changed files or any large files as they are usually the
  most useful part of the codebase.
- if it is a Python codebase, you could try to run `coverage` on one of its
  entry points or minimal examples. This allows you to focus your attention to
  what really matter even without any understanding the codes.
- if something genuinely do not make any sense, try renaming it or commenting
  them out to see what happens.

:::

#### How the coding agent can help

Prompting the coding agents with 'explain the codebase' will get you something
that looks plausible, however you have no idea whether the well-organised
claims are true or not. They may silently infer some claims from its knowledge
as it was trained with possibly thousands similar codebases.

::::challenge

How can we instruct the coding agents to avoid the above problem? Or at least
decrease the likelihood of hallucinating false claims about the codebase.

::::

:::solution

We cannot completely avoid the coding agents hallucinating about the codebase
(that is the nature of LLM unfortunately). However, we can use the information
gathered by above methods and instruct the coding agent to explain the
structure of the codebase based on the information we provide. Such a prompt
should contain:

- the evidence itself, such as a coverage report or some major functions that
  you think they have been called
- the command you run with the configuration/arguments
- a constraint to explain only **the traced path**
- a request to cite the file and location for each claim so you can check it

One example could be:

> Here is the coverage report from running `entry_point_1` with `config.yaml`.
> Trace the path and explain what each of the functions does given these
> configurations, citing the file and line for each step, and how they connect
> with each other. Based on the cited location, generate an interactive HTML
> map so I could visualise them.

:::

Research codebases typically contain experimental or unfinished features that
are never used in the existing analysis. Depending on your situation, those
features may be important to you as you are the one to extend some of the
features in your research. Isolating them from the production run path is one
of the most important insights to gain from understanding the structure of the
codebase, and coding agents can be a powerful tool here as they possess some
semantic understanding.

::::challenge

After understanding the run paths from different entry points, you should have
a better understanding of what functions are used and what they are for.

Assume you have some text files containing the functions used from all the
entry points you are interested in and the order of which they are called,
write a prompt to identify 'the dead code' (which may be of interest to you as
you are going to work on them).

::::

:::solution

Such a prompt should contain:

- the evidence (the text files with functions in the order they ran)
- a definition of 'dead' **relative to your entry points**
- a request to group the unused code by purpose and relationship
- an assessment per group (unfinished, superseded, ...) with rationale

One such prompt could be:

> Given these text files which contain the functions in the order they are
> used in running `entry_point_1`, classify the part of the codebase that
> `entry_point_1` does not currently use into groups based on how they relate
> to each other and what they intend to do. For each function in different
> groups, give a concise summary about its status, such as whether it is
> unfinished or has been superseded by other functions, and the evidence you
> found.

:::

:::callout{variant="note"}

**'The structure of the codebase' section in `AGENTS.md`/`CLAUDE.md`:** in
your own words, the functions/modules that really matter and what each is
for, the path data takes from entry point to output, and the parts that are
currently dead (unused) and why.

:::

### Reproduce previous results

Most of the research codebases you inherited have produced some sort of
research outputs, such as publication or conference abstract. If not, there
will be at least some reports or presentations internally in your research
group. These actually give you some sort of specifications you can check the
implementation against with the current codebases you inherited.

This also opens you to one of the characteristics of research codebases: **the
differences between what a figure or a table in a publication or a report says
and what the codes actually do**. This is because researchers often tinker
things when new data arrived or tune some parameters in an ad-hoc way, and if
you are lucky, those would be recorded in the report but often it is just in
the 'mind of the researchers'. All tests and CI pipelines will pass despite the
code cannot reproduce the figure or table you want.

#### How the coding agent can help

Be extremely cautious about the sycophancy nature of coding agents here. If you
prompt with 'does this code implement what this paper describes?', you will
certainly get a confident 'yes' with supporting details that look correct. It
tends to find agreements and avoid any discrepancy, unless you tell the
coding agents explicitly. The output will be misleading, especially if you are
unfamiliar with the inherited codebase.

This is one area that the domain knowledge of a human can genuinely help a lot
before asking the coding agent. You, the researcher, hopefully knows roughly,
for instance, how a certain figure was produced **in theory**. What you do not
know is how to use the inherited codebase to reproduce such figure, and with
the help of coding agents, this could be cut down from months to weeks.

Equipped with the knowledge of the structure of the codebase (from last
section), you should now know what functions/files are related to the
production of such figure, or in the worst case, you can eliminate irrelevant
code.

::::challenge

The powerful ability of coding agents to collect information and summarise them
is very useful in this situation. Write a prompt that guide the coding agent to
reproduce a figure in a publication that you know was once generated by your
inherited codebase.

::::

::::solution

This is a tricky question as rarely a one-shot attempt to reproduce something
in a publication will work. To be honest, if it works, you should be cautious
about its output... However, as a first attempt, this is what such a prompt
should contain:

- the exact target (figure/table etc.) and everything you know about it such as
  the algorithm to produce it or even the functions that are involved
- an investigation in the inputs needed
- a report of every discrepancy found between the publication and the code

> I want to reproduce Figure 4b in publication.pdf with this codebase.
> Summarise everything related to Figure 4b. The plot in Figure 4b was analysed
> by ABC algorithm, which is implemented by function `XX` in file `YY`. Let me
> know what raw data I should provide.
>
> Make the strongest possible case that the code's implementation does not do
> what the paper describes about Figure 4b. For any discrepancy you find, cite
> the location and detail your rationale. If there is no real discrepancy, say
> so explicitly rather than fabricating something.

:::callout{variant="tip"}

The idea is to utilise the ability of coding agents to collect information from
various sources and make association with them. **You exploit the sycophancy
nature of coding agents to find discrepancies in its implementation** and
whether the discrepancies claimed by the coding agents are valid or not solely
depends on you, the researcher, who knows the domain knowledge and thus is able
to make an educated judgement.

:::

::::

Below are something that often result in discrepancy between the published
result and the code:

- hard-coded constant, e.g. threshold
- convention, e.g. normalisation
- hidden assumptions, e.g. background is removed in an ambiguous way

:::callout{variant="note"}

**'How to reproduce XXX' section in `AGENTS.md`/`CLAUDE.md`:** in your own
words, for the result you want to work on, the claims made in the
publication/report and the corresponding parts of the code, any discrepancy
with your resolution, and the steps to reproduce it, at least partially.

:::

:::callout{variant="warning"}

When the coding agents fail to reproduce something, you should watch out for
them silently tweaking something that makes the code agree with the published
result. For instance, it may adjust some default values without any valid
scientific reason just to match the numbers in the publication. Whether this is
'cheating' or not entirely relies on you to decide. When iterating with the
coding agents to reproduce something, you may need to provide additional
constraints progressively if you spot something it should not change.

:::

## Conclusion

When you are handed a legacy research codebase, it could be a frustrating
experience, especially when there is minimal help from your colleagues (not that
they hate you, but no one really knows what it is about) and you need to
produce something out of it. It used to take months of significant effort from
a researcher to make sense of it. With the power of modern coding agents, the
time can be shortened to weeks. Coding agents genuinely provide values in
modern scientific research.

This module provided some guidance about how you could use coding agents to
handle legacy research codebase by manually producing an `AGENTS.md`/`CLAUDE.md`
file with the assistance of coding agents using either your own inherited
codebase or one of the curated ones. This `AGENTS.md`/`CLAUDE.md` file serves
as the context for coding agents which is a starting point for you to further
work on the codebase, such as refactoring, fixing bugs, implementing new
features or algorithm, and optimisation etc.

:::callout{variant="keypoints"}

- Coding agents excel at gathering information from messy sources such as a
  legacy research codebase.
- It is easier to start with metadata of a codebase (packaging, CI pipelines,
  tests, commit history etc.) than the code themselves when exploring an
  unfamiliar codebase.
- Provide concrete evidence (e.g. tracebacks) to the coding agents when you
  want it to further investigate something.
- Domain knowledge is invaluable in handling legacy research codebases as, for
  instance, only you can decide a reproduced result is actually right or not.
  You should make your own judgement based on evidence you have verified.

:::
