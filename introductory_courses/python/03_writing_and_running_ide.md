---
name: Writing and running Python from an IDE
dependsOn: [
    introductory_courses.python.02_variables_and_types
]
tags: [python]
---

## Python scripts

So far we've worked in the REPL and we cannot save our programs.

Instead of the interactive mode, Python can read a file that contains
Python instructions.  This file is commonly referred to as a Python *script*.

Python scripts are plain text files (see below for a discussion about
plain vs rich text formats).  To create a plain text file, you need to
use a *text editor*.  Depending on whether you're using GNU/Linux,
MacOS or Windows, you'll need different software for that. Click on
the box below that corresponds to your situation.

> ## Writing text files with nano on macOS or GNU/Linux
>
> `nano` is a bare-bones text editor that's available on most
> GNU/Linux distributions and macOS.  `nano` runs inside a terminal emulator.
> To create a new Python file with `nano`, start a terminal emulator ("Terminal" app on macOS) and
> type:
{: .callout}

~~~
  nano myfile.py
~~~
{: .language-bash}

> Useful commands are described at the bottom of the terminal window. The symbol `^` means the Control key. So `^X` means hold the Control key and press the x key.
{: .callout}


> ## Writing text files with notepad on Windows
>
> Here's how to use notepad on windows
{: .callout}

> ## Text vs. Whatever
>
> We usually call programs like Microsoft Word or LibreOffice Writer "text
> editors", but we need to be a bit more careful when it comes to
> programming. By default, Microsoft Word uses `.docx` files to store not
> only text, but also formatting information about fonts, headings, and so
> on. This extra information isn't stored as characters and doesn't mean
> anything to tools like `head`: they expect input files to contain
> nothing but the letters, digits, and punctuation on a standard computer
> keyboard. When editing programs, therefore, you must either use a plain
> text editor, or be careful to save files as plain text.
{: .callout}

Let's try to write a simple Python script.  Open a new plain text file (with the method described above depending on your operating system), name it for instance `myfirstscript.py`.
Write the following python code and save the file.

~~~
print("hello world")
varint = 1
print("Variable 'varint' is a", type(varint))
varstr = "astring"
print("Variable 'varfl' is a", type(varfl))
~~~
{: .language-python}

Now let's execute this script. In the terminal (macOS/Linux) or the Anaconda prompt (Windows),
type

~~~
$ python myfirstscript.py
hello world
Variable 'varint' is a <class 'int'>
Variable 'varstr' is a <class 'str'>
~~~
{: .language-python}

Can you guess what happened? Python read the file, executing each line
one after the other.  This is equivalent to typing the 5 lines in the
REPL, except you wrote them once and for all in the file.

## Programming in Python in practice

When programming in Python, you will find yourself working inside a
text editor most of the time, building your program. You can also go
back and forth between the text editor and the REPL to try things out
in an interactive way, for instance if you're unsure about syntax.
When you're happy with your program, you can run it with the command
`python <yourfile>.py`.

This means your need three pieces of software running concurrently:

- A text editor.
- The Python REPL.
- A command-prompt or terminal emulator.

You could go a long way with this, but this quickly goes unwieldly,
especially as your programms grow.  We now learn about Spyder, which
is a software that brings everything under one roof

## Using Spyder

Spyder is software that provides a convenient environment to develop Python programs.

You can start it from the Anaconda navigator or from the command line by typing `spyder`.

On the left is a text editor, on the bottom right a Python REPL. The top right corner is dedicated to displaying documentation.

Spyder is an **I**ntegrated **D**evelopment **Environment**. It
combines a text editor and a python REPL. You can also run Python
scripts directly from Spyder by clicking on the green arrow in the
taskbar.

In addition, Spyder reports syntax errors in real time, reminds you of the parameters for a given function, includes a debugger, allows to send a selection to the REPL for execution... and more. It's all about developping in Python in an efficient way.

### Creating a new file
- Setting the current dir
- Creating new file

### Running the script

### Using the REPL

### From script to REPL

- Running a line or selection
- Cells

> ## A word on IPython
>
> IPython is a Python REPL that builds on top of the default one to
> provide more functionalities.
{: .callout}