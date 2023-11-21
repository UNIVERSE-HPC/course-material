---
name: Krylov subspace methods and CG
dependsOn: [
  'scientific_computing.sparse_linear_algebra.06-jacobi-relaxation-methods'
]
tags: []
---

## The Krylov subspace

The set of basis vectors for the Krylov subspace $\mathcal{K}_k(A, b)$ are formed by 
repeated application of a matrix $A$ on a vector $b$

$$
\mathcal{K}_k(A, b) = \text{span}\{ b, Ab, A^2b, ..., A^{k-1}b \}
$$

## Krylov subspace methods

Krylov subspace methods are an important family of iterative algorithms for solving 
$Ax=b$. Lets suppose that $A$ is an $n \times n$ invertible matrix, and our only 
knowledge of $A$ is its matrix-vector product with an arbitrary vector $\mathbf{x}$. 
Repeated application of $A$, $n$ times on an initial vector $b$ gives us a sequence of 
vectors $\mathbf{b}, A \mathbf{b}, A^2 \mathbf{b}, ..., A^{n} \mathbf{b}$. Since we are 
in $n$-dimensional space, we are guaranteed that these $n+1$ vectors are linearly 
dependent, and therefore

$$
a_0 \mathbf{b} + a_1 A \mathbf{b} + ... + a_n A^n \mathbf{b} = 0
$$

for some set of coefficients $a_i$. Let now pick the smallest index $k$ such that $a_k 
\ne 0$, and multiply the above equation by $\frac{1}{a_k} A^{-k-1}$, giving

$$
A^{-1} b = \frac{1}{a_k} ( a_{k+1} b + ... + a_n A^{n-k-1} b )
$$

This implies that $A^{-1} b$ can be found only via repeated application of $A$, and also 
motivates the search for solutions from the Krylov subspace.

For each iteration $k$ of a Krylov subspace method, we choose the "best" linear 
combination of the Krylov basis vectors $\mathbf{b}, A\mathbf{b}, ... , A^{k−1} 
\mathbf{b}$ to form an improved solution $\mathbf{x}_k$. Different methods give various 
definitions of "best", for example:

1. The residual $\mathbf{r}_k = \mathbf{b} − A\mathbf{x}_k$
is orthogonal to $\mathcal{K}_k$ (Conjugate Gradients).
2. The residual $\mathbf{r}_k$ has minimum norm for $\mathbf{x}_k$
in $\mathcal{K}_k$ (GMRES and MINRES).
3. $\mathbf{r}_k$ is orthogonal to a different space $\mathcal{K}_k(A^T)$ (BiConjugate 
   Gradients).

## Conjugate Gradient Method


Here we will give a brief summary of the CG method, for more details you can consult the 
text by Golub and Van Loan (Chapter 10).

The CG method is based on minimising the function

$$
\phi(x) = \frac{1}{2}x^T A x - x^T b
$$

If we set $x$ to the solution of $Ax =b$, that is $x = A^{-1} b$, then the value of 
$\phi(x)$ is at its minimum $\phi(A^{-1} b) = -b^T A^{-1} b / 2$, showing that solving 
$Ax = b$ and minimising $\phi$ are equivalent.

At each iteration $k$ of CG we are concerned with the *residual*, defined as $r_k = b - 
A x_k$. If the residual is nonzero, then at each step we wish to find a positive 
$\alpha$ such that $\phi(x_k + \alpha p_k) < \phi(x_k)$, where $p_k$ is the *search 
direction* at each $k$. For the classical steepest descent optimisation algorithm the 
search direction would be the residual $p_k = r_k$, however, steepest descent can suffer 
from convergence problems, so instead we aim to find a set of search directions $p_k$ so 
that $p_k^T r\_{k-1} \ne 0$ (i.e. at each step we are guaranteed to reduce $\phi$), and 
that the search directions are linearly independent. The latter condition guarantees 
that the method will converge in at most $n$ steps, where $n$ is the size of the square 
matrix $A$.

It can be shown that the best set of search directions can be achieved by setting

$$
\begin{aligned}
\beta_k &= \frac{-p^T_{k-1} A r_{k-1}}{p^T_{k-1} A p_{k-1}} \\
p_k &= r_{k-1} + \beta_k p_{k-1} \\
\alpha_k &= \frac{p^T_k r_{k-1}}{p^T_k A p_k}
\end{aligned}
$$

This ensures that the search direction $\mathbf{p}\_k$ is the closest vector to 
$\mathbf{r}_{k-1}$ that is also *A-conjugate* to $\mathbf{p}\_1, ..., 
\mathbf{p}\_{k-1}$, i.e. $p^T_i A p_j=0$ for all $i \ne j$, which gives the algorithm its 
name. After $k$ iterations the sequence of residuals $\mathbf{r}_i$ for $i=1..k$ form a 
set of mutually orthogonal vectors that span the Krylov subspace $\mathcal{K}_k$.

