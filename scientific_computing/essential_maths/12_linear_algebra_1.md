---
name: Linear algebra 1
dependsOn: [scientific_computing.essential_maths.08_complex_numbers]
tags: []
learningOutcomes:
  - Understand the concept of a matrix
  - Be able to perform addition and subtraction of matrices
  - Be able to multiply a matrix by a scalar
  - Understand the associative, commutative and distributive laws of matrices
  - Be able to find the determinant of a matrix
  - Be able to invert a matrix
  - Be able to find the transpose of a matrix
  - Be able to express a set of simultaneous equations as a matrix equation
  - Understand how to solve a matrix equation $Ax=b$
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

[![Youtube lecture thumbnail](fig/youtube12.jpg)](https://youtu.be/L2K3dnZ8Mk8)

---

## Introduction to matrices

## Simultaneous equations

Consider 2 simultaneous equations:

$$
\begin{aligned}
a_1x+b_1y &= c_1, &&\text{(1)}\\
a_2x+b_2y &= c_2, &&\text{(2)}
\end{aligned}
$$

where the values $x$ and $y$ are to be found, and $a_1$, $b_1$, $a_2$, $b_2$, $c_1$ and $c_2$ are given constants.

$$
\begin{aligned}
(1) \times b_2: &\qquad b_2a_1x+b_2b_1y = b_2c_1, &&\text{(3)}\\
(2) \times b_1: &\qquad b_1a_2x+b_1b_2y = b_1c_2, &&\text{(4)}\\
(3) - (4): &\qquad b_2a_1x-b_1a_2x = b_2c_1-b_1c_2.
\end{aligned}
$$

Thus:

$$
x=\frac{b_2c_1-b_1c_2}{b_2a_1-b_1a_2},
$$

and similarly:

$$
y=\frac{a_1c_2-a_2c_1}{a_1b_2-a_2b_1}.
$$

This works, provided that $a_1b_2-a_2b_1\neq 0.$

While the algebraic manipulation is straightforward when solving two equations, it can get really messy when solving large systems.

What we want is a way to be able to easily manipulate **linear** systems, regardless of how big they are.

## The matrix

Matrices are a structure that allow us to more easily manipulate linear systems.

Consider the original system

$$
\begin{aligned}
a_1x+b_1y &= c_1, \\
a_2x+b_2y &= c_2.
\end{aligned}
$$

We rewrite this, in the form of a matrix as:

$$
\left(\begin{matrix}a_1&b_1\\ a_2&b_2\end{matrix}\right)
\left(\begin{matrix}x\\y\end{matrix}\right)
=\left(\begin{matrix}c_1\\ c_2 \end{matrix}\right).
$$

Think about how this form relates to the original linear system.

### What is a matrix?

A matrix is an array of numbers such as:

$$
\left(\begin{matrix}a&b&c\\ d&e&f\\ g&h&i\end{matrix}\right)
$$

- $3\times3$ is the **size** of the matrix.

- A $3\times3$ matrix is said to be **square** and have **order** (dimension) 3.

### Addition, subtraction, and scalar multiplication

We can add or subtract two matrices as long as they have the **same** size:

$$
\left(\begin{matrix} 2&1 \\ 3&-4 \end{matrix}\right) +\left(\begin{matrix} 6&-5 \\ 1&-7 \end{matrix}\right)= \left(\begin{matrix} 8&-4 \\ 4&-11\end{matrix}\right)
$$

We can multiply a matrix by a scalar, by multiplying every element:

$$
5\times\left(\begin{matrix} 2&1\\ 3&-4 \end{matrix}\right)=\left(\begin{matrix}10&5\\ 15&-20\end{matrix}\right)
$$

### Matrix multiplication

To multiply two matrices, we multiply each **row** in the first matrix by each **column** in the second one, and put the results into a new matrix.

A row and column are multiplied by summing up each element in the row, multiplied by the corresponding element in the column.

$$
\left(\begin{matrix} 1&2 \\ 3&4 \end{matrix}\right) \left(\begin{matrix} 5&6\\7&8\end{matrix}\right) = \left(\begin{matrix} 1 \times 5 + 2 \times 7 & 1 \times 6 + 2 \times 8 \\ 3 \times 5 + 4 \times 7 & 3 \times 6 + 4\times 8\end{matrix}\right) = \left(\begin{matrix} 19&22\\43&46\end{matrix}\right)
$$

$$
\left(\begin{matrix} 1&2&3&4\\ 5&6&7&8 \end{matrix}\right) \left(\begin{matrix} 1&2&3\\ 4&5&6\\ 7&8&9\\ 10&11&12 \end{matrix}\right) = \left(\begin{matrix} 70&80&90\\ 158&184&210 \end{matrix}\right)
$$

$$
\left(\begin{matrix} a & b & c \end{matrix}\right)
\left(\begin{matrix} p & q \\ r & s \\ v & w \end{matrix}\right)
= \left(\begin{matrix} ap+br+cv & aq+bs+cw \end{matrix}\right).
$$

If the number of **columns** in the first matrix doesn't match the number of **rows** in the second, they **cannot** be multiplied.

$$
\left(\begin{matrix} 2 & 3 & 1 \\ 2 & -1 & 3\end{matrix}\right)\left(\begin{matrix} 1 & 0 \\ -1 & -4\end{matrix}\right) = ???
$$

### Matrix multiplication is not commutative

This means that $A \times B$ is not the same as $B \times A$.
This can be easily seen from the fact that multiplying different sized matrices doesn't always work:

$$
\begin{aligned}
(3\times 2 \text{ matrix}) \times (2\times 2 \text{ matrix}) &= (3\times 2 \text{ matrix}) \\
(2\times 2 \text{ matrix}) \times (3\times 2 \text{ matrix}) &= ???
\end{aligned}
$$

However, even when sizes match, the product is usually not the same.

### The identity matrix

$I$ is the identity matrix, which has the property that:

$$
A I = I A = A
$$

for a square matrix $A$. It is the matrix equivalent of multiplying by 1.

The 2x2 identity matrix is:

$$
I_2 = \left(\begin{matrix} 1 & 0 \\ 0 & 1 \end{matrix}\right).
$$

The 3x3 identity matrix is:

$$
I_3 = \left(\begin{matrix} 1 & 0 & 0 \\ 0 & 1 & 0 \\ 0 & 0 & 1 \end{matrix}\right).
$$

and so on for higher dimensions.

## Inverting matrices

### The determinant

If

$$
A = \left(\begin{matrix} p & q \\ r & s\end{matrix}\right)
$$

then the **determinant** of A is:

$$
|A| = ps-qr
$$

That is, (top left $\times$ bottom right) - (top right $\times$ bottom left).

If $|A| = 0$, A is said to be **singular** (and it has no inverse).

### Inverting 2x2 matrices

If $AB = I$, what is B?

A is called the inverse of B, and vice versa. I.e.

$$
A = B^{-1}, \quad B = A^{-1}.
$$

If

$$
A = \left(\begin{matrix} p & q \\ r & s\end{matrix}\right)
$$

then:

$$
A^{-1} = \frac{1}{ps-qr} \left(\begin{matrix} s & -q \\ -r & p\end{matrix}\right).
$$

### Example of inverting a 2x2 matrix

Let us take a matrix

$$
A=\left(\begin{matrix}2&-3\\ -2&4\end{matrix}\right).
$$

First, calculate its determinant:

$$
|A|=(2\times 4)-(-3\times-2)=8-6=2.
$$

Then:

$$
A^{-1}=\frac{1}{2}\left(\begin{matrix}4&3\\ 2&2\end{matrix}\right)
$$

As a check, calculate $A^{-1}A$:

$$
\begin{aligned}
A^{-1}A &= \frac{1}{2}\left(\begin{matrix}4&3\\ 2&2\end{matrix}\right)\left(\begin{matrix}2&-3\\ -2&4\end{matrix}\right) \\
&= \frac{1}{2}\left(\begin{matrix}2&0\\ 0&2\end{matrix}\right) \\
&= \left(\begin{matrix}1&0\\ 0&1\end{matrix}\right) \\
&=I_2.
\end{aligned}
$$

## The transpose of a matrix

$A^T$ is the transpose of $A$.

Swap elements across the leading diagonal so that $A^T_{ij}= A_{ji}$.

$$
\begin{aligned}
A &=\left(\begin{matrix}2&1&2\\ 1&4&6\\ 1&-1&2\end{matrix}\right) \\
A^T &=\left(\begin{matrix}2&1&1\\ 1&4&-1\\ 2&6&2\end{matrix}\right)
\end{aligned}
$$

## Solving a linear system using matrices

To solve a matrix system $A \mathbf{x} = \mathbf{b}$ for an unknown left-hand side $\mathbf{x}$.

- If it's of order 2 then use the formula to write $A^{-1}$ and hence $\mathbf{x} = A^{-1}\mathbf{b}$.

- If it's larger $(3\times3)$ then there's still a formula for $A^{-1}$ (not in this course).

- Use an analytical method (Gaussian elimination) to find the inverse (not in this course).

- Use a numerical scheme to find an approximation to $\mathbf{x}$, such as Newton's method (not in this course).

- Solve using linear algebra software (e.g. in Python, which we will see shortly).

### Example of solving a 2x2 linear system

$$
A^{-1}A\mathbf{x}=A^{-1}\mathbf{b}
$$

and

$$
\mathbf{x}=A^{-1}\mathbf{b}
$$

Let's take an example:

$$
\begin{aligned}
x+5y &= 11, \\
-x+5y &= 9
\end{aligned}
$$

In matrix form, this gives:

$$
\left(\begin{matrix}1 &5\\ -1&5\end{matrix}\right) \left(\begin{matrix}x\\ y\end{matrix}\right) = \left(\begin{matrix}11\\ 9\end{matrix}\right)
$$

We have:

$$
A^{-1}= \frac{1}{10} \left(\begin{matrix}5 &-5\\ 1&1 \end{matrix}\right)
$$

Thus:

$$
\left(\begin{matrix}x\\ y\end{matrix}\right) = \frac{1}{10}\left(\begin{matrix}5 &-5\\ 1&1\end{matrix}\right)\left(\begin{matrix}11\\ 9\end{matrix}\right) =\frac{1}{10} \left(\begin{matrix}10\\ 20\end{matrix}\right) =\left(\begin{matrix}1\\ 2\end{matrix}\right)
$$

And $x=1, y=2$

This process seems like more effort than its worth for small systems.
But it allows for a much more systematic approach when dealing with large systems.

As the size of the matrix grows, this process can be easily performed with Python (or other tools).

### Example: solving a 4x4 system in Python

$$
\begin{aligned}
x + 5y + 3z - w &= 5, \\
x - 2y + z + 4w &= 2, \\
-3x + y - z + 2w &= -5, \\
x + y + z &= 0.
\end{aligned}
$$

In matrix form, this gives:

$$
\left(\begin{matrix} 1 & 5 & 3 & -1 \\ 1 & -2 & 1 & 4 \\ -3 & 1 & -1 & 2\\ 1 & 1 & 1 & 0 \end{matrix}\right) \left(\begin{matrix} x \\ y \\ z \\ w\end{matrix}\right) = \left(\begin{matrix} 5 \\ 2 \\ -5 \\ 0\end{matrix}\right).
$$

#### Numerically, using NumPy

```python
# In python, we use numpy arrays to store the needed matrices
# the procedure linalg.solve, solves the system Ax = b
# We could also calculate the inverse of A (linalg.inv), and then multiply.
# But this is faster
import numpy as np
A = np.array([[1,5,3,-1],[1,-2,1,4],[-3,1,-1,2],[1,1,1,0]])

b = np.array([5, 2, -5, 0])

x = np.linalg.solve(A,b)

print(x)

print(np.matmul(A,x))
```

```text
[-5.94444444 -5.11111111 11.05555556 -3.33333333]
[ 5.0000000e+00  2.0000000e+00 -5.0000000e+00 -8.8817842e-16]
```

#### Symbolically, using SymPy

```python
import sympy as sp
A = sp.Matrix([[1,5,3,-1],[1,-2,1,4],[-3,1,-1,2],[1,1,1,0]])

A.inv() * sp.Matrix([5, 2, -5, 0])

```

$$
\def\arraystretch{1.8}\left[\begin{matrix}- \frac{107}{18}\\- \frac{46}{9}\\\frac{199}{18}\\- \frac{10}{3}\end{matrix}\right]
$$

## Introductory problems

:::callout{variant="tip"}
For more practice, an AI tool can write new problems like the ones below.
Ask it for problems of the same type and difficulty, or for problems set in your own field.
For example, you could ask it to "write five engineering problems on matrices and simultaneous equations, at the level of these ones".
Work them on paper first, then ask the tool to check your answers and to explain any step you are unsure of.
:::

::::challenge{id="12_intro_01" title="Introductory problems 1"}
Write the following system of equations:

$$
\begin{aligned}
x + y + z &= 7\\
2x - y + z &= 2\\
x - 2y + 2z &= 5
\end{aligned}
$$

in the form

1. $A\mathbf{x} = \mathbf{b}$
1. $\mathbf{y}B = \mathbf{c}$

Where $A$ and $B$ are $3\times3$ matrices, and $\mathbf{x}$, $\mathbf{y}$, $\mathbf{b}$ and $\mathbf{c}$ are vectors whose size and shape you should carefully indicate.

Check that your answers make sense by expanding your expressions to ensure you get back to the original equations.

:::solution

1. The coefficients of each equation form a row of $A$, and the unknowns form a column vector:

   $$
   \begin{pmatrix} 1 & 1 & 1 \\ 2 & -1 & 1 \\ 1 & -2 & 2 \end{pmatrix}
   \begin{pmatrix} x \\ y \\ z \end{pmatrix} =
   \begin{pmatrix} 7 \\ 2 \\ 5 \end{pmatrix}
   $$

   $A$ is $3\times 3$, and $\mathbf{x}$ and $\mathbf{b}$ are $3\times 1$ column vectors.
   Multiplying out, the first row of $A$ times $\mathbf{x}$ gives $x + y + z$, which must equal the first entry of $\mathbf{b}$, 7, and so on.

1. Now the unknowns form a row vector on the left, so the coefficients of each equation form a _column_ of $B$.
   This means that $B = A^T$:

   $$
   \begin{pmatrix} x & y & z \end{pmatrix}
   \begin{pmatrix} 1 & 2 & 1 \\ 1 & -1 & -2 \\ 1 & 1 & 2 \end{pmatrix} =
   \begin{pmatrix} 7 & 2 & 5 \end{pmatrix}
   $$

   $B$ is $3\times 3$, and $\mathbf{y}$ and $\mathbf{c}$ are $1\times 3$ row vectors.
   Multiplying out, $\mathbf{y}$ times the first column of $B$ gives $x + y + z$, which must equal the first entry of $\mathbf{c}$, 7, and so on.

:::

::::

::::challenge{id="12_intro_02" title="Introductory problems 2"}

Given

$$
\begin{gathered}
A = \begin{pmatrix} 2 & 1 \\ 3 & 4 \end{pmatrix}, \quad
B = \begin{pmatrix} 1 & 4 \\ 7 & 2 \end{pmatrix}, \quad
C = \begin{pmatrix} 3 & -1 \\ -5 & 2 \end{pmatrix}, \\
D = \begin{pmatrix} 1 \\ 3 \end{pmatrix}, \quad
E = \begin{pmatrix} 2 & -1 \end{pmatrix}
\end{gathered}
$$

1. Write down $a_{21}$, $b_{12}$ and $c_{22}$
1. Calculate:
   - $A + A$
   - $A - B$
   - $4C$
1. Calculate, where possible, or explain why the product is not defined:
   - $AB$
   - $CD$
   - $DC$
   - $EE$
1. Do A and B commute? Do A and C commute?
1. Does $(AB)C = A(BC)$? Does this either prove or disprove that matrix multiplication is associative?
1. Does $AC + BC = (A+B)C$? Does this either prove or disprove the distributive property of matrices?

:::solution

1. $a_{21}$ is the entry in row 2, column 1 of $A$, so $a_{21} = 3$.
   Similarly, $b_{12} = 4$ and $c_{22} = 2$.

1. Add, subtract and multiply entry by entry:

   $$
   A + A = \begin{pmatrix} 4 & 2 \\ 6 & 8 \end{pmatrix}, \qquad
   A - B = \begin{pmatrix} 1 & -3 \\ -4 & 2 \end{pmatrix}, \qquad
   4C = \begin{pmatrix} 12 & -4 \\ -20 & 8 \end{pmatrix}
   $$

1. A product is defined only when the number of columns of the first matrix equals the number of rows of the second.

   $$
   \begin{aligned}
   AB &= \begin{pmatrix} 2\times 1 + 1\times 7 & 2\times 4 + 1\times 2 \\ 3\times 1 + 4\times 7 & 3\times 4 + 4\times 2 \end{pmatrix} = \begin{pmatrix} 9 & 10 \\ 31 & 20 \end{pmatrix}, \\
   CD &= \begin{pmatrix} 3 - 3 \\ -5 + 6 \end{pmatrix} = \begin{pmatrix} 0 \\ 1 \end{pmatrix}
   \end{aligned}
   $$

   $DC$ is not defined, because $D$ is $2\times 1$ and $C$ is $2\times 2$ (1 column, but 2 rows).
   $EE$ is not defined, because $E$ is $1\times 2$ (2 columns, but 1 row).

1. No, and no:

   $$
   \begin{aligned}
   AB &= \begin{pmatrix} 9 & 10 \\ 31 & 20 \end{pmatrix} \neq BA = \begin{pmatrix} 14 & 17 \\ 20 & 15 \end{pmatrix}, \\
   AC &= \begin{pmatrix} 1 & 0 \\ -11 & 5 \end{pmatrix} \neq CA = \begin{pmatrix} 3 & -1 \\ -4 & 3 \end{pmatrix}
   \end{aligned}
   $$

1. Yes:

   $$
   \begin{aligned}
   (AB)C &= \begin{pmatrix} 9 & 10 \\ 31 & 20 \end{pmatrix}C = \begin{pmatrix} -23 & 11 \\ -7 & 9 \end{pmatrix}, \\
   A(BC) &= A\begin{pmatrix} -17 & 7 \\ 11 & -3 \end{pmatrix} = \begin{pmatrix} -23 & 11 \\ -7 & 9 \end{pmatrix}
   \end{aligned}
   $$

   But this does not prove that matrix multiplication is associative.
   It is only one example, and a proof has to work for all matrices.
   (Matrix multiplication _is_ associative, but showing it needs a general argument.)
   One example can disprove a rule, though, as part **4.** does for commuting.

1. Yes:

   $$
   \begin{aligned}
   AC + BC &= \begin{pmatrix} 1 & 0 \\ -11 & 5 \end{pmatrix} + \begin{pmatrix} -17 & 7 \\ 11 & -3 \end{pmatrix} = \begin{pmatrix} -16 & 7 \\ 0 & 2 \end{pmatrix}, \\
   (A + B)C &= \begin{pmatrix} 3 & 5 \\ 10 & 6 \end{pmatrix}C = \begin{pmatrix} -16 & 7 \\ 0 & 2 \end{pmatrix}
   \end{aligned}
   $$

   Again, this is one example, so it does not prove the distributive property (which does hold for all matrices).

:::

::::

## Main problems

::::challenge{id="12_main_01" title="Main problems 1"}
If

$$
A = \frac{1}{2}\left(\begin{array}{cc} 1 & 1 \\ 1 & 1 \end{array}\right),
$$

find $A^2$ and $A^3$ and comment on your results.

:::solution

$$
A^2 = \frac{1}{4}\begin{pmatrix} 1 & 1 \\ 1 & 1 \end{pmatrix}\begin{pmatrix} 1 & 1 \\ 1 & 1 \end{pmatrix} = \frac{1}{4}\begin{pmatrix} 2 & 2 \\ 2 & 2 \end{pmatrix} = \frac{1}{2}\begin{pmatrix} 1 & 1 \\ 1 & 1 \end{pmatrix} = A
$$

So, $A^2 = A$, and then $A^3 = A^2A = AA = A$.
In fact $A^n = A$ for every positive whole number $n$.
A matrix with $A^2 = A$ is called _idempotent_.
Multiplying a vector by this $A$ replaces both of its entries by their average, and doing that a second time changes nothing.

:::

::::

::::challenge{id="12_main_02" title="Main problems 2"}
Given

$$
A = \begin{pmatrix} 2 & 1 & 3 \\ 3 & -2 & 1 \\ -1 & 0 & 1 \end{pmatrix}; \qquad B = \begin{pmatrix} 0 & -1 & 1 \\ -5 & 2 & -1 \\ 3 & 0 & 2 \end{pmatrix}
$$

Find:

- $AB$
- $BA$
- ${A}^T {B}^T$
- ${B}^T {A}^T$
- ${(AB)^T}$
- ${(BA)^T}$

Comment on your results.

:::solution

$$
AB = \begin{pmatrix} 4 & 0 & 7 \\ 13 & -7 & 7 \\ 3 & 1 & 1 \end{pmatrix}, \qquad
BA = \begin{pmatrix} -4 & 2 & 0 \\ -3 & -9 & -14 \\ 4 & 3 & 11 \end{pmatrix}
$$

$$
A^TB^T = \begin{pmatrix} -4 & -3 & 4 \\ 2 & -9 & 3 \\ 0 & -14 & 11 \end{pmatrix}, \qquad
B^TA^T = \begin{pmatrix} 4 & 13 & 3 \\ 0 & -7 & 1 \\ 7 & 7 & 1 \end{pmatrix}
$$

$$
(AB)^T = \begin{pmatrix} 4 & 13 & 3 \\ 0 & -7 & 1 \\ 7 & 7 & 1 \end{pmatrix}, \qquad
(BA)^T = \begin{pmatrix} -4 & -3 & 4 \\ 2 & -9 & 3 \\ 0 & -14 & 11 \end{pmatrix}
$$

$AB \neq BA$, so $A$ and $B$ do not commute.
But $(AB)^T = B^TA^T$ and $(BA)^T = A^TB^T$.
In other words, the transpose of a product is the product of the transposes in the _reverse_ order (this is true for all matrices).

:::

::::

::::challenge{id="12_main_03" title="Main problems 3"}
Find the determinant, $|A|$, of the following matrices:

1. $$
   A = \begin{pmatrix} 1 & 2 \\ 1 & 6 \end{pmatrix}
   $$

1. $$
   A = \begin{pmatrix} 1 &-1 \\ 2 &-4 \end{pmatrix}
   $$

:::solution

For a $2\times 2$ matrix, $|A| = a_{11}a_{22} - a_{12}a_{21}$.

1. $|A| = 1\times 6 - 2\times 1 = 4$
1. $|A| = 1\times(-4) - (-1)\times 2 = -4 + 2 = -2$

:::

::::

::::challenge{id="12_main_04" title="Main problems 4"}
Find the inverse, $A^{-1}$, of the following matrices:

1. $$
   A = \begin{pmatrix} 2 & 5 \\-1 & 4 \end{pmatrix}
   $$

1. $$
   A = \begin{pmatrix} -3 & 2 \\-1 & 7 \end{pmatrix}
   $$

:::solution

For a $2\times 2$ matrix, swap the diagonal entries, change the sign of the other two, and divide by the determinant:

$$
\begin{pmatrix} a & b \\ c & d \end{pmatrix}^{-1} = \frac{1}{ad - bc}\begin{pmatrix} d & -b \\ -c & a \end{pmatrix}
$$

1. $|A| = 2\times 4 - 5\times(-1) = 13$, so

   $$
   A^{-1} = \frac{1}{13}\begin{pmatrix} 4 & -5 \\ 1 & 2 \end{pmatrix}
   $$

1. $|A| = (-3)\times 7 - 2\times(-1) = -19$, so

   $$
   A^{-1} = -\frac{1}{19}\begin{pmatrix} 7 & -2 \\ 1 & -3 \end{pmatrix} = \frac{1}{19}\begin{pmatrix} -7 & 2 \\ -1 & 3 \end{pmatrix}
   $$

You can check each answer by multiplying it by $A$, which should give the identity matrix.

:::

::::

::::challenge{id="12_main_05" title="Main problems 5"}

Use Python's `numpy.linalg.solve` to solve the following systems of equations:

1. $$
   \begin{aligned} x + 2y - 3z &= 9,\\ 2x - y + z &= 0,\\ 4x - y + z &= 4. \end{aligned}
   $$

1. $$
   \begin{aligned}x + 5y + 3z &= 17,\\ 5x + y - 2z &= 4,\\ x + 2y + z &= 7. \end{aligned}
   $$

1. $$
   \begin{aligned} 2y + z &= -8,\\ x - 2y - 3z &= 0,\\ -x + y + 2z &= 3. \end{aligned}
   $$

```python
# hint
import numpy as np

A = np.array([[1,2,-3], [2,-1,1], [4,-1,1]])
b = np.array([9,0,4])

x = np.linalg.solve(A, b)

print(x)
```

:::solution

Write each system as $A\mathbf{x} = \mathbf{b}$, with the coefficients of $x$, $y$ and $z$ in the rows of $A$ (using a 0 for a missing term):

```python
import numpy as np

A = np.array([[1, 2, -3], [2, -1, 1], [4, -1, 1]])
b = np.array([9, 0, 4])
print(np.linalg.solve(A, b))

A = np.array([[1, 5, 3], [5, 1, -2], [1, 2, 1]])
b = np.array([17, 4, 7])
print(np.linalg.solve(A, b))

A = np.array([[0, 2, 1], [1, -2, -3], [-1, 1, 2]])
b = np.array([-8, 0, 3])
print(np.linalg.solve(A, b))
```

```text
[2. 5. 1.]
[0.66666667 2.66666667 1.        ]
[-4. -5.  2.]
```

1. $x = 2$, $y = 5$, $z = 1$
1. $x = 2/3$, $y = 8/3$, $z = 1$
1. $x = -4$, $y = -5$, $z = 2$

Substituting each solution back into its equations checks it.
For example, in **1.**, $2 + 2\times 5 - 3\times 1 = 9$.

:::

::::

::::challenge{id="12_main_06" title="Main problems 6"}

$$
X = \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}; \qquad Y = \begin{pmatrix} 5 & 6 \\ 7 & 8 \end{pmatrix}
$$

