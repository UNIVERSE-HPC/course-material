---
name: Jacobi and Relaxation Methods
dependsOn: ["scientific_computing.sparse_linear_algebra.04-scipy-sparse"]
tags: []
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

## Iterative Methods

Previously we have discussed _direct_ linear algebra solvers based on decompositions of
the original matrix $A$. The amount of computational effort required to achieve these
decomposisions is $\mathcal{O}(n^3)$, where $n$ is the number of rows of a square
matrix. They are therefore unsuitable for the large, sparse systems of equations that
are typically encountered in scientific applications. An alternate class of linear
algebra solvers are the _iterative_ methods, which produce a series of _approximate_
solutions $x_k$ to the $A x = b$ problem. The performance of each algorithm is then
based on how quickly, or how many iterations $k$ are required, for the solution $x_k$ to
converge to within a set tolerance of the true solution $x$.

## Jacobi Method

The Jacobi method is the simplest of the iterative methods, and relies on the fact that
the matrix is _diagonally dominant_. Starting from the problem definition:

$$
A\mathbf{x} = \mathbf{b}
$$

we decompose $A$ in to $A = L + D + U$, where $L$ is lower triangular, $D$ is diagonal,
$U$ is upper triangular.

$$
A\mathbf{x} = L\mathbf{x} + D\mathbf{x} + U\mathbf{x} =  \mathbf{b}
$$

We then assume that we have an initial guess at the solution $\mathbf{x}^0$, and try to
find a new estimate $\mathbf{x}^1$. Assuming that the diagonal $D$ dominates over $L$
and $U$, a sensible choice would be to insert $x^0$ and the unknown $x^1$ into the
equation like so:

$$
L\mathbf{x}^0 + D\mathbf{x}^1 + U\mathbf{x}^0 =  \mathbf{b}
$$

we can rearrange to get an equation for $x^1$. This is easily solved as we can take the
inverse of the diagonal matrix by simply inverting each diagonal element individually:

$$
D\mathbf{x}_1 =  \mathbf{b} - (L+U)\mathbf{x}_0
$$

Thus we end up with the general Jacobi iteration:

$$
\mathbf{x}_{k+1} =  D^{-1}(\mathbf{b} - (L+U)\mathbf{x}_k)
$$

## Relaxation methods

The Jacobi method is an example of a relaxation method, where the matrix $A$ is split
into a dominant part $M$ (which is easy to solve), and the remainder $N$. That is, $A =
M - N$

$$
M\mathbf{x}_{k+1} = N\mathbf{x}_k + \mathbf{b}
$$

$$
\mathbf{x}_{k+1} = M^{-1}N\mathbf{x}_k + M^{-1}\mathbf{b}
$$

This can be rearranged in terms of the _residual_ $\mathbf{r}_k = \mathbf{b} - A
\mathbf{x}_k$ to the update equation

$$
\mathbf{x}_{k+1} = \mathbf{x}_{k} + M^{-1}\mathbf{r}_k
$$

For the Jacobi method $M = D$ and $N = -(L + U)$. Other relaxation methods include
Gauss-Seidel, where $M = (D + L)$ and $N = -U$, and successive over-relaxation (SOR),
where $M = \frac{1}{\omega} D + L$ and $N = -(\frac{\omega - 1}{\omega} D + U)$, where
$\omega$ is the _relaxation_ parameter that is within the range $0 \le \omega \le 2$.

For any relaxation method to converge we need $\rho(M^{-1}N) < 1$, where $\rho()$ is the
_spectral radius_ of $M^{-1} N$, which is defined as the largest eigenvalue $\lambda$ of
a a given matrix $G$:

$$
\rho(G) = \max{|\lambda|: \lambda \in \lambda(G)}
$$

