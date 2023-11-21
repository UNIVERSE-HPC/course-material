---
name: Iterative solvers in Scipy
dependsOn: [
  'scientific_computing.sparse_linear_algebra.07-conjugate-gradient-method',
]
tags: []
---

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
    periodically *restart* the method. When to do this is highly dependence on the 
    properties of $A$
  - Requires only matrix-vector products with $A$
- 
  [LGMRES](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.lgmres.html#scipy.sparse.linalg.lgmres)
  - Modification to GMRES that uses alternating residual vectors to improve 
    convergence.
  - It is possible to supply the algorithm with "guess" vectors used to augment the 
    Krylov subspace, which is useful if you are solving several very similar 
    matrices one after another.
- [MINimum RESidual iteration 
  (MINRES)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.minres.html#scipy.sparse.linalg.minres)
  - Applicable to symmetric $A$
- 
[GCROT(m,k)](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.gcrotmk.html#scipy.sparse.linalg.gcrotmk)

`scipy.sparse.linalg` also contains two iterative solvers for least-squares problems, 
[`lsqr`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.lsqr.html#scipy.sparse.linalg.lsqr) 
and 
[`lsmr`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.linalg.lsmr.html#scipy.sparse.linalg.lsmr)

### Problems

{{% notice question %}}
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
{{% /notice %}}


{{% expand "Expand for solution" %}}
{{% notice solution %}}
```python
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
{{% /notice %}}
{{% /expand %}}
