---
name: Parallelisation with OpenMP
dependsOn: [high_performance_computing.hpc_openmp.02_intro_openmp]
tags: [parallelisation, OMP APIs]
learningOutcomes:
  - Describe the functionality of OpenMP pragma directives.
  - Explain the concept of a parallel region and its significance in OpenMP.
  - Understand the scope of variables in OpenMP parallel regions.
  - Implement parallelisation in a program using OpenMP directives.
  - Use OpenMP library functions to manage threads and thread-specific information.
  - Evaluate different schedulers available in OpenMP for loop iterations.
  - Assess the impact of scheduling behaviors on program execution.
---


## Using OpenMP in a Program

As we introduced in the last episode,
OpenMP directives are special comments indicated by `#pragma omp` statements that guide the compiler in creating parallel code.
They mark sections of code to be executed concurrently by multiple threads.
At a high level, the C/C++ syntax for pragma directives is as follows:

```c
#pragma omp <name_of_directive> [ <optional_clause> ...]
```

Following a directive are multiple optional clauses, which are themselves C expressions and may contain other clauses,
with any arguments to both directives and clauses enclosed in parentheses and separated by commas. For example:

```c
#pragma omp a-directive a-clause(argument1, argument2)
```

OpenMP offers a number of directives for parallelisation, although the two we'll focus on in this episode are:

- The `#pragma omp parallel` directive specifies a block of code for concurrent execution.
- The `#pragma omp for` directive parallelizes loops by distributing loop iterations among threads.

## Our First Parallelisation

For example, amending our previous example,
in the following we specify a specific block of code to run parallel threads,
using the OpenMP runtime routine `omp_get_thread_num()` to return
the unique identifier of the calling thread:

```c
#include <stdio.h>
#include <omp.h>
int main() {
    #pragma omp parallel
    {
        printf("Hello from thread %d\n", omp_get_thread_num());
    }
}
```

So assuming you've specified `OMP_NUM_THREADS` as `4`:

```text
Hello from thread 0
Hello from thread 1
Hello from thread 3
Hello from thread 2
```

Although the output may not be in the same order,
since the order and manner in which these threads (and their `printf` statements) run is not guaranteed.

So in summary, simply by adding this directive we have accomplished a basic form of parallelisation.

## What about Variables?

So how do we make use of variables across, and within, our parallel threads?
Of particular importance in parallel programs is how memory is managed and how and where variables can be manipulated,
and OpenMP has a number of mechanisms to indicate how they should be handled.
Essentially, OpenMP provides two ways to do this for variables:

- **Shared**: A single instance of the variable is shared among all threads, meaning every thread can access
and modify the same data. This is useful for shared resources but requires careful management to prevent conflicts or unintended behavior.
- **Private**: Each thread gets its own isolated copy of the variable, similar to how variables are private in `if` statements or functions, where each thread’s version is independent and doesn't affect others.

For example, what if we wanted to hold the thread ID and the total number of threads within variables in the code block?
Let's start by amending the parallel code block to the following:

```c
        ...
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        printf("Hello from thread %d out of %d\n", thread_id, num_threads);
```

Here, `omp_get_num_threads()` returns the total number of available threads.
If we recompile and re-run we should see:

```text
Hello from thread 0 out of 4
Hello from thread 1 out of 4
Hello from thread 3 out of 4
Hello from thread 2 out of 4
```

::::challenge{id=scoping, title='OpenMP and C Scoping'}
Try printing out `num_threads` at the end of the program, after the `#pragma` code block, and recompile.
What happens? Is this what you expect?
:::solution
Since the variable is scoped only to the code block within the curly braces,
as with any C code block, `num_threads` is no longer in scope and cannot be read.
:::
::::

Now by default, variables declared within parallel regions are private to each thread.
But what about declarations outside of this block? For example:

```c
    ...
    int num_threads, thread_id;

    #pragma omp parallel
    {
        num_threads = omp_get_num_threads();
        thread_id = omp_get_thread_num();
        printf("Hello from thread %d out of %d\n", thread_id, num_threads);
    }
```

Which may seem on the surface to be correct.
However, this illustrates a critical point about why we need to be careful.
Now, since the variable declarations are outside the parallel block, they are, by default, *shared* across threads. This means any thread can modify these variables at any time,
which is potentially dangerous. So here, `thread_id` may hold the value for another thread identifier when it's printed,
since there is an opportunity between its assignment and its access within `printf` to be changed in another thread.
This could be particularly problematic with a much larger data set and complex processing of that data,
where it might not be obvious that incorrect behaviour has happened at all,
and lead to incorrect results.
This is known as a *race condition*, and we'll look into them in more detail in the next episode.

