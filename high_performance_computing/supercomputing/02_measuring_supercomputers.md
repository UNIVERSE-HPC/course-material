---
name: Measuring Supercomputers
dependsOn: [high_performance_computing.supercomputing.03_comparison]
tags: [foundation]
learningOutcomes:
  - Calculate and interpret theoretical peak and measured floating-point performance.
  - Distinguish between hardware specifications, benchmark results and application performance.
  - Interpret the TOP500 and Green500 rankings without treating either as a universal measure of performance.
  - Relate performance, electrical power and energy consumption.
attribution:
  - citation: >
      This course material was developed by Alasdair Wilson as part of the Oxford Research
      Software Engineering group (OxRSE).
    url: https://www.rse.ox.ac.uk/
    image: ./supercomputing/images/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Measurements of Performance

Calling one computer *faster* than another is incomplete unless we say what work is being measured.
One system might finish a particular simulation sooner, while another might process more independent jobs or hold a larger problem in memory.

A widely used measure of numerical computing performance is the number of **floating-point operations per second**.
A floating-point operation, or FLOP, is an arithmetic operation on floating-point data, such as an addition or multiplication.
A rate of one billion such operations each second is one gigaflop per second, written \(1\ \text{GFLOP/s}\).

:::callout{variant="info"}
A floating-point operation is not necessarily the same as a processor instruction.
A vector instruction can apply the same operation to several numbers at once, while a fused multiply-add performs both a multiplication and an addition.
Processor specifications account for this when stating the maximum number of FLOPs that can be performed in each clock cycle.
:::

Supercomputer performance reaches very large values, so decimal prefixes are used:

| Operations per second | Prefix | Unit    |
| --------------------- | ------ | ------- |
| \(10^3\)              | kilo   | kFLOP/s |
| \(10^6\)              | mega   | MFLOP/s |
| \(10^9\)              | giga   | GFLOP/s |
| \(10^{12}\)           | tera   | TFLOP/s |
| \(10^{15}\)           | peta   | PFLOP/s |
| \(10^{18}\)           | exa    | EFLOP/s |

FLOP/s describes only one kind of work, basic floating point number crunching.
It does not measure memory capacity, data-transfer rates, storage performance or the runtime of a particular application which might rely on the performance of many aspects of the system.
A single performance figure is therefore useful only when its meaning and method of measurement are understood.

## Theoretical Peak Performance

The maximum floating-point rate implied by a processor's design is its **theoretical peak performance**.
For CPU-only work, a simplified calculation would be:

```math
R_{\mathrm{peak}} =
\text{CPU-cores}
\times
\text{clock cycles per second}
\times
\text{FLOPs per cycle}.
```

Consider a system with 512 nodes, two 64-core processors per node, a clock frequency of 2.0 GHz and a maximum of 16 double-precision FLOPs per cycle.
It contains:

```math
512 \times 2 \times 64 = 65\,536\ \text{CPU-cores}.
```

Its theoretical peak performance is:

```math
65\,536
\times
2.0 \times 10^9
\times
16
=
2.097152 \times 10^{15}\ \text{FLOP/s},
```

or approximately \(2.10\ \text{PFLOP/s}\).

The TOP500 project calls this value \(R_{\mathrm{peak}}\).
For a system containing different kinds of processor or accelerator, the theoretical contributions of those devices must be calculated separately and added together.

\(R_{\mathrm{peak}}\) is a ceiling, not a rate that applications should be expected to sustain.
Reaching it would require every processing unit to perform the maximum number of floating-point operations during every clock cycle.
Real programs also move data, wait for memory, communicate between nodes, make decisions and perform operations that are not floating-point arithmetic.

## Measuring Performance with a Benchmark

A **benchmark** is a defined workload used to measure and compare computing systems.
Running the same workload according to the same rules provides a more useful comparison than relying on hardware specifications alone.

