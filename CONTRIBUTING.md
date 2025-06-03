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

The top level directories are _themes_, each containing a number of _courses_
(you can think of each course as being 1/2 or 1 day worth of material). Each
course is further subdivided into _sections_, which are markdown files (with
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

The material itself is written using [GitHub-flavored Markdown](https://docs.github.com/en/get-started/writing-on-github). A few special directives have been defined to allow us to include problems/challenges and solutions and callouts. Each directive specifies a block that starts and ends with three or more colons, e.g.

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

The solution directive produces a section that is initially hidden, but which a user can click to display. It can be written using the following syntax:

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

### Internal Links

You can obviously include links in markdown, using the standard markdown syntax:

```markdown
[link text](https://example.com)
```

However, since the full url varies depending on the deployment we have a dynamic system to generate internal urls, that is URLs that point to other sections of this markdown.

This is done automatically by using the following syntax:

```markdown
[internal link]([theme.id]/[course.id]/[section.id])
[with leading slash](/[theme.id]/[course.id]/[section.id])
[link where you forgot to remove the .md](/[theme.id]/[course.id]/[section.id].md)
```

Also, you can use internal links in text as well, for ecxample:

```markdown
{{ base_url }}/[theme.id]/[course.id]/[section.id]
```

All of these examples will prepend the base URL and the material directory of the deployment to the link, so that it will work in any environment.

For example, if the base URL is `https://example.com`, and the repo being used is `HPCu`, then all of the example lins will become `https://example.com/material/HPCu/[theme.id]/[course.id]/[section.id]`.

### Callout directive

The callout directive produces a highlighted and bordered block of markdown content. It
can be written using the following syntax:

```pandoc
:::callout

More information on Douglas Adams' "The Hitchhikers Guide to the Galaxy" book series can be found on [the series Wikipedia entry](https://en.wikipedia.org/wiki/The_Hitchhiker%27s_Guide_to_the_Galaxy)

:::
```

Different variants/flavours of callout are available by using the syntax

```pandoc
:::callout{variant="variant"}
Text
:::
```

Variants available are "danger", "warning", "tip", "discussion", "note" and "keypoints".

![image of callout variants](https://imgur.com/r2RcIXJ)

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

## Linting

There are 3 linting actions that are run on the material in this repo, these are:

### Front matter

To check the front matters yaml is properly formatted, we use a custom `front-matter-lint` action.
As it is not a command line tool, it can only be run locally with [act](https://github.com/nektos/act), a tool to run GitHub actions locally.

### Markdownlint

To ensure that the material is consistent and well formatted, we use `markdownlint` to check for common issues.
You can also run the markdown linter manually using the following command:

```bash
markdownlint '**/*.md' --ignore '*/*/slides/*' --ignore README.md
```

### Python code

Python code is checked using a custom action, this works by stitching all python blocks together into a single `.py` file, and then running a python linter on it.
There is no command line tool for this, so it can only be run locally with `act`.

There are no corresponding tools for other languages, e.g. C++, due to difficulties in stitching together a non-interpreted language.

## Link-checking

To ensure that all links in the material are valid, we use a custom `link-check` action.
This requires the fully built next.js app, so it is ran on the gutenberg platform rather than on this repo.
To run the link checker manually first clone gutenberg then you can checkout your branch and run act locally with the following commands or via an equivalent action in your text editor:

```bash
yarn pullmat
cd .material/HPCu
git checkout <your-branch>
cd ../..
act -j check-links
```

This will run the link checker on your branch, and report any broken links in the material.

Alternatively, you can run the link checker using npm by doing the previous up to the act command, then running the following command:

```bash
yarn build
yarn start &
npm install -g broken-link-checker
npx broken-link-checker http://localhost:3000/ -rof \
            --exclude "github.com" \
            --exclude "intel.com" \
            --exclude "visualstudio.com" \
            --exclude "pixabay.com" \
            --exclude "doi.org" \
            --exclude "stackoverflow.com" \
            --exclude "stackexchange.com" \
            --exclude "mathworks.com" \
            --exclude "shinyapps.io" \
            --exclude "essential_maths/essential_maths/"
```

While it is not ideal that links are not checked in this repos CI, it is the best solution we have and mistakes should be caught in the gutenberg platform CI/CD before changes to the course material are deployed to production.

>[!WARNING]
>On the excluded sites: the sites excluded are those that will `403` when our robot tries to access them, with the exception of `github` which we hammer with so many requests (hundreds of links to gh) that we get `429` errors.
>Therefore, links to these sites will not be checked, and you should ensure that they are valid yourself!
>Lastly, if you use a relative url to point to a attribution image in your front-matters then you will need to add an exclusion for a duplicate of the course name, e.g. `essential_maths/essential_maths/` for the essential maths course.
>This is due to the way that relative urls are handled on the `/diagram` page, don't worry the link will still be checked on the `/[course]` page.
