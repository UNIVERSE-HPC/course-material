## Contributing to a GitHub project

This episode will walk you through your first contribution to a GitHub
repository. Before contributing, the first thing to find out is what to
contribute on! Usually projects will have a README.md file
([example](https://github.com/OxfordRSE/oxrse_unit_conv)) which tells you
what the project is about. Often you will also find files like a [Code of
Conduct](https://github.com/OxfordRSE/oxrse_unit_conv/blob/main/.github/CODE_OF_CONDUCT.md)
which codifies expectations of the community with respect to communication
and behaviour. There's often a CONTRIBUTING.md file which has instructions
for first time contributions. Some of these files may be missing, or in
many cases, they are all within the main README.md file.

## Cloning the repository

Sometimes you will have a clear understanding of what you want
to contribute to the project, at other times, you may be willing
to contribute, but not sure where to start. Either way, the first
thing to do is to clone the repository.

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: callout

If you have already cloned the repository, there is no need to
clone it again! Remember that Git keeps a record of where the
remote repository is, so doing a `git pull` should update
your repository to the latest changes, as long as you do not
have any conflicts with the copy of files that you have.

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Once you have cloned, you should look through the README.md
file for build or installation instructions. Once you have
successfully built and/or installed the project, you are
ready to start contributing!

## GitHub Issues

Now that the project is working, you can go through the GitHub
Issues page for the repository that you will be contributing to.
Usually projects will discuss bugs and report problems to the
issue tracker. This is also usually the place to post feature
requests (but read the README, sometimes projects prefer feature
requests to be sent to a mailing list).

If you have found a issue that you think hasn't been reported
on the issue tracker, you should open an issue. This by itself
is a valuable contribution, and usually there is no expectation
that the person reporting the issue also has to solve it!

Alternatively, you may have found an issue that you are eager
to get started on. If no one is working on the issue, then you
can start digging into it.

## Creating a branch

At this point, you have identified an issue, verified no one
is working on it. The first step is to check that the unit testing
infrastructure of the project is working, if one exists. Modern
software development best practices incorporate techniques like
continuous testing and integration, which ensures bugs are caught
quickly.

Once that is working, you can create a branch to start working on the
issues. There is usually no naming restrictions on branches, but if you are
working to fix an issue, we usually start the branch name with the issue
number. So if there is a issue 12 which wants to add a unit `stones` to our
repository, then the branch name could be `unit/stones`.

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: callout

It is always a good idea, when creating a branch, to branch off the latest
working copy of the main branch on GitHub. To do this, use the Pull command
in your Git client, or on the command line, use `git pull`

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

If there is a testing infrastructure, it is important to test your code as
you go. This will speed up development and catch bugs. Often you will be
working on new code; in that case, it is best practice to add a test
corresponding to the new code.

While you are changing the code, it is a good idea to commit frequently.
Ideally each commit is a well-defined unit of change, which could be across
multiple files. Writing a [good commit
message](https://cbea.ms/git-commit/) also helps you and others to keep
track of what changed.

## Creating a pull request

You have successfully fixed the issue, or created a new feature. GitHub
uses [pull requests](https://docs.github.com/en/pull-requests) to manage
and merge contributions to the *main* branch. First, you need to push your
changes to the the upstream remote repository in GitHub. You can do this
using your preferred Git client, or from the command line

```shell
git push --set-upstream origin <branchname>
```

where *branchname* is the branch on the remote you want to push to, usually
the same as your local branch name. If that branch doesn't exist on the
remote repository, it will be created. If it _does_ exist, you probably don't
want to push to it!

Once you have done this, navigate to the repository. GitHub should show
a banner at the top indicating recently pushed branches, with a button
to *Compare and pull request*. Clicking that will open GitHub's pull request
form, where you can describe your changes.

## Handling merge conflicts

If your changes **conflict** with the code already in the *main* branch of
the repository, then GitHub will not allow the pull request to be merged
and instead display an alert showing the files where the conflict has
happened. For certain types of merge conflicts, [GitHub has a *Resolve
conflicts*
button](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/addressing-merge-conflicts/resolving-a-merge-conflict-on-github)
that allows you to resolve the conflict using the web UI.

A merge conflict happens when the same line has been updated differently by
more than one author. As an example, say there is a text file with the
following line, a maintainer Alice and contributor Bob working on the code.

1. The text file initially starts like this

   ```
   Roses are red, violets are blue
   ```

2. Alice then adds a line in a commit and pushes to the `main` branch.

   ```
   Roses are red, violets are blue
   Venus is hot, and Mars is red
   ```

3. Bob starts work on a different branch `more-flowers` and adds a
   different line.

   ```
   Roses are red, violets are blue
   Sunflowers are yellow, and jasmines are white
   ```

4. Bob then wants to merge `more-flowers` into `main` and
   creates a pull request. GitHub then shows a conflict. Clicking
   on *Resolve conflicts* opens a web editor showing the line
   where the conflict happened

   ```
   Roses are red, violets are blue
   <<<<<<< main
   Venus is hot, and Mars is red
   =======
   Sunflowers are yellow, and jasmines are white
   >>>>>>> more-flowers
   ```

5. The markers show the two options for the set of lines, one
   corresponding to the branch that is being merged into (usually `main`)
   and the other corresponding to the branch being merged (the pull request branch).
   Bob will now need to choose which version to keep, or alternatively
   can choose some other text altogether to resolve the conflict.

## Code Review

The next step in the contribution process is usually a code review from
another contributor or the maintainer of the project. You can request a
review in the pull request form. Code reviews usually lead to three
possible outcomes, accepted (and merged), changes requested, or rejected.
The reviewer may also comment on the code without taking a decision. Once
you have addressed the changes, you can re-request a review.

Your contribution is then merged into the project. Congratulations!
