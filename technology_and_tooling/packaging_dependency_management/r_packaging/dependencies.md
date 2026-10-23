---
name: "Package dependencies"
teaching: 15
dependsOn: [
  technology_and_tooling.r_packaging.testing
]
tags: [rpackaging]
attribution:
    - citation: >
        Wickham, Hadley, and Jennifer Bryan. R packages. " O'Reilly Media, Inc.", 2023.
      url: https://r-pkgs.org/
      license:  CC BY-NC-ND 4.0
---

## Package dependencies

Suppose we want to use the `tibble` function from `dplyr` to store our split string in a dataframe with a column 'order' which specifies the position of each element in the string and another column 'string' which contains the splitted string. To do this, we will create a function called  `string_df(x, split)` that internally calls `tibble`. To do so, we will need to make the `dplyr` function a dependency of our package.

Call `use_package("dplyr")`. Examine the `DESCRIPTION` file to see that this file has been added under the "Imports" section.

Call `use_r("string_df")` to create a blank `.R` file. Insert the following into the file:

```
string_df <- function(x, split) {
  x <- strsplit1(x, split)
  dplyr::tibble(
    order=seq_along(x),
    string=x
  )
}
```

Note that we have used `dplyr::tibble` rather than `tibble` to indicate that we are wanting to access a function that is amongst the exported variables of the `dplyr` package. In practice, when developing an R package, it should be your default behaviour to use `::` to access functions belonging to a specific package. This adds clarity to your code and helps you (and anyone else developing a package after you) to keep track of exactly which functions are being used from which packages.

Create documentation for the above function.

Call `use_test()` from within the R file to create a test file, and add a unit test for `string_df`.

Check that all is ok with the package (and the tests) by running `check()`.

Try installing your package and running its functionality in a fresh R session.
