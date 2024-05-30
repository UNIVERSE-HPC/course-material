---
id: sparse_linear_algebra
name: Sparse Matrices and Iterative Solvers
dependsOn: [
  scientific_computing.linear_algebra,
]
files: [
  01-sparse-matrices.md,
  02-coo-matrix.md,
  03-finite-difference.md,
  04-scipy-sparse.md,
  06-jacobi-relaxation-methods.md,
  07-conjugate-gradient-method.md,
]
summary: |
  This course covers the basics of iterative methods for
  solving linear systems, and how to use sparse matrices to represent and solve
  these systems efficiently.
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

Many problems in science and engineering involve the solution of large systems
of linear equations that are *sparse*, i.e. most of the entries in the matrix
are zero. In this case, it is often more efficient to use *iterative* methods
to solve the system of equations, rather than the direct methods covered in
the previous course. This course covers the basics of iterative methods for
solving linear systems, and how to use sparse matrices to represent and solve
these systems efficiently.

