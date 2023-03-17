---
name: GitHub Actions
dependsOn: [
]
tags: [github]
---

With a GitHub repository there's a very easy way to set up CI that runs when your 
repository changes: simply add a file to your repository in the directory 
`.github/workflows`.
Each file in this directory will, when triggered, spin up a virtual machine and run the 
sequence of commands in the file.

Information about the specifications of these VMs can be found 
[here](https://docs.github.com/en/free-pro-team@latest/actions/reference/specifications-for-github-hosted-runners).
At the time of writing, each VM will have a 2-core CPU, 7GB of RAM and 14 GB of SSD 
space available, and each workflow can run for up to 6 hours.
These resources are all free for public repositories, and for private repositories you 
have a monthly quota of VM-minutes before any payment is required.

In this section you will create two workflows by using the wizard and built-in editor on 
the GitHub website.

<iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/O-91cuGP24U" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

1. Create a basic workflow from your repository's "Actions" tab, and verify that it runs and executes correctly.
1. Create a workflow based on the default "Python package" workflow, editing the contents as required for this project:
   - Remove the unnecessary dependency of `pytest`
   - Change `flake8` to __not__ treat errors as warnings
   - Run the tests with `python -m unittest`, rather than `pytest`
1. Check that the four runners start, and watch them fail.
1. On your machine, fix the errors that `flake8` found, and commit and push those changes. Make sure the CI now passes.
   - Remember to pull the changes you made on GitHub before commiting your fixes
1. Read the contents of the `.flake8` file in your repository. What is it doing?
1. \[optional\] read more about [GitHub's hosted runners](https://docs.github.com/en/free-pro-team@latest/actions/reference/specifications-for-github-hosted-runners).
1. \[optional\] read more about the [syntax for GitHub Actions](https://docs.github.com/en/free-pro-team@latest/actions/reference/workflow-syntax-for-github-actions).
1. \[optional, after completing exercises\] set up a similar workflow using [Travis CI](https://travis-ci.com/).