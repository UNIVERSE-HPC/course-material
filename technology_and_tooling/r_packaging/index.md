---
name: "How to create an R package tutorial"
id: rpackaging
dependsOn: [
  is there a software licensing tutorial?
]
files: [
  setup.md,
  basics.md,
  testing.md
]
attribution:
    - citation: >
        Wickham, Hadley, and Jennifer Bryan. R packages. " O'Reilly Media, Inc.", 2023.
      url: https://r-pkgs.org/
      license:  CC BY-NC-ND 4.0
summary: |
    This tutorial explains how to develop an R package, including how to write
    basic unit tests.
---

This short course describes how to make an R package. It broadly follows Chapter 2 of the [online version](https://r-pkgs.org/) of "R Packages (2nd edition)", by Hadley Wickham and Jennifer Bryan.

Creating an R package is important because the fundamental unit of shareable code in R is a package. Packages can either be shared directly with collaborators or, if they are hosted on online repositories and are accompanied by appropriate software licenses, they can be made freely available for others to use.

In this tutorial, we use a toy example to explain how to setup an R package. Whenever you are developing code, and especially if that code is intended for use by the community, taking steps to minimise the risks of incorrect code. Unit testing is one way to achieve this, and in this tutorial, we explain how to write basic unit tests for your package.



Requirements:

* have R and RStudio installed
* have the `devtools` package installed
