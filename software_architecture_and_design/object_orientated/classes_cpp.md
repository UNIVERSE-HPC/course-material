---
name: Classes
dependsOn: []
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

## Structuring Data

Let's assume we're writing a game and we have data related to characters and items.
In a procedural style, with no classes, we might structure our game data with separate vectors for each attribute:

```cpp
// Character data
std::vector<std::string> character_names;
std::vector<int> character_healthPoints;
std::vector<float> character_positions_x;
std::vector<float> character_positions_y;

// Item data
std::vector<std::string> item_names;
std::vector<float> item_positions_x;
std::vector<float> item_positions_y;
```

Structuring our data in this way does have some advantages: it may be possible to update every character's position at once, and having those positions contiguous in memory may make this an efficient operation.

However, as we build out the functionality of characters and items, this will quickly become unwieldy.

1. There will be lots of vectors!
2. A function that operates on a character might need to take many arguments.
3. There is a very loose tie between attributes: the implicit assumption that position `i` contains information about item `i`.

:::::challenge{id=structuring-data title="Structuring Data"}

Write a function, called `move_character` that will take an index and update the character's `x` and `y` position.

::::solution

One possible solution is as follows.

```cpp
void move_character(int character_index, float new_x, float new_y) {
    character_positions_x[character_index] = new_x;
    character_positions_y[character_index] = new_y;
}
```

This works fine, but has a few undesirable characteristics.

1. It's not ideal to have multiple function parameters of the same type next to each other. It's asking for user error, confusing between x and y.
2. There are two lines of code that are almost identical. This is asking for copy-and-paste errors. It's so easy to copy the first line and forget to change the `_x` in the vector name.

::::
:::::

## Classes in C++

In the object oriented paradigm, we use **classes** to help with managing this data and the operations we would want to perform on it.
A class is a **blueprint** for a structured piece of data, so when we create some data using a class, we can be certain that it has the same structure each time.

With the bunch of vectors we had in the example above, we have no guarantee that any fixed structure is being adhered to.
For instance, we could easily add a new character name and forget to add a new position or health.
With a class, if an object is an **instance** of that class (i.e. it was made using that blueprint), we know it will have the structure defined by that class.

You may already be familiar with some classes in C++.
Just now, we have used `vector` and `string`, both of which are classes.

