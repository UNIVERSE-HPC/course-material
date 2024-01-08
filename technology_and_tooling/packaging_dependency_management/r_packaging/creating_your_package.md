---
name: "Creating your package"
teaching: 15
exercises: 15
dependsOn: [
	technology_and_tooling.r_packaging.basics
]
tags: [rpackaging]
attribution:
    - citation: >
        Wickham, Hadley, and Jennifer Bryan. R packages. " O'Reilly Media, Inc.", 2023.
      url: https://r-pkgs.org/
      license:  CC BY-NC-ND 4.0
---

## Create package outline

We are now going to create the skeleton of our package within your newly created directory. Fortunately, much of this process is automated, minimising the effort required by you!

Execute `create_package("regexcite")` in the R console which will initialise a package called "regexcite" in your directory:

  - this will likely open up a new RStudio session so you may need to call `library(devtools)` again to load this package
  - notice that there now exists various files in the directory:
	  - `DESCRIPTION` which is an editable file containing package metadata, which we will later edit.
	  - `.Rbuildignore` lists files that we need to have around but that should not be included when building the R package from source.
	  - `NAMESPACE` declares the functions your package exports for external use and the external functions your package imports from other packages. Generally, this should not be edited by hand.
	  - the `R/` directory is the “business end” of your package. It will soon contain `.R` files with function definitions.

Optionally, also make our package also a Git repository, with `use_git()`.

## Create basic function
Create `x <- "alfa,bravo,charlie,delta"`. Suppose we want to write a function that splits this into a vector of individual words:

  - this can be done with `unlist(strsplit(x, split=","))` but we'd like to create a function to do so.

We now want to write a function we are going to call `strsplit1` which takes as input a string with a given separator and splits it into a vector of strings:

  - call `use_r("strsplit1")` which should create an `strsplit1.R` file within the `R/` folder.
  - write your `strsplit1` function in that file:

```R
strsplit1 <- function(x, split) {
	unlist(strsplit(x, split = split))
}
```

Test drive your function by calling `load_all()` which should make the function available for you to play with

  - when developing an R package, we don't manually instantiate functions typically.
  - `load_all` provides a more robust way to test functions as it simulates what a user would experience by loading the package.

Now check that the package as a whole works by calling `check()`
  - this runs `R CMD check`, which is executed in the shell and is the gold standard for checking that an R package is in full working order

`check` should have raised a warning about `Non-standard license specification` and we will soon address this

## Add descriptive info for your package

Open up the `DESCRIPTION` file:

  - make yourself the author; if you don’t have an ORCID, you can omit the `comment = ...` portion
  - add some descriptive text to the `Title` and `Description` fields

Add an MIT license (see [course x](xx) for an intro to software licenses) via `use_mit_license()`

  - this adds two license files to the folder:
     - `LICENSE` which contains the year and copyright owners
     - `LICENSE.md` which holds the full license info
