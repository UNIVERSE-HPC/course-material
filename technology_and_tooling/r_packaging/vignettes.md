---
name: "Package vignettes"
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

## Package vignettes

Vignettes in R help others to understand the broader purpose of your package and how to use its functionality to solve real(ish) problems. Creating vignettes is really straightforward with the `usethis` package, and now we will create a really simple one.

1. Call `use_vignette("playing_with_strings")`
  - You'll notice that this creates a folder called `vignettes` and within this folder there should be a Rmarkdown file with the correct name

2. Edit the vignette, including both code and illustrative text to exemplify the use of your package.