Calculate:

- $XY$
- $YX$
- $X^{-1}$
- $Y^{-1}$
- $X^{-1}Y^{-1}$
- $Y^{-1}X^{-1}$
- $(XY)^{-1}$
- $(YX)^{-1}$

Comment on your results.

:::solution

$$
XY = \begin{pmatrix} 19 & 22 \\ 43 & 50 \end{pmatrix}, \qquad
YX = \begin{pmatrix} 23 & 34 \\ 31 & 46 \end{pmatrix}
$$

$|X| = 4 - 6 = -2$ and $|Y| = 40 - 42 = -2$, so

$$
\begin{aligned}
X^{-1} &= -\frac{1}{2}\begin{pmatrix} 4 & -2 \\ -3 & 1 \end{pmatrix} = \begin{pmatrix} -2 & 1 \\ 3/2 & -1/2 \end{pmatrix}, \\
Y^{-1} &= -\frac{1}{2}\begin{pmatrix} 8 & -6 \\ -7 & 5 \end{pmatrix} = \begin{pmatrix} -4 & 3 \\ 7/2 & -5/2 \end{pmatrix}
\end{aligned}
$$

Then

$$
X^{-1}Y^{-1} = \frac{1}{4}\begin{pmatrix} 46 & -34 \\ -31 & 23 \end{pmatrix}, \qquad
Y^{-1}X^{-1} = \frac{1}{4}\begin{pmatrix} 50 & -22 \\ -43 & 19 \end{pmatrix}
$$

