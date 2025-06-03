---
name: Packaging and Dependency Management
id: packaging_dependency_management
dependsOn: [technology_and_tooling.ide, software_architecture_and_design.procedural]
files:
  [
    pack_python_01intro.md,
    pack_python_02making.md,
    pack_python_03reusing.md,
    pack_python_04sharing.md,
    virtual_environments_python.md,
    cmake.md,
  ]
learningOutcomes:
  - Organise your Python code into a package.
  - Import functions from your Python package. 
  - Configure your Python package to distribute to others.
  - Share your Python package by publishing to a package index.
  - Use Python virtual environments for reproducibility.
  - Allow reproducible builds of your C++ code with CMake.
attribution:
  - citation: >
      "Python Packaging" course developed by Thibault Lestang and the Oxford Research 
      Software Engineering group
    url: https://github.com/OxfordRSE/python-packaging-course
    image: https://www.rse.ox.ac.uk/sites/default/files/styles/listing_tile_text_displayed_image/public/rse/images/media/oxrse_banner_2.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0

summary: |
  This course introduces the basics of packaging and dependency management in Python and C++.
  For Python, we introduce `venv` for virtual envronments, and `pip` for package management and how to structure a modern Python package and publish it to PyPI.
  For C++, we introduce the CMake build system and how use it to manage dependencies and the build process.
---
