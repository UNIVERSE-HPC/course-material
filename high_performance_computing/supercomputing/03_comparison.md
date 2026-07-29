---
name: Accelerators
dependsOn: [high_performance_computing.supercomputing.03_distributed_memory]
tags: [foundation]
attribution:
  - citation: >
      This course was developed by Alasdair Wilson as part of the Oxford Research
      Software Engineering group (OxRSE).
    url: https://www.rse.ox.ac.uk/
    image: ./supercomputing/images/2024_oxrse_square.svg
    license: CC-BY-4.0
  - citation: >
      An earlier version of this material was produced for the "Introduction to
      HPC" course by David Henty, Manos Farsarakis, Weronika Filinger, James
      Richings, and Stephen Farr at EPCC, with funding from EuroCC.
    url: https://epcced.github.io/Intro-to-HPC/
    image: https://epcced.github.io/Intro-to-HPC/_static/epcc_logo.svg
    license: CC-BY-4.0
---

## Graphics Processors

Many of the world’s largest supercomputers use some kind of accelerator in addition to standard CPUs.
A popular accelerator is a General Purpose Graphics Processing Unit, or GPGPU.
Since we have seen how a modern multicore CPU works, we can also begin to understand the design of a GPGPU.

Supercomputers have traditionally relied on general-purpose components, primarily multicore CPUs, driven by commercial demand for desktop and business computing.
However, computer gaming also represents a significant market where processor performance is critical.

The massive demand for computer games hardware has driven the development of specialized processors - Graphics Processing Units (GPUs) — designed to produce high-quality 3D graphics.
Although complex in design, a GPU can be thought of as a specialized multicore processor with a vast number of simplified cores.
The cores can be simplified because they have been designed for a single purpose: 3D graphics.
To render high-quality graphics at dozens of frames per second, GPUs require the ability to process massive amounts of data.
To achieve this, they utilize specialised memory with significantly higher bandwidth than the memory typically used by CPUs.

The simplified nature of each core, the much higher number of cores, and the high memory bandwidth means that the performance, in terms of pure number crunching, of a single GPU can easily outstrip that of a CPU at the expense of it being less adaptable.

### Accelerated supercomputers

Despite being developed for a different purpose, GPUs are highly suited for supercomputing: The calculations required for 3D graphics are very similar to those required for scientific simulations - large numbers of simple operations on huge quantities of floating-point numbers.

- designed for very fast floating-point calculation;
- power-efficient due to the simple core design;
- high memory bandwidth to keep the computational cores supplied with data.

The inherently parallel architecture of GPUs, with thousands of computational cores, aligns well with the decades-long focus on parallel processing in supercomputing.

Using GPUs for applications other than graphics is called General Purpose or GPGPU computing. With a relatively small amount of additional development effort, GPU manufacturers produce versions of their processors for the general purpose market.
The supercomputing community directly benefits from the multi-billion pound research and development investments in the games market.

Programming a GPGPU isn’t quite as straightforward as a CPU, and not all applications are suitable for its specialised architecture, but one of the main areas of research in supercomputing at the moment is making GPGPUs easier to program for supercomputing applications.
