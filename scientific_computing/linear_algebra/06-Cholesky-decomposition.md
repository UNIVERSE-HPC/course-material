---
name: Cholesky decomposition
dependsOn: [
    'scientific_computing.linear_algebra.04-LU-decomposition',
]
tags: []
---



## Cholesky decomposition

*Symmetric positive definite* matrices are a very special type of matrix that often 
arise in practice. From a computational point of view, this class of matrix is very 
attractive because it is possible to decompose a symmetic positive definite matrix $A$ 
very efficiently into a single lower triangular matrix $G$ so that $A = GG^T$. 

A matrix $A$ is positive definite if $x^T A x > 0$  for any nonzero $x \in \mathbb{R}$. 
This statement by itself is not terribly intuitive, so lets look at also look at an 
example of a $2 \times 2$ matrix

$$
A = \left(\begin{matrix}
a_{11} & a_{12} \\
a_{21} & a_{22}
\end{matrix}\right)
$$

If $A$ is symmetic positive definite (SPD) then

$$
\begin{aligned}
x &= (1, 0)^T \Rightarrow x^T A x = a_{11} > 0 \\
x &= (0, 1)^T \Rightarrow x^T A x = a_{22} > 0 \\
x &= (1, 1)^T \Rightarrow x^T A x = a_{11} + 2a_{12} + a_{22} > 0 \\
x &= (1,-1)^T \Rightarrow x^T A x = a_{11} - 2a_{12} + a_{22} > 0 \\
\end{aligned}
$$

The first two equations show that the diagonal entries of $A$ must be positive, and 
combining the last two equations imply $|a_{12}| \le (a_{11} + a_{22}) / 2$, that is 
that the matrix has much of its "mass" on the diagonal (note: this is *not* the same as 
the matrix being diagonally dominant, where $|a_{ii}| > \sum_{i=1...n,j \ne i} 
|a_{ij}|$). These two observations for our $2 \times 2$ matrix also apply for a general 
$n \times n$ SPD matrix. One of the very nice consequences of this "weighty" diagonal 
for SPD matrices is that it precludes the need for pivoting.

