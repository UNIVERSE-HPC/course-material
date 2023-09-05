---
name: Parameter sets
dependsOn: [
    experiments
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://github.com/pybamm-team/pybamm.org/blob/main/static/images/pybamm_logo.svg
      license: BSD-3
---

PyBaMM comes with 12 ready-made parameter sets for lithium-ion batteries. To select one, pass the name of one of them as an input argument to the `ParameterValues` class. For now, select `Marquis2019`, which is the default parameter set for lithium-ion models.

```
import pybamm
parameter_values = pybamm.ParameterValues("Marquis2019")
```

There are over 100 parameter values! Fortunately, `ParameterValues` objects are dictionaries so you can look up the parameters you're interested in:

```
print(parameter_values["Upper voltage cut-off [V]"])
print(parameter_values["Lower voltage cut-off [V]"])
```

Recall `experiment3` from the morning session:

```
experiment3 = pybamm.Experiment(
    ["Hold at 4.2 V until C/100", "Rest for 4 hours",] +  # Initialize
    [("Discharge at C/10 until 2.5 V", "Charge at C/10 until 4.2 V", "Hold at 4.2 V until C/100")] +  # Capacity check
    [("Discharge at 1C until 2.5 V", "Charge at 0.3C until 4.2 V", "Hold at 4.2 V until C/100",)] * 10 +  # Ageing cycles
    [("Discharge at C/10 until 2.5 V", "Charge at C/10 until 4.2 V", "Hold at 4.2 V until C/100")]  # Capacity check
)
```

The above `experiment3` will not work with the default parameters, because it was designed for a different cell. Marquis et al. studied the Kokam SLPB78205130H 16 Ah prismatic cell, whereas `experiment3` is designed for the LG M50 5 Ah cylindrical cell. Four of PyBaMM's built-in parameter sets correspond to the LG M50:
* `Chen2020` comes from the first study, published in 2020.
* `Chen2020_composite` is an upgrade of `Chen2020` designed to work with PyBaMM's composite electrode model
* `OKane2022` is a superset of `Chen2020` designed to work with PyBaMM's various degradation models
* `ORegan2022` comes from a follow-up paper to `Chen2020` that addressed some questions the first paper didn't answer

Like `Experiment` objects, `ParameterValues` objects are an optional argument to the `Simulation` class:

```
simulation3 = pybamm.Simulation(model, experiment=experiment3, parameter_values=parameter_values)
```

With updated parameter values, the simulation will now run, but the results won't be very interesting. Try changing the model options to enable one or more degradation mechanisms. [This example notebook](https://docs.pybamm.org/en/latest/source/examples/notebooks/models/coupled-degradation.html) shows you how to do this, and how to interpret the results. However, 10 cycles isn't enough for the battery to degrade very much. Fortunately, `ParameterValues` objects are dictionaries and can therefore be updated easily:

```
parameter_values.update({
    "Outer SEI solvent diffusivity [m2.s-1]": 1.25e-20,
    "Lithium plating kinetic rate constant [m.s-1]: 1e-8,
    "Dead lithium decay constant [s-1]: 4e-6,
    "Negative electrode cracking rate": 1.95e-18,
    "Negative electrode LAM constant proportional term [s-1]": 5.5556e-6,
    "Positive electrode LAM constant proportional term [s-1]": 5.5556e-6,
})
```

The updated parameter values will lead to faster degradation.

You can also create, import and use your own parameter sets:

```
from your_parameter_set import get_parameter_values
parameter_values = get_parameter_values()
```

::::challenge{id=dot_product title="Create your own parameter set!"}

Try creating your own parameter set and running a simulation with it! Take one of PyBaMM's existing parameter sets and change some of the function parameters to make it your own!

::::