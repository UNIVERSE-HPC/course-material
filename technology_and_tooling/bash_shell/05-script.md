---
name: Shell Scripts
dependsOn: [
    technology_and_tooling.bash_shell.04-pipefilter
]
tags: [bash]
attribution:
- citation: >
      This material was originally taken from training materials developed by the
      University of Southampton Research Software Group, which are based on
      the Software Carpentries course "Version Control with Git".
  url: https://github.com/Southampton-RSG-Training/shell-novice/
  image: https://southampton-rsg-training.github.io/shell-novice/assets/img/home-logo.png
  license: CC-BY-4.0
---

We are finally ready to see what makes the shell such a powerful programming environment.
We are going to take the commands we repeat frequently and save them in files
so that we can re-run all those operations again later by typing a single command.
For historical reasons,
a bunch of commands saved in a file is usually called a **shell script**,
but make no mistake:
these are actually small programs.

### Our first shell script

Let's start by going back to `data` and putting some commands into a new file called `middle.sh` using an editor like `nano`:

~~~bash
cd ~/shell-novice/data
nano middle.sh
~~~

So why the .sh extension to the filename? Adding `.sh` is the convention to show that this is a Bash shell script.

Enter the following line into our new file:

~~~bash
head -15 sc_climate_data_1000.csv | tail -5
~~~

Then save it and exit `nano` (using `Control-O` to save it and then `Control-X` to exit `nano`).

This pipe selects lines 11-15 of the file `sc_climate_data_1000.csv`. It selects the first 15
lines of that file using `head`, then passes that to `tail` to show us only the last 5 lines - hence lines 11-15.
Remember, we are *not* running it as a command just yet:
we are putting the commands in a file.

Once we have saved the file,
we can ask the shell to execute the commands it contains.
Our shell is called `bash`, so we run the following command:

~~~bash
bash middle.sh
~~~

~~~text
299196.8188,972890.0521,48.07,61.41,0.78
324196.8188,972890.0521,48.20,-9999.00,0.72
274196.8188,968890.0521,47.86,60.94,0.83
275196.8188,968890.0521,47.86,61.27,0.83
248196.8188,961890.0521,46.22,58.98,1.43
~~~

Sure enough,
our script's output is exactly what we would get if we ran that pipeline directly.

:::callout

## Text vs. Whatever

We usually call programs like Microsoft Word or LibreOffice Writer "text
editors", but we need to be a bit more careful when it comes to
programming. By default, Microsoft Word uses `.docx` files to store not
only text, but also formatting information about fonts, headings, and so
on. This extra information isn't stored as characters, and doesn't mean
anything to tools like `head`: they expect input files to contain
nothing but the letters, digits, and punctuation on a standard computer
keyboard. When editing programs, therefore, you must either use a plain
text editor, or be careful to save files as plain text.
:::

### Enabling our script to run on any file

What if we want to select lines from an arbitrary file?
We could edit `middle.sh` each time to change the filename,
but that would probably take longer than just retyping the command.
Instead,
let's edit `middle.sh` and replace `sc_climate_data_1000.csv` with a special variable called `$1`:

~~~bash
nano middle.sh
~~~

~~~text
head -15 "$1" | tail -5
~~~

Inside a shell script,
`$1` means the first filename (or other argument) passed to the script on the command line.
We can now run our script like this:

~~~bash
bash middle.sh sc_climate_data_1000.csv
~~~

~~~text
299196.8188,972890.0521,48.07,61.41,0.78
324196.8188,972890.0521,48.20,-9999.00,0.72
274196.8188,968890.0521,47.86,60.94,0.83
275196.8188,968890.0521,47.86,61.27,0.83
248196.8188,961890.0521,46.22,58.98,1.43
~~~

or on a different file like this (our full data set!):

~~~bash
bash middle.sh sc_climate_data.csv
~~~

~~~text
299196.8188,972890.0521,48.07,61.41,0.78
324196.8188,972890.0521,48.20,-9999.00,0.72
274196.8188,968890.0521,47.86,60.94,0.83
275196.8188,968890.0521,47.86,61.27,0.83
248196.8188,961890.0521,46.22,58.98,1.43
~~~

Note the output is the same, since our full data set contains the same first 1000 lines as `sc_climate_data_1000.csv`.

:::callout

## Double-Quotes Around Arguments

We put the `$1` inside of double-quotes in case the filename happens to contain any spaces.
The shell uses whitespace to separate arguments,
so we have to be careful when using arguments that might have whitespace in them.
If we left out these quotes, and `$1` expanded to a filename like
`climate data.csv`,
the command in the script would effectively be:

~~~bash
head -15 climate data.csv | tail -5
~~~

This would call `head` on two separate files, `climate` and `data.csv`,
which is probably not what we intended.
:::

## Exercises

::::challenge{id=variables-shell-scripts title="Variables in Shell Scripts"}
In the `test_directory/molecules` directory, you have a shell script called `script.sh` containing the
following commands:

~~~bash
head $2 $1
tail -n $3 $1
~~~

The shell allows us to access arguments other than just the first. Here, we are using `$2` and `$3`
to obtain and use the second and third arguments passed to the script (where arguments are separated by spaces, as with any other commands).

Note that here, we use the explicit `-n` flag to pass the number of lines to `tail` that we want to extract,
since we're passing in multiple `.pdb` files. Otherwise, `tail` can give us an error about incorrect options on
certain machines if we don't.

While you are in the molecules directory, you type the following command:

~~~bash
bash script.sh '*.pdb' -1 -1
~~~

Which of the following outputs would you expect to see?

1. All of the lines between the first and the last lines of each file ending in `*.pdb`
   in the molecules directory
2. The first and the last line of each file ending in `*.pdb` in the molecules directory
3. The first and the last line of each file in the molecules directory
4. An error because of the quotes around `*.pdb`

:::solution
The answer is **2**. The quotes around the wildcard `'*.pdb'` mean it isn't expanded when we call the script - but it will get expanded *inside* the script. There, it gets expanded to match every file in the directory that ends in `*.pdb`, and effectively the script calls:

~~~bash
head -1 *.pdb
tail -n -1 *.pdb*
~~~

This prints out the first line (`head -1`) of each `.pdb` file, and then the last line of each `.pdb` file.

If we'd called the script as:

~~~bash
bash script.sh *.pdb -1 -1
~~~

Then it wouldn't work as the wildcard would've expanded before the script started and we'd have effectively run it as:

~~~bash
bash script.sh cubane.pdb ethane.pdb methane.pdb octane.pdb pentane.pdb propane.pdb -1 -1
~~~

This would have caused an error, as we expect the second and third arguments to be numbers for `head` and `tail`!
:::
::::
