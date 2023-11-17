---
name: "Function documentation"
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

## Creating function documentation

Function documentation tells future users of your package (yourself included) what inputs a function expects and the types of output it produces. Thankfully much of this process is entirely automated, reducing the demands on users.

We are now going to create documentation for your recently created `strsplit1` function.

Open again your `R/strsplit1.R` file and place your cursor within the `strsplit1` function body:
  - in your Rstudio window do *Code > Insert roxygen skeleton*, which should create boilerplate code above the function written in the `roxygen` style
  - add a title
  - describe the inputs and the return value
  - include an example

Trigger creation of the documentation via `document()`. This will create a folder `man/` which houses automatically built documentation for your package.

After doing so, test your documentation using `?strsplit1`. Does it appear as you hope it should to a user? If not, adjust the documentation and iteratively execute `document()` and `?strsplit1` until you are happy with it.

## External and internal functions

Notice that when you executed *Code > Insert roxygen skeleton* above, an `@export` was added near the bottom of your function documentation. This means that your `strsplit1` function will be accessible by users of the package; after users execute `library(regexcite)` they can then use your `strsplit1` function.

To see that this is the case, examine the `NAMESPACE` file. You will see that `document()` added *export(strsplit1)* there which is the way that R packages keep tabs on which functions should be exposed to users.

You may also want internal functions in your package, which you don't make available to users. For example, helper functions that are used by other functions in your package but which a user is unlikely to want to use themselves. To achieve this, just remove the `@export` from your function's documentation.


## Test driving your package
Double check that your package is all ok by running `check()`.

Now that we have a package that works, we can install it via `install()`.

Restart your R session and do `library(regexcite)` to load the package. Then test the package using:

```R
 x <- "alfa,bravo,charlie,delta"
 strsplit1(x, split = ",")
```
