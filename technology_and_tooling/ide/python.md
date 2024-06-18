---
name: VSCode
dependsOn: [
]
tags: [python]

attribution:
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1 
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0


---

## Introduction to VSCode

Microsoft's VSCode is a lightweight IDE which is great when starting out developing
programs. It not only supports Python, but also C++, C#, JavaScript, CSS, and Java,
amongst others. It's also available for macOS, Linux, and Windows. Whilst lightweight,
it's features can be readily extended for a variety of languages via installation of
plugins to suit your needs, and you can even develop your own plugins for VSCode. As
well as features like live debugging and context-sensitive code autocompletion, other
notable features include:

- Revision/version control support: ability to work with Git source code repositories,
  uploading and synchronising changes to/from such repositories on e.g. GitHub. We'll be
  covering Git version control later in the course
- Remote development: enabling users to work
  with source code and projects that are stored on remote machines or in virtual
  environments. This functionality provides greater flexibility for developers who
  need to work with code that is not stored locally on their machine.
- Live code development sharing: without exchanging changes using version control, you
  can view live changes being made by another team member collaboratively within your
  own VSCode editor

## Installation instructions for VSCode

To install Visual Studio Code, follow these steps:

1. Go to the [official website](https://code.visualstudio.com/) and click the "Download for Windows" button, or choose the appropriate download link for your operating system.

2. Once the installer has finished downloading, run the file and follow the prompts to install VSCode.

3. After installation is complete, launch VSCode.

For more detailed installation instructions, see the [official documentation](https://code.visualstudio.com/docs/setup/setup-overview).

## Running VSCode for the First Time

A current project in VSCode is defined by a folder, either on on your local
harddrive or on a remote folder or container. When you open VSCode for the first
time it will prompt you to select a folder. Create a new, empty folder on your
computer and open this folder in VSCode.

:::callout

### Trusting Code

You may be asked whether you trust the authors of the files in this folder. Select the
checkbox, and click 'Yes, I trust the authors' (although in general use some caution is
recommended!)
:::

This directory is now the current working directory for VSCode, so when we run scripts
from VSCode, this will be the working directory they'll run from.

If you'd like to explore VSCode in more depth than this course offers, see the [VSCode
documentation](https://code.visualstudio.com/docs).

## Our First Python Standalone Script

Let's create our first Python script in VSCode and save it:

- Select `File` -> `New Text File` from the menu. A new file will appear.
- On line 1, you'll see a message about selecting a language. Click on `Select a
  language`, and type in `python`, and select `Python (python) Built-In`
- Select `File` > `Save As...`. You'll find yourself in your root project directory.
  Enter the filename `hello_world.py` at the top and select `Save`.

Let's start with a classic 'Hello world' script. Enter this into the editor:

~~~python
print('Hello world!')
~~~

VSCode comes with Python support built-in. You'll notice that as you type, the editor is
suggesting possible statements, functions (and also variables and other Python
artifacts) that match what you've typed so far. When you write a function it fully
recognises and understands, it will also pop-up some context-sensitive help about the
function itself, including any documentation associated with it and a breakdown of the
function's arguments. This is very helpful when dealing with libraries with many modules
and functions. Also, for convenience, if you've only half-typed a function, variable,
statement, etc. that it recognises as the only option, you can press `Tab` and it will
autocomplete the rest of the typing for you.

You may find you see a `Python - Get Started` window tab pop up that gives you some
things to do next in Python. But for now, we'll keep editing our file in the
`hello_world.py` tab, so select that. Once you've finished, select `File` -> `Save`.

Now let's try running our script from within VSCode. Select the `Run` icon on the far
left navigation bar (it looks like an arrow pointing right with a bug in it), then
select `Run and Debug`. It will ask you to `Select a debug configuration`, so select
`Python File`. It will now run our script, and you should see a terminal window pop-up
at the bottom, with something like the following text in it:

~~~bash
user@mycomputer:~/my/project/dir$  /usr/bin/env /usr/bin/python3 /home/user/.vscode/extensions/ms-python.python-2022.14.0/pythonFiles/lib/python/debugpy/launcher 38613 -- /home/user/my/project/dir/hello_world.py
Hello world!
~~~

Here, we can see that the interpreter `/usr/bin/python3` has been used to run the VSCode
debugger on our `hello_world.py` script, which produces the shown 'Hello world!' output.

## Setting up a Virtual Environment

Before we start using VSCode beyond a 'Hello world' example, we should set up a new
*virtual environment* for running our Python scripts. We are currently using the
*global* installation of Python 3, and this is not considered good development practice.

:::callout

## Why use a Virtual Environment, and what are they?

Consider developing a number of different Python scripts that each have their own
package dependencies (and versions of those dependencies) on the same machine. It could
quickly become confusing as to which packages and package versions are required by each
script, making it difficult for others to run your script themselves (or yourself on
another machine!). Additionally, different scripts may need to use different versions of
a given package.

A virtual environment is a self-contained directory tree that houses a specific Python
interpreter and specific versions of a number of Python packages, so as package
dependencies are added to a script (or set of scripts), you can add them to this
specific virtual environment. So, you can avoid a great deal of confusion by having
separate virtual environments for each script.
:::

Go back to the terminal window, and exit the Python interpreter (either by typing
`exit()` or pressing `Ctrl` and `D` at the same time).

In the Bash shell, type the following (whilst in the root project directory):

~~~bash
python3 -m venv venv
~~~

This instructs Python to construct a new Python virtual environment for us. Within our
`code` directory now, you should see a new `venv` directory. This will contain a
localised copy of the Python3 interpreter, and any associated libraries we wish to
install. But this local environment is particular to our current work; if we were to
start a new project, we'd create another virtual environment for that one, and so on.

The first `venv` is the name of the tool we need to use to create a virtual environment,
while the second `venv` is the name of the directory that the virtual environment will
be put in. Most people use either `venv` or `env` as the name for their virtual
environment.

We can activate this virtual environment, and see what it contains, by doing:

~~~bash
source venv/bin/activate
pip3 list
~~~

`source` runs a script that activates our virtual environment. `pip` is the de-facto
Python package installer; in this case we're using the version for Python 3 specifically
and asking it to list the packages that are currently resident in the virtual
environment:

~~~text
Package       Version
------------- -------
pip        22.0.2
setuptools 59.6.0
~~~

In addition to Python which is also installed, as we can see, we don't have any other
packages installed yet, aside from `pip` itself, and `setuptools` (which contains
functionality for building and distributing Python packages).

Note that this virtual environment is only active within our current terminal. If we
start another terminal and want to use this virtual environment, we'd  have to activate
it there as well. Also, if we were to close the terminal, the activation of this
environment (not the environment itself) will be forgotten. When we want to use this
virtual environment we have to remember to start it using the `source venv/bin/activate`
command above from within `se-day1/code` directory each time we open a new terminal.
Otherwise, by default, we will the using the global Python interpreter and not the
specific environment we have created.

If we wanted to deactivate our virtual environment, and return to the globally available
set of Python packages, we'd use `deactivate` on the command line (although don't do
this now!).

Other languages make use of virtual environments, such as Ruby, JavaScript, and Go -
it's a great way to keep your environments separate and avoid confusion over which
dependencie belong with which project.

### Running the Script from the Command Line

You'll remember that we were originally running the Python interpreter directly from the
command line earlier. From within the same terminal, type:

~~~bash
which python3
~~~

And you should see something like:

~~~text
/home/user/my/project/dir/venv/bin/python3
~~~

Which confirms that we are using the Python 3 interpreter from within our virtual environment at `/home/user/my/project/dir/venv`.

Now let's run our new script using our virtual environment from the command line:

~~~bash
python3 hello_world.py
~~~

~~~text
Hello world!
~~~

So here, we're doing a very similar thing to what VSCode was doing when running our
script: we give the command line the Python interpreter to run (which will use the one
in the environment we created in this case) and our script, which resides in the local
directory.