But there’s another common scenario to watch out for. What happens when we want to declare a variable outside the parallel region, make it private, and retain its initial value inside the block? Let’s consider the following example:

```c
int initial_value = 15;

#pragma omp parallel private(initial_value)
{
    printf("Thread %d sees initial_value = %d\n", omp_get_thread_num(), initial_value);
}
```

You might expect each thread to start with `initial_value` set to `15`.
However, this is not the case. When a variable is declared as `private`, each thread gets its own copy
of the variable, but those copies are **uninitialised**—they don’t inherit the value from the variable outside
the parallel region. As a result, the output may vary and include seemingly random numbers, depending on the compiler and runtime.

To handle this, you can use the `firstprivate` directive. With `firstprivate`, each thread gets its own private copy of the variable,
and those copies are initialised with the value from the variable outside the parallel region. For example:

```c
int initial_value = 15;

#pragma omp parallel firstprivate(initial_value)
{
    printf("Thread %d sees initial_value = %d\n", omp_get_thread_num(), initial_value);
}
```

Now, the initial value is correctly passed to each thread:

```text

Thread 0 sees initial_value = 15
Thread 1 sees initial_value = 15
Thread 2 sees initial_value = 15
Thread 3 sees initial_value = 15

```

Each thread begins with initial_value set to `15`. This avoids the unpredictable behavior of uninitialised
variables and ensures that the initial value is preserved for each thread.

::::callout

## Observing the Race Condition

We can observe the race condition occurring by adding a sleep command between the `thread_id` assignment
and use. Add `#include <unistd.h>` to the top of your program, and after `thread_id`'s assignment, add `sleep(2);`
which will force the code to wait for 2 seconds before the variable is accessed,
providing more opportunity for the race condition to occur.
Hopefully you'll then see the unwanted behaviour emerge, for example:

```text
Hello from thread 2 out of 4
Hello from thread 2 out of 4
Hello from thread 2 out of 4
Hello from thread 2 out of 4
```

::::

But with our code, this makes variables potentially *unsafe*, since within a single thread,
we are unable to guarantee their expected value.
One approach to ensuring we don't do this accidentally is to specify that there is no default behaviour for variable
classification.
We can do this by changing our directive to:

```c
    #pragma omp parallel default(none)
```

Now if we recompile, we'll get an error mentioning that these variables aren't specified for use within the parallel region:

```text
hello_world_omp.c: In function 'main':
hello_world_omp.c:10:21: error: 'num_threads' not specified in enclosing 'parallel'
   10 |         num_threads = omp_get_num_threads();
      |         ````````````^`````````````````````~
hello_world_omp.c:8:13: note: enclosing 'parallel'
    8 |     #pragma omp parallel default(none)
      |             ^~~
hello_world_omp.c:11:19: error: 'thread_id' not specified in enclosing 'parallel'
   11 |         thread_id = omp_get_thread_num();
      |         `````````~^`````````````````````
hello_world_omp.c:8:13: note: enclosing 'parallel'
    8 |     #pragma omp parallel default(none)
      |             ^~~
```

So we now need to be explicit in every case for which variables are accessible within the block,
and whether they're private or shared:

```c
    #pragma omp parallel default(none) private(num_threads, thread_id)
```

So here, we ensure that each thread has its own private copy of these variables,
which is now thread safe.

## Parallel `for` Loops

A typical program uses `for` loops to perform many iterations of the same task,
and fortunately OpenMP gives us a straightforward way to parallelise them,
which builds on the use of directives we've learned so far.

```c
    ...
    int num_threads, thread_id;
    
    omp_set_num_threads(4);
    
    #pragma omp parallel for default(none) private(num_threads, thread_id)
    for (int i = 1; i <= 10; i++)
    {
        num_threads = omp_get_num_threads();
        thread_id = omp_get_thread_num();
        printf("Hello from iteration %i from thread %d out of %d\n", i, thread_id, num_threads);
    }
}
```

So essentially, very similar format to before, but here we use `for` in the pragma preceding a loop definition,
which will then assign 10 separate loop iterations across the 4 available threads.
Later in this episode we'll explore the different ways in which OpenMP is able to schedule iterations from loops across these threads,
and how to specify different scheduling behaviours.

:::callout

## Nested Loops with `collapse`

By default, OpenMP parallelises only the outermost loop in a nested structure. This works fine for many cases,
but what if the outer loop doesn’t have enough iterations to keep all threads busy, or the inner loop does most of the work?  In these situations, we can use the `collapse` clause to combine the iteration
spaces of multiple loops into a single loop for parallel execution.

For example, consider a nested loop structure:

```c
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
        ...
    }
}
```

Without the `collapse` clause, the outer loop is divided into `N` iterations, and the inner loop is executed sequentially
within each thread. If `N` is small or `M` contains the bulk of the work, some threads might finish their work quickly
and sit idle, waiting for others to complete. This imbalance can slow down the overall execution of the program.

Adding `collapse` changes this:

```c
#pragma omp parallel for collapse(2)
for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
        ...
    }
}
```

The number `2` in `collapse(2)` specifies how many nested loops to combine.
Here, the two loops `(i and j)` are combined into a single iteration space with `N * M` iterations.
These iterations are then distributed across the threads, ensuring a more balanced workload.
:::

:::callout

## A Shortcut for Convenience

The `#pragma omp parallel for` is actually equivalent to using two separate directives.
For example:

