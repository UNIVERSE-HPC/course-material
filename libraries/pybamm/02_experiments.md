---
name: The Experiment class
id: experiments
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

We've already seen how you can run PyBaMM simulations using the current as an input. With the `Experiment` class, however, you can do so much more. The `Experiment` class works by converting text strings into instructions that PyBaMM can use to create a `Simulation` object. Here are some examples:

```
"Discharge at 1C for 0.5 hours",
"Discharge at C/20 for 0.5 hours",
"Charge at 0.5 C for 45 minutes",
"Discharge at 1 A for 0.5 hours",
"Charge at 200 mA for 45 minutes",
"Discharge at 1 W for 0.5 hours",
"Charge at 200 mW for 45 minutes",
"Rest for 10 minutes",
"Hold at 1 V for 20 seconds",
"Charge at 1 C until 4.1 V",
"Hold at 4.1 V until 50 mA",
"Hold at 3V until C/50",
"Discharge at C/3 for 2 hours or until 2.5 V",
```

The input argument for the `Experiment` class is a list [square brackets] of text strings like these. The output is an `Experiment` object that can then be used as an optional argument for the `Simulation` class:

```
import pybamm
model = pybamm.lithium_ion.DFN()
parameter_values = pybamm.ParameterValues("Chen2020")
experiment = pybamm.Experiment(["Discharge at 1C until 2.5 V", "Charge at 0.3C until 4.2 V", "Hold at 4.2 V until C/100"])
simulation = pybamm.Simulation(model, parameter_values=parameter_values, experiment=experiment)
```

If you solve the resulting simulation, the solution will have different cycles, one for each string in the list used to create the experiment.

```
solution = simulation.solve()
import matplotlib.pyplot as plt
fig, axs = plt.subplots(1, 3)
for i in range(3):
    t = solution.cycles[i]["Time (s)"].entries
    V = solution.cycles[i]["Voltage [V]].entries
    axs[i].plot(t, V)
    axs[i].set_xlabel("Time [s]")
    axs[i].set_ylabel("Voltage [V"])
plt.show()
```

You can of course plot the entire solution without specifying a cycle, but this is impractical for long simulations or simulations where the cycle you're interested in takes a very short time compared to everything else.

You may have noticed that the experiment above, with three "cycles", is in fact only one cycle with three steps. PyBaMM allows you to specify this explititly using round brackets:

```
experiment2 = pybamm.Experiment([
    ("Discharge at C/4 until 2.5 V", "Charge at C/4 until 4.2 V", "Hold at 4.2 V until C/100"),
    ("Discharge at C/2 until 2.5 V", "Charge at C/2 until 4.2 V", "Hold at 4.2 V until C/100")
])
simulation2 = pybamm.Simulation(model, parameter_values=parameter_values, experiment=experiment2)
solution2 = simulation2.solve()
fig, axs = plt.subplots(2, 3)
for i in range(2):
    for j in range(3):
        t = solution2.cycles[i].steps[j]["Time [s]"].entries
        V = solution2.cycles[i].steps[j]["Voltage [V]"].entries
        axs[i,j].plot(t, V)
        axs[i,j].set_xlabel("Time [s]")
        axs[i,j].set_ylabel("Voltage [V]")
plt.show()
```

Like any list, you can use arithmetic operators to make more complex experiments:

```
experiment3 = pybamm.Experiment(
    ["Hold at 4.2 V until C/100", "Rest for 4 hours",] +  # Initialize
    [("Discharge at C/10 until 2.5 V", "Charge at C/10 until 4.2 V", "Hold at 4.2 V until C/100")] +  # Capacity check
    [("Discharge at 1C until 2.5 V", "Charge at 0.3C until 4.2 V", "Hold at 4.2 V until C/100",)] * 10 +  # Ageing cycles
    [("Discharge at C/10 until 2.5 V", "Charge at C/10 until 4.2 V", "Hold at 4.2 V until C/100")]  # Capacity check
)
```

::::challenge{id=dot_product title="How many cycles?"}

How many cycles does `experiment3` have, and how many steps in each cycle?

:::solution

There are 14 cycles in total. Each cycle has three steps, except for the first two, which only have one each.

:::

::::

Don't try to run `experiment3` yet. We'll be doing that in Practical Session 2.