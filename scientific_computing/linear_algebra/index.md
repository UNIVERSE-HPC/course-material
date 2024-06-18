---
id: linear_algebra
name: Linear Algebra Solvers
dependsOn: [
  scientific_computing.essential_maths,
]
files: [
  01-matrix-form-of-equations.md,
  02-gaussian-elimination.md,
  04-LU-decomposition.md,
  06-Cholesky-decomposition.md,
  07-QR-decomposition.md,
]
summary: |
  This course covers the basics of linear algebra focusing on the solution of
  dense systems of linear equations using direct methods and matrix
  decompositions. Both the theory and implementation of these methods are
  covered, as well as some standard Python libraries for linear algebra.
attribution: 
- citation: This material has been adapted from material by Martin Robinson from the "Scientific Computing" module of the SABS R³ Center for Doctoral Training.
  url: https://www.sabsr3.ox.ac.uk
  image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
  license: CC-BY-4.0
- citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1 
  url: https://www.universe-hpc.ac.uk
  image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
  license: CC-BY-4.0


---

Linear algebra is the branch of mathematics concerning sets of linear equations
of the form $a_1x_1 + a_2x_2 + \cdots + a_nx_n = b$, which can be written using matrices and vectors as $Ax = b$.

The solution of these types of equations is fundamental to many areas of
science and engineering, and is essential to the study of machine learning,
data science, modelling, and simulation.