It can be shown that if $A$ is a SPD matrix, then the $LDL^T$ decomposition exists and 
that $D = \text{diag}(d_1, ..., d_n)$ has positive diagonal entries. Therefore, it is 
straightforward to see that $LDL^T$ = $GG^T$, where $G = L \text{diag}(\sqrt{d_1}, ..., 
\sqrt{d_n})$. The decomposition $A = GG^T$ is known as the cholesky decomposition and 
can be efficiently constructed in $n^3 / 3$ flops. There are a number of algorithms to 
construct this decomposition, and both the [wikipedia 
entry](https://en.wikipedia.org/wiki/Cholesky_decomposition) and Chapter 4.2 of the 
Matrix Computations textbook by Golub and Van Loan gives a number of different varients.

Note that a $LDL$ decomposition can also be used to calculate a cholesky decomposition, 
and this could be more efficient approach since (a) the SPD structure means that we can 
neglect pivoting in the $LDL$ decomposition, and (b) the $LDL$ decomposition does not 
requiring taking the square root of the diagonal elements. 

### Other Reading

- Golub, G. H. & Van Loan, C. F. Matrix Computations, 3rd Ed. (Johns Hopkins University 
  Press, 1996). Chapter 4.2
- https://en.wikipedia.org/wiki/Cholesky_decomposition

### Software

[`scipy.linalg.cholesky`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.cholesky.html)

[`scipy.linalg.cho_factor`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.cho_factor.html)

[`scipy.linalg.cho_solve`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.cho_solve.html)

[`scipy.linalg.cholesky_banded`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.cholesky_banded.html)

[`scipy.linalg.cho_solve_banded`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.cho_solve_banded.html)

## Problems

{{% notice question %}}
Imagine that we wanted to sample an array of values $x_i$, for $i = 1...n$, where each 
value is sampled from an independent normal distribution with standard deviation 
$\sigma$

 $$x_i \sim \mathcal{N}(0, \sigma)$$

 This could be achieved, for example, by sampling from a normal distribution with unit 
 standard deviation, a function that typically exists in any computer language, then 
 multiplying by $\sigma$

 $$x_i = \sigma \eta$$

 where $\eta \sim \mathcal{N}(0, 1)$

 Now imagine that instead of an independent normal distribution you wish to sample 
 $\mathbf{x} = [x_1, x_2, ..., x_n]$ from a multivariate normal distribution with some 
 covariance matrix $\Sigma$

 $$\mathbf{x} \sim \mathcal{N}(\mathbf{0}, \Sigma)$$

 We can achive this in practice by using the Cholesky decomposition. A covariance 
 matrix is a symmetic positive semidefinite matrix (i.e. $x^T \Sigma x \ge 0$}, and 
 therefore can be decomposed into  $\Sigma = LL^T$. We can then draw a sample from 
 $\mathcal{N}(\mathbf{0}, \Sigma)$ by scaling an independently generated random vector 
 by $L$

 $$\mathbf{x} = L \mathbf{\eta}$$

 where each element of the vector $\eta$ is $\eta_i \sim \mathcal{N}(0, 1)$.

 Write Python code to randomly sample an n-dimensional vector $x$ from 
 
 1. an independent normal distribution with variance $\sigma_1^2$.

 2. a multivariate normal distribution using a covariance matrix $\Sigma_{ij} = 
    \sigma_1^2 \exp{(-(i- j)^2 / \sigma_2^2)}$. Try different values for the magnitute 
    $\sigma_1$, and lenghtscale $\sigma_2$ parameters and their effect on the sampled 
    $\mathbf{x}$. Hint: while the expression for $\Sigma$ is guarrenteed to be positive 
    definte for all values of $\sigma_1$ and $\sigma_2$, numerical round-off can mean 
    that the Cholesky decomposition can fail. To guarrentee a positive definite 
    $\Sigma$, try adding a small amount (e.g. 1e-5) to the diagonal of $\Sigma$. This 
    is equivilent to adding a very small amount of independent normal noise to 
    $\mathbf{x}$.

{{% /notice %}}

{{% expand "Expand for solution" %}}
{{% notice solution %}}

```python
import numpy as np
import matplotlib.pylab as plt
import scipy
import scipy.linalg

n = 100
xs = np.arange(0, n)

def construct_covariance(sigma1, sigma2):
    K = sigma1**2 * np.exp(
        -(xs[:, np.newaxis] - xs[:, np.newaxis].T)**2 / sigma2**2
    )
    K += 1e-5 * np.eye(n)
    return K

def sample_zero_mean_random_field(covariance_matrix):
    L, _ = scipy.linalg.cho_factor(covariance_matrix, lower=True)
    indep_random_sample = np.random.normal(size=n)
    return np.dot(np.tril(L), indep_random_sample)

sigma1 = 1.0
sigma2 = 10.0
K1 = sigma1**2 * np.eye(n)
K2 = construct_covariance(sigma1, sigma2)

plt.plot(sample_zero_mean_random_field(K1))
plt.plot(sample_zero_mean_random_field(K2))
plt.show()
```
{{% /notice %}}
{{% /expand %}}

{{% notice question %}}
Now imagine that we have a vector of measurements $\mathbf{x}$, and we assume that a 
suitable model for these measurements is that they are generated from a zero-mean, 
multivariate normal distribuion, i.e.

$$\mathbf{x} \sim \mathcal{N}(\mathbf{0}, \Sigma)$$

We assume that the covariance matrix is of the following form, with two parameters 
$\mathbf{\theta} = (\sigma_1, \sigma_2)$. 

$$\Sigma_{ij} = \sigma_1^2 \exp{(-(i- j)^2/ \sigma_2^2)}$$

We can write down the *likelihood* of the covariance parameters $\mathbf{\theta}$, given 
a given dataset $\mathbf{x}$, by using the probability distribution function (PDF) for a 
zero-mean multivariate normal distribution 

$$
P(\mathbf{\theta} | \mathbf{x}) = (2 \pi)^{\frac{n}{2}} \text{ 
det}(\Sigma)^{\frac{1}{2}} \exp{\left( \frac{1}{2}\mathbf{x}^T \Sigma^{-1} 
\mathbf{x}\right)}
$$

Typically we work with the log of the likelihood for numerical reasons, which is

$$
\mathcal{L} = -\frac{1}{2} \log(|\Sigma|) + \mathbf{x}^T \Sigma^{-1} \mathbf{x} + 
\frac{n}{2} \log(2\pi)
$$

3. Generate a simulated dataset $\mathbf{x}$ using your code for question (2) using 
   "true" parameters $\mathbf{\theta}^t = (\sigma^t_1, \sigma^t_2)$. Then calculate 
   the log-likelihood using the Cholesky decomposition to efficiently calculate the 
   log determinant and the inverse of the covariance matrix. Vary $\mathbf{\theta}$ 
   and satisfy yourself that the maximum of the likelihood occurs at your "true" 
   parameters. In practice, when you don't know the true parameters, you could use an 
   optimisation algorithm to automatically determine the *most likely* model 
   parameters that give rise to your data.


{{% /notice %}}

{{% expand "Expand for solution" %}}
{{% notice solution %}}
```python
def log_likelihood(sigma1, sigma2, x):
    K = construct_covariance(sigma1, sigma2)
    L, lower = scipy.linalg.cho_factor(K, lower=True)

    logdet = 2 * np.sum(np.log(np.diag(L)))
    xinvSx = x.dot(scipy.linalg.cho_solve((L, lower), x))

    return -0.5 * (logdet + xinvSx)

sigma1 = np.linspace(0.5, 1.5, 100)
sigma2 = np.linspace(5.0, 15.0, 100)
Sigma1, Sigma2 = np.meshgrid(sigma1, sigma2)
x = sample_zero_mean_random_field(K2)

L = np.vectorize(log_likelihood, excluded=['x'])(Sigma1, Sigma2, x=x)
max_log_likelihood = log_likelihood(1, 10, x)
levels = np.linspace(0.9 * max_log_likelihood, max_log_likelihood, 5)

plt.clf()
contours = plt.contour(Sigma1, Sigma2, L, levels=levels, colors='black')
plt.clabel(contours, inline=True, fontsize=8)

plt.imshow(L, extent=[0.5, 1.5, 5.0, 15.0], origin='lower',
           cmap='RdGy', alpha=0.5, vmin=levels[0], aspect='auto')
c = plt.colorbar();
c.set_label(r'$\mathcal{L}$')
plt.xlabel(r'$\sigma_1$')
plt.ylabel(r'$\sigma_2$')
plt.show()
```
{{% /notice %}}
{{% /expand %}}
 