- [std::vector documentation on C++ reference](https://en.cppreference.com/w/cpp/container/vector)
- [std::string documentation on C++ reference](https://en.cppreference.com/w/cpp/string/basic_string)

Vecotrs and strings contain many of the attributes that are common to classes:

- They each hold some data (**attributes** or **state**).
- They also provide some methods used to interact with the data - what can the data do and what can we do to the data?

Read more about the methods that exist on vectors and strings at the links above.

## Encapsulating Data

Let's start by tidying up one of the problems with the previous solution where we moved a character.
It's not ideal that the position is two unrelated floats.
Let's create an class called `Position`:

```cpp
class Position {
public:
    float x;
    float y;

    Position(float x, float y)
        : x(x), y(y) {}
};
```

Let's break down the syntax.

- `class Position { ... };` is the declaration of a class called Position. The class keyword is used to declare a new data type in C++. The contents of the class are enclosed in braces `{}` and followed by a semicolon `;`.

- `public:` is an access specifier. In C++, members of a class (both data members and member functions) can have specific access rules defined by these access specifiers. The public keyword means that the following members can be accessed from any function, whether it's inside or outside of the class.

- `float x;` and `float y;` are data members of the class Position. These members hold the state of an object of the class. In this case, `x` and `y` represent the x and y coordinates of a position. They are public, so they can be directly accessed and modified from outside the class.

- `Position(float x, float y) : x(x), y(y) {}` is a constructor for the Position class. A constructor is a special member function that is automatically called when an object of the class is created. This constructor takes two arguments, `x` and `y`, and initializes the data members of the class with those values.

- The syntax `: x(x), y(y)` is called a member initializer list. This list directly initializes the `x` and `y` data members with the `x` and `y` values passed into the constructor. The `x` and `y` before the parentheses are the data members, and the `x` and `y` inside the parentheses are the constructor parameters.

Here's an example of how to create an object of the Position class:

```cpp
// Creates a Position object at coordinates (10.0, 20.0) called pos
Position pos(10.0, 20.0);
```

We can then modify the position as follows:

```cpp
pos.x = 30.0;  // Changes the x coordinate to 30.0
pos.y = 40.0;  // Changes the y coordinate to 40.0
```

:::::challenge{id=using-position title="Using Position"}

Use this new Position object to tidy up the vectors so we no longer need separate ones for x and y.
Then, update the `move_character` method appropriately.

::::solution

By using a `std::vector<Position>` we can reduce the number of vectors we are storing.

```cpp
// Character data
std::vector<std::string> character_names;
std::vector<int> character_healthPoints;
std::vector<Position> character_positions;

// Item data
std::vector<std::string> item_names;
std::vector<Position> item_positions;
```

We can use the Position object just like it's a float.

- Because the object is simple\*, the compiler will automatically generate the methods necessary to assign one Position to another.
- Because the object is small (it just contains two floats), it does not need to be passed by reference.

```cpp
void move_character(int character_index, Position new_position) {
    character_positions[character_index] = new_position;
}
```

If you're interested in the \* next to simple above, you may want to read about the [rule of zero](https://en.cppreference.com/w/cpp/language/rule_of_three).

::::
:::::

:::::challenge{id=character-and-item-classes title="Write a class for characters and items"}

Write a class that encapsulates the data relating to characters and items.

::::solution

```cpp
class Character {
public:
    std::string name;
    int healthPoints;
    Position position;

    Character(std::string name, int healthPoints, Position position)
        : name(name), healthPoints(healthPoints), position(position) {}
};

class Item {
public:
    std::string name;
    Position position;

    Item(std::string name, Position position)
        : name(name), position(position) {}
};
```

::::
:::::

After writing the three classes `Position`, `Character` and `Item`, we can re-write all of our data that we originally had has:

```cpp
std::vector<Character> characters;
std::vector<Item> items;
```

## Encapsulating Behaviour

In addition to representing a piece of structured data (e.g. a character who has health points and a position, a class can also provide a set of functions, or **methods**, which describe the **behaviours** of the data encapsulated in the instances of that class.
To define the behaviour of a class we add functions which operate on the data the class contains. These functions are the member functions or methods.

Methods on classes are the same as normal functions, except that they live inside a class.
We can relocate our `move_character` method from being a free function to being a member function of the class `character`:

```cpp
class Character {
public:
    std::string name;
    int healthPoints;
    Position position;

    Character(std::string name, int healthPoints, Position position)
        : name(name), healthPoints(healthPoints), position(position) {}

    void move(Position new_position) {
        position = new_position;
    }
};
```

We can then create an object of type `Character` and change its position:

```cpp
// Create a Character object
Position initialPosition(10.0, 20.0);  // Position at coordinates (10.0, 20.0)
Character character("Hero", 100, initialPosition);  // Character named "Hero" with 100 health points at initialPosition

// Call move to change the character's position
Position newPosition(30.0, 40.0);  // New position at coordinates (30.0, 40.0)
character.move(newPosition);  // Move the character to newPosition
```

## Taking the basics further

This basic class syntax above only scratches the surface.
Let's briefly touch on a few additional features.

### Access Specifiers and Getters

It's generally good practice for class data to be private, and for it to be accessed through a function called a **getter**.

We'll start by making the position member of the Character class private. Then, we'll add a public "getter" method to provide access to it.
With a getter, you can control how a class's data is accessed.
Similarly, with a setter, you can control how data is modified.
Our `move` method is an example of a setter, although it would be more standard to call the method `setPosition`.
For example, you could check if new data is valid before setting a variable, or you could make a variable read-only (by providing a getter but not a setter).

```cpp
class Character {
private:
    Position position;

public:
    std::string name;
    int healthPoints;

    Character(std::string name, int healthPoints, Position position)
        : name(name), healthPoints(healthPoints), position(position) {}

    void move(Position new_position) {
        position = new_position;
    }

    Position getPosition() const {
        return position;
    }
};
```

::::challenge{id="make-all-data-private" title="Make all data private"}

Make all data members private, and implement getters to access the data.

:::solution

```cpp
class Character {
private:
    std::string name;
    int healthPoints;
    Position position;

public:
    Character(std::string name, int healthPoints, Position position)
        : name(name), healthPoints(healthPoints), position(position) {}

    void move(Position new_position) {
        position = new_position;
    }

    // Getters
    std::string getName() const {
        return name;
    }

    int getHealthPoints() const {
        return healthPoints;
    }

    Position getPosition() const {
        return position;
    }
};

class Item {
private:
    std::string name;
    Position position;

public:
    Item(std::string name, Position position)
        : name(name), position(position) {}

    // Getters
    std::string getName() const {
        return name;
    }

    Position getPosition() const {
        return position;
    }
};
```

:::
::::

### Operator Overloading

C++ allows classes to define custom behavior for operators (like `+`, `-`, `*`, `/`, `==`, `!=`, and others) when they are used with class objects.
This can make your classes more intuitive to use.

Now let's overload the `==` operator to compare two Character objects.
We'll say that two characters are the same if they have the same name and healthPoints:

```cpp
class Character {
    // ...existing code...

    bool operator==(const Character &other) const {
        return name == other.name && healthPoints == other.healthPoints;
    }
};
```

You can now compare two `Character` objects like this:

```cpp
Character character1("Hero", 100, Position(10.0, 20.0));
Character character2("Hero", 100, Position(30.0, 40.0));
if (character1 == character2) {
    std::cout << "The characters are the same.\n";
} else {
    std::cout << "The characters are different.\n";
}
```

### Static Members

These are members that belong to the class itself, rather than any specific object of the class.
They are declared with the keyword `static`.

Let's add a static member to the Character class to keep track of how many characters have been created.
Every time a new character is created, we'll increment this counter:

```cpp
class Character {
    // ...existing code...

    inline static int characterCount = 0;

public:
    Character(std::string name, int healthPoints, Position position)
        : name(name), healthPoints(healthPoints), position(position) {
        characterCount++;  // this line is new, and the counter is
    }

    static int getCharacterCount() {
        return characterCount;
    }
};
```

In C++, a static data member is usually initialised outside the class body, typically in a source file.
However, as of C++17, you can declare and initialize an inline static data member inside the class body.
The inline keyword tells the compiler that the static member might be defined in multiple translation units (i.e., source files), but they all refer to the same member.

::::challenge{id="experiment-with-classes" title="Experiment with classes"}

Add data or behaviour to these classes.

::::

## Key Points

- Object oriented programming is a programming paradigm based on the concept of classes, which encapsulate data and behaviour.
- Classes allow us to organise data into distinct concepts.
- By breaking down our data into classes, we can reason about the behaviour of parts of our data.

## Full code sample for lession

Here is working code for this lession that defines the classes and then gives an example of how to use them.
You can also see this code in action, and play with it and run it, on [Compiler Explorer](https://gcc.godbolt.org/z/x7b38ba4e):

```cpp
#include <iostream>
#include <string>
#include <vector>

class Position {
public:
    float x;
    float y;

    Position(float x, float y)
        : x(x), y(y) {}

    // Getter methods
    float getX() const { return x; }
    float getY() const { return y; }
};

class Character {
private:
    std::string name;
    int healthPoints;
    Position position;

    inline static int characterCount = 0;

public:
    Character(std::string name, int healthPoints, Position position)
        : name(name), healthPoints(healthPoints), position(position) {
        characterCount++;
    }

    void move(Position new_position) {
        position = new_position;
    }

    // Overloaded == operator
    bool operator==(const Character &other) const {
        return name == other.name && healthPoints == other.healthPoints;
    }

    // Getter methods
    std::string getName() const { return name; }
    int getHealthPoints() const { return healthPoints; }
    Position getPosition() const { return position; }

    // Static getter method
    static int getCharacterCount() { return characterCount; }
};

class Item {
private:
    std::string name;
    Position position;

    inline static int itemCount = 0;

public:
    Item(std::string name, Position position)
        : name(name), position(position) {
        itemCount++;
    }

    // Getter methods
    std::string getName() const { return name; }
    Position getPosition() const { return position; }

    // Static getter method
    static int getItemCount() { return itemCount; }
};

int main() {
    // Create vector of Characters
    std::vector<Character> characters;
    characters.push_back(Character("Hero", 100, Position(10.0, 20.0)));
    characters.push_back(Character("Villain", 100, Position(30.0, 40.0)));

    // Compare Characters
    if (characters.at(0) == characters.at(1)) {
        std::cout << "The characters are the same.\n";
    } else {
        std::cout << "The characters are different.\n";
    }

    // Move first Character
    characters.at(0).move(Position(50.0, 60.0));
    std::cout << characters.at(0).getName() << " is now at (" << characters.at(0).getPosition().getX() << ", " << characters.at(0).getPosition().getY() << ")\n";

    // Create vector of Items
    std::vector<Item> items;
    items.push_back(Item("Sword", Position(10.0, 20.0)));
    items.push_back(Item("Shield", Position(30.0, 40.0)));

    // Print item info
    std::cout << "Found " << items.at(0).getName() << " at (" << items.at(0).getPosition().getX() << ", " << items.at(0).getPosition().getY() << ")\n";

    // Print counts
    std::cout << "Number of characters: " << Character::getCharacterCount() << '\n';
    std::cout << "Number of items: " << Item::getItemCount() << '\n';

    return 0;
}

```
