---
name: Differentiation 3
dependsOn: [scientific_computing.essential_maths.04_differentiation_2]
tags: []
learningOutcomes:
  - Be able to calculate the derivative of exponential and logarithmic functions
  - Understand how to differentiate functions of multiple independent variables
  - Be able to calculate higher order derivatives with respect to multiple variables
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

[![Youtube lecture thumbnail](fig/youtube05.jpg)](https://youtu.be/zIgJF-4xu90)

---

## Why exponentials and partial derivatives?

In this lecture we use the chain rule to differentiate the exponential and logarithmic functions from the lecture on indices and logs.
We need these whenever a quantity grows or decays (e.g. $e^{-kt}$).

We then look at functions of more than one variable (e.g. the temperature at each point on a plate).
We often want to know how such a function changes when just one of its variables changes.
This is called _partial differentiation_, and we will need it when we study systems of differential equations.

## Examples of applying chain rule to the exponential function

1. $y=e^{-ax}$

   Let $u=-ax$, so $du/dx=-a$. Thus $y=e^u$ and

   $$
   \frac{dy}{du}=e^u \quad\Rightarrow\quad \frac{dy}{dx}=\frac{dy}{du}\times\frac{du}{dx}=e^u\times(-a)=-ae^{-ax}.
   $$

1. $y = e^{x^2}$

   Then, letting $u = x^2$:

   $$
   \frac{d}{dx}e^{x^2}=\frac{dy}{dx}=\frac{dy}{du}\times\frac{du}{dx}=e^u\cdot 2x = e^{x^2}\cdot 2x.
   $$

So an important generalisation is:

$$
\frac{d}{dx}e^{f(x)}=e^{f(x)}f'(x)
$$

for any function $f(x)$.

## Example with the natural logarithm

1. $y=\ln\left((a-x)^2\right)=2\ln|a-x|=2\ln|u|$.

   Let $u=(a-x)$:

   $$
   \Rightarrow \frac{du}{dx}=-1 \quad\text{and}\quad \frac{dy}{du}=\frac{2}{u} \quad\text{Thus}\quad \frac{dy}{dx}=\frac{2}{u}\times (-1)=\frac{-2}{a-x}
   $$

This also generalises:

$$
\frac{d}{dx}\ln(f(x)) = \frac{f'(x)}{f(x)}
$$

## The derivative of $a^x$

For $a>0$, by the properties of logarithms and indices we have

$$
a^x = \left(e^{\ln a}\right)^x=e^{\left(x\cdot\ln a\right)}.
$$

Thus, as we saw above we have:

$$
\frac{d}{dx}a^x = \frac{d}{dx}e^{\left(x\cdot\ln a\right)} = e^{\left(x\cdot\ln a\right)}\frac{d}{dx}\left(x\cdot\ln a\right) =a^x\cdot\ln a
$$

Similarly, in general:

$$
\frac{d}{dx}a^{f(x)} = a^{f(x)}\cdot \ln a\cdot f'(x)
$$

### SymPy example

Let's try and use SymPy to check this:

```python
import sympy as sp
x, a = sp.symbols('x a') # declare the variables x and a
f = sp.Function('f')     # declare a function dependent on another variable
sp.diff(a**f(x),x)       # write the expression we wish to evaluate
```

$$
a^{f{\left(x \right)}} \log{\left(a \right)} \frac{d}{d x} f{\left(x \right)}
$$

(SymPy writes $\log$ for the natural logarithm, $\ln$.)

## The derivative of $\log_a x$

For $a>0$, $a\neq 1$, recall the conversion formula $\log_a x = \ln x/\ln a$ and note that $\ln a$ is a constant.
Thus:

$$
\begin{aligned}
\frac{d}{dx}\log_a x &= \frac{d}{dx}\left(\frac{1}{\ln a}\cdot\ln x\right) \\
&= \left(\frac{1}{\ln a}\right)\cdot\frac{d}{dx}\ln x \\
&= \left(\frac{1}{\ln a}\right)\cdot\frac{1}{x} \\
&= \frac{1}{x\cdot\ln a}
\end{aligned}
$$

In general:

$$
\frac{d}{dx}\log_a f(x) = \frac{f'(x)}{f(x)(\ln a)}
$$

### SymPy example

Again, let's use SymPy to check this:

```python
x, a = sp.symbols('x a')  # declare the variables x and a
f = sp.Function('f')      # declare a function dependent on another variable
sp.diff(sp.log(f(x),a),x) # write the expression we wish to evaluate
```

$$
\frac{\frac{d}{d x} f{\left(x \right)}}{f{\left(x \right)} \log{\left(a \right)}}
$$

## Further examples

1. Product rule: let $y = x^2\,e^x$. Then:

   $$
   \frac{dy}{dx}=\frac{d}{dx}x^2e^x=\frac{d}{dx}x^2\cdot e^x+x^2\cdot\frac{d}{dx}e^x = (2x + x^2)e^x
   $$

1. Quotient rule: let $y = e^x/x$. Then:

   $$
   \frac{dy}{dx}=\frac{\frac{d}{dx}e^x\cdot x - e^x\cdot \frac{d}{dx}x}{x^2}=\frac{e^x\cdot x - e^x\cdot 1}{x^2}=\frac{x - 1}{x^2}e^x
   $$

1. Chain rule: $y = e^{x^2}$. Then, letting $f(x) = x^2$:

   $$
   \frac{d}{dx}e^{x^2} = e^{f(x)}f'(x) = e^{x^2}\cdot 2x
   $$

1. $y=\ln (x^2 + 1)$. Then, letting $f(x) = x^2+1$:

   $$
   \frac{d}{dx}\ln(x^2+1) = \frac{f'(x)}{f(x)} = \frac{2x}{x^2+1}
   $$

1. $$
   \frac{d}{dx}2^{x^3}=2^{x^3}\cdot\ln 2\cdot 3x^2
   $$

1. $$
   \frac{d}{dx}10^{x^2+1}=10^{x^2+1}\cdot\ln 10\cdot 2x
   $$

1. $$
   \frac{d}{dx}\log_{10}(7x+5)=\frac{7}{(7x+5)\cdot \ln10}
   $$

1. $$
   \frac{d}{dx}\log_2(3^x+x^4)=\frac{3^x\cdot\ln3 + 4x^3}{\ln 2\cdot(3^x+x^4)}
   $$

## Functions of several variables: partial differentiation

**Definition:** Given a function $z=f(x,y)$ of two variables $x$ and $y$, the **partial derivative of $z$ with respect to $x$** is the function obtained by differentiating $f(x,y)$ with respect to $x$, holding $y$ constant.

We denote this using $\partial$ (a "curly" d, sometimes pronounced "del") as shown below:

$$
\frac{\partial z}{\partial x}=\frac{\partial}{\partial x}f(x,y) = f_x(x,y)
$$

## Example 1

$$
f(x,y)=z=x^2-2y^2
$$

$$
f_x=\frac{\partial z}{\partial x}=2x\qquad\text{and}\qquad f_y=\frac{\partial z}{\partial y}=-4y
$$

## Example 2

Let $z=3x^2y+5xy^2$. Then the partial derivative of $z$ with respect to $x$, holding $y$ fixed, is:

$$
\begin{aligned}
\frac{\partial z}{\partial x}&=\frac{\partial}{\partial x}\,\left(3x^2y+5xy^2\right) \\
&=3y\cdot 2x + 5y^2\cdot 1 \\
&=6xy+5y^2
\end{aligned}
$$

while the partial of $z$ with respect to $y$ holding $x$ fixed is:

$$
\begin{aligned}
\frac{\partial z}{\partial y}&=\frac{\partial}{\partial y}\,\left(3x^2y+5xy^2\right) \\
&=3x^2\cdot 1 + 5x\cdot 2y = 3x^2+10xy
\end{aligned}
$$

### SymPy example

Above, we had:

$$
\frac{\partial}{\partial x}\,\left(3x^2y+5xy^2\right) = 6xy+5y^2
$$

Let's redo this in SymPy:

```python
x, y = sp.symbols('x y')
sp.diff(3*x**2*y + 5*x*y**2,x)
```

$$
6 x y + 5 y^{2}
$$

## Higher-order partial derivatives

Given $z = f(x,y)$ there are now four distinct possibilities for the
second-order partial derivatives.

1. With respect to $x$ twice:

   $$
   \frac{\partial}{\partial x}\left(\frac{\partial z}{\partial x}\right) =\frac{\partial^2z}{\partial x^2} =z_{xx}
   $$

1. With respect to $y$ twice:

   $$
   \frac{\partial}{\partial y}\left(\frac{\partial z}{\partial y}\right) =\frac{\partial^2z}{\partial y^2} =z_{yy}
   $$

1. First with respect to $x$, then with respect to $y$:

   $$
   \frac{\partial}{\partial y}\left(\frac{\partial z}{\partial x}\right) =\frac{\partial^2z}{\partial y\partial x} =z_{xy}
   $$

1. First with respect to $y$, then with respect to $x$:

   $$
   \frac{\partial}{\partial x}\left(\frac{\partial z}{\partial y}\right) =\frac{\partial^2z}{\partial x\partial y} =z_{yx}
   $$

## Example: Laplace's equation for equilibrium temperature distribution on a copper plate

Let $T(x,y)$ give the temperature at the point $(x,y)$.

According to a result of the French mathematician Pierre-Simon Laplace (1749 - 1827), at every point $(x,y)$ the second-order partials of $T$ must satisfy the equation:

$$
T_{xx} + T_{yy} = 0
$$

We can verify that the function $T(x,y)=y^2-x^2$ satisfies Laplace's equation:

First with respect to $x$:

$$
T_x(x,y)=0-2x=-2x\qquad\text{so}\qquad T_{xx}(x,y)=-2
$$

Then with respect to $y$:

$$
T_y(x,y)=2y-0=2y\qquad\text{so}\qquad T_{yy}(x,y)=2
$$

Finally:

$$
T_{xx}(x,y)+T_{yy}(x,y) = -2 + 2 = 0
$$

which proves the result.

The function $z=x^2y - xy^2$ does _not_ satisfy Laplace's equation (and so cannot be a model for thermal equilibrium).

First note that

$$
\begin{aligned}
z_x &= 2xy - y^2 \\
z_{xx} &= 2y
\end{aligned}
$$

and that

$$
\begin{aligned}
z_y &= x^2 - 2xy \\
z_{yy} &= -2x
\end{aligned}
$$

Therefore:

$$
z_{xx}+z_{yy}=2y-2x
$$

which is not zero everywhere (it is only zero on the line $y=x$).

We can also verify this in SymPy like so:

```python
T1 = y**2 - x**2
sp.diff(T1, x, x) + sp.diff(T1, y, y)
```

$$
0
$$

and for the second function:

```python
T2 = x**2*y - x*y**2
sp.diff(T2, x, x) + sp.diff(T2, y, y)
```

$$
- 2 x + 2 y
$$

## A note on the mixed partials $f_{xy}$ and $f_{yx}$

For almost any function you will meet, $f_{xy}=f_{yx}$, so it does not matter in which order we take the partial derivatives (there are exceptions, but you are unlikely to meet them).

### Example

Let $z = x^2y^3+3x^2-2y^4$. Then $z_x=2xy^3+6x$ and $z_y = 3x^2y^2-8y^3$.

Taking the partial of $z_x$ with respect to $y$ we get

$$
z_{xy}=\frac{\partial}{\partial y}\left(2xy^3+6x\right)=6xy^2
$$

Taking the partial of $z_y$ with respect to $x$ we get the same thing:

$$
z_{yx}=\frac{\partial}{\partial x}\left(3x^2y^2-8y^3\right)=6xy^2
$$

So the operators $\partial/\partial x$ and $\partial/\partial y$ are **commutative**:

$$
\text{i.e.} \qquad \frac{\partial}{\partial x}\left(\frac{\partial z}{\partial y}\right) = \frac{\partial}{\partial y}\left(\frac{\partial z}{\partial x}\right)
$$

## Introductory problems

:::callout{variant="tip"}
For more practice, an AI tool can write new problems like the ones below.
Ask it for problems of the same type and difficulty, or for problems set in your own field.
For example, you could ask it to "write five chemistry problems on partial derivatives, at the level of these ones".
Work them on paper first, then ask the tool to check your answers and to explain any step you are unsure of.
:::

::::challenge{id="05_intro_01" title="Introductory problems 1"}
Differentiate the following functions with respect to $x$, using the stated rules where indicated:

1. Product rule: $\sqrt{x}\,e^x$
1. Product rule: $3x^2 \ln(x)$
1. Chain rule: $e^{-4x^3}$
1. Chain rule: $\ln \sqrt{x^{3/2}/6}$
1. Chain rule: $10^{x^2}$
1. Any rules: $\ln x/(5x-7)$
1. Any rules: $e^x/(2x^3-1)$
1. Any rules: $\log_2\left(x\cos(x)\right)$

:::solution

1. By the product rule,

   $$
   \frac{d}{dx}\left(\sqrt{x}\,e^x\right) = \frac{1}{2\sqrt{x}}\,e^x + \sqrt{x}\,e^x = e^x\left(\frac{1}{2\sqrt{x}} + \sqrt{x}\right)
   $$

1. By the product rule,

   $$
   \frac{d}{dx}\left(3x^2\ln(x)\right) = 6x\ln(x) + 3x^2\cdot\frac{1}{x} = 6x\ln(x) + 3x
   $$

1. Let $u = -4x^3$, so that $du/dx = -12x^2$. Then

   $$
   \frac{d}{dx}\left(e^{-4x^3}\right) = e^{u}\,\frac{du}{dx} = -12x^2e^{-4x^3}
   $$

1. It is easiest to simplify first, using the laws of logarithms:

   $$
   \ln\sqrt{x^{3/2}/6} = \frac{1}{2}\left(\ln\left(x^{3/2}\right) - \ln(6)\right) = \frac{3}{4}\ln(x) - \frac{1}{2}\ln(6)
   $$

   The second term is a constant, so

   $$
   \frac{d}{dx}\left(\ln\sqrt{x^{3/2}/6}\right) = \frac{3}{4x}
   $$

   (The chain rule gives the same answer, with more work.)

1. Let $u = x^2$, so that $du/dx = 2x$. Using the derivative of $a^u$,

   $$
   \frac{d}{dx}\left(10^{x^2}\right) = 10^{u}\ln(10)\,\frac{du}{dx} = 2x\ln(10)\,10^{x^2}
   $$

1. By the quotient rule,

   $$
   \frac{d}{dx}\left(\frac{\ln(x)}{5x-7}\right) = \frac{\frac{1}{x}(5x-7) - 5\ln(x)}{(5x-7)^2} = \frac{5x - 7 - 5x\ln(x)}{x(5x-7)^2}
   $$

1. By the quotient rule,

   $$
   \frac{d}{dx}\left(\frac{e^x}{2x^3-1}\right) = \frac{e^x(2x^3-1) - 6x^2e^x}{(2x^3-1)^2} = \frac{e^x\left(2x^3 - 6x^2 - 1\right)}{(2x^3-1)^2}
   $$

1. Let $u = x\cos(x)$, so that, by the product rule, $du/dx = \cos(x) - x\sin(x)$. Using the derivative of $\log_a u$,

   $$
   \begin{aligned}
   \frac{d}{dx}\left(\log_2\left(x\cos(x)\right)\right) &= \frac{1}{u\ln(2)}\,\frac{du}{dx} \\
   &= \frac{\cos(x) - x\sin(x)}{x\cos(x)\ln(2)} \\
   &= \frac{1}{\ln(2)}\left(\frac{1}{x} - \tan(x)\right)
   \end{aligned}
   $$

:::

::::

::::challenge{id="05_intro_02" title="Introductory problems 2"}
If $y=e^{-ax}$, show that

$$
2\frac{d^2y}{dx^2}+a\frac{dy}{dx}-a^2y=0.
$$

:::solution

By the chain rule,

$$
\frac{dy}{dx} = -ae^{-ax} = -ay \qquad\text{and}\qquad \frac{d^2y}{dx^2} = a^2e^{-ax} = a^2y
$$

Substituting these into the left-hand side,

$$
2\frac{d^2y}{dx^2}+a\frac{dy}{dx}-a^2y = 2a^2y - a^2y - a^2y = 0
$$

:::

::::

::::challenge{id="05_intro_03" title="Introductory problems 3"}
If $y=e^{-x}\sin(x)$, show that

$$
\frac{d^2y}{dx^2} + 2\frac{dy}{dx} + 2y = 0.
$$

:::solution

By the product rule,

$$
\begin{aligned}
\frac{dy}{dx} &= -e^{-x}\sin(x) + e^{-x}\cos(x) = e^{-x}\left(\cos(x) - \sin(x)\right) \\
\frac{d^2y}{dx^2} &= -e^{-x}\left(\cos(x) - \sin(x)\right) + e^{-x}\left(-\sin(x) - \cos(x)\right) = -2e^{-x}\cos(x)
\end{aligned}
$$

Substituting these into the left-hand side,

$$
\begin{aligned}
\frac{d^2y}{dx^2} + 2\frac{dy}{dx} + 2y &= -2e^{-x}\cos(x) + 2e^{-x}\left(\cos(x) - \sin(x)\right) + 2e^{-x}\sin(x) \\
&= 0
\end{aligned}
$$

:::

::::

::::challenge{id="05_intro_04" title="Introductory problems 4"}
Find the partial derivatives $f_x$ and $f_y$ of the following functions:

1. $f(x,y) = x^3 + 4xy^2 - y^4$
1. $f(x,y) = e^{2x}\sin(y)$
1. $f(x,y) = \ln\left(x^2 + y^2\right)$

:::solution

1. Holding $y$ constant, then holding $x$ constant:

   $$
   f_x = 3x^2 + 4y^2 \qquad\text{and}\qquad f_y = 8xy - 4y^3
   $$

1. When we differentiate with respect to $x$, $\sin(y)$ is a constant, and when we differentiate with respect to $y$, $e^{2x}$ is a constant:

   $$
   f_x = 2e^{2x}\sin(y) \qquad\text{and}\qquad f_y = e^{2x}\cos(y)
   $$

1. Use the chain rule, with $u = x^2 + y^2$:

   $$
   f_x = \frac{1}{x^2+y^2}\times 2x = \frac{2x}{x^2+y^2} \qquad\text{and}\qquad f_y = \frac{2y}{x^2+y^2}
   $$

:::

::::

## Main problems

::::challenge{id="05_main_01" title="Main problems 1"}
The power, $W$, that a certain machine develops is given by the formula

$$
W=EI-RI^2
$$

where $I$ is the current and $E$ and $R$ are positive constants.

Find the maximum value of $W$ as $I$ varies.

:::solution

$$
\frac{dW}{dI} = E - 2RI = 0 \quad\implies\quad I = \frac{E}{2R}
$$

To show this is a maximum, we check the second derivative:

$$
\frac{d^2W}{dI^2} = -2R < 0
$$

since $R$ is positive.
So the maximum power is

$$
W = E\cdot\frac{E}{2R} - R\left(\frac{E}{2R}\right)^2 = \frac{E^2}{2R} - \frac{E^2}{4R} = \frac{E^2}{4R}
$$

:::

::::

::::challenge{id="05_main_02" title="Main problems 2"}
Environmental health officers monitoring an outbreak of food poisoning after a wedding banquet were able to model the time course of the recovery of the guests using the equation:

$$
r = \frac{100t}{1+t}
$$

where $t$ represents the number of days since infection and $r$ is the percentage of guests who no longer display symptoms.
Determine an expression for the rate of recovery.

:::solution

The rate of recovery is $dr/dt$.
By the quotient rule,

$$
\frac{dr}{dt} = \frac{100(1+t) - 100t}{(1+t)^2} = \frac{100}{(1+t)^2}
$$

in percent of guests per day.
This is largest at $t=0$ (100% per day), and gets smaller as time goes on, so the guests recover quickly at first and then more slowly.

:::

::::

::::challenge{id="05_main_03" title="Main problems 3"}
An experiment called 'the reptilian drag race' looks at how agamid lizards accelerate from a standing start.

The distance $x$ travelled in time $t$ on a horizontal surface has been modelled as

$$
x = v_{\text{max}} \left(t + \frac{e^{-kt}}{k} - \frac{1}{k}\right),
$$

where $v_{\text{max}}$ is the maximum velocity, and $k$ is a rate constant.

1. Find expressions for the velocity $v$ and acceleration $a$ as functions of time.
1. For $v_{\text{max}} = 3\,$m$\,$s$^{-1}$ and $k=10\,$s$^{-1}$, sketch $x$, $v$ and $a/10$ on the same axes for $0\leq t\leq 1\,$s.

:::solution

1. Differentiating once and then again,

   $$
   v = \frac{dx}{dt} = v_{\text{max}}\left(1 - e^{-kt}\right) \qquad\text{and}\qquad a = \frac{dv}{dt} = v_{\text{max}}\,k\,e^{-kt}
   $$

   So the lizard starts from rest, and its velocity approaches $v_{\text{max}}$ as $t$ increases.

1. With these values, $x = 3\left(t + e^{-10t}/10 - 1/10\right)$, $v = 3\left(1-e^{-10t}\right)$ and $a/10 = 3e^{-10t}$.

   - $x$ starts at 0 with zero gradient, curves upwards, and soon becomes close to a straight line of gradient 3, reaching about 2.7 m at $t=1\,$s.
   - $v$ rises from 0 towards 3 m$\,$s$^{-1}$, and is already at 95% of this by $t=0.3\,$s.
   - $a/10$ starts at 3 and decays towards 0, so the lizard does almost all of its accelerating in the first few tenths of a second.

   ```python
   import numpy as np
   from matplotlib import pyplot as plt

   v_max = 3
   k = 10
   t = np.linspace(0, 1, 200)
   x = v_max * (t + np.exp(-k * t) / k - 1 / k)
   v = v_max * (1 - np.exp(-k * t))
   a = v_max * k * np.exp(-k * t)

   plt.plot(t, x, label='x (m)')
   plt.plot(t, v, label='v (m/s)')
   plt.plot(t, a / 10, label='a/10 (m/s$^2$)')
   plt.xlabel('t (s)')
   plt.legend()
   ```

:::

::::

::::challenge{id="05_main_04" title="Main problems 4"}
The distance $x$ that a particular organism travels over time from its starting location is modelled by the equation

$$
x(t) = t^2 e^{k(1-t)},
$$

where $k$ is a positive constant and $0\leq t\leq 1\,$s.

1. Sketch $x$ over time for $k=1/2$ and $k=3$.
1. Calculate an expression for the organism's velocity as a function of time.
1. What is the largest value of $k$ such that the organism never starts moving back towards where it started?

:::solution

1. For every $k$, $x(0) = 0$ and $x(1) = 1$.
   For $k=1/2$, $x$ increases all the way from 0 to 1.
   For $k=3$, $x$ rises to a maximum of $(4/9)e \approx 1.21$ at $t = 2/3$ (see part **2.**), then falls back to 1 at $t=1$.

   ```python
   import numpy as np
   from matplotlib import pyplot as plt

   t = np.linspace(0, 1, 200)
   for k in [0.5, 3]:
       plt.plot(t, t**2 * np.exp(k * (1 - t)), label=f'k = {k}')
   plt.xlabel('t (s)')
   plt.ylabel('x')
   plt.legend()
   ```

1. By the product rule and the chain rule,

   $$
   v = \frac{dx}{dt} = 2te^{k(1-t)} + t^2\left(-k\right)e^{k(1-t)} = t\left(2 - kt\right)e^{k(1-t)}
   $$

1. The organism moves back towards its start when $v<0$.
   For $0 < t \leq 1$, both $t$ and $e^{k(1-t)}$ are positive, so $v$ has the same sign as $2 - kt$.
   This means that $v < 0$ when $t > 2/k$.
   So, the organism never moves back during $0\leq t\leq 1$ if $2/k \geq 1$, i.e. if $k \leq 2$.
   The largest value is $k=2$.

:::

::::

::::challenge{id="05_main_05" title="Main problems 5"}
The function

$$
S = S_{\text{max}}(1- e^{-t/\tau})
$$

is used to describe sediment thickness accumulating in an extensional basin through time.
What is the sedimentation rate?

:::solution

The sedimentation rate is $dS/dt$.
By the chain rule,

$$
\frac{dS}{dt} = S_{\text{max}}\cdot\left(-e^{-t/\tau}\right)\cdot\left(-\frac{1}{\tau}\right) = \frac{S_{\text{max}}}{\tau}\,e^{-t/\tau}
$$

The rate starts at $S_{\text{max}}/\tau$, and decreases towards zero as the thickness approaches $S_{\text{max}}$.

:::

::::

::::challenge{id="05_main_06" title="Main problems 6"}
Which of these functions could describe the equilibrium temperature $T(x,y)$ of a copper plate?
That is, which satisfy Laplace's equation, $T_{xx} + T_{yy} = 0$?

1. $T = x^3 - 3xy^2$
1. $T = e^x\sin(y)$
1. $T = x^2 + y^2$
1. $T = \ln\left(x^2 + y^2\right)$, for $(x,y) \neq (0,0)$

:::solution

1. $T_x = 3x^2 - 3y^2$, so $T_{xx} = 6x$.
   $T_y = -6xy$, so $T_{yy} = -6x$.
   So, $T_{xx} + T_{yy} = 6x - 6x = 0$, and $T$ satisfies Laplace's equation.

1. $T_{xx} = e^x\sin(y)$ and $T_{yy} = -e^x\sin(y)$.
   So, $T_{xx} + T_{yy} = 0$, and $T$ satisfies Laplace's equation.

1. $T_{xx} = 2$ and $T_{yy} = 2$.
   So, $T_{xx} + T_{yy} = 4$, which is not zero, and $T$ does _not_ satisfy Laplace's equation.

1. From Introductory problems 4, $T_x = 2x/(x^2+y^2)$.
   Using the quotient rule,

   $$
   T_{xx} = \frac{2(x^2+y^2) - 2x\times 2x}{(x^2+y^2)^2} = \frac{2(y^2-x^2)}{(x^2+y^2)^2}
   $$

   In the same way, $T_y = 2y/(x^2+y^2)$, and

   $$
   T_{yy} = \frac{2(x^2-y^2)}{(x^2+y^2)^2}
   $$

   These add to zero, so $T$ satisfies Laplace's equation (everywhere except the origin, where it is not defined).

:::

::::

## Extension problems

::::challenge{id="05_ext_01" title="Extension problems 1"}
Let

$$
z = \frac{2}{3}x^3 - \frac{3}{4}x^2y + \frac{2}{5}y^3.
$$

1. Find $z_x$ and $z_y$
1. Find $z_{xx}$ and $z_{yy}$
1. Show that $z_{xy} = z_{yx}$

:::solution

1. Treating $y$ as a constant, and then $x$ as a constant,

   $$
   z_x = 2x^2 - \frac{3}{2}xy \qquad\text{and}\qquad z_y = -\frac{3}{4}x^2 + \frac{6}{5}y^2
   $$

1. Differentiating again with respect to the same variable,

   $$
   z_{xx} = 4x - \frac{3}{2}y \qquad\text{and}\qquad z_{yy} = \frac{12}{5}y
   $$

1. Differentiating $z_x$ with respect to $y$, and $z_y$ with respect to $x$,

   $$
   z_{xy} = -\frac{3}{2}x \qquad\text{and}\qquad z_{yx} = -\frac{3}{2}x
   $$

   so $z_{xy} = z_{yx}$.

:::

::::

::::challenge{id="05_ext_02" title="Extension problems 2"}
Show that $f_{xy}=f_{yx}$ for the following functions:

1. $f(x,y) = x^2 - xy + y^3$
1. $f(x,y) = e^y\,\ln(2x-y)$
1. $f(x,y) = 2\,x\,y\,e^{2xy}$
1. $f(x,y) = x\,\sin(y)$

:::solution

In each case we find $f_x$ and $f_y$, then differentiate $f_x$ with respect to $y$ and $f_y$ with respect to $x$.

1. $f_x = 2x - y$ and $f_y = -x + 3y^2$, so $f_{xy} = -1$ and $f_{yx} = -1$.

1. By the chain rule and the product rule,

   $$
   f_x = \frac{2e^y}{2x-y} \qquad\text{and}\qquad f_y = e^y\ln(2x-y) - \frac{e^y}{2x-y}
   $$

   Then

   $$
   \begin{aligned}
   f_{xy} &= \frac{2e^y}{2x-y} + \frac{2e^y}{(2x-y)^2} \\
   f_{yx} &= \frac{2e^y}{2x-y} + \frac{2e^y}{(2x-y)^2}
   \end{aligned}
   $$

   so $f_{xy} = f_{yx}$.

1. By the product rule and the chain rule,

   $$
   \begin{aligned}
   f_x &= 2ye^{2xy} + 2xy\cdot 2ye^{2xy} \\
   &= 2y(1+2xy)e^{2xy} \\
   \text{and, in the same way,}\qquad f_y &= 2x(1+2xy)e^{2xy}
   \end{aligned}
   $$

   Writing $f_x = \left(2y + 4xy^2\right)e^{2xy}$ and differentiating with respect to $y$,

   $$
   f_{xy} = \left(2 + 8xy\right)e^{2xy} + \left(2y + 4xy^2\right)2xe^{2xy} = 2\left(1 + 6xy + 4x^2y^2\right)e^{2xy}
   $$

   Swapping the roles of $x$ and $y$ in this working gives $f_{yx}$, and the result is unchanged, so $f_{xy} = f_{yx}$.

1. $f_x = \sin(y)$ and $f_y = x\cos(y)$, so $f_{xy} = \cos(y)$ and $f_{yx} = \cos(y)$.

:::

::::

::::challenge{id="05_ext_03" title="Extension problems 3"}
The body mass index, $B$, is used as a parameter to classify people as underweight, normal, overweight and obese.
It is defined as their weight in kg, $w$, divided by the square of their height in metres, $h$.

1. Sketch a graph of $B$ against $w$ for a person who is 1.7m tall.
1. Find the rate of change of $B$ with weight of this person.
1. Sketch a graph of $B$ against $h$ for a child whose weight is constant at 35 kg.
1. Find the rate of change of $B$ with height $h$ of this child.
1. Show that

   $$
   \frac{\partial^2 B}{\partial h \partial w}=\frac{\partial^2 B}{\partial w \partial h}.
   $$

:::solution

We have $B = w/h^2$.

1. With $h = 1.7$, $B = w/1.7^2 \approx 0.346\,w$, which is a straight line through the origin with gradient about 0.346.

   ```python
   import numpy as np
   from matplotlib import pyplot as plt

   w = np.linspace(40, 120, 100)
   plt.plot(w, w / 1.7**2)
   plt.xlabel('w (kg)')
   plt.ylabel('B')
   ```

1. Treating $h$ as a constant,

   $$
   \frac{\partial B}{\partial w} = \frac{1}{h^2} = \frac{1}{1.7^2} \approx 0.346\,\text{m}^{-2}
   $$

   So this person's BMI rises by about 0.35 for each extra kg.

1. With $w = 35$, $B = 35/h^2$.
   This is a decreasing curve: $B$ is large for small $h$, and falls towards zero as $h$ increases (for example, $B \approx 17.9$ at $h = 1.4$).

   ```python
   h = np.linspace(1, 1.8, 100)
   plt.plot(h, 35 / h**2)
   plt.xlabel('h (m)')
   plt.ylabel('B')
   ```

1. Treating $w$ as a constant,

   $$
   \frac{\partial B}{\partial h} = -\frac{2w}{h^3} = -\frac{70}{h^3}
   $$

   in kg$\,$m$^{-3}$.
   For example, at $h = 1.4$ this is about $-25.5$, so the child's BMI falls by about 0.26 for each extra cm of height.

1. Differentiating $\partial B/\partial w = 1/h^2$ with respect to $h$, and $\partial B/\partial h = -2w/h^3$ with respect to $w$,

   $$
   \frac{\partial^2 B}{\partial h \partial w} = -\frac{2}{h^3} \qquad\text{and}\qquad \frac{\partial^2 B}{\partial w \partial h} = -\frac{2}{h^3}
   $$

   so the two are equal.

:::

::::

::::challenge{id="05_ext_04" title="Extension problems 4"}
A light wave or a sound wave propagated through time and space can be represented in a simplified form by:

$$
y=A\sin\left(2\pi\left(\frac{x}{\lambda}+f t\right)\right)
$$

where $A$ is the amplitude, $\lambda$ is the wavelength and $f$ is the frequency of the wave.
$x$ and $t$ are position and time respectively.

An understanding of this function is essential for many problems such as sound, light microscopy, phase microscopy and X-ray diffraction.

1. Draw a graph of $y$ as a function of $x$ assuming $t=0$.
1. Draw a graph of $y$ as a function of $t$ assuming $x=0$.
1. At what values of $x$ and $t$ does the function repeat itself?
1. Find the rate at which $y$ changes at an arbitrary fixed position.
1. Show that $y_{xt} = y_{tx}$.

:::solution

1. At $t=0$, $y = A\sin(2\pi x/\lambda)$.
   This is a sine wave in $x$ with amplitude $A$, which is zero at $x = 0, \lambda/2, \lambda, \ldots$ and repeats every $\lambda$.

1. At $x=0$, $y = A\sin(2\pi f t)$.
   This is a sine wave in $t$ with amplitude $A$, which repeats every $1/f$.

   ```python
   import numpy as np
   from matplotlib import pyplot as plt

   A = 1
   wavelength = 2
   f = 0.5
   x = np.linspace(0, 3 * wavelength, 300)
   t = np.linspace(0, 3 / f, 300)

   plt.subplot(1, 2, 1)
   plt.plot(x, A * np.sin(2 * np.pi * x / wavelength))
   plt.xlabel('x (at t = 0)')
   plt.subplot(1, 2, 2)
   plt.plot(t, A * np.sin(2 * np.pi * f * t))
   plt.xlabel('t (at x = 0)')
   ```

1. The sine function repeats when its argument increases by $2\pi$.
   Increasing $x$ by $\lambda$ adds $2\pi$ to the argument, and so does increasing $t$ by $1/f$.
   So, the function repeats every wavelength $\lambda$ in $x$, and every period $1/f$ in $t$.

1. At a fixed position we treat $x$ as a constant, so the rate of change is

   $$
   \frac{\partial y}{\partial t} = 2\pi fA\cos\left(2\pi\left(\frac{x}{\lambda}+f t\right)\right)
   $$

1. Differentiating in either order,

   $$
   \begin{aligned}
   y_x &= \frac{2\pi A}{\lambda}\cos\left(2\pi\left(\frac{x}{\lambda}+f t\right)\right), &\quad y_{xt} &= -\frac{4\pi^2 fA}{\lambda}\sin\left(2\pi\left(\frac{x}{\lambda}+f t\right)\right) \\
   y_t &= 2\pi fA\cos\left(2\pi\left(\frac{x}{\lambda}+f t\right)\right), &\quad y_{tx} &= -\frac{4\pi^2 fA}{\lambda}\sin\left(2\pi\left(\frac{x}{\lambda}+f t\right)\right)
   \end{aligned}
   $$

   so $y_{xt} = y_{tx}$.

:::

::::