$|XY| = 950 - 946 = 4$ and $|YX| = 1058 - 1054 = 4$, so

$$
(XY)^{-1} = \frac{1}{4}\begin{pmatrix} 50 & -22 \\ -43 & 19 \end{pmatrix}, \qquad
(YX)^{-1} = \frac{1}{4}\begin{pmatrix} 46 & -34 \\ -31 & 23 \end{pmatrix}
$$

So, $(XY)^{-1} = Y^{-1}X^{-1}$ and $(YX)^{-1} = X^{-1}Y^{-1}$.
Like the transpose, the inverse of a product is the product of the inverses in the _reverse_ order.
This makes sense: to undo "multiply by $Y$, then by $X$", we first undo $X$, then undo $Y$.
Note also that $|XY| = |X|\,|Y| = 4$.

:::

::::

::::challenge{id="12_main_07" title="Main problems 7"}

Write each of the following systems in the form $A\mathbf{x} = \mathbf{b}$, and solve it by hand by finding $A^{-1}$ and calculating $\mathbf{x} = A^{-1}\mathbf{b}$:

1. $$
   \begin{aligned} 3x - 2y &= 4,\\ x + y &= 3. \end{aligned}
   $$

1. $$
   \begin{aligned} 2x + 3y &= 1,\\ 4x - y &= 9. \end{aligned}
   $$

