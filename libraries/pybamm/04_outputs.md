---
name: Making the most of PyBaMM outputs
dependsOn: [libraries.pybamm.03_parameter_values]
tags: [pybamm]
attribution:
  - citation: >
      PyBaMM documentation by the PyBaMM Team
    url: https://docs.pybamm.org
    image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
    license: BSD-3
---

There is a large overlap between this exercise and [PyBaMM Tutorial notebook 6](https://docs.pybamm.org/en/latest/source/examples/notebooks/getting_started/tutorial-6-managing-simulation-outputs.html), so we recommended you read both.

## Getting a solution object

After solving a simulation, there are two says of accessing the solution:

```python
import pybamm
sim = pybamm.Simulation()
sol = sim.solve()
```

when calling the `solve` command and

```python
sol = sim.solution
```

## Accessing solution variables

if `sim` has already been solved. There are many things you can do with PyBaMM solutions. A common one is to plot the solution variables, which we saw in [Lesson 1](./01_running_pybamm).

You might also want to access the solution variables directly. For example, to get the terminal voltage at the end of the simulation, you can use:

```python
sol["Terminal voltage [V]"]
```

This returns a `pybamm.ProcessedVariable`, which can be used to calculate the value of that variable at an array of times:

```python
import numpy as np
times = np.linespace(0, 3600, 100)
terminal_voltage = sol["Terminal voltage [V]"](times)
```

Here we use numpy to create an array of 100 times between 0 and 3600 seconds,
and then use the `ProcessedVariable` to calculate the terminal voltage at those
times. We can then directly plot the terminal voltage against time using
matplotlib

```python
import matplotlib.pyplot as plt
plt.plot(times, terminal_voltage)
plt.xlabel("Time [s]")
plt.ylabel("Terminal voltage [V]")
plt.show()
```

The solution object contains the entire solution of your simulation which is calculated a set of timepoint chosen by the solver. You can access the timepoints using

```python
sol.t
```

It is often useful to know these times, as you don't have to guess the
resolution needed to plot a smooth or complete solution (e.g. if the solution is
changing rapidly in one time period, and slowly in another). If you want to get
the associated values of a variable at these times, you can use the `entries`
attribute of the `ProcessedVariable`:

```python
terminal_voltage = sol["Terminal voltage [V]"].entries
```

::::challenge{id=solution_times title="Solution times"}

Run a simulation using an experiment of your choice. Using matplotlib, plot a
variable of your choice against the times used by the solver. Add points to the
plot to show where the solver has calculated the solution.

:::solution

```python
model = pybamm.lithium_ion.SPM()
parameter_values = pybamm.ParameterValues("Marquis2019")
parameter_values.update({
    "Current function [A]": "[input]",
})
experiment = pybamm.Experiment([
    "Discharge at 1C until 3.3 V",
    "Charge at 0.3C until 4.0 V",
    "Hold at 4.0 V until C/100",
])
simulation = pybamm.Simulation(
    model,
    parameter_values=parameter_values,
    experiment=experiment,
)

solution = simulation.solve(inputs={"Current function [A]": 1})
times = solution.t
terminal_voltage = solution["Terminal voltage [V]"].entries
plt.plot(times, terminal_voltage, '.-')
plt.ylabel("Terminal voltage [V]")
plt.xlabel("Time [s]")
plt.show()
```

:::
::::

## Saving solutions

It is often useful to save the solution to disk, so you can load it later on for
processing or plotting. For example, you can save the entire solution to a
`.pkl` file and load it later.

```python
path = "/mnt/c/Users/YourUsername/"
sol.save(path + "my_pybamm_solution.pkl")
```

If you're a Windows Subsystem for Linux (WSL) user like me, you can read and write to your Windows drives (instead of the Linux partition) by putting `/mnt/` before the letter denoting the drive. Anyway...

```python
sol2 = pybamm.load(path + "my_pybamm_solution.pkl")
```

PyBaMM has a lot of variables, so these `.pkl` files are huge! So why bother?

- You can run another PyBaMM model, with the final results of the saved solution as the initial conditions for the next, by using `model.set_initial_conditions_from(sol2)`, as shown in [this example](https://docs.pybamm.org/en/latest/source/examples/notebooks/initialize-model-with-solution.html)
- You can do the same post-processing on a solution loaded from disk as you can on a "fresh" solution.

If saving the entire solution would take up too much space, you can use `save_data` to only save the variables you need:

```python
sol.save_data(
    path + "tIV.pkl", ["Time [s]", "Current [A]", "Voltage [V]"]
)
sol.save_data(
    path + "tIV.csv", ["Time [s]", "Current [A]", "Voltage [V]"], to_format="csv"
)
```

::::challenge{id=dot_product title="When to save what?"}

Can you think of three situations where you would save the entire solution, and three where you would only save selected variables?

:::solution

There is no right answer to this question, but some examples are the following.

When to save entire solution?

- If you might want to do additional post-processing later.
- If you're likely to need the solution as an initial condition for anoher simulation.
- If you're submitting the data to an archive.

When to save selected data?

- If the full `.pkl` file would take up too much space or take too long to upload.
- To feed the data to another software package.
- To share the data with non-PyBaMM users.

:::

::::

To help you think, have a look at this example from my own research:

```python
model = pybamm.lithium_ion.DFN()
param = pybamm.ParameterValues("ORegan2022")
exp = pybamm.Experiment(
    [
        "Rest for 60 seconds (0.1 second period)",
        "Discharge at 5 A for 144 seconds or until 2.5 V (0.1 second period)",
        "Rest for 1 hour (1 second period),"
    ] * 25
)
sim = pybamm.Simulation(model, parameter_values=param, experiment=exp)
sol = sim.solve()
path = "/mnt/c/Users/sokane/pybamm_data/"
sol.save_data(
    path + "tIVQ.csv",
    ["Time [s]", "Current [A]", "Voltage [V]", "Discharge capacity [A.h]"],
    to_format="csv"
)
```

::::challenge{id=dot_product title="What does this code do?"}

What does the above code do? What do you think the intended application was?

:::solution

This code simulates a GITT experiment. By exporting the parameters into a `.csv` file, you can use the simulated data to parameterize an equivalent circuit network in the same way as experimental GITT data.

:::

::::
