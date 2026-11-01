---
name: Systems of differential equations 2
dependsOn: [scientific_computing.essential_maths.14_system_1]
tags: []
learningOutcomes:
  - Be able to analyse non-linear systems in multiple dimensions
  - Understand how to simplify the form of a system using parameter re-scaling
  - Understand how to use properties of a system to simplify the set of equations
  - Understand the concept of a phase plane
  - Be able to identify nullclines of a non-linear system
  - Be able to plot the phase plane of a non-linear system
  - Be able to plot the phase plane of a non-linear system using matplotlib in Python
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

[![Youtube lecture thumbnail](fig/youtube15.jpg)](https://youtu.be/Mi5gTeTYM30)

---

## System simplification

### Recap

- So far we have looked at systems of **first order**, **linear** ODEs in **two dimensions**

- These systems can be solved analytically
  - 3 methods of solving systems of first order ODEs
  - Diagonalisation extends to $N$-dimensional systems

### Plan

- Aim to look at systems of **first order**, **nonlinear** ODEs in **more dimensions**
- How we go about modelling a problem
- Simplifying systems of ODEs
  - Reducing number of parameters
  - Reducing number of equations
- Phase plane analysis

## System simplification: rescaling

This predator-prey model is in dimensional terms and contains 5 parameters.

$$
\begin{aligned}
\frac{dN}{dT} &= rN\left(1-\frac{N}{K}\right) - bNP,\\
\frac{dP}{dT} &= ebNP - mP.
\end{aligned}
$$

- $r$: prey intrinsic growth rate
- $K$: prey carrying capacity (bound to logistic growth)
- $b$: predation rate
- $e$: conversion efficiency
- $m$: per capita predator mortality rate

If we substitute

- $N=\theta{n}$
- $P=\phi{p}$
- $T=\tau{t}$

where $n,p,t$ are nondimensional, and $\theta$, $\phi$ and $\tau$ is a typical values for $N$, $P$ and $T$.

Note that, when we are re-scaling time:

$$
\frac{d}{dT} = \frac{d}{dt}\frac{dt}{dT} = \frac{1}{\tau} \frac{d}{dt}.
$$

The original equations are:

$$
\begin{aligned}
\frac{dN}{dT} &= rN\left(1-\frac{N}{K}\right) - bNP,\\
\frac{dP}{dT} &= ebNP - mP.
\end{aligned}
$$

And, after rescaling, we have:

$$
\begin{aligned}
\frac{\theta}{\tau}\frac{dn}{dt} &= {\theta}rn\left(1-\frac{{\theta}n}{K}\right) - bnp{\theta}{\phi},\\
\frac{\phi}{\tau}\frac{dp}{dt} &= ebnp{\theta}{\phi} - mp{\phi}.
\end{aligned}
$$

Note that $\theta$, $\phi$ and $\tau$ are arbitrary values for scaling $N$, $P$, and $T$.

$$
\begin{aligned}
\frac{dn}{dt} &= \tau\left( rn\left(1-\frac{{\theta}n}{K}\right) - bnp{\phi}\right),\\
\frac{dp}{dt} &= \tau\left( ebnp{\theta} - mp\right).
\end{aligned}
$$

After rearranging we note that choosing $\theta=K$ will remove the parameter $K$ from the equations.

Choosing $\tau = 1/m$ (or $\tau =1/r$) will remove a further parameter.

$$
\begin{aligned}
\dot{n} &= \left( \frac{r}{m}n(1-n) - \frac{b{\phi}}{m}np\right),\\
\dot{p} &= \left( \frac{ebpK}{m}n - p\right).
\end{aligned}
$$

$\phi=m/b$ gives the two parameter system:

$$
\begin{aligned}
\dot{n} &= \left( \frac{r}{m}n(1-n) - np\right),\\
\dot{p} &= \left( \frac{ebpK}{m}n - p\right),
\end{aligned}
$$

where we have shifted the groups of parameters to only two places.

We could make it more obvious that there are only two "tuning dials" by writing it as:

$$
\begin{aligned}
\dot{n} &= v_1 n(1-n) - np,\\
\dot{p} &= p(v_2 n - 1),
\end{aligned}
$$

## System simplification: conservation

Consider a chemical system where a product is formed from a substrate via an intermediate complex involving an enzyme.

$$
S + E \overset{k_1}{\underset{k_{-1}}\rightleftharpoons} C \overset{k_2}\rightarrow P + E
$$

This system can be written as a system of 4 ODEs for $s, e, c, p$ which describe the concentrations of $S, E, C$ and $P$:

$$
\begin{aligned}
\dot{s} &= -k_1 se + k_{-1}c, \\
\dot{e} &= -k_1 se + k_{-1}c + k_{2}c,\\
\dot{c} &= k_1 se - k_{-1}c - k_{2}c, \\
\dot{p} &= k_{2}c.
\end{aligned}
$$

However, the enzyme is recycled: it is used in the complex and then released. This means that $e + c = e_{\text{tot}}$ where $e_{\text{tot}}$ is constant.

Making the substitution $e = e_{\text{tot}} - c$ to eliminate $e$ we arrive at the 3 ODE system:

$$
\begin{aligned}
\dot{s} &= -k_1 s(e_{\text{tot}} - c) + k_{-1}c, \\
\dot{c} &= k_1 s(e_{\text{tot}} - c) - k_{-1}c - k_{2}c,\\
\dot{p} &= k_{2}c.
\end{aligned}
$$

## System simplification: quasi-steady state

In the previous example it may be known that the rate of conversion to complex (from $k_1$ and $k_{-1}$) is much faster than the degradation of complex into product (rate $k_2$).

If this is the case then we can assume that the concentration of $C$ arrives at some level $c^*$ (a quasi-steady state) and then moves very slowly.

Solving for $\dot{c}=0$:

$$
\begin{aligned}
\dot{c} &= k_1 s(e_{\text{tot}} - c) - k_{-1}c - k_{2}c = 0, \\
\implies
c^*&=\frac{k_1 e_{\text{tot}} s}{k_{-1} + k_2 + k_1 s},
\end{aligned}
$$

which is **not a steady-state**: it moves slowly with $s$.

Substituting this quasi-steady state into the remaining equations gives an **approximate** system of 2 ODEs:

$$
\begin{aligned}
\dot{s} &= -\frac{k_1 k_2 e_{\text{tot}} s}{ k_{-1} + k_2 + k_1 s}, \\
\dot{p} &= \frac{k_1 k_2 e_{\text{tot}} s}{ k_{-1} + k_2 + k_1 s}.
\end{aligned}
$$

This means that we have used conservation and quasi-steady state to go from a 4-dimensional system $(s,e,c,p)$ to a two-dimensional approximation which captures some of the behaviour.

Two dimensions are good because we can plot their behaviour on a phase plane diagram.

## Phase planes and nullclines

A system of **nonlinear** ODEs may have more than one fixed point (or may have none).
Finding fixed points in two-dimensional systems is aided by **nullclines**.

An $x$-nullcline is a line where $\dot{x}=0$ and a $y$-nullcline is a line where $\dot{y}=0$.
(These are straight lines in the linear case.)

For example:

$$
\begin{aligned}
\dot{x} &= x(1-x) -xy,\\
\dot{y} &= 2y\left(1-\frac{y}{2}\right) - 3xy,
\end{aligned}
$$

has $x$-nullclines at $x=0$ and $1-x-y=0$; and $y$-nullclines at $y=0$ and $2-3x-y=0$.

Nullcline intersections give us the fixed points.
Nullclines can be annotated to give the direction (and magnitude) of the non-zero derivative.

### Plot of the nullclines

![Plot of the nullclines of the ODE system](fig/15_01_nullclines.svg)

### Plot of the phase plane

![Plot of the phase plane of the ODE system](fig/15_02_phase_plane.svg)

The nullclines allow us to add arrows demonstrating the flow direction, and by following the arrows we can sketch the behaviour of solutions (green lines).
The arrows can only cross the $x$-nullclines vertically, and the $y$-nullclines horizontally.

### Python code to plot the phase plane

```python
import numpy as np
from matplotlib import pyplot as plt
from scipy.integrate import solve_ivp
def dX_dt(t, X):
    return np.array([ X[0]*(1. - X[0]) - X[0]*X[1],
                     2.*X[1]*(1.-X[1]/2.) -3*X[0]*X[1]])

def plot_phase_plane():

    plt.figure(figsize=(10,10))

    init_x = [1.05, 0.9, 0.7, 0.5, 0.5, 0.32, 0.1]
    init_y = [1.0, 1.3, 1.6, 1.8, 0.2, 0.2, 0.2]

    plt.plot(init_x, init_y, 'g*', markersize=20)

    for v in zip(init_x,init_y):
        X0 = v                              # starting point
        sol = solve_ivp(dX_dt, (0, 10), X0, t_eval=np.linspace(0, 10, 100))
        plt.plot(sol.y[0], sol.y[1], lw=3, color='green')



    # plot nullclines
    x = np.linspace(-0.1,1.1,24)
    y = np.linspace(-0.1,2.1,24)

    plt.hlines(0,-1,15, color='#F39200', lw=4, label='y-nullcline 1')
    plt.plot(x,1 - x, color='#0072bd', lw=4, label='x-nullcline 2')
    plt.vlines(0,-1,15, color='#0072bd', lw=4, label='x-nullcline 1')
    plt.plot(x,2 - 3*x, color='#F39200', lw=4, label='y-nullcline 2')

    # quiverplot - define a grid and compute direction at each point
    X, Y = np.meshgrid(x, y)                  # create a grid
    DX = X*(1-X) - X*Y                          # evaluate dx/dt
    DY = 2*Y*(1 - Y/2.0) - 3*X*Y                # evaluate dy/dt
    M = (np.hypot(DX, DY))                      # norm growth rate
    M[ M == 0] = 1.                             # avoid zero division errors

    plt.quiver(X, Y, DX/M, DY/M, M)
    plt.xlim(-0.05,1.1)
    plt.ylim(-0.05,2.1)
    plt.xlabel('x')
    plt.ylabel('y')
```

## Summary

- Simplification
  - Rescaling to dimensionless quantities
  - Conservation
  - Quasi-steady state approximation
- Nullclines are a powerful way of finding steady states and phase flow

## Introductory problems

:::callout{variant="tip"}
For more practice, an AI tool can write new problems like the ones below.
Ask it for problems of the same type and difficulty, or for problems set in your own field.
For example, you could ask it to "write five chemistry problems on fixed points and rescaling, at the level of these ones".
Work them on paper first, then ask the tool to check your answers and to explain any step you are unsure of.
:::

::::challenge{id="15_intro_01" title="Introductory problems 1"}
Find the fixed points of the following linear systems:

1. $\dot{x} = x+3y, \qquad \dot{y}=-6x+5y;$
1. $\dot{x} = x+3y+4, \qquad \dot{y}=-6x+5y-1;$
1. $\dot{x} = x+3y+1, \qquad \dot{y}=-6x+5y.$

:::solution

At a fixed point $\dot{x} = 0$ and $\dot{y} = 0$, so in each case we solve a pair of simultaneous equations.

1. $x + 3y = 0$ gives $x = -3y$.
   Substituting into $-6x + 5y = 0$ gives $18y + 5y = 0$, so $y = 0$ and $x = 0$.
   The fixed point is $(0, 0)$.

1. Multiply the first equation by 6 and add it to the second:

   $$
   (6x + 18y + 24) + (-6x + 5y - 1) = 23y + 23 = 0
   $$

   So, $y = -1$, and then $x = -4 - 3y = -1$.
   The fixed point is $(-1, -1)$.

1. In the same way, $(6x + 18y + 6) + (-6x + 5y) = 23y + 6 = 0$, so $y = -6/23$, and then $x = -1 - 3y = -5/23$.
   The fixed point is $(-5/23, -6/23)$.

:::

::::

::::challenge{id="15_intro_02" title="Introductory problems 2"}
Find the fixed points of the following nonlinear systems:

1. $\dot{x} = -4y+2xy-8 \qquad \dot{y}=4y^2-x^2;$
1. $\dot{x} = y-x^2+2, \qquad \dot{y}=2(x^2-y^2).$

:::solution

1. $\dot{y} = 0$ gives $x^2 = 4y^2$, so $x = 2y$ or $x = -2y$.
   Substitute each into $\dot{x} = 0$.

   - $x = 2y$: $-4y + 4y^2 - 8 = 0$, so $y^2 - y - 2 = (y - 2)(y + 1) = 0$.
     This gives $y = 2$, $x = 4$ and $y = -1$, $x = -2$.
   - $x = -2y$: $-4y - 4y^2 - 8 = 0$, so $y^2 + y + 2 = 0$.
     This has no real solutions, because $1^2 - 4\times 2 < 0$.

   The fixed points are $(4, 2)$ and $(-2, -1)$.

1. $\dot{x} = 0$ gives $y = x^2 - 2$, and $\dot{y} = 0$ gives $y = x$ or $y = -x$.

   - $y = x$: $x^2 - x - 2 = (x - 2)(x + 1) = 0$, which gives $(2, 2)$ and $(-1, -1)$.
   - $y = -x$: $x^2 + x - 2 = (x + 2)(x - 1) = 0$, which gives $(-2, 2)$ and $(1, -1)$.

   There are four fixed points: $(2, 2)$, $(-1, -1)$, $(-2, 2)$ and $(1, -1)$.

:::

::::

## Main problems

::::challenge{id="15_main_01" title="Main problems 1"}
Consider the chemical reaction network:

$$
\overset{k_0}\longrightarrow A \overset{k_1}{\underset{k_{-1}}\rightleftharpoons} B \overset{k_2}\longrightarrow
$$

1. Write down the system of two linear ODEs which describe the evolution of the concentrations of A and B in this system under the law of mass action.
1. Find the ratio of concentrations of A and B for which this system is in steady state: that is the concentrations do not change over time.

:::solution

Let $a = [A]$ and $b = [B]$.

1. A is produced at the constant rate $k_0$, turns into B at rate $k_1 a$, and is made from B at rate $k_{-1}b$.
   B is removed at rate $k_2 b$.
   So,

   $$
   \begin{aligned}
   \frac{da}{dt} &= k_0 - k_1 a + k_{-1} b \\
   \frac{db}{dt} &= k_1 a - k_{-1} b - k_2 b
   \end{aligned}
   $$

1. In steady state $db/dt = 0$, so $k_1 a = (k_{-1} + k_2) b$, and

   $$
   \frac{a}{b} = \frac{k_{-1} + k_2}{k_1}
   $$

   We can also find the concentrations themselves.
   Adding the two equations gives $k_0 - k_2 b = 0$, so $b^* = k_0/k_2$, and then $a^* = k_0(k_{-1} + k_2)/(k_1 k_2)$.

:::

::::

::::challenge{id="15_main_02" title="Main problems 2"}
Consider the reversible enzyme reaction:

$$
S + E \overset{k_1}{\underset{k_{-1}}\rightleftharpoons} C \overset{k_2}{\underset{k_{-2}}\rightleftharpoons} P + E
$$

Verify the Haldane relation, which states that when the reaction is in equilibrium,

$$
\frac{p}{s} = \frac{k_1 k_2}{k_{-1}k_{-2}},
$$

where $p$ and $s$ are the concentrations of $P$ and $S$, respectively.

:::solution

Let $s$, $e$, $c$ and $p$ be the concentrations of $S$, $E$, $C$ and $P$.
By mass action,

$$
\begin{aligned}
\frac{ds}{dt} &= -k_1 s e + k_{-1} c \\
\frac{dp}{dt} &= k_2 c - k_{-2} p e
\end{aligned}
$$

In equilibrium the concentrations do not change, so both are zero:

$$
k_1 s e = k_{-1} c \qquad\text{and}\qquad k_2 c = k_{-2} p e
$$

The first gives $c = k_1 s e/k_{-1}$.
Substituting this into the second,

$$
\frac{k_1 k_2 s e}{k_{-1}} = k_{-2} p e
$$

The enzyme concentration $e$ cancels, so

$$
\frac{p}{s} = \frac{k_1 k_2}{k_{-1}k_{-2}}
$$

(We can check that $dc/dt = k_1 s e - k_{-1}c - k_2 c + k_{-2} p e$ is then zero too.)

:::

::::

::::challenge{id="15_main_03" title="Main problems 3"}
The population of a host, $H(t)$, and a parasite, $P(t)$, are described approximately by the equations:

$$
\frac{dH}{dT}=(a-bP)H,\qquad \frac{dP}{dT}=\left(c-\frac{kP}{H}\right)P, \qquad H>0,
$$

where $a,b,c,k$ are positive constants.

By a suitable change of scales show that these equations may be put in the simpler form

$$
\dot{y} = (1-x)y, \qquad \dot{x}=\alpha x\left(1-\frac{x}{y}\right),
$$

where $\alpha=c/a$.

Sketch the phase flow across the following lines:

1. $y=x$;
1. $x=0$;
1. $y=0$;
1. $x=1$;
1. $y=\beta x$, for $\beta$ greater than and less than 1.

:::solution

As in the lecture, let $T = \tau t$, $P = \theta x$ and $H = \phi y$, where $\tau$, $\theta$ and $\phi$ are scales still to be chosen, so that $d/dT = (1/\tau)\,d/dt$.
Then

$$
\frac{\phi}{\tau}\dot{y} = (a - b\theta x)\phi y \qquad\text{and}\qquad \frac{\theta}{\tau}\dot{x} = \left(c - \frac{k\theta x}{\phi y}\right)\theta x
$$

so

$$
\dot{y} = \tau(a - b\theta x)y \qquad\text{and}\qquad \dot{x} = \tau\left(c - \frac{k\theta x}{\phi y}\right)x
$$

Choosing $\tau = 1/a$ and $\theta = a/b$ gives $\dot{y} = (1-x)y$, and

$$
\dot{x} = \left(\frac{c}{a} - \frac{kx}{b\phi y}\right)x = \frac{c}{a}\,x\left(1 - \frac{ka}{bc\phi}\,\frac{x}{y}\right)
$$

Finally, choosing $\phi = ka/(bc)$ gives $\dot{x} = \alpha x(1 - x/y)$, with $\alpha = c/a$.

Now the flow across each line.
$x$ and $y$ are populations, so we only need $x \geq 0$ and $y > 0$.

1. On $y = x$, $\dot{x} = 0$ (this is an $x$-nullcline), so the flow crosses it vertically.
   $\dot{y} = (1-x)x$, so the flow is upwards for $x < 1$ and downwards for $x > 1$.
1. On $x = 0$, $\dot{x} = 0$ and $\dot{y} = y > 0$, so the flow is up the $y$ axis.
   With no parasites, the hosts grow exponentially.
1. On $y = 0$, $\dot{y} = 0$ (a $y$-nullcline).
   The equations are not defined on the line itself, because of $x/y$, but just above it $x/y$ is very large, so $\dot{x}$ is large and negative and the flow moves sharply to the left.
   With almost no hosts, the parasites die out.
1. On $x = 1$, $\dot{y} = 0$ (a $y$-nullcline), so the flow crosses it horizontally.
   $\dot{x} = \alpha(1 - 1/y)$, so the flow is to the right for $y > 1$ and to the left for $y < 1$.
1. On $y = \beta x$, $\dot{x} = \alpha x(1 - 1/\beta)$ and $\dot{y} = \beta x(1 - x)$.
   For $\beta > 1$ (above the line $y = x$) the flow is to the right, and for $\beta < 1$ (below it) the flow is to the left.
   In both cases it is upwards for $x < 1$ and downwards for $x > 1$.

Putting these together, the flow goes clockwise around the point $(1, 1)$, where the nullclines $y = x$ and $x = 1$ cross (we find out what happens there in the next lecture).
For example, with $\alpha = 1$:

```python
import numpy as np
from matplotlib import pyplot as plt

alpha = 1
x, y = np.meshgrid(np.linspace(0.01, 2.5, 30), np.linspace(0.01, 2.5, 30))
xdot = alpha * x * (1 - x / y)
ydot = (1 - x) * y

plt.streamplot(x, y, xdot, ydot, color='grey')
plt.plot([0, 2.5], [0, 2.5], color='#0072bd', label='x-nullcline y = x')
plt.axvline(0, color='#0072bd', label='x-nullcline x = 0')
plt.axvline(1, color='#F39200', label='y-nullcline x = 1')
plt.axhline(0, color='#F39200', label='y-nullcline y = 0')
plt.xlabel('x (parasites)')
plt.ylabel('y (hosts)')
plt.legend()
```

:::

::::

::::challenge{id="15_main_04" title="Main problems 4"}
Consider a lake with some fish attractive to anglers.
We wish to model the fish-angler interaction under the following assumptions:

- the fish population grows logistically in the absence of fishing;
- the presence of anglers depresses the fish growth rate at a rate jointly proportional to the size of the fish and angler populations;
- anglers are attracted to the lake at a rate directly proportional to the number of fish in the lake;
- anglers are discouraged from the lake at a rate directly proportional to the number of anglers already there.

1. Write down a mathematical model for this situation, clearly defining your terms.
1. Use a suitable scaling to show that a non-dimensionalised version of the model is

   $$
   \dot{x} = rx(1 - x) - xy,\qquad \dot{y} = \beta x - y
   $$

:::solution

1. Let $F(T)$ be the number of fish and $A(T)$ the number of anglers at time $T$.
   Then

   $$
   \begin{aligned}
   \frac{dF}{dT} &= aF\left(1 - \frac{F}{K}\right) - bFA \\
   \frac{dA}{dT} &= cF - eA
   \end{aligned}
   $$

   where all the constants are positive:
   - $a$ is the growth rate of the fish, and $K$ is the largest number of fish the lake can support (its carrying capacity);
   - $b$ measures how quickly anglers catch fish;
   - $c$ measures how strongly the fish attract anglers;
   - $e$ is the rate at which anglers are discouraged and leave.

1. As in the lecture, let $F = \theta x$, $A = \phi y$ and $T = \tau t$, so that $d/dT = (1/\tau)\,d/dt$.
   Then

   $$
   \begin{aligned}
   \dot{x} &= \tau a x\left(1 - \frac{\theta x}{K}\right) - \tau b\phi xy \\
   \dot{y} &= \frac{\tau c\theta}{\phi}x - \tau e y
   \end{aligned}
   $$

   Choosing $\theta = K$, $\tau = 1/e$ and $\phi = e/b$ gives

   $$
   \dot{x} = \frac{a}{e}x(1 - x) - xy \qquad\text{and}\qquad \dot{y} = \frac{bcK}{e^2}x - y
   $$

   which is the model given, with $r = a/e$ and $\beta = bcK/e^2$.

:::

::::
