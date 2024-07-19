---
name: Single Particle Model (now as a class)
dependsOn: [libraries.pybamm-developer.03_spm]
tags: [pybamm]
attribution:
  - citation: >
      PyBaMM documentation by the PyBaMM Team
    url: https://docs.pybamm.org
    image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
    license: BSD-3
---

# The Single Particle Model (now as a class)

In the previous lesson we built the Single Particle Model. We implemented it in a script/notebook, which is a good way to build the model for the first time, but not so much if we want to use the model often or share it with other people. In these cases, it is better to wrap the model into a class.

## What is a class?

A [python class](https://www.w3schools.com/python/python_classes.asp) is a "blueprint" to create an object. A python object is a special variable that contain data (variables) and methods (functions) that can manipulate this data. In our case, the object will be a model, which we can then solve.

The goal of this lesson will be to convert the SPM model we wrote in the previous lesson into a class. If you are familiar with classes feel free to start from scratch, otherwise please use [this template](./spm_template.py) as a starting point.

Let's first explain the first few lines of the template. After the header and importing PyBaMM we see the declaration of the class:

```python nolint
class WorkshopSPM(pybamm.BaseModel)
```

This tells us that we are creating a class called `WorkshopSPM` (but you could call it whatever you want), and that this new class inherits from `pybamm.BaseModel`. This simply means that this new class will have all the variables and functions from the `BaseModel` class, so we can build on it.

Next, we see that the class has an `__init__` method. All classes have an `__init__` method, which gets called when an object is created from a class. In this case, the `__init__` method takes one optional variable (ignore `self`) called `name`, which is the name of the model (this is used when plotting, amongst others). The definition of the model needs to be included in this `__init__` method.

The next line

```python nolint
super().__init__(name=name)
```

looks a bit obscure, but basically calls the `__init__` method of the class we are inheriting from (i.e. `BaseModel`). This does some useful initialisation of the class, but we do not need to delve in the details.

::::challenge{id="spm-class" title="Wrap SPM as a class"}
The main task for this lesson is to wrap the SPM model we previously built into a class, and to open a pull request to add it to PyBaMM which passes all the tests.

The first question is where do we need to place the model within the PyBaMM directory structure. For the purpose of the exercise, we want to be able to call the model as `pybamm.WorkshopSPM`, is there anything else we need to do?

:::solution
The solution should be included in the directory `pybamm/models/full_battery_models/lithium_ion`, and we will call the file `spm_workshop.py`. To ensure it can be called as `pybamm.WorkshopSPM` we need to a line to the `__init__.py` file:

```python
from .spm_workshop import WorkshopSPM
```

:::

Next we need to add the equations in the `__init__` method of our class. There are multiple ways things can be defined, but it helps to follow this structure:

1. Variables
2. Parameters
3. Governing equations (in this case the particle models)
4. Output variables

We have broken this into various challenges to guide you step-by-step.

### Variables

First we need to define the variables of our model. These are any variables we need to solve for (e.g. concentrations), but not any derived quantities we will compute from them (e.g. voltage), as these will be defined in the output variables section.

If you want to integrate your model into PyBaMM you should stick to the PyBaMM name convention (see [Tutorial 3](https://docs.pybamm.org/en/latest/source/examples/notebooks/getting_started/tutorial-3-basic-plotting.html) for a list of all variables in the DFN model).

:::solution
In this case we only need to define two variables: the concentrations in the positive and negative particle, respectively.

```python
import pybamm
electrodes = ["negative", "positive"]
c_i = [pybamm.Variable(f"{e.capitalize()} particle concentration [mol.m-3]", domain=f"{e} particle") for e in electrodes]
```

:::

### Parameters

Next we need to define the parameters used in our model. Similarly to the variables, we can use any name convention we want, but it is useful to stick to the one PyBaMM uses so you can use the in-build parameter sets. Check [Tutorial 4](https://docs.pybamm.org/en/latest/source/examples/notebooks/getting_started/tutorial-4-setting-parameter-values.html) to see the standard names for the parameters.

:::solution

```python
# define parameters
I = pybamm.FunctionParameter("Current function [A]",    {"Time [s]": pybamm.t})
D_i = [pybamm.Parameter(f"{e.capitalize()} electrode diffusivity [m2.s-1]") for e in electrodes]
R_i = [pybamm.Parameter(f"{e.capitalize()} particle radius [m]") for e in electrodes]
c0_i = [pybamm.Parameter(f"Initial concentration in {e} electrode [mol.m-3]") for e in electrodes]
delta_i = [pybamm.Parameter(f"{e.capitalize()} electrode thickness [m]") for e in electrodes]
A = pybamm.Parameter("Electrode width [m]") * pybamm.Parameter("Electrode height [m]")  # PyBaMM takes the width and height of the electrodes (assumed rectangular) rather than the total area
epsilon_i = [pybamm.Parameter(f"{e.capitalize()} electrode active material volume fraction") for e in electrodes]
T = pybamm.Parameter("Ambient temperature [K]")
c_e = pybamm.Parameter("Initial concentration in electrolyte [mol.m-3]")
k_i = [pybamm.Parameter(f"{e.capitalize()} electrode reaction rate [m.s-1]") for e in electrodes]
c_i_max = [pybamm.Parameter(f"Maximum concentration in {e} electrode [mol.m-3]") for e in electrodes]
U_i = [pybamm.FunctionParameter(f"{e.capitalize()} electrode OCP [V]", {"stoichiometry": c_i[i] / c_i_max[i]}) for (i, e) in enumerate(electrodes)]

# define universal constants (PyBaAMM has them built in)
F = pybamm.constants.F
R = pybamm.constants.R

# define variables that depend on the parameters
a_i = [3 * epsilon_i[i] / R_i[i] for i in [0, 1]]
j_i = [I / a_i[0] / delta_i[0] / F / A, -I / a_i[1] / delta_i[1] / F / A]
```

:::

### Particle model

Once we have defined the variables and the parameters we can write the governing equations. In this case, we only need to write the model for each particle.

:::solution

```python nolint
# governing equations
dcdt_i = [pybamm.div(D_i[i] * pybamm.grad(c_i[i])) for i in [0, 1]]
self.rhs = {c_i[i]: dcdt_i[i] for i in [0, 1]}

# boundary conditions
lbc = pybamm.Scalar(0)
rbc = [-j_i[i] / D_i[i] for i in [0, 1]]
self.boundary_conditions = {c_i[i]: {"left": (lbc, "Neumann"), "right": (rbc[i], "Neumann")} for i in [0, 1]}

# initial conditions
self.initial_conditions = {c_i[i]: c0_i[i] for i in [0, 1]}
```

:::

### Output variables

Finally, we can define the output variables. Here we want to define all the variables that we might want to plot, save or analyse after solving the model.

:::solution

```python nolint
# define intermediate variables and OCP function parameters
c_i_s = [pybamm.surf(c_i[i]) for i in [0, 1]]
x_i_s = [c_i_s[i] / c_i_max[i] for i in [0, 1]]
i_0_i = [k_i[i] * F * (pybamm.sqrt(c_e) * pybamm.sqrt(c_i_s[i]) * pybamm.sqrt(c_i_max[i] - c_i_s[i])) for i in [0, 1]]
eta_i = [2 * R * T / F * pybamm.arcsinh(j_i[i] * F / (2 * i_0_i[i])) for i in [0, 1]]


# define output variables
[U_n_plus_eta, U_p_plus_eta] = [pybamm.surf(U_i[i]) + eta_i[i] for i in [0, 1]]
V = U_p_plus_eta - U_n_plus_eta
self.variables = {
    "Time [s]": pybamm.t,
    "Voltage [V]": V,
    "Current [A]": I,
    "Negative particle concentration [mol.m-3]": c_i[0],
    "Positive particle concentration [mol.m-3]": c_i[1],
    "Negative particle surface concentration [mol.m-3]": c_i_s[0],
    "Positive particle surface concentration [mol.m-3]": c_i_s[1],
}
```

:::

### Default attributes (optional)

What we have done so far is enough to run the model, but there are some optional extra steps we can take that will make our lives easier going forward. When we process and solve a model, we need to define several additional objects (e.g. geometry, solver...). We can define what should be the default ones for our model. This means that when we process and solve the model we do not need to define them again but we can simply call the default ones (e.g. `default_geometry`, `default_solver`...) or, even better, use a PyBaMM simulation that will use the default ones automatically.

:::solution

```python
@property
def default_geometry(self):
    electrodes = ["negative", "positive"]
    r_i = [pybamm.SpatialVariable("r", domain=[f"{e} particle"], coord_sys="spherical polar") for e in electrodes]
    R_i = [pybamm.Parameter(f"{e.capitalize()} particle radius [m]") for e in electrodes]
    geometry = {f"{e} particle": {r_i[i]: {"min": pybamm.Scalar(0), "max": R_i[i]}} for (i, e) in enumerate(electrodes)}

    return geometry

@property
def default_submesh_types(self):
    domains = ["negative particle", "positive particle"]
    return {d: pybamm.Uniform1DSubMesh for d in domains}

@property
def default_var_pts(self):
    domains = ["negative particle", "positive particle"]
    r_i = [pybamm.SpatialVariable("r", domain=[d], coord_sys="spherical polar") for d in domains]
    return {r: 20 for r in r_i}

@property
def default_spatial_methods(self):
    domains = ["negative particle", "positive particle"]
    return {d: pybamm.FiniteVolume() for d in domains}

@property
def default_solver(self):
    return pybamm.ScipySolver()

@property
def default_quick_plot_variables(self):
    return [
        "Voltage [V]",
        "Current [A]",
        "Negative particle concentration [mol.m-3]",
        "Positive particle concentration [mol.m-3]",
        "Negative particle surface concentration [mol.m-3]",
        "Positive particle surface concentration [mol.m-3]",
    ]
```

Note that we could significantly simplify the code by defining `self.domains`, `self.r_i`, etc. in the `__init__`.

:::

::::

## Adding the model to PyBaMM

We have now written the model into a class, so the next milestone will be to open a PR to add it to PyBaMM. Before we do that, though, there a few other things we need to do.

::::challenge{id="spm-example" title="Write an example for the model"}
We still have not checked if the model works so the first thing we need to do is to write an example for the model. You can write it either as a script or a notebook (just place it in the relevant folder) and ensure the example is well-commented so other users can understand it.
::::

When running the example you might realise your model does not work so you need to do some debugging (see the Troubleshooting section below for some tips). Once the example runs it is time to write some tests.

::::challenge{id="spm-tests" title="Write some tests"}
Tests are bits of code that check that the rest of the code performs as expected. The tests can be found in the `pybamm/tests` directory, and they are automatically run for every pull request to ensure the changes do not break the code. Tests are split between unit tests and integration tests. Unit tests focus on specific parts of code in isolation, while integration tests check that various parts of the code work well together. For example, a unit test for our model would check that when called it is properly initialised, while an integration tests would check that when the model is solved yields the right result.

The goal for this task is to write some tests for your model (at least unit tests, ideally unit and integration tests as well). You can draw some inspiration from the following existing tests for models ([unit](https://github.com/pybamm-team/PyBaMM/blob/develop/tests/unit/test_models/test_full_battery_models/test_lithium_ion/test_basic_models.py) and [integration](https://github.com/pybamm-team/PyBaMM/blob/develop/tests/integration/test_models/test_full_battery_models/test_lithium_ion/test_compare_basic_models.py)).

To run the tests locally you can simply type

```bash
nox -s unit
```

for the unit tests and

```bash
nox -s tests
```

to run both unit and integration tests.

Once your tests run locally you can open a pull request (PR) to the PyBaMM main repository. This will run a whole suite of tests on the cloud (they will take a few minutes to run) that might unearth some more issues with the code. It will also generate a coverage report that will tell you if any parts of your code are not tested.

## Troubleshooting

Writing the equations is not hard, what is hard is getting the model to actually work. Here are a list of the most common errors when writing PyBaMM models and some tips on how to fix it. Remember that using the debugging mode in your code editor is also very useful.

### Domain error

```bash
pybamm.expression_tree.exceptions.DomainError: children must have same or empty domains, not ['positive particle'] and ['negative particle']
```

This means that at some point in your expression tree an operator takes two nodes (i.e. children) that have incompatible domains. The error message will tell you which line in your model triggered the error, so a good way is to set a debug breakpoint there and analyse the domains of the various symbols in the expression by running

```python
from pybamm import Symbol
Symbol.domains
```

Sometimes the issue is further down the expression tree, remember you can visualise the expression tree (see [ODE models in PyBaMM](./01_ode.md)). To access the list of children of a node, you can call the `children` command. You can then access the relevant element in the list and call the `children` command again to navigate down the tree.

### Missing parameters

```bash
KeyError: "'Applied current [A]' not found. Best matches are ['Current function [A]']"
```

This error means that the model requires a parameter that has not been passed when processing the model. PyBaMM will try to provide a guess of the best matches. In this case, the model has a parameter called `Applied current [A]` but the parameters when solving the model do not include it and the best match is `Current function [A]`. This is a common error if the naming convention of your model does not match PyBaMM's.
