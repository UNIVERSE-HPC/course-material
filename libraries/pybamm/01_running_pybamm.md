---
name: Running PyBaMM
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

Before starting this course, you should be comfortable with the basics of Python and have PyBaMM installed in your machine. If you need a refresher about Python you can check the [Intro to Python](https://train.oxrse.uk/material/introductory_courses/python) course. The instructions on how to install PyBaMM are available in the [PyBaMM docs](https://docs.pybamm.org/en/latest/).

PyBaMM (Python Battery Mathematical Modelling) is an open-source battery simulation package written in Python. Our mission is to accelerate battery modelling research by providing open-source tools for multi-institutional, interdisciplinary collaboration. Broadly, PyBaMM consists of

1. a framework for writing and solving systems of differential equations,
2. a library of battery models and parameters, and
3. specialized tools for simulating battery-specific experiments and visualizing the results.

Together, these enable flexible model definitions and fast battery simulations, allowing users to explore the effect of different battery designs and modeling assumptions under a variety of operating scenarios.

## First steps with PyBaMM

The very first thing we need to do is to import PyBaMM by doing

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
sim.plot()
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
output_variables = ["Voltage [V]", ["Electrode current density [A.m-2]", "Electrolyte current density [A.m-2]"]]
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

When running our simulations we have been using the default parameter values, which might not match the battery we want to simulate. PyBaMM has a number of in-built parameter sets (check the list here) that we can use. For example, to use the Chen et al (2020) parameter set we can do

```python
parameter_values = pybamm.ParameterValues("Chen2020")
```

and then, when defining the simulation, we pass the parameter values as a keyword argument

```python
simulation = pybamm.Simulation(model, parameter_values=parameter_values)
```

and solve and plot as usual.

## Running experiments

The default settings are to do a 1C discharge, but we normally want to simulate more complex cycling conditions. To do so, we can pass an experiment to the simulation. An experiment can be defined as a list of strings with the instructions on how to cycle the battery, for example

```python
experiment = [
	"Discharge at 1C for 0.5 hours",
	"Rest for 1 hour",
	"Charge at C/3 until 4.2 V",
	"Hold at 4.2 V until C/20",
]
```

And then pass it to the simulation as

```python
simulation = pybamm.Simulation(model, experiment=experiment)
```

Note that, given the experiment is defined from a list, we can use the operators `+` and `*` to combine and repeat cycles

```python
experiment = [
	"Discharge at 1C for 0.5 hours",
	"Discharge at C/20 for 0.5 hours"
] * 3 + ["Charge at 0.5 C for 45 minutes"]
```