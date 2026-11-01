---
name: Integration 1
dependsOn: [scientific_computing.essential_maths.05_differentiation_3]
tags: []
learningOutcomes:
  - Understand the concept of the area under a graph
  - Be able to calculate the integral of a function
  - Recognise integration as the inverse operation of differentiation
  - Be able to use SymPy to integrate functions in Python
  - Be able to use the substitution method to integrate a function of a function
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

[![Youtube lecture thumbnail](fig/youtube06.jpg)](https://youtu.be/nEWqxSs6eLs)

---

## Why integration?

We often know the _rate_ at which something happens (e.g. how quickly a product is made), and want to know the total amount over some time.
Integration lets us work this out.

It is also how we find the area under a curve, and, as we will see, it reverses differentiation.

## Integration

**Calculating the area under a curve**:

If we want to find the area under a curve, we can divide the area into strips, calculate the area of each strip, and sum these areas together.

e.g. find the area of

$$
y = f(x) = x^2\qquad\text{between}\qquad x=1\text{ and }x=2
$$

![Individual rectangle under a curve](fig/06_01_single_rect.svg)

The area of the strip is $\delta A_i \approx y(x_i)\,\delta x$

We can sum up all the strips between $x=1$ and $x=2$ to give an estimate for the total area:

$$
A \approx \sum \delta A_i = \sum y(x_i)\,\delta x.
$$

This becomes more accurate as $\delta x$ becomes smaller.
We can use the same trick as when differentiating: let $\delta x$ tend to $0$:

$$
A = \lim_{\delta x \to 0}\sum y(x_i)\,\delta x= \int\limits^{2}_{1} y\,dx
$$

If we use a coarse approximation, then we are over-counting the area under the curve by quite a lot:

![Coarse approximation to the integral](fig/06_02_coarse.svg)

But, as the size of each rectangle reduces, we converge to the true area under the curve:

![Fine approximation to the integral](fig/06_03_fine.svg)

$\int$ is an elongated S, for the Latin _summa_ (sum).

This process is called **integration**.

## Calculating the integral

Let us 'invent' a function $F(x)$ that gives the area from some fixed point to $x$.
Then $A = F(2) - F(1)$.

With this (imagined) function, we can find the area of one of our tiny steps **exactly**, as

$$
F(x + \delta x) - F(x)
$$

Remember we approximated this as $y\,\delta x = f(x)\,\delta x$, so:

$$
\begin{aligned}
F(x + \delta x) - F(x) &\approx f(x)\delta x \\
f(x) &\approx \frac{F(x+\delta x)-F(x)}{\delta x}
\end{aligned}
$$

The error in this approximation tends to $0$ as $\delta x\to 0$, so

$$
f(x) = \lim_{\delta x\to 0} \frac{F(x+\delta x)-F(x)}{\delta x} = F'(x)
$$

(recalling the definition of the derivative).

In other words, for our example,

$$
A(x) = \int\limits^{x}_{1} y(t)\,dt\qquad\text{then}\qquad y(x) = A'(x)
$$

**Integration reverses the process of differentiation**.

## Calculating integrals

We know that:

$$
\frac{d \left(x^2\right)}{dx}= 2 x \qquad\Rightarrow\qquad \Big[x^2\Big]_{x_1}^{x_2} = \int_{x_1}^{x_2} 2 x\,dx
$$

Likewise:

$$
\frac{d\left(\frac{x^3}{3}\right)}{dx}= x^2 \qquad\Rightarrow\qquad \int_{x=1}^{x=2} x^2\,dx = \Big[\frac{1}{3}x^3\Big]_{x=1}^{x=2}=\frac{8}{3}-\frac{1}{3}= \frac{7}{3}
$$

**In general:**

$$
\frac{d\left(a\, x^{n+1}\right)}{dx}=(n+1)a\, x^n \qquad \Rightarrow \qquad \int_{x_1}^{x_2} a\, x^n\,dx = \Big[\frac{a}{n+1}\, x^{n+1}\Big]_{x_1}^{x_2} \qquad (n\neq -1)
$$

## SymPy examples

$$
\int_{x_1}^{x_2} x^6\,dx =
$$

```python
import sympy as sp
x, x1, x2 = sp.symbols('x x_1 x_2')
sp.integrate(x**6,(x,x1,x2))
```

$$
- \frac{x_{1}^{7}}{7} + \frac{x_{2}^{7}}{7}
$$

---

$$
\int_{x_1}^{x_2} x^{\frac{3}{2}}\,dx =
$$

```python
sp.integrate(x**(sp.sympify(3)/2),(x,x1,x2))
```

$$
- \frac{2 x_{1}^{\frac{5}{2}}}{5} + \frac{2 x_{2}^{\frac{5}{2}}}{5}
$$

---

$$
\int_{x_1}^{x_2} x^{-\frac{1}{2}}\,dx =
$$

```python
sp.integrate(x**(-sp.sympify(1)/2),(x,x1,x2))
```

$$
- 2 \sqrt{x_{1}} + 2 \sqrt{x_{2}}
$$

## Indefinite integrals

Consider now the functions:

$$
\begin{aligned}
y&=x^2+7 &&\text{(1)} \\
y&=x^2-100 &&\text{(2)}
\end{aligned}
$$

Differentiating (1):

$$
\frac{dy}{dx}=2x
$$

Differentiating (2):

$$
\frac{dy}{dx}=2x
$$

This implies that the integral:

$$
y=\int 2x\,dx=x^2 + C \qquad \text{where } C \text{ is an unknown constant}
$$

An integral without limits is called an **indefinite integral**.

## Indefinite integrals in SymPy

```python
x = sp.symbols('x')
sp.integrate(2*x,x)
```

$$
x^{2}
$$

## Other integrals

Recall that $\frac{d}{dx}\ln x = \frac{1}{x}$. In fact, for any $x \neq 0$:

$$
\frac{d}{dx}\ln|x|=\frac{1}{x} \qquad\Rightarrow\qquad \int\frac{1}{x}\,dx=\ln |x| + C
$$

We need the modulus because $\ln$ is only defined for positive numbers, but $1/x$ is defined for negative $x$ too.
For $x>0$, $|x| = x$, so this is just the derivative of $\ln x$ that we already know.
For $x<0$, $|x| = -x$, and the chain rule gives

$$
\frac{d}{dx}\ln(-x) = \frac{1}{-x}\times(-1) = \frac{1}{x}.
$$

So $\ln|x|$ has derivative $1/x$ on both sides of zero.

This constant $C$ can be written as:

$$
C=\ln B
$$

where $B$ is another constant. Thus:

$$
\int \frac{1}{x}\,dx = \ln |x| + C=\ln |x| +\ln B =\ln (B |x|)
$$

## Integrating trigonometric functions

Recall that:

$$
\frac{d}{dx}\Big(\sin x\Big)= \cos x \qquad\text{and}\qquad \frac{d}{dx}\Big(\cos x\Big)= -\sin x
$$

Example 1:

$$
\int_{0}^{\pi/2} \cos x \, dx = \Big[\sin x \Big]_0^{\pi/2}=1-0=1
$$

Example 2:

$$
\int_{0}^{\pi/2} \sin x \, dx = \Big[-\cos x \Big]_0^{\pi/2}=0-(-1)=1
$$

## Summary of integration formulae

$$
\begin{aligned}
\int a\,dx &= ax+C \\
\int ax\,dx &= \frac{1}{2}ax^2+C \\
\int ax^n\,dx &= \frac{1}{n+1}ax^{n+1}+C \qquad\text{for all } n \text{ except } n=-1 \\
\int x^{-1}\,dx &= \int \frac{1}{x}\,dx = \ln \vert x\vert +C
\end{aligned}
$$

## Application

Recall that $f'(t)$ gives the rate at which $f(t)$ changes at time $t$.

Integrating the derivative $f'(t)$, we see:

$$
\int_a^b f'(t) \, dt = \Big[f(t)\Big]_a^b = f(b)-f(a)
$$

Therefore, the definite integral from $a$ to $b$ of $f'(t)$ with respect to $t$ will always give the **net** change that $f(t)$ has undergone as the parameter $t$ moves from $a$ to $b$.

## Example

A chemical process produces NaCl at the rate of $3\sqrt{t}$ grams per minute.
We ask three questions:

1. What is the rate of production one minute into the process?
2. What is the quantity of NaCl produced over the next three minutes?
3. What is the mean rate of NaCl production over this interval?

## Solution

Let $f(t)$ denote the grams of NaCl produced after $t$ minutes. Then $f'(t)= 3\sqrt{t}$.

1. The rate of production one minute into the process is $f'(1) = 3\sqrt{1} = 3$ grams per minute.

2. The quantity of NaCl produced over the next three minutes is:

   $$
   f(4)-f(1) = \int_1^4\,f'(t)\,dt = \int_1^4\,3\sqrt{t}\,dt = \left[2t^{3/2}\right]_1^4 = 2\times(8-1) = 14
   $$

   ```python
   t = sp.Symbol('t')
   sp.integrate(3*sp.sqrt(t),(t,1,4))
   ```

   $$
   14
   $$

3. The mean rate is the constant rate which would give the same overall effect:

   $$
   \frac{1}{b-a}\int_a^b\,f'(t)\,dt = \frac{1}{4-1}\int_1^4\,3\sqrt{t}\,dt = \frac{14}{3}
   $$

   ```python
   sp.integrate(3*sp.sqrt(t),(t,1,4))/(4-1)
   ```

   $$
   \frac{14}{3}
   $$

## Substitution method

Suppose we wish to integrate:

$$
I=\int f(g(x))\,dx \qquad \text{e.g.} \quad \int_{a}^{b} (3-4x)^{-5}\,dx
$$

In general, for $g(x)=u$, we can write:

$$
\int_{x_1}^{x_2} f(g(x))\,dx = \int_{u_1}^{u_2} f(u) \frac{dx}{du}\, du
$$

This can be thought of as being like the integral version of the chain rule.

### Examples

Consider:

$$
\int_{a}^{b} (2x + 3)^4\,dx \qquad \text{(3)}
$$

Multiplying this out and then integrating it would be very tedious.
Try a substitution instead:

$$
u=2x+3 \quad\Longleftrightarrow\quad x=\frac{1}{2}(u-3)=\frac{u}{2}-\frac{3}{2} \quad\Longleftrightarrow\quad \frac{dx}{du}=\frac{1}{2}
$$

Substitute into equation (3) above:

$$
\begin{aligned}
\int_{u(a)}^{u(b)} u^4 \times \frac{1}{2}\,du &= \frac{1}{2}\int_{u(a)}^{u(b)} u^4\,du \\
&= \frac{1}{2}\Big[\frac{1}{5}u^5\Big]_{u(a)}^{u(b)} = \frac{1}{2}\Big[\frac{1}{5}(2x+3)^5\Big]_a^b
\end{aligned}
$$

```python
a, b = sp.symbols('a b')
sp.integrate((2*x + 3)**4,(x,a,b))
```

$$
\begin{aligned}
&- \frac{16 a^{5}}{5} - 24 a^{4} - 72 a^{3} - 108 a^{2} - 81 a \\
&+ \frac{16 b^{5}}{5} + 24 b^{4} + 72 b^{3} + 108 b^{2} + 81 b
\end{aligned}
$$

---

Let's look at another example:

$$
\int_{a}^{b} (3-4x)^{-5}\,dx \qquad \text{(4)}
$$

Let:

$$
u=3-4x \quad\Longleftrightarrow\quad x=\frac{1}{4}(3-u)=\frac{3}{4}-\frac{u}{4} \quad\Longleftrightarrow\quad \frac{dx}{du}=-\frac{1}{4}
$$

Substitute into equation (4) above:

$$
\begin{aligned}
\int_{u(a)}^{u(b)} u^{-5} \times \frac{-1}{4}\,du = -\frac{1}{4}\int_{u(a)}^{u(b)} u^{-5}\,du &= -\frac{1}{4}\Big[\frac{1}{-4}u^{-4}\Big]_{u(a)}^{u(b)} \\
&= \frac{1}{16}\Big[(3-4x)^{-4}\Big]_a^b
\end{aligned}
$$

```python
sp.integrate((3 - 4*x)**(-5),(x,a,b))
```

$$
\frac{1}{4096 b^{4} - 12288 b^{3} + 13824 b^{2} - 6912 b + 1296} - \frac{1}{4096 a^{4} - 12288 a^{3} + 13824 a^{2} - 6912 a + 1296}
$$

## Introductory problems

:::callout{variant="tip"}
For more practice, an AI tool can write new problems like the ones below.
Ask it for problems of the same type and difficulty, or for problems set in your own field.
For example, you could ask it to "write five climate science problems on definite integrals, at the level of these ones".
Work them on paper first, then ask the tool to check your answers and to explain any step you are unsure of.
:::

::::challenge{id="06_intro_01" title="Introductory problems 1"}
Integrate the following functions with respect to $x$. Remember that you can check your own answers by differentiating your results:

1. $x^3-1/x^4+x^2$
1. $\sqrt[3]{x}+1/(3\sqrt[4]{x})$
1. $1/x^2 + 1/\sqrt[3]{x} - 7$

:::solution

In each case, first write every term as a power of $x$, then use $\int x^n\,dx = x^{n+1}/(n+1) + C$.

1. Since $1/x^4 = x^{-4}$,

   $$
   \int\left(x^3 - x^{-4} + x^2\right)dx = \frac{x^4}{4} + \frac{x^{-3}}{3} + \frac{x^3}{3} + C = \frac{x^4}{4} + \frac{1}{3x^3} + \frac{x^3}{3} + C
   $$

1. Since $\sqrt[3]{x} = x^{1/3}$ and $1/\sqrt[4]{x} = x^{-1/4}$,

   $$
   \int\left(x^{1/3} + \frac{1}{3}x^{-1/4}\right)dx = \frac{3}{4}x^{4/3} + \frac{1}{3}\cdot\frac{4}{3}x^{3/4} + C = \frac{3}{4}x^{4/3} + \frac{4}{9}x^{3/4} + C
   $$

1. Since $1/x^2 = x^{-2}$ and $1/\sqrt[3]{x} = x^{-1/3}$,

   $$
   \int\left(x^{-2} + x^{-1/3} - 7\right)dx = -x^{-1} + \frac{3}{2}x^{2/3} - 7x + C
   $$

:::

::::

::::challenge{id="06_intro_02" title="Introductory problems 2"}
Evaluate the following definite integrals:

1. $\int_1^2 x^{1/2}\,dx$
1. $\int_2^3 x^{-2/3}\,dx$
1. $\int_0^{\ln(2)}e^{3x}\,dx$
1. $\int_0^2(x+1)^{1/5}\,dx$

:::solution

In each case, find the indefinite integral, then subtract its value at the lower limit from its value at the upper limit.

1. With $n = 1/2$,

   $$
   \int_1^2 x^{1/2}\,dx = \left[\frac{2}{3}x^{3/2}\right]_1^2 = \frac{2}{3}\left(2\sqrt{2} - 1\right) \approx 1.219
   $$

1. With $n = -2/3$,

   $$
   \int_2^3 x^{-2/3}\,dx = \left[3x^{1/3}\right]_2^3 = 3\left(3^{1/3} - 2^{1/3}\right) \approx 0.547
   $$

1. Using $e^{3\ln(2)} = \left(e^{\ln(2)}\right)^3 = 2^3$,

   $$
   \int_0^{\ln(2)}e^{3x}\,dx = \left[\frac{1}{3}e^{3x}\right]_0^{\ln(2)} = \frac{1}{3}\left(e^{3\ln(2)} - 1\right) = \frac{1}{3}\left(2^3 - 1\right) = \frac{7}{3}
   $$

1. Treating $x+1$ like $x$ (its derivative is 1),

   $$
   \int_0^2(x+1)^{1/5}\,dx = \left[\frac{5}{6}(x+1)^{6/5}\right]_0^2 = \frac{5}{6}\left(3^{6/5} - 1\right) \approx 2.281
   $$

:::

::::

::::challenge{id="06_intro_03" title="Introductory problems 3"}
Find the integrals below by making the substitution suggested:

1. $\int x^2(2x^3-5)^3\,dx \qquad\text{using}\qquad u = 2x^3-5$
1. $\int x\sqrt{7-2x^2}\,dx \qquad\text{using}\qquad u = 7-2x^2$
1. $\int_0^1 e^x(3e^x-10)^4\,dx \qquad\text{using}\qquad u = 3e^x-10$
1. $\int x^3\sqrt{15-3x^4}\,dx \qquad\text{using}\qquad u = 15-3x^4$
1. $\int_0^4 \sqrt{x^3}\,\sqrt{4+x^{5/2}}\,dx \qquad\text{using}\qquad u = 4+x^{5/2}$
1. $\int_0^1 x^{n-1}(1-x^{n})^2\,dx \qquad\text{using}\qquad u = 1-x^{n}$

:::solution

1. $du = 6x^2\,dx$, so $x^2\,dx = du/6$:

   $$
   \int x^2(2x^3-5)^3\,dx = \frac{1}{6}\int u^3\,du = \frac{u^4}{24} + C = \frac{(2x^3-5)^4}{24} + C
   $$

1. $du = -4x\,dx$, so $x\,dx = -du/4$:

   $$
   \int x\sqrt{7-2x^2}\,dx = -\frac{1}{4}\int u^{1/2}\,du = -\frac{1}{4}\cdot\frac{2}{3}u^{3/2} + C = -\frac{1}{6}\left(7-2x^2\right)^{3/2} + C
   $$

1. $du = 3e^x\,dx$, so $e^x\,dx = du/3$.
   We also change the limits: at $x=0$, $u = -7$, and at $x=1$, $u = 3e - 10$:

   $$
   \int_0^1 e^x(3e^x-10)^4\,dx = \frac{1}{3}\int_{-7}^{3e-10} u^4\,du = \frac{1}{15}\left[u^5\right]_{-7}^{3e-10} = \frac{1}{15}\left((3e-10)^5 + 7^5\right) \approx 1119
   $$

1. $du = -12x^3\,dx$, so $x^3\,dx = -du/12$:

   $$
   \int x^3\sqrt{15-3x^4}\,dx = -\frac{1}{12}\int u^{1/2}\,du = -\frac{1}{12}\cdot\frac{2}{3}u^{3/2} + C = -\frac{1}{18}\left(15-3x^4\right)^{3/2} + C
   $$

1. Note that $\sqrt{x^3} = x^{3/2}$.
   $du = \frac{5}{2}x^{3/2}\,dx$, so $x^{3/2}\,dx = \frac{2}{5}\,du$.
   At $x=0$, $u = 4$, and at $x=4$, $u = 4 + 4^{5/2} = 36$:

   $$
   \int_0^4 \sqrt{x^3}\,\sqrt{4+x^{5/2}}\,dx = \frac{2}{5}\int_4^{36} u^{1/2}\,du = \frac{2}{5}\cdot\frac{2}{3}\left[u^{3/2}\right]_4^{36} = \frac{4}{15}(216 - 8) = \frac{832}{15} \approx 55.47
   $$

1. $du = -nx^{n-1}\,dx$, so $x^{n-1}\,dx = -du/n$.
   At $x=0$, $u = 1$, and at $x=1$, $u = 0$:

   $$
   \int_0^1 x^{n-1}(1-x^{n})^2\,dx = -\frac{1}{n}\int_1^0 u^2\,du = \frac{1}{n}\int_0^1 u^2\,du = \frac{1}{n}\left[\frac{u^3}{3}\right]_0^1 = \frac{1}{3n}
   $$

:::

::::

::::challenge{id="06_intro_04" title="Introductory problems 4"}
By making suitable substitutions, find the indefinite integrals of:

1. $2(6x-5)^3$
1. $7x/(x^2-2)$
1. $3/\sqrt{5-x}$
1. $1/(x-a)$

:::solution

1. Let $u = 6x-5$, so $dx = du/6$:

   $$
   \int 2(6x-5)^3\,dx = \frac{2}{6}\int u^3\,du = \frac{u^4}{12} + C = \frac{(6x-5)^4}{12} + C
   $$

1. Let $u = x^2-2$, so $x\,dx = du/2$:

   $$
   \int\frac{7x}{x^2-2}\,dx = \frac{7}{2}\int\frac{1}{u}\,du = \frac{7}{2}\ln|u| + C = \frac{7}{2}\ln\left|x^2-2\right| + C
   $$

1. Let $u = 5-x$, so $dx = -du$:

   $$
   \int\frac{3}{\sqrt{5-x}}\,dx = -3\int u^{-1/2}\,du = -6u^{1/2} + C = -6\sqrt{5-x} + C
   $$

1. Let $u = x-a$, so $dx = du$:

   $$
   \int\frac{1}{x-a}\,dx = \int\frac{1}{u}\,du = \ln|x-a| + C
   $$

:::

::::

## Main problems

::::challenge{id="06_main_01" title="Main problems 1"}
Electrostatic work: the force acting between two electric charges $q_1$ and $q_2$ separated by distance $x$ in a vacuum is given by Coulomb's inverse square law:

$$
F(x) = \frac{q_1 q_2}{4\pi \epsilon_0 x^2}
$$

where $\epsilon_0$ is the permittivity of a vacuum.

Like charges (charges with the same sign, such as 2 nuclei or 2 electrons) repel so that the force acting on $q_2$ due to the presence of $q_1$ acts in a positive $x$ direction, away from $q_1$.
Unlike charges (of opposite signs such as the proton and electron in the hydrogen atom) attract, and the force on $q_2$ is directed towards $q_1$ (i.e. F is negative).

Consider 2 like charges, initially infinitely far apart.
Because the charges repel, work must be done on the system to bring $q_2$ from infinity to the distance $x$ from $q_1$.
The force $F$ must be applied to overcome the repulsion.

1. Explain why the total work done is given by

   $$
   W=-\int_\infty^x F(x')\,dx' = -\frac{q_1 q_2}{4\pi \epsilon_0} \int_\infty^x \frac{dx'}{{x'}^2}.
   $$

1. Calculate the work, $W$, in Joules when
   - $x =5.3 \times 10^{-11}\,\text{m}$,
   - $q_1=q_2=1.6 \times 10^{-19}\,\text{C}$,
   - $\epsilon_0=8.85 \times 10^{-12}\,\text{Fm}^{-1}$.

:::solution

1. To move $q_2$ inwards we must push it with a force that balances the repulsion, i.e. a force $-F(x')$ at each distance $x'$.
   Work is force times distance moved, and when the force changes with distance we add up the small amounts of work $-F(x')\,dx'$ along the way.
   We start at infinity and end at $x$, so the total is

   $$
   W = \int_\infty^x \left(-F(x')\right)dx' = -\int_\infty^x F(x')\,dx'
   $$

   Substituting Coulomb's law for $F$ gives the second form.
   (Both the applied force and the movement point towards $q_1$, so $W$ is positive.)

1. First we do the integral.
   As $x'\to\infty$, $1/x'\to 0$, so

   $$
   \int_\infty^x \frac{dx'}{{x'}^2} = \left[-\frac{1}{x'}\right]_\infty^x = -\frac{1}{x} - 0 = -\frac{1}{x}
   $$

   Therefore,

   $$
   W = \frac{q_1 q_2}{4\pi \epsilon_0 x} = \frac{\left(1.6\times 10^{-19}\right)^2}{4\pi\times 8.85\times 10^{-12}\times 5.3 \times 10^{-11}} \approx 4.34\times 10^{-18}\,\text{J}
   $$

   (This distance is the radius of a hydrogen atom, so $W$ is the work needed to bring two protons that close together.)

:::

::::

::::challenge{id="06_main_02" title="Main problems 2"}
The rate at which the world's oil is being consumed is continuously increasing.
Suppose the rate (in billions of barrels per year) is given by the function $r=f(t)$, where $t$ is measured in years and $t=0$ is the start of 1990.

1. Write down a definite integral which represents the total quantity of oil used between the start of 1990 and the end of 2020.
1. Calculate this integral using the function $r(t)=24 e^{0.015t}$.

:::solution

1. The end of 2020 is $t = 31$, so the total quantity of oil used, in billions of barrels, is

   $$
   \int_0^{31} f(t)\,dt
   $$

1. With $r(t) = 24e^{0.015t}$,

   $$
   \int_0^{31} 24e^{0.015t}\,dt = \left[\frac{24}{0.015}e^{0.015t}\right]_0^{31} = 1600\left(e^{0.465} - 1\right) \approx 947
   $$

   So about 947 billion barrels of oil were used.
   (In this model the rate rises from 24 to about 38 billion barrels per year, which is close to the real figures.)

:::

::::

::::challenge{id="06_main_03" title="Main problems 3"}
Since 1850, global carbon dioxide (CO$_2$) emissions have been rising exponentially.
Let $C(t)$ represent the rate that CO$_2$ is emitted into the atmosphere, measured in Gigatonnes per year, where $t$ measures the number of years since the start of 1850.
A model for the rate of emission over time is given by

$$
C(t) = ke^{pt},
$$

where $k$ and $p$ are positive constants.

1. What are the units of $k$ and $p$?
1. Given that, at the start of 1850, the global emission rate was 0.2 Gigatonnes per year, and that at the start of 2010 the global emission rate was 32 Gigatonnes per year, calculate $k$ and $p$.
1. Calculate the total quantity of CO$_2$ emitted between the start of 1850 and the start of 2010.

:::solution

1. $C$ is in Gigatonnes per year, and $e^{pt}$ is just a number, so $k$ is also in Gigatonnes per year.
   The power $pt$ must be a number too, so $p$ is in "per year" (year$^{-1}$).

1. At the start of 1850, $t=0$, so $C(0) = k = 0.2$ Gigatonnes per year.
   The start of 2010 is $t = 160$, so

   $$
   0.2\,e^{160p} = 32 \quad\implies\quad e^{160p} = 160 \quad\implies\quad p = \frac{\ln(160)}{160} \approx 0.0317\,\text{year}^{-1}
   $$

1. The total emitted is the integral of the rate:

   $$
   \int_0^{160} 0.2\,e^{pt}\,dt = \left[\frac{0.2}{p}e^{pt}\right]_0^{160} = \frac{0.2}{p}\left(e^{160p} - 1\right) = \frac{0.2\times 159}{p} \approx 1003
   $$

   So about 1000 Gigatonnes of CO$_2$ were emitted between 1850 and 2010.

:::

::::

::::challenge{id="06_main_04" title="Main problems 4"}
The velocity $v$ of blood in a cylindrical vessel of radius $R$ and length $l$ is given by

$$
v(r) = \frac{P(R^2 - r^2)}{4\eta l}
$$

where $\eta$ and $P$ are constants, and $r$ is the radial distance from the cylinder's axis.

Find the average velocity of blood along the radius of the cylinder (i.e. for $0\leq r\leq R$), and compare this with the maximum velocity.

:::solution

The average of $v$ over $0\leq r\leq R$ is its integral divided by the length of the interval, $R$:

$$
\bar{v} = \frac{1}{R}\int_0^R \frac{P(R^2 - r^2)}{4\eta l}\,dr = \frac{P}{4\eta l R}\left[R^2r - \frac{r^3}{3}\right]_0^R = \frac{P}{4\eta l R}\cdot\frac{2R^3}{3} = \frac{PR^2}{6\eta l}
$$

The velocity is largest on the axis, at $r=0$, since $R^2 - r^2$ decreases as $r$ increases.
So the maximum velocity is $v(0) = PR^2/(4\eta l)$, and

$$
\frac{\bar{v}}{v(0)} = \frac{4}{6} = \frac{2}{3}
$$

The average velocity along the radius is two thirds of the maximum.

Note, this is the average along a line from the axis to the wall.
The average over the whole cross-section of the vessel is $\frac{1}{2}v(0)$, which is smaller, because more of the cross-section lies near the wall, where the blood is slow.

:::

::::

::::challenge{id="06_main_05" title="Main problems 5"}
Consider the function $y = 4x^3 +2x^2-8x + 2$.

1. Draw an accurate graph of this function for values of $x$ between $-4$ and $4$.
1. Calculate the turning points of the curve and mark these on the graph.
1. On your graph, shade in the region under the curve between $x=-2$ and $x=2$ and _estimate_ its area.
1. Integrate the function between $x=-2$ and $x=2$. Is this an accurate calculation of the area of the shaded region in part **3.**?
1. Identify and explain any differences you find between your estimate in part **3.** and your calculation in part **4.**

:::solution

1. Some useful points: $y(0) = 2$, $y(-4) = -190$ and $y(4) = 258$.
   The curve crosses the $x$ axis where

   $$
   4x^3+2x^2-8x+2 = 2(x-1)\left(2x^2+3x-1\right) = 0
   $$

   i.e. at $x = 1$ and, from the quadratic formula, $x = (-3\pm\sqrt{17})/4$, which is $x\approx -1.78$ and $x\approx 0.28$.

   ```python
   import numpy as np
   from matplotlib import pyplot as plt

   x = np.linspace(-4, 4, 400)
   y = 4 * x**3 + 2 * x**2 - 8 * x + 2
   plt.plot(x, y)
   plt.axhline(0, color='black', linewidth=0.5)
   plt.xlabel('x')
   plt.ylabel('y')
   ```

1. The turning points are where

   $$
   \frac{dy}{dx} = 12x^2 + 4x - 8 = 4(3x-2)(x+1) = 0
   $$

   i.e. at $x=-1$ and $x = 2/3$.
   The second derivative is $d^2y/dx^2 = 24x + 4$.
   At $x=-1$ it is $-20 < 0$, so $(-1, 8)$ is a maximum.
   At $x=2/3$ it is $20 > 0$, so $(2/3, -34/27)$, i.e. about $(0.67, -1.26)$, is a minimum.

1. Between $x=-2$ and $x=2$ the curve is below the axis for $-2 < x < -1.78$ and for $0.28 < x < 1$, and above it elsewhere ($y(-2) = -6$ and $y(2) = 26$).
   Counting squares, or splitting the region into triangles, gives two large pieces of about 10 each and two small pieces of under 1 each, so an estimate of about 21.
   (Any careful estimate close to this is fine.)

1. Integrating term by term,

   $$
   \int_{-2}^{2}\left(4x^3+2x^2-8x+2\right)dx = \left[x^4 + \frac{2}{3}x^3 - 4x^2 + 2x\right]_{-2}^{2} = \frac{28}{3} - \left(-\frac{28}{3}\right) = \frac{56}{3} \approx 18.67
   $$

   This is not the area of the shaded region.

1. The integral counts the parts of the region below the $x$ axis as negative, so it subtracts them instead of adding them.
   To find the area, we integrate separately between the points where the curve crosses the axis, and add up the sizes of the results.
   The two pieces below the axis have areas of about 0.62 and 0.60, so

   $$
   \text{area} \approx 18.67 + 2\times(0.62 + 0.60) \approx 21.11
   $$

   Any remaining difference from part **3.** is the error in the estimate.

:::

::::

## Extension problems

::::challenge{id="06_ext_01" title="Extension problems 1"}
Evaluate the following definite integrals:

1. $\int_{-\pi/2}^{\pi/2} 3\cos (x)\,dx$
1. $\int_{\pi/2}^{\pi}\cos (x)\,\sin(x)\,dx$
1. $\int_0^\pi(\cos^2(x)+\sin^2(x))\,dx$

:::solution

1. Since $\sin(x)$ has derivative $\cos(x)$,

   $$
   \int_{-\pi/2}^{\pi/2} 3\cos (x)\,dx = 3\left[\sin(x)\right]_{-\pi/2}^{\pi/2} = 3\left(1 - (-1)\right) = 6
   $$

1. Let $u = \sin(x)$, so that $du = \cos(x)\,dx$.
   At $x = \pi/2$, $u = 1$, and at $x = \pi$, $u = 0$:

   $$
   \int_{\pi/2}^{\pi}\cos (x)\,\sin(x)\,dx = \int_1^0 u\,du = \left[\frac{u^2}{2}\right]_1^0 = -\frac{1}{2}
   $$

1. Since $\cos^2(x)+\sin^2(x) = 1$ for every $x$,

   $$
   \int_0^\pi(\cos^2(x)+\sin^2(x))\,dx = \int_0^\pi 1\,dx = \pi
   $$

:::

::::

::::challenge{id="06_ext_02" title="Extension problems 2"}
Let $u$ and $v$ be functions of $x$.

1. Given that

   $$
   \frac{d}{dx}(uv) = u\frac{dv}{dx} + v\frac{du}{dx},
   $$

   show that

   $$
   \int v\,\frac{du}{dx}\,dx = uv-\int u\,\frac{dv}{dx}\,dx.
   $$

1. By using this 'integration by parts' formula, and substituting $z=x^2$ or otherwise, show that

   $$
   \int_0^{\infty} x^n\,e^{-x^2}\,dx=\frac{1}{2}(n-1)\int_0^{\infty} x^{n-2}e^{-x^2}\,dx\qquad \text{for } n>1.
   $$

1. Hence evaluate

   $$
   \int_0^{\infty} x^5\,e^{-x^2}\,dx.
   $$

:::solution

1. Integrating both sides of the product rule with respect to $x$,

   $$
   uv = \int u\,\frac{dv}{dx}\,dx + \int v\,\frac{du}{dx}\,dx
   $$

   since integration reverses differentiation.
   Rearranging gives the formula.

1. Write $x^n e^{-x^2} = x^{n-1}\cdot xe^{-x^2}$, and choose

   $$
   v = x^{n-1}, \qquad \frac{du}{dx} = xe^{-x^2}
   $$

   Then $dv/dx = (n-1)x^{n-2}$, and, substituting $z = x^2$ (so $dz = 2x\,dx$), $u = \int xe^{-x^2}\,dx = \frac{1}{2}\int e^{-z}\,dz = -\frac{1}{2}e^{-x^2}$.
   The integration by parts formula gives

   $$
   \int_0^{\infty} x^n\,e^{-x^2}\,dx = \left[-\frac{1}{2}x^{n-1}e^{-x^2}\right]_0^{\infty} + \frac{1}{2}(n-1)\int_0^{\infty} x^{n-2}e^{-x^2}\,dx
   $$

   The first term is zero at both ends.
   At $x=0$, $x^{n-1} = 0$ because $n>1$.
   As $x\to\infty$, $e^{-x^2}$ shrinks much faster than $x^{n-1}$ grows, so their product tends to 0.
   This leaves the result.

1. Using the result with $n=5$ and then $n=3$,

   $$
   \int_0^{\infty} x^5\,e^{-x^2}\,dx = \frac{4}{2}\int_0^{\infty} x^3\,e^{-x^2}\,dx = 2\cdot\frac{2}{2}\int_0^{\infty} x\,e^{-x^2}\,dx
   $$

   From part **2.**, $\int x\,e^{-x^2}\,dx = -\frac{1}{2}e^{-x^2}$, so

   $$
   \int_0^{\infty} x\,e^{-x^2}\,dx = \left[-\frac{1}{2}e^{-x^2}\right]_0^{\infty} = 0 - \left(-\frac{1}{2}\right) = \frac{1}{2}
   $$

   Therefore,

   $$
   \int_0^{\infty} x^5\,e^{-x^2}\,dx = 2\times\frac{1}{2} = 1
   $$

:::

::::

::::challenge{id="06_ext_03" title="Extension problems 3"}
A country wishes to achieve net-zero $\text{CO}_2$ emissions in 50 years.
Let $t$ be the time in years since the start of its programme, and let $E(t)$ be the rate at which the country emits $\text{CO}_2$, in $\text{Mt CO}_2\,\text{year}^{-1}$.
At the start, $E = 800$.
The country plans to reduce this rate steadily, so that each year its rate of emission is 12 $\text{Mt CO}_2\,\text{year}^{-1}$ lower than the year before.

**1.** Write down $dE/dt$, and hence find $E(t)$.
Use this to calculate the total $\text{CO}_2$ that the country emits over the 50 years.

After 10 years, the country starts a $\text{CO}_2$ removal programme, which captures $\text{CO}_2$ from the atmosphere and stores it underground.
The rate of removal $R(t)$, in $\text{Mt CO}_2\,\text{year}^{-1}$, is

$$
R(t) = 0.1t^{2} - t \qquad\text{for}\qquad t \geq 10
$$

(where $t$ is still the time since the start of the programme).

**2.** Find the time at which the rate of removal equals the rate of emission.
Does the country reach net-zero emissions within 50 years?

After 50 years, the country's rate of emission stays at its value at $t=50$, while the rate of removal carries on following $R(t)$.
The country wants the total $\text{CO}_2$ it has emitted since the start of the programme to be balanced by the total it has removed, so that overall it has added no $\text{CO}_2$ to the atmosphere.

**3.** Show that this happens about 108.5 years after the start of the programme.

:::solution

**1.** The rate of emission falls by 12 every year, so

$$
\frac{dE}{dt} = -12
$$

Integrating, $E = -12t + C$, and $E(0) = 800$ gives $C = 800$, so $E(t) = 800 - 12t$.
The total emitted over the 50 years is

$$
\int_0^{50}(800 - 12t)\,dt = \left[800t - 6t^2\right]_0^{50} = 40000 - 15000 = 25000\,\text{Mt CO}_2
$$

**2.** The two rates are equal when

$$
0.1t^2 - t = 800 - 12t \quad\implies\quad t^2 + 110t - 8000 = 0 \quad\implies\quad (t - 50)(t + 160) = 0
$$

The negative root has no meaning here, so the rates are equal at $t = 50$.
So, the country just reaches net-zero emissions at 50 years.

**3.** After 50 years the rate of emission stays at $E(50) = 800 - 12\times 50 = 200$.
So, by a time $T > 50$, the total emitted is

$$
25000 + 200(T - 50) = 15000 + 200T
$$

and the total removed is

$$
\int_{10}^{T}\left(0.1t^2 - t\right)dt = \left[\frac{t^3}{30} - \frac{t^2}{2}\right]_{10}^{T} = \frac{T^3}{30} - \frac{T^2}{2} + \frac{50}{3}
$$

Setting these equal and multiplying through by 30,

$$
T^3 - 15T^2 - 6000T - 449500 = 0
$$

We can't easily solve this cubic exactly, so we try values.
The left-hand side is $-12748$ at $T = 108$ and $13314$ at $T = 109$, so it is zero between them, at about $T = 108.5$ years.

:::

::::
