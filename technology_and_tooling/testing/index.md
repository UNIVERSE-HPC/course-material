---
name: Testing
id: testing
dependsOn: [technology_and_tooling.ide, software_architecture_and_design.procedural]
files: [automated_testing.md, diagnosing_issues.md, scaling_up.md, testable_code_fixtures.md, mocking.md]
learningOutcomes:
  - Understand different types of tests and the advantages of automated testing.
  - Make use of the Pytest testing framework.
  - Scale up testing using parameterisation and fixtures.
  - Use the debugger in VSCode and improve code quality using linting tools.
  - Learn good practices for writing testable code.
  - Be able to use mocking to mimic the use of external libraries and resources.
attribution:
  - citation: >
      "Aleksandra Nenadic, Steve Crouch, James Graham, et al. (2022). carpentries-incubator/python-intermediate-development: beta (beta). Zenodo. https://doi.org/10.5281/zenodo.6532057"
    url: https://doi.org/10.5281/zenodo.6532057
    image: https://carpentries-incubator.org/assets/img/carpentries-incubator.svg
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0

summary: |
  This course introduces the basics of automated testing and debugging in Python, using the Pytest framework.
---

In this section, we'll look at testing approaches that can help us ensure that the software we write is behaving as intended, and how we can diagnose and fix issues once faults are found. Using such approaches requires us to change our practice of development. This can take time, but potentially saves us considerable time in the medium to long term by allowing us to more comprehensively and rapidly find such faults, as well as giving us greater confidence in the correctness of our code - so we should try and employ such practices early on. We will also make use of techniques and infrastructure that allow us to do this in a scalable, automated and more performant way as our codebase grows.

In this section we will:

- Make use of a **test framework** called Pytest, a free and open source Python library to help us structure and run automated tests.
- Design, write and run **unit tests** using Pytest to verify the correct behaviour of code and identify faults, making use of test **parameterisation** to increase the number of different test cases we can run.
- Use VsCodes's integrated **debugger** to help us locate a fault in our code while it is running, and then fix it.
- Explore more advanced testing topics such as writing **testable code**, using **fixtures** and implementing **mocking**.
