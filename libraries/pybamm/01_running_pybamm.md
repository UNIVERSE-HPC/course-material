---
name: Running PyBaMM
dependsOn: []
tags: [pybamm]
learningOutcomes:
  - Be able to make the PyBaMM library available in a Python program
  - Be able to use PyBaMM's built-in lithium-ion battery models
  - Be able to solve a PyBaMM model over a specified time interval
  - Be able to generate a plot of the key variables of a PyBaMM simulation
  - Understand how to query the variables and parameters used in a model
  - Understand how to query the relevant citations for parts of the PyBaMM library
attribution:
  - citation: >
      PyBaMM documentation by the PyBaMM Team
    url: https://docs.pybamm.org
    image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
    license: BSD-3
---

Before starting this course, you should be comfortable with the basics of Python and have PyBaMM installed in your machine. If you need a refresher about Python you can check the [Intro to Python](/material/introductory_courses/python) course. The instructions on how to install PyBaMM are available in the [PyBaMM docs](https://docs.pybamm.org/en/latest/).

PyBaMM (Python Battery Mathematical Modelling) is an open-source battery simulation package written in Python. Our mission is to accelerate battery modelling research by providing open-source tools for multi-institutional, interdisciplinary collaboration. Broadly, PyBaMM consists of

1. a framework for writing and solving systems of differential equations,
2. a library of battery models and parameters, and
3. specialized tools for simulating battery-specific experiments and visualizing the results.

Together, these enable flexible model definitions and fast battery simulations, allowing users to explore the effect of different battery designs and modeling assumptions under a variety of operating scenarios.

## First steps with PyBaMM

The very first thing we need to do is to import PyBaMM

```python
import pybamm
```

This will make all the PyBaMM functionality available in the current script/notebook. As a very first example, we will run the Doyle-Fuller-Newman (DFN) model with all the basic settings. The DFN is one of the many models implemented in PyBaMM, so we can create it by typing

```python
model = pybamm.lithium_ion.DFN()
```

The next step is to create a `pybamm.Simulation` to process the model and make it ready to be solved:

```python
simulation = pybamm.Simulation(model)
```

The simulation can be solved by calling the `solve` method in the simulation object. The method takes an argument which should be the time interval to solve for in seconds (here we do 1 hour, as the default setup is a 1C discharge):

```python
simulation.solve([0, 3600])
```

Now that the simulation has been solved, we can simply call the `plot` method to generate an interactive plot of the key variables:

```python
simulation.plot()
```

## Comparing multiple models

We have seen how to run a simulation of the DFN model, but PyBaMM includes many other standard electrochemical models and makes comparing their outputs very easy. Let’s now compare the DFN model with the Single Particle Model (SPM) and the Single Particle Model with electrolyte (SPMe). We can leverage the functionality of Python lists to do so. We start by creating a list of all the models we wish to solve

```python
models = [
    pybamm.lithium_ion.SPM(),
    pybamm.lithium_ion.SPMe(),
    pybamm.lithium_ion.DFN(),
]
```

so we can now loop over the list, creating and solving the simulations as we go. The solved simulations are stored in the list `simulations`

```python
simulations = []
for model in models:
    simulation = pybamm.Simulation(model)
    simulation.solve([0, 3600])
    simulations.append(simulation)
```

We can now plot the results. Because we want to plot all the simulations in the same plot we cannot use the same syntax as before, instead we can use the `pybamm.dynamic_plot` method, which takes the list of simulations as an input:

```python
pybamm.dynamic_plot(simulations)
```

## Basic plotting

So far we have been using the default plotting settings, but we can choose which variables to plot by passing the keyword argument `output_variables` which should be a list of the plotting variables.

```python
output_variables = ["Voltage [V]", "Current [A]"]
simulation.plot(output_variables=output_variables)
```

the argument should be a list of strings with the names of the variables to plot. We can also plot multiple variables on the same plot by nesting lists.

```python
output_variables = [
    "Voltage [V]",
    ["Electrode current density [A.m-2]", "Electrolyte current density [A.m-2]"]
]
simulation.plot(output_variables=output_variables)
```

To find which variables can be plotted for a given model, we can call the `variable_names` method

```python
model.variable_names()
```

There are a lot of variables, so we can also search the list of variables for a particular string (e.g. “electrolyte”)

```python
model.variables.search("electrolyte")
```

## Changing the parameters

When running our simulations we have been using the default parameter values, which might not match the battery we want to simulate. PyBaMM has a number of in-built parameter sets (check the list [here](https://docs.pybamm.org/en/latest/source/api/parameters/parameter_sets.html)) that we can use. For example, to use the Chen et al (2020) parameter set we can do

```python
parameter_values = pybamm.ParameterValues("Chen2020")
```

and then, when defining the simulation, we pass the parameter values as a keyword argument

```python
simulation = pybamm.Simulation(model, parameter_values=parameter_values)
```

and solve and plot as usual.

## Running experiments

The default settings are to do a 1C discharge, but we normally want to simulate other conditions. To do so, we can pass an experiment to the simulation, for example

```python
simulation = pybamm.Simulation(model, experiment="Discharge at 3C until 3.3 V")
```

## Printing citations

We aim to recognize all contributions by automatically generating citations to the relevant papers on which different parts of the code are built.
These will change depending on what models and solvers you use. Adding the command

```python
pybamm.print_citations()
```

to the end of a script or notebook will print all citations that were used by that piece of code. This will print BibTeX information to the terminal; passing a filename to `print_citations` will print the BibTeX information to the specified file instead.