```c
#pragma omp parallel
{
    #pragma omp for     
    for (int i = 1; i <=10; i++)
    {
        ...
    }
}
```

...is equivalent to:

```c
#pragma omp parallel for
for (int i = 1; i <=10; i++)
{
    ...
}
```

In the first case, `#pragma omp parallel` spawns a group of threads, whilst `#pragma omp for` divides the loop iterations between them.
But if you only need to do parallelisation within a single loop, the second case is more convenient.
:::

Note we also explicitly set the number of desired threads to 4, using the OpenMP `omp_set_num_threads()` function,
as opposed to the environment variable method.
Use of this function will override any value set in `OMP_NUM_THREADS`.

You should see something (but perhaps not exactly) like:

```text
Hello from iteration 1 from thread 0 out of 4
Hello from iteration 2 from thread 0 out of 4
Hello from iteration 3 from thread 0 out of 4
Hello from iteration 4 from thread 1 out of 4
Hello from iteration 5 from thread 1 out of 4
Hello from iteration 6 from thread 1 out of 4
Hello from iteration 9 from thread 3 out of 4
Hello from iteration 10 from thread 3 out of 4
Hello from iteration 7 from thread 2 out of 4
Hello from iteration 8 from thread 2 out of 4
```

So with careful attention to variable scoping,
using OpenMP to parallelise an existing loop is often quite straightforward.
However, particularly with more complex programs, there are some aspects and potential pitfalls with OpenMP parallelisation
we need to be aware of - such as race conditions - which we'll explore in the next episode.

::::challenge{id=callingelsewhere, title="Calling Thread Numbering Functions Elsewhere?"}

Write, compile and run a simple OpenMP program that calls both `omp_get_num_threads()` and `omp_get_thread_num()` outside of a parallel region,
and prints the values received. What happens?

:::solution

`omp_get_num_threads()` will return 1 as you might expect, since there is only the primary thread active.

`omp_get_thread_num()` will return 0, which refers to the identifier for the primary thread, which is zero.
:::
::::

## Using Schedulers

Whenever we use a `parallel for`, the iterations have to be split into smaller chunks so each thread has something to do.
In most OpenMP implementations, the default behaviour is to split the iterations into equal sized chunks,

```c
int CHUNK_SIZE = NUM_ITERATIONS / omp_get_num_threads();
```

If the amount of time it takes to compute each iteration is the same, or nearly the same, then this is a perfectly
efficient way to parallelise the work. Each thread will finish its chunk at roughly the same time as the other thread.
But if the work is imbalanced, even if just one thread takes longer per iteration, then the threads become out of
sync and some will finish before others. This not only means that some threads will finish before others and have to
wait until the others are done before the program can continue, but it's also an inefficient use of resources to have
threads/cores idling rather than doing work.

Fortunately, we can use other types of "scheduling" to control how work is divided between threads. In simple terms, a
scheduler is an algorithm which decides how to assign chunks of work to the threads. We can control the schedule we
want to use with the `schedule` directive:

```c
#pragma omp parallel for schedule(SCHEDULER_NAME, OPTIONAL_ARGUMENT)
for (int i = 0; i < NUM_ITERATIONS; ++i) {
    ...
}
```

`schedule` takes two arguments: the name of the scheduler and an optional argument.

