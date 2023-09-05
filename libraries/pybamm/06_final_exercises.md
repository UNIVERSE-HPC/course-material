---
name: Final exercises
dependsOn: [
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://github.com/pybamm-team/pybamm.org/blob/main/static/images/pybamm_logo.svg
      license: BSD-3
---

To complete the course, here is a list of open-ended exercises to review the content of the course and go into detail into various features. By open-ended we mean that the exercises will often deliberately not specify every single aspect of the model. Some of this additional detail has not been covered in the course, so more information can be found in the [PyBaMM documentation](https://docs.pybamm.org). The exercises are independent from each other so they can be tackled in any order.

## Exercise 1: Coupled degradation mechanisms

## Exercise 2: Long experiments
In [CROSSREF]() we saw how to run experiments, that is how to change the cycling conditions between constant current, constant voltage, drive cycles... The goal of this exercise is to explore how to run very long experiments (i.e. experiments with hundreds or even thousands of cycles).

1. Run an experiment consisting of 10 cycles of 1C discharge, rest, a C/3 4.2 V CCCV charge and another rest.
2. Now try running the same experiment but for 500 cycles. Which issues are you encountering? Tip: you might want to use a simple model (e.g. SPM) to get faster simulations.
3. Use the `plot_summary_variables` function to plot the summary degradation variables. Tip: check the docs for more information about this function.
4. Create two different models with different ageing mechanisms, run the long experiment for both and plot the results. Which one ages faster? What are the main contributions to the ageing?
5. In the previous tasks you might have encountered some memory issues. Use the `save_at_cycles` option in the `Simulation.solve` method to not save all the cycles and speed up the computations.
6. Plot both the standard and summary variables for this new solution. What do you observe?

## Exercise 3: Half-cell models

## Exercise 4: Multi-particle models

## Exercise 5: Batch study and sensitivity analysis
One of the first examples we saw in [CROSSREF]() was how to compare various PyBaMM models. The goal of this exercise is to explore how to compare various simulations and perform sensitivity analysis.
1. Compare the SPM, SPMe and DFN models as explained in [CROSSREF]().
2. PyBaMM has the `BatchStudy` function that allows to streamline comparisons. Repeat the comparison above but using `BatchStudy`. Tip: check the docs for more information about this function.
3. Compare the three models for two parameter sets of your choice. Tip: you may want to check what the `permutations` option does.
4. Perform a parameter sweep for one parameter and one model of your choice.
5. TODO: SOMETHING ON SENSITIVITIES