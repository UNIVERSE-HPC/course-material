---
name:Using CuPy for GPU Computation
dependsOn: []
tags: []
---

## Introduction to CuPy
CuPy is an open-source GPU-accelerated array library that provides a NumPy-compatible API. It allows NumPy-like operations to run on NVIDIA GPUs by leveraging CUDA (Compute Unified Device Architecture) for massive parallelism.

## Installing CuPy

CuPy can be installed using the pip package installer:
```bash
pip install cupy
```

After installation, you can verify that CuPy detects your GPU:
```python
import cupy as cp
print(cp.cuda.runtime.getDeviceCount())
```

This should return 1 or more devices is a GPU is available.

## Basic Usage

CuPy arrays work just like NumPy arrays, but they live in GPU memory.

```python
import cupy as cp

x = cp.array([1,2,3,4,5])
print(x)
```

Most NumPy functions have a CuPy equivalent
