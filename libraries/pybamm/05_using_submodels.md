---
name: Using submodels
dependsOn: [
    libraries.pybamm.04_outputs,
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
---

One of the main features of PyBaMM is its modular structure that allows for plug and play models. At the core, all models in PyBaMM are built as a collection of submodels, where a submodel determines a specific subset of the physics. For example, the particle submodel would specify how lithium is transported in the particles. 

The full list of submodels can be found in the [PyBaMM docs](https://docs.pybamm.org/en/latest/source/api/models/submodels/index.html). You can check which submodels a given model uses by calling

```python
model.submodels
```

In this lesson we will focus only on the subset of models to include additional physics, in particular:

* Thermal
* SEI growth
* Lithium plating
* Mechanics
* Active material

## Thermal models

We start with the thermal models. These models account for the changes in the temperature caused by the operation of the battery. The thermal models available in PyBaMM are:

* **Isothermal**: temperature stays constant.
* **Lumped**: the temperature is taken to be homogeneous in the battery, so only the average temperature is computed.
* **X-lumped**: the temperature is taken to be homogeneous across the thickness of the cell, but can vary in the directions parallel to the current collectors. Need to be used in conjunction with a current collector model.
* **X-full**: the temperature is allowed to vary across the thickness of the cell.

More information on the thermal models can be found in [the documentation](https://docs.pybamm.org/en/latest/source/examples/notebooks/models/thermal-models.html).

Thermal models add extra physics on top of the electrochemical models, so we need to choose a base electrochemical model to start with. Then, the extra physics can be specified via the model options. For example, if we want to use the DFN model with a lumped thermal model we do

```python
model = pybamm.lithium_ion.DFN(options={"thermal": "lumped"})
```
and then we can solve the model as usual. 


::::challenge{id=thermal title="Comparing thermal models"}

Compare the results of the DFN model with isothermal and x-full. Plot the relevant variables, and recall that you can find the list of variables to plot by typing `model.variable_names()`. What differences do you observe in the models?

:::solution
A possible implementation is

```python
thermal_options = ["isothermal", "x-full"]
solutions = []

for option in thermal_options:
	model = pybamm.lithium_ion.DFN(name=option, options={"thermal": option})
	simulation = pybamm.Simulation(model)
	solutions.append(simulation.solve([0, 3600]))

pybamm.dynamic_plot(
    solutions,
    output_variables=[
        "Negative particle surface concentration [mol.m-3]",
        "Electrolyte concentration [mol.m-3]",
        "Positive particle surface concentration [mol.m-3]",
        "Negative electrode potential [V]",
        "Electrolyte potential [V]",
        "Positive electrode potential [V]",
        "Current [A]",
        "Voltage [V]",
        "Cell temperature [K]",
    ],
  )

```

We observe that the temperature in the isothermal model remains constant, while for the x-full model the temperature rises almost 20 K. Despite being a spatial-dependent model, the temperature gradients are negligible, which is reasonable given how thin the cell is. We also observe that the voltage for the isothermal case is lower than for the temperature dependent one, given that at higher temperatures the overpotentials are smaller.

:::

::::

## SEI growth

Let's now focus our attention to SEI growth models. These models capture the growth of the solid-electrolyte interphase, which is cause by a side reaction between the electrolyte and lithium. There are multiple ways of simulating SEI growth, and PyBaMM has the following options:

* **None**: no SEI included.
* **Constant**: includes an SEI layer which does not grow.
* **Reaction limited**: assumes reaction is the limiting phenomenon, see Section 5.5.3 of Marquis (2020). It can also be specified to be asymmetric.
* **Solvent-diffusion limited**: assumes that solvent diffusion is the limiting phenomenon, see Section 5.5.4 of Marquis (2020).
* **Electron-migration limited**: assumes that migration of electrons is the limiting phenomenon, see Section 5.5.5 of Marquis (2020).
* **Interstitial-diffusion limited**: assumes that diffusion of lithium-ion intestitials is the limiting phenomenon, see Section 5.4 of Marquis (2020).
* **EC reaction limited**: assumes the model is limited by both reaction and dissuions, see Yang et al (2017).

See all the available options in [the docs](https://docs.pybamm.org/en/latest/source/api/models/base_models/base_battery_model.html#pybamm.BatteryModelOptions). For more information on these models, please see [Marquis (2020)](https://ora.ox.ac.uk/objects/uuid:8afdcc34-cc42-48ba-b316-96a6d0f33a45) and [Yang et al (2017)](https://www.sciencedirect.com/science/article/pii/S0378775317307619).

By default, the change in porosity due to the growth of SEI is not taken into account. To enable it, you need to use the option `"SEI porosity change": "true"`.

::::challenge{id=SEI title="SEI growth models"}

Compare the results of the DFN model with various SEI growth submodels of your choice. Plot the relevant variables, and recall that you can find the list of variables to plot by typing `model.variable_names()`. What differences do you observe in the model?

:::solution
A possible implementation is

```python
SEI_options = [
    "reaction limited", 
    "solvent-diffusion limited", 
    "interstitial-diffusion limited"
]
solutions = []

for option in SEI_options:
	model = pybamm.lithium_ion.DFN(
        name=option, 
        options={"SEI": option, "SEI porosity change": "true"}
    )
	simulation = pybamm.Simulation(model)
	solutions.append(simulation.solve([0, 3600]))

pybamm.dynamic_plot(
    solutions,
    output_variables=[
        "Loss of capacity to SEI [A.h]",
        "Total SEI thickness [m]",
        "X-averaged SEI thickness [m]",
        "Voltage [V]",
        "Negative electrode porosity",
        "X-averaged negative electrode porosity",
    ],
  )
```

We observe that the voltage response is the same for all models, as the SEI contribution is very small. The SEI layer grows fairly homogeneously, and the fastest growing model is the reaction limited one, even though this is likely to be due to the choice of parameters. A possible way to extend this exercise would be to simulate many cycles.

:::

::::

## Particle mechanics
Finally, we consider the models for particle mechanics. These models account for the deformation and cracking on the particles. The models available in PyBaMM are

* None: no mechanical effects included.
* Swelling only: accounts for the deformation of the particles in the lithiation-delithiation cycle.
* Swelling and cracking: accounts for the swelling and also the crack formation on the particle surface.

The mechanical models can be set differently for each electrode by passing a tuple as the option. For example, the following option
```python
model = pybamm.lithium_ion.DFN(
    options={"particle mechanics": ("swelling only", "none")}
)
```
will include swelling model for the negative electrode and no mechanical effects for the positive electrode.

::::challenge{id=mechanics title="Particle mechanics models"}

Run the DFN model with swelling in the negative electrode and swelling and cracking in the positive electrode. Plot the relevant variables, and recall that you can find the list of variables to plot by typing `model.variable_names()`. What do you observe in the model?

Note: you may want to use the "Ai2020" parameter set.

:::solution
A possible implementation is

```python
model = pybamm.lithium_ion.DFN(
    options={"particle mechanics": ("swelling only", "swelling and cracking")}
)
parameter_values = pybamm.ParameterValues("Ai2020")
simulation = pybamm.Simulation(model, parameter_values=parameter_values)
solution = simulation.solve([0, 3600])

solution.plot([
    "Negative particle surface radial stress [Pa]",
    "Negative particle surface tangential stress [Pa]",
    "Negative particle surface displacement [m]",
	"Negative particle crack length [m]",
    "Positive particle surface radial stress [Pa]",
    "Positive particle surface tangential stress [Pa]",
    "Positive particle surface displacement [m]",
    "Positive particle crack length [m]",
])
```

A few key observations are that the surface radial stress is always zero. As expected, there is no cracking in the negative electrode (we did not enable that option) but there is cracking in the positive one. 

:::

::::