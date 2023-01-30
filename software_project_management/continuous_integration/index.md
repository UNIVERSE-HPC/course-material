---
name: Continuous Integration with Github Actions
id: continuous_integration
dependsOn: [
    software_project_management.version_control_with_git
]
files: [
    github_actions.md,
    testing.md,
    code_coverage.md,
    read_the_docs.md,
    wrapping_up.md
]
---

## What is Continuous Integration?

The automated testing we've done so far only takes into account the state of the repository we have on our own machines.
In a software project involving multiple developers working and pushing changes to a repository, it would be great to know holistically how all these changes are affecting our codebase, without everyone having to pull down all the changes and test them.
If we also take into account the testing required on different target user platforms for our software (e.g. different versions of Python, different operating systems, different compilers, ...) and the changes being made to many repository branches, the effort required to conduct testing at this scale can quickly become intractable for a research project to sustain.

Continuous Integration (CI) aims to reduce this burden by further automation, and automation - wherever possible - helps us to reduce errors and makes predictable processes more efficient.
The idea is that when a new change is committed to a repository, CI clones the repository, builds it if necessary, and performs certain tasks such as running tests, performing static analysis, or building documentation.
Once complete, it presents a report to let you see what happened.

There are many CI infrastructures and services, free and paid for, and subject to change as they evolve their features.
This afternoon you will be using GitHub Actions, which unsurprisingly is available as part of GitHub.
There are other free options, for instance [Travis CI](https://travis-ci.com/) and [AppVeyor](https://www.appveyor.com/).
All three of these make use of common features across many CI implementations, and you are certainly advised to look at the options to see some of the commonalities and differences in how features are typically provided.


## This afternoon

This will be a hands-on session where you will all set up CI for a small Python project, and see some of its benefits in action.
Each section below has a suggested start time, a brief description, a YouTube video lecture, and a list of tasks to perform after watching the video.

Don't worry if you don't keep to the suggested start times.
If you get stuck on something, please reach out to the demonstrators.
If you want to explore a particular aspect more fully, that's fine - these resources will remain online for the foreseeable future.
If you get through everything quickly and have time left at the end of the day, I would suggest that you either look at performing some of the same integrations using a different CI service, or that you investigate some of the GitHub Actions features in more depth.

