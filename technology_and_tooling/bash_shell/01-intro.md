---
name: Introducing the Shell
dependsOn: []
tags: [bash]
learningOutcomes:
  - Describe the benefits of using the shell over other styles of interface.
attribution:
  - citation: >
      This material was originally taken from training materials developed by the
      University of Southampton Research Software Group, which are based on
      the Software Carpentries course "Version Control with Git".
    url: https://github.com/Southampton-RSG-Training/shell-novice/
    image: https://southampton-rsg-training.github.io/shell-novice/assets/img/home-logo.png
    license: CC-BY-4.0
---

The Bash shell a text-based program that interactively allows you to run other programs.

You'll be familiar with the graphical way of dealing with computers, like the interfaces that Windows and Macs give you - sometimes called GUIs (graphical user interfaces).
You run an application, it gives you windows and buttons and menus to interact with to access its functions and achieve a result.
The Bash shell also gives you a means to access the functionality provided by your computer and other programs, but it does so in a different way.
It allows you to type commands into your computer to get results instead, and when the command has finished running, it prints out the results.
And then the shell allows you to type in another command…
And so on.

:::callout

## Analogies

Imagine the shell a little like working with a voice assistant like Siri or Alexa.
You ask your computer questions, and your computer responds with an answer.
:::

The shell is called _the shell_ because it encloses the machine's **operating system** - which could be Windows, Mac OS X, or Linux - giving you a wrapper-like interface to interact with it. Another, more general way, of referring to the shell is the **command line**, since it provides an interface into which you type commands line-by-line.

## Why use it?

So why use the Bash shell?

- **Capturing a process:** Being able to capture how programs are run and in what order in a Bash script - and essentially automating how we run that process - is invaluable.
  It's really helpful with making your pipelines reproducible: once you've defined this process in a script, you can re-run it whenever you want.
  This is both helpful for others to achieve the same results, but also for yourself
  perhaps six months from now, when it would be otherwise difficult to remember exactly what you did.
  What you are effectively doing is building a narrative - telling a story in recorded, programmatic form - of how you generated your research results.

- **Repetition:** Bash is great at repeating the same commands many times.
  This could be renaming a hundred files in a certain way, or something more complex, such as running a data analysis program over many input data files,
  or running a program to generate a chart for every one of those output data files produced by that program.

- **Availability:** Bash is available on different types of machines.
  You can already use the Bash shell on computers like Macs and those that run Linux, where it's already installed, but you can also install and use it on Windows.

- **Using other computational resources:** if you need to use another computational resource, such as a supercomputer to run your programs even faster, they almost exclusively use the shell.

Now on the flip side, it does have a steeper learning curve generally than using graphical programs. Applications and programs also need to work on the command line to be able to take advantage of it. But knowing just a little can be very useful, and in your careers you will very likely come across quite a few programs that have command line interfaces so it's helpful to have some experience with it.
