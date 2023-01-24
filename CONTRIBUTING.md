
# Structure

The file structure in this repo defines the structure of the generated material, there are three levels of subdirectories, each with their own `index.md` file, which contains metadata:

```
- index.md
- [theme]
  - index.md
  - [course]
    - index.md
    - [section_files.md]
```

The top level directories are *themes*, each containing a number of *courses* (you can think of each course as being 1/2 or a days worth of material). Each course is further subdivided into *sections*, which are markdown files (with extension `.md`) with the section content.

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

Each entry in `dependsOn` indicates a theme dependency using `<theme.id>`, a
course dependency using `<theme.id>.<course.id>` or a section dependency using
`<theme.id>.<course.id><section.id>`. For example, a section called "Arrays"
might have the following yaml block:

```yaml
---
name: Arrays
dependsOn: [
    software_architecture_and_design.procedural.functions_python,
]
---
```

# Markdown

The material itself is written using [GitHub-flavored Markdown](https://docs.github.com/en/get-started/writing-on-github). There are a few special directives defined to describe problems and solutions

## Challenge directive

A problem or challenge can be defined using the following syntax:

```md
::::challenge{id=dot_product, title="Example challenge" }

This is an example challenge, with some code

```cpp
const int index = 1;
```

::::
```

The id must be unique within this particular section, and the title is any string

## Solution directive

The solution directive produces a section that is initially hidden, but which a user can click to display. It can be written using the following syntax:


```md
:::solution

The answer is 42.

:::
```

Note that solutions can be nested within challenges by matching the number of colons:


```md
::::challenge{id=big_question, title="Hitchhikers question" }

What is the answer to life universe and everything?

:::solution
The answer is 42.
:::

::::
```




