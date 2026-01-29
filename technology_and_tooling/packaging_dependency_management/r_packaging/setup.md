---
name: "Setup"
dependsOn: [
    technology_and_tooling.r_packaging.index
]
tags: [rpackaging]
attribution:
    - citation: >
        Wickham, Hadley, and Jennifer Bryan. R packages. " O'Reilly Media, Inc.", 2023.
      url: https://r-pkgs.org/
      license:  CC BY-NC-ND 4.0

---

# Setup

## Requirements

To go through this tutorial, you need R installed on your machine, which can be freely downloaded from [R](https://www.r-project.org/). Whilst it is not prerequisite, we strongly suggest that you use the [RStudio](https://posit.co/download/rstudio-desktop/) IDE when developing your package since it helps to automate a number of steps (like automating the generation of docstrings for a function).

You also need the [devtools](https://www.r-project.org/nosvn/pandoc/devtools.html) package installed on your machine which can be achieved by typing `install.packages(devtools)` from the R console. Internally, loading `devtools` also loads the `usethis` package which we will make extensive use of throughout this tutorial.

## Before starting the tutorial

1. Create a fresh folder where you intend to house an R package
2. Open up RStudio and change the working directory to be within the folder
3. Load `devtools` via `library(devtools)` (which loads `usethis`):
   - this is the interface to a range of tools which greatly facilitate R package development
