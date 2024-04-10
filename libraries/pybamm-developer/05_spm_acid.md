---
name: SPM with Acid Dissolution
dependsOn: [
    libraries.pybamm-developer.04_spm_class
]
tags: [pybamm]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
---

# The Single Particle Model with Acid Dissolution
The next step will be to extend the Single Particle Model to include acid dissolution.  We will use the model introduced by [Kindermann et al (2017)](https://iopscience.iop.org/article/10.1149/2.0321712jes), in particular equations [8]-[10]. Rewritten to match our notation, we have

$$
\frac{\mathrm{d} \epsilon_p}{\mathrm{d} t} = - \frac{i_{0,\mathrm{diss}} \exp \left( \frac{F \eta_\mathrm{diss}}{R T} \right)}{c_p^{max} \delta_p F}
$$
with
$$
\eta_\mathrm{diss} = \phi_p - U_\mathrm{diss},
$$
where $i_{0,\mathrm{diss}}$ is the dissolution exchange current density and $U_\mathrm{diss}$ is the dissolution open-circuit potential. The positive electrode potential is given by $\phi_p = U_p + \eta_p$.

::::challenge{id=spm-acid, title=Write SPM with acid dissolution}
The challenge for this lesson is to write a new class for SPM with acid dissolution. You can create a copy of the SPM model you wrote in the previous lesson and update it accordingly. As before, you should include an example on how to run it (with the additional parameters required, which you can find in [Kindermann et al (2017)](https://iopscience.iop.org/article/10.1149/2.0321712jes)) and tests. Once it is done, you can push to the same PR as before.

:::solution
To extend the SPM model to account for acid dissolution we need to add some additional lines in various parts of the code. Below we include these lines with a hint on which part of the code they should be.
```python
# variables
epsilon_s_p = pybamm.Variable("Volume fraction of active material for positive particle")

# parameters
epsilon_s_p_0 = pybamm.Parameter("Initial volume fraction of active material for positive particle")
i_0_diss = pybamm.Parameter("Dissolution exchange-current density [A.m-2]")
U_diss = pybamm.Parameter("Dissolution open-circuit potential [V]")

# model
depsdt = - i_0_diss * pybamm.exp(F / (R * T) * (U_p_plus_eta - U_diss)) / (c_i_max[1] * delta_i[1] * F)
self.rhs[epsilon_s_p] = depsdt

self.initial_conditions[epsilon_s_p] = epsilon_s_p_0
```
:::

::::
