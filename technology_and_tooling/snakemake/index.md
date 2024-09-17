---
name: "Snakemake"
id: snakemake
dependsOn: [technology_and_tooling.bash_shell]
files: [setup.md, basics.md, advanced.md, additional_features.md, short.md]
attribution:
  - citation: >
      Mölder, F., Jablonski, K.P., Letcher, B., Hall, M.B., Tomkins-Tinch,
      C.H., Sochat, V., Forster, J., Lee, S., Twardziok, S.O., Kanitz, A.,
      Wilm, A., Holtgrewe, M., Rahmann, S., Nahnsen, S., Köster, J., 2021.
      Sustainable data analysis with Snakemake. F1000Res 10, 33.
      Revision c7ae161c.
    url: https://snakemake.readthedocs.io/en/stable/tutorial/basics.html
    image: https://raw.githubusercontent.com/snakemake/snakemake/main/snakemake/report/template/logo.svg
    license: MIT license
summary: |
  This course introduces the text-based workflow system Snakemake. Snakemake is a general-purpose workflow management system for any discipline.
---

This tutorial introduces the text-based workflow system
[Snakemake](https://snakemake.readthedocs.io). Snakemake follows the
[GNU Make](https://www.gnu.org/software/make) paradigm: workflows are
defined in terms of rules that define how to create output files from
input files. Dependencies between the rules are determined
automatically, creating a DAG (directed acyclic graph) of jobs that can
be automatically parallelized.

Snakemake sets itself apart from other text-based workflow systems in
the following way. Hooking into the Python interpreter, Snakemake offers
a definition language that is an extension of
[Python](https://www.python.org) with syntax to define rules and
workflow specific properties. This allows Snakemake to combine the
flexibility of a plain scripting language with a pythonic workflow
definition. The Python language is known to be concise yet readable and
can appear almost like pseudo-code. The syntactic extensions provided by
Snakemake maintain this property for the definition of the workflow.
Further, Snakemake\'s scheduling algorithm can be constrained by
priorities, provided cores and customizable resources and it provides a
generic support for distributed computing (e.g., cluster or batch
systems). Hence, a Snakemake workflow scales without modification from
single core workstations and multi-core servers to cluster or batch
systems. Finally, Snakemake integrates with the package manager
[Conda](https://conda.io) and the container engine
[Singularity](https://www.sylabs.io) such that defining the software
stack becomes part of the workflow itself.

The examples presented in this tutorial come from Bioinformatics.
However, Snakemake is a general-purpose workflow management system for
any discipline. We ensured that no bioinformatics knowledge is needed to
understand the tutorial.

Also have a look at the corresponding
[slides](https://slides.com/johanneskoester/snakemake-tutorial).
