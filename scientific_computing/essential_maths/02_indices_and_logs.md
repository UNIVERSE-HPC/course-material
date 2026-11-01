---
name: Indices, logs and exponentials
dependsOn: [scientific_computing.essential_maths.01_graphs]
tags: []
learningOutcomes:
  - Understand the meaning of raising a number to an index
  - Be able to rearrange algebraic expressions with indices
  - Understand logarithms and their relationship with indices
  - Be able to apply the laws of logarithms to algebraic expressions
  - Understand the use of logarithmic axes in graphs
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

[![Youtube lecture thumbnail](fig/youtube02.jpg)](https://youtu.be/gbZtu0mAq2I)

---

## Why indices and logs?

- The laws of indices let us simplify and rearrange expressions with powers in them.
  We will need them throughout the course (e.g. when we cover differentiation).
- _Taking the logarithm_ undoes _raising to a power_.
  This means that we can use logarithms to solve for an unknown power, to work with quantities that have a huge dynamic range (e.g. pH), and to turn power laws and exponentials into straight lines.
- We often want to describe growth and decay in biology, and for this we use the exponential function, $e^x$, and its inverse, the natural logarithm.

## Indices

In mathematics, an index refers to the power to which a number (the base) is raised.
The index is usually written as a small number above and to the right of the base.
For example, in the expression $2^3$, $2$ is the base, and $3$ is the index or power.
This expression is read as "2 to the power of 3" or simply "2 cubed", and it equals $2 \times 2 \times 2 = 8$.

### Multiplication of expressions with powers

Let's recap by example:

$$
5^2 = 5 \times 5 = 25,
$$

and

$$
5^3 = 5 \times 5 \times 5 = 125.
$$

Now,

$$
5^5 = 5 \times 5 \times 5 \times 5 \times 5 = 3125
$$

and

$$
25 \times 125 = 3125.
$$

So,

$$
5^5 = 5^2 \times 5^3.
$$

And, by extension:

$$
a^m\times a^n=a^{m+n}.
$$

When we multiply two expressions with powers, we **add** the powers.

### Division of expressions with powers

Again, let's look at an example:

$$
\frac{5^3}{5^2} = \frac{5 \times 5 \times 5}{5 \times 5}
$$

We can cancel both fives on the bottom with two from the top, leaving just $5$, or, $5^1$ left.

Generalising, by extension:

$$
\frac{a^n}{a^m}=a^{n-m}
$$

When we divide two expressions with powers, we **subtract** the powers.

### Powers of expressions with powers

Back to our example:

$$
\left(5^3\right)^4 = 5^3 \times 5^3 \times 5^3 \times 5^3 = 244140625
$$

Using the addition rule from above, the resulting power should be $3 + 3 + 3 + 3 = 12$.
And, sure enough:

$$
5^{12} = 244140625.
$$

So, generalising again:

$$
(a^n)^m=a^{nm}
$$

When we raise an expression containing a power to another power, we **multiply** the powers.

### Negative powers

What happens when we divide indices, but there are more numbers on the bottom of the fraction?

$$
\frac{a^3}{a^5} = \frac{a \times a \times a}{a \times a \times a \times a \times a} = \frac{1}{a \times a} = \frac{1}{a^2}
$$

Using the division rule above, the power should be $3 - 5 = -2$.
And, so:

$$
a^{-2}=\frac{1}{a^2}
$$

and, in general,

$$
a^{-n} =\frac{1}{a^n}
$$

where $n$ is a positive integer.

### Zero index

Similarly,

$$
\frac{a^3}{a^3} = \frac{a \times a \times a}{a \times a \times a} = 1,
$$

and by using the division rule, the power should be $3-3=0$.

Thus, anything to the power of zero equals one, except $0^0$
which is undefined:

$$
a^0=1
$$

### Fractional (rational) indices

For $a>0$:

$$
\begin{aligned}
a^1 &= a^{1/2} \times a^{1/2} &&\text{so}\quad a^{1/2} = \sqrt{a} \\
a^{1/n} &= \sqrt[n]{a} \\
a^{n/m} &= \left(\sqrt[m]{a}\right)^n &&\text{or}\quad \sqrt[m]{a^n} \\
a^{-n/m} &= \frac{1}{\left(\sqrt[m]{a}\right)^n} &&\text{or}\quad \frac{1}{\sqrt[m]{a^n}}
\end{aligned}
$$

For example,

$$
a^{3/4}=\sqrt[4]{a^3} \quad \text{or} \quad \left(\sqrt[4]{a}\right)^3.
$$

In Python:

```python
a = 5.0
a**(3.0/4.0)
```

```text
3.34370152488211
```

```python
(a**3.0)**0.25
```

```text
3.34370152488211
```

```python
(a**0.25)**3.0
```

```text
3.34370152488211
```

So, the order of these operations does not matter (for $a>0$).

### Different bases, same index

$$
(a\cdot b)^x = a^x\cdot b^x\qquad \text{and}\qquad\left(\frac{a}{b}\right)^x=\frac{a^x}{b^x}
$$

### Two cautionary remarks

1. Powers of sums are **not** pretty: $(a+b)^x \ne a^x + b^x$
2. Powers of differences are **not** pretty: $(a-b)^x \ne a^x - b^x$

A useful formula for products of sums:

$$
(p+q)\cdot(s+t) = ps + pt+qs + qt
$$

Three important consequences:

1. $(a+b)^2=(a+b)(a+b)=a^2+2ab+b^2$
1. $(a-b)^2=(a-b)(a-b)=a^2-2ab+b^2$
1. $(a+b)(a-b)=a^2-b^2$

## Logarithms

1. If $y=a^x$, $x$ is the **power** to which the **base** $a$ must be raised to give
   $y$, OR
2. $x$ is the **logarithm** which, with **base** $a$, gives $y$.

In general we write

$$
x = \log_a y \quad \Leftrightarrow \quad y=a^x.
$$

The base of a logarithm may be any positive number other than 1. Commonly, logarithms either have **base** 10 or **base** $e$.
It is almost always a good idea to explicitly state the base, e.g. $\log_3 9=2$ implies $3^2=9$.
In this course we always write the base, e.g. $\log_{10}$, except for base $e$ (more on this below).

## Getting a feel for logarithms

Here's a graph of $y=\log_{10}x$:

![Graph of log](fig/02_01_log.svg)

Some physical phenomena use log metrics due to their huge dynamic range:

- volume of sound (decibels)
- pitch (1 octave is doubling of frequency, 2 octaves is quadrupling of
  frequency)
- earthquakes (Richter scale)
- pH ($[H^+]$ ion concentration)

## The laws of logarithms

### Using the same base $a$ for both operations

- _Taking the logarithm_ undoes _raising to a power_:

  $$
  \log_a\,a^r=r
  $$

- _Raising to a power_ undoes _taking the logarithm_:

  $$
  a^{\log_a\,b}=b
  $$

### Multiplication, division and powers

Each law of logarithms comes from a law of indices.
If $b=a^m$ and $c=a^n$, then $\log_a b = m$ and $\log_a c = n$, so:

|                | Indices                     | Logarithms                                      |
| -------------- | --------------------------- | ----------------------------------------------- |
| Multiplication | $a^m\times a^n=a^{m+n}$     | $\log_a (bc) = \log_a b + \log_a c$ (add)       |
| Division       | $a^m/a^n=a^{m-n}$           | $\log_a (b/c) = \log_a b - \log_a c$ (subtract) |
| Powers         | $\left(a^m\right)^r=a^{mr}$ | $\log_a b^r = r \log_a b$ (multiply)            |

This means that the laws of logarithms are the laws of indices written in terms of the powers, with the same (add), (subtract) and (multiply).

### Changing the base of a logarithm

$$
\log_a c = \frac{\log_b c}{\log_b a}
$$

### Special case

If $b$ and $c$ are the same, changing the base reduces to

$$
\log_a b =\frac{\log_b b}{\log_b a}=\frac{1}{\log_b a}
$$

### The log of any number to the base itself is 1

$$
\log_a a =1
$$

### The log of 1 is 0

$$
\log_a 1 = 0 \quad\text{since}\quad a^0=1
$$

### Negative logs

$$
\log_a \frac{1}{x} = \log_a1-\log_ax= 0 - \log_a x =-\log_ax
$$

### Two cautionary remarks about logarithms

1. $\log_a (x + y)$ and $\log_a (x-y)$ cannot be simplified any further, and should be left as they are.

2. Neither can $\log_a\,x \cdot \log_a\,y$ or $\log_a x/\log_a y$. Leave them as they are.

## The exponential function and the real number e

### Definition of the real number $e$

The real number $e$, also known as Euler's number, is that base for which the graph $y=e^x$
passes through the point $(0, 1)$ with gradient exactly equal to $1$.

![Graph of exp](fig/02_04_exp.svg)

The dashed line in the graph is the tangent to $y=e^x$ at $(0, 1)$, which is the line $y = x + 1$.

So for the function (called the **exponential function**):

$$
y=e^x \qquad \text{the gradient at } (0,1) \text{ is } 1 = e^0.
$$

This turns out to be true at **any** point on the curve, i.e. the gradient of $y=e^x$ at any point is equal to the value of $e^x$ there.
This means that the function $y=e^x$ is unchanged when differentiated (more on this when we cover differentiation).

$e$ has been found to arise in many branches of mathematics. It is also used as a **base** for logarithms. e.g.:

$$
y=\log_e10=\ln 10 \quad\text{and}\quad y=\log_ex=\ln x
$$

Logs to base $e$ are called **natural logarithms**.

## Definition of the natural logarithm

The **natural logarithm** of a number is the logarithm of that number taken using the base $e$.
We usually write $\ln(x)$ for $\log_e(x)$. Here are some examples:

- $\ln(e)=\log_e(e)=1$
- $\ln(10)=\log_e(10)$ = "The power I need to raise $e$ to in order to get 10"
- $\ln(10)\approx 2.303$. Which means that $e^{2.303}\approx 10$
- $\ln(\exp(b)) = \ln e^b =\log_e e^b = b$
- $\exp(\ln(b))=e^{\ln(b)}=e^{\log_e b}=b$

Note that the last two examples confirm the property that
the functions "$\exp$" and "$\ln$" are _functional
inverses_ of one another.

## Logarithms in Python

In numpy, `np.log` is the natural logarithm, $\ln$.
For base 10 and base 2 there are `np.log10` and `np.log2`:

```python
import numpy as np
print(np.log(10))
print(np.log10(1000))
print(np.log2(8))
```

```text
2.302585092994046
3.0
3.0
```

For any other base, use the change of base formula, e.g. $\log_3 81 = \ln 81 / \ln 3$:

```python
print(np.log(81)/np.log(3))
```

```text
4.0
```

## Example 1: power laws

Can the data below be fitted to the form: $y=Ax^n$?

```python
import numpy as np
x = np.array([1, 2, 4, 6, 9, 12, 16, 20, 25, 30, 36, 42, 49, 56, 64])
y = np.array([3.03, 4.11, 5.85, 6.35, 10.03, 11.13, 11.77, 14.05, 15.26, 15.89, 19.09, 19.08, 20.59, 21.41, 24.66])
```

![Graph of data table](fig/02_02_data.svg)

Take logs of both sides:

$$
\log_{10} y =\log_{10} Ax^n = \log_{10} A + \log_{10} x^n= \log_{10} A + n \log_{10} x
$$

This is the equation of a straight line, $Y=mX+c$.
Here $Y=\log_{10} y$, $X=\log_{10} x$, $m=n$ and $c=\log_{10} A$.

![Graph of data table, straightened](fig/02_03_data_straightened.svg)

The points lie close to a straight line, so the data are consistent with the model.
To find its gradient and intercept, we fit the best straight line through the points.
`np.polyfit(X, Y, 1)` does this (in the least-squares sense, so the line passes as close as it can to all the points at once), and returns the gradient and then the intercept:

```python
X = np.log10(x)
Y = np.log10(y)
# Fit the best straight line through the points
gradient, intercept = np.polyfit(X, Y, 1)
print('gradient =',gradient)
print('intercept =',intercept)
```

```text
gradient = 0.505264797790789
intercept = 0.47080411584219156
```

Gradient $\approx 0.5 = n$

Intercept $\approx 0.47 = \log_{10}A$ so $A\approx 3.0$

Data fit curve of the form:

$$
y=3.0\times x^{1/2}
$$

Plotting $\log_{10} y$ against $\log_{10} x$ is the same as plotting $y$ against $x$ on logarithmic axes (called a _log-log_ plot).

## Example 2: exponential growth

We often want to know how quickly a population grows.
Here, a population of bacteria is counted every hour, for 10 hours.
Can the data below be fitted to the form: $N=N_0e^{kt}$?

```python
t = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
N = np.array([199, 292, 386, 570, 758, 1190, 1618, 2308, 3280, 4259, 6708])
```

![Graph of bacterial growth data](fig/02_05_growth_data.svg)

Take the natural log of both sides:

$$
\ln N = \ln N_0e^{kt} = \ln N_0 + \ln e^{kt} = \ln N_0 + kt
$$

This is the equation of a straight line, $Y=mX+c$.
Here $Y=\ln N$, $X=t$, $m=k$ and $c=\ln N_0$.
Only the $N$ axis is on a log scale, so this is called a _semi-log_ plot.

![Graph of bacterial growth data, straightened](fig/02_06_growth_data_straightened.svg)

As in Example 1, we fit the best straight line through the points:

```python
Y = np.log(N)
# Fit the best straight line through the points
gradient, intercept = np.polyfit(t, Y, 1)
print('gradient =',gradient)
print('intercept =',intercept)
```

```text
gradient = 0.3480313382093428
intercept = 5.294429813364659
```

Gradient $\approx 0.35 = k$ (per hour)

Intercept $\approx 5.3 = \ln N_0$ so $N_0 = e^{5.3} \approx 200$

Data fit curve of the form:

$$
N=200\,e^{0.35t}
$$

The population doubles when $e^{kt}=2$, i.e. when $t=\ln 2/k\approx 2$ hours.

## Example 3: pH

1. What is the pH of a 0.011 M solution of HCl?

   $$
   \text{pH} = -\log_{10}[H^+]
   $$

   ```python
   import numpy as np
   pH = -np.log10(0.011)
   print('pH =',pH)
   ```

   ```text
   pH = 1.958607314841775
   ```

2. What is the H$^+$ concentration of a solution of HCl with a pH of 3?

   $$
   \text{pH} = -\log_{10} [H^+] = 3 \quad \text{so} \quad [H^+] = 10^{-3}
   $$

   ```python
   hydrogen = 10**(-3)
   print('[H+] =',hydrogen,'M')
   ```

   ```text
   [H+] = 0.001 M
   ```

## Example 4: simplifying logs

Write an expression for $x$ without using logarithms:

$$
\begin{aligned}
\log_{10}(x) &= \log_{10}(p) + 2 \log_{10}(q) - \log_{10}(k) -3 \\
\log_{10}(x) &= \\
x &=
\end{aligned}
$$

1. Use the laws of logarithms (above) to put all terms on the right hand side
   **within** the logarithm. This means we have to rewrite $3$ as $3\log_{10}(10)$.
1. Raise 10 to the power of each side, which will **undo** the logarithm function.

## Introductory problems

:::callout{variant="tip"}
For more practice, an AI tool can write new problems like the ones below.
Ask it for problems of the same type and difficulty, or for problems set in your own field.
For example, you could ask it to "write five earth science problems on logarithms, at the level of these ones".
Work them on paper first, then ask the tool to check your answers and to explain any step you are unsure of.
:::

::::challenge{id="02_intro_01" title="Introductory problems 1"}
Simplify:

1. $a^{10}/a^{-3}$
1. $\left(\sqrt{x}\right)^8/x^4$
1. $y^{1/4}/y^{-2/4}$
1. $$
   \frac{10^{-2/3} \times 10^7 \times 10^{-16} \times x^{1/2} \times y^4 \times z^{-1/3}}{10^{-19}\times 10^{43} \times 10^{2/3} \times z^{-1/3} \times y^{1/4} \times x^{5/2}}
   $$

:::solution

1. $a^{10}/a^{-3} = a^{10-(-3)} = a^{13}$
1. $\left(\sqrt{x}\right)^8/x^4 = x^{8/2}/x^4 = x^{4-4} = x^0 = 1$
1. $y^{1/4}/y^{-2/4} = y^{1/4+2/4} = y^{3/4}$
1. Collect the powers of each base, subtracting the powers in the denominator from those in the numerator.
   For the powers of 10:

   $$
   \left(-\frac{2}{3} + 7 - 16\right) - \left(-19 + 43 + \frac{2}{3}\right) = -\frac{29}{3} - \frac{74}{3} = -\frac{103}{3}
   $$

   The powers of $x$ give $1/2 - 5/2 = -2$, the powers of $y$ give $4 - 1/4 = 15/4$, and the $z$ terms cancel.
   So, the expression simplifies to

   $$
   10^{-103/3}\,x^{-2}\,y^{15/4}
   $$

:::

::::

::::challenge{id="02_intro_02" title="Introductory problems 2"}
Evaluate the following expressions without using a calculator:

1. $2^{-4}$
1. $36^{1/2}+64^{2/3}$
1. $\left(1/3\right)^{-2}$
1. $\left(81/9\right)^{3/2}$

:::solution

1. $2^{-4} = 1/2^4 = 1/16$
1. $36^{1/2} + 64^{2/3} = \sqrt{36} + \left(\sqrt[3]{64}\right)^2 = 6 + 4^2 = 22$
1. $\left(1/3\right)^{-2} = 3^2 = 9$
1. $\left(81/9\right)^{3/2} = 9^{3/2} = \left(\sqrt{9}\right)^3 = 3^3 = 27$

:::

::::

::::challenge{id="02_intro_03" title="Introductory problems 3"}
Express the following in logarithmic form:

1. $5^3 = 125$
1. $8^{-1/3} = 1/2$
1. $x^y = 4$

:::solution

1. $\log_5(125) = 3$
1. $\log_8(1/2) = -1/3$
1. $\log_x(4) = y$

:::

::::

::::challenge{id="02_intro_04" title="Introductory problems 4"}
Evaluate the following expressions without using a calculator:

1. $\log_4\,(64)$
1. $\log_{\pi}\,(1)$
1. $\log_{b}\, (b^a)$
1. $6^{\log_6\,(\pi)}$

:::solution

1. $4^3 = 64$, so $\log_4(64) = 3$.
1. $\pi^0 = 1$, so $\log_\pi(1) = 0$ (the log of 1 is 0 in any base).
1. $\log_b(b^a) = a$, since $\log_b$ undoes raising $b$ to a power.
1. $6^{\log_6(\pi)} = \pi$, since raising 6 to a power undoes $\log_6$.

:::

::::

::::challenge{id="02_intro_05" title="Introductory problems 5"}
Solve the following equations for $x$:

1. $e^x=4$
1. $e^{-x}=2$

One of your solutions should be exactly $-2$ times the value of your other solution. Why?

:::solution

1. Take the natural log of both sides: $x = \ln 4 \approx 1.386$.
1. Take the natural log of both sides: $-x = \ln 2$, so $x = -\ln 2 \approx -0.693$.

$4 = 2^2$, so $\ln 4 = 2\ln 2$.
This means that the first solution, $2\ln 2$, is exactly $-2$ times the second, $-\ln 2$.

:::

::::

::::challenge{id="02_intro_06" title="Introductory problems 6"}
Simplify:

1. $\ln \left(e^3\right)$
1. $\ln \left(1/(2e)\right)$
1. $e^{\ln x^4}$
1. $e^{3+\ln x}$

:::solution

1. $\ln\left(e^3\right) = 3$
1. $\ln\left(1/(2e)\right) = \ln 1 - \ln 2 - \ln e = -1 - \ln 2$
1. $e^{\ln x^4} = x^4$
1. $e^{3 + \ln x} = e^3\,e^{\ln x} = e^3 x$

:::

::::

## Main problems

::::challenge{id="02_main_01" title="Main problems 1"}
In an experiment, the mass, $m$ grams, of a reaction product is measured at various times, $t$ minutes. It is believed that $m$ and $t$ are related by a law of the form:

$$
m=At^n
$$

The results are shown in the table below:

| time (min) |  1  |  2   |  3   |  4  |  5   |  6  |  7   |  8   | 12  |
| ---------: | :-: | :--: | :--: | :-: | :--: | :-: | :--: | :--: | :-: |
|   mass (g) | 0.5 | 0.63 | 0.72 | 0.8 | 0.85 | 0.9 | 0.95 | 0.99 | 1.0 |

1. Confirm this postulate graphically. There is one result that does not conform to this law. Which?
1. Find appropriate values of $A$ and $n$, and in this context, explain the meaning of $n$. What are the units of $A$?
1. Explain, with reasons, whether you would use these results to predict the mass when $t=15$.

:::solution

1. Take logs of both sides, as in Example 1:

   $$
   \log_{10} m = \log_{10} A + n \log_{10} t
   $$

   So, if the law holds, plotting $Y = \log_{10} m$ against $X = \log_{10} t$ should result in a straight line.

   ```python
   import numpy as np
   from matplotlib import pyplot as plt

   t = np.array([1, 2, 3, 4, 5, 6, 7, 8, 12])
   m = np.array([0.5, 0.63, 0.72, 0.8, 0.85, 0.9, 0.95, 0.99, 1.0])
   X = np.log10(t)
   Y = np.log10(m)

   plt.plot(X, Y, 'o')
   plt.xlabel('$X = \\log_{10}(t)$')
   plt.ylabel('$Y = \\log_{10}(m)$')
   ```

   The first eight points lie close to a straight line, so they are consistent with the law.
   The last point ($t = 12$, $m = 1.0$) falls well below the line, so this is the result that does not conform.

1. We fit the best straight line through the first eight points, leaving out the last one:

   ```python
   # Fit the best straight line through the first eight points, leaving out t = 12
   gradient, intercept = np.polyfit(X[:-1], Y[:-1], 1)
   print('gradient =',gradient)
   print('intercept =',intercept)
   print('A =',10**intercept)
   ```

   ```text
   gradient = 0.3284064309500228
   intercept = -0.29959116068954406
   A = 0.5016592670730619
   ```

   Gradient $\approx 0.33 = n$, and intercept $\approx -0.30 = \log_{10} A$, so $A \approx 0.50$.
   So, the data fit a curve of the form

   $$
   m = 0.5\,t^{1/3}
   $$

   The power $n$ tells us how the mass grows with time.
   Here $n \approx 1/3$, so the mass grows like the cube root of time.
   This means that doubling the time multiplies the mass by only $2^{1/3} \approx 1.26$, and the growth slows down as time goes on.
   $m$ is in grams and $t^n$ is in minutes$^n$, so the units of $A$ are g min$^{-n}$, here g min$^{-1/3}$.

1. No.
   The law predicts $m = 0.5 \times 15^{1/3} \approx 1.2$ g at $t = 15$, but the result at $t = 12$ suggests the mass has stopped growing at about 1 g (perhaps because one of the reactants has run out).
   $t = 15$ is beyond the range where the law holds, so a prediction from it would not be reliable.

:::

::::

::::challenge{id="02_main_02" title="Main problems 2"}
These problems deal with pH:

1. What is the pH of 130ml of a solution containing 4.7mg HCl, assuming that HCl is completely ionised in solution, and its molecular mass is 36.46?
1. What would be the pH if the concentration of HCl were tripled?

:::solution

1. First find the number of moles of HCl:

   $$
   \frac{4.7 \times 10^{-3}\,\text{g}}{36.46\,\text{g}\,\text{mol}^{-1}} \approx 1.289 \times 10^{-4}\,\text{mol}
   $$

   This is in $130\,\text{ml} = 0.130\,\text{dm}^3$, and each HCl gives one H$^+$, so

   $$
   [H^+] = \frac{1.289 \times 10^{-4}}{0.130} \approx 9.92 \times 10^{-4}\,\text{M}
   $$

   and $\text{pH} = -\log_{10}\left(9.92 \times 10^{-4}\right) \approx 3.00$.

1. Tripling the concentration gives

   $$
   \text{pH} = -\log_{10}\left(3[H^+]\right) = -\log_{10} 3 - \log_{10}[H^+] \approx 3.00 - 0.48 = 2.53
   $$

   In other words, tripling the concentration lowers the pH by $\log_{10} 3 \approx 0.48$, whatever the starting concentration.

:::

::::

::::challenge{id="02_main_03" title="Main problems 3"}
Express in terms of $\log_{10} (a)$, $\log_{10} (b)$, $\log_{10} (c)$ and $\log_{10} (d)$:

1. $\log_{10}\left(b/(ac)\right)$
1. $\log_{10} (a^2 b c^3 d^4)$
1. $\log_{10} \left(\sqrt{cd/(ab)} \right)$

:::solution

1. $\log_{10}\left(b/(ac)\right) = \log_{10}(b) - \log_{10}(a) - \log_{10}(c)$
1. $\log_{10}(a^2 b c^3 d^4) = 2\log_{10}(a) + \log_{10}(b) + 3\log_{10}(c) + 4\log_{10}(d)$
1. The square root is a power of $1/2$, so

   $$
   \log_{10}\left(\sqrt{cd/(ab)}\right) = \frac{1}{2}\left(\log_{10}(c) + \log_{10}(d) - \log_{10}(a) - \log_{10}(b)\right)
   $$

:::

::::

::::challenge{id="02_main_04" title="Main problems 4"}
Simplify:

1. $\log_{10}(8) + \log_{10}(3) - \log_{10}(6)$
1. $\log_{10}\left(x^2-1\right) - \log_{10}\left(x^2+1\right)$
1. $3\log_a(4) + \log_a(5) - 2\log_a(9)$
1. $\log_{10}\left(x^9\right) - \log_{10}\left(x^6\right)$

:::solution

1. $\log_{10}(8) + \log_{10}(3) - \log_{10}(6) = \log_{10}\left(\frac{8 \times 3}{6}\right) = \log_{10}(4)$
1. $\log_{10}\left(x^2-1\right) - \log_{10}\left(x^2+1\right) = \log_{10}\left(\frac{x^2-1}{x^2+1}\right)$ (for $|x| > 1$, so that $x^2 - 1$ is positive)
1. $3\log_a(4) + \log_a(5) - 2\log_a(9) = \log_a\left(\frac{4^3 \times 5}{9^2}\right) = \log_a\left(\frac{320}{81}\right)$
1. $\log_{10}\left(x^9\right) - \log_{10}\left(x^6\right) = \log_{10}\left(x^9/x^6\right) = \log_{10}\left(x^3\right) = 3\log_{10}(x)$

:::

::::

::::challenge{id="02_main_05" title="Main problems 5"}
This problem gives you a chance to use the identity

$$
\log_a (b) =\frac{1}{\log_b (a)}.
$$

Consider the equation $\log_3(x) + 4\log_x(3) = 5$:

1. Verify that $x=3$ satisfies this equation
1. There is one other value of $x$ that also satisfies this equation. Find it.

:::solution

1. $\log_3(3) + 4\log_3(3) = 1 + 4 = 5$, so $x = 3$ satisfies the equation.
1. Using the identity, $\log_x(3) = 1/\log_3(x)$.
   Write $u = \log_3(x)$, so that the equation becomes

   $$
   u + \frac{4}{u} = 5
   $$

   Multiply both sides by $u$ and rearrange:

   $$
   u^2 - 5u + 4 = (u-1)(u-4) = 0
   $$

   So, $u = 1$, which gives $x = 3$ as in part **1.**, or $u = 4$, which gives $x = 3^4 = 81$.

:::

::::

::::challenge{id="02_main_06" title="Main problems 6"}
Solve the following equations for $x$:

1. $2\left(2^{2x}\right)- 5\left(2^x\right) + 2 = 0$
1. $3^{2x+1} - 28\left(3^x\right) + 9 = 0$
1. $16 = \log_2 (x)$
1. $\left(2 \sqrt 3 \log_{10}(x)\right)^2 - 7 \log_{10}(x^2) + 2 = 0$

:::solution

1. $2^{2x} = \left(2^x\right)^2$, so write $u = 2^x$:

   $$
   2u^2 - 5u + 2 = (2u-1)(u-2) = 0
   $$

   So, $2^x = 1/2$, giving $x = -1$, or $2^x = 2$, giving $x = 1$.

1. $3^{2x+1} = 3\left(3^x\right)^2$, so write $u = 3^x$:

   $$
   3u^2 - 28u + 9 = (3u-1)(u-9) = 0
   $$

   So, $3^x = 1/3$, giving $x = -1$, or $3^x = 9$, giving $x = 2$.

1. $x = 2^{16} = 65536$

1. Write $L = \log_{10}(x)$.
   Then $\left(2\sqrt{3}L\right)^2 = 12L^2$ and $\log_{10}(x^2) = 2L$, so

   $$
   12L^2 - 14L + 2 = 0 \quad\implies\quad 6L^2 - 7L + 1 = (6L-1)(L-1) = 0
   $$

   So, $L = 1/6$, giving $x = 10^{1/6} \approx 1.47$, or $L = 1$, giving $x = 10$.

:::

::::

::::challenge{id="02_main_07" title="Main problems 7"}
Write an expression for $x$ or $y$ without using logarithms:

1. $\log_{10}(x) = \log_{10}(3r) - 5 \log_{10}(s) + 3\log_{10}(t) - 3$
1. $\log_{10}(2y) = 5 + 5\log_{10}\left(4^3\right) -15\log_{10}\left(2/x\right) - 6\log_{10}(y)$

:::solution

1. Write $3 = \log_{10}\left(10^3\right)$ and combine the logs on the right hand side:

   $$
   \log_{10}(x) = \log_{10}\left(\frac{3rt^3}{10^3 s^5}\right) \quad\implies\quad x = \frac{3rt^3}{1000\,s^5}
   $$

1. Move the $y$ term to the left hand side, and write $5 = \log_{10}\left(10^5\right)$:

   $$
   \log_{10}(2y) + 6\log_{10}(y) = \log_{10}\left(10^5\right) + \log_{10}\left(4^{15}\right) - \log_{10}\left((2/x)^{15}\right)
   $$

   Combine the logs on each side:

   $$
   \log_{10}\left(2y^7\right) = \log_{10}\left(\frac{10^5\,4^{15}\,x^{15}}{2^{15}}\right)
   $$

   $4^{15} = 2^{30}$, so $2y^7 = 10^5\,2^{15}\,x^{15}$ and $y^7 = 10^5\,2^{14}\,x^{15}$.
   Taking the seventh root, and using $2^{14/7} = 4$,

   $$
   y = 4 \times 10^{5/7}\,x^{15/7}
   $$

:::

::::

::::challenge{id="02_main_08" title="Main problems 8"}
Write $x$ in terms of $y$ for each of the following:

1. $y=2e^{4x}$
1. $\ln y = 3 + 2\ln x$

:::solution

1. Divide by 2, then take the natural log of both sides:

   $$
   \frac{y}{2} = e^{4x} \quad\implies\quad 4x = \ln\left(\frac{y}{2}\right) \quad\implies\quad x = \frac{1}{4}\ln\left(\frac{y}{2}\right)
   $$

1. Write $3 = \ln\left(e^3\right)$ and $2\ln x = \ln\left(x^2\right)$:

   $$
   \ln y = \ln\left(e^3 x^2\right) \quad\implies\quad y = e^3 x^2 \quad\implies\quad x = e^{-3/2}\sqrt{y}
   $$

   We take the positive square root, because $\ln x$ is only defined for $x > 0$.

:::

::::

::::challenge{id="02_main_09" title="Main problems 9"}
Express as a sum or difference of logarithms:

1. $\ln\sqrt{(x-1)/(x+1)}$

:::solution

1. The square root is a power of $1/2$, so

   $$
   \ln\sqrt{\frac{x-1}{x+1}} = \frac{1}{2}\ln\left(\frac{x-1}{x+1}\right) = \frac{1}{2}\ln(x-1) - \frac{1}{2}\ln(x+1)
   $$

   (for $x > 1$, so that both logs are defined).

:::

::::

::::challenge{id="02_main_10" title="Main problems 10"}
Express as a single logarithm:

1. $1 - \ln 4x$
1. $3\ln x - \frac{1}{2} \ln\left(5-x^2\right)$

:::solution

1. Write $1 = \ln e$:

   $$
   1 - \ln 4x = \ln e - \ln 4x = \ln\left(\frac{e}{4x}\right)
   $$

1. $3\ln x - \frac{1}{2}\ln\left(5-x^2\right) = \ln\left(x^3\right) - \ln\left(\sqrt{5-x^2}\right) = \ln\left(\frac{x^3}{\sqrt{5-x^2}}\right)$

:::

::::
