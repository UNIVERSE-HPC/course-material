---
name: Graphs
dependsOn: []
tags: []
learningOutcomes:
  - Be able to describe the fundamental components of a graph
  - Find and interpret the gradient and intercepts of a straight line
  - Be able to sketch the graph of a function from its formula, using its zeros, its sign, and its behaviour for small and large values
  - Be able to rearrange a model into straight-line form, and use the resulting graph to test the model and estimate its parameters
attribution:
  - citation: This material has been adapted from material by Fergus Cooper and others from the "Essential Mathematics" module at the Doctoral Training Centre, University of Oxford.
    url: https://www.dtc.ox.ac.uk/
    image: ./fig/dtc_hex.svg
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

---

## YouTube lecture recording from October 2020

A recording of this lecture from October 2020; the material has changed a little since.

[![Youtube lecture thumbnail](fig/youtube01.jpg)](https://youtu.be/HIRpQ1caeKk)

---

## Why graphs?

A graph is where a model meets data.
We use graphs in two directions:

- **From formula to picture:** what shape does this model predict?
  This is _sketching_.
- **From picture to formula:** which model describes these data, and what are its parameters?
  This is _linearising_.

## Anatomy of a graph

Terminology:

- $Y$ or $y$ is the _dependent_ variable, sometimes called the _ordinate_,
  marked on the vertical axis
- $X$ or $x$ is the _independent_ variable, sometimes called the _abscissa_,
  marked on the horizontal axis
- The dependent variable is said to be graphed _against_ the independent
  variable

Essential features:

- Title
- Axis labels (and units if appropriate)

![Example graph with a title and axis labels](fig/01_01_graph.svg)

## Equation of a straight line

Defined by a gradient, $m$, and a $y$-axis intercept, $c$:

$$
y = m x + c
$$

Interpretation:

- The intercept of this line on the $y$ axis is given by $y=c$, since at $x=0$,
  $y = c$

- The gradient of this line (also called its "slope") is given by

  $$
  m = \frac{y_2-y_1}{x_2 - x_1}
  $$

  ("change in $y$ divided by change in $x$")

- The gradient is a _rate of change_: how much $y$ changes for each unit change in $x$

- The intercept of this line on the $x$ axis is given by $x = -c/m$,
  since at $y=0$ we must have $mx=-c$

## Sketching graphs

Given a formula, what shape is its graph?
Polynomials are a good place to start.

### Graphs of polynomials

An expression involving higher powers of $x$ is called a _polynomial_ in $x$:

$$
y = a_n x^n + a_{n-1} x^{n-1} + a_{n-2} x^{n-2}+\ldots+a_1 x^1 + a_0x^0
$$

The highest power, $n$, is its _degree_.
The graph of a polynomial of degree $n$ has at most $n-1$ bends (turning points) in it.

### Sketching a polynomial

Consider

$$
y=x^5-5x^3+4x
$$

- **Zeros:** it factorises as

  $$
  y = x(x+1)(x-1)(x+2)(x-2)
  $$

  so $y=0$ at $x = -2, -1, 0, 1, 2$

- **Large $|x|$:** the $x^5$ term dominates, so $y$ rises to the right and falls to the left

- **Bends:** degree 5, so at most 4 bends, just enough to pass through all five zeros

- **Sign:** each zero comes from a single factor, so $y$ changes sign there

That is enough to sketch it. Plotting it confirms the sketch:

![Example graph of a polynomial](fig/01_02_polynomial.svg)

### A checklist for sketching

For any function, ask:

- Where is it zero? Where is it positive, and where negative?
- What happens as $x\to 0$, and as $x\to\pm\infty$? Which term dominates?
- Is it undefined anywhere, or does it blow up (an _asymptote_)?
- How many bends can it have? (Differentiation will find them exactly.)

Then check your sketch by plotting the function in Python.

## From data to a model

Going the other way: given some data, which model fits, and what are its parameters?

- Curves are hard to tell apart by eye: is a trend quadratic, exponential, or something else?
- Straight lines are easy: we see at once when points leave a line, and a line has just two numbers, its gradient and intercept.
- So transform the data so that, **if the model is right**, the points lie on a straight line.

### Linearising: the recipe

1. Write down the model, e.g. $y = ax^2 + b$.
2. Rearrange it into the form $Y = mX + c$, where $X$ and $Y$ depend only on the data, and $m$ and $c$ only on the parameters.
   Here $Y = y$, $X = x^2$, $m = a$ and $c = b$.
3. Plot $Y$ against $X$.
   A straight line supports the model; a systematic curve rules it out.
4. Read off the gradient and intercept, and work back to the parameters.

### Example: is this quadratic?

The trend is clearly curved. Is it a parabola, $y = ax^2 + b$?

![Graph of data with nonlinear trend](fig/01_03_nonlinear_trend.svg)

#### Testing the model

If the parabolic relationship holds, plotting $Y=y$ against $X=x^2$ should result in a straight line.
Plotting against $x$ or $x^3$ instead would leave the points on a curve.

![Graph of data transformed to a linear trend](fig/01_04_linear_trend.svg)

#### Gradient and intercept

We next add a trendline through these points which we can use to determine the gradient and intercept.

![Graph of linear trend with trendline](fig/01_05_linear_trend_with_trendline.svg)

- We find $(X,Y)$ lie along a straight line with slope 5 and $Y$-intercept 87.

- This means that $Y=5X+87$, so $a = 5$ and $b = 87$.

- So, $y$ and $x$ can be modelled by the polynomial equation $y=5x^2+87$.

### Example: enzyme kinetics

The _Michaelis–Menten_ model relates the rate, $V$, of an enzyme-catalysed reaction to the substrate concentration, $S$:

$$
V = \frac{V_{\max}S}{K_m + S}
$$

where $V_{\max}$ is the maximum rate, and $K_m$ is the substrate concentration at which the rate is half its maximum.

First, plot the original data, which are observations of $V$ given varying $S$:

![Graph of original data](fig/01_06_original_data.svg)

#### Straight-line form

Take the reciprocal of both sides:

$$
\frac{1}{V} = \frac{K_m + S}{V_{\max}S} = \frac{K_m}{V_{\max}}\cdot\frac{1}{S} + \frac{1}{V_{\max}}
$$

- So plot $Y=1/V$ against $X=1/S$: this is the _Lineweaver–Burk_ plot

- It will have gradient $m = K_m/V_{\max}$ and ordinate intercept $c = 1/V_{\max}$

#### Transformed data

If the Michaelis–Menten model holds, plotting $Y=1/V$ against $X=1/S$ should result in a straight line.

![Graph of linear trend](fig/01_07_linear_trend.svg)

#### Gradient and intercept

We next add a trendline through these points which we can use to determine the gradient and intercept.

![Graph of linear trend with trendline](fig/01_08_linear_trend_with_trendline.svg)

- We find $(X,Y)$ lie along a straight line with slope 3 and $Y$-intercept 5.

- This means that $Y=3X+5$

- Working back, $V_{\max} = 1/c = 0.2$ and $K_m = m/c = 0.6$.

- So the data are modelled by $V = 0.2S/(0.6+S)$.

### Linearising in practice

- Transforming the data transforms its noise too: taking $1/V$ magnifies the errors in small values of $V$, and those points pull hardest on the line.
- So today we would usually fit the nonlinear model directly, e.g. with `scipy.optimize.curve_fit`.
- But a straight-line plot is still the clearest check that a model fits, and you'll meet them throughout the literature: Lineweaver–Burk, Arrhenius, semi-log growth curves.
- Next: logarithms, which straighten exponentials and power laws.

## Introductory problems

::::challenge{id="01_intro_01" title="Introductory problems 1"}
Sketch the following graphs.
First, use pen & paper, then use Python to check your answers.
The notation after each equation indicates the range of values that the variable can take.

1. $y = 3x+5 \qquad\qquad x\in[0,10]$
1. $y = x^2/2 - 4 \qquad\qquad x\in[-4,4]$
1. $y = \sqrt{x} \qquad\qquad x\in[0,25]$
1. $y = \sin(t) \qquad\qquad t\in[0,2\pi]$
1. $y = \sin(2t) \qquad\qquad t\in[0,2\pi]$
1. $y = 1/x \qquad\qquad x\in[0.1,5]$
1. $y = -1/x^2 \qquad\qquad x\in[0.1,5]$

```python
# hint
import numpy as np
from matplotlib import pyplot as plt

x = np.linspace(0, 10, 100)
y = 3 * x + 5

plt.plot(x, y)
```

::::

::::challenge{id="01_intro_02" title="Introductory problems 2"}
For which values of $x$ are the following functions positive? Negative? Zero?

1. $x^2 - 9$
1. $\sin(x)$
1. $\sin(3x)$
1. $2/x - 1/x^2$

::::

## Main problems

Text relevant to these problems: Croft and Davison, 5th Edition, Chapters 17 & 18.

::::challenge{id="01_main_01" title="Main problems 1"}
The Lennard-Jones potential energy between two non-polar atoms may be given by the equation:

$$
V(R)=\frac{A}{R^{12}} - \frac{B}{R^6}
$$

where $A$ and $B$ are positive constants, $V(R)$ is the potential energy, measured in Joules, and $R$ is the internuclear distance measured in $\AA$.

1. For which values of $R$ is $V(R)$ positive? Negative? Zero?
1. Sketch a graph (on paper) showing the potential energy between the two atoms as a function of $R$ given that $A=0.06$ and $B = 0.03$.
1. What is the potential energy between the two atoms at infinite separation?
1. What would happen to the two atoms if they were brought very close together?
1. What is the physical interpretation of the sign of $V(R)$, and of its slope?
1. What are the dimensions ([Length], [Mass], [Time]) and units of the constants $A$ and $B$?
1. Use Python to plot the graph of $V$ versus $R$ for $A=0.06$ and $B = 0.03$. Remember to add relevant axis labels. Plot on the same graph the line of $V = 0$, so you can verify your answers to **1.** and **2.**

::::

::::challenge{id="01_main_02" title="Main problems 2"}
How should these equations be rearranged to allow the plotting of a suitable **linear** graph, assuming that the constant parameters $a$ and $b$ are unknown, and we wish to use the graph to find them?
Write down expressions for the gradient, $X$-intercept and $Y$-intercept of each rearranged equation:

1. $y = a/x$
1. $y = b - a \sqrt{x}$
1. $y = b/(1+ax)$

::::

::::challenge{id="01_main_03" title="Main problems 3"}
The _osmotic pressure_ of a solution of a protein is related to the concentration of that protein by the equation:

$$
Z = RTb
$$

where $Z$ is the osmotic pressure in kPa, $T$ is the temperature in Kelvin, $R$ is the gas constant ($R=8.314\,\text{kPa}\cdot\text{dm}^3\cdot\text{mol}^{-1}\cdot\text{K}^{-1}$) and $b$ is the molarity of the protein (mol. solute per dm$^3$ solution).
Plot a suitable graph to determine, as accurately as possible, the molecular mass (take care with units!) of the protein given the following data taken at room temperature (usually taken as 21$^{\circ}$C):

|                                        |       |       |       |       |       |
| -------------------------------------- | :---: | :---: | :---: | :---: | :---: |
| Protein concentration (in g dm$^{-3}$) |  7.3  | 18.4  | 27.6  | 42.1  | 57.4  |
| Osmotic pressure (in kPa)              | 0.211 | 0.533 | 0.804 | 1.236 | 1.701 |

Hint: compare the function with the equation of a straight line, $y=mx+c$, and think about the relationship between concentration, molar concentration and molecular weight.

Use Python to plot the graph and confirm your pen & paper solution.
::::

## Extension problems

::::challenge{id="01_ext_01" title="Extension problems 1"}
The rate at which a given enzyme catalyses a reaction is dependent upon the substrate concentration:

$$
V = \frac{S}{m+cS}
$$

where $V$ is the rate of the reaction, $S$ is the substrate concentration and $m$ and $c$ are _unknown_ constants.
How can we transform $V$ and $S$ to derive a straight line graph relating them?
What will be the gradient and the ordinate intercepts?
::::
