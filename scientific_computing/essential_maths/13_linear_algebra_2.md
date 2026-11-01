---
name: Linear algebra 2
dependsOn: [scientific_computing.essential_maths.12_linear_algebra_1]
tags: []
learningOutcomes:
  - Understand the concept of a singular matrix
  - Understand the concept of the null space for a matrix
  - Be able to find the eigenvalues and eigenvectors of a matrix equation
  - Be able to diagonalise a matrix
attribution:
  - citation: This material has been adapted from material by Fergus Cooper and others from the "Essential Mathematics" module at the Doctoral Training Centre, University of Oxford.
    url: https://www.dtc.ox.ac.uk/
    image: fig/dtc_hex.svg
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

---

## YouTube lecture recording from October 2020

A recording of this lecture from October 2020; the material has changed a little since.

[![Youtube lecture thumbnail](fig/youtube13.jpg)](https://youtu.be/D_aYtsfmBYo)

---

## Eigenvalues and eigenvectors

## Using Python to calculate the inverse

To find $A^{-1}$ for

$$
A = \begin{pmatrix}3&0&2\\ 3&0&-3\\ 0&1&1\end{pmatrix}
$$

### NumPy

```python
import numpy as np
A = np.array([[3, 0, 2], [3, 0, -3], [0, 1, 1]])
np.linalg.inv(A)
```

```text
array([[ 0.2       ,  0.13333333,  0.        ],
       [-0.2       ,  0.2       ,  1.        ],
       [ 0.2       , -0.2       , -0.        ]])
```

### SymPy

```python
import sympy as sp
A = sp.Matrix([[3, 0, 2], [3, 0, -3], [0, 1, 1]])
A.inv()
```

$$
\def\arraystretch{1.8}\left[\begin{matrix}\frac{1}{5} & \frac{2}{15} & 0\\- \frac{1}{5} & \frac{1}{5} & 1\\\frac{1}{5} & - \frac{1}{5} & 0\end{matrix}\right]
$$

## It doesn't always work

Consider the following system

$$
\begin{array}{cccccccc}
eq1: &  x & + & y & + & z & =  & a \\
eq2: & 2x & + & 5y & + &  2z &  = & b \\
eq3: & 7x & +&  10y  & + & 7z &  = &  c
\end{array}
$$

```python nolint
A = np.array([[1, 1, 1], [2, 5, 2], [7, 10, 7]])
np.linalg.inv(A)
```

```text
LinAlgError: Singular matrix
```

## Singular matrices

The **rank** of an $n\times n$ matrix $A$ is the number of linearly independent rows in $A$ (rows not combinations of other rows).

When $\text{rank}(A) < n$ then

- The system $A\mathbf{x} = \mathbf{b}$ has _fewer_ equations than unknowns
- The matrix is said to be singular
- $A$ has no inverse
- The determinant of $A$ is 0
- The equation $A\mathbf{u} = \mathbf{0}$ has non-trivial solutions (solutions where $\mathbf{u} \neq \mathbf{0}$)

## Singular matrix example

An under-determined system (fewer equations than unknowns) may mean that there are **many solutions** or that there are **no solutions**.

An example with many solutions is

$$
\begin{aligned}
x+y &= 1,\\
2x+2y &= 2,
\end{aligned}
$$

has infinitely many solutions (x=0, y=1; x=-89.3, y=90.3...)

An example with no solutions is

$$
\begin{aligned}
x+y &= 1,\\
2x+2y &= 0,
\end{aligned}
$$

where the second equation is inconsistent with the first.

These examples use the matrix

$$
\begin{pmatrix}1&1\\ 2&2\end{pmatrix},
$$

which has rank 1.

## Null space

When a matrix is singular we can find non-trivial solutions to $A\mathbf{u} = \mathbf{0}$.

These are vectors which form the **null space** for $A$.

Any vector in the null space makes no difference to the effect that $A$ is having:

$$
A(\mathbf{x} + \mathbf{u}) = A\mathbf{x} + A\mathbf{u} = A\mathbf{x} + \mathbf{0} = A\mathbf{x}.
$$

Note that any combination or scaling of vectors in the null space is also in the null space.

That is, if $A\mathbf{u} = \mathbf{0}$ and $A\mathbf{v} = \mathbf{0}$ then

$$
A(\alpha\mathbf{u} + \beta\mathbf{v}) = \mathbf{0}
$$

The number of linearly independent vectors in the null space is denoted $\text{null}(A)$ and

$$
\text{null}(A) + \text{rank}(A) = \text{order}(A).
$$

## Null space example

Previous example of a singular system:

$$
A = \left(\begin{matrix} 1& 1& 1\\ 2& 5& 2\\ 7& 10&7 \end{matrix}\right)
$$

```python
A = np.array([[1, 1, 1], [2, 5, 2], [7, 10, 7]])
np.linalg.matrix_rank(A)
```

```text
np.int64(2)
```

```python
import scipy.linalg
scipy.linalg.null_space(A)
```

```text
array([[-7.07106781e-01],
       [-1.11022302e-16],
       [ 7.07106781e-01]])
```

remember, scaled vectors in the null space are also in the null space, for example, $x=1, y=0, z=-1$ is in the null space.

Try it:

$$
\left(\begin{matrix} 1& 1& 1\\ 2& 5& 2\\ 7& 11&7 \end{matrix}\right) \left(\begin{matrix} -1000\\ 0 \\ 1000 \end{matrix}\right) = \quad ?
$$

```python
np.matmul(A,np.array([-1000,0,1000]))
```

```text
array([0, 0, 0])
```

## Eigenvectors: motivation

The **eigenvalues** and **eigenvectors** give an indication of how much effect the matrix has, and in what direction.

$$
\begin{aligned}
A&=\left(\begin{matrix} \cos(45)&-\sin(45)\\ \sin(45)&\cos(45)\end{matrix}\right) &&\text{has no scaling effect.} \\
B&=\left(\begin{matrix} 2& 0 \\ 0&\frac{1}{2}\end{matrix}\right) &&\text{doubles in } x \text{, but halves in } y\text{.}
\end{aligned}
$$

Repeated applications of $A$ stay the same distance from the origin, but repeated applications of $B$ move towards $(\infty, 0).$

- Transitions with probability
- Markov chains
- Designing bridges
- Solution of systems of linear ODEs
- Stability of systems of nonlinear ODEs

## Eigenvectors and eigenvalues

$A$ is a matrix, $\mathbf{v}$ is a **non-zero** vector, $\lambda$ is a scalar,

If:

$$
A \mathbf{v} = \lambda \mathbf{v}
$$

then $\mathbf{v}$ is called an **eigenvector** and $\lambda$ is the corresponding **eigenvalue**.

Note that if $\mathbf{v}$ is a solution, then so is a scaling $a\mathbf{v}$:

$$
A (a \mathbf{v}) = \lambda (a \mathbf{v}).
$$

## Finding eigenvalues

Another way to write previous equation:

$$
\begin{aligned}
A \mathbf{v} &= \lambda \mathbf{v},\\
A \mathbf{v} - \lambda I \mathbf{v} &= \mathbf{0},\\
(A - \lambda I) \mathbf{v} &= \mathbf{0}.
\end{aligned}
$$

Consider the equation:

$$
B\mathbf{x}=\mathbf{0}
$$

where $B$ is a matrix and $\mathbf{x}$ is a non-zero column vector.

Assume $B$ is nonsingular:

$$
B^{-1}(B\mathbf{x})=B^{-1}\mathbf{0}=\mathbf{0}
$$

But:

$$
B^{-1}(B\mathbf{x})=(B^{-1}B)\mathbf{x}=I\mathbf{x}=\mathbf{x}
$$

This means that:

$$
B^{-1}(B\mathbf{x})=\mathbf{0}=\mathbf{x}
$$

but we stated above that $\mathbf{x}\neq\mathbf{0}$ so our assumption that $B$ is nonsingular must be false: $B$ is singular.

We can state that:

$$
(A-\lambda I)\mathbf{v} = \mathbf{0} \quad \text{with} \quad \mathbf{v}\neq \mathbf{0},
$$

so $(A-\lambda I)$ must be singular:

$$
|A-\lambda I|=0.
$$

## Example

What are the eigenvalues for this matrix?

$$
A=\left(\begin{matrix}-2&-2\\ 1&-5\end{matrix}\right)
$$

$$
\begin{aligned}
|A-\lambda I| &=\left\vert\begin{matrix}-2-\lambda&-2\\ 1&-5-\lambda\end{matrix}\right\vert=(-2-\lambda)(-5-\lambda)-(-2) \\
&=10+5\lambda+\lambda^2+2\lambda+2=\lambda^2+7\lambda+12=(\lambda+3)(\lambda+4)=0
\end{aligned}
$$

So the eigenvalues are $\lambda_1=-3$ and $\lambda_2=-4$.

## Eigenvalues using Python

Numpy:

```python
A = np.array([[-2, -2], [1, -5]])
np.linalg.eig(A)[0]
```

```text
array([-3.+0.j, -4.+0.j])
```

SymPy:

```python
A2 = sp.Matrix([[-2, -2], [1, -5]])
A2.eigenvals()
```

$$
\left\{ -4 : 1, \  -3 : 1\right\}
$$

## Finding eigenvectors

For an eigenvalue, the corresponding vector comes from substitution into $A \mathbf{v} = \lambda \mathbf{v}$:

### Example

What are the eigenvectors for

$$
A=\left(\begin{matrix}-2&-2\\ 1&-5\end{matrix}\right)?
$$

The eigenvalues are $\lambda_1=-3$ and $\lambda_2=-4.$
The eigenvectors are $\mathbf{v}_1$ and $\mathbf{v}_2$ where:

$$
A\mathbf{v}_1=\lambda_1 \mathbf{v}_1.
$$

$$
\begin{aligned}
\left(\begin{matrix}-2&-2\\ 1&-5\end{matrix}\right) \left(\begin{matrix}u_1\\ v_1\end{matrix}\right) &= \left(\begin{matrix}-3u_1\\ -3v_1\end{matrix}\right),\\
u_1 &= 2v_1. \quad \text{(from the top or bottom equation)}\\
\left(\begin{matrix}u_1\\ v_1\end{matrix}\right) &= \left(\begin{matrix}2 \\ 1\end{matrix}\right), \left(\begin{matrix}1 \\ 0.5\end{matrix}\right), \left(\begin{matrix}-4.4 \\ -2.2\end{matrix}\right), \left(\begin{matrix}2\alpha \\ \alpha\end{matrix}\right)\ldots
\end{aligned}
$$

## Eigenvectors in Python

Numpy:

```python
A = np.array([[-2, -2], [1, -5]])
np.linalg.eig(A)[1]
```

```text
array([[0.89442719+0.j, 0.70710678+0.j],
       [0.4472136 +0.j, 0.70710678+0.j]])
```

SymPy:

```python
c = sp.symbols('c')
A2 = sp.Matrix([[-2, c], [1, -5]])
A2.eigenvects()
```

$$
\def\arraystretch{1.8}\left[ \left( - \frac{\sqrt{4 c + 9}}{2} - \frac{7}{2}, \  1, \  \left[ \left[\begin{matrix}\frac{3}{2} - \frac{\sqrt{4 c + 9}}{2}\\1\end{matrix}\right]\right]\right), \  \left( \frac{\sqrt{4 c + 9}}{2} - \frac{7}{2}, \  1, \  \left[ \left[\begin{matrix}\frac{\sqrt{4 c + 9}}{2} + \frac{3}{2}\\1\end{matrix}\right]\right]\right)\right]
$$

## Diagonalising matrices

Any nonsingular matrix $A$ can be rewritten as a product of eigenvectors and eigenvalues.

If $A$ has eigenvalues $\lambda_1$ and $\lambda_2$ with corresponding eigenvectors $(u_1, v_1)^T$ and $(u_2, v_2)^T$ then

$$
A =
\left(\begin{matrix}u_1 & u_2\\v_1 & v_2\end{matrix}\right)
\left(\begin{matrix}\lambda_1 & 0\\0 & \lambda_2\end{matrix}\right)
\left(\begin{matrix}u_1 & u_2\\v_1 & v_2\end{matrix}\right)^{-1}.
$$

This is like a scaling surrounded by rotations and separates how much effect the matrix has $(\lambda_i)$ from the directions $(\mathbf{v}_i).$

## For example

$$
A=\left(\begin{matrix}-2&-2\\ 1&-5\end{matrix}\right)
$$

```python
A = np.array([[-2, -2], [1, -5]])
w, v = np.linalg.eig(A)
inv_v = np.linalg.inv(v)

np.matmul( np.matmul(v, np.diag(w)) , inv_v )
```

```text
array([[-2.+0.j, -2.+0.j],
       [ 1.+0.j, -5.+0.j]])
```

## Orthogonal eigenvectors

If $\mathbf{x}_1$ and $\mathbf{x}_2$ are perpendicular or **orthogonal** vectors, then the scalar/dot product is zero.

$$
\mathbf{x}_1.\mathbf{x}_2=0
$$

e.g.

$$
\mathbf{x}_1.\mathbf{x}_2=\left(\begin{matrix}2\\ -1\\ 3\end{matrix}\right).\left(\begin{matrix}2\\ 7\\ 1\end{matrix}\right)=2\times 2+(-1\times 7)+(3\times1)=4-7+3=0.
$$

Since this dot product is zero, the vectors $\mathbf{x}_1$ and $\mathbf{x}_2$ are orthogonal.

## Symmetric matricies

Symmetric matricies have orthogonal eigenvectors, e.g.

$$
A=\left(\begin{matrix}19&20&-16\\ 20&13&4 \\ -16&4&31\end{matrix}\right)
$$

```python
A = np.array([[19, 20, -16], [20, 13, 4], [-16, 4, 31]])
w, v = np.linalg.eig(A)
print(v)
print('\ndot products of eigenvectors:\n')
print(np.dot(v[:,0],v[:,1]))
print(np.dot(v[:,0],v[:,2]))
print(np.dot(v[:,1],v[:,2]))
```

```text
[[ 0.66666667+0.j -0.66666667+0.j  0.33333333+0.j]
 [-0.66666667+0.j -0.33333333+0.j  0.66666667+0.j]
 [ 0.33333333+0.j  0.66666667+0.j  0.66666667+0.j]]

dot products of eigenvectors:

(-1.942890293094024e-16+0j)
(1.3877787807814457e-16+0j)
(1.1102230246251565e-16+0j)
```

## Normalised eigenvectors

If

$$
\left(\begin{matrix}x\\ y\\ z\end{matrix}\right),
$$

is an eigenvector, then

$$
\def\arraystretch{2}\left(\begin{matrix}\frac{x}{\sqrt{x^2+y^2+z^2}}\\ \frac{y}{\sqrt{x^2+y^2+z^2} }\\ \frac{z}{\sqrt{x^2+y^2+z^2} }\end{matrix}\right)
$$

is the corresponding normalised vector: a vector of unit length (magnitude).

## Orthogonal matrices

A matrix is orthogonal if its columns are normalised orthogonal vectors.
One can prove that if $M$ is orthogonal then:

$$
M^TM=I\qquad M^T=M^{-1}
$$

Note that if the eigenvectors are written in orthogonal form then the diagonalising equation
is simplified:

$$
\begin{aligned}
A &= \left(\begin{matrix}u_1 & u_2\\v_1 & v_2\end{matrix}\right) \left(\begin{matrix}\lambda_1 & 0\\0 & \lambda_2\end{matrix}\right) \left(\begin{matrix}u_1 & u_2\\v_1 & v_2\end{matrix}\right)^{-1} \\
&= \left(\begin{matrix}u_1 & u_2\\v_1 & v_2\end{matrix}\right) \left(\begin{matrix}\lambda_1 & 0\\0 & \lambda_2\end{matrix}\right) \left(\begin{matrix}u_1 & v_1\\u_2 & v_2\end{matrix}\right).
\end{aligned}
$$

## Summary

- Matrix representation of simultaneous equations
- Matrix-vector and matrix-matrix multiplication
- Determinant, inverse and transpose
- Null space of singular matrices
- Finding eigenvalues and eigenvectors
- Python for solving systems, finding inverse, null space and eigenvalues/vectors
- Diagonalising matrices (we will use this for systems of differential equations)

## Introductory problems

:::callout{variant="tip"}
For more practice, an AI tool can write new problems like the ones below.
Ask it for problems of the same type and difficulty, or for problems set in your own field.
For example, you could ask it to "write five physics problems on eigenvalues and eigenvectors, at the level of these ones".
Work them on paper first, then ask the tool to check your answers and to explain any step you are unsure of.
:::

::::challenge{id="13_intro_01" title="Introductory problems 1"}
Given

$$
A = \left(\begin{array}{cc} 1 & 0 \\ 0 & i \end{array}\right); \qquad
B = \left(\begin{array}{cc} 0 & i \\ i & 0 \end{array}\right); \qquad
C = \left(\begin{array}{cc} \frac{1}{\sqrt{2}} & \frac{1}{2}\left(1-i\right) \\\frac{1}{2}\left(1+i\right) & -\frac{1}{\sqrt{2}} \end{array}\right);
$$

verify by hand, and using the `numpy.linalg` module, that

1. $AA^{-1}=A^{-1}A =I$;
1. $BB^{-1}=B^{-1}B =I$;
1. $CC^{-1}=C^{-1}C =I$;

```python
# hint
import numpy as np

# In Python the imaginary unit is "1j"
A = np.array([[1, 0], [0, 1j]])

print(A @ np.linalg.inv(A))
```

:::solution

By hand, we use the formula for the inverse of a $2\times 2$ matrix:

$$
\left(\begin{array}{cc} a & b \\ c & d \end{array}\right)^{-1} = \frac{1}{ad-bc}\left(\begin{array}{cc} d & -b \\ -c & a \end{array}\right)
$$

1. $|A| = 1\times i - 0 = i$, so

   $$
   A^{-1} = \frac{1}{i}\left(\begin{array}{cc} i & 0 \\ 0 & 1 \end{array}\right) = \left(\begin{array}{cc} 1 & 0 \\ 0 & -i \end{array}\right)
   $$

   since $1/i = -i$.
   Multiplying out, $AA^{-1}$ and $A^{-1}A$ are both diagonal, with diagonal entries $1\times 1 = 1$ and $i\times(-i) = 1$, so they equal $I$.

1. $|B| = 0 - i\times i = 1$, so

   $$
   B^{-1} = \left(\begin{array}{cc} 0 & -i \\ -i & 0 \end{array}\right)
   $$

   Multiplying out,

   $$
   BB^{-1} = \left(\begin{array}{cc} 0 + i\times(-i) & 0 \\ 0 & i\times(-i) + 0 \end{array}\right) = \left(\begin{array}{cc} 1 & 0 \\ 0 & 1 \end{array}\right)
   $$

   and $B^{-1}B = I$ in the same way.

1. $|C| = -\frac{1}{2} - \frac{1}{4}(1-i)(1+i) = -\frac{1}{2} - \frac{2}{4} = -1$, so

   $$
   C^{-1} = -\left(\begin{array}{cc} -\frac{1}{\sqrt{2}} & -\frac{1}{2}(1-i) \\ -\frac{1}{2}(1+i) & \frac{1}{\sqrt{2}} \end{array}\right) = C
   $$

   So, $C$ is its own inverse, and we need to check that $CC = I$.
   The top-left entry is $\frac{1}{2} + \frac{1}{4}(1-i)(1+i) = \frac{1}{2} + \frac{1}{2} = 1$, and the top-right entry is $\frac{1}{\sqrt{2}}\cdot\frac{1}{2}(1-i) - \frac{1}{2}(1-i)\cdot\frac{1}{\sqrt{2}} = 0$.
   The bottom row works in the same way, so $CC = I$.

In Python, note that `*` multiplies matrices element by element, so we use `@` for matrix multiplication:

```python
import numpy as np

A = np.array([[1, 0], [0, 1j]])
B = np.array([[0, 1j], [1j, 0]])
C = np.array([[1 / np.sqrt(2), (1 - 1j) / 2], [(1 + 1j) / 2, -1 / np.sqrt(2)]])

for M in [A, B, C]:
    print(M @ np.linalg.inv(M))
    print(np.linalg.inv(M) @ M)
```

```text
[[1.+0.j 0.+0.j]
 [0.+0.j 1.+0.j]]
[[1.+0.j 0.+0.j]
 [0.+0.j 1.+0.j]]
[[1.+0.j 0.+0.j]
 [0.+0.j 1.+0.j]]
[[1.+0.j 0.+0.j]
 [0.+0.j 1.+0.j]]
[[ 1.00000000e+00+0.00000000e+00j -5.55111512e-17+5.55111512e-17j]
 [-2.29934717e-17+0.00000000e+00j  1.00000000e+00+0.00000000e+00j]]
[[1.00000000e+00+0.00000000e+00j 5.55111512e-17-5.55111512e-17j]
 [0.00000000e+00+0.00000000e+00j 1.00000000e+00+0.00000000e+00j]]
```

For $C$, the off-diagonal entries are about $10^{-17}$ rather than exactly 0.
This is rounding error, because the computer stores numbers such as $1/\sqrt{2}$ to only about 16 significant figures.
`np.allclose(C @ np.linalg.inv(C), np.eye(2))` checks that the result equals $I$ to within this rounding.

:::

::::

::::challenge{id="13_intro_02" title="Introductory problems 2"}
Let A be an $n \times n$ invertible matrix. Let $I$ be the $n\times n$ identity matrix and let $B$ be an $n\times n$ matrix.

Suppose that $ABA^{-1}=I$.
Determine the matrix $B$ in terms of the matrix $A$.

:::solution

Multiply both sides on the right by $A$:

$$
ABA^{-1}A = IA \quad\implies\quad AB = A
$$

Then multiply both sides on the left by $A^{-1}$:

$$
A^{-1}AB = A^{-1}A \quad\implies\quad B = I
$$

So, $B$ is the identity matrix, whatever the matrix $A$.

:::

::::

## Main problems

::::challenge{id="13_main_01" title="Main problems 1"}
Let $A$ be the coefficient matrix of the system of linear equations

$$
\begin{aligned}
-x_1 - 2 x_2 &= 1,\\
2 x_1 + 3 x_2 &= -1.
\end{aligned}
$$

1. Solve the system by finding the inverse matrix $A^{-1}$.
1. Let $\mathbf{x} = (x_1, x_2)^T$ be the solution of the system obtained in part **1.**
   Calculate and simplify $A^{2017} \mathbf{x}$.

:::solution

1. The system is $A\mathbf{x} = \mathbf{b}$, with

   $$
   A = \left(\begin{array}{cc} -1 & -2 \\ 2 & 3 \end{array}\right) \quad\text{and}\quad \mathbf{b} = \left(\begin{array}{c} 1 \\ -1 \end{array}\right)
   $$

   $|A| = -3 + 4 = 1$, so

   $$
   A^{-1} = \left(\begin{array}{cc} 3 & 2 \\ -2 & -1 \end{array}\right) \quad\text{and}\quad \mathbf{x} = A^{-1}\mathbf{b} = \left(\begin{array}{c} 3 - 2 \\ -2 + 1 \end{array}\right) = \left(\begin{array}{c} 1 \\ -1 \end{array}\right)
   $$

1. Multiplying out,

   $$
   A\mathbf{x} = \left(\begin{array}{cc} -1 & -2 \\ 2 & 3 \end{array}\right)\left(\begin{array}{c} 1 \\ -1 \end{array}\right) = \left(\begin{array}{c} 1 \\ -1 \end{array}\right) = \mathbf{x}
   $$

   So, multiplying $\mathbf{x}$ by $A$ leaves it unchanged, and doing this 2017 times still leaves it unchanged:

   $$
   A^{2017}\mathbf{x} = \mathbf{x} = \left(\begin{array}{c} 1 \\ -1 \end{array}\right)
   $$

   In other words, $\mathbf{x}$ is an eigenvector of $A$ with eigenvalue 1.

:::

::::

::::challenge{id="13_main_02" title="Main problems 2"}
For each of the following matrices

$$
\begin{gathered}
A = \left(\begin{array}{cc} 2 & 3 \\ 1 & 4 \end{array}\right); \qquad
B = \left(\begin{array}{cc} 4 & 2 \\ 6 & 8 \end{array}\right); \\
C = \left(\begin{array}{cc} 1 & 4 \\ 1 & 1 \end{array}\right); \qquad
D = \left(\begin{array}{cc} x & 0 \\ 0 & y \end{array}\right),
\end{gathered}
$$

compute the determinant, eigenvalues and eigenvectors by hand.

Check your results by verifying that $Q\mathbf{x} = \lambda_i \mathbf{x}$, where $Q=A$, $B$, $C$ or $D$, and by using the `numpy.linalg` module.

```python
# hint
import numpy as np

A = np.array([[2, 3], [1, 4]])

e_vals, e_vecs = np.linalg.eig(A)

print(e_vals)
print(e_vecs)
```

:::solution

In each case we solve $|Q - \lambda I| = 0$ for the eigenvalues, then substitute each one into $(Q - \lambda I)\mathbf{v} = \mathbf{0}$ to find an eigenvector $\mathbf{v} = (u, v)^T$.
Any multiple of an eigenvector is also an eigenvector.

- **$A$:** $|A| = 8 - 3 = 5$, and

  $$
  |A - \lambda I| = (2-\lambda)(4-\lambda) - 3 = \lambda^2 - 6\lambda + 5 = (\lambda - 1)(\lambda - 5) = 0
  $$

  For $\lambda_1 = 1$, $u + 3v = 0$, so $\mathbf{v}_1 = (-3, 1)^T$.
  For $\lambda_2 = 5$, $-3u + 3v = 0$, so $\mathbf{v}_2 = (1, 1)^T$.
  Check: $A(1, 1)^T = (5, 5)^T = 5(1, 1)^T$.

- **$B$:** $|B| = 32 - 12 = 20$, and

  $$
  \begin{aligned}
  |B - \lambda I| &= (4-\lambda)(8-\lambda) - 12 \\
  &= \lambda^2 - 12\lambda + 20 \\
  &= (\lambda - 2)(\lambda - 10) = 0
  \end{aligned}
  $$

  For $\lambda_1 = 2$, $2u + 2v = 0$, so $\mathbf{v}_1 = (-1, 1)^T$.
  For $\lambda_2 = 10$, $-6u + 2v = 0$, so $\mathbf{v}_2 = (1, 3)^T$.
  Check: $B(1, 3)^T = (10, 30)^T = 10(1, 3)^T$.

- **$C$:** $|C| = 1 - 4 = -3$, and

  $$
  |C - \lambda I| = (1-\lambda)^2 - 4 = \lambda^2 - 2\lambda - 3 = (\lambda - 3)(\lambda + 1) = 0
  $$

  For $\lambda_1 = 3$, $-2u + 4v = 0$, so $\mathbf{v}_1 = (2, 1)^T$.
  For $\lambda_2 = -1$, $2u + 4v = 0$, so $\mathbf{v}_2 = (-2, 1)^T$.
  Check: $C(-2, 1)^T = (2, -1)^T = -1\times(-2, 1)^T$.

- **$D$:** $|D| = xy$.
  $D$ is diagonal, so $|D - \lambda I| = (x - \lambda)(y - \lambda) = 0$, and the eigenvalues are its diagonal entries, $\lambda_1 = x$ and $\lambda_2 = y$.
  The eigenvectors are $\mathbf{v}_1 = (1, 0)^T$ and $\mathbf{v}_2 = (0, 1)^T$, since $D(1, 0)^T = (x, 0)^T$ and $D(0, 1)^T = (0, y)^T$.
  (If $x = y$, every vector is an eigenvector.)

In Python:

```python
import numpy as np

for Q in [np.array([[2, 3], [1, 4]]), np.array([[4, 2], [6, 8]]), np.array([[1, 4], [1, 1]])]:
    e_vals, e_vecs = np.linalg.eig(Q)
    print(np.linalg.det(Q))
    print(e_vals)
    print(e_vecs)
```

```text
5.000000000000001
[1.+0.j 5.+0.j]
[[-0.9486833 +0.j -0.70710678+0.j]
 [ 0.31622777+0.j -0.70710678+0.j]]
19.999999999999996
[ 2.+0.j 10.+0.j]
[[-0.70710678+0.j -0.31622777+0.j]
 [ 0.70710678+0.j -0.9486833 +0.j]]
-3.0000000000000004
[ 3.+0.j -1.+0.j]
[[ 0.89442719+0.j -0.89442719+0.j]
 [ 0.4472136 +0.j  0.4472136 +0.j]]
```

The eigenvalues agree with ours (the `+0.j` shows that their imaginary parts are zero).
The eigenvectors are the _columns_ of `e_vecs`, and NumPy scales each one to have length 1, so they are multiples of ours.
For example, the first column for $A$ is $(-0.949, 0.316)^T = (-3, 1)^T/\sqrt{10}$, and the second is $(-0.707, -0.707)^T = -(1, 1)^T/\sqrt{2}$.
The determinants are not exact whole numbers because of rounding error.

:::

::::

::::challenge{id="13_main_03" title="Main problems 3"}
Orthogonal vectors.

Two vectors $\mathbf{x_1}$ and $\mathbf{x_2}$ are said to be perpendicular or _orthogonal_ if their dot/scalar product is zero:

$$
\mathbf{x_1}.\mathbf{x_2}=\left(\begin{array}{c} 2 \\ -1 \\ 3 \end{array} \right).\left(\begin{array}{c} 2 \\ 7 \\ 1 \end{array} \right)=(2{\times}2)+(-1{\times}7)+(3{\times}1)=4-7+3=0,
$$

thus $\mathbf{x_1}$ and $\mathbf{x_2}$ are orthogonal.

Find vectors that are orthogonal to

$$
\left(\begin{array}{c} 1 \\ 2 \end{array} \right) \quad\text{and}\quad \left(\begin{array}{c} 1 \\ 2 \\ -1 \end{array} \right).
$$

How many such vectors are there?

:::solution

A vector $(a, b)^T$ is orthogonal to $(1, 2)^T$ when

$$
a + 2b = 0, \quad\text{so}\quad a = -2b
$$

So, any vector of the form $t(-2, 1)^T$ is orthogonal to $(1, 2)^T$, for any number $t \neq 0$.
There are infinitely many such vectors, but they all lie along one line.

A vector $(a, b, c)^T$ is orthogonal to $(1, 2, -1)^T$ when

$$
a + 2b - c = 0, \quad\text{so}\quad c = a + 2b
$$

Here we can choose both $a$ and $b$, so the vectors are

$$
\left(\begin{array}{c} a \\ b \\ a + 2b \end{array}\right) = a\left(\begin{array}{c} 1 \\ 0 \\ 1 \end{array}\right) + b\left(\begin{array}{c} 0 \\ 1 \\ 2 \end{array}\right)
$$

for example $(1, 0, 1)^T$, $(0, 1, 2)^T$ or $(-2, 1, 0)^T$.
Again there are infinitely many, but this time they fill a whole plane (the plane through the origin perpendicular to $(1, 2, -1)^T$).

:::

::::

::::challenge{id="13_main_04" title="Main problems 4"}
Diagonalise the $2 \times 2$ matrix

$$
A=\left(\begin{array}{cc} 2 & -1 \\ -1 & 2 \end{array} \right)
$$

by finding a non-singular matrix $S$ and a diagonal matrix $D$ such that $A = SDS^{-1}$.

:::solution

First we find the eigenvalues:

$$
|A - \lambda I| = (2-\lambda)^2 - 1 = \lambda^2 - 4\lambda + 3 = (\lambda - 1)(\lambda - 3) = 0
$$

For $\lambda_1 = 1$, $(A - I)\mathbf{v} = \mathbf{0}$ gives $u - v = 0$, so $\mathbf{v}_1 = (1, 1)^T$.
For $\lambda_2 = 3$, $(A - 3I)\mathbf{v} = \mathbf{0}$ gives $-u - v = 0$, so $\mathbf{v}_2 = (1, -1)^T$.

$S$ has the eigenvectors as its columns, and $D$ has the eigenvalues on its diagonal, in the same order:

$$
S = \left(\begin{array}{cc} 1 & 1 \\ 1 & -1 \end{array}\right), \qquad
D = \left(\begin{array}{cc} 1 & 0 \\ 0 & 3 \end{array}\right), \qquad
S^{-1} = \frac{1}{2}\left(\begin{array}{cc} 1 & 1 \\ 1 & -1 \end{array}\right)
$$

We can check this by multiplying out:

$$
SDS^{-1} = \left(\begin{array}{cc} 1 & 3 \\ 1 & -3 \end{array}\right)\frac{1}{2}\left(\begin{array}{cc} 1 & 1 \\ 1 & -1 \end{array}\right) = \frac{1}{2}\left(\begin{array}{cc} 4 & -2 \\ -2 & 4 \end{array}\right) = A
$$

(Other answers are possible: the eigenvectors can be scaled, or listed in the other order, as long as $D$ matches.)

:::

::::

::::challenge{id="13_main_05" title="Main problems 5"}
Find a $2{\times}2$ matrix $A$ such that

$$
A\left(\begin{array}{c} 2 \\ 1 \end{array} \right) = \left(\begin{array}{c} -1 \\ 4 \end{array} \right)\quad\text{and}\quad A \left(\begin{array}{c} 5 \\ 3\end{array} \right)=\left(\begin{array}{c} 0 \\ 2\end{array} \right).
$$

:::solution

Put the two vectors side by side as the columns of one matrix, and the two results as the columns of another.
Then both conditions together say that

$$
A\left(\begin{array}{cc} 2 & 5 \\ 1 & 3 \end{array}\right) = \left(\begin{array}{cc} -1 & 0 \\ 4 & 2 \end{array}\right)
$$

The determinant of the left-hand matrix is $6 - 5 = 1$, so its inverse is

$$
\left(\begin{array}{cc} 2 & 5 \\ 1 & 3 \end{array}\right)^{-1} = \left(\begin{array}{cc} 3 & -5 \\ -1 & 2 \end{array}\right)
$$

Multiplying both sides on the right by this inverse,

$$
A = \left(\begin{array}{cc} -1 & 0 \\ 4 & 2 \end{array}\right)\left(\begin{array}{cc} 3 & -5 \\ -1 & 2 \end{array}\right) = \left(\begin{array}{cc} -3 & 5 \\ 10 & -16 \end{array}\right)
$$

Check: $A(2, 1)^T = (-6 + 5, 20 - 16)^T = (-1, 4)^T$ and $A(5, 3)^T = (-15 + 15, 50 - 48)^T = (0, 2)^T$.

:::

::::

## Extension problems

::::challenge{id="13_ext_01" title="Extension problems 1"}
If there exists a matrix $M$ whose columns are those of normalised (unit length) and orthogonal vectors, prove that $M^TM=I$ which implies that $M^T=M^{-1}$.

:::solution

Let $M$ be an $n\times n$ matrix with columns $\mathbf{c}_1, \mathbf{c}_2, \ldots, \mathbf{c}_n$.
The rows of $M^T$ are then $\mathbf{c}_1^T, \mathbf{c}_2^T, \ldots, \mathbf{c}_n^T$.
The entry in row $i$ and column $j$ of $M^TM$ is row $i$ of $M^T$ times column $j$ of $M$, which is the dot product $\mathbf{c}_i\cdot\mathbf{c}_j$:

$$
M^TM = \left(\begin{array}{cccc}
\mathbf{c}_1\cdot\mathbf{c}_1 & \mathbf{c}_1\cdot\mathbf{c}_2 & \cdots & \mathbf{c}_1\cdot\mathbf{c}_n \\
\mathbf{c}_2\cdot\mathbf{c}_1 & \mathbf{c}_2\cdot\mathbf{c}_2 & \cdots & \mathbf{c}_2\cdot\mathbf{c}_n \\
\vdots & \vdots & \ddots & \vdots \\
\mathbf{c}_n\cdot\mathbf{c}_1 & \mathbf{c}_n\cdot\mathbf{c}_2 & \cdots & \mathbf{c}_n\cdot\mathbf{c}_n
\end{array}\right)
$$

The columns have unit length, so $\mathbf{c}_i\cdot\mathbf{c}_i = 1$, and each pair is orthogonal, so $\mathbf{c}_i\cdot\mathbf{c}_j = 0$ for $i\neq j$.
So, every diagonal entry is 1 and every other entry is 0, which means that $M^TM = I$.

By definition, $M^{-1}$ is the matrix that multiplies $M$ to give $I$, so $M^T = M^{-1}$.
(For a square matrix, if $M^TM = I$ then $MM^T = I$ too.)

:::

::::
