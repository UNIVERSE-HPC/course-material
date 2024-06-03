---
name: Scipy.sparse and problems
dependsOn: [
  'scientific_computing.sparse_linear_algebra.03-finite-difference',
]
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

There are seven available sparse matrix types in `scipy.sparse`:

- `csc_matrix`: Compressed Sparse Column format
- `csr_matrix`: Compressed Sparse Row format
- `bsr_matrix`: Block Sparse Row format
- `lil_matrix`: List of Lists format
- `dok_matrix`: Dictionary of Keys format
- `coo_matrix`: COOrdinate format (aka IJV, triplet format)
- `dia_matrix`: DIAgonal format

As indicated by the excellent 
[documentation](https://docs.scipy.org/doc/scipy/reference/sparse.html), the 
`dok_matrix` or `lil_matrix` formats are preferable to construct matrices as they 
support basic slicing and indexing similar to a standard NumPy array.

You will notice that the FD matrix we have constructed for the Poisson problem is 
composed entirely of diagonal elements, as is often the case. If you were constructing a 
similar matrix in MATLAB, you would use the 
[`spdiags`](https://uk.mathworks.com/help/matlab/ref/spdiags.html) function, and 
`scipy.sparse` has its own 
[equivalent](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.spdiags.html). 
However, all the `scipy.sparse` formats also have special methods 
[`setdiag`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.lil_matrix.setdiag.html) 
which provide a more object-orientated method of doing the same thing.

Scipy has a few different direct solvers for sparse matrics, given below:
 
[`spsolve`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.spsolve.html#scipy.sparse.linalg.spsolve): 
This solves $Ax=b$ where $A$ is converted into CSC or CSR form
 
[`spsolve_triangular`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.spsolve_triangular.html#scipy.sparse.linalg.spsolve_triangular): 
Solves $Ax=b$, where $A$ is assumed to be triangular.

 
[`factorized`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.factorized.html#scipy.sparse.linalg.factorized): 
This computes the $LU$ decomposition of the input matrix $A$, returning a Python 
function that can be called to solve $Ax = b$

[`splu`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.splu.html#scipy.sparse.linalg.splu): 
This computes the $LU$ decomposition of the input matrix $A$ using the popular SuperLU 
library. It returns a Python object of class
[`SuperLU`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.SuperLU.html#scipy.sparse.linalg.SuperLU), 
that has a `solve` method you can use to solve $Ax = b$

Note, `scipy.sparse.linalg` also has many iterative solvers, which we will investigate 
further in the next chapter.

### Problems

Your goal for this problem is to construct the FD matrix $A$ given above, using 
`scipy.sparse`, and:

::::challenge{id=scipy-sparse-poisson title="Visualise Poisson Matrix"}

1. Visualise the matrix $A$ using the Matplotlib 
   [`spy`](https://matplotlib.org/3.1.1/api/_as_gen/matplotlib.pyplot.spy.html) plot

:::solution
```python
import scipy.sparse.linalg
import scipy.sparse as sp
import matplotlib.pylab as plt
import numpy as np

N = 100
e = np.ones(N, dtype=float)
A = sp.spdiags([e, -2*e, e], [-1, 0, 1], N, N, format='csc')

plt.spy(A)
plt.show()
```
:::
::::

::::challenge{id=scipy-sparse-poisson-solve title="Solve Poisson problem"}
2. Solve the Poisson problem using:
  - $f(x) = 2 \cos(x) / e^x$, with boundary conditions $g(0) = 0$ and $g(2 \pi)=0$. The 
    analytical solution is  $u_{a}(x) = -\sin(x) / e^x$.
  - $f(x) = 2 \sin(x) / e^x$, with boundary conditions $g(0) = 1$ and $g(2 \pi)=1 / e^{2 
    \pi}$. The analytical solution is  $u_{a}(x) = \cos(x) / e^x$

:::solution
```python
L = 2*np.pi
x = np.linspace(0, L, N+2)
h = x[1] - x[0]
fcos = 2 * np.cos(x) / np.exp(x)
analytical_cos = -np.sin(x) / np.exp(x)
fsin = 2 * np.sin(x) / np.exp(x)
analytical_sin = np.cos(x) / np.exp(x)

# Use sparse lu decomposition, matrix is tridiagonal so no fill-in
splu = sp.linalg.splu(A / h**2)
fig, axs = plt.subplots(1, 2)
axs[0].spy(splu.L)
axs[0].set_title('L')
axs[1].spy(splu.U)
axs[1].set_title('U')
plt.show()

# Use decomposition to solve both problems
for f, a in zip([fcos, fsin], [analytical_cos, analytical_sin]):
    b = f[1:-1]
    b[0] -= a[0] / h**2
    b[-1] -= a[-1] / h**2
    v = splu.solve(b)
    plt.plot(x[1:-1], v, label='solution')
    plt.plot(x, a, label='analytical')
    plt.legend()
    plt.show()
```
:::
::::


::::challenge{id=sparse-versus-dense-mult title="Sparse versus dense: matrix multiplication"}
3. Vary the number of discretisation points $N$ and calculate $AA$ using both sparse and 
   dense matrices. For each $N$ calculate the time to calculate the matix 
   multiplicatiion using Python's 
   [`time.perf_counter`](https://docs.python.org/3/library/time.html#time.perf_counter), 
   and plot execution time versus $N$ for dense and sparse matrix multiplication. 
   Comment on how the time varies with $N$.


:::solution
```python
import time

num = 100
times = np.empty(num, dtype=float)
times_dense = np.empty(num, dtype=float)
times_dense[:] = np.nan
Ns = np.logspace(0.5, 6, num=num, dtype=int)
for i, N in enumerate(Ns):
    e = np.ones(N, dtype=float)
    A = sp.spdiags([e, -2*e, e], [-1, 0, 1], N, N, format='csc')

    t0 = time.perf_counter()
    AA = A @ A
    t1 = time.perf_counter()
    times[i] = t1 - t0

    if N < 2000:
        Adense = A.toarray()
        t0 = time.perf_counter()
        AA = Adense @ Adense
        t1 = time.perf_counter()
        times_dense[i] = t1 - t0

plt.clf()
plt.loglog(Ns, times, label='sparse')
plt.loglog(Ns, times_dense, label='dense')
plt.xlabel('N')
plt.ylabel('time taken')
plt.legend()
plt.show()
```
:::
::::

::::challenge{id=sparse-versus-dense-solve title="Sparse versus dense: solving Poisson problem"}

4. Vary the number of discretisation points $N$ and solve the Poisson problem with 
   varying $N$, and with using both the sparse and direct $LU$ solvers. For each $N$ 
   record the time taken for both the dense and sparse solvers, and record the numerical 
   error $||\mathbf{v} - \mathbf{v}_a||_2$. Generate plots of both error and time versus 
   $N$, and comment on how they vary with $N$

:::solution
```python
times = np.empty(num, dtype=float)
times_dense = np.empty(num, dtype=float)
times_dense[:] = np.nan

for i, N in enumerate(Ns):
    e = np.ones(N, dtype=float)
    A = sp.spdiags([e, -2*e, e], [-1, 0, 1], N, N, format='csc')

    x = np.linspace(0, L, N+2)
    h = x[1] - x[0]
    fcos = 2 * np.cos(x) / np.exp(x)
    analytical_cos = -np.sin(x) / np.exp(x)

    A /= h**2

    b = fcos[1:-1]
    b[0] -= analytical_cos[0] / h**2
    b[-1] -= analytical_cos[-1] / h**2

    t0 = time.perf_counter()
    splu = sp.linalg.splu(A)
    v = splu.solve(b)
    t1 = time.perf_counter()
    times[i] = t1 - t0
    if N < 2000:
        Adense = A.toarray()
        t0 = time.perf_counter()
        lu = scipy.linalg.lu_factor(Adense)
        v = scipy.linalg.lu_solve(lu, b)
        t1 = time.perf_counter()
        times_dense[i] = t1 - t0

plt.clf()
plt.loglog(Ns, times, label='sparse LU')
plt.loglog(Ns, times_dense, label='dense LU')
plt.xlabel('N')
plt.ylabel('time taken')
plt.legend()
plt.show()
```
:::
::::
