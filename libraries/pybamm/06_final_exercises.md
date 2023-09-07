---
name: Final exercises
dependsOn: [
    libraries.pybamm.05_using_submodels,
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
---

To complete the course, here is a list of open-ended exercises to review the content of the course and go into detail into various features. By open-ended we mean that the exercises will often deliberately not specify every single aspect of the model. Some of this additional detail has not been covered in the course, so more information can be found in the [PyBaMM documentation](https://docs.pybamm.org). The exercises are independent from each other so they can be tackled in any order.

## Exercise 1: Coupled degradation mechanisms
We saw in [Lesson 5](./05_using_submodels.md) how to include additional physics to the models. The goal of this exercise is to further explore the degradation submodels, by coupling several of them together. [O'Kane et al (2022)](https://pubs.rsc.org/en/content/articlehtml/2022/cp/d2cp00417h) is a very good reference to read more about coupled degradation mechanisms.

1. Run an electrochemical model of your choice with the SEI growth submodel of your choice too.
2. Add a lithium plating model of your choice to the previous model. You may want to run multiple cycles to see noticeable differences, and you may want to use the SPM model for faster simulations.
3. Now add an `SEI on cracks` submodel. Hint: [this notebook](https://docs.pybamm.org/en/latest/source/examples/notebooks/models/SEI-on-cracks.html) can be useful to understand better SEI on cracks models.
4. Compare the previous three models together. What differences do you observe?
5. Use the `plot_summary_variables` function and compate the summary degradation variables for the various degradation models. What do you observe? Tip: check the docs for more information about this function.

## Exercise 2: Long experiments
In [lesson 2](./02_experiments.md) we saw how to run experiments, that is how to change the cycling conditions between constant current, constant voltage, drive cycles... The goal of this exercise is to explore how to run very long experiments (i.e. experiments with hundreds or even thousands of cycles).

1. Run an experiment consisting of 10 cycles of 1C discharge, rest, a C/3 4.2 V CCCV charge and another rest.
2. Now try running the same experiment but for 500 cycles. Which issues are you encountering? Tip: you might want to use a simple model (e.g. SPM) to get faster simulations.
3. Use the `plot_summary_variables` function to plot the summary degradation variables. Tip: check the docs for more information about this function.
4. Create two different models with different ageing mechanisms, run the long experiment for both and plot the results. Which one ages faster? What are the main contributions to the ageing?
5. In the previous tasks you might have encountered some memory issues. Use the `save_at_cycles` option in the `Simulation.solve` method to not save all the cycles and speed up the computations.
6. Plot both the standard and summary variables for this new solution. What do you observe?

## Exercise 3: Half-cell models
A lithium counter-electrode can be used instead of a porous electrode, either to make a lithium-metal battery or to test the properties of an electrode in the laboratory. PyBaMM models can be defined for half-cells, by passing the option `"working electrode": "positive"`.

1. Simulate the DFN model for a half-cell. What differences do you observe with the standard DFN model?
2. Compare the SPM, SPMe and DFN models for half-cell.
3. Now use the `Chen2020` parameter set to simulate the half-cell model. Why does it fail?
4. Find the default parameter value used by half-cell models, you can access them by calling the `default_parameter_values` attribute of the model.
5. Using the parameter values found in the previous question, modify the `Chen2020` parameter set to work with the half-cell models.

## Exercise 4: Batch study and sensitivity analysis
One of the first examples we saw in [lesson 1](./01_running_pybamm.md) was how to compare various PyBaMM models. The goal of this exercise is to explore how to compare various simulations and perform sensitivity analysis.
1. Compare the SPM, SPMe and DFN models as explained in [lesson 1](./01_running_pybamm.md).
2. PyBaMM has the `BatchStudy` function that allows to streamline comparisons. Repeat the comparison above but using `BatchStudy`. Tip: check the docs for more information about this function.
3. Compare the three models for two parameter sets of your choice. Tip: you may want to check what the `permutations` option does.
4. Perform a parameter sweep for one parameter and one model of your choice.
5. TODO: SOMETHING ON SENSITIVITIES