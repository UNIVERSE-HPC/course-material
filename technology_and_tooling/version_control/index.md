---
id: version_control_git
name: Version Control
dependsOn: [
    technology_and_tooling.bash_shell
]
files: [
  01-background.md,
  02-setup.md,
  03-create.md,
  04-changes.md,
  05-history.md,
  06-remote.md,
  07-branches.md,
  08-ignore.md
]
learningOutcomes:
    - Learn how version control systems work.
    - Configure Git and Github.
    - Create or clone repositories.
    - Learn the modify-add-commit cycle.
    - Compare files with previous versions.
    - Manage branches and resolve merge conflicts.
    - Exclude certain files from version control.
attribution:
  - citation: >
        This material was originally taken from training materials developed by the
        University of Southampton Research Software Group, which are based on
        the [software carpentries course "Version Control with Git".
    url: https://github.com/Southampton-RSG-Training/git-novice/
    image: https://swcarpentry.github.io/git-novice/assets/img/home-logo.png
    license: CC-BY-4.0
summary: |
    This course introduces the basics of version control using the Git version control system.
    We will learn how to setup Git, and use it to track changes in our code.

---

Version control (or VC for short ) can also be called revision control or source
control. The simplest description is that VC is a tool that tracks changes to
files. It’s like turning on “Track Changes” in Word or Google Docs, but for code.
So why would you want to do that?

Nothing that is committed to version control is ever lost, unless you work
really, really hard at it. Since all old versions of files are saved, it’s
always possible to go back in time to see exactly who wrote what on a particular
day, or what version of a program was used to generate a particular set of
results.

As we have this record of who made what changes when, we know who to ask if we
have questions later on, and, if needed, revert to a previous version, much like
the “undo” feature in an editor.

When several people collaborate in the same project, it’s possible to
accidentally overlook or overwrite someone’s changes. The version control system
automatically notifies users whenever there’s a conflict between one person’s
work and another’s.

Teams are not the only ones to benefit from version control: lone researchers
can benefit immensely. Keeping a record of what was changed, when, and why is
extremely useful for all researchers if they ever need to come back to the
project later on (e.g., a year later, when memory has faded).

Version control is the lab notebook of the digital world: it’s what
professionals use to keep track of what they’ve done and to collaborate with
other people. Every large software development project relies on it, and most
programmers use it for their small jobs as well. And it isn’t just for software:
books, papers, small data sets, and anything that changes over time or needs to
be shared can and should be stored in a version control system.
