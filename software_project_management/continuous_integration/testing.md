---
name: Testing
dependsOn: [
  software_project_management.continuous_integration.github_actions
]
tags: [github]
---

To ensure good portability of software for all potential users, you should ideally be 
testing on Windows, Linux and macOS.
You can do that using GitHub Actions.

Information about the operating systems that GitHub Actions supports can be found [in their documentation](https://docs.github.com/en/free-pro-team@latest/actions/reference/specifications-for-github-hosted-runners#supported-runners-and-hardware-resources).
At the time of writing, you can access virtual machines running Windows Server 2019, Ubuntu 20.04, Ubuntu 18.04, Ubuntu 16.04, and macOS Catalina 10.15.

While not covered in this course, you can also use GitHub Actions with [Docker](https://www.docker.com/), allowing you to test on many other environments than the defaults.
You can also add [self hosted runners](https://docs.github.com/en/free-pro-team@latest/actions/hosting-your-own-runners/adding-self-hosted-runners) if you need to run your CI on specific hardware that you own, or if you need to use software (such as Matlab) that you have local licenses for.

<iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/eIf1KxNpn68" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

1. Remove the basic workflow file.  You don't need that any more.
1. Add a status badge to your repository's `README.md` file.
1. Create a new workflow file to test your repository on Windows, Linux and macOS. Use a build matrix with the following:
```yml
os: [windows-latest, macos-latest, ubuntu-latest]
```
1. Add a status badge for your new workflow.
1. \[optional, after completing exercises\] set up a similar workflow using [Travis CI](https://travis-ci.com/).