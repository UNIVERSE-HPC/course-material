---
name: Inheritance and Composition
dependsOn: [software_architecture_and_design.object_orientated.classes_cpp]
tags: [cpp]
attribution:
  - citation: >
      This material was adapted from an "Introduction to C++" course developed by the
      Oxford RSE group.
    url: https://www.rse.ox.ac.uk
    image: https://www.rse.ox.ac.uk/sites/default/files/styles/listing_tile_text_displayed_image/public/rse/images/media/oxrse_banner_2.png
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
#include <string>
#include <vector>
```

We will also assume that you are using the C++17 language standard, or later.
This will be the default with most modern compilers.

Furthermore, for this lesson it will be assumed you are writing all your code in a single file.
Class definitions will be assumed to exist before `main`, and all other code will be assumed to exist within `main`.

## Relationships Between Classes

We now have a language construct for grouping data and behaviour related to a single conceptual object.
The next step we need to take is to describe the relationships between the concepts in our code.

There are two fundamental types of relationship between objects which we need to be able to describe:

1. Ownership - x **has a** y - this is **composition**
2. Identity - x **is a** y - this is **inheritance**

### Composition

In the real world, when something is composed of something else, there exists a relationship where one thing is made up of other things.
In the same way, in object oriented programming, we can make classes which have components from other classes.
This is known as **composition**.

We often use composition in circumstances where we can say 'x _has a_ y' - for example in our game, we might want to say that a character _has_ an inventory, and that an inventory _has_ items.

In the case of our example, we're already saying that a character _has a_ position, so we're already using composition here.

:::::challenge{id=inventory title="Write an inventory"}

Write a class called `Inventory` that has a capacity, and a vector of `Item` objects.
These should be private, with a method that adds an item unless the inventory is full, and anything else you think is relevant.

Modify your `Character` class to contain an `Inventory` data member.

::::solution

Here is an example of what that might look like:

```cpp
class Inventory {
private:
    std::vector<Item> items;
    int capacity;

public:
    Inventory(int capacity)
        : capacity(capacity) {}

    bool addItem(Item item) { // Passed by value
        if (items.size() < capacity) {
            items.push_back(item);
            return true;  // Item was added successfully
        }
        return false;  // Inventory is full
    }

    // ...Other Inventory methods...

    const Item& getItem(int index) const { // Returned by reference
        return items.at(index);
    }
};

class Character {
private:
    // ...Other Character data members...
    Inventory inventory;

public:
    Character(const std::string& name, int health, const Position& position, int inventoryCapacity)
        : name(name), health(health), position(position), inventory(inventoryCapacity) {
        characterCount++;
    }

    // ...Other Character methods...

    bool addItemToInventory(Item item) {
        return inventory.addItem(item);
    }

    const Item& getInventoryItem(int index) const {
        return inventory.getItem(index);
    }
};
```

::::
:::::

We now have several examples of composition:

- Character _has a_ position
- Item _has a_ position
- Characher _has an_ inventory
- Inventory _has many_ items

You can see how we can build quickly build up complex behaviours.
Now have a think: would it be simple to build this behaviour without classes?
It would probably be very messy.

### Inheritance

The other type of relationship used in object oriented programming is **inheritance**.
Inheritance is about data and behaviour shared by classes, because they have some shared identity - 'x _is a_ y'.

For instance, we might have two types of character: warriors and mages.
We can create two classes: `Warrior` and `Mage`.
But, fundamentally, they are both characters and have common code such as an inventory and a position.
We should not duplicate this code.

We achieve this through _inheritance_.
If class `Warrior` inherits from (_is a_) `Character`, we say that `Character` is the **base class**, **parent class**, or **superclass** of `Warrior`.
We say that `Warrior` is a **derived class**, **child class**, or **subclass** of `Character`.

The base class provides a set of attributes and behaviors that the derived class can inherit.
The derived class can then add or override these attributes and behaviors as needed.
This terminology is common across many object-oriented programming languages.

A Warrior class may look something like this:

```cpp
class Warrior : public Character {
private:
    int strength;

public:
    Warrior(std::string name, int health, Position position, int inventoryCapacity, int strength)
        : Character(name, health, position, inventoryCapacity), strength(strength) {}

    void physicalAttack() {
        // Unique behavior for Warrior...
    }

