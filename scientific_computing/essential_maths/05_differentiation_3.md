---
name: Differentiation 3
dependsOn: [scientific_computing.essential_maths.04_differentiation_2]
tags: []
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

The following YouTube video was recorded for the 2020 iteration of the course.
The material is still very similar:

[![Youtube lecture thumbnail](fig/youtube05.jpg)](https://youtu.be/zIgJF-4xu90)

---

## Exponentials and Partial Differentiation

## Examples of applying chain rule to the exponential function

1. $\displaystyle y=e^{-ax}$

   - Let $$\displaystyle u=-ax\Rightarrow\frac{{\rm d}u}{{\rm d}x}=-a$$.
   - Thus $\displaystyle y=e^u$ and
   - $$\displaystyle \frac{{\rm d}y}{{\rm d}u}=e^u~~\Rightarrow~~\frac{{\rm d}y}{{\rm d}x}=\frac{{\rm d}y}{{\rm d}u}\times\frac{{\rm d}u}{{\rm d}x}=e^u\times(-1.=-ae^{-ax}.$$

1. $\displaystyle y = e^{x^2}$
   - Then, letting $\displaystyle u = x^2$
   - $$\displaystyle \frac{{\rm d}}{{\rm d}x}e^{x^2}=\frac{{\rm d}y}{{\rm d}x}=\frac{{\rm d}y}{{\rm d}u}\times\frac{{\rm d}u}{{\rm d}x}=e^u\cdot 2x = e^{x^2}\cdot 2x.$$

So an important generalization is:

> $$\displaystyle \frac{{\rm d}}{{\rm d}x}e^{f(x)}=e^{f(x)}f'(x)$$ for any function $f(x)$

## Example with the natural logarithm

1. $\displaystyle y=\ln(a-x)^2=2\ln(a-x)=2\ln u$.
   - Let $\displaystyle u=(a-x)$:
   - $$\displaystyle \Rightarrow {{\rm d}u\over {\rm d}x}=-1~~{\rm and~~~~~}{{\rm d}y\over {\rm d}u}={2\over u}~~~{\rm Thus~~~~}{{\rm d}y\over {\rm d}x}={2\over u}\times (-1)={-2\over a-x}$$

This also generalises:

> $$\displaystyle \frac{{\rm d}}{{\rm d}x}\ln(f(x)) = {f'(x)\over f(x)}$$

## The Derivative of $a^x$

By the properties of logarithms and indices we have

$\displaystyle a^x = \left({e^{\ln a}}\right)^x=e^{\left({x\cdot\ln a}\right)}$.

Thus, as we saw above we have:

> $$\displaystyle \frac{{\rm d}}{{\rm d}x}a^x = \frac{{\rm d}}{{\rm d}x}e^{\left({x\cdot\ln a}\right)} = e^{\left({x\cdot\ln a}\right)}\frac{{\rm d}}{{\rm d}x}{\left({x\cdot\ln a}\right)} =a^x\cdot\ln a$$

Similarly, in general:

> $$\displaystyle \frac{{\rm d}}{{\rm d}x}a^{f(x)} = a^{f(x)}\cdot \ln a\cdot f'(x)$$

### Sympy Example

Let's try and use Sympy to demonstrate this:

```python
import sympy as sp
x, a = sp.symbols('x a') # declare the variables x and a
f = sp.Function('f')     # declare a function dependent on another variable
sp.diff(a**f(x),x)       # write the expression we wish to evaluate
```

> $\displaystyle a^{f{\left(x \right)}} \log{\left(a \right)} \frac{d}{d x} f{\left(x \right)}$

## The Derivative of $\displaystyle \log_a x\,\,$

Recall the conversion formula $\displaystyle \log_a x = {{\ln x}\over {\ln a}}$
and note that $\ln a$ is a constant.
Thus:

> $$\displaystyle \frac{{\rm d}}{{\rm d}x}\log_a x = \frac{{\rm d}}{{\rm d}x}\left({1\over{\ln a}}\cdot\ln x\right) = \left({1\over{\ln a}}\right)\cdot\frac{{\rm d}}{{\rm d}x}\ln x = \left({1\over{\ln a}}\right)\cdot{1\over {x}} = {1\over{x\cdot\ln a}}$$

In general:

> $$\displaystyle \frac{{\rm d}}{{\rm d}x}\log_a f(x) = {{f'(x)} \over {f(x){(\ln 1.}}}$$

### Sympy Example

Again, let's use SymPy to demonstrate this:

```python
x, a = sp.symbols('x a')  # declare the variables x and a
f = sp.Function('f')      # declare a function dependent on another variable
sp.diff(sp.log(f(x),a),x) # write the expression we wish to evaluate
```

> $$\displaystyle \frac{\frac{d}{d x} f{\left(x \right)}}{f{\left(x \right)} \log{\left(a \right)}}$$

## Further examples

1. Product Rule: Let $\displaystyle y = x^2\,e^x$. Then:

   > $$\displaystyle {{dy\over dx}}={d\over dx}x^2e^x={d\over dx}x^2\cdot e^x+x^2\cdot{d\over dx}e^x = (2x + x^2)e^x$$

1. Quotient Rule: Let $\displaystyle y = {{e^x}\over x}$. Then:

   > $$\displaystyle {{dy\over dx}}={{{{d\over dx}e^x}\cdot x - e^x\cdot {d\over dx}x}\over {x^2}}={{e^x\cdot x - e^x\cdot 1\over {x^2}}}={{x - 1}\over x^2}e^x$$

1. Chain Rule: $\displaystyle y = e^{x^2}$. Then, letting $\displaystyle f(x) = x^2$:

   > $$\displaystyle \frac{{\rm d}}{{\rm d}x}e^{x^2} = e^{f(x)}f'(x) = e^{x^2}\cdot 2x$$

1. $\displaystyle y=\ln (x^2 + 1)$. Then, letting $f(x) = x^2+1$:

   > $$\displaystyle \frac{{\rm d}}{{\rm d}x}\ln(x^2+1) = {f'(x)\over f(x)} = {2x\over {x^2+1}}$$

1. $\displaystyle {{\rm d}\over {\rm d}x}2^{x^3}=2^{x^3}\cdot\ln 2\cdot 3x^2$

1. $\displaystyle {{\rm d}\over {\rm d}x}10^{x^2+1}=10^{x^2+1}\cdot\ln 10\cdot 2x$

1. $\displaystyle \frac{{\rm d}}{{\rm d}x}\log_{10}(7x+5)={7\over {(7x+5)\cdot \ln10}}$

1. $\displaystyle \frac{{\rm d}}{{\rm d}x}\log_2(3^x+x^4)={{3^x\cdot\ln3 + 4x^3}\over{\ln 2\cdot(3^x+x^4)}}$

## Functions of several variables: Partial Differentiation

**Definition:** Given a function $z=f(x,y)$ of two variables $x$ and $y$, the **partial derivative of $z$ with respect to $x$** is the function obtained by differentiating $f(x,y)$ with respect to $x$, holding $y$ constant.

We denote this using $\partial$ (the "curly" delta, sometimes pronounced "del") as shown below:

> $$\displaystyle \frac{\partial z}{\partial x}=\frac{\partial}{\partial x}f(x,y) = f_x(x,y)$$

## Example 1

> $\displaystyle f(x,y)=z=x^2-2y^2$ > $$\displaystyle f_x={\partial z\over \partial x}=2x\qquad\rm{and}\qquad f_y={\partial z\over \partial y}=-4y$$

## Example 2

Let $\displaystyle z=3x^2y+5xy^2$. Then the partial derivative of $z$ with respect to $x$, holding $y$ fixed, is:

> $$\displaystyle \frac{\partial z}{\partial x}=\frac{\partial}{\partial x}\,\left(3x^2y+5xy^2\right)$$ > $$\displaystyle \qquad =3y\cdot 2x + 5y^2\cdot 1$$ > $$\displaystyle \qquad =6xy+5y^2$$

while the partial of $z$ with respect to $y$ holding $x$ fixed is:

> $$\displaystyle \frac{\partial z}{\partial y}=\frac{\partial}{\partial y}\,\left(3x^2y+5xy^2\right)\,$$ > $$\displaystyle \qquad =3x^2\cdot 1 + 5x\cdot 2y = 3x^2+10xy$$

### Sympy example

In the previous slide we had:

$$\displaystyle \frac{\partial}{\partial x}\,\left(3x^2y+5xy^2\right)\, = 6xy+5y^2$$

Let's redo this in Sympy:

```python
x, y = sp.symbols('x y')
sp.diff(3*x**2*y + 5*x*y**2,x)
```

$\displaystyle 6 x y + 5 y^{2}$

## Higher-Order Partial Derivatives

Given $z = f(x,y)$ there are now four distinct possibilities for the
second-order partial derivatives.

1. With respect to $x$ twice:

   $$\displaystyle \frac{\partial}{\partial x}\left(\frac{\partial z}{\partial x}\right) =\frac{\partial^2z}{\partial x^2} =z_{xx}$$

1. With respect to $y$ twice:

   $$\displaystyle \frac{\partial}{\partial y}\left(\frac{\partial z}{\partial y}\right) =\frac{\partial^2z}{\partial y^2} =z_{yy}$$

1. First with respect to $x$, then with respect to $y$:

   $$\displaystyle \frac{\partial}{\partial y}\left(\frac{\partial z}{\partial x}\right) =\frac{\partial^2z}{\partial y\partial x} =z_{xy}$$

1. First with respect to $y$, then with respect to $x$:

   $$\displaystyle \frac{\partial}{\partial x}\left(\frac{\partial z}{\partial y}\right) =\frac{\partial^2z}{\partial x\partial y} =z_{yx}$$

## Example: LaPlace's equation for equilibrium temperature distribution on a copper plate

Let $\displaystyle T(x,y)$ give the temperature at the point $\displaystyle (x,y)$.

According to a result of the French mathematician Pierre LaPlace (1749 - 1827), at every point $\displaystyle (x,y)$ the second-order partials of $\displaystyle T$ must satisfy the equation:

> $$\displaystyle T_{xx} + T_{yy} = 0$$

We can verify that the function $\displaystyle T(x,y)=y^2-x^2$ satisfies LaPlace's equation:

First with respect to $x$:

> $$\displaystyle T_x(x,y)=0-2x=-2x\qquad\rm{so}\qquad T_{xx}(x,y)=-2$$

Then with respect to $y$:

> $$\displaystyle T_y(x,y)=2y-0=2y\qquad\rm{so}\qquad T_{yy}(x,y)=2$$

Finally:

> $$\displaystyle T_{xx}(x,y)+T_{yy}(x,y) = 2 + (-2) = 0$$

which proves the result.

The function $\displaystyle z=x^2y - xy^2$ does _not_ satisfy LaPlace's equation (and so cannot be a model for thermal equilibrium).

First note that

> $$\displaystyle z_x = 2xy - y^2$$ > $$\displaystyle z_{xx}=2y$$

and that

> $$\displaystyle z_y = x^2 - 2xy$$ > $$\displaystyle z_{yy} =-2x$$

Therefore:

> $$\displaystyle z_{xx}+z_{yy}=2y-2x\ne 0$$

We can also verify this in Sympy like so:

```python
T1 = y**2 - x**2
sp.diff(T1, x, x) + sp.diff(T1, y, y)
```

$\displaystyle 0$

and for the second function:

```python
T2 = x**2*y - x*y**2
sp.diff(T2, x, x) + sp.diff(T2, y, y)
```

$\displaystyle - 2 x + 2 y$

## A Note on the Mixed Partials $f_{xy}$ and $f_{yx}$

If all of the partials of $\displaystyle f(x,y)$ exist, then $\displaystyle f_{xy}=f_{yx}$ for all $\displaystyle (x,y)$.

### Example

Let $\displaystyle z = x^2y^3+3x^2-2y^4$. Then $\displaystyle z_x=2xy^3+6x$ and $\displaystyle z_y = 3x^2y^2-8y^3$.

Taking the partial of $\displaystyle z_x$ with respect to $\displaystyle y$ we get

> $$\displaystyle z_{xy}=\frac{\partial}{\partial y}\left(2xy^3+6x\right)=6xy^2$$

Taking the partial of $\displaystyle z_y$ with respect to $x$ we get the same thing:

> $$\displaystyle z_{yx}=\frac{\partial}{\partial x}\left(3x^2y^2-8y^3\right)=6xy^2$$

So the operators $\displaystyle {\partial \over \partial x}$ and $\displaystyle {\partial \over \partial y}$ are **commutative**:

> $$\displaystyle {\rm~i.e.~~~~}~{\partial\over \partial x}\biggr({\partial z\over \partial y}\biggl)~~~~={\partial\over \partial y}\biggr({\partial z\over \partial x}\biggl)$$

### Introductory problems

::::challenge{id="05_intro_01" title="Introductory problems 1"}
Differentiate the following functions with respect to $x$, using the stated rules where indicated:

1. Product rule: $\displaystyle \sqrt{x}\,e^x$
1. Product rule: $\displaystyle 3x^2 \log(x)$
1. Chain rule: $\displaystyle e^{-4x^3}$
1. Chain rule: $\displaystyle \ln \sqrt {\left({x^{3/2}\over 6}\right)}$
1. Chain rule: $\displaystyle 10^{x^2}$
1. Any rules: $\displaystyle \frac{\ln x}{5x-7}$
1. Any rules: $\displaystyle \frac{e^x}{2x^3-1}$
1. Any rules: $\displaystyle \log_2\left(x\cos(x)\right)$

::::

::::challenge{id="05_intro_02" title="Introductory problems 2"}
If $\displaystyle \quad y=e^{-ax} \quad\rm{show~that}\quad 2{{{{\rm d}^2y}\over{{{\rm d}x}^2}}}+a{{\frac{{\rm d}y}{{\rm d}x}}}-a^2y=0$.
::::

::::challenge{id="05_intro_03" title="Introductory problems 3"}
If $\displaystyle \quad y=e^{-x}\sin(x) \quad\rm{show~that}\quad {{{{\rm d}^2y}\over{{{\rm d}x}^2}}} + 2{{\frac{{\rm d}y}{{\rm d}x}}} + 2y = 0$.
::::

### Main problems

::::challenge{id="05_main_01" title="Main problems 1"}
The power, $W$, that a certain machine develops is given by the formula

> $$\displaystyle W=EI-RI^2$$

where $I$ is the current and $E$ and $R$ are positive constants.

Find the maximum value of $W$ as $I$ varies.
::::

::::challenge{id="05_main_02" title="Main problems 2"}
Environmental health officers monitoring an outbreak of food poisoning after a wedding banquet were able to model the time course of the recovery of the guests using the equation:

> $$\displaystyle r = {100t\over 1+t}$$

where $t$ represents the number of days since infection and $r$ is the percentage of guests who no longer display symptoms.
Determine an expression for the rate of recovery.
::::

::::challenge{id="05_main_03" title="Main problems 3"}
An experiment called 'the reptilian drag race' looks at how agamid lizards accelerate from a standing start.

The distance $x$ travelled in time $t$ on a horizontal surface has been modelled as

> $$\displaystyle x = v_{\rm max} \left(t + {e^{-kt}\over k} - {1\over k}\right),$$

where $v_{\rm max}$ is the maximum velocity, and $k$ is a rate constant.

1. Find expressions for the velocity $v$ and acceleration $a$ as functions of time.
1. For $v_{\rm max} = 3\,$m$\,$s$^{-1}$ and $k=10\,$s$^{-1}$, sketch $x$, $v$ and $a/10$ on the same axes for $0\leq t\leq 1\,$s.

::::

::::challenge{id="05_main_04" title="Main problems 4"}
The distance $x$ that a particular organism travels over time from its starting location is modelled by the equation

> $$\displaystyle x(t) = t^2 e^{k(1-t)},$$

where $k$ is a positive constant and $0\leq t\leq 1\,$s.

1. Sketch $x$ over time for $k={1\over 2}$ and $k=3$.
1. Calculate an expression for the organism's velocity as a function of time.
1. What is the largest value of $k$ such that the organism never starts moving back towards where it started?

::::

::::challenge{id="05_main_05" title="Main problems 5"}
The function

> $$\displaystyle S = S_{max}(1- e^{-t\over\tau})$$

is used to describe sediment thickness accumulating in an extensional basin through time.
What is the sedimentation rate?
::::

### Extension problems

::::challenge{id="05_ext_01" title="Extension problems 1"}
Let $\displaystyle \quad z = {2\over 3}x^3 - {3\over 4}x^2y + {2\over 5}y^3$.

1. Find $\displaystyle z_x$ and $\displaystyle z_y$
1. Find $\displaystyle z_{xx}$ and $\displaystyle z_{yy}$
1. Show that $\displaystyle z_{xy} = z_{yx}$

::::

::::challenge{id="05*ext_02" title="Extension problems 2"}
Show that $f_{xy}=f_{yx}$ for the following functions:

1. $\displaystyle f(x,y) = x^2 - xy + y^3$
1. $\displaystyle f(x,y) = e^y\,\ln(2x-y)$
1. $\displaystyle f(x,y) = 2\,x\,y\,e^{2xy}$
1. $\displaystyle f(x,y) = x\,\sin(y)$

::::

::::challenge{id="05_ext_03" title="Extension problems 3"}
The body mass index, $B$, is used as a parameter to classify people as underweight, normal, overweight and obese.
It is defined as their weight in kg, $w$, divided by the square of their height in meters, $h$.

1. Sketch a graph of $B$ against $w$ for a person who is 1.7m tall.
1. Find the rate of change of $B$ with weight of this person.
1. Sketch a graph of $B$ against $h$ for a child whose weight is constant at 35 kg.
1. Find the rate of change of $B$ with height $h$ of this child.
1. Show that $\displaystyle \left({\partial^2 B\over \partial h \partial w}\right)=\left({\partial^2 B\over \partial w \partial h}\right)$.

::::

::::challenge{id="05_ext_04" title="Extension problems 4"}
A light wave or a sound wave propagated through time and space can be represented in a simplified form by:

$$y=A\sin\left(2\pi\left({x\over\lambda}+\omega t\right)\right)$$

where $A$ is the amplitude, $\lambda$ is the wavelength and $\omega$ is the frequency of the wave.
$x$ and $t$ are position and time respectively.

An understanding of this function is essential for many problems such as sound, light microscopy, phase microscopy and X-ray diffraction.

1. Draw a graph of $y$ as a function of $x$ assuming $t=0$.
1. Draw a graph of $y$ as a function of $t$ assuming $x=0$.
1. At what values of $x$ and $t$ does the function repeat itself?
1. Find the rate at which $y$ changes at an arbitrary fixed position.
1. Show that $\displaystyle y_{xt} = y_{tx}$.

::::
