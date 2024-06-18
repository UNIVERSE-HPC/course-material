---
name: Single Particle Model
dependsOn: [
    libraries.pybamm-developer.02_pde
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
---

# The Single Particle Model

Now we will extend our PDE model to the full single particle model. The single
particle model is a system of PDEs that describes the behaviour of a lithium-ion
battery electrode particle.

## The Single Particle Model state equations

The SPM model only needs to solve for the concentration of lithium ions in the
positive and negative electrodes, $c_n$ and $c_p$. The diffusion of lithium ions
in each electrode particle $c_i$ is given by:

$$
\frac{\partial c_i}{\partial t} = \nabla \cdot (D_i \nabla c_i)
$$

subject to the following boundary and initial conditions:

$$
\left.\frac{\partial c_i}{\partial r}\right\vert_{r=0} = 0, \quad \left.\frac{\partial c}{\partial r}\right\vert_{r=R_i} = -j_i, \quad \left.c\right\vert_{t=0} = c^0_i
$$

where $c_i$ is the concentration of lithium ions in the positive ($i=n$) or
negative ($i=p$) electrode, $D_i$ is the diffusion coefficient, $j_i$ is the
interfacial current density, and $c^0_i$ is the concentration at the particle
surface.

The primary addition here in comparison with our previous PDE model is the
fluxes of lithium ions in the positive and negative electrodes $j_i$. These are
dependent on the applied current $I$:

$$
\begin{align*}
j_n &= \frac{I}{a_n \delta_n F \mathcal{A}}, \qquad
j_p &= \frac{-I}{a_p \delta_p F \mathcal{A}},
\end{align*}
$$

where $a_i = 3 \epsilon_i / R_i$ is the specific surface area of the electrode,
$\epsilon_i$ is the volume fraction of active material, $\delta_i$ is the
thickness of the electrode, $F$ is the Faraday constant, and
$\mathcal{A}$ is the electrode surface area.

## Function Parameters in PyBaMM

The applied current $I(t)$ is a parameter to the model, but unlike the
other parameters we have seen so far, it is a function of time. We can define
this using `pybamm.FunctionParameter`:

```python
I = pybamm.FunctionParameter("Current function [A]", {"Time [s]": pybamm.t})
```

This will allow us to define the applied current as a function of time when we
create our `pybamm.ParameterValues` object.

```python
def current(t):
    return 1
param = pybamm.ParameterValues(
    {
        "Current function [A]": current
    }
)
```

## Domains in PyBaMM

In the SPM model we have two different spatial domains, one for the positive
electrode and one for the negative electrode. Remember that when we define a variable in
PyBaMM, we can specify the domain using the `domain` keyword argument:

```python
c_n = pybamm.Variable("Negative particle concentration [mol.m-3]", domain="negative particle")
```

::::challenge{id="spm-state-equations" title="SPM governing equations"}

Copy your PDE model from the previous challenge to a new file, and modify it to
include the state equations for the concentration of lithium ions in both the
positive and negative electrodes. Note that now you will have two variables,
$c_n$ and $c_p$, that must be defined on separate spatial domains, and a number
of new parameters. Define the applied current $I$ as a input parameter that is a
function of time using `pybamm.FunctionParameter`.

:::solution

```python
import pybamm

model = pybamm.BaseModel()

# define parameters
electrodes = ["negative", "positive"]
I = pybamm.FunctionParameter("Current function [A]", {"Time [s]": pybamm.t})
D_i = [pybamm.Parameter(f"{e.capitalize()} electrode diffusivity [m2.s-1]") for e in electrodes]
R_i = [pybamm.Parameter(f"{e.capitalize()} particle radius [m]") for e in electrodes]
c0_i = [pybamm.Parameter(f"Initial concentration in {e} electrode [mol.m-3]") for e in electrodes]
delta_i = [pybamm.Parameter(f"{e.capitalize()} electrode thickness [m]") for e in electrodes]
A = pybamm.Parameter("Electrode width [m]") * pybamm.Parameter("Electrode height [m]")  # PyBaMM takes the width and height of the electrodes (assumed rectangular) rather than the total area
epsilon_i = [pybamm.Parameter(f"{e.capitalize()} electrode active material volume fraction") for e in electrodes]

# define universal constants (PyBaAMM has them built in)
F = pybamm.constants.F

# define variables that depend on the parameters
a_i = [3 * epsilon_i[i] / R_i[i] for i in [0, 1]]
j_i = [I / a_i[0] / delta_i[0] / F / A, -I / a_i[1] / delta_i[1] / F / A]

# define state variables
c_i = [pybamm.Variable(f"{e.capitalize()} particle concentration [mol.m-3]", domain=f"{e} particle") for e in electrodes]

# governing equations
dcdt_i = [pybamm.div(D_i[i] * pybamm.grad(c_i[i])) for i in [0, 1]]
model.rhs = {c_i[i]: dcdt_i[i] for i in [0, 1]}

# boundary conditions
lbc = pybamm.Scalar(0)
rbc = [-j_i[i] / D_i[i] for i in [0, 1]]
model.boundary_conditions = {c_i[i]: {"left": (lbc, "Neumann"), "right": (rbc[i], "Neumann")} for i in [0, 1]}

# initial conditions
model.initial_conditions = {c_i[i]: c0_i[i] for i in [0, 1]}
```

:::
::::

## Output variables for the Single Particle Model

Now that we have defined the equations to solve, we turn to the output variables
that we need to calculate from the state variables $c_n$ and $c_p$. The terminal
voltage of the battery is given by:

$$
V = U_p(x_p^s) - U_n(x_n^s) + \eta_p - \eta_n
$$

where $U_i$ is the open circuit potential (OCP) of the electrode, $x_i^s =
c_i(r=R_i) / c_i^{max}$ is the surface stoichiometry, and $\eta_i$ is the
overpotential.

Assuming Butler-Volmer kinetics and $\alpha_i = 0.5$, the overpotential is given by:

$$
\eta_i = \frac{2RT}{F} \sinh^{-1} \left( \frac{j_i F}{2i_{0,i}} \right)
$$

where the exchange current density $i_{0,i}$ is given by:

$$
i_{0,i} = k_i F \sqrt{c_e} \sqrt{c_i(r=R_i)} \sqrt{c_i^{max} - c_i(r=R_i)}
$$

where $c_e$ is the concentration of lithium ions in the electrolyte, and $k_i$
is the reaction rate constant.

### Surface stoichiometry

Lets revisit the definition of the surface stoichiometry $x_i^s$, which is

$$
x_i^s = \frac{c_i(r=R_i)}{c_i^{max}}
$$

$c_i^{max}$ is the maximum concentration of lithium ions in the electrode, and
is a parameter of the model. However, $c_i(r=R_i)$ is the concentration of
lithium ions at the surface of the electrode particle. How can we express this
in PyBaMM, given that we only have the concentration $c_i$ defined on the whole
domain?

To get the surface concentration, we can use the `pybamm.boundary_value` or
`pybamm.surf` functions. The `pybamm.boundary_value` function returns the value
of a variable at the boundary of a domain (either "left" or "right"), and the
`pybamm.surf` function returns the value of a variable at the right boundary of
a domain. For example, to get the surface concentration of $c_n$ we can use:

```python
c_n_surf = pybamm.surf(c_n)
```

or

```python
c_n_surf = pybamm.boundary_value(c_n, "right")
```

### Open Circuit Potentials (OCPs)

The OCPs $U_i$ are functions of the surface stoichiometries $x_i^s$, and we can
define them using `pybamm.FunctionParameter` in a similar way to the applied
current $I$. For example, to define the OCP of the positive electrode as a
function of the surface stoichiometry $x_p^s$:  

```python
U_p = pybamm.FunctionParameter("Positive electrode OCP [V]", {"stoichiometry": x_p_s})
```

### PyBaMM's built-in functions

PyBaMM has a number of built-in functions that can be used in expressions. For
the SPM model, you will need to use the
[`pybamm.sqrt`](https://docs.pybamm.org/en/stable/source/api/expression_tree/functions.html#pybamm.sqrt)
and
[`pybamm.arcsinh`](https://docs.pybamm.org/en/stable/source/api/expression_tree/functions.html#pybamm.arcsinh)
functions:

```python
four = pybamm.Scalar(4)
two = pybamm.sqrt(four)
arcsinh_four = pybamm.arcsinh(four)
```

You can see a list of all the functions available in PyBaMM in the [documentation](https://docs.pybamm.org/en/stable/source/api/expression_tree/functions.html).

::::challenge{id="spm-output-variables" title="SPM output variables"}

Define the output variables for the SPM model. You will need to define the
overpotentials $\eta_n$ and $\eta_p$, the exchange current densities $i_{0,n}$
and $i_{0,p}$, and the terminal voltage $V$. You will also need to define the
OCPs $U_n$ and $U_p$ as functions of the surface stoichiometries $x_n^s$ and
$x_p^s$. You can use `pybamm.FunctionParameter` to define the OCPs as functions
of the surface stoichiometries.

Define the following output variables for the model

- Terminal voltage $V$
- Surface concentration in negative particle $c_n^s$

:::solution

```python
# call universal constants (PyBaMM has them built in)
R = pybamm.constants.R

# define temperature
T = pybamm.Parameter("Ambient temperature [K]")

# define new parameters for the output variables
c_e = pybamm.Parameter("Initial concentration in electrolyte [mol.m-3]")
k_i = [pybamm.Parameter(f"{e.capitalize()} electrode reaction rate [m.s-1]") for e in electrodes]
c_i_max = [pybamm.Parameter(f"Maximum concentration in {e} electrode [mol.m-3]") for e in electrodes]

# define intermediate variables and OCP function parameters
c_i_s = [pybamm.surf(c_i[i]) for i in [0, 1]]
x_i_s = [c_i_s[i] / c_i_max[i] for i in [0, 1]]
i_0_i = [k_i[i] * F * (pybamm.sqrt(c_e) * pybamm.sqrt(c_i_s[i]) * pybamm.sqrt(c_i_max[i] - c_i_s[i])) for i in [0, 1]]
eta_i = [2 * R * T / F * pybamm.arcsinh(j_i[i] * F / (2 * i_0_i[i])) for i in [0, 1]]
U_i = [pybamm.FunctionParameter(f"{e.capitalize()} electrode OCP [V]", {"stoichiometry": x_i_s[i]}) for (i, e) in enumerate(electrodes)]

# define output variables
[U_n_plus_eta, U_p_plus_eta] = [U_i[i] + eta_i[i] for i in [0, 1]]
V = U_p_plus_eta - U_n_plus_eta
model.variables = {
  "Voltage [V]": V,
  "Negative particle surface concentration [mol.m-3]": c_i_s[0],
}
```

:::
::::

## Discretising and solving the Single Particle Model

Now that we have defined the SPM model, we can discretise and solve it using
PyBaMM. We can use the same meshing and discretisation methods as in the
previous section, but we now need to discretise the model on two different
spatial domains.

::::challenge{id="spm-discretise-solve" title="Discretise and solve the SPM model"}

Discretise and solve the SPM model using the same methods as in the previous
section. The following parameter values object copies the parameters from the PyBaMM
Chen2020 model, feel free to use this to define the parameters for the SPM model.

```python
param = pybamm.ParameterValues("Chen2020")
# PyBaMM parameters provide the exchange current density directly, rather than the reaction rate, so define here
param.update({
    "Positive electrode reaction rate [m.s-1]": 1e-3,
    "Negative electrode reaction rate [m.s-1]": 1e-3,
}, check_already_exists=False)
```

:::solution

```python

import numpy as np
import matplotlib.pyplot as plt

# create geometry
r_i = [pybamm.SpatialVariable("r", domain=[f"{e} particle"], coord_sys="spherical polar") for e in electrodes]
geometry = {f"{e} particle": {r_i[i]: {"min": pybamm.Scalar(0), "max": R_i[i]}} for (i, e) in enumerate(electrodes)}

param.process_model(model)
param.process_geometry(geometry)

# meshs
submesh_types = {f"{e} particle": pybamm.Uniform1DSubMesh for e in electrodes}
var_pts = {r: 20 for r in r_i}
mesh = pybamm.Mesh(geometry, submesh_types, var_pts)

# spatial methods
spatial_methods = {f"{e} particle": pybamm.FiniteVolume() for e in electrodes}

# discretise model
disc = pybamm.Discretisation(mesh, spatial_methods)
disc.process_model(model)


# solve
solver = pybamm.ScipySolver()
t = np.linspace(0, 3600, 600)
solution = solver.solve(model, t)

# post-process, so that the solution can be called at any time t or space r
# (using interpolation)
v = solution["Voltage [V]"]
csurf = solution["Negative particle surface concentration [mol.m-3]"]

# plot
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 4))

ax1.plot(solution.t, v(solution.t))
ax1.set_xlabel("Time [s]")
ax1.set_ylabel("Voltage [V]")

ax2.plot(solution.t, csurf(solution.t))
ax2.set_xlabel("Time [s]")
ax2.set_ylabel("Negative particle surface concentration [mol.m-3]")

plt.tight_layout()
plt.show()
```

:::
::::