For the SOR method, the relaxation parameter $\omega$ is generally chosen to minimise
$\rho(M^{-1}N)$, so that the speed of convergence is maximised. In some cases this
optimal $\omega$ is known, for example for finite difference discretisation of the
[Poisson equation](https://www.sciencedirect.com/science/article/pii/S0893965908001523).
However, in many cases sophisticated eigenvalue analysis is required to determine the
optimal $\omega$.

### Other Reading

- Golub, G. H. & Van Loan, C. F. Matrix Computations, 3rd Ed. (Johns Hopkins University
  Press, 1996). Chapter 10
- Barrett, R., Berry, M., Chan, T. F., Demmel, J., Donato, J., Dongarra, J., ... & Van
  der Vorst, H. (1994). Templates for the solution of linear systems: building blocks
  for iterative methods. Society for Industrial and Applied Mathematics.

### Problems

::::challenge{id=2d-poisson-jacobi-relaxation title="2D Poisson Jacobi Relaxation"}

This exercise involves the manipulation and solution of the linear system resulting from
the finite difference solution to Poisson's equation in _two_ dimensions. Let $A$ be a
sparse symmetric positive definite matrix of dimension $(N-1)^2 \times (N-1)^2$ created
using `scipy.sparse` (for a given $N$) by the function
`buildA` as follows:

```python
import numpy as np
import scipy.sparse as sp

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

and let $\mathbf{f}_1$ and $\mathbf{f}_2$ be the vectors defined in
`buildf1` and `buildf2`

```python
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

We will consider manipulation of the matrix $A$ and solution of the linear
systems $A\mathbf{U}_i=\mathbf{f}_i$. The solution to this linear system
corresponds to a finite difference solution to Poisson's equation $-\nabla^2 u
= f$ on the unit square with zero Dirichlet boundary conditions where $f$ is
either $\sin(\pi x) \sin (\pi y)$ or $\max(x,1-x) \max(y,1-y)$. PDEs of this type occur
(usually with some additional reaction and or convection terms) very frequently
in mathematical modelling of physiological processes, and even in image
analysis.

- Write a function to solve a linear system using the Jacobi method. In
  terms of $N$, how many iterations does it take to converge? (Try
  $N=4,8,16,32,64$.)

:::solution

```python
import numpy as np
import scipy.sparse as sp
import scipy.sparse.linalg
import scipy.optimize
import matplotlib.pylab as plt

def jacobi(A, b, x0=None, tol=1e-5, max_iter=1000):
    if x0 is None:
        x0 = np.zeros_like(b)
    x = np.copy(x0)
    b_norm = np.linalg.norm(b)

    # jacobi method: M = D
    M = A.diagonal().reshape(-1, 1)
    invM = 1/M

    # main relaxation iteration
    for i in range(max_iter):
        r = b - A @ x
        error = np.linalg.norm(r) / b_norm
        if error < tol:
            break
        x += invM * r
    return x, i

num = 20
iterations = np.empty((num, 2), dtype=int)
iterations[:] = np.nan
Ns = np.logspace(0.5, 1.5, num=num, dtype=int)
for j, buildf in enumerate((buildf1, buildf2)):
    for i, N in enumerate(Ns):
        A = buildA(N)
        f = buildf(N)
        max_iter = 10*N
        x, iters = jacobi(A, f, max_iter=max_iter)
        if iters < max_iter:
            iterations[i, j] = iters

plt.plot(Ns, iterations)
plt.xlabel('N')
plt.ylabel('iterations')
plt.show()
```

:::
::::

::::challenge{id=2d-poisson-sor-relaxation title="2D Poisson SOR Relaxation"}

- Write a function to solve a linear system using the SOR method.
  For $N=64$ and right-hand-side $\mathbf{f}_2$ determine numerically the best choice of the relaxation parameter t2 decimal places and compare this with theory.
  Hint, use
  [`scipy.optimize.minimize_scalar`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.minimize_scalar.html#scipy.optimize.minimize_scalar)

:::solution

```python
def SOR(A, b, omega, x0=None, tol=1e-5, max_iter=300):
    if x0 is None:
        x0 = np.zeros_like(b)
    x = np.copy(x0)
    b_norm = np.linalg.norm(b)

    # SOR method
    D = sp.spdiags((A.diagonal()), (0), *A.shape)
    L = sp.tril(A, k=-1)
    M = (1/omega) * D + L

    # main relaxation iteration
    for i in range(max_iter):
        r = b - A @ x
        error = np.linalg.norm(r) / b_norm
        if error < tol:
            break
        x += sp.linalg.spsolve_triangular(M, r)
    return x, i


N = 64
A = buildA(N)
f = buildf2(N)

def SOR_iterations(omega):
    x, i = SOR(A, f, omega, max_iter=10, tol=1e-32)
    return np.linalg.norm(A @ x - f)

res = scipy.optimize.minimize_scalar(SOR_iterations, bracket=[0.1, 1.0, 1.99], tol=1e-2)
print('ideal omega is', res.x, 'versus analytic value of', 2 / (1 + np.sin(np.pi/N)))
```

:::
::::
