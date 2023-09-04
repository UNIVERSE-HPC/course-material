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
1. You can run another PyBaMM model, with the final results of the saved solution as the initial conditions for the next, by using `model.set_initial_conditions_from(sol2)`, as shown in [this example](https://docs.pybamm.org/en/latest/source/examples/notebooks/initialize-model-with-solution.html)
2. You can do the same post-processing on a solution loaded from disk as you can on a "fresh" solution.

If saving the entire solution would take up too much space, you can use `save_data` to only save the variables you need:

```
sol.save_data(path+"tIV.pkl", ["Time [s]", "Current [A]", "Voltage [V]"])
sol.save_data(path+"tIV.csv", ["Time [s]", "Current [A]", "Voltage [V]"], to_format="csv")
```

Can you think of three situations where you would save the entire solution, and three where you would only save selected variables? To help you think, have a look at this example from my own research:

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
path = "/mnt/c/Users/MyUsername/pybamm_data/"
sol.save_data(
    path+"tIVQ.csv",
    ["Time [s]", "Current [A]", "Voltage [V]", "Discharge capacity [A.h]"],
    to_format="csv
    )
```

What does the above code do? What do you think the intended application was?