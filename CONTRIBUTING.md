# Contributing 

To contribute new material, or update existing material please:
  1. Create an issue on this repo with a description of the proposed change
  2. Fork the repo, then add commits to your fork with the changes. Please provide the issue number in each commit message, e.g. a commit message for issue number 5 might be "#5 added version control material"
  3. When you are ready, open a PR to merge your new commits to this repo

# Rendering website

It is useful to see how your change is rendered into a webpage. You can render
your forked repository by using this [deploy
action](https://github.com/OxfordRSE/gutenberg/actions/workflows/deploy.yml).
This is part of a web app that takes material in the format described below, and
renders it as HTML. 

To render your material, simply click the "Run workflow" button on the RHS pane
of the [deploy
action](https://github.com/OxfordRSE/gutenberg/actions/workflows/deploy.yml). If
this button does not appear on your screen then you don't have the neccessary
permissions to run the workflow. In this case first fork the repository into
your personal github account, then click on the "Run workflow" button within
your fork.

Clicking this button will open a small dialog where you can enter the name of
your fork in the textfield labeled "Github path of material repository". Then
click the "Run workflow" button, and wait until the action has completed. Once
the "deploy" job has completed, navigate to the [deployed
site](https://oxfordrse.github.io/gutenberg/) and here you will hopefully see
your updated material.

# Structure

The file structure in this repo defines the structure of the generated material, there are three levels of subdirectories, each with their own `index.md` file, which contains metadata:

```
- index.md
- [theme.id]
  - index.md
  - [course.id]
    - index.md
    - [section.id.md]
```

The top level directories are *themes*, each containing a number of *courses*
(you can think of each course as being 1/2 or 1 day worth of material). Each
course is further subdivided into *sections*, which are markdown files (with
extension `.md`) with the section content.

# Metadata

Each folder has an `index.md` with metadata for that theme/course/section.

## Material metadata

The top level `index.md` has a yaml block containing the keys:
  - `id`, a string with a unique id for this material
  - `name`, a string with the material title
  - `themes`, an array of folder names of the themes within this material

The theme names correspond to subfolders in this repo. The rest of the content of this file is markdown formatted content with a top-level description of the material.

## Theme metadata

The theme level `index.md` has a yaml block containing the keys:

  - `id`, a string with a unique id (unique within this material) for this theme
  - `name`, a string with the theme title
  - `courses`, an array of folder names of the courses within this material

The course names correspond to subfolders in this theme folder. The rest of the content of this file is markdown formatted content with a top-level description of the theme.

## Course metadata

The course level `index.md` has a yaml block containing the keys:

  - `id`, a string with a unique id (unique within this theme) for this theme
  - `name`, a string with the theme title
  - `files`, an array of filenames of the sections within this material

The file names correspond to markdown files in this course folder. The rest of the content of this file is markdown formatted content with a top-level description of the course.

## Section metadata

Each section markdown file has a yaml block containing the keys. Note that the `id` of each section is implicitly defined from the filename, so a section filename `array.md` would have an id `array`.

  - `name`, a string with the section title
  - `dependsOn`, an array of identifiers indicating the pre-requisite sections/courses/themes for this section

Each entry in `dependsOn` indicates a course dependency using
`<theme.id>.<course.id>` or a section dependency using
`<theme.id>.<course.id><section.id>`. For example, a section titled "Arrays"
that depended on the `functions_python` section in the `procedural` course in
the `software_architecture_and_design` theme, might have the following yaml
block:

```yaml
name: Arrays
dependsOn: [
    software_architecture_and_design.procedural.functions_python,
]
```

# Markdown

The material itself is written using [GitHub-flavored
Markdown](https://docs.github.com/en/get-started/writing-on-github). There are a
few special directives defined to describe problems and solutions, each
directive specifies a block that starts and ends with three or more colons, e.g.

```pandoc
:::directive_name

Any markdown content can go here

:::
```

The start and end indicators must have the same number of colons each.

## Challenge directive

A problem or challenge can be defined using the following syntax:

```pandoc
::::challenge{id=dot_product, title="Example challenge"}

This is an example challenge

::::
```

The id must be unique within this particular section, and the title is any string

## Solution directive

The solution directive produces a section that is initially hidden, but which a
user can click to display. It can be written using the following syntax:


```pandoc
:::solution

The answer is 42.

:::
```

Note that solutions can be nested within challenges by matching the number of colons:


```pandoc
::::challenge{id=big_question, title="Hitchhikers question"}

What is the answer to life universe and everything?

:::solution
The answer is 42.
:::

::::
```

## Callout directive

The callout directive produces a highlighed and bordered block of markdown content. It 
can be written using the following syntax:


```pandoc
:::callout

More information on Douglas Adams' "The Hitchhikers Guide to the Galaxy" book series can 
be found on wikipedia 
[here](https://en.wikipedia.org/wiki/The_Hitchhiker%27s_Guide_to_the_Galaxy)

:::
```