Check your answers using Python's `numpy.linalg.solve`.

:::solution

1. In matrix form:

   $$
   \begin{pmatrix} 3 & -2 \\ 1 & 1 \end{pmatrix} \begin{pmatrix} x \\ y \end{pmatrix} = \begin{pmatrix} 4 \\ 3 \end{pmatrix}
   $$

   $|A| = 3\times 1 - (-2)\times 1 = 5$, so

   $$
   A^{-1} = \frac{1}{5}\begin{pmatrix} 1 & 2 \\ -1 & 3 \end{pmatrix}
   $$

   and

   $$
   \begin{pmatrix} x \\ y \end{pmatrix} = \frac{1}{5}\begin{pmatrix} 1 & 2 \\ -1 & 3 \end{pmatrix}\begin{pmatrix} 4 \\ 3 \end{pmatrix} = \frac{1}{5}\begin{pmatrix} 10 \\ 5 \end{pmatrix} = \begin{pmatrix} 2 \\ 1 \end{pmatrix}
   $$

   So, $x = 2$ and $y = 1$.

1. In matrix form:

   $$
   \begin{pmatrix} 2 & 3 \\ 4 & -1 \end{pmatrix} \begin{pmatrix} x \\ y \end{pmatrix} = \begin{pmatrix} 1 \\ 9 \end{pmatrix}
   $$

   $|A| = 2\times(-1) - 3\times 4 = -14$, so

   $$
   A^{-1} = -\frac{1}{14}\begin{pmatrix} -1 & -3 \\ -4 & 2 \end{pmatrix}
   $$

   and

   $$
   \begin{pmatrix} x \\ y \end{pmatrix} = -\frac{1}{14}\begin{pmatrix} -1 & -3 \\ -4 & 2 \end{pmatrix}\begin{pmatrix} 1 \\ 9 \end{pmatrix} = -\frac{1}{14}\begin{pmatrix} -28 \\ 14 \end{pmatrix} = \begin{pmatrix} 2 \\ -1 \end{pmatrix}
   $$

   So, $x = 2$ and $y = -1$.

