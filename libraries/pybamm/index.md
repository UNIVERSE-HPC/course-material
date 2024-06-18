---
id: intro_to_pybamm
name: Intro to PyBaMM
dependsOn: [
    introductory_courses.python
]
files: [
    01_running_pybamm.md, 02_experiments.md, 03_parameter_values.md, 04_outputs.md, 05_using_submodels.md, 06_final_exercises.md
]
attribution: 
    - citation: >
        PyBaMM documentation by the PyBaMM Team
      url: https://docs.pybamm.org
      image: https://raw.githubusercontent.com/pybamm-team/pybamm.org/main/static/images/pybamm_logo.svg
      license: BSD-3
summary: |
    This course introduces the basics of PyBaMM (Python Battery Mathematical Modelling), an open-source battery simulation package written in Python.
    We will learn how to run PyBaMM models for various parameters and operating conditions.
    We will also learn how to process and visualise the outputs of the models.
---

## PyBaMM: A High-Level Overview

PyBaMM (Python Battery Mathematical Modelling) is an open-source battery simulation package written in Python. Our mission is to accelerate battery modelling research by providing open-source tools for multi-institutional, interdisciplinary collaboration. Broadly, PyBaMM consists of

1. a framework for writing and solving systems of differential equations,
2. a library of battery models and parameters, and
3. specialized tools for simulating battery-specific experiments and visualizing the results.

Together, these enable flexible model definitions and fast battery simulations, allowing users to explore the effect of different battery designs and modeling assumptions under a variety of operating scenarios.
