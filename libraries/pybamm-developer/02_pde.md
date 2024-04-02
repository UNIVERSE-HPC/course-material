---
name: PDE models in PyBaMM
dependsOn: [
    libraries.pybamm_developer.01_ode
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
---

# Creating a simple PDE model

In the previous section, we showed how to create, and solve an ODE model in
pybamm. In this section we show how to create and solve a PDE problem, which is
more complex and requires meshing of the spatial domain.

As an example, we consider the problem of linear diffusion on a unit sphere,

$$
  \frac{\partial c}{\partial t} = \nabla \cdot (\nabla c),
$$

with the following boundary and initial conditions:

$$
  \left.\frac{\partial c}{\partial r}\right\vert_{r=0} = 0, \quad \left.\frac{\partial c}{\partial r}\right\vert_{r=1} = 2, \quad \left.c\right\vert_{t=0} = 1.
$$


## Setting up the model

As in the previous example, we start with a
[`pybamm.BaseModel`](https://docs.pybamm.org/en/stable/source/api/models/base_models/base_model.html#pybamm.BaseModel)
object and define our model variables. Since we are now solving a PDE we need to
tell pybamm the domain each variable belongs to so that it can be discretised in
space in the correct way. This is done by passing the keyword argument `domain`,
and in this example we choose the domain "negative particle".

```python
model = pybamm.BaseModel()

c = pybamm.Variable("Concentration", domain="negative particle")
```

Note that we have given our variable the (useful) name "Concentration", but the symbol representing this variable is simply `c`. As noted in the previous section, the name of the variable is arbitrary and you can choose any variable name that is most useful/meaningful to you.

We then state out governing equations. Sometime it is useful to define
intermediate quantities in order to express the governing equations more easily.
In this example we define the flux, then define the rhs to be minus the
divergence of the flux. The equation is then added to the dictionary `model.rhs`

```python
N = -pybamm.grad(c)  # define the flux
dcdt = -pybamm.div(N)  # define the rhs equation

model.rhs = {c: dcdt}  # add the equation to rhs dictionary
```

Unlike ODE models, PDE models require both initial and boundary conditions.
Similar to initial conditions, boundary conditions can be added using the
dictionary [`model.boundary_conditions`](https://docs.pybamm.org/en/stable/source/api/models/base_models/base_model.html#pybamm.BaseModel.boundary_conditions). Boundary conditions for each variable
are provided as a dictionary of the form `{side: (value, type)}`, where, in 1D,
side can be "left" or "right", value is the value of the boundary conditions,
and type is the type of boundary condition (at present, this can be "Dirichlet"
or "Neumann").

```python
# initial conditions
model.initial_conditions = {c: pybamm.Scalar(1)}

# boundary conditions
lbc = pybamm.Scalar(0)
rbc = pybamm.Scalar(2)
model.boundary_conditions = {c: {"left": (lbc, "Neumann"), "right": (rbc, "Neumann")}}
```

Note that in our example the boundary conditions take constant values, but the
value can be any valid pybamm expression.

Finally, we add any variables of interest to the dictionary [`model.variables`](https://docs.pybamm.org/en/stable/source/api/models/base_models/base_model.html#pybamm.BaseModel.variables)

```python
model.variables = {"Concentration": c, "Flux": N}
```

## Using the model
Now the model is now completely defined all that remains is to discretise and solve. Since this model is a PDE we need to define the geometry on which it will be solved, and choose how to mesh the geometry and discretise in space.

### Defining a geometry

We can define spatial variables in a similar way to how we defined model
variables, providing a domain and a coordinate system. The relevent class to use here is [`pybamm.SpatialVariable`](https://docs.pybamm.org/en/stable/source/api/expression_tree/independent_variable.html#pybamm.SpatialVariable):

```python
# define geometry
r = pybamm.SpatialVariable(
    "r", domain=["negative particle"], coord_sys="spherical polar"
)
```

The geometry on which we wish to solve the model is defined using a nested
dictionary. The first key is the domain name (here "negative particle") and the
entry is a dictionary giving the limits of the domain.

```python
geometry = {"negative particle": {r: {"min": pybamm.Scalar(0), "max": pybamm.Scalar(1)}}}
```

### Defining a mesh

We then create a uniform one-dimensional mesh with 20 points, using the
[`pybamm.Mesh`](https://docs.pybamm.org/en/stable/source/api/meshes/meshes.html#pybamm.Mesh)
class. As well as the geometry, the mesh class also takes a dictionary of
submesh types (see below, for more details), and a dictionary specifying the number of mesh
points for each spatial variable.

```python
# mesh and discretise
submesh_types = {"negative particle": pybamm.Uniform1DSubMesh}
var_pts = {r: 20}
mesh = pybamm.Mesh(geometry, submesh_types, var_pts)
```

Here we have used the [`pybamm.Uniform1DSubMesh`](https://docs.pybamm.org/en/stable/source/api/meshes/one_dimensional_submeshes.html#pybamm.Uniform1DSubMesh) class to create a uniform mesh.
This class does not require any parameters, and so we can pass it directly to
the `submesh_types` dictionary. However, many other submesh types can take
additional parameters.  Example of meshes that do require parameters include the
[`pybamm.Exponential1DSubMesh`](https://docs.pybamm.org/en/stable/source/api/meshes/one_dimensional_submeshes.html#pybamm.Exponential1DSubMesh) which clusters points close to one or both
boundaries using an exponential rule. It takes a parameter which sets how
closely the points are clustered together, and also lets the users select the
side on which more points should be clustered. For example, to create a mesh
with more nodes clustered to the right (i.e. the surface in the particle
problem), using a stretch factor of 2, we pass an instance of the exponential
submesh class and a dictionary of parameters into the `MeshGenerator` class as
follows: 

```python
exp_mesh = pybamm.MeshGenerator(pybamm.Exponential1DSubMesh, submesh_params={"side": "right", "stretch": 2})
```

The list of available submeshes is given in the [documentation](https://docs.pybamm.org/en/stable/source/api/meshes/index.html).

## Discretising the domains

After defining a mesh we choose a spatial method to discretise the "nagative
particle" domain. Here we choose the Finite Volume Method. We then set up a
discretisation by passing the mesh and spatial methods to the class
[`pybamm.Discretisation`](https://docs.pybamm.org/en/stable/source/api/spatial_methods/discretisation.html#pybamm.Discretisation). The model is then processed, turning the variables into
(slices of) a statevector, spatial variables into vector and spatial operators
into matrix-vector multiplications.

```python
spatial_methods = {"negative particle": pybamm.FiniteVolume()}
disc = pybamm.Discretisation(mesh, spatial_methods)
disc.process_model(model);
```

Now that the model has been discretised we are ready to solve. 

## Solving the model
As before, we choose a solver and times at which we want the solution returned. We then solve, extract the variables we are interested in, and plot the result.

```python
# solve
solver = pybamm.ScipySolver()
t = np.linspace(0, 1, 100)
solution = solver.solve(model, t)

# post-process, so that the solution can be called at any time t or space r
# (using interpolation)
c = solution["Concentration"]

# plot
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 4))

ax1.plot(solution.t, c(solution.t, r=1))
ax1.set_xlabel("t")
ax1.set_ylabel("Surface concentration")
r = np.linspace(0, 1, 100)
ax2.plot(r, c(t=0.5, r=r))
ax2.set_xlabel("r")
ax2.set_ylabel("Concentration at t=0.5")
plt.tight_layout()
plt.show()
```

::::challenge{id="single-particle-pde" title="Create a single particle PDE model"}

Now it is time to solve a real-life battery problem! We consider the problem of spherical diffusion in the negative electrode particle within the single particle model. That is,

$$
  \frac{\partial c}{\partial t} = \nabla \cdot (D \nabla c),
$$

with the following boundary and initial conditions:

$$
  \left.\frac{\partial c}{\partial r}\right\vert_{r=0} = 0, \quad \left.\frac{\partial c}{\partial r}\right\vert_{r=R} = -\frac{j}{FD}, \quad \left.c\right\vert_{t=0} = c_0,
$$

where $c$ is the concentration, $r$ the radial coordinate, $t$ time, $R$ the
particle radius, $D$ the diffusion coefficient, $j$ the interfacial current
density, $F$ Faraday's constant, and $c_0$ the initial concentration. 

We use the following parameters:

| Symbol | Units              | Value                                          |
|:-------|:-------------------|:-----------------------------------------------|
| $R$      | m                | $10 \times 10^{-6}$                            |
| $D$      | m${^2}$ s$^{-1}$ | $3.9 \times 10^{-14}$                          |
| $j$      | A m$^{-2}$       | $1.4$                                          |
| $F$      | C mol$^{-1}$     | $96485$                                        |
| $c_0$    | mol m$^{-3}$     | $2.5 \times 10^{4}$                            |

Create a model for this problem, discretise it and solve it. Use a uniform mesh
with 20 points, and discretise the domain using the Finite Volume Method. Solve
the model for 1 hour, and plot the surface concentration as a function of time,
and the concentration as a function of radius at $t=1000$ seconds.

:::solution

```python
import pybamm
import numpy as np
import matplotlib.pyplot as plt

model = pybamm.BaseModel()

# define parameters
R = pybamm.Parameter("Particle radius [m]")
D = pybamm.Parameter("Diffusion coefficient [m2.s-1]")
j = pybamm.Parameter("Interfacial current density [A.m-2]")
F = pybamm.Parameter("Faraday constant [C.mol-1]")
c0 = pybamm.Parameter("Initial concentration [mol.m-3]")

# define variable
c = pybamm.Variable("Concentration [mol.m-3]", domain="negative particle")

# governing equations
N = -D * pybamm.grad(c)  # flux
dcdt = -pybamm.div(N)
model.rhs = {c: dcdt}  

# boundary conditions 
lbc = pybamm.Scalar(0)
rbc = -j / F / D
model.boundary_conditions = {c: {"left": (lbc, "Neumann"), "right": (rbc, "Neumann")}}

# initial conditions 
model.initial_conditions = {c: c0}

model.variables = {
    "Concentration [mol.m-3]": c,
    "Surface concentration [mol.m-3]": pybamm.surf(c),
    "Flux [mol.m-2.s-1]": N,
}

param = pybamm.ParameterValues(
    {
        "Particle radius [m]": 10e-6,
        "Diffusion coefficient [m2.s-1]": 3.9e-14,
        "Interfacial current density [A.m-2]": 1.4,
        "Faraday constant [C.mol-1]": 96485,
        "Initial concentration [mol.m-3]": 2.5e4,
    }
)

r = pybamm.SpatialVariable("r", domain=["negative particle"], coord_sys="spherical polar")
geometry = {"negative particle": {r: {"min": pybamm.Scalar(0), "max": R}}}

param.process_model(model)
param.process_geometry(geometry)

submesh_types = {"negative particle": pybamm.Uniform1DSubMesh}
var_pts = {r: 20}
mesh = pybamm.Mesh(geometry, submesh_types, var_pts)

spatial_methods = {"negative particle": pybamm.FiniteVolume()}
disc = pybamm.Discretisation(mesh, spatial_methods)
disc.process_model(model)

# solve
solver = pybamm.ScipySolver()
t = np.linspace(0, 3600, 600)
solution = solver.solve(model, t)

# post-process, so that the solution can be called at any time t or space r
# (using interpolation)
c = solution["Concentration [mol.m-3]"]
c_surf = solution["Surface concentration [mol.m-3]"]

# plot
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 4))

ax1.plot(solution.t, c_surf(solution.t))
ax1.set_xlabel("Time [s]")
ax1.set_ylabel("Surface concentration [mol.m-3]")

r = mesh["negative particle"].nodes # radial position
time = 1000  # time in seconds
ax2.plot(r * 1e6, c(t=time, r=r), label="t={}[s]".format(time))
ax2.set_xlabel("Particle radius [microns]")
ax2.set_ylabel("Concentration [mol.m-3]")
ax2.legend()

plt.tight_layout()
plt.show()
```
:::
::::

