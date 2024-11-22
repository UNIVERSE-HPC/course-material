---
name: COOrdinate format
dependsOn: ["scientific_computing.sparse_linear_algebra.01-sparse-matrices"]
tags: []
learningOutcomes:
  - Understand the COOrdinate (COO) format and the advantages of using it. 
  - Be able to create a C00 matrix.
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

As an example of a sparse matrix format, this section describes one of the sparse
formats implemented in Scipy, the The COOrdinate format (COO). This is also known as the
"ijv" or "triplet" format, and stores the non-zero elements in three arrays, `row`,
`col`, and `data`. The `data[i]` value is the non-zero entry in row `row[i]` and column
`col[i]` of the matrix. The advantages of this format are:

- fast format for constructing sparse matrices
- fast conversions to/from the CSR and CSC formats
- fast matrix-vector multiplication
- fast elementwise operations (e.g. multiply each element by 2 is just `data * 2`)

However, slicing using this format is difficult.

Here are some examples of the COO matrix format using
[`scipy.sparse.coo_matrix`](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.coo_matrix.html).
Again, these have been taken from
[scipy-lectures](http://scipy-lectures.org/advanced/scipy_sparse/introduction.html#why-sparse-matrices),
which is an excellent resource and contains examples of the other sparse matrix formats
implemented in Scipy.

### create empty COO matrix

```python
from scipy import sparse
import numpy as np
mtx = sparse.coo_matrix((3, 4), dtype=np.int8)
mtx.todense()
```

Output:

```text
matrix([[0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0]], dtype=int8)
```

### create using (data, ij) tuple

```python
row = np.array([0, 3, 1, 0])
col = np.array([0, 3, 1, 2])
data = np.array([4, 5, 7, 9])
mtx = sparse.coo_matrix((data, (row, col)), shape=(4, 4))
mtx
mtx.todense()
```

Output:

```text
>>> mtx
<4x4 sparse matrix of type '<class 'numpy.int64'>'
        with 4 stored elements in COOrdinate format>
>>> mtx.todense()
matrix([[4, 0, 9, 0],
        [0, 7, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 5]])
```

### duplicates entries are summed together

```python
row = np.array([0, 0, 1, 3, 1, 0, 0])
col = np.array([0, 2, 1, 3, 1, 0, 0])
data = np.array([1, 1, 1, 1, 1, 1, 1])
mtx = sparse.coo_matrix((data, (row, col)), shape=(4, 4))
mtx.todense()
```

Output:

```text
>>> mtx.todense()
matrix([[3, 0, 1, 0],
        [0, 2, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 1]])
```

### no slicing…

```python
mtx[2, 3]
```

Output:

```text
>>> mtx[2, 3]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: 'coo_matrix' object is not subscriptable
```
