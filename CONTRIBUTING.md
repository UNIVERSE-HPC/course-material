# Contributing guidelines

To contribute new material, or update existing material please:

  1. Create an issue on this repo with a description of the proposed change
  2. Fork the repo and create a new branch. Add commits to your branch in your own fork with the changes. Please provide the issue number in each commit message, e.g. a commit message for issue number 5 might be "#5 added version control material"
  3. When you are ready, open a pull request to merge your new commits to the `main` branch of this repo.

## Rendering website

It is useful to see how your change is rendered into a webpage using the [Gutenberg platform](https://github.com/OxfordRSE/gutenberg). In development, you will need to follow the [Gutenburg development guide](https://blog.oxrse.uk/gutenberg/development/) to install the platform locally. You will then be able to update material in the "MATERIAL_DIR" directory which will be built into a webapp and hosted on your local machine.

## Structure

The file structure in this repo defines the structure of the generated material, there are three levels of subdirectories, each with their own `index.md` file, which contains metadata:

```text
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

## Metadata

Each folder has an `index.md` with metadata for that theme/course/section.

### Material metadata

The top level `index.md` has a yaml block containing the keys:

- `id`, a string with a unique id for this material
- `name`, a string with the material title
- `themes`, an array of folder names of the themes within this material

The theme names correspond to subfolders in this repo. The rest of the content of this file is markdown formatted content with a top-level description of the material.

### Theme metadata

The theme level `index.md` has a yaml block containing the keys:

- `id`, a string with a unique id (unique within this material) for this theme
- `name`, a string with the theme title
- `courses`, an array of folder names of the courses within this material

The course names correspond to subfolders in this theme folder. The rest of the content of this file is markdown formatted content with a top-level description of the theme.

### Course metadata

The course level `index.md` has a yaml block containing the keys:

- `id`, a string with a unique id (unique within this theme) for this theme
- `name`, a string with the theme title
- `files`, an array of filenames of the sections within this material

The file names correspond to markdown files in this course folder. The rest of the content of this file is markdown formatted content with a top-level description of the course.

### Section metadata

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

## Markdown

The material itself is written using [GitHub-flavored
Markdown](https://docs.github.com/en/get-started/writing-on-github). A few special directives have been defined to allow us to include problems/challenges and solutions. Each directive specifies a block that starts and ends with three or more colons, e.g.

```pandoc
:::directive_name

Any markdown content can go here

:::
```

The start and end indicators must have the same number of colons each.

### Challenge directive

A problem or challenge can be defined using the following syntax:

```pandoc
::::challenge{id=dot_product title="Example challenge"}

This is an example challenge

::::
```

The id must be unique within this particular section, and the title is any string

### Solution directive

The solution directive produces a section that is initially hidden, but which a
user can click to display. It can be written using the following syntax:

```pandoc
:::solution

The answer is 42.

:::
```

Note that solutions can be nested within challenges by matching the number of colons:

```pandoc
::::challenge{id=big_question title="Hitchhikers question"}

What is the answer to life universe and everything?

:::solution
The answer is 42.
:::

::::
```

### Callout directive

The callout directive produces a highlighted and bordered block of markdown content. It
can be written using the following syntax:

```pandoc
:::callout

More information on Douglas Adams' "The Hitchhikers Guide to the Galaxy" book series can 
be found on [the series Wikipedia entry](https://en.wikipedia.org/wiki/The_Hitchhiker%27s_Guide_to_the_Galaxy)

:::
```

Different variants/flavours of callout are available by using the syntax

```pandoc
:::callout{variant="variant"}
Text
:::
```

Variants available are "danger", "warning", "tip", "discussion", "note" and "keypoints".

![image](https://private-user-images.githubusercontent.com/60351846/301895586-343ade2a-0c4e-4f20-9559-8e3a4986a523.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MTQxMjkxMjcsIm5iZiI6MTcxNDEyODgyNywicGF0aCI6Ii82MDM1MTg0Ni8zMDE4OTU1ODYtMzQzYWRlMmEtMGM0ZS00ZjIwLTk1NTktOGUzYTQ5ODZhNTIzLnBuZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNDA0MjYlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjQwNDI2VDEwNTM0N1omWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPThjYjRmMGVkYTJmNWRmNTZkZWQwYWUzMzI0MTg0N2I0M2Q4ZDBkYWJlMmMwOWU3MWNmYTkzMTdiYzFmZGRhZjAmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0JmFjdG9yX2lkPTAma2V5X2lkPTAmcmVwb19pZD0wIn0.I4hgqTxWYcVN9TXzcFjDcyxotBrq_-o0TFWuVlJXW14)

#### Danger

Used to inform students to be aware that there is a danger that they could break their development environment or lose data if an action is not performed correctly.

#### Warning

Used to warn students that they should be careful when performing a task in case there is a risk of a breaking change or issue arising that is less serious than something in the "danger" category or precautions to take to ensure good practice.

#### Tip

Used to inform students of a useful tip that may help them complete a task or help them in a wider context.

#### Discussion

Used to introduce a discussion topics as part of a training session or as a thinking point if completing the training individually, these help students better understand a concept.

#### Note

Used to highlight additional information that the student may wat to bear in mind when completing a task or thinking about a topic.

#### Key Points

Used to summarise the most essential or critical information in a topic. These can be takeaways or highlights that students should focus on.
