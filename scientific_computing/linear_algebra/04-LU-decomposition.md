---
name: LU decomposition
dependsOn: [
    'scientific_computing.linear_algebra.02-gaussian-elimination',
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

---

## Matrix decompositions

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

## $LU$ decomposition

The $LU$ decomposition is closely related to gaussian elimination. It takes the original 
equation to be solved $A x = b$ and splits it up into two separate equations involving a 
unit lower triangular matrix $L$, and the row echelon matrix $U$:

$$
\begin{aligned}
L y &= b \\
U x &= y
\end{aligned}
$$

where $A = LU$. The $L$ matrix is a *unit* lower triangular matrix and thus has ones on 
the diagonal, whereas $U$ is in row echelon form with pivot values in the leading 
coefficients of each row.

$$
A = \left( \begin{matrix}
1 & 0 & 0 & 0 \\ 
\ast & 1 & 0 & 0 \\
\ast & \ast & 1 & 0 \\
\ast & \ast & \ast & 1 \end{matrix}\right)
\left(\begin{matrix}
p_1 & \ast & \ast & \ast \\
0 & p_2 & \ast & \ast \\
0 & 0 & p_3 & \ast \\
0 & 0 & 0 & p_4
\end{matrix}\right)
$$

Thus, we have converted our original problem of solving $A x = b$ into two separate 
solves, first solving the equation $L y = b$, and then using the result $y$ to solve $U 
x = y$. 

$$
\begin{aligned}
\left(\begin{matrix}
1 & 0 & 0 & 0 \\ \ast & 1 & 0 & 0 \\
\ast & \ast & 1 & 0 \\
\ast & \ast & \ast & 1 
\end{matrix}\right)
\left(\begin{matrix}
y_1 \\ 
y_2 \\
y_3 \\
y_4 
\end{matrix}\right)
&= \left(\begin{matrix}
b_1 \\ 
b_2 \\
b_3 \\
b_4 
\end{matrix}\right)
\\
\left(\begin{matrix}
p_1 & \ast & \ast & \ast \\ 
0 & p_2 & \ast & \ast \\
0 & 0 & p_3 & \ast \\
0 & 0 & 0 & p_4 
\end{matrix}\right)
\left(\begin{matrix}
x_1 \\ 
x_2 \\
x_3 \\
x_4 
\end{matrix}\right)
&= \left(\begin{matrix}
y_1 \\ 
y_2 \\
y_3 \\
y_4 
\end{matrix}\right)
\end{aligned}
$$


However, each of those solves is very cheap to compute, in this case for the 4x4 matrix 
shown above the solution of $L y = b$ only needs 6 multiplication and 6 additions, 
whereas $U x = y$ requires 4 divisions, 6 multiplications and 6 additions, leading to a 
total of 28 arithmetic operations, much fewer in comparison with the 62 operations 
required to solve the original equation $A x = b$. In general, $LU$ decomposition for an 
$n \times n$ matrix takes about $2 n^3 / 3$ flops, or floating point operations, to 
compute.


## $LU$ factorisation without pivoting

A relativly simple $LU$ algorithm can be described if we assume that no pivoting is 
required during a gaussian elimination. In this case, the gaussian elimination process 
is a sequence of $p$ linear operations $E_1, E_2, ..., E_p$, with each operation $E_i$ 
being a row replacement that adds a multiple of one row to another below it (i.e. $E_i$ 
is lower triangular). The final matrix after applying the sequence of row reductions is 
$U$ in row echelon form, that is:

$$
E_p \cdots E_2 E_1 A = U
$$

Since we have $A = LU$, we can show that the sequence of operations $E_1, E_2, ..., E_p$ 
is also the sequence that reduces the matrix $L$ to an identity matrix:

$$
A = (E_p \cdots E_2 E_1)^{-1} U = LU,
$$

therefore, 

$$
L = (E_p \cdots E_2 E_1)^{-1},
$$

and,

$$
(E_p \cdots E_2 E_1) L = (E_p \cdots E_2 E_1) (E_p \cdots E_2 E_1)^{-1} = I
$$


This implies how we can build up the matrix $L$. We choose values for $L$ such that the 
series of row operations $E_1, E_2, ..., E_p$ convert the matrix $L$ to the identity 
matrix. Since each $E_i$ is lower triangular, we know that both $(E_p \cdots E_2 E_1)$ 
and $(E_p \cdots E_2 E_1)^{-1}$ are also lower triangular.

For example, consider the following matrix

$$
A = \left(\begin{matrix}
3 & 2 & 1 & -3 \\ 
-6 & -2 & 1 & 5 \\
3 & -4 & -7 & 2 \\
-9 & -6 & -1 & 15 
\end{matrix}\right)
$$

After three row reductions, $R_2 \mathrel{{+}{=}} 2 R_1$, $R_3 \mathrel{{+}{=}} -1 R_1$, 
and $R_3 \mathrel{{+}{=}} 3 R_1$, we have the following result:

$$
E_1 E_2 E_3 A = \left(\begin{matrix}
3 & 2 & 1 & -3 \\ 
0 & 2 & * & * \\
0 & -6 & * & * \\
0 & 0 & * & * 
\end{matrix}\right)
$$

To build the 1st column of $L$, we simply divide the 1st column of $A$ by the pivot 
value 3, giving

$$
L = \left(\begin{matrix}
1  & 0 & 0 & 0 \\ 
-2 & 1 & 0 & 0 \\
1  & * & 1 & 0 \\
-3 & * & * & 1 
\end{matrix}\right)
$$

For the next column we do the same, using the new pivot value $A_{2,2} = 2$ in row 2 to 
reduce $A_{3,2}$ and $A_{4,2}$ to zero, and then dividing the column vector under the 
pivot $(-6, 0)^T$ by the pivot value 2 to obtain the next column of $L$.

Repeating this process for all the columns in $A$, we obtain the final factorisation. 
You can verify for yourself that repeating the same row operations we did to form $U$ to 
the matrix $L$ reduces it to the identity matrix.

$$
L = \left(\begin{matrix}
1 & 0 & 0 & 0 \\ 
-2 & 1 & 0 & 0 \\
1 & -3 & 1 & 0 \\
-3 & 0 & 2 & 1 
\end{matrix}\right)
$$

$$
E_1 E_2 ... E_p A = U = \left(\begin{matrix}
3 & 2 & 1 & -3 \\ 
0 & 2 & 3 & -1 \\
0 & 0 & 1 & 2 \\
0 & 0 & 0 & 2 
\end{matrix}\right)
$$

## Pivoting

Of course, for any practial $LU$ factorisation we need to consider pivoting. Any matrix 
$A$ can be factorised into $PLU$, where $P$ is a permutation matrix, and $L$ and $U$ are 
defined as before. During the gaussian elimination steps we store an array of row 
indices $p_i$ indicating that row $i$ is interchanged with row $p_i$, and the resultant 
array of $p_i$ can be used to build the permutation matrix $P$ (It would be wasteful to 
store the entire martix $P$ so the array $p_i$ is stored instead). 

Thus, the LU algorithm proceeds as follows:

1. Begin with the left-most column $i=0$, find an appropriate pivot (e.g. maximum entry 
   in the column) and designate this row as the pivot row. Interchange this row with row 
   $i$, and store the pivot row index as $p_i$. Use row replacements to create zeros 
   below the pivot. Create the corresponding column for $L$ by dividing by the pivot 
   value.
2. Continue along to the next column $i$, again choosing a pivot row $p_i$, 
   interchanging it with row $i$ and creating zeros below the pivot, creating the new 
   column in $L$, and making sure to record which pivot row has been chosen for each 
   column. Repeat this step for all the columns of the matrix.
3. Once the last column has been done, $U$ should be in row echlon form and $L$ should 
   be a unit lower triangular matrix. The array $p_i$ implicitly defines the permutation 
   matrix $P$

In practice, most library implementation store $L$ and $U$ in the same matrix since they 
are lower and upper triangular respectivly.

## $LDL$ decomposition

It is often very benificial when solving linear systems to consider and take advantage 
of any special structure that the matrix $A$ might possesses. The $LDL$ decomposition is 
a varient on LU decomposition which is only applicable to a symmetric matrix $A$ (i.e. 
$A = A^T$). The advantage of using this decomposition is that it takes advantage of the 
redundent entries in the matrix to reduce the amount of computation to $n^3/3$, which is 
about a half that required for the $LU$ decomposition.

## Other Reading

- Linear algebra and its applications by David C. Lay. Chaper 2.5  
- Golub, G. H. & Van Loan, C. F. Matrix Computations, 3rd Ed. (Johns Hopkins University 
  Press, 1996). Chapter 3.2 & 4.1
- https://en.wikipedia.org/wiki/LU_decomposition
- https://en.wikipedia.org/wiki/Cholesky_decomposition#LDL_decomposition_2

## Software

[`scipy.linalg.lu_factor`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.lu_factor.html).

[`scipy.linalg.lu_solve`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.lu_solve.html).

[`scipy.linalg.lu`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.lu.html).

[`scipy.linalg.ldl`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.ldl.html#scipy.linalg.ldl)

## Problems

::::challenge{id=lu-decomposition title="LU decomposition"}

Take your gaussian elimination code that you wrote in the previous lesson and use it to 
write an LU decomposition function that takes in a martix $A$, and returns $L$, $U$ and 
the array $p_i$. You can check your answer using 
[`scipy.linalg.lu_factor`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.lu_factor.html). 
Hint: the permutation matrix is tricky to construct, so you might want to use the test 
given in the documentation for `lu_factor`.

:::solution

```python
import numpy as np
import matplotlib.pylab as plt
import scipy
import scipy.linalg
import sys

def lu_decomposition(A):
    m, n = A.shape

    LU = np.copy(A)
    pivots = np.empty(n, dtype=int)
    # initialise the pivot row and column
    h = 0
    k = 0
    while h < m and k < n:
        # Find the k-th pivot:
        pivots[k] = np.argmax(LU[h:, k]) + h
        if LU[pivots[k], k] == 0:
            # No pivot in this column, pass to next column
            k = k+1
        else:
            # swap rows
            LU[[h, pivots[k]], :] = LU[[pivots[k], h], :]
            # Do for all rows below pivot:
            for i in range(h+1, m):
                f = LU[i, k] / LU[h, k]
                # Store f as the new L column values
                LU[i, k] = f
                # Do for all remaining elements in current row:
                for j in range(k + 1, n):
                    LU[i, j] = LU[i, j] - LU[h, j] * f
            # Increase pivot row and column
            h = h + 1
            k = k + 1
    return LU, pivots

def random_matrix(n):
    R = np.random.rand(n, n)
    A = np.zeros((n, n))
    triu = np.triu_indices(n)
    A[triu] = R[triu]
    return A

def random_non_singular_matrix(n):
    A = np.random.rand(n, n)
    while np.linalg.cond(A) > 1/sys.float_info.epsilon:
        A = np.random.rand(n, n)
    return A

As = [
    np.array([[1, 0, 0], [0, 1, 0], [0, 0, 1]]),
    random_non_singular_matrix(3),
    random_non_singular_matrix(4),
    random_non_singular_matrix(5),
    random_non_singular_matrix(6),
]

def pivots_to_row_indices(pivots):
    n = len(pivots)
    indices = np.array(range(0, n))
    for i, p in enumerate(pivots):
        indices[i], indices[p] = indices[p], indices[i]
    return indices

def calculate_L_mult_U(LU):
    L = np.tril(LU)
    np.fill_diagonal(L, 1)
    U = np.triu(LU)
    return L @ U

for A in As:
    LU_scipy, pivots_scipy = scipy.linalg.lu_factor(A)
    row_indices_scipy = pivots_to_row_indices(pivots_scipy)
    LU_mine, pivots_mine = lu_decomposition(A)
    row_indices_mine = pivots_to_row_indices(pivots_mine)

    np.testing.assert_almost_equal(calculate_L_mult_U(LU_scipy),  A[row_indices_scipy])
    np.testing.assert_almost_equal(calculate_L_mult_U(LU_mine),  A[row_indices_mine])
```
:::
::::
