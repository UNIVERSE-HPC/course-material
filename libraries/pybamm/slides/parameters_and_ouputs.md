---
title: Using Parameters and Outputs
---

## Outline

- Parameters:
  - Using Parameters Sets
  - Changing Parameters
  - Input Parameters
- Outputs:
  - Variables
  - Processed Variables
  - Plots
  - Saving and Loading

## Parameter Sets

- PyBaMM comes with a number of parameter sets that have been published in the
  literature.
- To see the available parameter sets and more information on each one:

~~~python
import pybamm
print(list(pybamm.parameter_sets))
# ['Ai2020', 'Chen2020', ...]
print(pybamm.parameter_sets.get_docstring("Ai2020"))
# Parameters for the Enertech cell (Ai2020), from the papers :footcite:t:`Ai2019`,
# :footcite:t:`rieger2016new` and references therein. ...
~~~

- To create a parameter set, use the `pybamm.ParameterValues` class:

~~~python
parameter_values = pybamm.ParameterValues("Marquis2019")
~~~

## Updating Parameters

- The `pybamm.ParameterValues` class is a dictionary of parameters and their values.
- To see the available parameters:

~~~python
list(parameter_values.keys())
['Negative electrode thickness [m]', 'Positive electrode thickness [m]', ...]
~~~

- To set the value of a parameter:

~~~python
parameter_values["Negative electrode thickness [m]"] = 1e-5
~~~

## Background - The PyBaMM Pipeline

- To understand how parameters are used in PyBaMM, we need to understand the PyBaMM
  pipeline.
- Most of these steps are done automatically by the `pybamm.Simulation` class, but it is
  useful to know what is going on under the hood.

### The PyBaMM Pipeline (cont.)

The pipeline is a series of steps that PyBaMM goes through to solve a model:

1. Define the model. The equations are continuous (i.e. not discretised in space or
   time)
  and the parameters are symbolic (i.e. not yet given a value)

~~~python
model = pybamm.lithium_ion.DFN()
~~~

2. Replace the symbolic parameters with values

~~~python
parameter_values = pybamm.ParameterValues("Marquis2019")
param.process_model(model)
param.process_geometry(geometry)
~~~

### The PyBaMM Pipeline (cont.)

3. Discretise the equations in space and time

~~~python
mesh = pybamm.Mesh(geometry, model.default_submesh_types, model.default_var_pts)
disc = pybamm.Discretisation(mesh, model.default_spatial_methods)
disc.process_model(model)
~~~

4. Solve the discretised model

~~~python
solver = pybamm.CasadiSolver(mode="fast")
solution = solver.solve(model, t_eval)
~~~

For more detail see the
[docs](https://docs.pybamm.org/en/latest/source/examples/notebooks/expression_tree/expression-tree.html)

## Updating Parameters in the Pipeline

- To update the parameters in a model, we need to repeat steps 2-4 of the pipeline (i.e.
  nearly all of it!).
- This overhead can be significant (e.g. paramter sweeps, optimisation).
- To avoid this, PyBaMM has the concept of an **input parameter**.

## Input Parameters

- An input parameter is a parameter that does not yet have a value, but one will be
  provided when the model is solved.

- An input parameter can be created by setting its value to the string `[input]`, like so:

~~~python
parameter_values = pybamm.ParameterValues("Marquis2019")
parameter_values.update({
    "Current function [A]": "[input]",
})
~~~

- This keeps the parameter symbolic through the pipeline, until the solve step, when the
  user provides a value:

~~~python
solution = simulation.solve([0, 3600], inputs={"Current function [A]": 2})
~~~

## Questions?
