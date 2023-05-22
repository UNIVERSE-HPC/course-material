---
id: object_orientated
name: Object-Orientated Programming
dependsOn: [
    software_architecture_and_design.procedural,
]
files: [
    classes.md,
    classes_cpp.md,
    inheritance_and_composition.md,
    inheritance_and_composition_cpp.md,
    polymorphism.md,
    polymorphism_cpp.md,
]
---

## The Object Oriented Paradigm

The Object Oriented Paradigm builds upon the Procedural Paradigm, but builds code around data.

In this paradigm, we shift our focus from the process of computation, to the data with which the computation is being performed.
The overarching idea here, is that data should be structured such that all data related to the same **object** should be stored together.
This is easiest to understand when thinking about the data representing a real-world, physical object, but is also applicable to more abstract concepts.

Since all of the data representing a single object is now stored together, it makes sense to store the code representing the **behaviour** of that object in the same place.

We need some way to describe the template for how an object's data should be structured, which we call a **class**.
So, a class is a template describing the structure of some collection of data, along with the code necessary to describe the behaviour of that data.

You may wish to think of the Object Oriented Paradigm as focussing on the **nouns** of a computation.