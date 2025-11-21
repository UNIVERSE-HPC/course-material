---
name: Understanding Code Scalability
dependsOn: []
tags: [optimisation]
learningOutcomes:
  - Describe why code scalability is important when using HPC resources.
  - Explain the difference between wall time and CPU time.
  - Describe the differences between strong and weak scaling.
  - Summarise the dangers of premature optimisation.
---

When we submit a job to a cluster that runs our code, we have the option of specifying the number of CPUs (and in some cases GPUs) that will be allocated to the job. We need to consider to what extent that code is _scalable_ with regards to how it uses these resources, to avoid the risk of consuming more resources than can be effectively used. As part of the application process for having new code installed on DiRAC, its scalability characteristics need to be measured. This helps inform how best to assign CPU resources when configuring jobs to run with that code.

There are two primary measures of execution time we need to consider for any given code:

- **Wall clock time (or actual time)** - this is the time it takes to run from start of execution to the end, as measured on a clock. In terms of scaling measurements, this does not include any time waiting for the job to start.
- **CPU time** - this is the time actually spent running your code on a CPU, when it is processing instructions. This does not include time waiting for input or output operations, such as reading in an input file, or any other waiting caused by the program or operating system.

<!-- In some cases where you are running on a system which is shared by other users, your run may be swapped out to enable other users to use the system. Most systems within DiRAC are configured to have exclusive access. So your code will not be competing with other programs on the compute nodes, but may be competing on the network fabric, or for storage bandwidth. -->

## How can we Characterise a Code's Scalability?

Before we consider running and using code on an HPC resource, we need to understand it's _scaling profile_ - so we can determine how the code will scale as we add more CPU cores to running it. That way, when we run code we can request a suitable amount of resources with minimal waste. There are two types of scaling profile we need to determine:

- **Strong scaling:** Strong scaling is how well your code run times changes whilst keeping the problem size constant, but increasing the number of CPU cores (and nodes) being used. Ideally, a 100% scalable application will have a profile that halves the time to complete when given twice as many cores. This is rarely met, as most real world applications have some serial portion or unintended delays (such as communication overheads) which will limit the code's scalability.
- **Weak scaling:** This is similar to strong scaling, but in this case as we increase the number of cores, we also increase the problem size by the same factor. This type of scaling is more easily met, and should result in a flat line in run times vs. core count if the code has good weak scaling characteristics. In theory this removes the impact of the serial portion of your code, but in reality there is still a limit.

 <!-- - the improvement in speed of execution of a task executed on two similar architectures with different resources.  -->

Once we understand these scaling profiles for our code, we'll have an idea of the **speedup** capable when using multiple cores. These measurements give us good indications for how our code should be specified on for our cluster, in terms of the overall job size and the amount of resources that should be requested.

## I'm a Developer, Should I Optimise my Code?

As a developer, if your code happens to take too long to run or scales badly it's tempting to dive in and try to optimise it straight away. But before you do, consider the following [rules of optimisation](https://hackernoon.com/the-rules-of-optimization-why-so-many-performance-efforts-fail-cf06aad89099):

1. Don't,
1. Don't... _yet_, and,
   Another rule is sometimes added:
1. If you must optimise your code, _profile_ it first.

In non-trivial cases premature optimisation is regarded as bad practice, since optimisation may lead to additional code complexity, incorrect results and reduced readability, making the code harder to understand and maintain. It is often effort-intensive, and difficult at a low level, particularly with modern compilers and interpreters, to improve on or anticipate the optimisations they already implement. A general maxim is to focus on writing understandable code and getting things working first - the former helps with the latter. Then, once strong and weak scaling profiles have been measured, if optimisation is justified you can _profile_ your code, and work out where the majority of time is being spent and how best to optimise it. If you want to take this philosophy further, consider the [Rules of Optimisation Club](https://wiki.c2.com/?RulesOfOptimizationClub).

So what is _profiling_? Profiling your code is all about understanding its complexity and performance characteristics. The usual intent of profiling is to work out how best to _optimise_ your code to improve its performance in some way, typically in terms of speedup or memory and disk usage. In particular, profiling helps identify _where_ bottlenecks exist in your code, and helps avoid summary judgments and guesses which will often lead to unnecessary optimisations.

::::callout

## Profilers

Each programming language will typically offer some open-source and/or free tools
on the web, with you can use to profile your code. Here are some examples of
tools. Note though, depending on the nature of the language of choice, the
results can be hard or easy to interpret. In the following we will only list
open and free tools:

>

- Python: [line_profiler](https://github.com/pyutils/line_profiler),
  [prof](https://docs.python.org/3.9/library/profile.html)
- JavaScript: [firebug](https://github.com/firebug/firebug)
- Ruby: [ruby-prof](https://github.com/ruby-prof/ruby-prof)
- C/C++: [xray](https://llvm.org/docs/XRay.html),
  [perf](https://perf.wiki.kernel.org/index.php/Main_Page),
  [gprof](https://man7.org/linux/man-pages/man1/gprof.1.html)
- R: [profvis](https://github.com/rstudio/profvis)

::::

Donald Knuth said _"we should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil. Yet we should not pass up our opportunities in that critical 3%."_ In short, optimise the obvious trivial things, but avoid non-trivial optimisations until you've understood what needs to change. Optimisation is often difficult and time consuming. Premature optimization may be a waste of your time!
