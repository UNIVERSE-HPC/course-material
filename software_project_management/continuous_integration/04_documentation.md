---
name: Documentation
dependsOn: [software_project_management.continuous_integration.02_code_coverage]
tags: [sphinx, github]
learningOutcomes:
  - List benefits of having good documentation for software.
  - Use Sphinx to generate API documentation from the docstrings in a project.
  - Publish documentation automatically with GitHub Pages.
  - Explain the difference between Continuous Delivery and Continuous Deployment.
attribution:
  - citation: This material has been adapted from the "Software Engineering" module of the SABS R³ Center for Doctoral Training.
    url: https://www.sabsr3.ox.ac.uk
    image: https://www.sabsr3.ox.ac.uk/sites/default/files/styles/site_logo/public/styles/site_logo/public/sabsr3/site-logo/sabs_r3_cdt_logo_v3_111x109.png
    license: CC-BY-4.0
  - citation: This course material was developed as part of UNIVERSE-HPC, which was funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
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

1. **Compliance and Auditing**: In some industries, documentation is a legal requirement for audits and regulatory compliance. It helps demonstrate that the software meets certain standards or regulations.

1. **Supports Debugging**: When problems arise, good documentation can help developers understand the system better and assist in debugging.

1. **Preserves Information**: Documentation helps to preserve the rationale and thought process behind the implementation. This is especially important when key decisions are made about how something was implemented and why.