    int getStrength() const {
        return strength;
    }
};
```

Let's examine the syntax:

1. **Inheritance declaration**: The colon (`:`) following the class name `Warrior` signifies inheritance. `public Character` specifies that `Warrior` is a derived class of the `Character` base class. The `public` keyword here specifies the type of inheritance: in this case, `public` means that `public` and `protected` members of the base class remain `public` and `protected` in the derived class.

2. **Private member variable**: `int strength;` declares a private integer variable `strength` which is specific to the `Warrior` class.

3. **Constructor**: The `Warrior` constructor accepts the same parameters as the `Character` constructor, plus an additional `strength` parameter. The constructor uses a member initializer list to call the `Character` constructor and initialize the `strength` member variable.

4. **Methods**: `void physicalAttack()` is a public method unique to `Warrior`. This could be an example of method overriding, if there was a `physicalAttack()` method in the `Character` class that we wanted to behave differently for `Warrior`. `int getStrength() const` is a getter method for `strength`.

Note: in this example, `Character(name, health, position, inventoryCapacity)` is the call to the base class constructor, which will be executed before the body of the `Warrior` constructor.
After the base class constructor has been called, the `Warrior` constructor will continue with its own initialisation, setting the value of `strength` in this case.
This sequence ensures that the base class portion of the `Warrior` object is properly constructed before the `Warrior` constructor attempts to use it or modify it.
This is a fundamental feature of how constructors and inheritance work together in C++.

:::::challenge{id=mage title="Write a Mage class"}

Write a class called `Mage` that inherits from `Character`, and give it some unique data and behaviour.

::::solution

Here is an example of what that might look like:

```cpp
class Mage : public Character {
private:
    int manaPoints;

public:
    Mage(std::string name, int health, Position position, int inventoryCapacity, int manaPoints)
        : Character(name, health, position, inventoryCapacity), manaPoints(manaPoints) {}

    void castSpell() {
        // Unique behavior for Mage...
    }

    int getManaPoints() const {
        return manaPoints;
    }
};
```

::::
:::::

## Composition vs Inheritance

When deciding how to implement a model of a particular system, you often have a choice of either composition or inheritance, where there is no obviously correct choice.
For example, it's not obvious whether a photocopier _is a_ printer and _is a_ scanner, or _has a_ printer and _has a_ scanner.

Both of these would be perfectly valid models and would work for most purposes.
However, unless there's something about how you need to use the model which would benefit from using a model based on inheritance, it's usually recommended to opt for **composition over inheritance**.

This is a common design principle in the object oriented paradigm and is worth remembering, as it's very common for people to overuse inheritance once they've been introduced to it.

Composition, on the other hand, tends to offer greater flexibility.
It allows you to change behavior on the fly by changing the component at runtime and leads to a more decoupled system, which is easier to maintain and evolve.
The downside can be that it might result in a little more boilerplate code as you delegate methods to the component classes.

:::::challenge{id=swords-and-shields title="Swords and Shields"}

Swords and shields are types of `Item`.
A warrior can carry a sword and a shield, but a mage can only carry a sword.

Update your code to reflect this, and identify the inheritance and composition necessary to achieve this.

::::solution

Here is an example of what that might look like:

```cpp
class Sword : public Item {
private:
    int damage;

public:
    Sword(std::string name, int damage)
        : Item(name), damage(damage) {}

    int getDamage() const {
        return damage;
    }
};

class Shield : public Item {
private:
    int defense;

public:
    Shield(std::string name, int defense)
        : Item(name), defense(defense) {}

    int getDefense() const {
        return defense;
    }
};



class Warrior : public Character {
private:
    Sword* equippedSword = nullptr;
    Shield* equippedShield = nullptr;

public:
    // ...

    void equipSword(Sword* sword) {
        equippedSword = sword;
    }

    void equipShield(Shield* shield) {
        equippedShield = shield;
    }

    Sword* getEquippedSword() {
        return equippedSword;
    }

    Shield* getEquippedShield() {
        return equippedShield;
    }
};

class Mage : public Character {
private:
    Sword* equippedSword = nullptr;

public:
    // ...

    void equipSword(Sword* sword) {
        equippedSword = sword;
    }

    Sword* getEquippedSword() {
        return equippedSword;
    }
};
```

Then we can use that functionality like this:

```cpp
Sword sword("Excalibur", 10);
Shield shield("Aegis", 5);

Warrior warrior("Arthur", Position(0, 0));
Mage mage("Merlin", Position(1, 1));

