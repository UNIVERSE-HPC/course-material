---
name: Running Python
dependsOn: [
]
tags: [python]
---
Python is a programming language, a collection of syntax rules,
keywords that can be used to specify operations to be executed by a
computer. But how to go from these instructions to actual operations
carried out by a computer?  This translation and execution is the job
of the *Python runtime*, a piece of software that, given some
instructions in Python, translates them into machine code and runs
them.  The Python runtime is, most of the time, called Python,
confusign the software that interprets the language with the language
itself.  This language shortcut is harmless most of the time, but it's good to
know that this it *is* a shortcut.

## Starting Python

You can start Python (understand the Python runtime) through the command line or through an application called 
`Anaconda Navigator`. Anaconda Navigator is included as part of the Anaconda Python distribution.

### macOS - Command Line
To start Python you will need to access the command line through the Terminal. 
There are two ways to open Terminal on Mac.

1. In your Applications folder, open Utilities and double-click on Terminal
2. Press <kbd>Command</kbd> + <kbd>spacebar</kbd> to launch Spotlight. Type `Terminal` and then 
double-click the search result or hit <kbd>Enter</kbd>

After you have launched Terminal, type the command to start Python

~~~
$ python
~~~
{: .language-bash}

### Windows Users - Command Line
To start Python you will need to access the Anaconda Prompt.

Press <kbd>Windows Logo Key</kbd> and search for `Anaconda Prompt`, click the result or press enter.

After you have launched the Anaconda Prompt, type the command:

~~~
$ python
~~~
{: .language-bash}

### GNU/Linux Users - Command Line

To start Python you will need to access the terminal emulator. You can usually find it under "Accessories".

After you have launched the terminal emulator, type the command:

~~~
$ python
~~~
{: .language-bash}

###  Anaconda Navigator

To start Python from Anaconda Navigator you must first [start Anaconda Navigator (click for detailed instructions on macOS, Windows, and Linux)](https://docs.anaconda.com/anaconda/navigator/getting-started/#starting-navigator). You can search for Anaconda Navigator via Spotlight on macOS (<kbd>Command</kbd> + <kbd>spacebar</kbd>), the Windows search function (<kbd>Windows Logo Key</kbd>) or opening a terminal shell and executing the `anaconda-navigator` executable from the command line.

After you have launched Anaconda Navigator, click the `Launch` button
under "CMD.exe prompt". You may need to scroll down to find it.

Here is a screenshot of an Anaconda Navigator page similar to the one that should open on either macOS
or Windows.

<p align='center'>
    <img alt="Anaconda Navigator landing page" src="../fig/0_anaconda_navigator_landing_page.png" width="750"/>
</p>

## First steps with Python

To start off with, you can think of Python as a fancy calculator.  You
issue commands, hit ENTER, and the result appears on the line below.

Give some of the examples below a try. Type the lines preceded by
`>>>` or `...` and hit ENTER between each one.

Try to guess what these little snippets of Python do, but don't try to
understand the details of them yet - it will be clear to you by the
end of this course.

~~~
>>> 1 + 6
7
~~~
{: .language-python}

~~~
>>> a = 2
>>> b = 3
>>> a + b
5
~~~
{: .language-python}

~~~
>>> print("Just printing this on the screen")
Just printing this on the screen
~~~
{: .language-python}

~~~
>>> word = "Hello"
>>> len(word)
4
~~~
{: .language-python}

~~~
>>> for word in ["Leeds", "Munich", "Marseille"]:
...   print("City name has", len(word), "letters in it.")
...
... 
City name has 5 letters in it.
City name has 6 letters in it.
City name has 9 letters in it.
~~~
{: .language-python}

~~~
>>> for word in ["London", 3, "Marseille"]:
...   print("City name has", len(word), "letters in it.")
... 
City name has 6 letters in it.
Traceback (most recent call last):
File "<stdin>", line 2, in <module>
    TypeError: object of type 'int' has no len()
    ~~~
    {: .language-python}

    ## Quitting

    You can quit Python by typing 

    ~~~
    >>> quit()
    ~~~
    {: .language-python}

    then ENTER.

    ## The REPL

    Running Python interactively from the command line, one command after
    the other, is commonly referred to as using the *Read-Eval-Print-Loop*
    (REPL, pronounced "repel").  Indeed, when doing so, Python **reads**
    the command, **evaluates** it, **prints** the result and **loops**
    (goes back to waiting for the next command).

    The REPL allows for very quick feedback while drafting a Python
    program or exploring data.  It makes it very easy to test a few lines
    of code and build programs iteratively.

    {% include links.md %}