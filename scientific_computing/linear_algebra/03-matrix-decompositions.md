---
name: Matrix decompositions
dependsOn: [
    'scientific_computing.linear_algebra.02-gaussian-elimination',
]
tags: []
questions:
- "How can matrix decompositions help with repeated solutions?"
- "What is the LU decomposition?"
- "What is the Cholesky decomposition?"
- "What is the QR decomposition?"
objectives:
- "Explain the main useful matrix decompositions"
---



Matrix factorisations play a key role in the solution of problems of the type $A x = b$. 
Often (e.g. ODE solvers), you have a fixed matrix $A$ that must be solved with many 
different $b$ vectors. A matrix factorisation is effectivly a pre-processing step that 
allows you to partition $A$ into multiple factors (e.g. $A = LU$ in the case of $LU$ 
decomposition), so that the actual solve is as quick as possible. Different 
decompositions have other uses besides solving $A x = b$, for example:

- the $LU$, $QR$ and Cholesky decomposition can be used to quickly find the determinant 
  of a large matrix, since $\det(AB) = \det(A) \det(B)$ and the determinant of a 
  triangular matrix is simply the product of its diagonal entries. 
- The Cholesky decomposition can be used to [sample from a multivariate normal 
  distribution](https://stats.stackexchange.com/questions/89796/can-i-use-the-cholesky-method-for-generating-correlated-random-variables-with-gi/89830#89830), 
  and is a very efficient technique to solve $A x = b$ for the specific case of a 
  positive definite matrix. 
- The $QR$ decomposition can be used to solve a minimum least squares problem, to find 
  the eigenvalues and eigenvectors of a matrix, and to calulcate the [Singular Value 
  Decomposition](https://en.wikipedia.org/wiki/Singular_value_decomposition) (SVD), 
  which is itself another very useful decomposition!

