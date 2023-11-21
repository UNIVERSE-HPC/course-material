---
id: linear_algebra
name: Linear Algebra
dependsOn: [
  scientific_computing.essential_maths,
]
files: [
  01-matrix-form-of-equations.md,
  02-gaussian-elimination.md,
  03-matrix-decompositions.md,
  04-LU-decomposition.md,
  06-Cholesky-decomposition.md,
  07-QR-decomposition.md,
]
summary: |
  This course covers the basics of linear algebra focusing on the solution of
  dense systems of linear equations using direct methods and matrix
  decompositions. Both the theory and implementation of these methods are
  covered, as well as some standard Python libraries for linear algebra.
---

Linear algebra is the branch of mathematics concerning sets of linear equations 
of the form $a_1x_1 + a_2x_2 + \cdots + a_nx_n = b$, which can be written using matrices and vectors as $Ax = b$.

The solution of these types of equations is fundamental to many areas of
science and engineering, and is essential to the study of machine learning,
data science, modelling, and simulation.