The principal benchmark used to compare supercomputers is the [High-Performance Linpack benchmark](https://www.netlib.org/benchmark/hpl/), usually abbreviated to HPL.
HPL solves a dense system of linear equations using double-precision floating-point arithmetic.
Its problem size can be chosen to use a large proportion of the available memory, allowing the benchmark to exercise a complete distributed-memory system.

The highest HPL performance achieved by a system is called \(R_{\mathrm{max}}\).
Like \(R_{\mathrm{peak}}\), it is reported in FLOP/s.

The ratio between the two shows what fraction of theoretical peak performance HPL achieved:

```math
\text{fraction of peak} =
\frac{R_{\mathrm{max}}}{R_{\mathrm{peak}}}.
```

An \(R_{\mathrm{max}}\) of \(1.5\ \text{PFLOP/s}\) on a system with an \(R_{\mathrm{peak}}\) of \(2.0\ \text{PFLOP/s}\) means the system reached 75% of its theoretical peak.

HPL performs a large amount of arithmetic on dense data, allowing a system to sustain a large fraction of its theoretical floating-point performance.
Applications dominated by memory access, network communication, storage or other kinds of computation can behave very differently.
An HPL result is therefore a reproducible measurement of HPL performance, not a prediction of every application's runtime.

The most informative benchmark for a particular workload is usually one that resembles that workload.
HPL is valuable for comparing a particular aspect of whole supercomputers, while an application-specific benchmark shows how well the available system supports a particular research workload.

## The TOP500

The [TOP500 project](https://www.top500.org/lists/top500/) has published a list of supercomputer results twice a year since 1993.
Systems are ranked primarily by their \(R_{\mathrm{max}}\) result from HPL, with \(R_{\mathrm{peak}}\) used to break a tie.

The list also records information such as processor count, processor and accelerator types, interconnect, installation site and power consumption.
It is therefore useful both as a ranking and as a dataset for examining how supercomputer architecture changes over time.

A TOP500 position has a precise meaning: it describes a system's position according to its reported HPL result.
It does not establish that the system will run every application faster than all systems below it.

The TOP500 is not a catalogue from which most users select a supercomputer.
System designers and funding bodies may use it when assessing proposed systems, while facility operators use performance measurements to understand the systems they run.
For users, its main value is explaining published specifications and placing the resources available to them in a wider context.

## Performance and Energy

Performance is only one consideration when designing and operating a supercomputing facility.
Electrical power has direct financial and environmental costs, and almost all of it eventually becomes heat that the cooling system must remove.

The [Green500](https://www.top500.org/lists/green500/) ranks TOP500 systems by the HPL performance delivered per watt of electrical power.
Its energy-efficiency measure is:

```math
\text{performance per watt} =
\frac{R_{\mathrm{max}}}{\text{average electrical power during the HPL run}}.
```

It is usually reported in GFLOP/s per watt.
Because one watt is one joule per second, this is equivalent to the number of gigaflops performed per joule of energy.

A large system can consume more power in total but deliver more work per watt.
High absolute performance and high energy efficiency are therefore different properties, but they are not opposites.

Power and energy must also be distinguished.
Power is the rate at which energy is used, while the energy consumed by a workload depends on both its average power and its runtime:

```math
\text{energy} = \text{average power} \times \text{runtime}.
```

A system drawing more power may still consume less energy if it completes a workload sufficiently quickly.
Like the TOP500 rankings, a supercomputer's Green500 rank cannot replace measurements made with the workloads and constraints that matter to its users.

::::challenge{id=sc_measure.compare title="Comparing Performance Measurements"}
Two fictional systems have produced the following results:

| System | \(R_{\mathrm{peak}}\) | \(R_{\mathrm{max}}\) | HPL power |
| ------ | --------------------- | -------------------- | --------- |
| Cedar  | 4.0 PFLOP/s           | 3.0 PFLOP/s          | 1.5 MW    |
| Rowan  | 3.0 PFLOP/s           | 2.7 PFLOP/s          | 0.75 MW   |

1. Calculate the fraction of theoretical peak achieved by each system.
1. Which system would be placed higher in the TOP500?
1. Calculate the HPL performance per watt of each system in GFLOP/s per watt.
1. Which system would be favoured by the Green500 measure?
1. Do these measurements justify describing either system as better overall?
1. What additional measurements would be needed to compare the systems for a particular research workload?

:::solution
Cedar achieves:

```math
\frac{3.0}{4.0} = 0.75 = 75\%
```

of its theoretical peak.

Rowan achieves:

```math
\frac{2.7}{3.0} = 0.90 = 90\%
```

of its theoretical peak.

Cedar would be placed higher in the TOP500 because its \(R_{\mathrm{max}}\) is \(3.0\ \text{PFLOP/s}\), compared with Rowan's \(2.7\ \text{PFLOP/s}\).

Dividing Cedar's \(3.0\ \text{PFLOP/s}\) by \(1.5\ \text{MW}\) gives \(2.0\ \text{GFLOP/s}\) per watt.
Dividing Rowan's \(2.7\ \text{PFLOP/s}\) by \(0.75\ \text{MW}\) gives \(3.6\ \text{GFLOP/s}\) per watt.
Rowan would therefore be favoured by the Green500 measure.

Neither system is better according to every measure.
Cedar produced the higher HPL performance, while Rowan achieved a larger fraction of its theoretical peak and more HPL performance per watt.

Comparing the systems for a particular research workload would require a representative application benchmark.
Relevant measurements could include time to solution, throughput, memory use, average power and total energy consumption while running that workload.
The HPL power values cannot be used to calculate the energy consumed by a different application.
:::
::::

## Investigating the Current Lists

The TOP500 and Green500 are updated regularly, so examining the current data is more useful than memorising a particular ranking.

::::challenge{id=sc_measure.lists title="Interpreting the TOP500 and Green500"}
Open the most recent [TOP500](https://www.top500.org/lists/top500/) and corresponding [Green500](https://www.top500.org/lists/green500/) lists, then choose two systems for which power measurements are reported.

For each system:

1. Record its TOP500 rank, \(R_{\mathrm{max}}\), \(R_{\mathrm{peak}}\), power and Green500 energy-efficiency value.
1. Calculate the fraction of theoretical peak achieved.
1. Calculate \(R_{\mathrm{max}}\) per watt and compare it with the published Green500 value.
1. Are there differences in the listed processor, accelerator or interconnect information that might explain the results?
1. What conclusions do the rankings support and which would require an application-specific benchmark?

:::solution
The selected systems and values will depend on the current list.

For each system, divide \(R_{\mathrm{max}}\) by \(R_{\mathrm{peak}}\) to calculate its fraction of peak.
Convert the units before dividing \(R_{\mathrm{max}}\) by power; dividing PFLOP/s by MW directly produces GFLOP/s per watt because *peta* is \(10^{15}\) and *mega* is \(10^6\).
Your calculated result should broadly agree with the published Green500 value.

The TOP500 comparison demonstrates relative HPL performance.
The Green500 comparison demonstrates relative HPL performance per watt under its measurement rules.
Processor, accelerator and interconnect information may help explain the results, but neither ranking by itself establishes how quickly or efficiently another application will run.
Conclusions about application performance would require a representative application benchmark.
:::
::::

These measurements describe different aspects of a supercomputer’s capabilities, but none predicts application performance on its own.
Real performance depends on how well a workload matches the processors, memory and network, and on how effectively the program uses them.
The parallel-programming courses examine how software turns that hardware capability into useful performance.
