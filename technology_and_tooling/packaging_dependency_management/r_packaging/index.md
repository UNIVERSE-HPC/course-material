---
name: "How to create an R package tutorial"
id: rpackaging
dependsOn: [
]
files: [
  setup.md,
  basics.md,
  creating_your_package.md,
  function_documentation.md,
  testing.md,
  dependencies.md,
  vignettes.md
]
attribution:
    - citation: >
        Wickham, Hadley, and Jennifer Bryan. R packages. " O'Reilly Media, Inc.", 2023.
      url: https://r-pkgs.org/
      license:  CC BY-NC-ND 4.0
summary: |
    This tutorial explains how to develop an R package, including how to write basic unit tests and create pedagogical vignettes which explain how to use a package
---

This short course describes how to make an R package. It broadly follows Chapter 2 of the [free online version](https://r-pkgs.org/) of "R Packages (2nd edition)", by Hadley Wickham and Jennifer Bryan, but it also expands on it in various ways. Here, we aim only to cover those aspects of package development which a user most typically encounters during this process. The [R Packages](https://r-pkgs.org/) book goes into much more detail and is well worth reading in its entirety (particularly as it is short).

Creating an R package is important because the fundamental unit of shareable code in R is a package. Packages can either be shared directly with collaborators or, if they are hosted on online repositories (such as [Github](https://github.com/)) and are accompanied by appropriate software licenses, others are able to use them.

Creating an R package is not only useful for sharing code. Doing so also enforces good habits such as creating more modular code which is well-documented -- both of which are helpful for developing robust code which a future self will undoubtedly be thankful for.

In this tutorial, we use a toy example to explain how to setup an R package. Whenever you are developing code, and especially if that code is intended for use by the community, it is important to take steps to minimise the risks of incorrect code. Unit testing is one way to achieve this, and in this tutorial, we explain how to write basic unit tests for your package. In R, it is also good practice to create a vignette (or multiple vignettes) which are typically pedagogical reference documents that explain to users how to use your package. The final part of this tutorial illustrates how to do this.
