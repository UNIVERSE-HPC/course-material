---
name: Introduction
dependsOn: []
tags: [genai, coding, debugging, python]
learningOutcomes:
  - Understand the structure and goals of the Repressilator analysis project
  - Identify the scope of errors introduced by LLM-generated code
---

# Introduction

To prevent a very large amount of repetition, I will state here that everything written here on the topic of LLMs and their capabilities is true as of writing, but may very rapidly become obsolete. 

## What is the purpose of this exercise?

When I learned to code, in your introduction to coding class, you'd be given an exercise (e.g. generate the 21st Fibonacci number), dutifully go to Stack Overflow, copy the highest rated answer and move on to the next question without really understanding anything. Eventually, you'd get to a problem where you couldn't find a solution verbatim, and you would have to start writing your own code. Ideally, understanding would then start to flourish.

In the age of the LLM, it is now possible to generate a very large amount of code personalised to your particular problem. As a result, the point at which you have to start writing your code occurs much later, if at all. This is problematic (from both the perspective of the teacher and the student) for several reasons. Firstly, although you do not necessarily have to understand every analysis in every piece of software you use (we all rely on external libraries after all), defending your work with "it was suggested by an LLM" will convince approximately nobody. Secondly, debugging code that someone else has written is often much harder than writing it yourself from scratch, and learning to debug generally comes from long experience fixing self-created problems in your own software. 

With that being said, it is likely that LLMs, at least for coding, are here to stay. They can generate a large amount of code very quickly, and with careful usage they can undoubtedly make an experienced programmer more productive. This exercise is intended to encourage you to critically engage with these technologies, finding a balance between using them to increase the amount of work you can do, whilst giving you the skills to investigate and resolve bugs as they occur. This exercise has been designed so that it cannot be fixed by an LLM running autonomously (yet!!); you will need to take an active role in the debugging process. It isn't expected that you'll be able to complete everything in a single day. 

## What do I need to do?

You have been given a dataset to analyse. It comprises:
  - Two sets of "microscopy" images, recorded at 15 minute intervals, one in "fluorescence" mode and one in "phase contrast" mode (in the `images/` folder)
  - A set of calibration data, relating pixel intensities to concentrations of proteins (in the `docs/` folder)

The cells you'll be analysing have been engineered to glow. Two of the three proteins have been tagged with fluorescent dyes: one glows green in the body of the cell (the cytoplasm) and one glows red in the cell's nucleus. By imaging these cells over time, you can watch the fluorescence intensity rise and fall as the proteins are produced and degraded. The third protein has no marker, and as such we are blind to it.

The reason for the oscillation is a feedback circuit called the "repressilator" (detailed in the paper in the `docs/` folder). Three proteins take turns suppressing each other's production in a cycle — protein A suppresses B, B suppresses C, and C suppresses A — which under certain conditions causes the amounts of each protein to oscillate rhythmically. The dynamics of this oscillation can be modelled using an [ordinary differential equation]([scientific_computing]/[essential_maths]/[09_differential_equations_1]). The parameters for this equation (for example, the half-life of a protein), and the form of the differential equations, can be found in the docs folder. 

Each cell circuit has slightly different parameter values. You are tasked with recovering the values of these parameters for a subset of the cell images. An LLM has been used to create an analysis pipeline, but it is not capable of achieving this aim. You are tasked with fixing the implementation. You can assess the success of this implementation by using the tests in the `tests/` folder.

> **Important:** The test code is correct. All code inside `repressilator_analysis/` is LLM-generated and should be treated as suspect. In some cases, you may need to alter the test functions (e.g. for diagnostic code, or if you have changed the structure of the values returned by the functions in `repressilator_analysis/`), but the test logic itself should not change.


## Analysis tasks

The three analysis modules are independent but feed into each other as shown below:

```
┌──────────────────────┐    ┌──────────────────────┐
│  Phase contrast &    │    │  Calibration .txt    │
│  fluorescence images │    │  files               │
└──────────┬───────────┘    └──────────┬───────────┘
           │                           │
           ▼                           ▼
┌──────────────────────┐    ┌──────────────────────┐
│  Fluorescence        │    │    Calibration       │
│  extraction          │    │  (AU → proteins)     │
└──────────┬───────────┘    └──────────┬───────────┘
           │                           │
  fluor./cell/time                     |     
           │                           │
           └─────────────┬─────────────┘
                         │
              proteins/cell/time
                         │
                         ▼
              ┌──────────────────────┐
              │  ODE simulation &    │
              │     inference        │
              └──────────┬───────────┘
                         │
                parameters/cell
```

### Image analysis

Each timepoint image contains 80 cells. For each timepoint, you need to be able to obtain the average fluorescence across 1) the cell in the green channel and 2) the nucleus in the red channel for each cell in the image. This can be decomposed into sub-tasks
- Obtain the regions of each timepoint image corresponding to the 80 distinct cells and nuclei
- Link these regions in time (i.e. associate the cell in timepoint 1 with the same cell in timepoint 0)
- Extract the mean fluorescence of each cell in time to create a trace of nuclear/cytoplasmic fluorescence for each cell as a function of time

