---
name: Finite Difference Matrix
dependsOn: ["scientific_computing.sparse_linear_algebra.02-coo-matrix"]
tags: []
learningOutcomes:
  -  Understand how to construct a sparse matrix that is derived from the finite difference discretisation of the Poisson equation.
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

Many matrices in scientific computing contain mostly zeros, particularly those arising
from the discretisation of partial differential equations (PDEs). Here we will construct
a sparse matrix using `scipy.sparse` that is derived from the finite difference
discretisation of the Poisson equation. In 1D, Poisson equation is

$$
u_{xx} = f(x)\text{ for }0 \le x \le 1
$$

The central FD approximation of $u_{xx}$ is:

$$
u_{xx} \approx \frac{u(x + h) - 2u(x) + u(x-h)}{h^2}
$$

We will discretise $u_{xx} = 0$ at $N$ regular points along $x$ from 0 to 1, given by
$x_1$, $x_2$:

              +----+----+----------+----+> x
              0   x_1  x_2    ... x_N   1

Using this set of point and the discretised equation, this gives a set of $N$ equations
at each interior point on the domain:

$$
\frac{v_{i+1} - 2v_i + v_{i-1}}{h^2} = f(x_i) \text{ for } i = 1...N
$$

where $v_i \approx u(x_i)$.

To solve these equations we will need additional equations at $x=0$ and $x=1$, known as
the _boundary conditions_. For this example we will use $u(x) = g(x)$ at $x=0$ and $x=1$
(also known as a non-homogenous Dirichlet bc), so that $v_0 = g(0)$, and $v\_{N+1} =
g(1)$, and the equation at $x_1$ becomes:

$$
\frac{v_{i+1} - 2v_i + g(0)}{h^2} = f(x_i)
$$

and the equation at $x_N$ becomes:

$$
\frac{g(1) - 2v_i + v_{i-1}}{h^2} = f(x_i)
$$

We can therefore represent the final $N$ equations in matrix form like so:

$$
\frac{1}{h^2}
\begin{bmatrix} -2      & 1      &         &   &     \\
 1      & -2     & 1       &       & \\
&\ddots & \ddots  &  \ddots &\\
&        & 1      &  -2     &  1     \\
&        &        &   1     & -2     \end{bmatrix}
\begin{bmatrix} v_1    \\
v_2    \\
\vdots \\
v_{N-1}\\
v_{N}
\end{bmatrix}
= \begin{bmatrix} f(x_1)    \\
f(x_2)    \\
\vdots \\
f(x_{N-1})  \\
f(x_N)
\end{bmatrix} - \frac{1}{h^2} \begin{bmatrix} g(0)    \\
0    \\
\vdots \\
0    \\
g(1)
\end{bmatrix}
$$

The relevant sparse matrix here is $A$, given by

$$
A = \begin{bmatrix} -2      & 1      &         &   &     \\
 1      & -2     & 1       &       & \\
&\ddots & \ddots  &  \ddots &\\
&        & 1      &  -2     &  1     \\
&        &        &   1     & -2     \end{bmatrix}
$$

As you can see, the number of non-zero elements grows linearly with the size $N$, so a
sparse matrix format is much preferred over a dense matrix holding all $N^2$ elements!

## Additional Reading

For more on the Finite Difference Method for solving PDEs:

K. W. Morton and D. F. Mayers. Numerical Solution of Partial Differential Equations: An
Introduction. Cambridge University Press, 2005.
