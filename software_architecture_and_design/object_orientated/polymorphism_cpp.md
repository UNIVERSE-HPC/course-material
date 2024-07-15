---
name: Polymorphism
dependsOn: [software_architecture_and_design.object_orientated.inheritance_and_composition_cpp]
tags: [cpp]
attribution:
  - citation: >
      This material was adapted from an "Introduction to C++" course developed by the
      Oxford RSE group.
    url: https://www.rse.ox.ac.uk
    image: https://www.rse.ox.ac.uk/images/banner_ox_rse.svg
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Prerequisites

The code blocks in this lesson will assume that some boilerplate C++ code is present.
In particular, we will assume that the following headers are included:

```cpp
#include <iostream>
#include <memory>
#include <vector>
```

We will also assume that you are using the C++17 language standard, or later.
This will be the default with most modern compilers.

Furthermore, for this lesson it will be assumed you are writing all your code in a single file.
Class definitions will be assumed to exist before `main`, and all other code will be assumed to exist within `main`.

## Class-based polymorphism

Class-based polymorphism is a fundamental concept in object-oriented programming that allows objects of different classes to be treated as if they were the same
type, while still maintaining their individual differences.
In other words, polymorphism enables objects to be manipulated in a generic way, irrespective of their specific class or implementation.

In our example, we have warriors and mages that are both types of character.
Our game might store all the characters in a vector, and we might want to loop over that vector and perform an action that might need to be specific to the type of character.
Let's say we want warriors to attack and mages to cast a spell.

In C++ we archive this with **method overriding**:

- We define a **virtual** method on the base class
- We override this method on each derived class

For this lesson we'll simplify the overall example, but feel free to modify your more extensive classes:

```cpp
class Character {
public:
    virtual void performAttack() const {
        // Default implementation
    }
};
```

Here, the **virtual** keyword indicates that this function can be overridden in derived classes.
We can then add the `performAttack()` method to the derived classes:

```cpp
class Warrior : public Character {
public:
    void performAttack() const override {
        std::cout << "Warrior attacks!" << std::endl;
    }
};

class Mage : public Character {
public:
    void performAttack() const override {
        std::cout << "Mage casts a spell!" << std::endl;
    }
};
```

Notice that the **virtual** keyword is onpy present in the base class.

The **override** keyword indicates that the function is intended to override a virtual function from the base class.
It is not mandatory to add the **override** keyword, but it is considered best practice for the following reasons:

- **Explicitness and Clarity**: The `override` keyword clearly indicates that the intention is to override a virtual function from the base class. It serves as a visual cue to programmers that the function is intended to provide a specific implementation for the derived class.

- **Detecting Errors at Compilation**: When you use the `override` keyword, the compiler performs a check to ensure that the function being declared in the derived class is indeed overriding a virtual function from the base class. It helps detect errors, such as misspelled function names or accidental deviations from the base class function signature. If the function in the derived class does not match any base class virtual function, a compilation error is generated, alerting you to the mistake.

We can use this new code in many ways, but in general we will need a pointer or reference to the base class.
Here's an example which we will then break down:

```cpp
std::vector<std::unique_ptr<Character>> characters;
characters.push_back(std::make_unique<Warrior>());
characters.push_back(std::make_unique<Mage>());

for (const auto& character : characters) {
    character->performAttack();
}
```

```text
Warrior attacks!
Mage casts a spell!
```

