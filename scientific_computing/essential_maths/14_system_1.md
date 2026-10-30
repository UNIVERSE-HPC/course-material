---
name: Systems of differential equations 1
dependsOn:
  [
    scientific_computing.essential_maths.11_differential_equations_3,
    scientific_computing.essential_maths.13_linear_algebra_2,
  ]
tags: []
learningOutcomes:
  - Understand the concept of a linear system of differential equations
  - Recognise the different methods to solve a linear system
  - Be able to turn a coupled linear system into a single differential equation
  - Be able to use matrix diagonalisation to solve a linear system in terms of its eigenvalues
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

[![Youtube lecture thumbnail](fig/youtube14.jpg)](https://youtu.be/B9DWIGrRkLE)

---

## Linear systems

Our aim is to solve systems of equations of the form:

$$
\frac{dy_i}{dt} = f_i(y_1,\ldots,y_n,t),
$$

for $i=1,\ldots,n$.

Let us first consider the simplest form: a $2\times2$ **linear** system

$$
\begin{aligned}
\frac{dx}{dt} &= ax+by,\\
\frac{dy}{dt} &= cx+dy.
\end{aligned}
$$

## Motivation

- [Lotka-Volterra](https://en.wikipedia.org/wiki/Lotka%E2%80%93Volterra_equations) (predator-prey)

- [SIR model](https://en.wikipedia.org/wiki/Compartmental_models_in_epidemiology) (epidemics)

- [Chemical reaction networks](https://en.wikipedia.org/wiki/Chemical_reaction_network_theory)

- [Heart cell dynamics](http://www.scholarpedia.org/article/Models_of_cardiac_cell)

In order to understand these systems, we must first understand coupled linear systems.

## Recap of eigenvalues

$$
A = \left(\begin{matrix} 1 & 1\\2 & 0 \end{matrix}\right)
$$

```python
import sympy as sp
A = sp.Matrix([[1, 1],[2, 0]])
A.eigenvals()
```

$$
\left\{ -1 : 1, \  2 : 1\right\}
$$

## Recap of eigenvectors

$$
A = \left(\begin{matrix} 1 & 1\\2 & 0 \end{matrix}\right)
$$

```python
A.eigenvects()
```

$$
\def\arraystretch{1.8}\left[ \left( -1, \  1, \  \left[ \left[\begin{matrix}- \frac{1}{2}\\1\end{matrix}\right]\right]\right), \  \left( 2, \  1, \  \left[ \left[\begin{matrix}1\\1\end{matrix}\right]\right]\right)\right]
$$

## Recap of diagonalisation

Recall that, for a matrix $A$ with **eigenvectors** $\mathbf{v}_1$ and $\mathbf{v}_2$, and **eigenvalues** $\lambda_1$ and $\lambda_2$, we can write a matrix of eigenvectors: $P = \left(\begin{matrix} \mathbf{v}_1 & \mathbf{v}_2 \end{matrix}\right)$. Then:

$$
A = P \left(\begin{matrix} \lambda_1 & 0 \\ 0& \lambda_2 \end{matrix}\right) P^{-1}
$$

(This is also true for general $n \times n$ matrices.)

In our example,

$$
P = \left(\begin{matrix} -\frac{1}{2} & 1 \\ 1& 1 \end{matrix}\right),\qquad\lambda_1 = -1,\qquad\lambda_2=2.
$$

## Coupled ODEs

For coupled system of first order linear differential equations of the form

$$
\begin{aligned}
\frac{dx}{dt} &= ax+by,\\
\frac{dy}{dt} &= cx+dy.
\end{aligned}
$$

we have three methods of analysing them mathematically:

1. Turn them into one second order equation (if we can solve second order!)
2. Divide one by other, to get one equation independent of $t$
3. Perform matrix diagonalisation (extends to $n \times n$ problems)

## Example

Solve

$$
\begin{aligned}
\frac{dx}{dt} &= x+y,\\
\frac{dy}{dt} &= 2x.
\end{aligned}
$$

Subject to

$$
\begin{aligned}
x(0) &= 0,\\
y(0) &= 3.
\end{aligned}
$$

## Method 1: second order

We start with:

$$
\begin{aligned}
\frac{dx}{dt} &= x+y,\\
\frac{dy}{dt} &= 2x.
\end{aligned}
$$

We can convert that into a second order equation:

$$
\frac{d^2x}{dt^2} = \frac{dx}{dt} + \frac{dy}{dt} = \frac{dx}{dt} + 2x
\qquad\implies\qquad
\boxed{\frac{d^2x}{dt^2} = \frac{dx}{dt} + 2x}
$$

## Method 2: eliminate $t$

We start with:

$$
\begin{aligned}
\frac{dx}{dt} &= x+y,\\
\frac{dy}{dt} &= 2x.
\end{aligned}
$$

Then, dividing:

$$
\frac{dy}{dx} = \frac{dy/dt}{dx/dt}
\qquad\implies\qquad
\boxed{\frac{dy}{dx} = \frac{2x}{x+y}}
$$

## Method 3: diagonalisation

Let

$$
\mathbf{v}=\left(\begin{matrix}x\\y\end{matrix}\right),
$$

then

$$
\frac{dx}{dt} = x+y,\quad
\frac{dy}{dt} = 2x \qquad\implies\qquad \boxed{\frac{d\mathbf{v}}{dt} = A\mathbf{v},}
$$

where

$$
A = \left(\begin{matrix} 1 & 1\\2 & 0 \end{matrix}\right).
$$

Substitute

$$
A = P \left(\begin{matrix} \lambda_1 & 0 \\ 0 & \lambda_2 \end{matrix}\right) P^{-1},
$$

then

$$
\frac{d\mathbf{v}}{dt} = P \left(\begin{matrix} \lambda_1 & 0 \\ 0 & \lambda_2 \end{matrix}\right) \left( P^{-1}\mathbf{v} \right)
$$

or

$$
\frac{d}{dt}(P^{-1} \mathbf{v}) = \left(\begin{matrix} \lambda_1 & 0 \\ 0 & \lambda_2 \end{matrix}\right) \left( P^{-1}\mathbf{v} \right).
$$

We can now introduce a new variable

$$
\mathbf{z} = \left(\begin{matrix} z_1 \\ z_2\end{matrix}\right) = P^{-1} \mathbf{v}
$$

so that:

$$
\frac{d\mathbf{z}}{dt} = \left(\begin{matrix} \lambda_1 & 0 \\ 0 & \lambda_2 \end{matrix}\right) \mathbf{z}.
$$

But now, because the matrix is diagonal, the system is no longer coupled.
The first equation **only** involves $z_1$ and the second **only** involves $z_2$, so we can solve each one individually:

$$
\begin{aligned}
\frac{dz_1}{dt} &= \lambda_1 z_1 &&\implies\qquad z_1(t) = A\,e^{\lambda_1 t} \\
\frac{dz_2}{dt} &= \lambda_2 z_2 &&\implies\qquad z_2(t) = B\,e^{\lambda_2 t}
\end{aligned}
$$

Finally, we can substitute $z_1$ and $z_2$ back in terms of $x$ and $y$ to find the solution to the original coupled system:

We have

$$
\mathbf{z} = \left(\begin{matrix} z_1 \\ z_2\end{matrix}\right) = P^{-1} \mathbf{v} = \frac{1}{3}\left(\begin{matrix} -2 & 2 \\ 2& 1 \end{matrix}\right) \left(\begin{matrix} x \\ y\end{matrix}\right) = \left(\begin{matrix} A\,e^{\lambda_1 t} \\ B\,e^{\lambda_2 t}\end{matrix}\right)
$$

Rearranging, we now have two equations relating $x$ and $y$:

$$
\begin{aligned}
-2x + 2y &= C\,e^{\lambda_1 t} \\
2x + y &= D\,e^{\lambda_2 t}
\end{aligned}
$$

where $C=3A$ and $D=3B$. Using our initial conditions, $x(0)=0$ and $y(0)=3$ we find $C=6$ and $D=3$.

Finally, solving the simultaneous equations, we have a solution:

$$
\begin{aligned}
x(t) &= -e^{\lambda_1 t} + e^{\lambda_2 t} \\
y(t) &= 2e^{\lambda_1 t} + e^{\lambda_2 t}
\end{aligned}
$$

## Summary

- Three methods to analytically solve systems of linear first
  order ODEs
- Best method depends on the system and what you need to ask about it

## Introductory problems

::::challenge{id="14_intro_01" title="Introductory problems 1"}
Find the general solution to the following system of ODEs:

$$
\frac{dx}{dt} = x, \quad\text{and}\quad \frac{dy}{dt}=y.
$$

Sketch the form of the solution in the $x,\,y$ plane, using arrows to indicate where the solution moves over time.
::::

::::challenge{id="14_intro_02" title="Introductory problems 2"}
Take the general decoupled linear system

$$
\frac{dx}{dt} = ax, \quad\text{and}\quad \frac{dy}{dt} = by.
$$

1. Integrate the two equations separately to solve for $x$ and $y$ in terms of $t$.
1. If you start at $t=0$, $x(0)=0$, $y(0)=0$ what happens to the solution over time?
1. If you start at a general position $x(0)=x_0$, $y(0)=y_0$ what happens to the solution as $t\rightarrow\infty$?
   - What if $a$ and $b$ are both negative?
   - What if only one of $a$ or $b$ is negative? What if either $x_0$ or $y_0$ is negative?
1. Either by eliminating $t$ from the original equations or by eliminating $t$ from your solutions to part **1.**, find a general solution of the system. (Why not try both methods?)
1. Sketch this solution for
   - $a>0$, $b>0$, $a=b$
   - $a>0$, $b<0$, $a=-b$.

::::

## Main problems

::::challenge{id="14_main_01" title="Main problems 1"}
By reformulating the following system as one first order equation (i.e eliminating $t$), find the general solution to:

$$
\frac{dx}{dt} = -y, \quad\text{and}\quad \frac{dy}{dt} = x.
$$

Sketch the form of the solutions in the $x,\,y$ plane.
::::

::::challenge{id="14_main_02" title="Main problems 2"}
Again by eliminating $t$ and reformulating the system as one first order equation, find the general solution to the following system of ODEs:

$$
\frac{dx}{dt} = y, \quad\text{and}\quad \frac{dy}{dt} = x.
$$

Sketch the form of the solutions in the $x,\,y$ plane.
::::

::::challenge{id="14_main_03" title="Main problems 3"}

Find the eigenvalues and two independent eigenvectors $\mathbf{v}_{1}$ and $\mathbf{v}_{2}$ of the matrix

$$
A = \left(\begin{array}{cc} 1 & 4 \\ 1 & 1 \end{array} \right).
$$

1. Put the vectors $\mathbf{v}_{1}$ and $\mathbf{v}_{2}$ as the columns of a $2\times 2$ matrix $P$. Find $P^{-1}$ and show (by calculation) that $P^{-1}AP$ is diagonal. What are the entries of this matrix? What do they correspond to?
1. Find the general solution of the system

   $$
   \frac{dx}{dt} = x+4y, \quad\text{and}\quad \frac{dy}{dt} = x + y.
   $$

1. Find the particular solution subject to $x(0)=0$ and $y(0)=2$.
1. Sketch the trajectory (the $x(t),\,y(t)$ coordinates over time) in the $x,\,y$ plane.
   - Draw the eigenvectors $\mathbf{v}_{1}$ and $\mathbf{v}_{2}$ on the same figure.
   - What happens as $t\rightarrow \infty$?
   - What about $t\rightarrow -\infty$?
   - What is $dy/dx$ at a general point on the $y$-axis?

::::

## Extension problems

::::challenge{id="14_ext_01" title="Extension problems 1"}
The force on a damped harmonic oscillator is

$$
f = -k x - m \nu \frac{dx}{dt},
$$

where $x$ is a displacement, $k>0$ is a spring force constant, $m>0$ is the mass and $\nu>0$ is the strength of the damping.

1. Use Newton's 2nd law of motion to write down an equation for the acceleration $d^2x/dt^2$.
1. Make the substitution

   $$
   y=\frac{dx}{dt}\quad\left(\text{and hence }\frac{dy}{dt} = \frac{d^2x}{dt^2}\right)
   $$

   to obtain a system of two first-order linear ODEs.

::::