We will be using [Sphinx](https://www.sphinx-doc.org/en/master/) to generate the documentation, and [GitHub Pages](https://pages.github.com/) to host it.

## Sphinx

Sphinx is a powerful and flexible open-source documentation generation tool primarily used for Python, but it can be used for other programming languages as well.

Here are some key features:

1. **Extensible Output Formats**: Sphinx can generate several output formats, including HTML, LaTeX (for printable PDF versions), manual pages, plain text, and more.

2. **Cross-referencing Code**: Sphinx supports automatic generation of API documentation from Python modules, and allows linking to specific function definitions, classes, methods, and more from other parts of the documentation.

3. **Indexing and Searching**: Sphinx provides robust indexing that is crucial for larger projects. It supports glossaries and can generate search pages for HTML output.

4. **Theming Support**: Sphinx supports themes for its HTML output, allowing documentation to match the aesthetic and branding of a project or organization.

## Where to put it

Generated HTML has to be served from somewhere.

Most scientific Python projects use [Read the Docs](https://readthedocs.org/), which builds your documentation on its own servers whenever you push, hosts it for free, and gives you two things that are genuinely hard to arrange yourself: documentation versioned alongside your releases, so a user on an old version reads the right page, and preview builds on pull requests.
It is worth knowing, and you will meet it constantly.

We are going to use [GitHub Pages](https://pages.github.com/) instead, for two reasons.

The first is that the build happens where you can watch it.
This course is about seeing your pipeline run, and Read the Docs does the interesting part somewhere else, behind a login on a different site.

The second is that Pages is general and you will find it useful for all sorts of things.
It serves static files out of a repository, and it does not care whether those files are API documentation, a coverage report, a project website, a set of benchmark results, a rendered notebook, or even an [interactive web app](https://ukancestrymap.github.io/).
Once you have built one thing and published it, you will know how to publish anything.

## Setting up Sphinx

Make sure the documentation dependencies are installed:

```bash
pip install -e ".[dev,docs]"
```

Create a directory called `docs` at the top level of your project, and from inside it run:

```bash
sphinx-quickstart
```

Accept the defaults, but give the project a name.

This generates `conf.py`, `index.rst` and a couple of build scripts.
`rst` stands for `restructured text` and is a markup format similar to, but a little more sophisticated than markdown.

### Telling Sphinx about your code

Out of the box, Sphinx will happily build a site containing nothing but the page you just made.
To get it to read your code, you need two extensions.

In `docs/conf.py`, find the `extensions` list and make it:

```python
extensions = ["sphinx.ext.autodoc", "sphinx.ext.napoleon"]
```

`autodoc` imports your package and pulls out the docstrings.
`napoleon` teaches it to read the numpydoc style those docstrings are written in; without it, the parameter and return sections render as an unformatted mess.

While you are in `conf.py`, delete this line:

```python
html_static_path = ['_static']
```

`sphinx-quickstart` creates an empty `docs/_static` directory for custom CSS and images.
Git does not track empty directories, so the moment you commit, the directory does not exist for anyone else, and Sphinx warns about it.
You have no static assets, so the simplest fix is the one above.
Put the line back if you ever add static assets (for instance, a project logo).

### Adding an API reference

Create `docs/reference.rst`:

```rst
API reference
=============

.. automodule:: ci_course.io
   :members:

.. automodule:: ci_course.stats
   :members:

.. automodule:: ci_course.report
   :members:
```

and add `reference` to the `toctree` in `docs/index.rst`.

Build it:

```bash
sphinx-build -W -b html docs docs/_build/html
```

Open `docs/_build/html/index.html` in a browser and look at the API reference.

The `-W` turns warnings into errors.
Documentation warnings are easy to ignore for months, and they are usually telling you that something is not appearing on the page you think it is.

::::challenge{id="find-the-missing-function" title="What is missing?"}

Compare the API reference against `src/ci_course/stats.py`.
If you skipped the previous section, one function is absent.

:::solution

Try briefly removing that docstring, and re-run the `sphinx-build` command above, and see how it looks instead.

Unfortunately, nothing in Sphinx warns you, not even with `-W`.
The page simply does not mention the function, and looks perfectly finished.

:::
::::

## Publishing it

Now automate the build and put the result online.

First, turn GitHub Pages on: on your GitHub repository, go to _Settings_, then _Pages_, and set the source to _GitHub Actions_ rather than a branch.
This is why your repository needed to be public: Pages on a free account requires it.

::::challenge{id="pages-workflow" title="Deploy the documentation"}

Write a workflow, `pages.yml`, that builds the documentation on every push to `main` and publishes it.

You will need three actions you have not used before: `actions/configure-pages`, `actions/upload-pages-artifact` and `actions/deploy-pages`.

You will also need a `permissions` block.
Deployment needs `pages: write` to publish, and `id-token: write` so the runner can prove to GitHub Pages which workflow it is, without you storing a secret anywhere.

:::solution

```yml
name: Documentation

on:
  push:
    branches: ["main"]
  workflow_dispatch:

permissions:
  contents: read
  pages: write
  id-token: write

jobs:
  deploy:
    runs-on: ubuntu-latest
    environment:
      name: github-pages
      url: ${{ steps.deployment.outputs.page_url }}

    steps:
      - uses: actions/checkout@v7

      - name: Set up Python 3.13
        uses: actions/setup-python@v7
        with:
          python-version: "3.13"

      - name: Install dependencies
        run: |
          python -m pip install .[docs]

      - name: Build documentation
        run: |
          sphinx-build -W -b html docs docs/_build/html

      - uses: actions/configure-pages@v6

      - uses: actions/upload-pages-artifact@v5
        with:
          path: docs/_build/html

      - name: Deploy to GitHub Pages
        id: deployment
        uses: actions/deploy-pages@v5
```

:::
::::

Push it, watch it run, and follow the URL in the _deploy_ job summary.
Your documentation is online, and it will now stay current without anyone remembering to update it!

## This is deployment

You pushed a commit, and a change went live on a website, with no human having to remember to do it.

That is **Continuous Deployment**.

The alternative is **Continuous Delivery**: every change that passes is built, tested and made ready to release, and then a person decides whether to release it.
The pipeline is identical right up to the last step. The difference is entirely whether somebody has to approve.

For documentation on a small project, deploying automatically is the right call.
For something with real users, or where a bad release is expensive to undo, you usually want the approval.
Later in the course you will add one.

## Next steps

::::challenge{id="start-documenting" title="Keep documenting"}

The API reference is generated, but a reference is not a guide.
Someone arriving at your project still needs to know what it is for and how to start.

Add an `install` and a `quickstart` page to the `toctree`, with a worked example of loading the sample data, summarising it and writing a report.

Push, and watch the site update itself.
::::

1. \[optional\] read the [Sphinx documentation](https://www.sphinx-doc.org/en/master/).

1. \[optional\] read about [publishing with GitHub Pages](https://docs.github.com/en/pages/getting-started-with-github-pages/configuring-a-publishing-source-for-your-github-pages-site#publishing-with-a-custom-github-actions-workflow).