- `std::vector<std::unique_ptr<Character>> characters;`: This declares a vector named `characters` that holds [`std::unique_ptr` smart pointers](https://en.cppreference.com/w/cpp/memory/unique_ptr) to `Character` objects. The use of `std::unique_ptr` ensures that the ownership and memory management of the objects in the vector are handled automatically.

- `characters.push_back(std::make_unique<Warrior>());`: This adds a new `Warrior` object to the `characters` vector using `std::make_unique`. [`std::make_unique`](https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique) creates a dynamically allocated object of type `Warrior` and returns a `std::unique_ptr` that owns it. The `push_back` function adds the `std::unique_ptr` to the end of the vector.

- `characters.push_back(std::make_unique<Mage>());`: Similarly, this line adds a new `Mage` object to the `characters` vector using `std::make_unique`. It creates a dynamically allocated `Mage` object and adds it to the vector.

- `for (const auto& character : characters) { ... }`: This is a range-based `for` loop that iterates over each element in the `characters` vector. The loop variable `character` is automatically assigned to each element of the vector in each iteration.

- `character->performAttack();`: Within the loop, this calls the `performAttack()` function on each `Character` object. The `->` operator is used to access the member function through the pointer.

During each iteration of the loop, the `performAttack()` function is called on each `Character` object, including both `Warrior` and `Mage` objects.
Polymorphism comes into play here, as the virtual `performAttack()` function is called on each object, and the appropriate overridden implementation in the derived class is executed based on the actual object type.

## Abstract classes

Sometimes we want a base class to define a structure that is common to all derived classes, but we don't want to be able to directly instantiate that object.
In our example, it may be that we can never have a character that is not either a warrior or a mage.
In this case, we would like `Character` to become an abstract class.

An abstract class cannot be instantiated directly, and it is meant to serve as a base for derived classes by providing an interface that derived classes must implement.
A class becomes abstract if it has at least one _pure virtual function_, that is, a virtual function that does not have an implementaiton.

1. **Pure Virtual Function**: The `Character` class would have at least one pure virtual function, declared as follows:

   ```cpp
   virtual void performAttack() const = 0;
   ```

   The `= 0` at the end of the function declaration signifies that this function is pure virtual. An abstract class must have at least one pure virtual function.

2. **Cannot Instantiate Abstract Class**: As an abstract class, `Character` cannot be directly instantiated. You cannot create objects of the `Character` class, but you can declare pointers or references to it.

3. **Derived Class Implementations**: Any class derived from the abstract `Character` class must provide implementations for all pure virtual functions declared in the base class. This is necessary to make the derived class concrete and enable object creation.

4. **Polymorphism with Derived Classes**: Polymorphism can still be achieved with the derived classes of the abstract `Character` class. You can use pointers or references of the abstract base class type to point to objects of derived classes. When calling virtual functions through these pointers or references, the appropriate overridden functions in the derived classes will be executed.

Here's an example to illustrate that Character is now abstract:

```cpp
class Character {
public:
    virtual void performAttack() const = 0;  // Pure virtual function

    virtual ~Character() = default;
};

class Warrior : public Character {
public:
    void performAttack() const override {
        std::cout << "Warrior attacks!" << std::endl;
    }
};

class Mage : public Character {
public:
    void performAttack() const override {
        std::cout << "Mage casts a spell!" << std::endl;
    }
};

int main() {
    std::vector<std::unique_ptr<Character>> characters;
    characters.push_back(std::make_unique<Warrior>());
    characters.push_back(std::make_unique<Mage>());

    for (const auto& character : characters) {
        character->performAttack();
    }

    return 0;
}
```

In this example, the `Character` class becomes abstract with the addition of the pure virtual function `performAttack()`. The derived classes `Warrior` and `Mage` override this function, providing their specific implementations.

By making the `Character` class abstract, you enforce that any derived class must provide an implementation for the pure virtual function `performAttack()`. This allows you to define a common interface for all derived classes while ensuring that each derived class implements its unique behavior.

Note that we have also added a virtual destructor:

```cpp
virtual ~Character() = default;
```

- In C++, when an object is deleted through a pointer to a base class type, the destructor of the base class is called, but not the derived class destructors.
  This can lead to a problem known as _slicing_, where only the base class portion of the object is destroyed, resulting in a potential resource leak or undefined behavior.
- When deleting an object through a base class pointer or reference, the derived class destructor is also called, ensuring that the derived class's resources are properly released.
- In the given example, although the `Character` class does not contain any member variables that need explicit cleanup, adding a virtual destructor is a good practice for future-proofing the code. If derived classes add their own resources or dynamically allocated memory, the virtual destructor will ensure proper destruction of those resources when deleting derived class objects through base class pointers.
- Therefore, when making a class abstract and intended to be used as a base class, it is generally advisable to include a virtual destructor in the base class, even if it has no explicit cleanup to perform.

## Key Points

- Class-based Polymorphism in programming languages allows objects of different classes to be treated as if they were the same type.
- Classes can be made abstract by providing at least one pure virtual function, but you should remember the virual destructor, too.

## Full code sample for lession

Here is working code for this lession that defines the classes and then gives an example of how to use them.
You can also see this code in action, and play with it and run it, on [Compiler Explorer](https://gcc.godbolt.org/z/KoaoET9v9):

```cpp
#include <iostream>
#include <vector>
#include <memory>

class Character {
public:
    virtual void performAttack() const = 0;

    virtual ~Character() = default;
};

class Warrior : public Character {
public:
    void performAttack() const override {
        std::cout << "Warrior attacks!" << std::endl;
    }
};

class Mage : public Character {
public:
    void performAttack() const override {
        std::cout << "Mage casts a spell!" << std::endl;
    }
};

int main() {
    std::vector<std::unique_ptr<Character>> characters;
    characters.push_back(std::make_unique<Warrior>());
    characters.push_back(std::make_unique<Mage>());

    for (const auto& character : characters) {
        character->performAttack();
    }

    return 0;
}
```