warrior.getInventory().addItemToInventory(sword);
warrior.getInventory().addItemToInventory(shield);

warrior.equipSword(&sword);
warrior.equipShield(&shield);

mage.getInventory().addItemToInventory(sword);
mage.equipSword(&sword);

if (warrior.getEquippedSword()) {
    std::cout << "Warrior's sword: " << warrior.getEquippedSword()->getName() << std::endl;
}

if (warrior.getEquippedShield()) {
    std::cout << "Warrior's shield: " << warrior.getEquippedShield()->getName() << std::endl;
}

if (mage.getEquippedSword()) {
    std::cout << "Mage's sword: " << mage.getEquippedSword()->getName() << std::endl;
}

return 0;
```

::::
:::::

## Key Points

- Relationships between concepts can be described using inheritance (_is a_) and composition (_has a_).

## Full code sample for lession

Here is working code for this lesson that defines the classes and then gives an example of how to use them.
You can also see this code in action, and play with it and run it, on [Compiler Explorer](https://gcc.godbolt.org/z/K51dPz1os):

```cpp
#include <iostream>
#include <vector>
#include <string>

class Position {
public:
    float x;
    float y;

    Position(float x, float y)
        : x(x), y(y) {}
};

class Item {
private:
    std::string name;

public:
    Item(const std::string& name)
        : name(name) {}

    std::string getName() const {
        return name;
    }
};

class Sword : public Item {
private:
    int damage;

public:
    Sword(const std::string& name, int damage)
        : Item(name), damage(damage) {}

    int getDamage() const {
        return damage;
    }
};

class Shield : public Item {
private:
    int defense;

public:
    Shield(const std::string& name, int defense)
        : Item(name), defense(defense) {}

    int getDefense() const {
        return defense;
    }
};

class Inventory {
private:
    int capacity;
    std::vector<Item> items;

public:
    Inventory(int capacity)
        : capacity(capacity) {}

    bool addItemToInventory(const Item& item) {
        if (items.size() < capacity) {
            items.push_back(item);
            return true;
        }
        return false;
    }

    Item& getInventoryItem(size_t index) {
        return items.at(index);
    }
};

class Character {
private:
    static inline int characterCount = 0;

    std::string name;
    Position position;
    Inventory inventory;

public:
    Character(const std::string& name, Position position)
        : name(name), position(position), inventory(10) {
        ++characterCount;
    }

    std::string getName() const {
        return name;
    }

    Position getPosition() const {
        return position;
    }

    Inventory& getInventory() {
        return inventory;
    }

    static int getCharacterCount() {
        return characterCount;
    }
};

class Warrior : public Character {
private:
    Sword* equippedSword = nullptr;
    Shield* equippedShield = nullptr;

public:
    Warrior(const std::string& name, Position position)
        : Character(name, position) {}

    void equipSword(Sword* sword) {
        equippedSword = sword;
    }

    void equipShield(Shield* shield) {
        equippedShield = shield;
    }

    Sword* getEquippedSword() {
        return equippedSword;
    }

    Shield* getEquippedShield() {
        return equippedShield;
    }
};

class Mage : public Character {
private:
    Sword* equippedSword = nullptr;

public:
    Mage(const std::string& name, Position position)
        : Character(name, position) {}

    void equipSword(Sword* sword) {
        equippedSword = sword;
    }

    Sword* getEquippedSword() {
        return equippedSword;
    }
};

int main() {
    Sword sword("Excalibur", 10);
    Shield shield("Aegis", 5);

    Warrior warrior("Arthur", Position(0, 0));
    Mage mage("Merlin", Position(1, 1));

    warrior.getInventory().addItemToInventory(sword);
    warrior.getInventory().addItemToInventory(shield);

    warrior.equipSword(&sword);
    warrior.equipShield(&shield);

    mage.getInventory().addItemToInventory(sword);
    mage.equipSword(&sword);

    if (warrior.getEquippedSword()) {
        std::cout << "Warrior's sword: " << warrior.getEquippedSword()->getName() << std::endl;
    }

    if (warrior.getEquippedShield()) {
        std::cout << "Warrior's shield: " << warrior.getEquippedShield()->getName() << std::endl;
    }

    if (mage.getEquippedSword()) {
        std::cout << "Mage's sword: " << mage.getEquippedSword()->getName() << std::endl;
    }

    return 0;
}
```
