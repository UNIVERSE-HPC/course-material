---
name: Making the most of PyBaMM outputs
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

There is a large overlap between this exercise and [PyBaMM Tutorial notebook 6](https://docs.pybamm.org/en/latest/source/examples/notebooks/getting_started/tutorial-6-managing-simulation-outputs.html), so we recommended you read both.

After solving a simulation, there are two says of accessing the solution:

```
sol = sim.solve()
```

when calling the `solve` command and

```
sol = sim.solution
```

if `sim` has already been solved. There are many things you can do with PyBaMM solutions. For example, you can save the entire solution to a `.pkl` file and load it later.

```
path = "/mnt/c/Users/YourUsername/"
sol.save(path+"my_pybamm_solution.pkl")
```

If you're a Windows Subsystem for Linux (WSL) user like me, you can read and write to your Windows drives (instead of the Linux partition) by putting `/mnt/` before the letter denoting the drive. Anyway...

```
sol2 = pybamm.load(path+"my_pybamm_solution.pkl")
```

PyBaMM has a lot of variables, so these `.pkl` files are huge! So why bother?
* You can run another PyBaMM model, with the final results of the saved solution as the initial conditions for the next, by using `model.set_initial_conditions_from(sol2)`, as shown in [this example](https://docs.pybamm.org/en/latest/source/examples/notebooks/initialize-model-with-solution.html)
* You can do the same post-processing on a solution loaded from disk as you can on a "fresh" solution.

If saving the entire solution would take up too much space, you can use `save_data` to only save the variables you need:

```
sol.save_data(path+"tIV.pkl", ["Time [s]", "Current [A]", "Voltage [V]"])
sol.save_data(path+"tIV.csv", ["Time [s]", "Current [A]", "Voltage [V]"], to_format="csv")
```

::::challenge{id=dot_product title="When to save what?"}

Can you think of three situations where you would save the entire solution, and three where you would only save selected variables?

:::solution

Save the entire solution if:
* You want to do post-processing on the solution later
* You want to run more simulations using the first as an initial condition
* You plan to submit the full `.pkl` file to an archive
Save only selected variables if:
* The full solution would take up too much space or take too long to upload
* You want to share the data with non-PyBaMM users
* You want the data to be read by other software

:::

::::

To help you think, have a look at this example from my own research:

```
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
    path+"tIVQ.csv",
    ["Time [s]", "Current [A]", "Voltage [V]", "Discharge capacity [A.h]"],
    to_format="csv
    )
```

::::challenge{id=dot_product title="What does this code do?"}

What does the above code do? What do you think the intended application was?

:::solution

This code simulates a GITT experiment. By exporting the parameters into a `.csv` file, you can use the simulated data to parameterize an equivalent circuit network in the same way as experimental GITT data. 

:::

::::