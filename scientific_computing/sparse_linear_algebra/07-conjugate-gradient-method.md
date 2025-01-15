---
name: Krylov subspace methods and CG
dependsOn: ["scientific_computing.sparse_linear_algebra.06-jacobi-relaxation-methods"]
tags: []
learningOutcomes:
  - Understand what are Krylov subspace methods, in particular the Conjugate Gradient Method
  - Be able to use the CG, BICGSTAB, GMRES iterative solvers in `scipy`
  - Awareness of the performance characteristics of these iterative solvers, compared with direct solvers such as LU or Cholesky
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

At each iteration $k$ of CG we are concerned with the _residual_, defined as $r_k = b -
A x_k$. If the residual is nonzero, then at each step we wish to find a positive
$\alpha$ such that $\phi(x_k + \alpha p_k) < \phi(x_k)$, where $p_k$ is the _search
direction_ at each $k$. For the classical steepest descent optimisation algorithm the
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
$\mathbf{r}_{k-1}$ that is also _A-conjugate_ to $\mathbf{p}\_1, ...,
\mathbf{p}\_{k-1}$, i.e. $p^T_i A p_j=0$ for all $i \ne j$, which gives the algorithm its
name. After $k$ iterations the sequence of residuals $\mathbf{r}_i$ for $i=1..k$ form a
set of mutually orthogonal vectors that span the Krylov subspace $\mathcal{K}_k$.

