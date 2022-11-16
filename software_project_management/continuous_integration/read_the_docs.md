## 16:45 Integrating third party tools: Read the Docs

[Read the Docs](https://readthedocs.org/) is a free place to host documentation for open source software projects.
In this section you will use a tool called [Sphinx](https://www.sphinx-doc.org/en/master/) to generate documentation for your repository, and host that documentation automatically on Read the Docs.

<iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/3kDdPtg3pwU" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

1. Create a directory called `docs` and run the `sphinx-quickstart`.
1. Edit `conf.py` to remove the empty `_static` directory from the configuration. The final line should end up as:
    ```
    html_static_path = []
    ```
1. Commit and push those files to GitHub.
1. Go to [https://readthedocs.org/](https://readthedocs.org/) and import your repository.
1. Check that it builds correctly. Find a status badge to add to your `README.md`.
1. Add a quickstart guide to your documentation. Push the changes, and check that they go live.
2. Read the contents of the `.readthedocs.yml` file in your repository. What is it doing?
3. \[optional\] Read more about [Sphinx](https://www.sphinx-doc.org/en/master/) and [Read the Docs](https://readthedocs.org/).