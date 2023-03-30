
---
name: Issue Management
dependsOn: [
  software_project_management.version_control_with_git.refresher
]
tags: [git]

---

## Introduction

Developing software is a project and, like most projects, it consists of
multiple tasks. Keeping track of identified issues with the software, the list
of tasks the team has to do, progress on each, prioritising tasks for future
development, planning sprints and releases, etc., can quickly become a
non-trivial task in itself.  Without a good team project management process and
framework, it can be hard to keep track of what’s done, or what needs doing, and
particularly difficult to convey that to others in the team or share the
responsibilities.

## Using GitHub to Manage Issues With Software

As a piece of software is used, bugs and other issues will inevitably come to
light - nothing is perfect!  If you work on your code with collaborators, or
have non-developer users, it can be helpful to have a single shared record of
all the problems people have found with the code, not only to keep track of them
for you to work on later, but to avoid people emailing you to report a bug that
you already know about!

GitHub provides **Issues** - a framework for managing bug reports, feature requests, and lists of future work.

Go back to the home page for your `python-intermediate-inflammation` repository in GitHub, and click on the **Issue** tab.
You should see a page listing the open issues on your repository - currently there should be none.

![List of project issues in GitHub](../fig/github-issue-list.png){: .image-with-shadow width="1000px"}

Let's go through the process of creating a new issue. Start by clicking the `New issue` button.

![Creating a new issue in GitHub](../fig/github-new-issue.png){: .image-with-shadow width="1000px"}

When you create an issue, you can add a range of details to them. They can be *assigned to a specific developer* for example - this can be a helpful way to know who, if anyone, is currently working to fix the issue, or a way to assign 
responsibility to someone to deal with it.

They can also be assigned a *label*. The labels available for issues can be customised, and given a colour, allowing you to see at a glance the state of your code's issues. The [default labels available in GitHub](https://docs.github.com/en/issues/using-labels-and-milestones-to-track-work/managing-labels) include:

- `bug` - indicates an unexpected problem or unintended behavior
- `documentation` - indicates a need for improvements or additions to documentation
- `duplicate` - indicates similar or already reported issues, pull requests, or discussions
- `enhancement` - indicates new feature requests, or if they are created by a developer, indicate planned new features
- `good first issue` - indicates a good issue for first-time contributors
- `help wanted` - indicates that a maintainer wants help on an issue or pull request
- `invalid` - indicates that an issue, pull request, or discussion is no longer relevant
- `question` - indicates that an issue, pull request, or discussion needs more information
- `wontfix` - indicates that work won't continue on an issue, pull request, or discussion

You can also create your own custom labels to help with classifying issues. There are no rules really about naming the labels - use whatever makes sense for your project. Some conventional custom labels include: `status:in progress` (to indicate that someone started working on the issue), `status:blocked` (to indicate that the progress on addressing issue is blocked by another issue or activity), etc.

As well as highlighting problems, the `bug` label can make code much more usable by allowing users to find out if anyone has had the same problem before, and also how to fix (or work around) it on their end. Enabling users to solve their own problems can save you a lot of time. In general, a good bug report should contain only one bug, specific details of the environment in which the issue appeared (e.g. operating system or browser, version of the software and its dependencies), and sufficiently clear and concise steps that allow a developer to reproduce the bug themselves. They should also be clear on what the bug reporter considers factual ("I did this and this happened") and speculation ("I think it was caused by this"). If an error report was generated from the software itself, it's a very good idea to include that in the issue.

The `enhancement` label is a great way to communicate your future priorities to your collaborators but also to yourself - it’s far too easy to leave a software project for a few months to work on something else, only to come back and forget the improvements you were going to make. If you have other users for your code, they can use the label to request new features, or changes to the way the code operates. It’s generally worth paying attention to these suggestions, especially if you spend more time developing than running the code. It can be very easy to end up with quirky behaviour because of off-the-cuff choices during development. Extra pairs of eyes can point out ways the code can be made more accessible - the easier the code is to use, the more widely it will be adopted and the greater impact it will have.

One interesting label is `wontfix`, which indicates that an issue simply won't be worked on for whatever reason. Maybe the bug it reports is outside of the use case of the software, or the feature it requests simply isn't a priority. This can make it clear 
you've thought about an issue and dismissed it.

:::callout
## Locking and Pinning Issues
The **Lock conversation** and **Pin issue** buttons are both available from individual issue pages. 
Locking conversations allows you to block future comments on the issue, e.g. if the conversation around the issue 
is not constructive or violates your team's code of conduct. Pinning issues allows you to pin up to three 
issues to the top of the issues page, e.g. to emphasise their importance.
:::

:::callout
## Manage Issues With Your Code Openly
Having open, publicly-visible lists of the the limitations and problems with
your code is incredibly helpful. Even if some issues end up languishing unfixed
for years, letting users know about them can save them a huge amount of work
attempting to fix what turns out to be an unfixable problem on their end. It can
also help you see at a glance what state your code is in, making it easier to
prioritise future work!
:::


