---
name: "Aims and workflows"
teaching: 15
exercises: 15
dependsOn: [
]
tags: [rpackaging]
attribution:
    - citation: >
        Wickham, Hadley, and Jennifer Bryan. R packages. " O'Reilly Media, Inc.", 2023.
      url: https://r-pkgs.org/
      license:  CC BY-NC-ND 4.0
---

# Basics: the aims and workflows

## Aims

We aim to create an R package, "regexcite", which contains a few simple functions for processing strings. In doing so, we will:

- create the functions themselves
- show how to set up version control
- explain workflows for package development including:
  - function documentation
  - unit testing
  - package documentation
  - how to include package dependencies

There's a short presentation [here](dependencies/how_to_make_an_R_package.pdf), which explains a little more about R packages and software licenses.

## Key workflows
Key to the process of our package development will be the following commands, which you will execute repeatedly throughout its development:

![Wickham's workflows](dependencies/workflow_wickham.png)

This figure has been reproduced from "R Packages (2nd edition)", by Hadley Wickham and Jennifer Bryan.

- `load_all()` loads all functions (both those internal to the package and exposed to the user -- more on this distinction later) so that a user can test them out interactively in the RStudio console. You should **never** during package development load code from your package into the environment directly by highlighting your own function then executing it. This is because `load_all()` much more accurately simulates the process of installing your package than relying on things defined in your global environment
- `test()` runs all unit tests for the package. A useful alternative is `test_active_file()` which runs only those unit tests given in a file currently active in RStudio
- `document()` builds the documentation for your package's functions using `roxygen`. It then allows you to browse the help page for your own functions by executing `?custom_function_name`
- `check()` automatically builds and checks a source package, using all known best practices. If your package passes this, it is a good sign that it may be ready to share with others.

One thing that the above workflow diagram is missing is a check of code unit testing coverage, which can be achieved via the `test_coverage()` function in the `covr` package.