### Ordinary differential equation simulation and inference

Given a set of protein number values in time, we want to be able to recover the model parameters that generate those values. This can also be decomposed into subtasks:
- Simulate the number of protein molecules you would expect to see given some parameters
- Using optimisation methods, and the ordinary differential equation model in the step above, recover the parameters used to generate a given set of protein number values
### Calibration

Given the fluorescence values from the image analysis, we need to be able to convert these values to protein numbers, for use by the ODE simulation and inference module. 
### Pipeline

Combine the three modules together to estimate model parameters for a subset of cells in the dataset. 

## Guidance notes

The image analysis, ODE inference and simulation and calibration modules each have a full "walkthrough" to modify the code to get the test to pass (although other solutions are of course possible). The more time you spend trying to fix the code, as opposed to just copying and pasting the solutions, the more useful this exercise will be. 

I'd suggest starting with Calibration, then Image analysis, then ODE Simulation. These are independent modules, so you could do them in any order, but calibration is the simplest entry point. Once these three have been completed, Pipeline integrates them.

## Notes on using LLMs

We still don't know the best way to use LLMs to write code whilst preserving personal understanding. Each walkthrough in this exercise is structured around the following debugging strategy, which you should try to follow when working through the bugs:

### Debugging strategy

1. Determining what success looks like: why has this error occurred? What would need to change to pass this test?
2. Isolating the source of error: what sequence of operations occur before the error check? Can we isolate which steps are correct?
3. Creating a fix for the error.
4. Iterate as needed

In general, I find LLMs are incredibly useful when used in this sequence. For step 1, they are infinitely patient, and will rephrase or drill down as many times as you want them to. For step 2, they are great at writing diagnostic code, for example showing images overlaid with segmentations or printing protein trace errors. Finally, for step 3, providing you are clear about the source of the error and what specifically you want changed, they also have a high hit rate.

Where LLMs go wrong, in my experience, is where the problem given is too broad. "Fix this error" compounds steps 1-3 together, and so their solution to the problem tends to explode in complexity. I prefer to keep the tasks small and focussed, which reduces the tendency somewhat. By a similar token, regularly exiting sessions to free up the context window (the amount of text that the model can "remember") generally keeps the LLM more "focussed", and regularly saving changes to git through `git checkout -b [branchname]` and `git commit` allows you to easily roll back changes if it goes really mad.

## Getting started

Make sure you have installed
- [git](https://git-scm.com/install/)
- A GenAI command line interface tool such as [Codex](https://developers.openai.com/codex/cli) or [Claude Code](https://claude.com/product/claude-code), or integrated design environment such as [VSCode](https://code.visualstudio.com/) that can use these services as a plugin
- [Python >3.9](https://www.python.org/downloads/)

To get started please navigate to the [Repressilator_analysis repo](https://github.com/HOLL95/Repressilator_analysis), create a template repository (the green `use this template` button)  to obtain your own copy of the repo (requires a github account). In your terminal then run
```bash
git clone https://github.com/<Your Gihub Username>/Repressilator_analysis
```
Setup a virtual environment and activate it 
```bash
python -m venv <path/to/env>
source env
```
then activate it (Linux/Mac)
```bash
source </path/to/env>/bin/activate
```
and on windows `cmd`
```bash
./.venv/Scripts/Activate.bat
```
Finally, install the repressilator_analysis module into the virtual environment by running
```bash
python -m pip install -e . 
```
## Glossary

**Protein** — a molecule produced inside a cell by reading the instructions in a gene. Proteins carry out most of the cell's functions.

**mRNA (messenger RNA)** — the intermediate molecule that carries a gene's instructions from the DNA to the protein-making machinery, produced during transcription. When we say a protein is "expressed", we mean the cell is actively making mRNA and translating it into protein.

**Transcription repressor** — a protein that binds to a gene and reduces (represses) how much mRNA is produced from it. 

**Fluorescence** — some proteins can be engineered to emit light of a specific colour when illuminated. This lets us track them in a living cell using a microscope.

**Fluorescence arbitrary units (A.U.)** — the raw intensity value recorded by the microscope camera. It is proportional to protein concentration but has no physical unit until calibrated.

**Cytoplasm** — the interior of a cell, excluding the nucleus.

**Nucleus** — the compartment inside the cell that contains DNA. The red fluorescent protein in this experiment is observed here.

**Phase contrast** — a microscopy technique that makes transparent cell structures visible without any dye, by exploiting differences in how light bends through different parts of the cell. 

**Ordinary differential equation (ODE)** — an equation that describes how a quantity changes over time as a function of its current value. 

**Hill coefficient (n)** — a parameter in the repressilator model that describes how sharply a repressor switches between "off" and "on". Higher values mean more switch-like behaviour.
