---
name: The Experiment class
id: experiments
dependsOn: [libraries.pybamm.01_running_pybamm]
tags: [pybamm]
attribution:
  - citation: >
      PyBaMM documentation by the PyBaMM Team
    url: https://docs.pybamm.org
    image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
    license: BSD-3
---

We already saw in [lesson 1](./01_running_pybamm) a very basic use of experiments in which we changed the discharge rate. With the `Experiment` class, however, you can do so much more. The `Experiment` class works by converting text strings into instructions that PyBaMM can use to create a `Simulation` object. Here are some examples:

```text
"Discharge at 1C for 0.5 hours",
"Discharge at C/20 for 0.5 hours",
"Charge at 0.5 C for 45 minutes",
"Discharge at 1 A for 0.5 hours",
"Charge at 200 mA for 45 minutes",
"Discharge at 1 W for 0.5 hours",
"Charge at 200 mW for 45 minutes",
"Rest for 10 minutes",
"Hold at 1 V for 20 seconds",
"Charge at 1 C until 4.1 V",+
"Hold at 4.1 V until 50 mA",
"Hold at 3V until C/50",
"Discharge at C/3 for 2 hours or until 2.5 V",
```

The input argument for the `Experiment` class is a string, or a list [square brackets] of strings like these. The output is an `Experiment` object that can then be used as an optional argument for the `Simulation` class:

```python
import pybamm
experiment = pybamm.Experiment([
    "Discharge at 1C until 3.3 V",
    "Charge at 0.3C until 4.0 V",
    "Hold at 4.0 V until C/100",
])
model = pybamm.lithium_ion.DFN()
simulation = pybamm.Simulation(model, experiment=experiment)
solution = simulation.solve()
```

If you solve the resulting simulation, the solution will have different cycles, one for each string in the list used to create the experiment. You can access them via the `cycles` attribute of the solution, and plot them as usual

```python
solution.cycles[0].plot()
```

You can of course plot the entire solution without specifying a cycle, but this is impractical for long simulations or simulations where the cycle you're interested in takes a very short time compared to everything else.

You may have noticed that the experiment above, with three "cycles", is in fact only one cycle with three steps. PyBaMM allows you to explicitly specify what is a cycle using round brackets:

```python
experiment2 = pybamm.Experiment([
    (
        "Discharge at C/4 until 2.5 V",
        "Charge at C/4 until 4.2 V",
        "Hold at 4.2 V until C/100"
    ),
    (
        "Discharge at C/2 until 2.5 V",
        "Charge at C/2 until 4.2 V",
        "Hold at 4.2 V until C/100"
    )
])
simulation2 = pybamm.Simulation(model, experiment=experiment2)
```

You can access a given step by accessing the `steps` attribute of the `cycles` (i.e. `solution.cycles[i].steps[j]`), and plot as usual

```python
solution.cycles[0].steps[1].plot()
```

Like any list, you can use the arithmetic operators `+` and `*` to make more complex experiments:

```python
experiment3 = pybamm.Experiment(
    # Initialize
    [
        "Hold at 4.2 V until C/100",
        "Rest for 4 hours",
    ]
    # Capacity check
    + [(
        "Discharge at C/10 until 2.5 V",
        "Charge at C/10 until 4.2 V",
        "Hold at 4.2 V until C/100"
    )]
    # Ageing cycles
    + [(
        "Discharge at 1C until 2.5 V",
        "Charge at 0.3C until 4.2 V",
        "Hold at 4.2 V until C/100",
    )] * 10
    # Capacity check
    + [(
        "Discharge at C/10 until 2.5 V",
        "Charge at C/10 until 4.2 V",
        "Hold at 4.2 V until C/100"
    )]
)
```

::::challenge{id=dot_product title="How many cycles?"}

How many cycles does `experiment3` have, and how many steps in each cycle?

:::solution

There are 14 cycles in total. Each cycle has three steps, except for the first two, which only have one each.

:::

::::

Don't try to run `experiment3` yet. We'll be doing that in the next session.
