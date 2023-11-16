---
name: "Setup"
dependsOn: [
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

To go through this tutorial, you need R installed on your machine, which can be freely downloaded from [R](https://www.r-project.org/). Whilst it is not prerequisite, we strongly suggest that you use the [RStudio](https://posit.co/download/rstudio-desktop/) IDE when developing your package.

You also need the [devtools](https://www.r-project.org/nosvn/pandoc/devtools.html) package installed on your machine which can be achieved by typing `install.packages(devtools)` from the R console. Internally, loading `devtools` also loads the `usethis` package which we will make extensive use of throughout this tutorial.
