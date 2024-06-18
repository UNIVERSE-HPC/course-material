---
name: Containerisation with Docker
id: docker
dependsOn: [
  technology_and_tooling.bash_shell
]
files: [
  introduction.md,
  meet-docker.md,
  managing-containers.md,
  running-containers.md,
  docker-hub.md,
  creating-container-images.md,
  advanced-containers.md,
  docker-image-examples.md,
  e01-github-actions.md,
  e02-jekyll-lesson-example.md,
  reproducibility.md,
  setup.md,
]
attribution: 
    - citation: >
        D. M. Eyers, S. L. R. Stevens, A. Turner, C. Koch and J. Cohen. "Reproducible computational environments using containers: Introduction to Docker".
        Version 2020.09a (4a93bd67aa), September 2020. Carpentries Incubator. 
      url: https://github.com/carpentries-incubator/docker-introduction
      image: https://carpentries-incubator.github.io/docker-introduction/assets/img/incubator-logo-blue.svg
      license: CC-BY-4.0
summary: |
    This course aims to introduce the use of Docker containers with the goal of
    using them to effect reproducible computational environments.
---

This session aims to introduce the use of Docker containers with the goal of
using them to effect reproducible computational environments. Such environments
are useful for ensuring reproducible research outputs, for example.

## After completing this session you should

- Have an understanding of what Docker containers are, why they are useful and the common terminology used
- Have a working Docker installation on your local system to allow you to use containers
- Understand how to use existing Docker containers for common tasks
- Be able to build your own Docker containers by understanding both the role of a Dockerfile in building containers, and the syntax used in Dockerfiles
- Understand how to manage Docker containers on your local system
- Appreciate issues around reproducibility in software, understand how containers can address some of these issues and what the limits to reproducibility using containers are
- The practical work in this lesson is primarily aimed at using Docker on your own laptop. Beyond your laptop, software container technologies such as Docker can also be used in the cloud and on high performance computing (HPC) systems. Some of the material in this lesson will be applicable to those environments too.

:::callout

## A note about Docker

Docker is a mature, robust and very widely used application. Nonetheless, it is
still under extensive development. New versions are released regularly often
containing a range of updates and new features.

While we do our best to ensure that this lesson remains up to date and the
descriptions and outputs shown match what you will see on your own computer,
inconsistencies can occur.
:::
