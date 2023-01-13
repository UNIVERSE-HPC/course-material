## Git

Git works by tracking changes to files.
It works best on text-based files, where changes to individual lines can be
kept or discarded as necessary.

![XKCD 1597. Title text: If that doesn't fix it, Git.txt contains the phone number of a friend of mine who understands Git. Just wait through a few minutes of 'It's really pretty simple, just think of branches as...' and eventually you'll learn the commands that will fix everything.](https://imgs.xkcd.com/comics/git.png){alt='XKCD comic with text: Person 1 - this is Git. It tracks collaborative work on projects through a beautiful distributed graph theory tree model. Person 2 - Cool. How do we use it? Person 1 - No idea. Just memorize these shell commands and type them to sync up. If you get errors, save your work elsewhere, delete the projects, and download a fresh copy.'}

> ### Quiz
>
> Let's start with [a little quiz](https://www.theserverside.com/quiz/Prove-your-DVCS-IQ-with-our-Git-quiz-for-beginners).
>
> There are [lots](https://www.w3schools.com/quiztest/quiztest.asp?qtest=GIT) and
> [lots](https://www.w3docs.com/quiz/git) of alternatives available.
{: .callout}


### Questions

> ### What is a text-based file?
>
> > A text-based file is one in which the contents are encoded in a text format.
> > These kinds of files include source code, markdown, XML, etc.
> > Non-text-based files are referred to as 'binary files', and these are not
> > decodable as text. Compiled code is in a binary file format, as are most images
> > and many documents.
> >
> > Remember that 'text-based' and 'binary' refer to how a file is stored on disk,
> > not its content. An SVG file is a text-based file that produces an image, while
> > a .docx file is a binary file that contains a text document!
> {: .solution}
>
> ### What is a *branch*?
>
> > A branch is a stand-alone copy of the contents of a repository.
> > It consists of a series of *commits* that represent the changes the contents
> > have undergone.
> >
> > You may be used to working alone where you simply make all your commits to
> > the `main` branch of your repository. This way of working is fine for solo
> > projects (if a bit messy), but it is not easy to collaborate this way, or
> > to maintain development versions of code that address different issues.
> >
> > For complex or collaborative projects, branches are the way.
> {: .solution}
>
> ### Why use *branch*es?
>
> > Changes made on a branch are isolated from the rest of the repository,
> > meaning we can freely explore and build without messing up the rest of the code.
> > When debugging something, especially something that cannot be debugged locally,
> > using a branch can help to avoid a long commit history with minor tweaks.
> >
> > A branch should represent a single useful contribution to the development
> > of the repository contents. It can vary in size from addressing a single bug
> > through to adding an entirely new version of an operating system, but it
> > should always be a coherent group of changes.
> {: .solution}
>
> ### What is a *stage* or *staging area*?
>
> > Staging refers to the changes that have been made to a repository's contents
> > that are not yet bundled into a *commit*.
> {: .solution}
>
> ### What is a *commit*?
>
> > A commit packages *staged* changes into a single unit of change. Each commit
> > has a message that describes briefly what changes it contains.
> >
> > Ideally, a commit should address a single thing, it should completely resolve
> > that thing, and it should do nothing else.
> {: .solution}
{: .callout}

## *Merge* and *rebase*

Once you've got a branch that packages together one or more commits that
constitute a useful addition to the software you are writing, you need to
copy those changes into the `main` branch.

This is usually done by merging, a process that takes all the commits made to
the branch you are merging and adds them on to the end of the target branch.

This is where it's quite common to see merge conflicts arise because it is not
always clear how to unify changes to a file if the branch you are merging into
has changed since you started your own branch.
This problem is often best addressed by *rebasing* your branch onto the latest
version of the target branch.

A *rebase* attempts to add any commits on your branch on to the end of the
target branch, making your branch as if you had started with the target branch
and then made all your changes.
