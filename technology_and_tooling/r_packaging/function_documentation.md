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

## Creating documentation

Function documentation tells future users of your package (yourself included) what inputs a function expects and the types of output it produces. Thankfully much of this process is entirely automated, reducing the demands on users.

We are going to create documentation for your recently created `strsplit1` function.

1. Create documentation for your `strsplit1` function:

   * open again your `R/strsplit1.R` file and place your cursor within the `strsplit1` function body

   * do *Code > Insert roxygen skeleton*, which should create boilerplate code above the function

   * add a title

   * describe the inputs and the return value

   * include an example

2. Trigger creation of the documentation via `document()`

   * this will create a folder `man/` which houses automatically built documentation for your package

3. Test your documentation using `?strsplit1`

4. Examine the `NAMESPACE` file. You will see that `document()` added *export(strsplit1)* there

   * this makes the function available to users of the package

   * note, you may want internal functions in your package which you don't make available to users

5. Double check that your package is all ok via `check()`

6. Now that we have a package that works, we can install it via `install()`

7. Restart your R session and do `library(regexcite)` to load the package

8. Test the package using:

   ```R
   x <- "alfa,bravo,charlie,delta"
   strsplit1(x, split = ",")
   ```