Directly using the above equations in an iterative algorithm results in the standard CG
algorithm. A more efficient algorithm can be derived from this by computing the
residuals recursively via $r_k = r\_{k-1} - \alpha_k A p_k$, leading to the final
algorithm given below (reproduced from
[Wikipedia](https://en.wikipedia.org/wiki/Conjugate_gradient_method)):

![Conjugate Gradient algorithm](images/cg_pseudocode.svg)

### Preconditioning

The CG method works well (i.e. converges quickly) if the _condition number_ of the
matrix $A$ is low. The condition number of a matrix gives a measure of how much the
solution $x$ changes in response to a small change in the input $b$, and is a property
of the matrix $A$ itself, so can vary from problem to problem. In order to keep the
number of iterations small for iterative solvers, it is therefore often necessary to use
a _preconditioner_, which is a method of transforming what might be a difficult problem
with a poorly conditioned $A$, into a well conditioned problem that is easy to solve.

Consider the case of preconditioning for the CG methods, we start from the standard
problem $A x = b$, and we wish to solve an _equivalent_ transformed problem given by

$$
\tilde{A} \tilde{x} = \tilde{b}
$$

where $\tilde{A} = C^{-1} A C^{-1}$, $\tilde{x} = Cx$, $\tilde{b} = C^{-1} b $, and $C$
is a symmetric positive matrix.

We then simply apply the standard CG method as given above to this transformed problem.
This leads to an algorithm which is then simplified by instead computing the transformed
quantities $\tilde{p}_k = C p_k$, $\tilde{x}_k = C x_k$, and $\tilde{r}_k = C^{-1} r_k$.
Finally we define a matrix $M = C^2$, which is known as the _preconditioner_, leading to
the final preconditioned CG algorithm given below (reproduced and edited from
[Wikipedia](https://en.wikipedia.org/wiki/Conjugate_gradient_method)):

> $\mathbf{r}\_0 := \mathbf{b} - \mathbf{A x}\_0$ >$\mathbf{z}\_0 := \mathbf{M}^{-1} \mathbf{r}\_0$ >$\mathbf{p}\_0 := \mathbf{z}\_0$
> $k := 0 \, $

**repeat until $|| \mathbf{r}_k ||_2 < \epsilon ||\mathbf{b}||_2$**

> $\alpha\_k := \frac{\mathbf{r}\_k^T \mathbf{z}\_k}{ \mathbf{p}\_k^T
\mathbf{A p}\_k }$ > $\mathbf{x}\_{k+1} := \mathbf{x}\_k + \alpha\_k \mathbf{p}\_k$ > $\mathbf{r}\_{k+1} := \mathbf{r}\_k - \alpha_k \mathbf{A p}\_k$ > **if** $r\_{k+1}$ is sufficiently small exit loop **end if** > $\mathbf{z}\_{k+1} := \mathbf{M}^{-1} \mathbf{r}\_{k+1}$ > $\beta\_k := \frac{\mathbf{r}\_{k+1}^T \mathbf{z}\_{k+1}}{\mathbf{r}\_k^T}$ > $\mathbf{p}\_{k+1} := \mathbf{z}\_{k+1} + \beta_k \mathbf{p}\_k$
> $k := k + 1 \, $

**end repeat**.

The key point to note here is that the preconditioner is used by inverting $M$, so this
matrix must be "easy" to solve in some fashion, and also result in a transformed problem
with better conditioning.

**Termination**: The CG algorithm is normally run until convergence to a given
tolerance which is based on the norm of the input vector $b$. In the algorithm above we
iterate until the residual norm is less than some fraction (set by the user) of the norm
of $b$.

What preconditioner to choose for a given problem is often highly problem-specific, but
some useful general purpose preconditioners exist, such as the _incomplete Cholesky
preconditioner_ for preconditioned CG (see Chapter 10.3.2 of the Golub & Van Loan text
given below). Chapter 3 of the [Barrett et al.
text](https://www.netlib.org/templates/templates.pdf), also cited below, contains
descriptions of a few more commonly used preconditioners.

### Other Reading

- Golub, G. H. & Van Loan, C. F. Matrix Computations, 3rd Ed. (Johns Hopkins University
  Press, 1996). Chapter 10
- Barrett, R., Berry, M., Chan, T. F., Demmel, J., Donato, J., Dongarra, J., ... & Van
  der Vorst, H. (1994). Templates for the solution of linear systems: building blocks
  for iterative methods. Society for Industrial and Applied Mathematics.

### Python: scipy.sparse.linalg

Once again the best resource for Python is the [`scipi.sparse.linalg`
documentation](https://docs.scipy.org/doc/scipy/reference/sparse.linalg.html). The
available iterative solvers in Scipy are:

- [BIConjugate Gradient iteration
  (BiCG)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.bicg.html#scipy.sparse.linalg.bicg)
  - Applicable to non-symmetric problems. Requires the matrix-vector product of $A$
    and its transpose $A^T$.
- [Quasi-Minimal Residual iteration
  (QMR)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.qmr.html#scipy.sparse.linalg.qmr)
  - Applicable to non-symmetric $A$
  - Designed as an improvement of BiCG, avoids one of the two failure situations of
    BiCG
  - Computational costs slightly higher than BiCG, still requires the transpose
    $A^T$.
- [Conjugate Gradient Squared iteration
  (CGS)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.cgs.html#scipy.sparse.linalg.cgs)
  - Applicable to non-symmetric $A$
  - Often converges twice as fast as BiCG, but is often irregular and can diverge if
    starting guess is close to solution.
  - Unlike BiCG, the two matrix-vector products cannot be parallelized.
- [BIConjugate Gradient STABilized iteration
  (BiCGSTAB)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.bicgstab.html#scipy.sparse.linalg.bicgstab)
  - Applicable to non-symmetric $A$
  - Computational cost similar to BiCG, but does not require the transpose of $A$.
  - Simliar convergence speed as CGS, but avoids the irregular convergence properties
    of this method
- [Conjugate Gradient iteration
  (CG)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.cg.html#scipy.sparse.linalg.cg)

  - Applicable only to symmetric positive definite $A$.
  - Speed of convergences depends on condition number

- [Generalized Minimal RESidual iteration
  (GMRES)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.gmres.html#scipy.sparse.linalg.gmres)
  - Applicable non-symmetric $A$
  - Best convergence properties, but each additional iteration becomes increasingly
    expensive, with large storage costs.
  - To limit the increasing cost with additional iterations, it is necessary to
    periodically _restart_ the method. When to do this is highly dependence on the
    properties of $A$
  - Requires only matrix-vector products with $A$
- [LGMRES](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.lgmres.html#scipy.sparse.linalg.lgmres)
  - Modification to GMRES that uses alternating residual vectors to improve
    convergence.
  - It is possible to supply the algorithm with "guess" vectors used to augment the
    Krylov subspace, which is useful if you are solving several very similar
    matrices one after another.
- [MINimum RESidual iteration
  (MINRES)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.minres.html#scipy.sparse.linalg.minres)
  - Applicable to symmetric $A$
- [GCROT(m,k)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.gcrotmk.html#scipy.sparse.linalg.gcrotmk)

`scipy.sparse.linalg` also contains two iterative solvers for least-squares problems,
[`lsqr`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.lsqr.html#scipy.sparse.linalg.lsqr)
and
[`lsmr`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.lsmr.html#scipy.sparse.linalg.lsmr)

### Problems

::::challenge{id=comparing-solvers title="Comparing solvers"}

For this problem we are going to compare many of the different types of solvers, both
direct and iterative, that we've looked at thus far.

Note: We will be using the Finite Difference matrix $A$ based on the two-dimensional
finite difference approximation of the Poisson problem that we developed in the previous
exercise.

For $N=4,8,16,32,64,128$ try the following:

1. Solve the linear systems using $\mathbf{U}_i=A^{-1} \mathbf{f}_i$ (see
   [`scipy.linalg.inv`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.inv.html)
   and record the time this takes on a $\log$-$\log$ graph. (Omit the case $N=128$
   and note this may take a while for $N=64$.)
2. Solve the linear systems using the $\text{LU}$ and Cholesky decompositions. Plot the
   time this takes on the same graph.
3. Now solve the systems iteratively using a conjugate gradients solver (you can use the
   one in `scipy.linalg.sparse`, or you can code up your own). How many iterations
   are needed for each problem? Explain the results for the right-hand-side
   $\mathbf{f}_1$. For the right-hand-side $\mathbf{f}_2$ what is the relationship
   between the number of iterations and $N$. How long do the computations take?
4. Repeat using the `scipy.sparse.linalg` BICGSTAB and GMRES solvers.

:::solution

We redefine our functions from the previous lesson:

```python
import sympy as sp
import numpy as np
def buildf1(N):
    x = np.arange(0, 1, 1/N).reshape(N, 1)
    y = x.T
    f = np.dot(np.sin(np.pi*x), np.sin(np.pi*y))
    return f[1:,1:].reshape(-1,1)
```

```python
def buildf2(N):
    x = np.arange(0, 1, 1/N).reshape(N, 1)
    y = x.T
    f = np.dot(np.maximum(x,1-x), np.maximum(y,1-y))
    return f[1:,1:].reshape(-1, 1)
```

```python
def buildA(N):
    dx = 1 / N
    nvar = (N - 1)**2
    e1 = np.ones((nvar), dtype=float)
    e2 = np.copy(e1)
    e2[::N-1] = 0
    e3 = np.copy(e1)
    e3[N-2::N-1] = 0
    A = sp.spdiags(
        (-e1, -e3, 4*e1, -e2, -e1),
        (-(N-1), -1, 0, 1, N-1), nvar, nvar
    )
    A = A / dx**2
    return A
```

```python
import numpy as np
from matplotlib import pyplot as plt
import time
import scipy
num = 20
times = np.empty((num, 2, 6), dtype=float)
iterations = np.empty((num, 2, 3), dtype=int)
times[:] = np.nan
iterations[:] = np.nan
Ns = np.logspace(0.5, 2.0, num=num, dtype=int)
for j, buildf in enumerate((buildf1, buildf2)):
    for i, N in enumerate(Ns):
        print('doing j=',j,' and N=',N)
        A = buildA(N)
        Adense = A.toarray()
        f = buildf(N)

        t0 = time.perf_counter()
        lu_A = scipy.linalg.lu_factor(Adense)
        x_using_lu = scipy.linalg.lu_solve(lu_A, f)
        t1 = time.perf_counter()
        times[i, j, 0] = t1 - t0
        np.testing.assert_almost_equal(A @ x_using_lu, f)

        t0 = time.perf_counter()
        cho_A = scipy.linalg.cho_factor(A.toarray())
        x_using_cho = scipy.linalg.cho_solve(cho_A, f)
        t1 = time.perf_counter()
        np.testing.assert_almost_equal(A @ x_using_cho, f)
        np.testing.assert_almost_equal(x_using_cho, x_using_lu)
        times[i, j, 1] = t1 - t0

        if N <= 64:
            t0 = time.perf_counter()
            invA = scipy.linalg.inv(Adense)
            x_using_inv = invA @ f
            t1 = time.perf_counter()
            np.testing.assert_almost_equal(x_using_inv, x_using_lu)
            times[i, j, 2] = t1 - t0

        global iters
        def count_iters(x):
            global iters
            iters += 1

        iters = 0
        t0 = time.perf_counter()
        x_using_cg, info = sp.linalg.cg(A, f, callback=count_iters)
        t1 = time.perf_counter()
        np.testing.assert_almost_equal(x_using_cg.reshape(-1, 1), x_using_lu,
                                       decimal=5)
        iterations[i, j, 0] = iters
        times[i, j, 3] = t1 - t0

        iters = 0
        t0 = time.perf_counter()
        x_using_bicgstab, info = sp.linalg.bicgstab(A, f, callback=count_iters)
        t1 = time.perf_counter()
        np.testing.assert_almost_equal(x_using_bicgstab.reshape(-1, 1), x_using_lu,
                                       decimal=5)
        iterations[i, j, 1] = iters
        times[i, j, 4] = t1 - t0

        iters = 0
        t0 = time.perf_counter()
        x_using_gmres, info = sp.linalg.gmres(A, f, callback=count_iters)
        t1 = time.perf_counter()
        np.testing.assert_almost_equal(x_using_gmres.reshape(-1, 1), x_using_lu,
                                       decimal=5)
        iterations[i, j, 2] = iters
        times[i, j, 5] = t1 - t0

plt.loglog(Ns, times[:,0,:], ls='-')
plt.gca().set_prop_cycle(None)
plt.loglog(Ns, times[:,1,:], ls='--')
plt.xlabel('N')
plt.ylabel('time')
plt.legend(['lu', 'cholesky', 'inv', 'cg', 'bicgstab', 'gmres'])
plt.show()

plt.loglog(Ns, iterations[:,0,:], ls='-')
plt.gca().set_prop_cycle(None)
plt.loglog(Ns, iterations[:,1,:], ls='--')
plt.xlabel('N')
plt.ylabel('iterations')
plt.legend(['cg', 'bicgstab', 'gmres'])
plt.show()

# Krylov subspace solvers only take 1 iteration to solve with b = f1 because x is a
# scalar multiple of f. i.e. x is in the k=1 Krylov subspace, and the initial search
# direction will directly lead to x
A = buildA(10)
f = buildf1(10)
np.testing.assert_almost_equal(A@f, 19.57739348*f)
```

:::
::::