::::challenge{id="first-issue", title="Our First Issue!"}

Individually, with a critical eye, think of an aspect of the code you have developed so far that needs improvement. 
It could be a bug, for example, or a documentation issue with your README, a missing LICENSE file, or an enhancement. 
In GitHub, enter the details of the issue and select `Submit new issue`. Add a label to your issue, if appropriate.

:::solution

 For example, "Add a licence file" could be a good first issue, with a label `documentation`. 

:::
::::

### Issue (and Pull Request) Templates

GitHub also allows you to set up issue and pull request templates for your
software project.  Such templates provide a structure for the issue/pull request
descriptions, and/or prompt issue reporters and collaborators to fill in answers
to pre-set questions. They can help contributors raise issues or submit pull
requests in a way that is clear, helpful and provides enough information for
maintainers to act upon (without going back and forth to extract it). GitHub
provides a range of default templates, but you can also [write your
own](https://docs.github.com/en/communities/using-templates-to-encourage-useful-issues-and-pull-requests/configuring-issue-templates-for-your-repository).

## Using GitHub's Notifications & Referencing System to Communicate

GitHub implements a comprehensive [notifications system](https://docs.github.com/en/account-and-profile/managing-subscriptions-and-notifications-on-github/setting-up-notifications/configuring-notifications) 
to keep the team up-to-date with activities in your code repository and notify you when something happens or changes 
in your software project. You can choose whether to watch or unwatch an individual repository,
or can choose to only be notified of certain event types such as updates to issues, pull requests, direct mentions, 
etc. GitHub also provides an additional useful notification feature for collaborative work - **Mentions**. 
In addition to referencing team members (which will result in an appropriate notification), GitHub allows us 
to reference issues, pull requests and comments from one another - providing a useful way of connecting things 
and conversations in your project.

### Referencing Team Members Using Mentions

The mention system notifies team members when somebody else references them in
an issue, comment or pull request - you can use this to notify people when you
want to check a detail with them, or let them know something has been fixed or
changed (much easier than writing out all the same information again in an
email).  

You can use the mention system to link to/notify an individual GitHub account or
a whole team for notifying multiple people. Typing <kbd>@</kbd> in GitHub will
bring up a list of all accounts and teams linked to the repository that can be
"mentioned". People will then receive notifications based on their preferred
notification methods - e.g. via email or GitHub's User Interface.

### Referencing Issues, Pull Requests and Comments 

GitHub also lets you mention/reference one issue or pull request from another
(and people "watching" these will be notified of any such updates). Whilst
writing the description of an issue, or commenting on one, if you type
<kbd>#</kbd> you should see a list of the issues and pull requests on the
repository.  They are coloured green if they're open, or white if they're
closed. Continue typing the issue number, and the list will narrow down, then
you can hit <kbd>Return</kbd> to select the entry and link the two.  For
example, if you realise that several of your bugs have common roots, or that one
enhancement can't be implemented before you've finished another, you can use the
mention system to indicate the depending issue(s). This is a simple way to add
much more information to your issues.

While not strictly notifying anyone, GitHub lets you also reference individual comments and commits. If you click the
`...` button on a comment, from the drop down list you can select to `Copy link` (which is a URL that points to that
comment that can be pasted elsewhere) or to `Reference [a comment] in a new issue` (which opens a new issue and references
the comment by its URL). Within a text box for comments, issue and pull request descriptions, you can reference
a commit by pasting its long, unique identifier (or its first few digits which uniquely identify it)
and GitHub will render it nicely using the identifier's short form and link to the commit in question.

![Referencing comments and commits in GitHub](../fig/github-reference-comments-commits.png){: .image-with-shadow width="700px"}

:::challenge{id="first-mention", title="Our First Mention/Reference!"}

Add a mention to one of your team members using the `@` notation 
in a comment within an issue or a pull request in your repository - e.g. to 
ask them a question or a clarification on something or to do some additional work. 

Alternatively, add another issue to your repository and reference the issue you created in the previous exercise using the 
`#` notation.

:::

:::callout
## You Are Also a User of Your Code

This section focuses a lot on how issues and mentions can help communicate the
current state of the code to others and document what conversations were held
around particular issues. As a sole developer, and possibly also the only user
of the code, you might be tempted to not bother with recording issues, comments
and new features as you don't need to communicate the information to anyone
else.

Unfortunately, human memory isn't infallible! After spending six months on a
different topic, it's inevitable you'll forget some of the plans you had and
problems you faced. Not documenting these things can lead to you having to
re-learn things you already put the effort into discovering before. Also, if
others are brought on to the project at a later date, the software's existing
issues and potential new features are already in place to build upon.  
:::