| Scheduler | Description | Argument |  Uses |
| - | - | - | - |
| **static** | The work is divided into equal-sized chunks, and each thread is assigned a chunk to work on at compile time. | The chunk size to use (default: divides iterations into chunks of approx. equal size). | Best used when the workload is balanced across threads, where each iteration takes roughly the same amount of time. |
| **dynamic** | The work is divided into lots of small chunks, and each thread is dynamically assigned a new chunk with it finishes its current work. | The chunk size to use (default: 1). |  Useful for loops with a workload imbalance, or variable execution time per iteration. |
| **guided** |  The chunk sizes start large and decreases in size gradually. | The smallest chunk size to use (default: 1). | Most useful when the workload is unpredictable, as the scheduler can adapt the chunk size to adjust for any imbalance. |
| **auto** | The best choice of scheduling is chosen at run time. | - | Useful in all cases, but can introduce additional overheads whilst it decides which scheduler to use. |
| **runtime** | Determined at runtime by the `OMP_SCHEDULE` environment variable or `omp_schedule` pragma. | - | - |

:::callout

## How the `auto` Scheduler Works

The `auto` scheduler lets the compiler or runtime system automatically decide the best way to distribute work among threads.
This is really convenient because you don’t have to manually pick a scheduling method—the system handles it for you.
It’s especially handy if your workload distribution is uncertain or changes a lot. But keep in mind that how
well `auto` works can depend a lot on the compiler. Not all compilers optimize equally well, and there might be a bit of overhead
as the runtime figures out the best scheduling method, which could affect performance in highly optimized code.

The [OpenMP documentation](https://www.openmp.org/wp-content/uploads/OpenMP4.0.0.pdf) states that with `schedule(auto)`, the scheduling decision is left to the compiler or runtime system. So, how does the compiler make this decision? When using GCC, which is common in many environments including HPC, the `auto` scheduler often maps to `static` scheduling. This means it splits the work into equal chunks ahead of time for simplicity and performance. `static` scheduling is straightforward and has low overhead, which often leads to efficient execution for many applications.

However, specialised HPC compilers, like those from Intel or IBM, might handle `auto` differently. These advanced compilers can dynamically adjust the scheduling
method during runtime, considering things like workload variability and specific hardware characteristics to optimize performance.

So, when should you use `auto`? It’s great during development for quick performance testing without having to manually adjust scheduling methods. It’s also
useful in environments where the workload changes a lot, letting the runtime adapt the scheduling as needed. While `auto` can make your code simpler, it’s
important to test different schedulers to see which one works best for your specific application.

:::

::::challenge{id=differentschedulers, title="Try Out Different Schedulers"}

Try each of the static and dynamic schedulers on the code below,
which uses `sleep` to mimic processing iterations that take increasing amounts of time to complete as the loop increases.
`static` is already specified, so replace this next with `dynamic`.
Which scheduler is fastest?

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 4
#define NUM_ITERATIONS 8

int main ( ) {
    int i;
    double start = omp_get_wtime();

    #pragma omp parallel for num_threads(NUM_THREADS) schedule(static)
    for (i = 0; i < NUM_ITERATIONS; i++) {
        sleep(i);
        printf("Thread %d finished iteration %d\n", omp_get_thread_num(), i);
    }

    double end = omp_get_wtime();
    printf("Total time for %d reps = %f\n", NUM_ITERATIONS, end - start);
}
```

Try out the different schedulers and see how long it takes to finish the loop.
Which scheduler was best?

:::solution

You should see something like:

```text
Static:  Total time for 8 reps = 13.003299
Dynamic: Total time for 8 reps = 10.007052
```

Here we can see that `dynamic` is the fastest,
which is better with iterations taking differing amounts of time.
But note there is an overhead to using dynamic scheduling,
threads that complete need to stop and await a new value to process from a next iteration.
:::
::::

::::challenge{id=differentchunksizes, title="Different Chunk Sizes"}

With a dynamic scheduler, the default chunk size is 1. What happens if specify a chunk size of 2,
i.e. `scheduler(dynamic, 2)`?
:::solution

```text
Dynamic: Total time for 16 reps = 13.004029
```

So here, we now see approximately the same results.
By increasing the chunk size, the dynamic scheduler behaves more like the static one,
since the workload for static would have the same chunk size calculated to be 2
(`NUM_ITERATIONS / NUM_THREADS = CHUNK_SIZE`, `8 / 4 = 2`).
:::
::::

:::callout

## A Matter of Convenience

We've seen that we can amend our code directly to use different schedulers,
but when testing our code with each of them editing and recompiling can become tedious.
Fortunately we can use the `OMP_SCHEDULE` environment variable to specify the scheduler instead,
as well as the chunk size, so we don't need to recompile.

Edit your code to specify `runtime` as the scheduler, i.e. `scheduler(runtime)`,
recompile, then set the environment variable in turn to each scheduler, e.g.

```bash
export OMP_SCHEDULE=dynamic
```

Then rerun. Try it with different chunk sizes too, e.g.:

```bash
export OMP_SCHEDULE=static,1
```

:::