Directly using the above equations in an iterative algorithm results in the standard CG 
algorithm. A more efficient algorithm can be derived from this by computing the 
residuals recursively via $r_k = r\_{k-1} - \alpha_k A p_k$, leading to the final 
algorithm given below (reproduced from 
[Wikipedia](https://en.wikipedia.org/wiki/Conjugate_gradient_method)):

![Conjugate Gradient algorithm](/scientific-computing/images/unit_02/cg_pseudocode.svg)

### Preconditioning

The CG method works well (i.e. converges quickly) if the *condition number* of the 
matrix $A$ is low. The condition number of a matrix gives a measure of how much the 
solution $x$ changes in response to a small change in the input $b$, and is a property 
of the matrix $A$ itself, so can vary from problem to problem. In order to keep the 
number of iterations small for iterative solvers, it is therefore often necessary to use 
a *preconditioner*, which is a method of transforming what might be a difficult problem 
with a poorly conditioned $A$, into a well conditioned problem that is easy to solve.

Consider the case of preconditioning for the CG methods, we start from the standard 
problem $A x = b$, and we wish to solve an *equivalent* transformed problem given by

$$
\tilde{A} \tilde{x} = \tilde{b}
$$

where $\tilde{A} = C^{-1} A C^{-1}$, $\tilde{x} = Cx$, $\tilde{b} = C^{-1} b $, and $C$ 
is a symmetric positive matrix.

We then simply apply the standard CG method as given above to this transformed problem. 
This leads to an algorithm which is then simplified by instead computing the transformed 
quantities $\tilde{p}_k = C p_k$, $\tilde{x}_k = C x_k$, and $\tilde{r}_k = C^{-1} r_k$. 
Finally we define a matrix $M = C^2$, which is known as the *preconditioner*, leading to 
the final preconditioned CG algorithm given below (reproduced and edited from 
[Wikipedia](https://en.wikipedia.org/wiki/Conjugate_gradient_method)):

$\mathbf{r}\_0 := \mathbf{b} - \mathbf{A x}\_0$\\
$\mathbf{z}\_0 := \mathbf{M}^{-1} \mathbf{r}\_0$\\
$\mathbf{p}\_0 := \mathbf{z}\_0$\\
$k := 0 \, $\\
**repeat until $|| \mathbf{r}_k ||_2 < \epsilon ||\mathbf{b}||_2$**\\
&nbsp;&nbsp; $\alpha\_k := \frac{\mathbf{r}\_k^T \mathbf{z}\_k}{ \mathbf{p}\_k^T 
\mathbf{A p}\_k }$\\
&nbsp;&nbsp; $\mathbf{x}\_{k+1} := \mathbf{x}\_k + \alpha\_k \mathbf{p}\_k$ \\
&nbsp;&nbsp; $\mathbf{r}\_{k+1} := \mathbf{r}\_k - \alpha_k \mathbf{A p}\_k$ \\
&nbsp;&nbsp; **if** $r\_{k+1}$ is sufficiently small then exit loop **end if** \\
&nbsp;&nbsp; $\mathbf{z}\_{k+1} := \mathbf{M}^{-1} \mathbf{r}\_{k+1}$\\
&nbsp;&nbsp; $\beta\_k := \frac{\mathbf{r}\_{k+1}^T \mathbf{z}\_{k+1}}{\mathbf{r}\_k^T 
\mathbf{z}\_k}$\\
&nbsp;&nbsp; $\mathbf{p}\_{k+1} := \mathbf{z}\_{k+1} + \beta_k \mathbf{p}\_k$\\
&nbsp;&nbsp; $k := k + 1 \, $\\
**end repeat**

The key point to note here is that the preconditioner is used by inverting $M$, so this 
matrix must be "easy" to solve in some fashion, and also result in a transformed problem 
with better conditioning.

**Termination**: The CG algorithm is normally run until convergence to a given 
tolerance which is based on the norm of the input vector $b$. In the algorithm above we 
iterate until the residual norm is less than some fraction (set by the user) of the norm 
of $b$.

What preconditioner to choose for a given problem is often highly problem-specific, but 
some useful general purpose preconditioners exist, such as the *incomplete Cholesky 
preconditioner* for preconditioned CG (see Chapter 10.3.2 of the Golub & Van Loan text 
given below). Chapter 3 of the [Barrett et al. 
text](https://www.netlib.org/templates/templates.pdf), also cited below, contains 
descriptions of a few more commonly used preconditioners.

### Other Reading

- Golub, G. H. & Van Loan, C. F. Matrix Computations, 3rd Ed. (Johns Hopkins University 
  Press, 1996). Chapter 10 
- Barrett, R., Berry, M., Chan, T. F., Demmel, J., Donato, J., Dongarra, J., ... & Van 
  der Vorst, H. (1994). Templates for the solution of linear systems: building blocks 
  for iterative methods. Society for Industrial and Applied Mathematics.
