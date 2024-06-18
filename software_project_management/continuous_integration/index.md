---
name: Continuous Integration
id: continuous_integration
dependsOn: [
    software_project_management.collaboration
]
files: [
    github_actions.md,
    code_coverage.md,
    documentation.md
]
summary: |
    This course introduces the concept of continuous integration and how to set it up for a Python project using GitHub Actions.
attribution: 
    - citation: This material has been adapted from the "Software Engineering" module of the SABS R³ Center for Doctoral Training.
      url: https://www.sabsr3.ox.ac.uk
      image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
      license: CC-BY-4.0
    - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1 
      url: https://www.universe-hpc.ac.uk
      image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
      license: CC-BY-4.0


---

## What is Continuous Integration?

Whenever we write software, we want to be ceratin that the changes we make are not introducing unexpected problems.
We should always write tests to verify new functionality works as we intend, but there are plenty of pitfalls:

1. Does the change work on different operating systems?
1. What about with different software versions, including versions of libraries, frameworks, and dependencies?
1. What about on different hardware?
1. Does it unexpectedly rely on specific user data on your development machine?
1. Is the change compatible with changes other developers are making?

We need an automated way to address these pitfalls in a quick and consistent way.
In a nutshell, this is Continuous Integration (CI).

CI is a software development practice where developers integrate code into a shared repository frequently.
Each time this happens, the software is automatically verified to detect integration errors as quickly as possible.
The primary goal of CI is to catch and address bugs, integration problems, and other issues early, when they are easier to fix.

Here are some key princples of CI:

1. **Maintain a single source repository:** All code and dependencies should be versioned and stored in a single, shared, and reliable repository. This is the primary source of all truth for the project, and everyone has access to it.

1. **Automate the build:** The process of building the project — compiling the source code into executable code, packaging that code, and creating installers — should be automated.

1. **Make your build self-testing:** Once the code is built, automated tests should run to check that the software behaves as expected. This can range from unit tests (which test individual components in isolation) to integration tests (which test how components work together) to testing code coverage.

1. **Everyone commits to the maineline frequently:** Developers should integrate their work frequently. This prevents integration conflicts from becoming too complex and difficult to resolve.

1. **Automated builds should happen on an integration machine:** Updated code should be built and tested on a separate integration machine, not a developer's local machine. This ensures that the build process works in a clean, standardized environment.

1. **Keep the build fast:** The build and test process should be fast to give quick feedback. If the process is slow, developers will be tempted to commit less frequently.

1. **Everyone can see what's happening:** All results from building and testing (like output, logs, test results) should be available to all team members. This increases transparency and makes it easier to spot and fix issues.

1. **Automate deployment**: The process of deploying the software should be automated, ensuring it's easily reproducible and reducing the chances of human error. This might include creating Python wheels and deploying them to [PyPI](https://pypi.org/), or building public documentation pages.

## How do we do it?

There are many CI infrastructures and services, free and paid for, and subject to change as they evolve their features.

In this course you will be using [GitHub Actions](https://github.com/features/actions), which unsurprisingly is available as part of GitHub.

There are other free options, for instance [Travis CI](https://travis-ci.com/) and [AppVeyor](https://www.appveyor.com/).
All three of these make use of common features across many CI implementations, and you are certainly advised to look at the options to see some of the commonalities and differences in how features are typically provided.

## This course

In this course we aim to walk you through a hands-on session which will set up CI for a small Python project, and see some of its benefits in action.
We will go through:

1. An introduction to GitHub actions ([link](./continuous_integration/github_actions))
1. Generating code coverage information ([link](./continuous_integration/code_coverage))
1. Generating and deploying documenation ([link](./continuous_integration/documentation))

## Getting started

Go to [https://github.com/OxfordRSE/ci_course_start](https://github.com/OxfordRSE/ci_course_start) and press "Use this template" -> "Create a new repository".

This will set up a repository in your own GitHub account with the same files as the template.

You can name it anything you like.
Clone your new repository, and you're ready for the next lesson.
