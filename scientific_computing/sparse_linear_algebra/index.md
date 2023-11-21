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
  08-scipy.sparse.linalg.md,
]
summary: |
  This course covers the basics of iterative methods for
  solving linear systems, and how to use sparse matrices to represent and solve
  these systems efficiently.
---

Many problems in science and engineering involve the solution of large systems
of linear equations that are *sparse*, i.e. most of the entries in the matrix
are zero. In this case, it is often more efficient to use *iterative* methods
to solve the system of equations, rather than the direct methods covered in
the previous course. This course covers the basics of iterative methods for
solving linear systems, and how to use sparse matrices to represent and solve
these systems efficiently.

