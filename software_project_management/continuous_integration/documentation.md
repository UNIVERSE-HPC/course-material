---
name: Documentation
dependsOn: [software_project_management.continuous_integration.code_coverage]
tags: [sphinx, readthedocs]
learningOutcomes:
  - List benefits of having good documentation for software.
  - Describe the key features of the Sphinx and Read the Docs documentation and hosting tools.
  - Use Sphinx to generate documentation for a software project.
attribution:
  - citation: This material has been adapted from the "Software Engineering" module of the SABS R³ Center for Doctoral Training.
    url: https://www.sabsr3.ox.ac.uk
    image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

Writing documentation for software is crucial.

1. **Improves Code Understanding**: Good documentation gives a clear understanding of the flow, architecture, and functionalities of the software, reducing the time it takes for new team members to understand how things work.

1. **Easier Maintenance**: Documentation is essential for maintaining software, particularly if the original engineers are no longer involved in the project. Without documentation, future developers may not understand why certain decisions were made.

1. **Boosts Productivity**: Developers spend a significant amount of time understanding existing code. If software is well documented, developers can understand it quicker and spend more time writing new code or improving existing features.

1. **Reduces Dependency**: Good documentation reduces the dependency on particular team members who have expert knowledge of the software. It enables any developer to understand and contribute to the software, irrespective of their prior involvement in the project.

1. **Enhances Software Quality**: Writing documentation forces developers to think through their design before implementing it, often leading to better design decisions. It can also serve as a form of review to catch potential problems before the software is built.

1. **Facilitates Code Reusability**: With adequate documentation, code functions, libraries, and modules can be reused easily in different projects, saving time and effort.

1. **Useful for Users and Stakeholders**: Besides developers, other stakeholders such as project managers, testers, end-users, and technical writers benefit from good documentation. It helps in using the software, testing it, and writing manuals or user guides.

1. **Compliance and Auditing**: In some industries, documentation is a legal requirement for audits and regulatory compliance. It helps demonstrate that the software meets certain standards or regulations.

1. **Supports Debugging**: When problems arise, good documentation can help developers understand the system better and assist in debugging.

1. **Encourages Collaboration**: Documentation encourages team collaboration by ensuring everyone understands the project's direction and status.

1. **Preserves Information**: Documentation helps to preserve the rationale and thought process behind the implementation. This is especially important when key decisions are made that will impact the software's future direction.

We will be using [Sphinx](https://www.sphinx-doc.org/en/master/) and [Read the Docs](https://readthedocs.org/) to create and deploy documentation pages for our repository.

## Sphinx

Sphinx is a powerful and flexible open-source documentation generation tool primarily used for Python, but it can be used for other programming languages as well.

Here are some key features:

1. **Extensible Output Formats**: Sphinx can generate several output formats, including HTML, LaTeX (for printable PDF versions), manual pages, plain text, and more.

1. **Structured and Semantic Documentation**: Sphinx uses reStructuredText as its markup language, which allows for structured and semantic documentation.

1. **Cross-referencing Code**: Sphinx supports automatic generation of API documentation from Python modules, and allows linking to specific function definitions, classes, methods, and more from other parts of the documentation.

1. **Indexing and Searching**: Sphinx provides robust indexing that is crucial for larger projects. It supports glossaries and can generate search pages for HTML output.

1. **Theming Support**: Sphinx supports themes for its HTML output, allowing documentation to match the aesthetic and branding of a project or organization.

## Read the Docs

Read the Docs is a free and open-source platform for hosting software documentation.
It's popular among developers as it provides a user-friendly and convenient way to publish, maintain, and view software documentation online.

Here are some of its key features:

1. **Versioning**: Read the Docs supports versioning of your documentation, similar to the way your source code is versioned. This means you can have separate documentation for different versions of your software and users can easily switch between them.

1. **Automatic Building**: When you push code to your repository, Read the Docs automatically builds and updates your online documentation. This reduces the manual effort involved in keeping your documentation up-to-date.

1. **Integration with Source Control Systems**: Read the Docs integrates well with popular source control systems like GitHub, GitLab, and Bitbucket. It simplifies the process of building documentation directly from these platforms.

1. **Support for Sphinx and MkDocs**: Read the Docs is compatible with popular documentation generators like Sphinx (using reStructuredText) and MkDocs (using Markdown), offering a good deal of flexibility in how you generate your documentation.

1. **PDF and EPUB Export**: Users can download a PDF or EPUB version of your documentation for offline reading.

## Getting started

From your repository, run:

```bash
pip install -e ."[dev,docs]"
```

to ensure you have all development and documentation dependencies installed.

Next, create a directory at the top level of your project called `docs`.

From the `docs` directory, run

```shell
sphinx-quickstart
```

Use the default values, but fill in a unique project name.

You should now have a number of files added to your repository.
Let's add one more: an empty file called anything you like, in the `docs/_static` directory.
This is just so the directory itself gets added to the repository, and this is needed for the next step.

Add, commit, and push these new files GitHub.

Next, go to [Read the Docs](https://readthedocs.org/).

- Log in with GitHub
- Import a Project, and select your repository
- Follow the instructions, leaving everything as default

You should then see your documentation building!
Wait for it to complete, and then click _View Docs_.
This will take you to the website `https://<project_name>.readthedocs.io/en/latest/`.

::::challenge{id="start-documenting" title="Start documenting"}

Read through the [Sphinx documentation](https://www.sphinx-doc.org/en/master/) and start fleshing out the documentation for your repository.
::::