In Python:

```python
import numpy as np

A = np.array([[3, -2], [1, 1]])
b = np.array([4, 3])
print(np.linalg.solve(A, b))

A = np.array([[2, 3], [4, -1]])
b = np.array([1, 9])
print(np.linalg.solve(A, b))
```

```text
[2. 1.]
[ 2. -1.]
```

:::

::::

## Extension problems

::::challenge{id="12_ext_01" title="Extension problems 1"}
Show that

$$
\begin{aligned}
\begin{pmatrix} 3 & 1 \\ 1 & 3 \end{pmatrix} \begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{1}{\sqrt 2} \end{pmatrix} &= \lambda_1 \begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{1}{\sqrt 2} \end{pmatrix} \\
\begin{pmatrix} 3 & 1 \\ 1 & 3 \end{pmatrix} \begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{-1}{\sqrt 2} \end{pmatrix} &= \lambda_2 \begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{-1}{\sqrt 2} \end{pmatrix}
\end{aligned}
$$

where $\lambda_1$ and $\lambda_2$ are constants to be determined.

:::solution

Multiply out the left-hand sides:

$$
\begin{aligned}
\begin{pmatrix} 3 & 1 \\ 1 & 3 \end{pmatrix} \begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{1}{\sqrt 2} \end{pmatrix} &= \begin{pmatrix} \frac{4}{\sqrt 2} \\ \frac{4}{\sqrt 2} \end{pmatrix} = 4\begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{1}{\sqrt 2} \end{pmatrix}, \\
\begin{pmatrix} 3 & 1 \\ 1 & 3 \end{pmatrix} \begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{-1}{\sqrt 2} \end{pmatrix} &= \begin{pmatrix} \frac{2}{\sqrt 2} \\ \frac{-2}{\sqrt 2} \end{pmatrix} = 2\begin{pmatrix} \frac{1}{\sqrt 2} \\ \frac{-1}{\sqrt 2} \end{pmatrix}
\end{aligned}
$$

So, $\lambda_1 = 4$ and $\lambda_2 = 2$.
Multiplying each of these vectors by the matrix just stretches it, without changing its direction.
Vectors like this are called _eigenvectors_, and $\lambda_1$ and $\lambda_2$ are their _eigenvalues_ (more on this in the next lecture).

:::

::::
