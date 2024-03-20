---
id: pybamm_developer
name: PyBaMM Model Development
dependsOn: [
    libraries.pybamm
]
files: [
    01_ode.md,
    02_pde.md,
    03_spm.md,
]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
summary: |
    This course covers battery model development in PyBaMM (Python Battery Mathematical Modelling).
    We will learn how to develop new models, submodels & parameter sets, and how to use them in PyBaMM.
---

**PyBaMM: A High-Level Overview**

PyBaMM (Python Battery Mathematical Modelling) is an open-source battery simulation package written in Python. Our mission is to accelerate battery modelling research by providing open-source tools for multi-institutional, interdisciplinary collaboration. Broadly, PyBaMM consists of
1. a framework for writing and solving systems of differential equations,
2. a library of battery models and parameters, and
3. specialized tools for simulating battery-specific experiments and visualizing the results.

Together, these enable flexible model definitions and fast battery simulations, allowing users to explore the effect of different battery designs and modeling assumptions under a variety of operating scenarios.

This course assumes a good understanding of how to use PyBaMM for battery simulations. We will learn how to develop new models, submodels & parameter sets, and how to use them in PyBaMM.