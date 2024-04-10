---
name: Submodels
dependsOn: [
    libraries.pybamm-developer.05_spm_acid
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
---

# Bonus: submodels
So far we have seen how to build a standalone model, but most battery models within PyBaMM are built up using submodels. Submodels are subsets of a model that describe a specfic type of physics (e.g. conservation of lithium in the particles, SEI growth...). Then, multiple submodels can be combined to create a model (e.g. SPM, DFN...). This is very powerful as it allows to mix and match physics and reduces the amount of code one needs to write, but comes at a cost: any new submodel needs to be written in a very specific way to fit into the submodel structure. 

Before heading to the task (implement the acid dissolution submodel), let's see how a submodel is structured. Submodels are structured in four steps, defining the:

1. Fundamental variables
2. Coupled variables
3. Equations (RHS and/or algebraic)
4. Initial and boundary conditions

Not all submodels will include all four steps. For example, if the submodel is an ODE, it will not have to define algebraic equations nor boundary conditions. If a submodel is analytical (e.g. the potentials in SPM), then it does not have equations nor initial/boundary conditions. Let's take the loss of active material (LAM) submodels (see `loss_active_material.py`) and explain each of the steps.

## Fundamental variables
First we need to define the fundamental variables, which are the variables that we need to solve for in our submodel. This is done by the `get_fundamental_variables` method. 

For the LAM submodel, we observe that first there is a check to see if `self.x_average` is `True` or `False`. Submodels need to work both in Single Particle Models (which average quantities across the electrodes) and Doyle-Fuller-Newman type models (which allow for spatial variations across the electrodes). The `x_average` boolean tells the submodel if it should average quantities across the electrode or not, and it is used to determined whether the fundamental variable we use is the x-averaged version or not. Once the variable is defined, t is passed to the internal method `_get_standard_active_material_variables` which computes any additional related variables (e.g. the averaged version).

## Coupled variables
Next we need to define the couple variables, which are any other variables involved in our model (but not fundamental). This could include, for example, fundamental variables from other submodels (that's why they are called coupled variables) or variables that can be derived from fundamental variables. All this is done by the `get_coupled_variables` method.

For the LAM submodel, we see that it has a series of if statements, based on the submodels options, to determine which LAM mechanisms need to be included. Looking at the `"stress"` case, we see that the method imports some variables that would come from the mechanical submodels and then define some new quantities, like the rate of change of active material `deps_solid_dt`. The other cases follow similarly. At the end, some additional processing is done by the internal method `_get_standard_active_material_change_variables`.

## Equations
Once the variables have been defined, we can write the equations. Note that, as mentioned earlier, if our submodel is analytic, then we would skip this step as there is nothing to solve for, and all the relevant quantities would be computed in `get_coupled_variables`. The definition of the equations is split between the differential (dealt by `set_rhs`) and the algebraic (dealt by `set_algebraic`) parts.

For LAM, we only have differential equations, so the `set_algebraic` method is not defined. The `set_rhs` method updates the `self.rhs` variable with the relevant equations. Note that, in order to write the equations, we need access to the variables we previously defined, but they will be available through the `variables` dictionary.

## Initial and boundary conditions
Finally, we can define initial and boundary conditions if our submodel requires them. This is done via the `set_initial_conditions` and `set_boundary_conditions` methods, respectively. These methods update the `intial_conditions` and `boundary_conditions` dictionaries, and again may use some of the already defined variables through the `variables` dictionary.

There are some other more advanced features of the submodels that we have not covered here. Before implementing any submodels, it is useful to check any existing submodels that are similar and how they have been implemented, and to open an issue to discuss ideas with the other developers.


::::challenge{id="acid-submodel" title="Implement the acid dissolution submodel"}
The task for this lesson is to implement the acid dissolution as a submodel. For a refresher on submodels it might be useful to check the [submodels notebook](https://docs.pybamm.org/en/stable/source/examples/notebooks/models/using-submodels.html). The acid dissolution is a specific type of loss of active material submodel, so it needs to be implemented in `loss_active_material.py` as an additional case in the if statement.

In addition to implementing the submodel, write an example (you can add a new case to the [loss of active material notebook](https://docs.pybamm.org/en/stable/source/examples/notebooks/models/loss_of_active_materials.html)) and add some tests. Once tests pass, you can push to your previous PR.
::::
