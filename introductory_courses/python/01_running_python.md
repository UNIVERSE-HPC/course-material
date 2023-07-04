---
name: Running Python
dependsOn: [
]
tags: [python]
attribution: 
    - citation: >
        "Programming with Python" course by the Carpentries
      url: https://swcarpentry.github.io/python-novice-inflammation/
      image: https://carpentries.org/assets/img/TheCarpentries.svg
      license: CC-BY-4.0
---

Before starting this course, you should install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/).

Python is a programming language, a collection of syntax rules and keywords that can be used to specify operations to be executed by a computer.
But how to go from these instructions to actual operations carried out by a computer?
This translation and execution is the job of the *Python runtime*, a piece of software that, given some instructions in Python, translates them into machine code and runs them.
The Python runtime is, most of the time, called Python, confusing the software that interprets the language with the language
itself.
This language shortcut is harmless most of the time, but it's good to know that this it *is* a shortcut.

## Starting Python

You can start Python (the *Python runtime*) through the command line or through an application also called 
`Python`.

### macOS - Command Line
To start Python you will need to access the command line through the Terminal. 
There are two ways to open Terminal on Mac.

1. In your Applications folder, open Utilities and double-click on Terminal
2. Press **_Command_** + **_spacebar_** to launch Spotlight. Type `Terminal` and then double-click the search result or hit **_Enter_**

After you have launched Terminal, type the command to start Python

~~~ bash
$ python
~~~

### Windows Users - Command Line
To start Python you will need to access the Terminal.

Press **_Windows Logo Key_** and search for `Terminal`, click the result or press enter.

After you have launched the Terminal, type the command:

~~~ bash
$ python
~~~

### GNU/Linux Users - Command Line

To start Python you will need to access the terminal emulator.
You can usually find it under "Accessories".

After you have launched the terminal emulator, type the command:

~~~ bash
$ python
~~~

###  Using the Python application

Instead of launching the terminal, you can instead search for and launch the Python application in a similar way.

## First steps with Python

To start off with, you can think of Python as a fancy calculator.
You issue commands, hit ENTER, and the result appears on the line below.

Give some of the examples below a try.
Type the lines preceded by `>>>` or `...` and hit ENTER between each one.

Try to guess what these little snippets of Python do, but don't try to understand the details of them yet - it will be clear to you by the end of this course.

~~~ python
>>> 1 + 6

7
~~~

~~~ python
>>> a = 2
>>> b = 3
>>> a + b

5
~~~

~~~ python
>>> print("Just printing this on the screen")

Just printing this on the screen
~~~

~~~ python
>>> word = "Hello"
>>> len(word)

4
~~~

~~~ python
>>> for word in ["Leeds", "Munich", "Marseille"]:
...   print("City name has", len(word), "letters in it.")
...

City name has 5 letters in it.
City name has 6 letters in it.
City name has 9 letters in it.
~~~

~~~ python
>>> for word in ["London", 3, "Marseille"]:
...   print("City name has", len(word), "letters in it.")
... 
City name has 6 letters in it.
Traceback (most recent call last):
File "<stdin>", line 2, in <module>
TypeError: object of type 'int' has no len()
~~~

## Quitting

You can quit Python by typing 

~~~ python
>>> quit()
~~~

then ENTER.

## The REPL

Running Python interactively from the command line, one command after the other, is commonly referred to as using the *Read-Eval-Print-Loop* (REPL, pronounced "repel").
Indeed, when doing so, Python **reads** the command, **evaluates** it, **prints** the result and **loops** (goes back to waiting for the next command).

The REPL allows for very quick feedback while drafting a Python program or exploring data.
It makes it very easy to test a few lines of code and build programs iteratively.
