---
id: object_orientated
name: Object-Orientated Programming
dependsOn: [software_architecture_and_design.procedural]
files:
  [
    classes.md,
    classes_cpp.md,
    inheritance_and_composition.md,
    inheritance_and_composition_cpp.md,
    polymorphism.md,
    polymorphism_cpp.md,
  ]
summary: |
  The Object Oriented Paradigm builds upon the Procedural Paradigm, but builds code around data.
  This course will introduce you to the basics of Object Oriented Programming in either Python or C++.
attribution:
  - citation: This material has been adapted from the "Software Engineering" module of the SABS R³ Center for Doctoral Training.
    url: https://www.sabsr3.ox.ac.uk
    image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## The Object Oriented Paradigm

The Object-Oriented Paradigm (OOP) builds upon the Procedural Paradigm but shifts the focus from actions and processes to organising code around data.

In OOP, we prioritise the data involved in computation rather than just the steps needed to perform that computation.
The core idea is that data related to the same entity, or **object**, should be grouped together.
This is intuitive when thinking about physical objects in the real world (a ball might have a size, a colour, and a position), but the concept also applies to more abstract entities.

By keeping all data related to an object together, it also makes sense to store the code (i.e., methods or functions) that define the **behaviour** of that object alongside the data.

To define how an object's data and behaviour should be organised, we use a **class**.
A class serves as a blueprint that specifies both the structure of the data and the behaviour associated with that data.

OOP also introduces key concepts such as **inheritance**, which allows new classes to build upon existing ones, promoting code reuse, and **encapsulation**, which allows the internal details of an object to be hidden from the outside.
Additionally, **polymorphism** enables objects of different classes to be treated as instances of the same class through shared interfaces, allowing for more flexible and reusable code.

You can think of OOP as focusing on the *nouns* of a computation (the objects themselves), while the procedural paradigm often emphasises the *verbs* (the actions).
