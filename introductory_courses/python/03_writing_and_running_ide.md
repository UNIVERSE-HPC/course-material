---
name: Writing and running Python from an IDE
dependsOn: [introductory_courses.python.02_variables_and_types]
tags: [python]
learningOutcomes:
  - Creating Python scripts
  - Using Integrated Development Environment (IDE)
attribution:
  - citation: >
      "Programming with Python" course by the Carpentries
    url: https://swcarpentry.github.io/python-novice-inflammation/
    image: https://carpentries.org/assets/img/TheCarpentries.svg
    license: CC-BY-4.0
---

## Python scripts

So far we've worked in the REPL which does not allow us to save our programs.

Instead of the interactive mode, Python can read a file that contains Python instructions.
This file is commonly referred to as a Python _script_.

Python scripts are plain text files.
To create a plain text file, you need to use a _text editor_.
When we do programming, typically we will use a special text editor called an IDE, or _integrated development environment_, that contains lots of tools to help us.
We will use Visual Studio Code (vscode), which is a free IDE available for Windows, Mac, and Linux.

:::callout

## Writing text files with Visual Studio Code

Follow these steps:

1. Create a folder on your computer for the python files you will write later
2. Open Visual Studio Code, and go `File` -> `Open Folder...`, and select your folder
3. Create a new file, with `File` -> `New Text File`
4. Click `Select a language` and choose `Python`
5. Write `print("hello world")` in the file, and save it as `my_first_script.py`
6. Press the triangular `Play` button in the top right

:::

Now, change your script to contain the following:

```python
print("hello world")
varint = 1
print("Variable 'varint' is a", type(varint))
varstr = "astring"
print("Variable 'varstr' is a", type(varstr))
```

Press the triangular `Play` button in the top right, and you should see the following output:

```text
hello world
Variable 'varint' is a <class 'int'>
Variable 'varstr' is a <class 'str'>
```

Can you guess what happened?
Python read the file, executing each line one after the other.
This is equivalent to typing the 5 lines in the REPL, except you wrote them all at once in the file.
This allows you to save your program, make changes, and run it again later.

## Programming in Python in practice

When programming in Python, you will find yourself working inside a text editor most of the time, building your program.

There are many different IDEs that you might want to try.
There is no right or wrong IDE: you should use the one you like best.
Some options are:

- [Visual studio code](https://code.visualstudio.com/)
- [PyCharm](https://www.jetbrains.com/pycharm/) (note, the Community Edition is free)
- [Spyder](https://www.spyder-ide.org/)
