---
name: Non-linear Optimisation
dependsOn: []
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

## Mathematical formulation

Optimisation aims to find the minimum (or equivilently the maximum) of some _objective_,
or _loss_ function $f$, given a set of $n$ parameters $\theta$

$$
\min_{\theta \in \mathcal{R}^n} f(\theta)
$$

We might also have a set of _constraints_, for example a parameter might be required to
be non-negative (e.g. a concentration or population number). These are often written as
a set of equality $\mathcal{E}$ and inequality $\mathcal{I}$ constraints

$$
\min_{\theta \in \mathcal{R}^n} f(\theta) \text{ subject to } \begin{cases}
c_i(\theta) = 0, & i \in \mathcal{E} \\
c_i(\theta) \ge 0, & i \in \mathcal{I} \end{cases}
$$

Or these might simply be defined as _bounds_ in parameter space that restrict the
minimisation to a given domain $\Omega \in \mathcal{R}^n$

$$
\min_{\theta \in \Omega} f(\theta)
$$

## Useful terms

_Modelling_ is the process of defining the objective function $f$, the parameters of
interest $\theta$, and the constraints. The algorithms for performing the minimisation
fall under the field of optimisation. Sub-fields of this are concerned with the
minimisation of discrete function, often called _integer programming_. Confusingly, it
is common to see the terms "optimisation" and "programming" used interchangeably, as the
latter term was coined before the 1940s, and does not refer to computer software
programming at all.

If the function $f$ is linear, then there are specific algorithms for this class of
problem that fall under the topic of _linear programming_, or _linear optimisation_. The
more general problem of a non-linear $f$ is termed _non-linear programming_, or
_non-linear optimisation_. If a set of equality and/or inequality constraints are needed
then algorithms that deal with these fall under the topic of _constrained_ optimisation.

An important distinction when looking at optimisation problems is the notion of _global_
versus _local_ optimisation. The latter finds a point in parameter space $\theta_m$ that
has a function value $f(\theta_m)$ greater than the surrounding points, but might not
necessarily be the global minimum. These algorithms are often initialised to a point
that is near to the minima of interest. The more general problem of global optimisation
is significantly more difficult as it requires that the optimisation be robust to
finding and rejecting such local minima. For a function that is _convex_, then local and
global minimisation are the same, which is very advantagous since local minimisation
algorithms are often both faster and often have more guarentees of convergence. The
function $f$ is a convex function if its domain $\Omega$ is a convex set, and for any
two points $\theta_x$ and $\theta_y$:

$$
f(\alpha \theta_x + (1 - \alpha) \theta_y ) \le \alpha f(\theta_x) + (1 - \alpha)
f(\theta_y), \text{ for all } \alpha \in [0, 1].
$$

The term _convex programming_ is used to describe the case of contrained optimisation
where $f$ is convex, the equality constraints are linear and the inequality contraints
are concave.

## Non-linear optimisation and Root-Finding

Non-linear optimisation is closely related to finding the roots, or zeros, of a
function. This can be seen easily by considering the fact that at each local minima or
maxima of the function the value of the gradient of $f$ is zero, i.e. $\nabla f = 0$.
Therefore finding a local minima or maxima of $f$ corresponds to finding the zeros of
the function $g = \nabla f$.

### Other reading

- Numerical optimization by Nocedal, Jorge; Wright, Stephen J., 1960-
- Bazaraa, Sherali, and Shetty, Nonlinear Optimization, 2/e, Wiley
- Griva, Nash, and Sofer, Linear and Nonlinear Optimization, 2/e, SIAM Press
- Luenberger, Linear and Nonlinear Programming, 3/e, Springer
- Bertsekas, Nonlinear Programming, Athena
- Ruszczynski, Nonlinear Optimization, Princeton University Press
- Broyden, C. G. (1972). "Quasi-Newton Methods". In Murray, W. (ed.). Numerical Methods
  for Unconstrained Optimization. London: Academic Press. pp. 87–106. ISBN
  0-12-512250-0.
